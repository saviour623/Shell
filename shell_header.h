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
#include <stdint.h>
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

#define	ERR_SHLL_NOENT 0
#define	ERR_SHLL_PERM 1
#define	ERR_SHLL_PROC 2
#define	ERR_SHLL_RTU 3
#define	ERR_SHLL_OMEM 4
#define	ERR_SHLL_EOF 5
#define ERR_SHLL_EAGAIN 6
#define ERR_SHLL_INVNUM 7
#define ERR_SHLL_INVOPT 8

extern char **environ;

#define GLN_MALLOC(nptr, optr, block)									\
	((nptr = glnrealloc(optr, sizeof(unsigned char) * block)) != NULL)

#define MAX_ENVPATH_LEN 4096UL << 5
#define SIG_ARGS_MAX 6
#define restrict __restrict__

typedef struct built_ins built_ins;
typedef struct shell_info shell_info;
typedef struct cmd_alias cmd_alias;

struct shell_info
{
	char *shell_name;
	char **argv;
	char *cmd;
	char **cmd_opt;
	built_ins *path;
	cmd_alias *alias;
	size_t cmd_cnt;
	int argc;
	int status;
	bool err_info;
	bool cmd_sep;
};

struct built_ins
{
	char *builtin_name;
	void (*builtin_exe)(struct shell_info *);
};

struct cmd_alias
{
	char *name;
	char *path;
	struct cmd_alias *nxt_alias;
};

void eRR_routine(long err) __attribute__((noreturn));
void errMsg(int errnum, struct shell_info *procinfo);

/* utils */
size_t _nputs(int fd, const char *str, int newline);
ssize_t str_cpy(char *__restrict__ dest, const char *__restrict__ src, size_t len);
int _natoi(const char *s);

void *glnrealloc(void *oldmem, size_t size);
int interactive_mode(shell_info *sh_info);
int getNumtoks(const char *__restrict__, const char *__restrict__);
char **getcmdString(char *__restrict__);
int delimCharcmp(const char *__restrict__ delim, const char *__restrict__ cmp);
ssize_t stdin_getline(char **lineptr, size_t *n);
int stdin_getchar(void);

void execteArg(shell_info *sh_info);
char *path(shell_info *sh_info);
char *search_path(char *__restrict__ env_path, char *cmd, int *status);
char *search_alias(char *__restrict__ cmd, cmd_alias *__alias);

/* builtin functions */
void exit_shell_func(struct shell_info *);
void cd_directory_func(struct shell_info *);
void env_func(struct shell_info *);
void set_environ_func(struct shell_info *);
void unset_environ_func(struct shell_info *);

/* alias */
void destroy_alias(cmd_alias *alias);
#endif
