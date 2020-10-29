all: server tweakfs

clean:
#	make -f Makefile.fuse_rpc clean
	rm *.o
	rm bbfs.log
	rm -f fuse_rpc_client
	rm -f fuse_rpc_server
	rm -f tweakfs
	rm -rf mountdir
	rm -rf rootdir

create:
	mkdir mountdir
	mkdir rootdir
	touch rootdir/secret.txt
	touch rootdir/secret_file.txt
	touch rootdir/bogus.txt
	echo "hello world" > rootdir/bogus.txt
	echo "secret" > rootdir/secret.txt
	echo "secret_file" > rootdir/secret_file.txt
	mkdir rootdir/testdir

header: fuse_rpc.x
#	rpcgen -M -a -C fuse_rpc.x
	rpcgen -M fuse_rpc.x

server: fuse_rpc.h fuse_rpc_svc.c fuse_rpc_server.c fuse_rpc_xdr.c fuse_rpc_clnt.c
	gcc -g3 -pthread fuse_rpc_svc.c fuse_rpc_server.c fuse_rpc_xdr.c fuse_rpc_clnt.c -o fuse_rpc_server

tweakfs: tweakfs.c log.c fuse_rpc_clnt.o fuse_rpc_xdr.o
	gcc -g3 -pthread $^ `pkg-config fuse --cflags --libs` -o $@

mount:
	./tweakfs -d -s rootdir/ mountdir/ localhost
#	./tweakfs rootdir/ mountdir/ localhost

debug:
	valgrind ./tweakfs -d -s rootdir/ mountdir localhost	

unmount:
	fusermount -u mountdir
	rm bbfs.log

rpc:
#	rpcgen -a -C fuse_rpc.x
	make -f Makefile.fuse_rpc

new:
	make unmount
	make clean
	make
	make create
