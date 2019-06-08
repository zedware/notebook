
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "file.h"

int get_fileargs(int argc, char *argv[], int *nfiles, int *npages)
{
	if (argc < 3)
		return -1;

	*nfiles = atoi(argv[1]);
	*npages = atoi(argv[2]);

	return 0;
}

void get_filename(char filename[], int id)
{
	snprintf(filename, MY_MAX_PATH, "%s%d", MY_PREFIX, id);
}

int write_file(int fd, int npages)
{
	int		i;
	char 	buf[MY_PAGESIZE];

	for (i = 0; i < npages; i++)
	{
		if (write(fd, buf, MY_PAGESIZE) != MY_PAGESIZE)
		{
			printf("write failed.\n");
			return -1;
		}
	}

	return 0;
}

void usage(void)
{
	printf("usage:\n");
	printf("    cmd <nfiles> <npages>\n");
}

