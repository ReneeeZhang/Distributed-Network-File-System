/*
  Big Brother File System
  Copyright (C) 2012 Joseph J. Pfeiffer, Jr., Ph.D. <pfeiffer@cs.nmsu.edu>

  This program can be distributed under the terms of the GNU GPLv3.
  See the file COPYING.

  This code is derived from function prototypes found /usr/include/fuse/fuse.h
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>
  His code is licensed under the LGPLv2.
  A copy of that code is included in the file fuse.h
  
  The point of this FUSE filesystem is to provide an introduction to
  FUSE.  It was my first FUSE filesystem as I got to know the
  software; hopefully, the comments in this code will help people who
  follow later to get a gentler introduction.

  This might be called a no-op filesystem:  it doesn't impose
  filesystem semantics on top of any other existing structure.  It
  simply reports the requests that come in, and passes them to an
  underlying filesystem.  The information is saved in a logfile named
  bbfs.log, in the directory from which you run bbfs.
*/
#include "params.h"

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#ifdef HAVE_SYS_XATTR_H
#include <sys/xattr.h>
#endif

#include "log.h"
#include "fuse_rpc.h"

// RPC-related variables.
char *host = NULL;

// Store the length of root directory, in order to transform to fullpath.
size_t rootdir_len = 0;

// Connect to host, use TCP by default.
static CLIENT *connect_server() {
    CLIENT *clnt = clnt_create (host, COMPUTE, COMPUTE_VERS, "tcp");
    if (clnt == NULL) {
        log_msg("Create RPC connection error\n");
        clnt_pcreateerror (host);
        exit (1);
    }
    return clnt;
}


//  All the paths I see are relative to the root of the mounted
//  filesystem.  In order to get to the underlying filesystem, I need to
//  have the mountpoint.  I'll save it away early on in main(), and then
//  whenever I need a path for something I'll call this to construct
//  it.
static void bb_fullpath(char fpath[PATH_MAX], const char *path)
{
    // For some functions, say symlink, argument path may not begin with '/',
    // so have to prepend purposefully.
    strcpy(fpath, BB_DATA->rootdir);
    if (path != NULL && strlen(path) > 0 && path[0] != '/') {
        fpath[rootdir_len] = '/';
    }
    strncat(fpath, path, PATH_MAX);

    log_msg("    bb_fullpath:  rootdir = \"%s\", path = \"%s\", fpath = \"%s\"\n",
	    BB_DATA->rootdir, path, fpath);
}

///////////////////////////////////////////////////////////
//
// Prototypes for all these functions, and the C-style comments,
// come from /usr/include/fuse.h
//
/** Get file attributes.
 *
 * Similar to stat().  The 'st_dev' and 'st_blksize' fields are
 * ignored.  The 'st_ino' field is ignored except if the 'use_ino'
 * mount option is given.
 */
