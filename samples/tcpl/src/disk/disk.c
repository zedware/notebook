
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define MAX_PATH		1024

static void usage(void);

int main(int argc, char *argv[])
{
	int	nbytes;
	int i;
	
	if (argc != 2)
	{
		usage();
		return 1;
	}

    for (i = 0; ;i++)
	{
		char filename[MAX_PATH];
		char buf[1];
		int  j;
		FILE *fp;

		sprintf(filename, "file%d", i);
		fp = fopen(filename, "w+");
		if (fp == NULL)
			break;

		for (j = 0; j < nbytes; j++)
		{
			if (fwrite(buf, 1, 1, fp) != 1)
			{
				printf("write failed: %d\n", errno);
				fclose(fp);
				goto error_exit;
			}
		}
	
		fclose(fp);
	}	

error_exit:

	return 0;
}

static void usage(void)
{
	printf("cmd <nbytes>\n");
}

