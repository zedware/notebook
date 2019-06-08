/*
 Exercise 1-21. Write a program entab that replaces strings of blanks
 by the minimum number of tabs and blanks to achieve the same spacing. 
 Use the same tab stops as for detab. When either a tab or a single 
 blank would suffice to reach a tab stop, which should be given
 preference?
*/

#include <stdio.h>

#define TABSTOP		8

int main(void)
{
	int	c;
	int ncol = 0;
	int nspace = 0;

	while ((c = getchar()) != EOF)
	{
		switch (c)
		{
		case ' ':
			ncol++;
			nspace++;
			if (ncol % TABSTOP == 0)
			{
				putchar('\t');
				nspace = 0;
			}
			break;
		
		case '\t':
			nspace = 0;
			ncol += TABSTOP - (ncol - 1) % TABSTOP;
			putchar('\t');
			break;

		case '\n':
			for ( ; nspace > 0; nspace--)
			{
				putchar(' ');
				ncol++;
			}
			nspace = 0;
			ncol = 0;
			break;

		default:
			for ( ; nspace > 0; nspace--)
			{
				putchar(' ');
				ncol++;
			}
			ncol++;
			putchar(c);
			break;
		}
	}

	return 0;
}

