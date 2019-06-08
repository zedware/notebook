
 Directory of source files
===========================

README           - this file
Makefile         - top level makefile
api/             - test non-LSB APIs
                   If it compiles and links ok, the APIs
                   exist. If it runs ok, the APIs work.
disk/            - test of disk storage
                   Write garbage files to disk to consume
                   disk space. 
file/            - test of file create/read/...
fsync/           - test of file read/write speed
                   See output for details.
memory/          - test of memory exhausted
                   Allocate memory until failed.
process_thread/  - test of process/thread creation
                   Compare the speed of creation.
socket/          - test of TCP server
                   A simple TCP server, listening on given
                   port.

 Usage of these files
======================

1. Goto the top level directory, that's this directory.
2. Type the following comand in shell:
   $ make test
   Most testcases will be executed automatically.
   But `disk`, `file` and `memory` should be executed 
manually.
