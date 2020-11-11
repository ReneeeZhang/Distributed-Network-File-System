### Distributed File system
This project is distributed file system based on [FUSE](https://github.com/libfuse/libfuse). Currently the cluster contains only two servers, one primary and one secondary, but it could be extended to more.

#### How to run the program
Before compiling the project, make sure the environment is set-up.
```
sudo apt update 
sudo apt dist-upgrade 
sudo apt install build-essential pkg-config libfuse-dev
```
This repo has provided Makefile, but several places needs to be updated.
`make header` compiles and updates the RPC header and stab files.
`make all` compiles client-side and server-side executable file `tweakfs` and `fuse_rpc_server` respectively.
`make create` generates the rootdir and mountdir automatically, which is the pre-requisite for the program.
`make debug` needs a little update: the last two arguments are IP addresses for primary and secondary server.
`make unmount` unmount the mountdir off rootdir.
`make clean` cleans all generated object files and rootdir, mountdir.
Additionally, on server-side, I did a hard code of secondary server IP address, ``connect_server``which needs to be corrected.
System configuration file `/etc/hosts` should also appended the target server IP. Example:
```
127.0.0.1       localhost
127.0.1.1       ubuntu

# For RPC.
67.159.88.71 hj110@vcm-16008.vm.duke.edu
10.148.54.200 localadmin@esa08.egr.duke.edu

# The following lines are desirable for IPv6 capable hosts
::1     localhost ip6-localhost ip6-loopback
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
```

#### Implementation details
1. On client side, every file system operation will be transmitted to server side via RPC. In this project, I select [Sun RPC](https://docs.oracle.com/cd/E19683-01/816-1435/6m7rrfn9k/index.html). Client side always tries to connect primary server first, then secondary server if fails.
2. `.x` file is the communication protocal for RPC.
Naming pattern:
- client-to-server message structs are named as function_arg, server-to-client message structs are named as function_ret.
- Every server-to-client response includes a data member ret, it returns 0 if RPC succeeds, otherwise it'd be assigned -errno.
- For r/w requests, a len member is included, which indicates the actual bytes completes.
- Every *_arg has a data member called ip, which is used to differentiate seperate path for distinct clients. To create path at server side, it's represented as a unsigned integer.
3. Server handles every RPC requests from client side.
For primary server, several updating operation needs to be completed on secondary server for replication and availability, say, `write`, `mkdir`, `rmdir`, etc. It first transmit these requests to secondary server, and handle the request whatever the second one succeeds or fails. Other read-only requests will only be handled on primary server.
4. Primary server goes through the "handle flow" of `opendir` - `readdir` - `releasedir`, `open` - `read` - `release`, so it could use the fd acquired from previous operations, while the secondary server have to rely on full path.
5. All operations are executed in the order of:

(1) transmit request to primary server, if possible

(2) check rwx validility, if needed

(3) lock the file or directory, if needed

(4) execute the operation

(5) unlock the locked resource

6. Server stores all files under the root directory `/DFS`, and create it at the very beginning it doesn't exist. All files are considered visible by all user mounted, but other operations, like updating, removing, reading needs further authentication.

7. About access authentication:
(1) file open is checked by read and write bit

(2) directory listing is checked by read bit, open is checked by execute bit

(3) file creation, rename and deletion need the rwx bits of parent directory, related operations include operation: unlink, mkdir, rmdir, mknod, rename, symlink, link

(4) chown needs root, chmod needs owner, utime needs owner

8. Locking mechanism:
(1) add shared lock for file read and directory read

(2) add exclusive lock for file write

(3) add exclusive lock for directory mutation, eg: rename, unlink, etc

9. About server recovery, we have provided a python script which does cold restoration. Eg, you could simply `sudo python rsync.py localadmin@esa08.egr.duke.edu:/9962309 /`

#### Known bugs
1. We have handled the cases where primary server or secondary one cannot be reached. But if the client goes down, fd won't be released, thus system resource is leaked.
2. Some operations need root identity, say, `chown`. We haven't dealt with that case.