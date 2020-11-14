/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _FUSE_RPC_H_RPCGEN
#define _FUSE_RPC_H_RPCGEN

#include <rpc/rpc.h>

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SIZE 4096

struct identity {
	int is_master;
	int is_degraded;
};
typedef struct identity identity;

struct init_arg {
	identity server_info;
	char *ip;
	char *rootdir;
};
typedef struct init_arg init_arg;

struct init_ret {
	int ret;
};
typedef struct init_ret init_ret;

struct getattr_arg {
	char *ip;
	char *path;
};
typedef struct getattr_arg getattr_arg;

struct getattr_ret {
	int st_dev;
	int st_ino;
	int st_mode;
	int st_nlink;
	int st_uid;
	int st_gid;
	int st_rdev;
	u_long st_size;
	u_long st_blksize;
	u_long st_blocks;
	long st_acc_time;
	long st_mod_time;
	long st_chg_time;
	int ret;
};
typedef struct getattr_ret getattr_ret;

struct access_arg {
	char *ip;
	char *path;
	int mask;
};
typedef struct access_arg access_arg;

struct access_ret {
	int ret;
};
typedef struct access_ret access_ret;

struct mkdir_arg {
	char *ip;
	identity server_info;
	char *path;
	int mode;
};
typedef struct mkdir_arg mkdir_arg;

struct mkdir_ret {
	int ret;
};
typedef struct mkdir_ret mkdir_ret;

struct rmdir_arg {
	char *ip;
	identity server_info;
	char *path;
};
typedef struct rmdir_arg rmdir_arg;

struct rmdir_ret {
	int ret;
};
typedef struct rmdir_ret rmdir_ret;

struct opendir_arg {
	char *ip;
	char *path;
};
typedef struct opendir_arg opendir_arg;

struct opendir_ret {
	int fd;
	int ret;
};
typedef struct opendir_ret opendir_ret;

struct readdir_arg {
	int fd;
};
typedef struct readdir_arg readdir_arg;

struct readdir_ret {
	int count;
	char entries[MAX_SIZE];
	int ret;
};
typedef struct readdir_ret readdir_ret;

struct releasedir_arg {
	int fd;
};
typedef struct releasedir_arg releasedir_arg;

struct releasedir_ret {
	int ret;
};
typedef struct releasedir_ret releasedir_ret;

struct rename_arg {
	char *ip;
	identity server_info;
	char *path;
	char *newpath;
};
typedef struct rename_arg rename_arg;

struct rename_ret {
	int ret;
};
typedef struct rename_ret rename_ret;

struct symlink_arg {
	char *ip;
	identity server_info;
	char *path;
	char *link;
};
typedef struct symlink_arg symlink_arg;

struct symlink_ret {
	int ret;
};
typedef struct symlink_ret symlink_ret;

struct link_arg {
	char *ip;
	identity server_info;
	char *path;
	char *newpath;
};
typedef struct link_arg link_arg;

struct link_ret {
	int ret;
};
typedef struct link_ret link_ret;

struct readlink_arg {
	char *ip;
	char *path;
	u_int size;
};
typedef struct readlink_arg readlink_arg;

struct readlink_ret {
	char buffer[MAX_SIZE];
	int len;
	int ret;
};
typedef struct readlink_ret readlink_ret;

struct mknod_arg {
	char *ip;
	identity server_info;
	char *path;
	int mode;
	int dev;
};
typedef struct mknod_arg mknod_arg;

struct mknod_ret {
	int ret;
};
typedef struct mknod_ret mknod_ret;

struct utime_arg {
	char *ip;
	identity server_info;
	char *path;
	long actime;
	long modtime;
};
typedef struct utime_arg utime_arg;

struct utime_ret {
	int ret;
};
typedef struct utime_ret utime_ret;

struct truncate_arg {
	char *ip;
	identity server_info;
	char *path;
	int newsize;
};
typedef struct truncate_arg truncate_arg;

struct truncate_ret {
	int ret;
};
typedef struct truncate_ret truncate_ret;

struct chmod_arg {
	char *ip;
	identity server_info;
	char *path;
	int mode;
};
typedef struct chmod_arg chmod_arg;

