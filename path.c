#include "shell_header.h"

#define ACCESS_CMD(cmd, mode, stat)		\
	(!((stat) = access((cmd), (mode))))

/**
 * path: finds the path and existence of a file from the PATH environment.
 * @cmd: file
 * @status: if -1, error; 0, file is found and is an executable; 1, file is a path; 2 file is not executable; 3, ordinary file; 4, file is a directory.
 * @Return: NULL on error, else the absolute path of the file.
 */
char *path(const char *cmd, int *status)
{
	char *env_path = getenv("PATH");
	size_t env_len;
	char *cpy_path;

	*status = -1;

	if (env_path == NULL && (cmd == NULL || *cmd == 0) || status == NULL)
		return (NULL);

	env_len = strlen(env_path);

	if (GLN_MALLOC(cpy_path, NULL, sizeof(char *) * env_len) == 0)
		return NULL;

	str_cpy(cpy_path, env_path, env_len);

	env_path = search_path(cpy_path, cmd, status);

	free(cpy_path);
	return (env_path);
}

/**
 * search_path - searches for executive permission of file.
 * @path: possible paths where file can be found.
 * @cmd: file
 * status: -1 on error, 0, if found, 2 if not in path but no error.
 * Return: returns the path appended to file is successful else NULL.
 */
char *search_path(char *__restrict__ env_path, const char *cmd, int *status)
{
	char *abs_path,  *tmp_path = env_path;
	register int cmdLen, noPath, oo, ee;

	*status = -1; /* initially set to error */

	if (env_path == NULL || cmd == NULL || status == NULL)
		return (NULL);

	/* check if cmd is a path (preceded with '/') */
	while(!(noPath = delimCharcmp("/", tmp_path++)));

	if (noPath == false)
	{
		*status = access(cmd, F_OK | X_OK) == 0 ? 1 : -1;
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

		*status = 2;
		free(abs_path);
		env_path = abs_path = NULL;
	}
	return *status == 0 ? abs_path : NULL;
}
