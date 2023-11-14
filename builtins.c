#include "shell_header.h"

void exit_shell_func(struct shell_info *sh_info __UNUSED__)
{
	int exit_status = EXIT_SUCCESS;

	destroy_alias(sh_info->alias);
	(void)(sh_info->cmd_opt[1] != NULL ?
		   (exit_status = _natoi(sh_info->cmd_opt[1])) : 0);

	if (exit_status == -1)
	{
		sh_info->err_info = true;
		errMsg(ERR_SHLL_INVNUM, sh_info);
		return;
	}
	free(sh_info->cmd);
	free(sh_info->cmd_opt);
	exit(exit_status);
}

int _natoi(const char *s)
{
	register unsigned int n = 0, c = 0, sgn = 1;
	
	if ((s == NULL) || (*s == 0))
		return (0);

	if (*s == '-')
		sgn = -1;

	for (; (c = *s) != 0; (void)*s++)
	{
		if (!(c > 47 && c < 58))
		{
			n = -1;
			sgn = 1;
			break;
		}
		n = ((n << 3) + (n << 1)) + (c - 48);
	}
	return (sgn * (int)n);
	
}
void destroy_alias(cmd_alias *alias)
{
	cmd_alias *n;

	if (alias == NULL)
		return;
	while (alias != NULL)
	{
		n = alias;
		alias = alias->nxt_alias;
		free(n);
	}
	n = alias = NULL;
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
	return NULL;
}
void set_environ_func(struct shell_info *info __UNUSED__)
{}
void unset_environ_func(struct shell_info *info __UNUSED__)
{}
void cd_directory_func(struct shell_info *info __UNUSED__)
{
}