struct chmod_ret {
	int ret;
};
typedef struct chmod_ret chmod_ret;

struct chown_arg {
	char *ip;
	identity server_info;
	char *path;
	u_int uid;
	u_int gid;
};
typedef struct chown_arg chown_arg;

struct chown_ret {
	int ret;
};
typedef struct chown_ret chown_ret;

struct unlink_arg {
	char *ip;
	identity server_info;
	char *path;
};
typedef struct unlink_arg unlink_arg;

struct unlink_ret {
	int ret;
};
typedef struct unlink_ret unlink_ret;

struct open_arg {
	char *ip;
	char *path;
	int flags;
};
typedef struct open_arg open_arg;

struct open_ret {
	int fd;
	int ret;
};
typedef struct open_ret open_ret;

struct release_arg {
	char *ip;
	int fd;
};
typedef struct release_arg release_arg;

struct release_ret {
	int ret;
};
typedef struct release_ret release_ret;

struct read_arg {
	char *ip;
	int fd;
	u_int size;
	u_int offset;
};
typedef struct read_arg read_arg;

struct read_ret {
	char buffer[MAX_SIZE];
	int len;
	int ret;
};
typedef struct read_ret read_ret;

struct write_arg {
	char *ip;
	identity server_info;
	int fd;
	char *path;
	u_int size;
	u_int offset;
	char buffer[MAX_SIZE];
};
typedef struct write_arg write_arg;

struct write_ret {
	int len;
	int ret;
};
typedef struct write_ret write_ret;

struct statfs_arg {
	char *path;
};
typedef struct statfs_arg statfs_arg;

struct statfs_ret {
	u_long f_bsize;
	u_long f_frsize;
	u_long f_blocks;
	u_long f_bfree;
	u_long f_bavail;
	u_long f_files;
	u_long f_ffree;
	u_long f_favail;
	u_long f_fsid;
	u_long f_flag;
	u_long f_namemax;
	int ret;
};
typedef struct statfs_ret statfs_ret;

#define COMPUTE 456123789
#define COMPUTE_VERS 6

