
all:
	make -C file all
	make -C process_thread all
	make -C disk all
	make -C memory all
	make -C socket all
	make -C api
	make -C fsync

test:
	make -C file test
	make -C process_thread test
#	make -C disk test
#	make -C memory test
#	make -C socket test
	make -C api test
	make -C fsync test

clean:
	make -C file clean
	make -C process_thread clean
	make -C disk clean
	make -C memory clean
	make -C socket clean
	make -C api clean
	make -C fsync clean

