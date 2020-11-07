/*
 * All operations are executed in the order of:
 * (1) transmit request to primary server, if possible
 * (2) check rwx validility, if possible
 * (3) execute the operation
 */

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

#include "fuse_rpc.h"

#define MAX_PATH_LEN 200

// Used to check operation validility.
#define R_REQ 0
#define W_REQ 1
#define X_REQ 2

// Master server will transmit to secondary first, if possible. Execution 
// status in return value is for primary server.
#define TRANSMIT_TO_SECONDATY(ARG, RET, FUNC)                                          \
	do {                                                                                 \
		int is_master = argp->server_info.is_master;                                       \
		int is_degraded = argp->server_info.is_degraded;                                   \
		if (is_master && !is_degraded) {                                                   \
			fprintf(stderr, "is master, and not degrades, transmit to secondary\n");         \
			int secondary_degraded = 0;                                                      \
			CLIENT *clnt = connect_server();                                                 \
			if (clnt == NULL) {                                                              \
				fprintf(stderr, "is master, connect secondary server error\n");                \
				secondary_degraded = 1;                                                        \
			}                                                                                \
			if (!secondary_degraded) {                                                       \
				ARG slave_arg = *argp;                                                         \
				slave_arg.server_info.is_master = 0;                                           \
				RET slave_ret;                                                                 \
				rpc_ret_t retval = FUNC(&slave_arg, &slave_ret, clnt);                         \
				if (retval != RPC_SUCCESS) {                                                   \
					fprintf(stderr, "Secondary server RPC return value error\n");                \
					clnt_perror (clnt, "call failed");                                           \
				} else {                                                                       \
					if (slave_ret.ret < 0) {                                                     \
						fprintf(stderr, "Secondary server error with errno %d\n", -slave_ret.ret); \
					}                                                                            \
				}                                                                              \
				clnt_destroy (clnt);                                                           \
			}                                                                                \
		}                                                                                  \
	} while (0)

// Using alias for RPC return type.
typedef enum clnt_stat rpc_ret_t;

// Connect to secondary server, use TCP by default.
// TODO: Hard code secondary server IP.
// Note: Need to update /etc/hosts.
static CLIENT *connect_server() {
	static char *host = "10.148.54.200";
	CLIENT *clnt = clnt_create (host, COMPUTE, COMPUTE_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
	}
	return clnt;
}

// Set owner and group after creating file and directory.
static int set_owner(char *fpath, char *ip) {
	fprintf(stderr, "Set owner for %s with owner %s\n", fpath, ip);
	int id = atoi(ip);
	if (chown(fpath, id, id) != 0) {
		fprintf(stderr, "Chown for %s for user %d error\n", fpath, id);
		return -1;
	}
	return 0;
}

// Check whether the operation is valid based on operation type and owner.
static int is_op_valid(char *fpath, char *ip, int requested) {
	struct stat statbuf;
	if (lstat(fpath, &statbuf) == -1) {
		fprintf(stderr, "Error when checking %s stat\n", fpath);
		return -1;
	}
	int id = atoi(ip);
	int uid = statbuf.st_uid;
	int mode = statbuf.st_mode;
	switch (requested) {
		case R_REQ:
			return uid == id ? ((mode & S_IRUSR) != 0) : ((mode & S_IROTH) != 0);
		case W_REQ:
			return uid == id ? ((mode & S_IWUSR) != 0) : ((mode & S_IWOTH) != 0);
		case X_REQ:
			return uid == id ? ((mode & S_IXUSR) != 0) : ((mode & S_IXOTH) != 0);
		default:
			assert(0);
	}
}

// Translate the absolute path from client side to /DFS/path on server side.
static void translate_abspath(char *buf, char *path) {
	static char *prefix = "/DFS";
	memset(buf, '\0', MAX_PATH_LEN);
	strncpy(buf, prefix, strlen(prefix));

	// Path from client is root '/'.
	if (strcmp(path, "/") == 0) {
		return;
	}

	// Otherwise, concatenate path.
	if (path[0] != '/') {
		buf[4] = '/';
	}
	strncat(buf, path, strlen(path));
}

