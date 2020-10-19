/*
 * Naming pattern:
 * (1) client-to-server message structs are named as <function>_arg,
 * server-to-client message structs are named as <function>_ret.
 * (2) Every server-to-client response includes a data member ret,
 * it returns 0 if RPC succeeds, otherwise it'd be assigned -errno.
 * (3) For r/w requests, a len member is included, which indicates the 
 * actual bytes completes.
 */

const MAX_SIZE = 4096;

struct getattr_arg {
    string path<>;
};

struct getattr_ret {
    int st_dev; /* ID of device containing file */
    int st_ino; /* inode number */
    int st_mode; /* protection */
    int st_nlink; /* number osf hard links */
    int st_uid; /* user ID of owner */
    int st_gid; /* group ID of owner */
    int st_rdev; /* device ID (if special file) */
    unsigned long st_size; /* total size, in bytes */
    unsigned long st_blksize; /* blocksize for file system I/O */
    unsigned long st_blocks; /* number of 512B blocks allocated */
    long st_atimensec; /* time of last access */
    long st_mtimensec; /* time of last modification */
    long st_ctimensec; /* time of last status change */
    int ret; /* status of RPC */
};

struct access_arg {
    string path<>;
    int mask;
};

struct access_ret {
    int ret; /* whether process can access file */
};

struct mkdir_arg {
    string path<>;
    int mode;
};

struct mkdir_ret {
    int ret; /* status of RPC */
};

struct rmdir_arg {
    string path<>;
};

struct rmdir_ret {
    int ret; /* status of RPC */
};

struct readdir_arg {
    string path<>;
};

struct readdir_ret {
    int count; /* number of struct dirent */
    opaque entries[MAX_SIZE]; /* dirent entry name */
    int ret; /* status of RPC */
};

struct opendir_arg {
    string path<>;
};

struct opendir_ret {
    int fd; /* file descriptor of the directory */
    int ret; /* status of RPC */
};

struct releasedir_arg {
    int fd;
};

struct releasedir_ret {
    int ret; /* status of RPC */
};

struct rename_arg {
    string path<>;
    string newpath<>;
};

struct rename_ret {
    int ret; /* status of RPC */
};

struct symlink_arg {
    char *path;
    char *link;
};

struct symlink_ret {
    int ret; /* status of RPC */
};

struct mknod_arg {
    string path<>;
    int mode; /* protection */
    int dev; /* ID of device containing file */
};

struct mknod_ret {
    int ret; /* status of RPC */
};

struct utime_arg {
    string path<>;
    long actime; /* access time */
    long modtime; /* modification time */
};

struct utime_ret {
    int ret; /* status of RPC */
};

struct truncate_arg {
    string path<>;
    int newsize;
};

struct truncate_ret {
    int ret; /* status of RPC */
};

struct unlink_arg {
    string path<>;
};

struct unlink_ret {
    int ret; /* status of RPC */
};

struct open_arg {
    string path<>;
    int flags;
};

struct open_ret {
    int fd; /* file descriptor of file */
    int ret; /* status of RPC */
};

struct release_arg {
    int fd;
};

struct release_ret {
    int ret; /* status of RPC */
};

struct read_arg {
    int fd;
    unsigned int size;
    unsigned int offset;
};

struct read_ret {
    opaque buffer[MAX_SIZE]; /* file content */
    int len; /* length of the content */
    int ret; /* status of RPC */
};

struct write_arg {
    int fd;
    unsigned int size;
    unsigned int offset;
    opaque buffer[MAX_SIZE]; /* write-in content */
};

struct write_ret {
    int len; /* length of the content */
    int ret; /* status of RPC */
};

program COMPUTE{
    version COMPUTE_VERS {
        getattr_ret BB_GETATTR(getattr_arg) = 1;
        access_ret BB_ACCESS(access_arg) = 2;
        mkdir_ret BB_MKDIR(mkdir_arg) = 3;
        rmdir_ret BB_RMDIR(rmdir_arg) = 4;
        readdir_ret BB_READDIR(readdir_arg) = 5;
        opendir_ret BB_OPENDIR(opendir_arg) = 6;
        releasedir_ret BB_RELEASEDIR(releasedir_arg) = 7;
        rename_ret BB_RENAME(rename_arg) = 8;
        symlink_ret BB_SYMLINK(symlink_arg) = 9;
        mknod_ret BB_MKNOD(mknod_arg) = 10;
        truncate_ret BB_TRUNCATE(truncate_arg) = 11;
        unlink_ret BB_UNLINK(unlink_arg) = 12;
        utime_ret BB_UTIME(utime_arg) = 13;
        open_ret BB_OPEN(open_arg) = 14;
        release_ret BB_RELEASE(release_arg) = 15;
        read_ret BB_READ(read_arg) = 16;
        write_ret BB_WRITE(write_arg) = 17;
    } = 6;
} = 456123789;