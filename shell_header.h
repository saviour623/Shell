#ifndef SHELL_HEADER_H
#define SHELL_HEADER_H

#define _GNU_SOURCE
#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include "shell_header.h"

#ifdef __UNUSED__
#undef __UNUSED__
#define __UNUSED__ __attribute__((unused))
#else
#define __UNUSED__ __attribute__((unused))
#endif

#define ERRNULL 0
#define ERRMSG 1
#define ERRSTR 2

static int interactive_mode(int argc, char **argv);
int cn_token(char *t, int *cnt);
int cnt_token(const char *__restrict__ st, const char *__restrict__ del);
int strtok_sh(const char *__restrict__ s, const char *__restrict__ del, char **vec, size_t vecsize);

#endif
