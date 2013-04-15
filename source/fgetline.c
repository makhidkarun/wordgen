#include <stdio.h>

fgetline(s, lim, fptr)
char s[];
int lim;
FILE *fptr;
{
	int c, i;

	i = 0;
	while (--lim > 0 && (c=getc(fptr)) != EOF && c != '\n')
		s[i++] = c;
	if (c == '\n')
		s[i++] = c;
	s[i] = '\0';
	return(i);
}