#if defined(__STDC__) || defined(__cplusplus)
#define INIT_ROOTDIR 0
extern  enum clnt_stat init_rootdir_6(init_arg *, init_ret *, CLIENT *);
extern  bool_t init_rootdir_6_svc(init_arg *, init_ret *, struct svc_req *);
#define BB_GETATTR 1
extern  enum clnt_stat bb_getattr_6(getattr_arg *, getattr_ret *, CLIENT *);
extern  bool_t bb_getattr_6_svc(getattr_arg *, getattr_ret *, struct svc_req *);
#define BB_ACCESS 2
extern  enum clnt_stat bb_access_6(access_arg *, access_ret *, CLIENT *);
extern  bool_t bb_access_6_svc(access_arg *, access_ret *, struct svc_req *);
#define BB_MKDIR 3
extern  enum clnt_stat bb_mkdir_6(mkdir_arg *, mkdir_ret *, CLIENT *);
extern  bool_t bb_mkdir_6_svc(mkdir_arg *, mkdir_ret *, struct svc_req *);
#define BB_RMDIR 4
extern  enum clnt_stat bb_rmdir_6(rmdir_arg *, rmdir_ret *, CLIENT *);
extern  bool_t bb_rmdir_6_svc(rmdir_arg *, rmdir_ret *, struct svc_req *);
#define BB_OPENDIR 5
extern  enum clnt_stat bb_opendir_6(opendir_arg *, opendir_ret *, CLIENT *);
extern  bool_t bb_opendir_6_svc(opendir_arg *, opendir_ret *, struct svc_req *);
#define BB_READDIR 6
extern  enum clnt_stat bb_readdir_6(readdir_arg *, readdir_ret *, CLIENT *);
extern  bool_t bb_readdir_6_svc(readdir_arg *, readdir_ret *, struct svc_req *);
#define BB_RELEASEDIR 7
extern  enum clnt_stat bb_releasedir_6(releasedir_arg *, releasedir_ret *, CLIENT *);
extern  bool_t bb_releasedir_6_svc(releasedir_arg *, releasedir_ret *, struct svc_req *);
#define BB_RENAME 8
extern  enum clnt_stat bb_rename_6(rename_arg *, rename_ret *, CLIENT *);
extern  bool_t bb_rename_6_svc(rename_arg *, rename_ret *, struct svc_req *);
#define BB_SYMLINK 9
extern  enum clnt_stat bb_symlink_6(symlink_arg *, symlink_ret *, CLIENT *);
extern  bool_t bb_symlink_6_svc(symlink_arg *, symlink_ret *, struct svc_req *);
#define BB_LINK 10
extern  enum clnt_stat bb_link_6(link_arg *, link_ret *, CLIENT *);
extern  bool_t bb_link_6_svc(link_arg *, link_ret *, struct svc_req *);
#define BB_READLINK 11
extern  enum clnt_stat bb_readlink_6(readlink_arg *, readlink_ret *, CLIENT *);
extern  bool_t bb_readlink_6_svc(readlink_arg *, readlink_ret *, struct svc_req *);
#define BB_MKNOD 12
extern  enum clnt_stat bb_mknod_6(mknod_arg *, mknod_ret *, CLIENT *);
extern  bool_t bb_mknod_6_svc(mknod_arg *, mknod_ret *, struct svc_req *);
#define BB_TRUNCATE 13
extern  enum clnt_stat bb_truncate_6(truncate_arg *, truncate_ret *, CLIENT *);
extern  bool_t bb_truncate_6_svc(truncate_arg *, truncate_ret *, struct svc_req *);
#define BB_UNLINK 14
extern  enum clnt_stat bb_unlink_6(unlink_arg *, unlink_ret *, CLIENT *);
extern  bool_t bb_unlink_6_svc(unlink_arg *, unlink_ret *, struct svc_req *);
#define BB_UTIME 15
extern  enum clnt_stat bb_utime_6(utime_arg *, utime_ret *, CLIENT *);
extern  bool_t bb_utime_6_svc(utime_arg *, utime_ret *, struct svc_req *);
#define BB_CHMOD 16
extern  enum clnt_stat bb_chmod_6(chmod_arg *, chmod_ret *, CLIENT *);
extern  bool_t bb_chmod_6_svc(chmod_arg *, chmod_ret *, struct svc_req *);
#define BB_CHOWN 17
extern  enum clnt_stat bb_chown_6(chown_arg *, chown_ret *, CLIENT *);
extern  bool_t bb_chown_6_svc(chown_arg *, chown_ret *, struct svc_req *);
#define BB_OPEN 18
extern  enum clnt_stat bb_open_6(open_arg *, open_ret *, CLIENT *);
extern  bool_t bb_open_6_svc(open_arg *, open_ret *, struct svc_req *);
#define BB_RELEASE 19
extern  enum clnt_stat bb_release_6(release_arg *, release_ret *, CLIENT *);
extern  bool_t bb_release_6_svc(release_arg *, release_ret *, struct svc_req *);
#define BB_READ 20
extern  enum clnt_stat bb_read_6(read_arg *, read_ret *, CLIENT *);
extern  bool_t bb_read_6_svc(read_arg *, read_ret *, struct svc_req *);
#define BB_WRITE 21
extern  enum clnt_stat bb_write_6(write_arg *, write_ret *, CLIENT *);
extern  bool_t bb_write_6_svc(write_arg *, write_ret *, struct svc_req *);
#define BB_STATFS 22
extern  enum clnt_stat bb_statfs_6(statfs_arg *, statfs_ret *, CLIENT *);
extern  bool_t bb_statfs_6_svc(statfs_arg *, statfs_ret *, struct svc_req *);
extern int compute_6_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define INIT_ROOTDIR 0
extern  enum clnt_stat init_rootdir_6();
extern  bool_t init_rootdir_6_svc();
#define BB_GETATTR 1
extern  enum clnt_stat bb_getattr_6();
extern  bool_t bb_getattr_6_svc();
#define BB_ACCESS 2
extern  enum clnt_stat bb_access_6();
extern  bool_t bb_access_6_svc();
#define BB_MKDIR 3
extern  enum clnt_stat bb_mkdir_6();
extern  bool_t bb_mkdir_6_svc();
#define BB_RMDIR 4
extern  enum clnt_stat bb_rmdir_6();
extern  bool_t bb_rmdir_6_svc();
#define BB_OPENDIR 5
extern  enum clnt_stat bb_opendir_6();
extern  bool_t bb_opendir_6_svc();
#define BB_READDIR 6
extern  enum clnt_stat bb_readdir_6();
extern  bool_t bb_readdir_6_svc();
#define BB_RELEASEDIR 7
extern  enum clnt_stat bb_releasedir_6();
extern  bool_t bb_releasedir_6_svc();
#define BB_RENAME 8
extern  enum clnt_stat bb_rename_6();
extern  bool_t bb_rename_6_svc();
#define BB_SYMLINK 9
extern  enum clnt_stat bb_symlink_6();
extern  bool_t bb_symlink_6_svc();
#define BB_LINK 10
extern  enum clnt_stat bb_link_6();
extern  bool_t bb_link_6_svc();
#define BB_READLINK 11
extern  enum clnt_stat bb_readlink_6();
extern  bool_t bb_readlink_6_svc();
#define BB_MKNOD 12
extern  enum clnt_stat bb_mknod_6();
extern  bool_t bb_mknod_6_svc();
#define BB_TRUNCATE 13
extern  enum clnt_stat bb_truncate_6();
extern  bool_t bb_truncate_6_svc();
#define BB_UNLINK 14
extern  enum clnt_stat bb_unlink_6();
extern  bool_t bb_unlink_6_svc();
#define BB_UTIME 15
extern  enum clnt_stat bb_utime_6();
extern  bool_t bb_utime_6_svc();
#define BB_CHMOD 16
extern  enum clnt_stat bb_chmod_6();
extern  bool_t bb_chmod_6_svc();
#define BB_CHOWN 17
extern  enum clnt_stat bb_chown_6();
extern  bool_t bb_chown_6_svc();
#define BB_OPEN 18
extern  enum clnt_stat bb_open_6();
extern  bool_t bb_open_6_svc();
#define BB_RELEASE 19
extern  enum clnt_stat bb_release_6();
extern  bool_t bb_release_6_svc();
#define BB_READ 20
extern  enum clnt_stat bb_read_6();
extern  bool_t bb_read_6_svc();
#define BB_WRITE 21
extern  enum clnt_stat bb_write_6();
extern  bool_t bb_write_6_svc();
#define BB_STATFS 22
extern  enum clnt_stat bb_statfs_6();
extern  bool_t bb_statfs_6_svc();
extern int compute_6_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_identity (XDR *, identity*);
extern  bool_t xdr_init_arg (XDR *, init_arg*);
extern  bool_t xdr_init_ret (XDR *, init_ret*);
extern  bool_t xdr_getattr_arg (XDR *, getattr_arg*);
extern  bool_t xdr_getattr_ret (XDR *, getattr_ret*);
extern  bool_t xdr_access_arg (XDR *, access_arg*);
extern  bool_t xdr_access_ret (XDR *, access_ret*);
extern  bool_t xdr_mkdir_arg (XDR *, mkdir_arg*);
extern  bool_t xdr_mkdir_ret (XDR *, mkdir_ret*);
extern  bool_t xdr_rmdir_arg (XDR *, rmdir_arg*);
extern  bool_t xdr_rmdir_ret (XDR *, rmdir_ret*);
extern  bool_t xdr_opendir_arg (XDR *, opendir_arg*);
extern  bool_t xdr_opendir_ret (XDR *, opendir_ret*);
extern  bool_t xdr_readdir_arg (XDR *, readdir_arg*);
extern  bool_t xdr_readdir_ret (XDR *, readdir_ret*);
extern  bool_t xdr_releasedir_arg (XDR *, releasedir_arg*);
extern  bool_t xdr_releasedir_ret (XDR *, releasedir_ret*);
extern  bool_t xdr_rename_arg (XDR *, rename_arg*);
extern  bool_t xdr_rename_ret (XDR *, rename_ret*);
extern  bool_t xdr_symlink_arg (XDR *, symlink_arg*);
extern  bool_t xdr_symlink_ret (XDR *, symlink_ret*);
extern  bool_t xdr_link_arg (XDR *, link_arg*);
extern  bool_t xdr_link_ret (XDR *, link_ret*);
extern  bool_t xdr_readlink_arg (XDR *, readlink_arg*);
extern  bool_t xdr_readlink_ret (XDR *, readlink_ret*);
extern  bool_t xdr_mknod_arg (XDR *, mknod_arg*);
extern  bool_t xdr_mknod_ret (XDR *, mknod_ret*);
extern  bool_t xdr_utime_arg (XDR *, utime_arg*);
extern  bool_t xdr_utime_ret (XDR *, utime_ret*);
extern  bool_t xdr_truncate_arg (XDR *, truncate_arg*);
extern  bool_t xdr_truncate_ret (XDR *, truncate_ret*);
extern  bool_t xdr_chmod_arg (XDR *, chmod_arg*);
extern  bool_t xdr_chmod_ret (XDR *, chmod_ret*);
extern  bool_t xdr_chown_arg (XDR *, chown_arg*);
extern  bool_t xdr_chown_ret (XDR *, chown_ret*);
extern  bool_t xdr_unlink_arg (XDR *, unlink_arg*);
extern  bool_t xdr_unlink_ret (XDR *, unlink_ret*);
extern  bool_t xdr_open_arg (XDR *, open_arg*);
extern  bool_t xdr_open_ret (XDR *, open_ret*);
extern  bool_t xdr_release_arg (XDR *, release_arg*);
extern  bool_t xdr_release_ret (XDR *, release_ret*);
extern  bool_t xdr_read_arg (XDR *, read_arg*);
extern  bool_t xdr_read_ret (XDR *, read_ret*);
extern  bool_t xdr_write_arg (XDR *, write_arg*);
extern  bool_t xdr_write_ret (XDR *, write_ret*);
extern  bool_t xdr_statfs_arg (XDR *, statfs_arg*);
extern  bool_t xdr_statfs_ret (XDR *, statfs_ret*);

