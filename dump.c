
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
