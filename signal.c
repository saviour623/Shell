#include "shell_header.h"
void /* Examine a wait() status using the W* macros */
printWaitStatus(const char *msg, int status)
{
	if (msg != NULL)
		printf("%s", msg);
	if (WIFEXITED(status)) {
		printf("child exited, status=%d\n", WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)) {
		printf("child killed by signal %d (%s)",
			   WTERMSIG(status), strsignal(WTERMSIG(status)));
#ifdef WCOREDUMP /* Not in SUSv3, may be absent on some systems */
		if (WCOREDUMP(status))
			printf(" (core dumped)");
#endif
		printf("\n");
	} else if (WIFSTOPPED(status)) {
		printf("child stopped by signal %d (%s)\n",
			   WSTOPSIG(status), strsignal(WSTOPSIG(status)));
#ifdef WIFCONTINUED /* SUSv3 has this, but older Linux versions and
					   some other UNIX implementations don't */
	} else if (WIFCONTINUED(status)) {
		printf("child continued\n");
#endif
	} else { /* Should never happen */
		printf("what happened to this child? (status=%x)\n",
			   (unsigned int) status);
	}
}


int
main(int argc, char *argv[])
{
	int status;
	pid_t childpid;
	setbuf(stdout, NULL);

	signal(SIGINT, SIG_IGN);
	switch ((childpid = fork()))
	{
		case -1:
			perror("fork");
			exit(-1);
			break;
		case 0:
			signal(SIGINT, SIG_DFL);
			execve("//", argv, 0);
			int eer = errno;

			exit(err == EACESS ? 8 : err == ENDEXEC ? 9
				 : err == ETXTBSY ? 10 : EXIT_FAILURE);
		default:
			do {
				waitpid(childpid, &status, WUNTRACED | WCONTINUED);
				if (errno == ECHILD)
					exit(0);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			//ENOMEM
	}
	perror("try");
	printf("%d\n", (int)status >> 8);
}

//EACCES permission denied
//ENOEXEC untecognized executable
//ETXTBSY unable to open file
