#include "shell_header.h"

__attribute__((nonnull)) char *_ngetenv(const char *_env)
{
	register int oo = 0, ee = 0;

	if (*_env == 0)
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
	static char ssenv[4096] __UNUSED__;
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
/**
 * _nstrcmp - compares two string until a specified single byte delimiter.
 * @c1: string 1
 * @c2: string 2
 * @delim: single byte delimiter. If not provide it assumes a null byte.
 * Return: -3 (invalid arguments) - 0 (if either c1 or c2 is empty -
 * -1 (c1 > c2), -2 (c2 > c1) - number of bytes read (c1 == c2).
 */  
int _nstrcmp(const char *restrict c1, const char *restrict c2, const char *delim)
{
	register int oo, b /* c2 */, c /* c1 */, d /* delim */, ret;

	if (c1 == NULL || c2 == NULL)
		return (-3);

	if (c1 == 0 || c2 == 0)
		return (0);

	if (delim == NULL)
		delim = "";

	oo = ret = 0;
	d = *delim;

	/* loop if c1 is non-null and c1 also not the delimiter byte or the delimiter is a null byte (this is the default case when a delimiter is not given */
	for (; ((c = c1[oo]) != 0) && (d == 0 || c != d); oo++, ret++)
	{
		b = c2[oo];

		if (b == c)
			continue;
		/* if c2 reaches it's end before c1 does then c2 is lesser than c1 */
		if (b == 0 || b == d)
			ret = -1;
		break;
	}
	/* if c2 is non-null or isn't the delimiter then it's indeed greater */
	if (c2[oo] != 0 && c2[oo] != d)
		ret = -2;
	return (ret);
}