bool_t
init_rootdir_6_svc(init_arg *argp, init_ret *result, struct svc_req *rqstp) {
	TRANSMIT_TO_SECONDATY(init_arg, init_ret, init_rootdir_6);

	// Make sure "/DFS" exists, create if not.
	char *root = "/DFS";
	if (access(root, F_OK) == -1) {
		if (mkdir(root, 0777) != 0) {
			fprintf(stderr, "mkdir %s failure\n", root);
			result->ret = -1;
			return TRUE;
		}
	}

	char *ip = argp->ip;
	char *rootdir = argp->rootdir;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, rootdir);
	fprintf(stderr, "Initialize rootdir with fpath %s.\n", fpath);
	result->ret = 0;
	return TRUE;
}

bool_t
bb_getattr_6_svc(getattr_arg *argp, getattr_ret *result, struct svc_req *rqstp)
{
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	fprintf(stderr, "Get attribute for path = %s\n", fpath);

	struct stat statbuf;
	if (lstat(fpath, &statbuf) == -1) {
		fprintf(stderr, "lstat %s error\n", fpath);
		result->ret = -errno;
		return TRUE;
	}

	result->st_dev = statbuf.st_dev;
	result->st_ino = statbuf.st_ino;
	result->st_mode = statbuf.st_mode;
	result->st_nlink = statbuf.st_nlink;
	result->st_uid = statbuf.st_uid;
	result->st_gid = statbuf.st_gid;
	result->st_rdev = statbuf.st_rdev;
	result->st_size = statbuf.st_size;
	result->st_blksize = statbuf.st_blksize;
	result->st_blocks = statbuf.st_blocks;
	result->st_atimensec = statbuf.st_atim.tv_nsec;
	result->st_mtimensec = statbuf.st_mtim.tv_nsec;
	result->st_ctimensec = statbuf.st_ctim.tv_nsec;
	result->ret = 0;
	return TRUE;
}

bool_t
bb_access_6_svc(access_arg *argp, access_ret *result, struct svc_req *rqstp)
{
	char *ip = argp->ip;
	char *path = argp->path;
  int mask = argp->mask;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	fprintf(stderr, "Access for path = %s, with mask = %d\n", fpath, mask);
    
	result->ret = access(fpath, mask);
	return TRUE;
}

bool_t
bb_mkdir_6_svc(mkdir_arg *argp, mkdir_ret *result, struct svc_req *rqstp)
{
	TRANSMIT_TO_SECONDATY(mkdir_arg, mkdir_ret, bb_mkdir_6);
	char *ip = argp->ip;
	char *path = argp->path;
	int mode = argp->mode;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	fprintf(stderr, "Make directory for %s with mode %d\n", fpath, mode);
	int syscall_ret = mkdir(fpath, mode);
	if (syscall_ret < 0) {
		fprintf(stderr, "make directory for %s with mode %d error\n", fpath, mode);
		result->ret = -1;
		return TRUE;
	}

	result->ret = 0;
	if (set_owner(fpath, ip) != 0) {
		result->ret = -1;
	}
	return TRUE;
}

bool_t
bb_rmdir_6_svc(rmdir_arg *argp, rmdir_ret *result, struct svc_req *rqstp)
{
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	fprintf(stderr, "Remove directory for %s\n", fpath);
	TRANSMIT_TO_SECONDATY(rmdir_arg, rmdir_ret, bb_rmdir_6);
	result->ret = rmdir(fpath);
	return TRUE;
}

bool_t
bb_opendir_6_svc(opendir_arg *argp, opendir_ret *result, struct svc_req *rqstp)
{
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	fprintf(stderr, "Open directory for %s\n", fpath);

	int is_valid = is_op_valid(fpath, ip, R_REQ);
	if (!is_valid) {
		fprintf(stderr, "Open directory is not valid by authentication\n");
		result->ret = -1;
		return TRUE;
	}

	DIR *dp = opendir(fpath);
	if (dp == NULL) {
		fprintf(stderr, "opendir %s error\n", fpath);
		result->ret = -errno;
		return TRUE;
	}

	result->fd = dirfd(dp);
	result->ret = 0;
	return TRUE;
}

