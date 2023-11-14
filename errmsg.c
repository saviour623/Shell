#include "shell_header.h"

void eRR_routine(long err)
{
	(void)err;
	exit(EXIT_FAILURE);
}

const char *GLOBAL_SHELL_RECG_ERROR_MSG[27] = {
	"not found", "Permission denied",
	"unable to start up process",
	"Resource temporarily unavailable",
	"Out of memory", "unexpected end of file",
	"Try again", "illegal number",
	"illegal option"
};

#define parse_colmn_space_tstr(s, l)\
	(s[l++] = ':', s[l++] = ' ', s[l] = 0)

void errMsg(int errnum, struct shell_info *procinfo)
{
	char s[256]; /* shoukd be enough */
	register size_t len, procnum, oo = 0;

	len = str_cpy(s, procinfo->shell_name, 0);
	parse_colmn_space_tstr(s, len);

    /* parse command count */
	procnum = procinfo->cmd_cnt;
	while (procnum && ++oo)
		procnum /= 10;

	procnum = procinfo->cmd_cnt;
	len += (oo - 1);

	while (procnum)
	{
		*(s + len) = (procnum % 10) + 48;
		procnum /= 10;
		len--;
	}
	len += (oo + 1);
	parse_colmn_space_tstr(s, len);

	if (procinfo->cmd != NULL)
	{
		len += str_cpy((s + len), procinfo->cmd, 0);
		parse_colmn_space_tstr(s, len);
	}

	_nputs(STDERR_FILENO, s, 0);

	if (procinfo->err_info == true && procinfo->cmd_opt[1] != NULL)
	{
		_nputs(STDERR_FILENO, GLOBAL_SHELL_RECG_ERROR_MSG[errnum], 0);
		_nputs(STDERR_FILENO, ": ", 0);
		_nputs(STDERR_FILENO, procinfo->cmd_opt[1], 1);
	}
	else
		_nputs(STDERR_FILENO, GLOBAL_SHELL_RECG_ERROR_MSG[errnum], 1);
}
