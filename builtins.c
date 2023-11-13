#include "shell_header.h"

void exit_shell_func(struct shell_info *info __UNUSED__)
{
	exit(EXIT_SUCCESS);
}

void env_func(struct shell_info *info __UNUSED__)
{
	register char **env = environ;

	if (env == NULL)
		eRR_routine(0);

	fflush(stdout);

	for (; *env != NULL; env++)
	{
		write(STDOUT_FILENO, *env, strlen(*env));
		write(STDOUT_FILENO, "\n", 1);
	}
}
char *get_env(char *restrict envpath __UNUSED__)
{
}
void set_environ_func(struct shell_info *info __UNUSED__)
{}
void unset_environ_func(struct shell_info *info __UNUSED__)
{}
void cd_directory_func(struct shell_info *info __UNUSED__)
{
}
