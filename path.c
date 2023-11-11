#include "shell_header.h"
//PATH=/usr/local/sbin:/usr/local/bin:/bin:/usr/bin:/sbin:/usr/sbin:/usr/games:/usr/local/games
char *path(void)
{
	char *env_path = getenv("PATH");

	if (env_path == NULL)
		perror("path");

	puts(env_path);

	printf("%d\n", access("a.out", X_OK));
}

int main(void)
{
	path();
	return 0;
}
