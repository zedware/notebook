
#include "process_thread.h"

int main(int argc, char *argv[])
{
	int	ncount;

	if (get_args(argc, argv, &ncount) < 0)
		exit(1);

	if (create_thread(ncount) < 0)
		exit(1);

	exit(0);
}

