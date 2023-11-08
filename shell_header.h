#ifndef SHELL_HEADER_H
#define SHELL_HEADER_H

#define _GNU_SOURCE
#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

static int interactive_mode(int argc, char **argv);
int getNumtoks(const char *__restrict__, const char *__restrict__);
char **getcmdString(char *__restrict__);
int delimCharcmp(const char *__restrict__ delim, const char *__restrict__ cmp);
void execteArg(char **cmd);
#endif
