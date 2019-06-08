
#include "file.h"

int main(int argc, char *argv[])
{
	char	buf[MY_PAGESIZE];
	char	filename[MY_MAX_PATH] = "";
	int		i;
	int		nfiles, npages;

	if (get_fileargs(argc, argv, &nfiles, &npages) < 0)
	{
		usage();
		exit(1);
	}

	/*
	 * init: create new files.
	 */	
	for (i = 0; i < nfiles; i++)
	{
		int		fd;

		get_filename(filename, i);
	
		if ((fd = open(filename, O_RDWR, MY_MASK)) < 0)
		{
			printf("open \"%s\" failed.\n", filename);
			exit(1);
		}
		
		if (write_file(fd, npages) < 0)
		{
			printf("write_file \"%s\" failed.\n", filename);
			exit(1);
		}

		close(fd);	
	}

	exit(0);
}

