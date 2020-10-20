/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "fuse_rpc.h"

bool_t
xdr_getattr_arg (XDR *xdrs, getattr_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_getattr_ret (XDR *xdrs, getattr_ret *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, 14 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->st_dev))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->st_ino))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->st_mode))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->st_nlink))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->st_uid))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->st_gid))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->st_rdev))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->st_size))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->st_blksize))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->st_blocks))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->st_atimensec))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->st_mtimensec))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->st_ctimensec))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->ret))
				 return FALSE;
		} else {
			IXDR_PUT_LONG(buf, objp->st_dev);
			IXDR_PUT_LONG(buf, objp->st_ino);
			IXDR_PUT_LONG(buf, objp->st_mode);
			IXDR_PUT_LONG(buf, objp->st_nlink);
			IXDR_PUT_LONG(buf, objp->st_uid);
			IXDR_PUT_LONG(buf, objp->st_gid);
			IXDR_PUT_LONG(buf, objp->st_rdev);
			IXDR_PUT_U_LONG(buf, objp->st_size);
			IXDR_PUT_U_LONG(buf, objp->st_blksize);
			IXDR_PUT_U_LONG(buf, objp->st_blocks);
			IXDR_PUT_LONG(buf, objp->st_atimensec);
			IXDR_PUT_LONG(buf, objp->st_mtimensec);
			IXDR_PUT_LONG(buf, objp->st_ctimensec);
			IXDR_PUT_LONG(buf, objp->ret);
		}
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, 14 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->st_dev))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->st_ino))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->st_mode))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->st_nlink))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->st_uid))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->st_gid))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->st_rdev))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->st_size))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->st_blksize))
				 return FALSE;
			 if (!xdr_u_long (xdrs, &objp->st_blocks))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->st_atimensec))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->st_mtimensec))
				 return FALSE;
			 if (!xdr_long (xdrs, &objp->st_ctimensec))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->ret))
				 return FALSE;
		} else {
			objp->st_dev = IXDR_GET_LONG(buf);
			objp->st_ino = IXDR_GET_LONG(buf);
			objp->st_mode = IXDR_GET_LONG(buf);
			objp->st_nlink = IXDR_GET_LONG(buf);
			objp->st_uid = IXDR_GET_LONG(buf);
			objp->st_gid = IXDR_GET_LONG(buf);
			objp->st_rdev = IXDR_GET_LONG(buf);
			objp->st_size = IXDR_GET_U_LONG(buf);
			objp->st_blksize = IXDR_GET_U_LONG(buf);
			objp->st_blocks = IXDR_GET_U_LONG(buf);
			objp->st_atimensec = IXDR_GET_LONG(buf);
			objp->st_mtimensec = IXDR_GET_LONG(buf);
			objp->st_ctimensec = IXDR_GET_LONG(buf);
			objp->ret = IXDR_GET_LONG(buf);
		}
	 return TRUE;
	}

	 if (!xdr_int (xdrs, &objp->st_dev))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->st_ino))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->st_mode))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->st_nlink))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->st_uid))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->st_gid))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->st_rdev))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->st_size))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->st_blksize))
		 return FALSE;
	 if (!xdr_u_long (xdrs, &objp->st_blocks))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->st_atimensec))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->st_mtimensec))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->st_ctimensec))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_access_arg (XDR *xdrs, access_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->mask))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_access_ret (XDR *xdrs, access_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mkdir_arg (XDR *xdrs, mkdir_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->mode))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mkdir_ret (XDR *xdrs, mkdir_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_rmdir_arg (XDR *xdrs, rmdir_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_rmdir_ret (XDR *xdrs, rmdir_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_opendir_arg (XDR *xdrs, opendir_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_opendir_ret (XDR *xdrs, opendir_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->fd))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_readdir_arg (XDR *xdrs, readdir_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->fd))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_readdir_ret (XDR *xdrs, readdir_ret *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_int (xdrs, &objp->count))
		 return FALSE;
	 if (!xdr_opaque (xdrs, objp->entries, MAX_SIZE))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_releasedir_arg (XDR *xdrs, releasedir_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->fd))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_releasedir_ret (XDR *xdrs, releasedir_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_rename_arg (XDR *xdrs, rename_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->newpath, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_rename_ret (XDR *xdrs, rename_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_symlink_arg (XDR *xdrs, symlink_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->link, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_symlink_ret (XDR *xdrs, symlink_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_readlink_arg (XDR *xdrs, readlink_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->size))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_readlink_ret (XDR *xdrs, readlink_ret *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_opaque (xdrs, objp->buffer, MAX_SIZE))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->len))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mknod_arg (XDR *xdrs, mknod_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->mode))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->dev))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_mknod_ret (XDR *xdrs, mknod_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_utime_arg (XDR *xdrs, utime_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->actime))
		 return FALSE;
	 if (!xdr_long (xdrs, &objp->modtime))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_utime_ret (XDR *xdrs, utime_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_truncate_arg (XDR *xdrs, truncate_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->newsize))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_truncate_ret (XDR *xdrs, truncate_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_chmod_arg (XDR *xdrs, chmod_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->mode))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_chmod_ret (XDR *xdrs, chmod_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_chown_arg (XDR *xdrs, chown_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->uid))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->gid))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_chown_ret (XDR *xdrs, chown_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_unlink_arg (XDR *xdrs, unlink_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_unlink_ret (XDR *xdrs, unlink_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_open_arg (XDR *xdrs, open_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->path, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->flags))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_open_ret (XDR *xdrs, open_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->fd))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_release_arg (XDR *xdrs, release_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->fd))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_release_ret (XDR *xdrs, release_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_read_arg (XDR *xdrs, read_arg *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->fd))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->size))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->offset))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_read_ret (XDR *xdrs, read_ret *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_opaque (xdrs, objp->buffer, MAX_SIZE))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->len))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_write_arg (XDR *xdrs, write_arg *objp)
{
	register int32_t *buf;

	int i;

	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, 3 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->fd))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->size))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->offset))
				 return FALSE;

		} else {
		IXDR_PUT_LONG(buf, objp->fd);
		IXDR_PUT_U_LONG(buf, objp->size);
		IXDR_PUT_U_LONG(buf, objp->offset);
		}
		 if (!xdr_opaque (xdrs, objp->buffer, MAX_SIZE))
			 return FALSE;
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, 3 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->fd))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->size))
				 return FALSE;
			 if (!xdr_u_int (xdrs, &objp->offset))
				 return FALSE;

		} else {
		objp->fd = IXDR_GET_LONG(buf);
		objp->size = IXDR_GET_U_LONG(buf);
		objp->offset = IXDR_GET_U_LONG(buf);
		}
		 if (!xdr_opaque (xdrs, objp->buffer, MAX_SIZE))
			 return FALSE;
	 return TRUE;
	}

	 if (!xdr_int (xdrs, &objp->fd))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->size))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->offset))
		 return FALSE;
	 if (!xdr_opaque (xdrs, objp->buffer, MAX_SIZE))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_write_ret (XDR *xdrs, write_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->len))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->ret))
		 return FALSE;
	return TRUE;
}
