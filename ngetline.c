#include "shell_header.h"

#define MAX_CNT SSIZE_MAX
#define MIN_ALLOC_SIZE 128
#define RESIZE_ALLOC_SIZE 256

#define I_OVERFLOW_MAX_CNT(n)									\
	((n) > MAX_CNT ? ((errno = EOVERFLOW), true) : false)
/**
 * stdin_getchar - reads a character byte from standard input.
 * Return: -1 on error, 0 if EOF, else character read.
 */
int stdin_getchar(void)
{
	static char c __attribute__((unused));
	int oo = 0;

	return ((oo = read(0, &c, 1)) < 0 ? -1 : oo == 0 ? 0 : c);
}

/**
 * stdin_getline - reads from standard input until new line.
 * @lineptr: stores read text.
 * @n: number of bytes to read (increased if less than actual bytes read).
 * Return: number of bytes read. if error or EOF, returns -1.
 * for the case of EOF, returns -1 only if line is empty.
 */
ssize_t stdin_getline(char **lineptr, size_t *n)
{
	register int c = 0;
	size_t cnt = 0;
	char *new_lineptr = NULL;

	if (lineptr == NULL || n == NULL)
	{
		errno = EINVAL;
		return (-1);
	}

	if (*n == 0)
		*n = MIN_ALLOC_SIZE;

	if (*lineptr == NULL)
	{
		if (!GLN_MALLOC(*lineptr, NULL, MIN_ALLOC_SIZE))
			return (-1);
	}

	while ((c = stdin_getchar()) != '\n')
	{
		if (I_OVERFLOW_MAX_CNT(*n))
			return (-1);

		if ((c == 0 && !cnt) || (c == -1))
			return (-1);

		if (cnt > *n)
		{
			if (!GLN_MALLOC(new_lineptr, *lineptr, RESIZE_ALLOC_SIZE))
				return (-1);

			*n += RESIZE_ALLOC_SIZE;
			*lineptr = new_lineptr;
		}
		(void)(c != 0 ? (((*lineptr)[cnt] = c), (cnt += 1)) : 0);
	}

	(*lineptr)[cnt] = '\0';
	return (cnt);
}
