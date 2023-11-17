#include "shell_header.h"
char *_nstrtok(char *__restrict__ s, const char *__restrict__ delim);

char *_nstrtok(char *__restrict__ s, const char *__restrict__ delim)
{
	static char *nxtptr;
	char *curptr, c;
	bool cmp = false;
	int oo = 0;
	int start = 0, stop = 0;
	if (delim == NULL)
	{
		errno = EINVAL;
		return (NULL);
	}

	if (s == NULL)
		s = nxtptr;

	if (*s == '\0' || *delim == '\0')
		return (NULL);

	for (; *s == ' ' && *s != 0; (void) *s++)
		;
//	puts(s);
	curptr = s;
	for (; *s; s++)
	{
		if (' ' != *s)
			s++;
	}

//	if (*s != 0)
		*s++ = 0;
		//puts(curptr);
	nxtptr = s;
	return curptr;
}

#include <time.h>
int main(void)
{
	char *s;
	char p[] = "happy birth\tday my friend happy birth\tday my friend";
	char *g = p;
	int oo = 0;
	char k[] = "happy birth\tday my friend happy birth\tday my friend";

	clock_t y = clock();
	s = strtok(k, " ");
	while ((s = strtok(NULL, " ")) != NULL) puts(s);
	y = clock() - y;

	puts("\n\n\n");
	clock_t x = clock();
	s = _nstrtok(p, " ");
	while ((s = _nstrtok(NULL, " ")) != NULL) puts(s);
	x = clock() - x;
	

	printf("%f %f\n", x/(double)CLOCKS_PER_SEC, y/(double)CLOCKS_PER_SEC);
///	puts(g);
	//puts(_nstrtok(NULL, "
//	printf("%d\n", oo);

//	strtok(NULL, " ");
}
