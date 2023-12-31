
_mv_dest_info destHandlingfunc(char *fl_dest, int numFls)
{
	/* set the destination file */
		struct stat _nfo_dest_fl;
		struct statfs _nfo_fsdest_fl;
		static _staticbf = {0};

		int fd;
		/* if destination file/one of it's path does not exist create a directory with that path */
		if (stat(_mv_destination_fl, &_nfo_dest_fl) == -1 && errno == ENOENT)
		{
			if (numfls > 2)
			{
				fd = open(_mv_destination_fl, O_CREAT | O_DIRECTORY,
						  /* TODO: use the default process permission */ S_IWUSR | S_IRUSR | S_IXUSR | S_IWGRP | S_IWGRP | S_IROTH);
				if (fd == -1)
					return (_mv_dest_info *)0;
			}
			else
			{
				fd = open(_mv_destination_fl, O_CREAT,
						  /* TODO: use the default process permission */ S_IWUSR | S_IRUSR | S_IXUSR | S_IWGRP | S_IWGRP | S_IROTH);
				if (fd == -1)
					return (_mv_dest_info *)0;
			}
			_staticbf.ds_fd = fd;
		}
		else {
			return (char *)1;
		}

		if ( !S_ISDIR(_nfo_dest_fl.st_mode) && (numFls > 2) )
			return (char *)2;

		TST_LOGFUN(statfs(_mv_destination_fl, &_nfo_fsdest_fl), -1, perror("statfs"));
		//	tpfl[fln - 1] = _nfo_fsdest_fl.f_type;

		return fl_dest;
	}
}


int _mvMultiFiles(int numOfiles, char **flelist)
{
	/* this function is uinimplemented */
	char *ptrfl = a;
	for (int oo = 0; oo < 2; oo++)
	{
		if (a == NULL || *a == 0)
		{
			puts("mv: error empty path");
			exit(EXIT_FAILURE);
		}
		fd = open(ptrfl, O_PATH);
		TST_LOGFUN(fd, -1, perror("open"));
		TST_LOGFUN(fstat(fd, &statbuf), -1, perror("stat"));

		permis[oo] = faccesswx(fd, a);
		if (permis[oo] == 0)
		{
			puts("permission denied");
			exit(-1);
		}

		TST_LOGFUN(statfs(a, &fsbuf), -1, perror("statfs"));
		tpfl[oo] = fsbuf.f_type;

		if (close(fd) == -1)
		{
			perror("close");
			exit(-1);
		}
		ptrfl = b;
	}
	if (tpfl[0] != tpfl[1])
	{
		/* actual copy file/directory but for now lets just signify */
		puts("no the same file system");
		exit(-1);
	}
	TST_LOGFUN(rename(a, b), -1, puts("couldn't move file"));
	return 0;
}