int bb_getattr(const char *path, struct stat *statbuf)
{
    log_msg("\nbb_getattr(path=\"%s\")\n", path);

    // Get attribute via RPC.
    getattr_ret *ret = NULL;
    getattr_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    arg.path = fpath;
    
    CLIENT *clnt = connect_server();
    ret = bb_getattr_6(&arg, clnt);
    if (ret == (getattr_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);
    
    // lstat() error.
    if (ret->ret < 0) {
        log_msg("Get attribute error with errno %d\n", -ret->ret);
        return ret->ret;
    }

    // Assign RPC return value to stat buf.
    statbuf->st_dev = ret->st_dev;
    statbuf->st_ino = ret->st_ino;
    statbuf->st_mode = ret->st_mode;
    statbuf->st_nlink = ret->st_nlink;
    statbuf->st_uid = ret->st_uid;
    statbuf->st_gid = ret->st_gid;
    statbuf->st_rdev = ret->st_rdev;
    statbuf->st_size = ret->st_size;
    statbuf->st_blksize = ret->st_blksize;
    statbuf->st_blocks = ret->st_blocks;
    statbuf->st_atimensec = ret->st_atimensec;
    statbuf->st_mtimensec = ret->st_mtimensec;
    statbuf->st_ctimensec = ret->st_ctimensec;
    return 0;
}

/** Read the target of a symbolic link
 *
 * The buffer should be filled with a null terminated string.  The
 * buffer size argument includes the space for the terminating
 * null character.  If the linkname is too long to fit in the
 * buffer, it should be truncated.  The return value should be 0
 * for success.
 */
// Note the system readlink() will truncate and lose the terminating
// null.  So, the size passed to to the system readlink() must be one
// less than the size passed to bb_readlink()
// bb_readlink() code by Bernardo F Costa (thanks!)
int bb_readlink(const char *path, char *buf, size_t size)
{
    log_msg("\nbb_readlink(path=\"%s\", link=\"%s\", size=%d)\n",
	  path, link, size);

    // Get attribute via RPC.
    readlink_ret *ret = NULL;
    readlink_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    arg.path = fpath;
    arg.size = size - 1;

    CLIENT *clnt = connect_server();
    ret = bb_readlink_6(&arg, clnt);
    if (ret == (readlink_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);

    if (ret->ret == -1) {
        log_msg("readlink %s with size %zu error\n", fpath, size - 1);
        return ret->ret;
    }

    buf[ret->len] = '\0';
    return 0;
}

/** Create a file node
 *
 * There is no create() operation, mknod() will be called for
 * creation of all non-directory, non-symlink nodes.
 */
// shouldn't that comment be "if" there is no.... ?
int bb_mknod(const char *path, mode_t mode, dev_t dev)
{
    log_msg("\nbb_mknod(path=\"%s\", mode=0%3o, dev=%lld)\n",
	  path, mode, dev);

    // Get attribute via RPC.
    mknod_ret *ret = NULL;
    mknod_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    arg.path = fpath;
    arg.mode = mode;
    arg.dev = dev;
    
    CLIENT *clnt = connect_server();
    ret = bb_mknod_6(&arg, clnt);
    if (ret == (mknod_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);

    if (ret->ret < 0) {
        log_msg("mknod for %s with mode %s and dev id %d error\n", fpath, mode, dev);
    }
    return ret->ret;
}

/** Create a directory */
int bb_mkdir(const char *path, mode_t mode)
{
    log_msg("\nbb_mkdir(path=\"%s\", mode=0%3o)\n", path, mode);

    // Get attribute via RPC.
    mkdir_ret *ret = NULL;
    mkdir_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    arg.path = fpath;
    arg.mode = mode;
    
    CLIENT *clnt = connect_server();
    ret = bb_mkdir_6(&arg, clnt);
    if (ret == (mkdir_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);
    
    if (ret->ret < 0) {
        log_msg("mkdir error for %s\n", fpath);
    }
    return ret->ret;
}

/** Remove a file */
int bb_unlink(const char *path)
{
    log_msg("bb_unlink(path=\"%s\")\n", path);

    // Get attribute via RPC.
    unlink_ret *ret = NULL;
    unlink_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    arg.path = fpath;
    
    CLIENT *clnt = connect_server();
    ret = bb_unlink_6(&arg, clnt);
    if (ret == (unlink_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);

    if (ret->ret < 0) {
        log_msg("unlink error for %s with errno %d\n", fpath, -ret->ret);
    }
    return ret->ret;
}

/** Remove a directory */
int bb_rmdir(const char *path)
{
    log_msg("bb_rmdir(path=\"%s\")\n",
	    path);
    
    // Get attribute via RPC.
    rmdir_ret *ret = NULL;
    rmdir_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    arg.path = fpath;
    
    CLIENT *clnt = connect_server();
    ret = bb_rmdir_6(&arg, clnt);
    if (ret == (rmdir_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);

    if (ret->ret < 0) {
        log_msg("rmdir error for %s with errno %d\n", fpath, -ret->ret);
    }
    return ret->ret;
}

/** Create a symbolic link */
// The parameters here are a little bit confusing, but do correspond
// to the symlink() system call.  The 'path' is where the link points,
// while the 'link' is the link itself.  So we need to leave the path
// unaltered, but insert the link into the mounted directory.
int bb_symlink(const char *path, const char *link)
{
    log_msg("\nbb_symlink(path=\"%s\", link=\"%s\")\n",
	    path, link);

    // Get attribute via RPC.
    symlink_ret *ret = NULL;
    symlink_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    char flink[PATH_MAX];
    bb_fullpath(flink, link);
    arg.path = fpath;
    arg.link = flink;

    log_msg("path = %s, full path = %s\n", path, fpath);
    log_msg("link path = %s, full link path = %s\n", link, flink);
    
    CLIENT *clnt = connect_server();
    ret = bb_symlink_6(&arg, clnt);
    if (ret == (symlink_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);

    if (ret->ret < 0) {
        log_msg("symlink error to create %s for %s with errno %d\n", flink, fpath, -ret->ret);
    }
    return ret->ret;
}

/** Rename a file */
// both path and newpath are fs-relative
int bb_rename(const char *path, const char *newpath)
{
    log_msg("\nbb_rename(fpath=\"%s\", newpath=\"%s\")\n",
	    path, newpath);

    // Get attribute via RPC.
    rename_ret *ret = NULL;
    rename_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    char fnewpath[PATH_MAX];
    bb_fullpath(fnewpath, newpath);
    arg.path = fpath;
    arg.newpath = fnewpath;
    
    CLIENT *clnt = connect_server();
    ret = bb_rename_6(&arg, clnt);
    if (ret == (rename_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);

    if (ret->ret < 0) {
        log_msg("rename error from %s to %s with errno %d\n", fpath, fnewpath, -ret->ret);
    }
    return ret->ret;
}

/** Create a hard link to a file */
int bb_link(const char *path, const char *newpath)
{
    char fpath[PATH_MAX], fnewpath[PATH_MAX];
    
    log_msg("\nbb_link(path=\"%s\", newpath=\"%s\")\n",
	    path, newpath);
    bb_fullpath(fpath, path);
    bb_fullpath(fnewpath, newpath);

    return log_syscall("link", link(fpath, fnewpath), 0);
}

/** Change the permission bits of a file */
int bb_chmod(const char *path, mode_t mode)
{
    log_msg("\nbb_chmod(fpath=\"%s\", mode=0%03o)\n",
	    path, mode);

    // Get attribute via RPC.
    chmod_ret *ret = NULL;
    chmod_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    arg.path = fpath;
    arg.mode = mode;
    
    CLIENT *clnt = connect_server();
    ret = bb_chmod_6(&arg, clnt);
    if (ret == (chmod_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);
    
    if (ret->ret < 0) {
        log_msg("Chmod for file %s to mode %d error\n", path, mode);
    }
    return ret->ret;
}

/** Change the owner and group of a file */
int bb_chown(const char *path, uid_t uid, gid_t gid)
{   
    log_msg("\nbb_chown(path=\"%s\", uid=%d, gid=%d)\n",
	    path, uid, gid);
    
    // Get attribute via RPC.
    chown_ret *ret = NULL;
    chown_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    arg.path = fpath;
    arg.uid = uid;
    arg.gid = gid;
    
    CLIENT *clnt = connect_server();
    ret = bb_chown_6(&arg, clnt);
    if (ret == (chown_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);
    
    if (ret->ret < 0) {
        log_msg("Chown for file %s to uid %ld gid %ld error\n", path, uid, gid);
    }
    return ret->ret;
}

/** Change the size of a file */
int bb_truncate(const char *path, off_t newsize)
{
    log_msg("\nbb_truncate(path=\"%s\", newsize=%lld)\n",
	    path, newsize);
    
    // Get attribute via RPC.
    truncate_ret *ret = NULL;
    truncate_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    arg.path = fpath;
    arg.newsize = newsize;
    
    CLIENT *clnt = connect_server();
    ret = bb_truncate_6(&arg, clnt);
    if (ret == (truncate_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);

    if (ret->ret < 0) {
        log_msg("truncate with path %s and new size %d error\n", fpath, newsize);
    }
    return ret->ret;
}

/** Change the access and/or modification times of a file */
/* note -- I'll want to change this as soon as 2.6 is in debian testing */
int bb_utime(const char *path, struct utimbuf *ubuf)
{
    log_msg("\nbb_utime(path=\"%s\", ubuf=0x%08x)\n",
	    path, ubuf);

    // Get attribute via RPC.
    utime_ret *ret = NULL;
    utime_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    arg.path = fpath;
    arg.actime = ubuf->actime;
    arg.modtime = ubuf->modtime;
    
    CLIENT *clnt = connect_server();
    ret = bb_utime_6(&arg, clnt);
    if (ret == (utime_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);

    if (ret->ret < 0) {
        log_msg("utime with path %s and access time %ld and modification time %ld error\n", fpath, arg.actime, arg.modtime);
    }
    return ret->ret;
}

/** File open operation
 *
 * No creation, or truncation flags (O_CREAT, O_EXCL, O_TRUNC)
 * will be passed to open().  Open should check if the operation
 * is permitted for the given flags.  Optionally open may also
 * return an arbitrary filehandle in the fuse_file_info structure,
 * which will be passed to all file operations.
 *
 * Changed in version 2.2
 */
int bb_open(const char *path, struct fuse_file_info *fi)
{
    log_msg("\nbb_open(path\"%s\", fi=0x%08x)\n",
	    path, fi);

    // Get attribute via RPC.
    open_ret *ret = NULL;
    open_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    arg.path = fpath;
    arg.flags = fi->flags;
    
    CLIENT *clnt = connect_server();
    ret = bb_open_6(&arg, clnt);
    if (ret == (open_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);

    if (ret->ret < 0) {
        log_msg("Open file %s error with errno %d\n", fpath, -ret->ret);
        return -1;
    }

    fi->fh = ret->fd;
    return 0;
}

/** Read data from an open file
 *
 * Read should return exactly the number of bytes requested except
 * on EOF or error, otherwise the rest of the data will be
 * substituted with zeroes.  An exception to this is when the
 * 'direct_io' mount option is specified, in which case the return
 * value of the read system call will reflect the return value of
 * this operation.
 *
 * Changed in version 2.2
 */
// I don't fully understand the documentation above -- it doesn't
// match the documentation for the read() system call which says it
// can return with anything up to the amount of data requested. nor
// with the fusexmp code which returns the amount of data also
// returned by read.
int bb_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    log_msg("\nbb_read(path\"%s\", fi=0x%08x)\n",
	    path, fi);

    // Get attribute via RPC.
    CLIENT *clnt = connect_server();
    read_ret *ret = NULL;
    read_arg arg;
    arg.fd = fi->fh;
    arg.size = size;
    arg.offset = offset;
    
    // Keep RPC until requsted size is already read.
    int total_len = 0;
    while (total_len < size) {
        ret = bb_read_6(&arg, clnt);
        if (ret == (read_ret *) NULL) {
            log_msg("RPC return value error\n");
            clnt_perror (clnt, "call failed");
        }
        if (ret->ret < 0) {
            log_msg("Read called error with errno %d\n", -ret->ret);
            clnt_destroy (clnt);
            return -1;
        }

        // If server has reached EOF.
        int len = ret->len;
        if (len == 0) {
            log_msg("Read for fd %d has finished\n", arg.fd);
            break;
        }

        // Assign read result to user space.
        memmove(buf + total_len, ret->buffer, MAX_SIZE);
        total_len += len;
        arg.size -= len;
        arg.offset += len;
    }
    
    clnt_destroy (clnt);
    return total_len;
}

/** Write data to an open file
 *
 * Write should return exactly the number of bytes requested
 * except on error.  An exception to this is when the 'direct_io'
 * mount option is specified (see read operation).
 *
 * Changed in version 2.2
 */
// As  with read(), the documentation above is inconsistent with the
// documentation for the write() system call.
int bb_write(const char *path, const char *buf, size_t size, off_t offset,
	     struct fuse_file_info *fi)
{
    log_msg("\nbb_write(path=\"%s\", buf=0x%08x, size=%d, offset=%lld, fi=0x%08x)\n",
	    path, buf, size, offset, fi
	    );
    
    // Get attribute via RPC.
    CLIENT *clnt = connect_server();
    write_ret *ret = NULL;
    write_arg arg;
    arg.fd = fi->fh;
    arg.size = size;
    arg.offset = offset;
    memset(arg.buffer, '\0', MAX_SIZE);
    memcpy(arg.buffer, buf, MAX_SIZE);

    // Keep RPC until all bytes has been writen to file.
    int total_len = 0;
    while (1) {
        ret = bb_write_6(&arg, clnt);
        if (ret == (write_ret *) NULL) {
            log_msg("RPC return value error\n");
            clnt_perror (clnt, "call failed");
        }

        // RPC write issue error.
        if (ret->ret < 0) {
            log_msg("Read called error with errno %d\n", -ret->ret);
            clnt_destroy (clnt);
            return -1;
        }

        // Check whether all requested bytes have been written.
        int len = ret->len;
        total_len += len;
        if (total_len == size) {
            log_msg("Write for fd %d has finished\n", arg.fd);
            break;
        }

        // Copy left parts to sent buffer, and continue RPC.
        arg.size = size - len;
        arg.offset = offset + len;
        memset(arg.buffer, '\0', MAX_SIZE);
        memcpy(arg.buffer, buf + total_len, MAX_SIZE);
    }
    return total_len;
}

/** Get file system statistics
 *
 * The 'f_frsize', 'f_favail', 'f_fsid' and 'f_flag' fields are ignored
 *
 * Replaced 'struct statfs' parameter with 'struct statvfs' in
 * version 2.5
 */
int bb_statfs(const char *path, struct statvfs *statv)
{
    int retstat = 0;
    char fpath[PATH_MAX];

    log_msg("\nbb_statfs(path=\"%s\", statv=0x%08x)\n",
	    path, statv);
    bb_fullpath(fpath, path);
    
    // get stats for underlying filesystem
    retstat = log_syscall("statvfs", statvfs(fpath, statv), 0);
    
    log_statvfs(statv);
    
    return retstat;
}

/** Possibly flush cached data
 *
 * BIG NOTE: This is not equivalent to fsync().  It's not a
 * request to sync dirty data.
 *
 * Flush is called on each close() of a file descriptor.  So if a
 * filesystem wants to return write errors in close() and the file
 * has cached dirty data, this is a good place to write back data
 * and return any errors.  Since many applications ignore close()
 * errors this is not always useful.
 *
 * NOTE: The flush() method may be called more than once for each
 * open().  This happens if more than one file descriptor refers
 * to an opened file due to dup(), dup2() or fork() calls.  It is
 * not possible to determine if a flush is final, so each flush
 * should be treated equally.  Multiple write-flush sequences are
 * relatively rare, so this shouldn't be a problem.
 *
 * Filesystems shouldn't assume that flush will always be called
 * after some writes, or that if will be called at all.
 *
 * Changed in version 2.2
 */
// this is a no-op in BBFS.  It just logs the call and returns success
int bb_flush(const char *path, struct fuse_file_info *fi)
{
    log_msg("\nbb_flush(path=\"%s\", fi=0x%08x)\n", path, fi);
    // no need to get fpath on this one, since I work from fi->fh not the path
    log_fi(fi);
	
    return 0;
}

/** Release an open file
 *
 * Release is called when there are no more references to an open
 * file: all file descriptors are closed and all memory mappings
 * are unmapped.
 *
 * For every open() call there will be exactly one release() call
 * with the same flags and file descriptor.  It is possible to
 * have a file opened more than once, in which case only the last
 * release will mean, that no more reads/writes will happen on the
 * file.  The return value of release is ignored.
 *
 * Changed in version 2.2
 */
int bb_release(const char *path, struct fuse_file_info *fi)
{
    log_msg("\nbb_release(path=\"%s\", fi=0x%08x)\n",
	  path, fi);
    
    // Get attribute via RPC.
    release_ret *ret = NULL;
    release_arg arg;
    arg.fd = fi->fh;
    
    CLIENT *clnt = connect_server();
    ret = bb_release_6(&arg, clnt);
    if (ret == (release_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);

    if (ret->ret < 0) {
        log_msg("Close file error with errno %d\n", -ret->ret);
        return -1;
    }

    return 0;
}

/** Synchronize file contents
 *
 * If the datasync parameter is non-zero, then only the user data
 * should be flushed, not the meta data.
 *
 * Changed in version 2.2
 */
int bb_fsync(const char *path, int datasync, struct fuse_file_info *fi)
{
    log_msg("\nbb_fsync(path=\"%s\", datasync=%d, fi=0x%08x)\n",
	    path, datasync, fi);
    log_fi(fi);
    
    // some unix-like systems (notably freebsd) don't have a datasync call
#ifdef HAVE_FDATASYNC
    if (datasync)
	return log_syscall("fdatasync", fdatasync(fi->fh), 0);
    else
#endif	
	return log_syscall("fsync", fsync(fi->fh), 0);
}

/** Open directory
 *
 * This method should check if the open operation is permitted for
 * this  directory
 *
 * Introduced in version 2.3
 */
int bb_opendir(const char *path, struct fuse_file_info *fi)
{
    log_msg("\nbb_opendir(path=\"%s\", fi=0x%08x)\n",
	  path, fi);

    // Get attribute via RPC.
    opendir_ret *ret = NULL;
    opendir_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    arg.path = fpath;
    
    CLIENT *clnt = connect_server();
    ret = bb_opendir_6(&arg, clnt);
    if (ret == (opendir_ret *) NULL) {
        log_msg("Return value null, RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);

    // readdir() error.
    if (ret->ret != 0) {
        log_msg("Open directory error\n");
        return -1;
    }

    fi->fh = ret->fd;
    return 0;
}

/** Read directory
 *
 * This supersedes the old getdir() interface.  New applications
 * should use this.
 *
 * The filesystem may choose between two modes of operation:
 *
 * 1) The readdir implementation ignores the offset parameter, and
 * passes zero to the filler function's offset.  The filler
 * function will not return '1' (unless an error happens), so the
 * whole directory is read in a single readdir operation.  This
 * works just like the old getdir() method.
 *
 * 2) The readdir implementation keeps track of the offsets of the
 * directory entries.  It uses the offset parameter and always
 * passes non-zero offset to the filler function.  When the buffer
 * is full (or an error happens) the filler function will return
 * '1'.
 *
 * Introduced in version 2.3
 */

int bb_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset,
	       struct fuse_file_info *fi)
{
    log_msg("\nbb_readdir(path=\"%s\", buf=0x%08x, filler=0x%08x, offset=%lld, fi=0x%08x)\n",
	    path, buf, filler, offset, fi);

    // Get attribute via RPC.
    readdir_ret *ret = NULL;
    readdir_arg arg;
    arg.fd = fi->fh;
    
    CLIENT *clnt = connect_server();
    ret = bb_readdir_6(&arg, clnt);
    if (ret == (readdir_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);

    // readdir() error.
    if (ret->ret != 0) {
        log_msg("Read directory error\n");
        return -1;
    }

    int count = ret->count;
    char *entries = ret->entries;
    for (int ii = 0; ii < count; ++ii) {
        log_msg("Ready to fill in dirent %s\n", entries);
        if (filler(buf, entries, NULL, 0) != 0) {
            log_msg("filler error: buffer full\n");
            return -ENOMEM;
        }
        entries += strlen(entries) + 1;
    }
    return 0;
}

/** Release directory
 *
 * Introduced in version 2.3
 */
int bb_releasedir(const char *path, struct fuse_file_info *fi)
{
    log_msg("\nbb_releasedir(path=\"%s\", fi=0x%08x)\n",
	    path, fi);
    
    // Get attribute via RPC.
    releasedir_ret *ret = NULL;
    releasedir_arg arg;
    arg.fd = fi->fh;
    
    CLIENT *clnt = connect_server();
    ret = bb_releasedir_6(&arg, clnt);
    if (ret == (releasedir_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);

    // releasedir() error
    if (ret->ret < 0) {
        log_msg("Close directory error with errno %d\n", -ret->ret);
        return -1;
    }
    return 0;
}

/** Synchronize directory contents
 *
 * If the datasync parameter is non-zero, then only the user data
 * should be flushed, not the meta data
 *
 * Introduced in version 2.3
 */
// when exactly is this called?  when a user calls fsync and it
// happens to be a directory? ??? >>> I need to implement this...
int bb_fsyncdir(const char *path, int datasync, struct fuse_file_info *fi)
{
    int retstat = 0;
    
    log_msg("\nbb_fsyncdir(path=\"%s\", datasync=%d, fi=0x%08x)\n",
	    path, datasync, fi);
    log_fi(fi);
    
    return retstat;
}

/**
 * Initialize filesystem
 *
 * The return value will passed in the private_data field of
 * fuse_context to all file operations and as a parameter to the
 * destroy() method.
 *
 * Introduced in version 2.3
 * Changed in version 2.6
 */
// Undocumented but extraordinarily useful fact:  the fuse_context is
// set up before this function is called, and
// fuse_get_context()->private_data returns the user_data passed to
// fuse_main().  Really seems like either it should be a third
// parameter coming in here, or else the fact should be documented
// (and this might as well return void, as it did in older versions of
// FUSE).
void *bb_init(struct fuse_conn_info *conn)
{
    log_msg("\nbb_init()\n");

    log_conn(conn);
    log_fuse_context(fuse_get_context());
    
    return BB_DATA;
}

/**
 * Clean up filesystem
 *
 * Called on filesystem exit.
 *
 * Introduced in version 2.3
 */
void bb_destroy(void *userdata)
{
    log_msg("\nbb_destroy(userdata=0x%08x)\n", userdata);
}

/**
 * Check file access permissions
 *
 * This will be called for the access() system call.  If the
 * 'default_permissions' mount option is given, this method is not
 * called.
 *
 * This method is not called under Linux kernel versions 2.4.x
 *
 * Introduced in version 2.5
 */
int bb_access(const char *path, int mask)
{
    // Check permission via RPC.
    access_ret *ret = NULL;
    access_arg arg;
    char fpath[PATH_MAX];
    bb_fullpath(fpath, path);
    arg.path = fpath;
    arg.mask = mask;
    
    CLIENT *clnt = connect_server();
    ret = bb_access_6(&arg, clnt);
    if (ret == (access_ret *) NULL) {
        log_msg("RPC return value error\n");
        clnt_perror (clnt, "call failed");
    }
    clnt_destroy (clnt);
    
    return ret->ret;
}

/**
 * Create and open a file
 *
 * If the file does not exist, first create it with the specified
 * mode, and then open it.
 *
 * If this method is not implemented or under Linux kernel
 * versions earlier than 2.6.15, the mknod() and open() methods
 * will be called instead.
 *
 * Introduced in version 2.5
 */
// Not implemented.  I had a version that used creat() to create and
// open the file, which it turned out opened the file write-only.

/**
 * Change the size of an open file
 *
 * This method is called instead of the truncate() method if the
 * truncation was invoked from an ftruncate() system call.
 *
 * If this method is not implemented or under Linux kernel
 * versions earlier than 2.6.15, the truncate() method will be
 * called instead.
 *
 * Introduced in version 2.5
 */
int bb_ftruncate(const char *path, off_t offset, struct fuse_file_info *fi)
{
    int retstat = 0;
    
    log_msg("\nbb_ftruncate(path=\"%s\", offset=%lld, fi=0x%08x)\n",
	    path, offset, fi);
    log_fi(fi);
    
    retstat = ftruncate(fi->fh, offset);
    if (retstat < 0)
	retstat = log_error("bb_ftruncate ftruncate");
    
    return retstat;
}

/**
 * Get attributes from an open file
 *
 * This method is called instead of the getattr() method if the
 * file information is available.
 *
 * Currently this is only called after the create() method if that
 * is implemented (see above).  Later it may be called for
 * invocations of fstat() too.
 *
 * Introduced in version 2.5
 */
int bb_fgetattr(const char *path, struct stat *statbuf, struct fuse_file_info *fi)
{
    int ret = bb_getattr(path, statbuf);
    return ret;
}

struct fuse_operations bb_oper = {
  .getattr = bb_getattr,
  .readlink = bb_readlink,
  // no .getdir -- that's deprecated
  .getdir = NULL,
  .mknod = bb_mknod,
  .mkdir = bb_mkdir,
  .unlink = bb_unlink,
  .rmdir = bb_rmdir,
  .symlink = bb_symlink,
  .rename = bb_rename,
  .link = bb_link,
  .chmod = bb_chmod,
  .chown = bb_chown,
  .truncate = bb_truncate,
  .utime = bb_utime,
  .open = bb_open,
  .read = bb_read,
  .write = bb_write,
  /** Just a placeholder, don't set */ // huh???
  .statfs = bb_statfs,
  .flush = bb_flush,
  .release = bb_release,
  .fsync = bb_fsync,
  
#ifdef HAVE_SYS_XATTR_H
  .setxattr = NULL,
  .getxattr = NULL,
  .listxattr = NULL,
  .removexattr = NULL,
#endif
  
  .opendir = bb_opendir,
  .readdir = bb_readdir,
  .releasedir = bb_releasedir,
  .fsyncdir = bb_fsyncdir,
  .init = bb_init,
  .destroy = bb_destroy,
  .access = bb_access,
  .ftruncate = bb_ftruncate,
  .fgetattr = bb_fgetattr
};

void bb_usage()
{
    fprintf(stderr, "usage:  bbfs [FUSE and mount options] rootDir mountPoint\n");
    abort();
}

int main(int argc, char *argv[])
{
    int fuse_stat;
    struct bb_state *bb_data;

    // bbfs doesn't do any access checking on its own (the comment
    // blocks in fuse.h mention some of the functions that need
    // accesses checked -- but note there are other functions, like
    // chown(), that also need checking!).  Since running bbfs as root
    // will therefore open Metrodome-sized holes in the system
    // security, we'll check if root is trying to mount the filesystem
    // and refuse if it is.  The somewhat smaller hole of an ordinary
    // user doing it with the allow_other flag is still there because
    // I don't want to parse the options string.
    if ((getuid() == 0) || (geteuid() == 0)) {
    	fprintf(stderr, "Running BBFS as root opens unnacceptable security holes\n");
    	return 1;
    }

    // See which version of fuse we're running
    fprintf(stderr, "Fuse library version %d.%d\n", FUSE_MAJOR_VERSION, FUSE_MINOR_VERSION);
    
    // Perform some sanity checking on the command line:  make sure
    // there are enough arguments, and that neither of the last two
    // start with a hyphen (this will break if you actually have a
    // rootpoint or mountpoint whose name starts with a hyphen, but so
    // will a zillion other programs)
    // if ((argc < 3) || (argv[argc-2][0] == '-') || (argv[argc-1][0] == '-'))
	// bb_usage();

    fprintf(stderr, "argc = %d\n", argc);

    // Assign host.
    host = argv[argc - 1];
    fprintf(stderr, "server address = %s\n", host);
    --argc;

    bb_data = malloc(sizeof(struct bb_state));
    if (bb_data == NULL) {
        perror("main calloc");
        abort();
    }

    // Pull the rootdir out of the argument list and save it in my
    // internal data
    bb_data->rootdir = realpath(argv[argc-2], NULL);
    rootdir_len = strlen(bb_data->rootdir);
    argv[argc-2] = argv[argc-1];
    argv[argc-1] = NULL;
    argc--;
    
    bb_data->logfile = log_open();
    
    // turn over control to fuse
    fprintf(stderr, "about to call fuse_main\n");
    fuse_stat = fuse_main(argc, argv, &bb_oper, bb_data);
    fprintf(stderr, "fuse_main returned %d\n", fuse_stat);
    
    return fuse_stat;
}