bool_t
bb_readdir_6_svc(readdir_arg *argp, readdir_ret *result, struct svc_req *rqstp)
{
	int fd = argp->fd;
	fprintf(stderr, "Read directory for fd %d\n", fd);

	result->count = 0;
	flock(fd, LOCK_SH);
	DIR *dp = fdopendir(fd);
	if (dp == NULL) {
		fprintf(stderr, "opendir of fd %d error\n", fd);
		result->ret = -errno;
		return TRUE;
	}

	// Iterate through the dirent, and fill it into entry.
	int offset = 0;
	char buf[MAX_SIZE];
	memset(buf, '\0', MAX_SIZE);
	struct dirent *de = NULL;
	while ((de = readdir(dp)) != NULL) {
		char *entry_name = de->d_name;
		int len = strlen(entry_name);
		memcpy(buf + offset, entry_name, len);
		offset += len + 1;
		++result->count;
	}
	flock(fd, LOCK_UN);

	result->ret = 0;
	memcpy(result->entries, buf, MAX_SIZE);
	return TRUE;
}

bool_t
bb_releasedir_6_svc(releasedir_arg *argp, releasedir_ret *result, struct svc_req *rqstp)
{
	int fd = argp->fd;
	fprintf(stderr, "Close directory with fd = %d\n", fd);

	DIR* dir = fdopendir(fd);
	if (dir == NULL) {
		fprintf(stderr, "Open directory associated with fd %d error\n", fd);
		result->ret = -errno;
		return TRUE;
	}
	int close_ret = closedir(dir);
	if (close_ret == -1) {
		fprintf(stderr, "Close directory error\n");
		result->ret = -errno;
		return TRUE;
	}

	result->ret = 0;
	return TRUE;
}

bool_t
bb_rename_6_svc(rename_arg *argp, rename_ret *result, struct svc_req *rqstp)
{
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	char *newpath = argp->newpath;
	char fnewpath[MAX_PATH_LEN];
	translate_abspath(fnewpath, newpath);
	fprintf(stderr, "Rename file from %s to %s\n", fpath, fnewpath);
	TRANSMIT_TO_SECONDATY(rename_arg, rename_ret, bb_rename_6);
	result->ret = rename(fpath, fnewpath);
	return TRUE;
}

bool_t
bb_symlink_6_svc(symlink_arg *argp, symlink_ret *result, struct svc_req *rqstp)
{
	char *ip = argp->ip;
	char *path = argp->path;
	char *link = argp->link;
	char flink[MAX_PATH_LEN];
	translate_abspath(flink, link);
	fprintf(stderr, "Create symlink target = %s, original path = %s\n", flink, path);
	TRANSMIT_TO_SECONDATY(symlink_arg, symlink_ret, bb_symlink_6);

	int syscall_ret = symlink(path, flink);
	if (syscall_ret == -1) {
		fprintf(stderr, "symlink error for creating link %s to file %s\n", flink, path);
		result->ret = -errno;
		return TRUE;
	}
	result->ret = 0;
	return TRUE;
}

bool_t
bb_link_6_svc(link_arg *argp, link_ret *result, struct svc_req *rqstp)
{
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	char *newpath = argp->newpath;
	char fnewpath[MAX_PATH_LEN];
	translate_abspath(fnewpath, newpath);
	fprintf(stderr, "Create hard link source = %s, new path = %s\n", fpath, fnewpath);
	TRANSMIT_TO_SECONDATY(link_arg, link_ret, bb_link_6);

	int syscall_ret = link(fpath, fnewpath);
	if (syscall_ret == -1) {
		fprintf(stderr, "hard link creation error for creating link %s to file %s\n", fnewpath, fpath);
		result->ret = -errno;
		return TRUE;
	}
	result->ret = 0;
	return TRUE;
}

bool_t
bb_readlink_6_svc(readlink_arg *argp, readlink_ret *result, struct svc_req *rqstp)
{
	unsigned int size = argp->size;
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	fprintf(stderr, "Read link %s with size %u\n", fpath, size);

	char buffer[MAX_SIZE];
	memset(buffer, '\0', MAX_SIZE);
	ssize_t read_len = readlink(fpath, buffer, size);

	if (read_len == -1) {
		fprintf(stderr, "read link %s, with size = %u error with errno %d\n", fpath, size, errno);
		result->ret = -errno;
		return TRUE;
	}

	result->len = read_len;
	result->ret = 0;
	memcpy(result->buffer, buffer, MAX_SIZE);
	return TRUE;
}

