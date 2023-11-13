#include "shell_header.h"

volatile char *sig_prompt;

#define ROOT_CMP(B)\
	((B[0] == '/') && (B[1] == 'r') &&  (B[2] == 'o')\
	 && (B[3] == 'o') &&  (B[4] == 't'))

void sig_interrupt(int sig __UNUSED__)
{
	/* another interrupt may occur when handling signal resulting to
	 * two prompt, unfortunately sigaction() is not supported so... yep */
	fflush(stdout);
	write(1, "\n", 2);
	write(1, (char *)sig_prompt, strlen((char *)sig_prompt));
}

int main(int argc __UNUSED__, char **argv __UNUSED__)
{
	shell_info sh_info = {"myshell", "ls", NULL, 6, 0, NULL};

	if (argc == 2 && strcmp("--help", argv[1]))
	{
		eRR_routine(ERRMSG);
	}

	errMsg(1, &sh_info);
	if ((argc == 1) || strcmp(argv[1], "-i"))
	{
		//	if (interactive_mode(argc, argv) == -1)
		//		eRR_routine(ERRSTR);
	}
	return 0;
}

int interactive_mode(int argc __UNUSED__, char **argv __UNUSED__)
{
	char *env_tmp = getenv("HOME"), *prompt;
	char **tokens, *line_buffer = NULL;
	size_t line = 0;
	ssize_t char_read;
	int is_interactive_tty, status;
	char *pathrc;

	if (env_tmp == NULL)
		eRR_routine(ERRNULL);
	prompt = ROOT_CMP(env_tmp) ? "# " : "$ ";

	if (signal(SIGINT, sig_interrupt) == SIG_ERR)
		eRR_routine(2);
	if (signal(SIGQUIT, sig_interrupt) == SIG_ERR)
		exit(0);
	do {
		is_interactive_tty = isatty(STDIN_FILENO);
		is_interactive_tty ? (void) printf("%s", prompt) : (void) 0;
		sig_prompt = prompt;
		fflush(stdout);

		line_buffer = NULL;
		line = 0;

		char_read = stdin_getline(&line_buffer, &line);

		if (char_read == -1)
		{
			free(line_buffer);
			putchar('\n');
			exit(-1);
		}

		line_buffer[char_read] = '\0';
		tokens = getcmdString(line_buffer);

		if (tokens != NULL)
		{
			pathrc = path(*tokens, &status);

			if ((pathrc == NULL) && (status != 2 || (status != 1)))
				goto free;

			pathrc != NULL ? *tokens = pathrc : 0;

			/* check if it is a directory */
			/* find builtin */
			/* find alias */
			execteArg(tokens);
			free(pathrc);
		}
	free:
		free(line_buffer);
		free(tokens);
	} while (is_interactive_tty == true);

	return 0;
}

void execteArg(char **cmd)
{
	pid_t pchild;
	int status;

	fflush(stdout);
	fflush(stdin);

	switch ((pchild = fork()))
	{
		case -1:
			eRR_routine(errno);
		case 0:
			errno = 0;
			execve(cmd[0], cmd, environ);
			perror("execve");
			exit(EXIT_SUCCESS);
		default:
			do {
				waitpid(pchild, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}


char **getcmdString(char *__restrict__ arg)
{
	char *delim = " \t\r\n";
	char **buftok;
	char *token = NULL;
	int oo;

	if ((arg == NULL) || (*arg == '\0'))
		return NULL;

	buftok = malloc((sizeof(char *) * (getNumtoks(arg, delim))) + 1);

	if (buftok == NULL)
		eRR_routine(ERRMEM);

	token = arg;
	for (oo = 0; (token = strtok(token, delim)) != NULL; oo++)
	{
		buftok[oo] = token;
		token = NULL;
	}

	buftok[oo] = NULL;
	return buftok;
}

int getNumtoks(const char *__restrict__ st, const char *__restrict__ delim)
{
	bool set;
	register int oo;

	if (st == NULL || delim == NULL)
		return -1;

	if (*st == '\0' || *delim == '\0')
		return 0;

	oo = 0;
	set = true;

	for (; *st; (void)*st++)
	{
		if (delimCharcmp(delim, st))
			set = true;
		else if (set)
		{
			oo++;
			set = false;
		}
	}
	return oo;
}

