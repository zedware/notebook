
#include "fsync.h"

void print_elapse(struct timeval start_t, struct timeval elapse_t)
{
	if (elapse_t.tv_usec < start_t.tv_usec)
	{
		elapse_t.tv_sec--;
		elapse_t.tv_usec += 1000000;
	}

	printf("%3ld.%06ld", (long) (elapse_t.tv_sec - start_t.tv_sec),
		   (long) (elapse_t.tv_usec - start_t.tv_usec));
}

void die(char *str)
{
	fprintf(stderr, "%s\n", str);
	exit(1);
}

