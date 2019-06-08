
/*
 * Write a file with size 128M, 100 loops by default.
 */

#include "fsync.h"

int main(int argc, char *argv[])
{
	struct timeval start_t;
	struct timeval elapse_t;
	int	   tmpfile,
		   i,
		   loops = 100;
	unsigned char *strout;
	char  *filename = FSYNC_FILENAME;

	if (argc > 2 && strcmp(argv[1], "-f") == 0)
	{
		filename = argv[2];
		argv += 2;
		argc -= 2;
	}

	if (argc > 1)
		loops = atoi(argv[1]);

	strout = (unsigned char *) malloc(MY_FILE_SIZE);
	if (strout == NULL)
		return 1;

	for (i = 0; i < MY_FILE_SIZE; i++)
		strout[i] = (unsigned char) (i % 256);

	if ((tmpfile = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
		die("Cannot open output file.");

	write(tmpfile, strout, MY_FILE_SIZE);
	fsync(tmpfile);	/* fsync so later fsync's don't have to do it */
	close(tmpfile);

	printf("Simple write timing:\n");
	/* write only */
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
	{
		if ((tmpfile = open(filename, O_RDWR)) == -1)
			die("Cannot open output file.");
		write(tmpfile, strout, 8192);
		close(tmpfile);
	}

	gettimeofday(&elapse_t, NULL);
	printf("\twrite                  ");
	print_elapse(start_t, elapse_t);
	printf("\n");

	printf("\nCompare fsync times on write() and non-write() descriptor:\n");
	printf("(If the times are similar, fsync() can sync data written\n on a different descriptor.)\n");

	/* write, fsync, close */
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
	{
		if ((tmpfile = open(filename, O_RDWR)) == -1)
			die("Cannot open output file.");
		write(tmpfile, strout, 8192);
		fsync(tmpfile);
		close(tmpfile);
		if ((tmpfile = open(filename, O_RDWR)) == -1)
			die("Cannot open output file.");
		/* do nothing but the open/close the tests are consistent. */
		close(tmpfile);
	}

	gettimeofday(&elapse_t, NULL);
	printf("\twrite, fsync, close    ");
	print_elapse(start_t, elapse_t);
	printf("\n");

	/* write, close, fsync */
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
	{
		if ((tmpfile = open(filename, O_RDWR)) == -1)
			die("Cannot open output file.");
		write(tmpfile, strout, 8192);
		close(tmpfile);
		/* reopen file */
		if ((tmpfile = open(filename, O_RDWR)) == -1)
			die("Cannot open output file.");
		fsync(tmpfile);
		close(tmpfile);
	}

	gettimeofday(&elapse_t, NULL);
	printf("\twrite, close, fsync    ");
	print_elapse(start_t, elapse_t);
	printf("\n");

	printf("\nCompare one o_sync write to two:\n");
	
	/* 16k o_sync write */
	if ((tmpfile = open(filename, O_RDWR | OPEN_SYNC_FLAG)) == -1)
		die("Cannot open output file.");
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
		write(tmpfile, strout, 16384);
	gettimeofday(&elapse_t, NULL);
	close(tmpfile);
	printf("\tone 16k o_sync write   ");
	print_elapse(start_t, elapse_t);
	printf("\n");

	/* 2*8k o_sync writes */
	if ((tmpfile = open(filename, O_RDWR | OPEN_SYNC_FLAG)) == -1)
		die("Cannot open output file.");
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
	{
		write(tmpfile, strout, 8192);
		write(tmpfile, strout, 8192);
	}

	gettimeofday(&elapse_t, NULL);
	close(tmpfile);
	printf("\ttwo 8k o_sync writes   ");
	print_elapse(start_t, elapse_t);
	printf("\n");

	printf("\nCompare file sync methods with one 8k write:\n");

#ifdef OPEN_DATASYNC_FLAG
	/* open_dsync, write */
	if ((tmpfile = open(filename, O_RDWR | O_DSYNC)) == -1)
		die("Cannot open output file.");
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
		write(tmpfile, strout, 8192);
	gettimeofday(&elapse_t, NULL);
	close(tmpfile);
	printf("\topen o_dsync, write    ");
	print_elapse(start_t, elapse_t);
#else
	printf("\t(o_dsync unavailable)  ");
#endif	
	printf("\n");

	/* open_fsync, write */
	if ((tmpfile = open(filename, O_RDWR | OPEN_SYNC_FLAG)) == -1)
		die("Cannot open output file.");
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
		write(tmpfile, strout, 8192);
	gettimeofday(&elapse_t, NULL);
	close(tmpfile);
	printf("\topen o_sync, write     ");
	print_elapse(start_t, elapse_t);
	printf("\n");
	
	/* write, fdatasync */
	if ((tmpfile = open(filename, O_RDWR)) == -1)
		die("Cannot open output file.");
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
	{
		write(tmpfile, strout, 8192);
		fdatasync(tmpfile);
	}
	gettimeofday(&elapse_t, NULL);
	close(tmpfile);
	printf("\twrite, fdatasync       ");
	print_elapse(start_t, elapse_t);
	printf("\n");

	/* write, fsync, close */
	if ((tmpfile = open(filename, O_RDWR)) == -1)
		die("Cannot open output file.");
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
	{
		write(tmpfile, strout, 8192);
		fsync(tmpfile);
	}
	gettimeofday(&elapse_t, NULL);
	close(tmpfile);
	printf("\twrite, fsync,          ");
	print_elapse(start_t, elapse_t);
	printf("\n");

	printf("\nCompare file sync methods with 2 8k writes:\n");

#ifdef OPEN_DATASYNC_FLAG
	/* open_dsync, write */
	if ((tmpfile = open(filename, O_RDWR | O_DSYNC)) == -1)
		die("Cannot open output file.");
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
	{
		write(tmpfile, strout, 8192);
		write(tmpfile, strout, 8192);
	}
	gettimeofday(&elapse_t, NULL);
	close(tmpfile);
	printf("\topen o_dsync, write    ");
	print_elapse(start_t, elapse_t);
#else
	printf("\t(o_dsync unavailable)  ");
#endif
	printf("\n");

	/* open_fsync, write */
	if ((tmpfile = open(filename, O_RDWR | OPEN_SYNC_FLAG)) == -1)
		die("Cannot open output file.");
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
	{
		write(tmpfile, strout, 8192);
		write(tmpfile, strout, 8192);
	}

	gettimeofday(&elapse_t, NULL);
	close(tmpfile);
	printf("\topen o_sync, write     ");
	print_elapse(start_t, elapse_t);
	printf("\n");

	/* write, fdatasync */
	if ((tmpfile = open(filename, O_RDWR)) == -1)
		die("Cannot open output file.");
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
	{
		write(tmpfile, strout, 8192);
		write(tmpfile, strout, 8192);
		fdatasync(tmpfile);
	}
	gettimeofday(&elapse_t, NULL);
	close(tmpfile);
	printf("\twrite, fdatasync       ");
	print_elapse(start_t, elapse_t);
	printf("\n");

	/* write, fsync, close */
	if ((tmpfile = open(filename, O_RDWR)) == -1)
		die("Cannot open output file.");
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
	{
		write(tmpfile, strout, 8192);
		write(tmpfile, strout, 8192);
		fsync(tmpfile);
	}
	gettimeofday(&elapse_t, NULL);
	close(tmpfile);
	printf("\twrite, fsync,          ");
	print_elapse(start_t, elapse_t);
	printf("\n");

	printf("\nCompare o_sync write with O_DIRECT to without O_DIRECT:\n");	

	/* o_sync write without o_direct */
	if ((tmpfile = open(filename, O_RDWR | OPEN_SYNC_FLAG)) == -1)
		die("Cannot open output file.");
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
		write(tmpfile, strout, 8192);
	gettimeofday(&elapse_t, NULL);
	close(tmpfile);
	printf("\to_sync without o_direct");
	print_elapse(start_t, elapse_t);
	printf("\n");

	/* o_sync write with o_direct */
#ifdef O_DIRECT
	if ((tmpfile = open(filename, O_RDWR | OPEN_SYNC_FLAG | O_DIRECT)) == -1)
		die("Cannot open output file.");
	gettimeofday(&start_t, NULL);
	for (i = 0; i < loops; i++)
		write(tmpfile, strout, 8192);
	gettimeofday(&elapse_t, NULL);
	close(tmpfile);
	printf("\to_sync with o_direct   ");
	print_elapse(start_t, elapse_t);
#else
	printf("\t(O_DIRECT unavailable)  ");
#endif
	printf("\n");

	unlink(filename);
	return 0;
}

