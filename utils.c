#include "shell_header.h"

/**
 * glnrealloc - reallocates memmory.
 * @oldmem: old memory.
 * @size: block to reallocate.
 * Return: sets errno and returns NULL error else returns new block of memory
 */
void *glnrealloc(void *oldmem, size_t size)
{
	void *newmem =  realloc(oldmem, size);

	if (newmem == NULL)
	{
		errno = ENOMEM;
		return (NULL);
	}
	return (newmem);
}

/**
 * str_cpy - copies @len bytes from src to dest.
 * str_cpy expects a null terminated string to copy else its behaviour is undefined .
 * @dest: destination buffemr.
 * @src: source.
 * @len: length of bytes to copy - however, it may depend of on the length of
 * the src string. Therefore, larger length can be supplied only if @src is
 * null terminated.
 * Return: number of bytes copied.
 * copy can be partial if while reading @dest or @src gets to its '\0' byte.
 */
ssize_t str_cpy(char *restrict dest, const char * restrict src, size_t len)
{
	register size_t c, oo;

	if (dest == NULL || src == NULL)
		return (-1);
	if (len == 0)
		len = SIZE_MAX; /* if len is zero lets rely on the null byte to end copy */

	for (oo = 0; (c = src[oo]) != 0 && (oo < len); oo++)
	{
		*dest++ = (char)c;

		if (dest == 0)
			goto ret;
	}
	*dest = '\0';
ret:
	return (oo);
}

/**
 * delimCharcmp - check if a char is a delimiter.
 * @delim: delimiter
 * @cmp: character
 * Return: true if it a delimiter, else false
 */
int delimCharcmp(const char *restrict delim, const char *restrict cmp)
{
	char c __UNUSED__;

	if (delim == NULL || cmp == NULL)
		return (-1);

	if (delim[0] != 0 && delim[1] == 0)
		return *delim == *cmp;

	for (; (c = *delim); delim++)
	{
		if (c == *cmp)
			return (true);
	}
	return (false);
}

/**
 * _nputs - writes @str to file
 * @fd: file descriptor
 * @str: bytes to write
 * @newline: if set, appends a newline after writing to file
 */
size_t _nputs(int fd, const char *str, int newline)
{
	register size_t len;

	if (str == NULL)
		return (-1);

	if (*str == 0)
		return (0);

	len = strlen(str);

	write(fd, str, len);

	if (newline)
	{
		write(fd, "\n", 1);
		len++;
	}
	return (len);
}
