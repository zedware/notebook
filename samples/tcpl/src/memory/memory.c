
#include <stdio.h>
#include <stdlib.h>

static void usage(void);

int main(int argc, char *argv[])
{
	int	nbytes;
	int i;
	char *p;

	if (argc != 2)
	{	
		usage();
		return 1;
	}

	nbytes = atoi(argv[1]);
	if (nbytes <= 0)
		return 1;

	for ( ; ; )
	{
		p = (char *) malloc(nbytes);
		if (p == NULL)
		{
			printf("malloc failed.\n");
			break;
		}

		for (i = 0; i < nbytes; i++)
			*p = (char) i;
	}
	
	/* free memory? */
	while (1)
		;

	return 0;
}

static void usage(void)
{
	printf("cmd <nbytes>\n");
}