bool_t
bb_mknod_6_svc(mknod_arg *argp, mknod_ret *result, struct svc_req *rqstp)
{
	TRANSMIT_TO_SECONDATY(mknod_arg, mknod_ret, bb_mknod_6);
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	int mode = argp->mode;
	int dev = argp->dev;
	fprintf(stderr, "Mknod for file %s with mode %d and dev %d\n", fpath, mode, dev);
	
	int func_ret = -1;

	// Create file, close it if succeeds.
	if (S_ISREG(mode)) {
		func_ret = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (func_ret < 0) {
			fprintf(stderr, "mknod for regular file %s with mode %d with errno %d\n", fpath, mode, -errno);
			result->ret = -1;
			return TRUE;
		} else {
			close(func_ret);
		}
	}

	// Create fifo.
	else if (S_ISFIFO(mode)) {
		func_ret = mkfifo(fpath, mode);
		if (func_ret < 0) {
			fprintf(stderr, "mknod for fifo file %s with mode %d with errno %d\n", fpath, mode, -errno);
			result->ret = -1;
			return TRUE;
		}
	}

	// Create device files.
	else {
		func_ret = mknod(fpath, mode, dev);
		if (func_ret < 0) {
			fprintf(stderr, "mknod for device file %s with mode %d device id %d with errno %d\n", fpath, mode, dev, -errno);
			result->ret = -1;
			return TRUE;
		}
	}

	result->ret = 0;
	if (set_owner(fpath, ip) < 0) {
		result->ret = -1;
	}
	return TRUE;
}

bool_t
bb_truncate_6_svc(truncate_arg *argp, truncate_ret *result, struct svc_req *rqstp)
{
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	int newsize = argp->newsize;
	fprintf(stderr, "Truncate file %s to new size %d\n", fpath, newsize);
	TRANSMIT_TO_SECONDATY(truncate_arg, truncate_ret, bb_truncate_6);

	int syscall_ret = truncate(fpath, newsize);
	if (syscall_ret < 0) {
		fprintf(stderr, "truncate file %s to new size %d fail with errno %d\n", fpath, newsize, errno);
		result->ret = -errno;
		return TRUE;
	}
	
	result->ret = 0;
	return TRUE;
}

bool_t
bb_unlink_6_svc(unlink_arg *argp, unlink_ret *result, struct svc_req *rqstp)
{
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	fprintf(stderr, "Unlink file for %s\n", fpath);
	TRANSMIT_TO_SECONDATY(unlink_arg, unlink_ret, bb_unlink_6);

	result->ret = unlink(fpath);
	return TRUE;
}

bool_t
bb_utime_6_svc(utime_arg *argp, utime_ret *result, struct svc_req *rqstp)
{
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	long actime = argp->actime;
	long modtime = argp->modtime;
	fprintf(stderr, "Utime %s with access time %ld and modification time %ld\n", fpath, actime, modtime);
	TRANSMIT_TO_SECONDATY(utime_arg, utime_ret, bb_utime_6);

	struct utimbuf ubuf;
	ubuf.actime = actime;
	ubuf.modtime = modtime;
	int syscall_ret = utime(fpath, &ubuf);
	if (syscall_ret < 0) {
		fprintf(stderr, "utime %s with access time %ld and modification time %ld error with errno %d\n", fpath, actime, modtime, errno);
		result->ret = -errno;
		return TRUE;
	}
	
	result->ret = 0;
	return TRUE;
}

bool_t
bb_chmod_6_svc(chmod_arg *argp, chmod_ret *result, struct svc_req *rqstp)
{
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	int mode = argp->mode;
	fprintf(stderr, "Change file %s to mode %d\n", fpath, mode);
	TRANSMIT_TO_SECONDATY(chmod_arg, chmod_ret, bb_chmod_6);

	int syscall_ret = chmod(fpath, mode);
	if (syscall_ret < 0) {
		fprintf(stderr, "change file to %s mode %d error with errno %d\n", fpath, mode, errno);
		result->ret = -errno;
		return TRUE;
	}

	result->ret = 0;
	return TRUE;
}

bool_t
bb_chown_6_svc(chown_arg *argp, chown_ret *result, struct svc_req *rqstp)
{
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	unsigned int uid = argp->uid;
	unsigned int gid = argp->gid;
	fprintf(stderr, "Change file %s to uid %u, gid %u\n", fpath, uid, gid);
	TRANSMIT_TO_SECONDATY(chown_arg, chown_ret, bb_chown_6);

	int syscall_ret = chown(fpath, uid, gid);
	if (syscall_ret < 0) {
		fprintf(stderr, "change file to %s to uid %u, gid %u with errno %d\n", fpath, uid, gid, errno);
		result->ret = -errno;
		return TRUE;
	}

	result->ret = 0;
	return TRUE;
}

