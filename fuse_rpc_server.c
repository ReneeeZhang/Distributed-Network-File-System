/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
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
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

#include "fuse_rpc.h"

bool_t
bb_getattr_6_svc(getattr_arg *argp, getattr_ret *result, struct svc_req *rqstp)
{
	char *path = argp->path;
	fprintf(stderr, "Get attribute for path = %s\n", path);

	struct stat statbuf;
	if (lstat(path, &statbuf) == -1) {
		fprintf(stderr, "lstat %s error\n", path);
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
	char *path = argp->path;
  int mask = argp->mask;
	fprintf(stderr, "Access for path = %s, with mask = %d\n", path, mask);
    
	result->ret = access(path, mask);
	return TRUE;
}

bool_t
bb_mkdir_6_svc(mkdir_arg *argp, mkdir_ret *result, struct svc_req *rqstp)
{
	char *path = argp->path;
	int mode = argp->mode;
	fprintf(stderr, "Make directory for %s with mode %d\n", path, mode);

	result->ret = mkdir(path, mode);
	return TRUE;
}

bool_t
bb_rmdir_6_svc(rmdir_arg *argp, rmdir_ret *result, struct svc_req *rqstp)
{
	char *path = argp->path;
	fprintf(stderr, "Remove directory for %s\n", path);

	result->ret = rmdir(path);
	return TRUE;
}

bool_t
bb_opendir_6_svc(opendir_arg *argp, opendir_ret *result, struct svc_req *rqstp)
{
	char *path = argp->path;
	fprintf(stderr, "Open directory for %s\n", path);

	DIR *dp = opendir(path);
	if (dp == NULL) {
		fprintf(stderr, "opendir %s error\n", path);
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
	char *path = argp->path;
	char *newpath = argp->newpath;
	fprintf(stderr, "Rename file from %s to %s\n", path, newpath);

	result->ret = rename(path, newpath);
	return TRUE;
}

bool_t
bb_symlink_6_svc(symlink_arg *argp, symlink_ret *result, struct svc_req *rqstp)
{
	char *path = argp->path;
	char *link = argp->link;
	fprintf(stderr, "Create symlink target = %s, original path = %s\n", link, path);

	int syscall_ret = symlink(path, link);
	if (syscall_ret == -1) {
		fprintf(stderr, "symlink error for creating link %s to file %s\n", link, path);
		result->ret = -errno;
		return TRUE;
	}
	result->ret = 0;
	return TRUE;
}

bool_t
bb_link_6_svc(link_arg *argp, link_ret *result, struct svc_req *rqstp)
{
	char *path = argp->path;
	char *newpath = argp->newpath;
	fprintf(stderr, "Create hard link source = %s, new path = %s\n", path, newpath);

	int syscall_ret = link(path, newpath);
	if (syscall_ret == -1) {
		fprintf(stderr, "hard link creation error for creating link %s to file %s\n", newpath, path);
		result->ret = -errno;
		return TRUE;
	}
	result->ret = 0;
	return TRUE;
}

bool_t
bb_readlink_6_svc(readlink_arg *argp, readlink_ret *result, struct svc_req *rqstp)
{
	char *path = argp->path;
	unsigned int size = argp->size;
	fprintf(stderr, "Read link %s with size %u\n", path, size);

	char buffer[MAX_SIZE];
	memset(buffer, '\0', MAX_SIZE);
	ssize_t read_len = readlink(path, buffer, size);

	if (read_len == -1) {
		fprintf(stderr, "read link %s, with size = %u error with errno %d\n", path, size, errno);
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
	char *path = argp->path;
	int mode = argp->mode;
	int dev = argp->dev;
	fprintf(stderr, "Mknod for file %s with mode %d and dev %d\n", path, mode, dev);

	int func_ret = -1;

	// Create file, close it if succeeds.
	if (S_ISREG(mode)) {
		func_ret = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (func_ret < 0) {
			fprintf(stderr, "mknod for regular file %s with mode %d with errno %d\n", path, mode, -errno);
			result->ret = -errno;
			return TRUE;
		} else {
			close(func_ret);
		}
	}

	// Create fifo.
	else if (S_ISFIFO(mode)) {
		func_ret = mkfifo(path, mode);
		if (func_ret < 0) {
			fprintf(stderr, "mknod for fifo file %s with mode %d with errno %d\n", path, mode, -errno);
			result->ret = -errno;
			return TRUE;
		}
	}

	// Create device files.
	else {
		func_ret = mknod(path, mode, dev);
		if (func_ret < 0) {
			fprintf(stderr, "mknod for device file %s with mode %d device id %d with errno %d\n", path, mode, dev, -errno);
			result->ret = -errno;
			return TRUE;
		}
	}

	result->ret = 0;
	return TRUE;
}

bool_t
bb_truncate_6_svc(truncate_arg *argp, truncate_ret *result, struct svc_req *rqstp)
{
	char *path = argp->path;
	int newsize = argp->newsize;
	fprintf(stderr, "Truncate file %s to new size %d\n", path, newsize);

	int syscall_ret = truncate(path, newsize);
	if (syscall_ret < 0) {
		fprintf(stderr, "truncate file %s to new size %d fail with errno %d\n", path, newsize, errno);
		result->ret = -errno;
		return TRUE;
	}
	
	result->ret = 0;
	return TRUE;
}

bool_t
bb_unlink_6_svc(unlink_arg *argp, unlink_ret *result, struct svc_req *rqstp)
{
	char *path = argp->path;
	fprintf(stderr, "Unlink file for %s\n", path);

	result->ret = unlink(path);
	return TRUE;
}

bool_t
bb_utime_6_svc(utime_arg *argp, utime_ret *result, struct svc_req *rqstp)
{
	char *path = argp->path;
	long actime = argp->actime;
	long modtime = argp->modtime;
	fprintf(stderr, "Utime %s with access time %ld and modification time %ld\n", path, actime, modtime);

	struct utimbuf ubuf;
	ubuf.actime = actime;
	ubuf.modtime = modtime;
	int syscall_ret = utime(path, &ubuf);
	if (syscall_ret < 0) {
		fprintf(stderr, "utime %s with access time %ld and modification time %ld error with errno %d\n", path, actime, modtime, errno);
		result->ret = -errno;
		return TRUE;
	}
	
	result->ret = 0;
	return TRUE;
}

bool_t
bb_chmod_6_svc(chmod_arg *argp, chmod_ret *result, struct svc_req *rqstp)
{
	char *path = argp->path;
	int mode = argp->mode;
	fprintf(stderr, "Change file %s to mode %d\n", path, mode);

	int syscall_ret = chmod(path, mode);
	if (syscall_ret < 0) {
		fprintf(stderr, "change file to %s mode %d error with errno %d\n", path, mode, errno);
		result->ret = -errno;
		return TRUE;
	}

	result->ret = 0;
	return TRUE;
}

bool_t
bb_chown_6_svc(chown_arg *argp, chown_ret *result, struct svc_req *rqstp)
{
	char *path = argp->path;
	unsigned int uid = argp->uid;
	unsigned int gid = argp->gid;
	fprintf(stderr, "Change file %s to uid %u, gid %u\n", path, uid, gid);

	int syscall_ret = chown(path, uid, gid);
	if (syscall_ret < 0) {
		fprintf(stderr, "change file to %s to uid %u, gid %u with errno %d\n", path, uid, gid, errno);
		result->ret = -errno;
		return TRUE;
	}

	result->ret = 0;
	return TRUE;
}

bool_t
bb_open_6_svc(open_arg *argp, open_ret *result, struct svc_req *rqstp)
{
	char *path = argp->path;
	int flags = argp->flags;
	fprintf(stderr, "Open file for %s with flag %d\n", path, flags);

	result->fd = open(path, flags);
	if (result->fd == -1) {
		fprintf(stderr, "open file %s with flag %d error with errno %d\n", path, flags, errno);
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
bb_read_6_svc(read_arg *argp, read_ret *result, struct svc_req *rqstp)
{
	int fd = argp->fd;
	unsigned int size = argp->size;
	unsigned int offset = argp->offset;
	fprintf(stderr, "Read file with fd = %d, with size = %u, offset = %u\n", fd, size, offset);

	char buf[MAX_SIZE];
	memset(buf, '\0', MAX_SIZE);
	ssize_t read_len = pread(fd, buf, size, offset);

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

	ssize_t write_ret = pwrite(fd, buf, size, offset);
	if (write_ret == -1) {
			fprintf(stderr, "write file with fd = %d, with size = %u, offset = %u error\n", fd, size, offset);
			result->ret = -errno;
			return TRUE;
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
