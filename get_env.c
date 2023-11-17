#include "shell_header.h"
int _nstrcmp(const char *__restrict__ c1, const char *__restrict__ c2, const char *delim);

__attribute__((nonnull)) char *_ngetenv(const char *_env)
{
	register int oo = 0, ee = 0;

	if (_env == NULL || *_env == 0)
		return (NULL);
	for (; environ[oo] != NULL; oo++)
	{
		ee = _nstrcmp(environ[oo], _env, "=");
		if (ee >= 0)
			break;
	}
	if (ee < 0)
		return (NULL);
	ee += 1; /* + 1 to remove the delimiter */
	return (*(environ + oo) + ee);
}

int _nsetenv(char *name, char *value, int replce)
{
	static char ssenv[4096];
	register int oo, ee;

	if (name == NULL || *name == 0 || value)
	{
		errno = EINVAL;
		return -1;
	}
	for (; environ[oo] != NULL; oo++)
	{
		ee = _nstrcmp(environ[oo], name, "=");
		if (ee >= 0)
			break;
	}
	if (environ[oo] != NULL && !replce)
		goto ret;
ret:
	return (0);
}
int _nstrcmp(const char *restrict c1, const char *restrict c2, const char *delim)
{
	register int oo, b, c, d, e, ret;

	if (c1 == NULL || c2 == NULL)
		return (-3);

	if (c1 == 0 || c2 == 0)
		return (0);

	if (delim == NULL)
		delim = "";

	oo = ret = 0;
	d = *delim;

	for (; ((c = c1[oo]) != 0) && (d == 0 || c != d); oo++, ret++)
	{
		b = c2[oo];

		if (b == c)
			continue;
		if (b == 0 || b == d)
			ret = -1;
		break;
	}
	if (c2[oo] != 0 && c2[oo] != d)
		ret = -2;
	return (ret);
}

#include <time.h>
#define AB CLOCKS_PER_SEC
int main(void)
{
	_nsetenv("OLDPWD", "/rot", 0);
}
/**
	clock_t k = clock();
	b = getenv("");
	k = clock() - k;

		clock_t p = clock();
	a = _ngetenv("");
	p = clock() - p;

	printf("%s\n%s\n", a, b);
	printf("%f - %f\n", p/(double)AB, k/(double)AB); 
*/
/**
	ee = str_cpy(ssenv, name, 0);

	if (ssenv[ee - 1] != '=')
		ssenv[ee++] = '='; /* let's permit '=' */
/*	ee += str_cpy((ssenv + ee), value, 0);

	environ[oo] ? (environ[oo] = ssenv) :
		((environ[oo++] = ssenv), (environ[oo] = NULL));
*/
