/*
 * Implement the function getline().
 * Read one line characters into s[].
 */

#include "getline.h"

/*
 * parameters:
 * 	char s[]: one line characters;
 * 	int len:  length of s[].
 * return:
 *	length of the line; 
 */ 
int getline(char s[], int len)
{
	int c, i;

	assert(len > 0);

	for (i = 0; i < len - 1; i++)
	{
		c = getchar();
		if (c == EOF || c == '\n')
			break;
	   	s[i] = c;
	}

	s[i] = '\0';	
	return i;
}

