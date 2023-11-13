#include "shell_header.h"

/**

struct shell_info
{
	char *cmd;
	char **cmd_opt;
	size_t cmd_cnt;
	bool cmd_sep;
	built_ins *cmd_bltn;
};
*/
//sh: 1: l: not found
__attribute__((noreturn)) void eRR_routine(long err)
{
	(void)err;
	exit(EXIT_FAILURE);
}
void errMsg(int errnum, struct shell_info *procinfo)
{
	char s[256];
	size_t len;
	len = str_cpy(s, procinfo->shell_name, 0);

	s[len++] = ':';
	s[len++] = ' ';

	/* parse num */
	len += str_cpy((s + len), procinfo->cmd, 0);
	len 
}
char *nitoa(ssize_t i)
{
}
size_t _nputs(const char *str, int newline)
{
	register size_t len;

	if (str == NULL)
		return (-1);

	if (*str == 0)
		return (0);

	len = strlen(str);

	fflush(stdout);

	write(STDOUT_FILENO, str, len);

	if (newline)
	{
		write(STDOUT_FILENO, "\n", 1);
		len++;
	}
	return (len);
}
