void sig_handler(int sig)
{
}

void sig_call(int sig)
{
#ifdef SIG_STRICT
	/* use signal */
#define SIG
#else
	/* use sigaction */
#enif
}