#else /* K&R C */
extern bool_t xdr_identity ();
extern bool_t xdr_init_arg ();
extern bool_t xdr_init_ret ();
extern bool_t xdr_getattr_arg ();
extern bool_t xdr_getattr_ret ();
extern bool_t xdr_access_arg ();
extern bool_t xdr_access_ret ();
extern bool_t xdr_mkdir_arg ();
extern bool_t xdr_mkdir_ret ();
extern bool_t xdr_rmdir_arg ();
extern bool_t xdr_rmdir_ret ();
extern bool_t xdr_opendir_arg ();
extern bool_t xdr_opendir_ret ();
extern bool_t xdr_readdir_arg ();
extern bool_t xdr_readdir_ret ();
extern bool_t xdr_releasedir_arg ();
extern bool_t xdr_releasedir_ret ();
extern bool_t xdr_rename_arg ();
extern bool_t xdr_rename_ret ();
extern bool_t xdr_symlink_arg ();
extern bool_t xdr_symlink_ret ();
extern bool_t xdr_link_arg ();
extern bool_t xdr_link_ret ();
extern bool_t xdr_readlink_arg ();
extern bool_t xdr_readlink_ret ();
extern bool_t xdr_mknod_arg ();
extern bool_t xdr_mknod_ret ();
extern bool_t xdr_utime_arg ();
extern bool_t xdr_utime_ret ();
extern bool_t xdr_truncate_arg ();
extern bool_t xdr_truncate_ret ();
extern bool_t xdr_chmod_arg ();
extern bool_t xdr_chmod_ret ();
extern bool_t xdr_chown_arg ();
extern bool_t xdr_chown_ret ();
extern bool_t xdr_unlink_arg ();
extern bool_t xdr_unlink_ret ();
extern bool_t xdr_open_arg ();
extern bool_t xdr_open_ret ();
extern bool_t xdr_release_arg ();
extern bool_t xdr_release_ret ();
extern bool_t xdr_read_arg ();
extern bool_t xdr_read_ret ();
extern bool_t xdr_write_arg ();
extern bool_t xdr_write_ret ();
extern bool_t xdr_statfs_arg ();
extern bool_t xdr_statfs_ret ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_FUSE_RPC_H_RPCGEN */
