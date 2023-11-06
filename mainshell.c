#include "shell_header.h"
__attribute__((noreturn)) void eRR_routine(long err)
{
	(void)err;
	exit(EXIT_FAILURE);
}

#define ROOT_CMP(B)\
	((B[0] == '\\') && (B[1] == 'r') &&  (B[2] == 'o')\
	 && (B[3] == 'o') &&  (B[4] == 't'))

#define ENV_MAX_SIZE

int main(int argc __UNUSED__, char **argv __UNUSED__)
{
	if (argc == 2 && strcmp("--help", argv[1]))
	{
		eRR_routine(ERRMSG);
	}

	if ((argc == 1) || strcmp(argv[1], "-i"))
	{
	   	if (interactive_mode(argc, argv) == -1)
			eRR_routine(ERRSTR);
	}
}

static int interactive_mode(int argc, char **argv)
{
	char *env_tmp = getenv("HOME"), *prompt;
	char *line_buffer = NULL;
	size_t line = 0;
	ssize_t char_read;

	if (env_tmp == NULL)
		eRR_routine(ERRNULL);
	prompt = ROOT_CMP(env_tmp) ? "# " : "$ ";

	while (1)
	{
		printf("%s", prompt);
		line_buffer = NULL;
		line = 0;
		char_read = getline(&line_buffer, &line, stdin);

		printf("%d\n", cnt_token(line_buffer, " "));
		free(line_buffer);
	}
	return 0;
}

int cnt_token(const char *__restrict__ st, const char *__restrict__ del)
{
	char c, d; /* temp */
	bool set = true;
	register int oo = 0;

	if (st == NULL || del == NULL)
		return -1;

	if (*st == '\0' || *del == '\0')
		return 0;

	d = *del;

	for (; (c = *st); *st++)
	{
		if (c == d)
			set = true;
		else if (c != d && set == true)
		{
			oo++;
			set = false;
		}
	}

	return oo;

}

int strtok_sh(const char *__restrict__ s, const char *__restrict__ del, char **vec, size_t vecsize)
{
	char c, d; /* temp */
	register int ee;

	if (s == NULL || del == NULL)
		return -1;

	if (*s == '\0' || *del == '\0')
		return 0;

	d = *del;
	ee = 0;

	for (; (c = *s); *s++)
	{
		if (vecsize < 0)
			return 0;

		if (c == d)
			continue;
		else if (c != d)
		{
			for (; ((c = *s) != d) && (c != '\0'); *s++, ee++)
				*vec[ee] = c;
			ee = 0;
			vec++;
			vecsize--;
		}
	}

	return 0;

}
