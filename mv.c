#include "shell_header.h"
struct statb {
               dev_t     st_dev;         /* ID of device containing file */
               ino_t     st_ino;         /* Inode number */
               mode_t    st_mode;        /* File type and mode */
               nlink_t   st_nlink;       /* Number of hard links */
               uid_t     st_uid;         /* User ID of owner */
               gid_t     st_gid;         /* Group ID of owner */
               dev_t     st_rdev;        /* Device ID (if special file) */
               off_t     st_size;        /* Total size, in bytes */
               blksize_t st_blksize;     /* Block size for filesystem I/O */
               blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */
};

#ifdef __linux__
#include <sys/fsuid.h>
#endif
#include <dirent.h>
#include <sys/vfs.h>
#include <stdarg.h>

#define IS_RDWRXTE(PERM) (PERM & (S_IRUSR | S_IWUSR | S_IXUSR))
#define PROC_OWNED(ENT, PROCID) (ENT.st_uid == PROCID)
#define MVFILE_MAX 256
typedef struct pathsc {
	char *pth_parenpath;
	char *sc_abspath;
} pathsc;

#define TST_LOGFUN(tst, err, act)\
	do { if ((tst) == (err)){ act; exit(EXIT_FAILURE); } } while(0)

char *path_ncpy(char *dest, const char *src, size_t len)
{
	register int c, oo = 0;

	if (src == NULL)
	{
		errno = EINVAL;
		return NULL;
	}
	if (dest == NULL && *src != 0)
	{
		dest = malloc(len * sizeof (char));
		if (dest == NULL)
		{
			errno = ENOMEM;
			return NULL;
		}
	}
	for (; len && (c = *src); len--, *src++)
	{
		if ((c == '/') && (*(src + 1) == '/'))
			continue;
		dest[oo++] = *src;
	}
	return dest;
}
__attribute__((nonnull)) static char *get_ppath(char *path)
{
	static char paren_path[PATH_MAX];
	register int oo = 0, n_bkslash = 0;
	register size_t len = strlen(path), tmlen = len;

	if (len > PATH_MAX || len == 0 || *path == 0)
		return NULL;

	for (; (tmlen > 0) && (n_bkslash = (path[tmlen - 1] != '/')); tmlen--, oo++)
		;
	if (n_bkslash)
		return ((paren_path[0] = '.'), (paren_path[1] = '/'), (paren_path[2] = 0), paren_path);
	return path_ncpy(paren_path, path, len - oo);
}
gid_t sgrplist(gid_t _sgid)
{
	gid_t grp[NGROUPS_MAX + 1];
	register int oo, ngrps = getgroups(NGROUPS_MAX + 1, grp), atgrp = 0;

	if (ngrps == -1)
		return (_sgid);
	for (oo = 0; oo < ngrps; oo++)
	{
		atgrp = grp[oo];
		if (_sgid == atgrp)
			return (atgrp);
	}
	return (_sgid);
}
int faccesswx(int fd, char *fileName)
{
	struct stat statbuf;
	mode_t file_mode, s_perm = 0;
	register uid_t euid_proc = geteuid(), fuid_proc __UNUSED__;
	register gid_t egid_proc = getegid(), fgid_proc __UNUSED__;
	register int flag_id;

	TST_LOGFUN(fstat(fd, &statbuf), -1, perror("fstat"));

	/* for a regular file, it's modification loosely depends on the ownership of its parent dir */
	if (S_ISREG(statbuf.st_mode))
		TST_LOGFUN(stat(get_ppath(fileName), &statbuf), -1, perror("stat"));
	file_mode = statbuf.st_mode;

#if defined(__linux__) && defined(_SYS_FSUID_H)
	fuid_proc = setfsuid(-1);
	fgid_proc = setfsgid(-1);

	if (fuid_proc == 0 || fuid_proc == statbuf.st_uid)
		flag_id = 1;
	else if (fgid_proc == statbuf.st_gid || sgrplist(statbuf.st_gid) == statbuf.st_gid)
		flag_id = 2;
	if (flag_id != 0)
		goto fsperm;
#endif
	/* file must be owned by a user or group(s) */
	if (euid_proc == 0 || euid_proc == statbuf.st_uid)
		flag_id = 1;
	else if (egid_proc == statbuf.st_uid || sgrplist(statbuf.st_gid) == statbuf.st_gid)
		flag_id = 2;
	else
		return (s_perm & 0);
fsperm:
	if (S_ISDIR(statbuf.st_mode))
	{
		/* if sticky bit is set on the dir and the dir doesnt belong to the process, then we dont have access */
		if ((file_mode & S_ISVTX) && !(PROC_OWNED(statbuf, euid_proc))) /* TODO: compare against fsuid */
			return (s_perm & 0);
	}

	/* we are only interested in the execute and write permission */
	if (euid_proc != 0)
		s_perm = flag_id == 1 ? (S_IWUSR | S_IXUSR) & file_mode
			: flag_id == 2 || (sgrplist(statbuf.st_gid) != egid_proc)
			? (S_IWGRP | S_IXGRP) & file_mode : (S_IWOTH | S_IXOTH) & file_mode;

	else
		s_perm = 1;
	return s_perm;
}

void mv_directory_func(int fln, ...)
{
	va_list flelist;
	int fd, file_mode, permis[2];;
	char *flepath[MVFILE_MAX], *ptrfl;;
	__fsword_t tpfl[2];

	char *a = NULL, *b = NULL;
	struct stat statbuf;
	struct statfs fsbuf;
	register int oo;

	va_start(flelist, fln);

	switch (fln)
	{
	case 0:
		puts("mv: mv requires two or more file operand");
		exit(-1);
	case 1:
		(ptrfl = va_arg(flelist, char *)) != NULL ? (printf("mv: missing destination file operand after "), puts(ptrfl))
			: puts("mv: empty operand");
		exit(-1);
	default:
		if (fln > MVFILE_MAX)
		{
			puts("mv: arguments exceeds MVFILE_MAX");
			exit(-1);
		}
	}
	oo = 0;
	while (oo < fln)
	{
		ptrfl = va_arg(flelist, char *);
		if (ptrfl == NULL || *ptrfl == 0)
		{
			printf("mv: argument %d is null or empty\n", oo + 1);
			exit(-1);
		}
		oo++;
	}
	ptrfl = a;
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
}

int main(void)
{
	struct stat statbuf;
	//stat("./new", &statbuf);
	//perror("stat");
//	char path[PATH_MAX];
//	char *p = cpy_ppath("hop");
//	if (p != NULL)
//		puts(p);
//	puts(path_ncpy(NULL, "/hello/hi/////kl//p/power", 26));
//	printf("%s\n", realpath("/h/b/t/y", path));
	mv_directory_func(2, NULL, "./empty_test/new2");
	return (0);
}
	/* also change timestamp with utimes in mv*/
/* add umask builtin */
