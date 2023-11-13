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
/* h: 1: l: not found */
__attribute__((noreturn)) void eRR_routine(long err)
{
	(void)err;
	exit(EXIT_FAILURE);
}

const char *GLOBAL_SHELL_RECG_ERROR_MSG[27] = {
	"not found", "permission denied"
};

#define parse_colmn_space_tstr(s, l) (s[l++] = ':', s[l++] = ' ')

void errMsg(int errnum, struct shell_info *procinfo)
{
	char s[256];
	register size_t len, procnum, oo = 0, pp = 0;

	len = str_cpy(s, procinfo->shell_name, 0);
	parse_colmn_space_tstr(s, len);

/* parse num */
	procnum = procinfo->cmd_cnt;
	while (procnum && oo++)
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

	len += str_cpy((s + len), procinfo->cmd, 0);
	parse_colmn_space_tstr(s, len);

	s[len] = 0;
	puts(s);

	_nputs(s, 0);
	_nputs(GLOBAL_SHELL_RECG_ERROR_MSG[errnum], 1);
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
