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
        readdir_ret BB_READDIR(readdir_arg) = 3;
        opendir_ret BB_OPENDIR(opendir_arg) = 4;
        releasedir_ret BB_RELEASEDIR(releasedir_arg) = 5;
        open_ret BB_OPEN(open_arg) = 6;
        release_ret BB_RELEASE(release_arg) = 7;
        read_ret BB_READ(read_arg) = 8;
        write_ret BB_WRITE(write_arg) = 9;
    } = 6;
} = 456123789;