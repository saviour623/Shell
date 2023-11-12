#include "shell_header.h"

void exe(char **s)
{
	pid_t pchild;
	int status;
	int oo = 2;
	fflush(stdout);
	fflush(stdin);
	
	switch ((pchild = fork()))
	{
	case -1:
		perror("fork");
		break;
	case 0:
		errno = 0;
		execve(s[0], s, environ);
		perror("execve");
		exit(EXIT_SUCCESS);
	default:
		do {
			waitpid(pchild, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return;
}
int main(){
	char *s[3] = {"/bin/watch", "p", NULL};

	pid_t pchild;
	int status;
	int oo = 2;
	fflush(stdout);
	fflush(stdin);

	while (oo--)
	{
		exe(s);
		s[0] = "/bin/ls";
		s[1] = "-l";
		exe(s);
	}
}
