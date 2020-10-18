/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "fuse_rpc.h"


void
compute_6(char *host)
{
	CLIENT *clnt;
	getattr_ret  *result_1;
	getattr_arg  bb_getattr_6_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, COMPUTE, COMPUTE_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = bb_getattr_6(&bb_getattr_6_arg, clnt);
	if (result_1 == (getattr_ret *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	compute_6 (host);
exit (0);
}