bool_t
bb_open_6_svc(open_arg *argp, open_ret *result, struct svc_req *rqstp)
{
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	int flags = argp->flags;
	fprintf(stderr, "Open file for %s with flag %d\n", fpath, flags);

	result->fd = open(fpath, flags);
	if (result->fd == -1) {
		fprintf(stderr, "open file %s with flag %d error with errno %d\n", fpath, flags, errno);
		result->ret = -errno;
		return TRUE;
	}

	result->ret = 0;
	return TRUE;
}

bool_t
bb_release_6_svc(release_arg *argp, release_ret *result, struct svc_req *rqstp)
{
	int fd = argp->fd;
	fprintf(stderr, "Close file with fd = %d\n", fd);

	int close_ret = close(fd);
	if (close_ret == -1) {
			fprintf(stderr, "Close file error\n");
			result->ret = -errno;
			return TRUE;
	}

	result->ret = 0;
	return TRUE;
}

bool_t
bb_read_6_svc(read_arg *argp, read_ret *result, struct svc_req *rqstp)
{
	int fd = argp->fd;
	char *ip = argp->ip;
	char *path = argp->path;
	char fpath[MAX_PATH_LEN];
	translate_abspath(fpath, path);
	unsigned int size = argp->size;
	unsigned int offset = argp->offset;
	fprintf(stderr, "Read file with fd = %d, with size = %u, offset = %u\n", fd, size, offset);

	int is_valid = is_op_valid(fpath, ip, R_REQ);
	if (!is_valid) {
		fprintf(stderr, "Read file %s error for user %s by authentication\n", fpath, ip);
		result->ret = -1;
		return TRUE;
	}

	char buf[MAX_SIZE];
	memset(buf, '\0', MAX_SIZE);
	flock(fd, LOCK_SH);
	ssize_t read_len = pread(fd, buf, size, offset);
	flock(fd, LOCK_UN);

	if (read_len == -1) {
			fprintf(stderr, "read file with fd = %d, with size = %u, offset = %u error with errno %d\n", fd, size, offset, errno);
			result->ret = -errno;
			return TRUE;
	}

	// read_len could be 0, which means server has reached EOF.
	result->len = read_len;
	result->ret = 0;
	memcpy(result->buffer, buf, MAX_SIZE);
	return TRUE;
}

bool_t
bb_write_6_svc(write_arg *argp, write_ret *result, struct svc_req *rqstp)
{
	int fd = argp->fd;
	unsigned int size = argp->size;
	unsigned int offset = argp->offset;
	char *buf = argp->buffer;
	fprintf(stderr, "Write file with fd = %d, with size = %u, offset = %u\n", fd, size, offset);
	TRANSMIT_TO_SECONDATY(write_arg, write_ret, bb_write_6);

	// For secondary server, open file to get its fd.
	int is_master = argp->server_info.is_master; 
	int is_degraded = argp->server_info.is_degraded; 
	if (!is_master && !is_degraded) {
		char *ip = argp->ip;
		char *path = argp->path;
		char fpath[MAX_PATH_LEN];
		translate_abspath(fpath, path);
		fd = open(fpath, 33793); // TODO: default flag
		if (fd < 0) {
			fprintf(stderr, "Open file %s in the secondary server error\n", fpath);
		}
		fprintf(stderr, "Open file %s with fd %d in the secondary server\n", fpath, fd);
	}

	flock(fd, LOCK_EX);
	ssize_t write_ret = pwrite(fd, buf, size, offset);
	flock(fd, LOCK_UN);
	if (write_ret == -1) {
			fprintf(stderr, "write file with fd = %d, with size = %u, offset = %u error\n", fd, size, offset);
			result->ret = -errno;
			return TRUE;
	}

	// Close fd for secondary server.
	if (!is_master && !is_degraded) {
		fprintf(stderr, "Close file with fd %d\n", fd);
		close(fd);
	}

	result->len = write_ret;
	result->ret = 0;
	return TRUE;
}

int
compute_6_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free (xdr_result, result);

	/*
	 * Insert additional freeing code here, if needed
	 */

	return 1;
}