
#ifndef MY_FILE_H
#define MY_FILE_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define MY_MAX_PATH		1024
#define MY_MASK			0666
#define MY_PREFIX		"./out/file"
#define MY_PAGESIZE		1024

extern void get_filename(char filename[], int id);
extern int write_file(int fd, int npages);
extern int get_fileargs(int argc, char *argv[], int *nfiles, int *npages); 
extern void usage(void);

#endif /* MY_FILE_H */
