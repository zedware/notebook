
#include "process_thread.h"

static void *start_routine(void *args);

int get_args(int argc, char *argv[], int *ncount)
{
	if (argc < 2)
	{
		printf("usage:\n");
		printf("    cmd <nchilds>\n");
		return -1;
	}

	*ncount = atoi(argv[1]);
	return 0;
}

int create_process(int ncount)
{
	int		i;

	for (i = 0; i < ncount; i++)
	{
		int 	pid;

		pid = fork();
		if (pid == 0)
		{
			printf("child\n");
			return 0;
		}
		else if (pid > 0)
		{
		}
		else
		{
			fprintf(stderr, "fork failed, %d forked.\n", i);
			return -1;
		}
	}
}

int create_thread(int ncount)
{
	int		i;

	for (i = 0; i < ncount; i++)
	{
		pthread_t	pt;
		int			tid;

		tid = pthread_create(&pt, NULL, start_routine, NULL);
		if (tid < 0)
		{
			fprintf(stderr, "fork child failed, %d forked.\n", i);
			return -1;
		}
	}

	return 0;
}

static void *start_routine(void *args)
{
	/* do nothing */
}

