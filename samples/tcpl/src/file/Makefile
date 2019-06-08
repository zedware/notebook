
all: file_init file_run

file_init: file_lib.o file_init.o

file_run: file_lib.o file_run.o

test: all
# create dir
	@rm -rf out log
	@mkdir out log
# create 1024 files, and write 1024 pages
	./file_init 1024 1024 >./log/file_init.1024
	time ./file_run 1024 1024 >./log/file_run.1024
# create 1 files, and write 1024 * 1024 pages
	./file_init 1 1048576 >./log/file_init.1
	time ./file_run 1 1048576 >./log/file_run.1

clean:
	@rm -rf *.o file_init file_run out log
