
all: fsync

fsync: fsync.o fsync_lib.o

test: all
	./fsync 

clean:
	@rm -f *.o fsync
	@rm -f log core

