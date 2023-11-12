#ifndef SHELL_HEADER_H
#define SHELL_HEADER_H

#define _GNU_SOURCE
#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

#ifdef __UNUSED__
#undef __UNUSED__
#define __UNUSED__ __attribute__((unused))
#else
#define __UNUSED__ __attribute__((unused))
#endif

#define ERRNULL 0
#define ERRMSG 1
#define ERRSTR 2
#define ERRMEM 3

extern char **environ;

#define GLN_MALLOC(nptr, optr, block)									\
	((nptr = glnrealloc(optr, sizeof(unsigned char) * block)) != NULL)

#define MAX_ENVPATH_LEN 4096UL << 5
#define restrict __restrict__

void *glnrealloc(void *oldmem, size_t size);
int interactive_mode(int argc, char **argv);
int getNumtoks(const char *__restrict__, const char *__restrict__);
char **getcmdString(char *__restrict__);
int delimCharcmp(const char *__restrict__ delim, const char *__restrict__ cmp);
ssize_t stdin_getline(char **lineptr, size_t *n);
int stdin_getchar(void);
ssize_t str_cpy(char *__restrict__ dest, const char *__restrict__ src, size_t len);
void execteArg(char **cmd);
char *path(char *, int *);
char *search_path(char *__restrict__ env_path, char *cmd, int *status);
#endif
