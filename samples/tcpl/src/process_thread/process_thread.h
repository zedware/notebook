
#ifndef MY_PROCESS_THREAD_H
#define MY_PROCESS_THREAD_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <pthread.h>

extern int get_args(int argc, char *argv[], int *ncount);
extern void usage(void);
extern int create_process(int ncount);
extern int create_thread(int ncount);

#endif /* */
