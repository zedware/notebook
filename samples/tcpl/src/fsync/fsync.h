#ifndef MY_FSYNC_H
#define MY_FSYNC_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define FSYNC_FILENAME	"/var/tmp/fsync.out"

/* O_SYNC and O_FSYNC are the same */
#if defined(O_SYNC)
#define OPEN_SYNC_FLAG		O_SYNC
#elif defined(O_FSYNC)
#define OPEN_SYNC_FLAG		O_FSYNC
#endif

#if defined(O_DSYNC) 
#define OPEN_DATASYNC_FLAG	O_DSYNC
#endif

#define MY_FILE_SIZE		(128 * 1024 * 1024)

extern void	die(char *str);
extern void	print_elapse(struct timeval start_t, struct timeval elapse_t);

#endif /* MY_FSYNC_H */
