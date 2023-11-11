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
 * str_cpy - copies len bytes from src to dest.
 * @dest: destination buffemr.
 * @src: source.
 * @len: len bytes to copy.
 * Return: number of bytes copied. If while reading dest gets to its '\0', it is returned with a partial copy.
 */
ssize_t str_cpy(char *__restrict__ dest, const char *__restrict__ src, size_t len)
{
	register size_t c, oo;

	if (dest == NULL || src == NULL)
		return (-1);

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
