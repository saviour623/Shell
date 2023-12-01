#include "shell_header.h"

static volatile char *sig_prompt;
static volatile shell_info *track_info;
static volatile sig_atomic_t sigterm;
static volatile sig_atomic_t terminal;
struct termios ttyacct, reset_term;
typedef struct sigaction sa_signal;
volatile sa_signal gb_newSig;
static jmp_buf bufenv __UNUSED__;

#define USESIG_JMP
//#define SYSV_SIGNAL

#define ROOT_CMP(B)										\
	((B[0] == '/') && (B[1] == 'r') &&  (B[2] == 'o')\
	 && (B[3] == 'o') &&  (B[4] == 't'))

#define ___assertSigterm(flag, cl) /* TODO */

void sig_interrupt(int sig __UNUSED__)
{
	ttyacct.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &ttyacct);
	if (sig == SIGINT || sig == SIGQUIT)
	{
#ifdef USESIG_JMP
		write(STDOUT_FILENO, "\n", 1);
		ttyacct.c_lflag |= ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &ttyacct);
		siglongjmp(bufenv, 1); /* sig-async unsafe */
#else
/* sigabrt would catch this since sig_interrupt would return */
		if (terminal == true)
		{
			write(STDOUT_FILENO, "\n", 2);
			write(STDOUT_FILENO, (char *)sig_prompt, strlen((char *)sig_prompt));
		}
#endif
	}
	ttyacct.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &ttyacct);
	if (sig == SIGTERM)
	{
		/* cleanup the shell and raise signal again */
		exit_shell_func((shell_info *)track_info);
		raise(SIGTERM);
	}
}

void set_signal(void (*sig_handler)(int), int nargs __UNUSED__, int *args)
{
	int pp, saveErrno = errno;
	sigset_t blockSig, tmpSet;
	struct sigaction sa_sig;

	((void)saveErrno, (void)blockSig, (void)tmpSet, (void)pp, (void)sa_sig); /* maybe unused */

	if (args == NULL)
		return;
	if (nargs > NSIG)
		return;

#ifdef SYSV_SIGNAL
	/* not portable */
	while (nargs--)
		signal(*args++, sig_handler); /* ignoring sig error here */
#else
	sigemptyset(&blockSig); /* Block SIGCHLD */
	sigaddset(&blockSig, SIGCHLD);
	sigprocmask(SIG_BLOCK, &blockSig, NULL);

	sigfillset(&sa_sig.sa_mask);
	sigdelset(&sa_sig.sa_mask, SIGKILL);
	sigdelset(&sa_sig.sa_mask, SIGSTOP);
	sa_sig.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	sa_sig.sa_handler = sig_handler;

	sigfillset(&tmpSet);
	for (; nargs && nargs < NSIG; nargs--)
	{
		pp = *args++;
		if (sigismember(&tmpSet, pp) && (pp != SIGSTOP || pp != SIGKILL))
			sigaction(pp, &sa_sig, NULL);
	}
#endif
	errno = saveErrno;
}
__inline__ void printPrompt(int ftty, char *strprmpt)
{
	if (ftty != 1)
		return;

	if (strprmpt == NULL)
	{
		strprmpt = _ngetenv("HOME");

		if (strprmpt == NULL)
			return;
		strprmpt = ROOT_CMP(strprmpt) ? "# " : "$ ";
	}
	sig_prompt = strprmpt;
	_nputs(STDOUT_FILENO, strprmpt, 0);

	fflush(stdout);
}

int interactive_mode(shell_info *sh_info)
{
	char *prmpt = (char *)0, *pathrc = (char *)0;
	char **tokens, *line_buffer = (char *)0;
	ssize_t char_read;
	size_t rdSize = 0;
	int is_tty, sig_list[] = {SIGINT, SIGQUIT, SIGTERM};

	tcgetattr(STDIN_FILENO, &ttyacct);
	track_info = sh_info;
	set_signal(sig_interrupt, sizeof sig_list / sizeof (int), sig_list);
	terminal = is_tty = isatty(STDIN_FILENO);
#ifdef USESIG_JMP
	sigsetjmp(bufenv, 1);
#endif
	do {
		printPrompt(is_tty, prmpt);
		line_buffer = NULL;
		char_read = stdin_getline(&line_buffer, &rdSize);

		if (char_read == -1)
		{
			if (line_buffer != NULL)
				free(line_buffer);
			/* tcsetattr(STDIN_FILENO, TCSANOW, &reset_term); */
			_nputs(STDOUT_FILENO, "\n", 0);
			exit_shell_func(sh_info);
		}
		line_buffer[char_read] = 0;
		tokens = getcmdString(line_buffer);

		if (tokens != NULL)
		{
			sh_info->cmd = *tokens;
			sh_info->cmd_opt = tokens;
			pathrc = path(sh_info);

			if (pathrc == NULL)
			{
				if ((sh_info->status == 2) || sh_info->status == -1)
					goto free;
			}

			pathrc != NULL ? sh_info->cmd = pathrc : 0;
			execteArg(sh_info);
			free(pathrc);
		}
	free:
		free(line_buffer);
		free(tokens);
	} while (is_tty == true);

	return 0;
}

void execteArg(shell_info *sh_info)
{
	pid_t pchild;
	int waitstat, status;

	fflush(stdout);
	switch ((pchild = fork()))
	{
	case -1:
		errMsg(ERR_SHLL_EAGAIN, sh_info);
		break;
	case 0:
		execve(sh_info->cmd, sh_info->cmd_opt, environ);
		exit(errno == EACCES ? 1 : errno == ENOEXEC ? 9
			 : errno == ENOMEM ? 4 : 126);
	default:
		do {
			waitstat = waitpid(pchild, &status, WUNTRACED | WCONTINUED);
			if (waitstat == -1 && waitstat != EINTR)
			{
				if (errno != ECHILD)
					errMsg(12, sh_info);
				return;
			}

		} while  (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	if (WIFEXITED(status))
	{
		(status = WEXITSTATUS(status)) && (status != 126)
			? errMsg(status, sh_info) : /* other error occured */ (void)0;
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


int main(int argc __UNUSED__, char **argv __UNUSED__)
{
	shell_info sh_info = {0};

	sh_info.shell_name = argv[0];
	if (argc == 2 && strcmp("--help", argv[1]))
	{
		eRR_routine(ERRMSG);
	}

	if ((argc == 1) || (_nstrcmp(argv[1], "-i", NULL) > 0))
	{
		sh_info.argv = argv;
		sh_info.argc = argc;
		if (interactive_mode(&sh_info) == -1)
			eRR_routine(ERRSTR);
	}
	return 0;
}
