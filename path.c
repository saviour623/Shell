#include "shell_header.h"

#define ACCESS_CMD(cmd, mode, stat)		\
	(!((stat) = access((cmd), (mode))))

/**
 * path - finds the path and existence of a file from the PATH environment.
 * @cmd: file
 * @status: if -1, error - 0, file is found and is an executable -
 * 1, file is a path - 2 file is not executable -
  * 3, ordinary file - 4, file is a directory.
 * Return: NULL on error, else the absolute path of the file.
 */
#define BUILT_IN_TABLE(NULL_NULL)\
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
	register char *ptr = NULL;
	register int oo = 0;
	char *env_path;
	char cpy_path[MAX_ENVPATH_LEN];
	built_ins builtin_func[] = BUILT_IN_TABLE(NULL);
	cmd_alias *alias = NULL;
	shell_info info = {0};

	if ((cmd == NULL || *cmd == 0) || (status == NULL))
		return (NULL);

	*status = -1;
	while ((ptr = (builtin_func[oo]).builtin_name) != NULL)
	{
		if (strcmp(ptr, cmd) == 0)
		{
			(builtin_func[oo]).builtin_exe(&info); /* cmd, arguments env */
			return (NULL);
		}
		oo++;
	}
	/* search alias */
	ptr = search_alias(cmd, alias);
	if (ptr != NULL)
		env_path = ptr;
	else
		env_path = getenv("PATH");
	if (env_path == NULL)
		return (NULL); /* can't locate file error here */

	str_cpy(cpy_path, env_path, MAX_ENVPATH_LEN);

	env_path = search_path(cpy_path, cmd, status);
	return (env_path);
}

char *search_alias(char *cmd, cmd_alias *__alias)
{
	cmd_alias *ptr = __alias;

	while (ptr != NULL)
	{
		if (strcmp(cmd, ptr->name) == 0)
			return (ptr->path);
		ptr = ptr->nxt_alias;
	}
	return (NULL);
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
