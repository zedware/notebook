
LIBS += -lpthread

all: process_run thread_run

process_run: process_thread_lib.o process_run.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(LIBS) -o $@

thread_run: process_thread_lib.o thread_run.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(LIBS) -o $@

test: all
# create dir
	@rm -rf log
	@mkdir log
	time ./process_run 6000 >./log/process_run
	time ./thread_run 6000 >./log/thread_run

clean:
	@rm -rf process_run thread_run *.o log
