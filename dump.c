#include "shell_header.h"
int strtok_sh(const char *__restrict__ s, const char *__restrict__ del, char **vec, size_t vecsize);

//used standard library: realloc; strtok; setjump, longjump, strlen

int strtok_sh(const char *__restrict__ s, const char *__restrict__ del, char **vec, size_t vecsize)
{
	char c, d; /* temp */
	register int ee;

	if (s == NULL || del == NULL || vec == NULL)
	{
		errno = EINVAL;
		return -1;
	}

	if (*s == '\0' || *del == '\0')
		return 0;

	d = *del;
	ee = 0;

	for (; (c = *s); *s++)
	{
		if (vecsize < 0)
			return 0;

		if (c == d)
			continue;
		else if (c != d)
		{
			for (; ((c = *s) != d) && (c != '\0'); *s++, ee++)
				(*vec)[ee] = c;
			ee = 0;
			vec++;
			vecsize--;
		}
	}

	return 0;
}

int main(void)
{
	char **s = malloc(sizeof(char *) * 12);
	char *p = "happy birthday my friend";
	int oo = 0;

	while (oo < 12)
		s[oo++] = malloc(sizeof(char) * 12);

	strtok_sh(p, " ", s, 127);
	oo = 0;
	while (oo < 6)
	{
		puts(s[oo]);
		oo++;
	}
}
