#include "shell_header.h"
#include<setjmp.h>

#define sahandler sigactionhandler.sahandler
#define sa_sigaction __sigaction_handler.sa_sigaction
static sigjmp_buf envbuf;

void sign(int sig __UNUSED__)
{
	sleep(10);
	puts("sig");
}
void sign2(int sig, siginfo_t *info, void *p)
{
	puts("here");
	pause();
}
int sysb(char *);
int main(int argc, char *argv[])
{
	int status;
	pid_t c;
	int oo = 0;
	struct sigaction sig, sig1;
	SA_NOCLDSTOP, SA_RESTART, SA_SIGINFO;

	sysb("sleep");
//	puts("here");

/*
//	sigaction();
	switch ((c = fork()))
	{
	case -1:
		puts("error");
		exit(0);
	case 0:
		execve("./test", (char *[]){"-l"}, 0);
	default:
		while (wait(&status) != -1);
		perror("wait");
		} */
	return(0);
}

int sysb(char *cmd)
{
	sigset_t blockMask, origMask;
	struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
	pid_t childPid;
	int status, savedErrno;

	sigemptyset(&blockMask); /* Block SIGCHLD */
	sigaddset(&blockMask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &blockMask, &origMask);
	saIgnore.sa_handler = SIG_IGN; /* Ignore SIGINT and SIGQUIT */
	saIgnore.sa_flags = 0;
	sigemptyset(&saIgnore.sa_mask);
	sigaction(SIGINT, &saIgnore, &saOrigInt);
	sigaction(SIGQUIT, &saIgnore, &saOrigQuit);

	switch (childPid = fork()) {
	case 1: /* fork() failed */
		puts("failed");
		status = -1;
		break; /* Carry on to reset signal attributes */
	case 0: /* Child: exec command */
		saDefault.sa_handler = SIG_DFL;
		saDefault.sa_flags = 0;
		sigemptyset(&saDefault.sa_mask);

		if (saOrigInt.sa_handler != SIG_IGN)
			sigaction(SIGINT, &saDefault, NULL);

		if (saOrigQuit.sa_handler != SIG_IGN)
			sigaction(SIGQUIT, &saDefault, NULL);

		sigprocmask(SIG_SETMASK, &origMask, NULL);

		execve("/bin/sleep", (char*[]){"/bin/sleep", "10", NULL}, environ);
		_exit(127); /* We could not exec the shell */
	default: /* Parent: wait for our child to terminate */
		while (waitpid(childPid, &status, 0) == -1) {
			if (errno != EINTR) { /* Error other than EINTR */
				status = 1;
				puts("here");
				break; /* So exit loop */
			}
		}
		break;
	}
	/* Unblock SIGCHLD, restore dispositions of SIGINT and SIGQUIT */

	savedErrno = errno; /* The following may change 'errno' */
	sigprocmask(SIG_SETMASK, &origMask, NULL);
	sigaction(SIGINT, &saOrigInt, NULL);
	sigaction(SIGQUIT, &saOrigQuit, NULL);
	errno = savedErrno;
	return status;
}

