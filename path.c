#include "shell_header.h"

#define ACCESS_CMD(cmd, mode, stat)		\
	(!((stat) = access((cmd), (mode))))

typedef struct built_ins
{
	char *builtin_name;
	void (*builtin_exe)(char *, int *status, struct shell_info);
} built_in_name;

typedef struct cmd_alias
{
	char *alias;
	char *path;
} cmd_alias;

typedef shell_info
{
	char *cmd;
	char **cmd_opt;
	bool cmd_sep;
	built_ins *cmd_bltn;
};

/**
 * path - finds the path and existence of a file from the PATH environment.
 * @cmd: file
 * @status: if -1, error - 0, file is found and is an executable -
 * 1, file is a path - 2 file is not executable -
 * 3, ordinary file - 4, file is a directory.
 * Return: NULL on error, else the absolute path of the file.
 */
#define BUILT_IN_TABLE(...)\
	{\
		{"exit", exit_shell_func},				\
		{"cd", cd_directory_func},						\
		{"env", env_func},\
		{"setenv", set_environ_func},\
		{"unsetenv", unset_environ_func},\
		{NULL, NULL}\
	}
char *path(char *cmd, int *status)
{
	char *env_path;
	char cpy_path[MAX_ENVPATH_LEN]; /* this is faster than DMA because we can skip finding path length when it is large */
	char *ptr;

	built_ins builtin_func[] = BUILT_IN_TABLE(NULL);

	while ((ptr = (builtin_func[oo]).builtin_name) != NULL)
	{
		if (strcmp(ptr, cmd))
		{
			*status = (builtin_func[oo]).builtin_exe(cmd); //cmd, arguments env
			if (status == -1)
				eRR_routine(0);
			return (NULL);
		}
	}
	
	env_path = getenv("PATH");
	*status = -1;

	if ((env_path == NULL) || (cmd == NULL || *cmd == 0) || (status == NULL))
		return (NULL);

	str_cpy(cpy_path, env_path, MAX_ENVPATH_LEN);

	env_path = search_path(cpy_path, cmd, status);
	return (env_path);
}

/**
 * search_path - searches for executive permission of file.
 * @cmd: file
 * @env_path: environment path
 * @status: -1 on error, 0, if found, 1, if already a path, not exe, 2 if not in path but no error.
 * Return: returns the path appended to file is successful else NULL.
 */
char *search_path(char *__restrict__ env_path, char *cmd, int *status)
{
	register int cmdLen, is_path, ee;
	char *tmp, *abs_path;

	*status = -1; /* initially set to error */

	if (env_path == NULL || cmd == NULL || status == NULL)
		return (NULL);

	/* check if cmd is already path (preceded with '/') */
	tmp = cmd;
	while (!(is_path = delimCharcmp("/", tmp)) && *tmp++)
		;
	if (is_path == true)
	{
		*status = access(cmd, F_OK | X_OK) == 0 ? 1 : 2;
		return (NULL);
	}
	cmdLen = strlen(cmd);

	while ((env_path = strtok(env_path, ":")) != NULL)
	{
		ee = strlen(env_path);

		if (GLN_MALLOC(abs_path, NULL, sizeof(char) * (ee + cmdLen + 4)) == 0)
			return (NULL);

		/* copy tokenize path (should be absolute) */
		ee = str_cpy(abs_path, env_path, ee);
		abs_path[ee++] = '/';

		/* append cmd and terminate with a null character */
		ee += str_cpy(abs_path + ee, cmd, cmdLen);

		if (ACCESS_CMD(abs_path, F_OK | X_OK, *status) == true)
			break;

		free(abs_path);
		env_path = abs_path = NULL;
	}
	return (*status == 0 ? abs_path : ((*status = 2), NULL));
}
