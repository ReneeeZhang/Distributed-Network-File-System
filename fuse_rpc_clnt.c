/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "fuse_rpc.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

getattr_ret *
bb_getattr_6(getattr_arg *argp, CLIENT *clnt)
{
	static getattr_ret clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, BB_GETATTR,
		(xdrproc_t) xdr_getattr_arg, (caddr_t) argp,
		(xdrproc_t) xdr_getattr_ret, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

access_ret *
bb_access_6(access_arg *argp, CLIENT *clnt)
{
	static access_ret clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, BB_ACCESS,
		(xdrproc_t) xdr_access_arg, (caddr_t) argp,
		(xdrproc_t) xdr_access_ret, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

readdir_ret *
bb_readdir_6(readdir_arg *argp, CLIENT *clnt)
{
	static readdir_ret clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, BB_READDIR,
		(xdrproc_t) xdr_readdir_arg, (caddr_t) argp,
		(xdrproc_t) xdr_readdir_ret, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

opendir_ret *
bb_opendir_6(opendir_arg *argp, CLIENT *clnt)
{
	static opendir_ret clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, BB_OPENDIR,
		(xdrproc_t) xdr_opendir_arg, (caddr_t) argp,
		(xdrproc_t) xdr_opendir_ret, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

releasedir_ret *
bb_releasedir_6(releasedir_arg *argp, CLIENT *clnt)
{
	static releasedir_ret clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, BB_RELEASEDIR,
		(xdrproc_t) xdr_releasedir_arg, (caddr_t) argp,
		(xdrproc_t) xdr_releasedir_ret, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

open_ret *
bb_open_6(open_arg *argp, CLIENT *clnt)
{
	static open_ret clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, BB_OPEN,
		(xdrproc_t) xdr_open_arg, (caddr_t) argp,
		(xdrproc_t) xdr_open_ret, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

release_ret *
bb_release_6(release_arg *argp, CLIENT *clnt)
{
	static release_ret clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, BB_RELEASE,
		(xdrproc_t) xdr_release_arg, (caddr_t) argp,
		(xdrproc_t) xdr_release_ret, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

read_ret *
bb_read_6(read_arg *argp, CLIENT *clnt)
{
	static read_ret clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, BB_READ,
		(xdrproc_t) xdr_read_arg, (caddr_t) argp,
		(xdrproc_t) xdr_read_ret, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

write_ret *
bb_write_6(write_arg *argp, CLIENT *clnt)
{
	static write_ret clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, BB_WRITE,
		(xdrproc_t) xdr_write_arg, (caddr_t) argp,
		(xdrproc_t) xdr_write_ret, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
