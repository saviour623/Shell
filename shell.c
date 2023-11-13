#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

static int bss;
extern char etext, edata, end;
//&etext gives us the address of the end of the program text i.e the start of the initialized data
int main(void)
{
	pid_t ppid = getppid();
	pid_t p_d = getpid();
	char *prt = &etext;
	char buffer[1024];
	(buffer[0] = 'o', buffer[1] = 'l');

	puts(buffer);
	printf("%p\n", prt);

	long fd, cnt;
	
	fd = open("/proc/self/cmdline", O_RDONLY);

	if (fd == -1)
	{
		perror("proc");
		exit(EXIT_FAILURE);
	}

	cnt = read(fd, buffer, 1024);

	if (cnt == -1)
	{
		perror("read");
		exit(-1);
	}

	buffer[cnt] = '\0';
	puts(buffer);

	return 0;
}
