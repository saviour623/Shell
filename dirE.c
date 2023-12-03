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
#define IS_RDWRXTE(PERM) (PERM & (S_IRUSR | S_IWUSR | S_IXUSR))
#define PROC_OWNED(ENT, PROCID) (ENT.st_uid == PROCID)
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
int faccesswx(int fd, char *fileName)
{
	struct stat statbuf;
	mode_t file_mode, s_perm = 0;
	register uid_t euid_proc = geteuid(), fuid_proc __UNUSED__;
	register gid_t egid_proc = getegid(), fgid_proc __UNUSED__;

	TST_LOGFUN(fstat(fd, &statbuf), -1, perror("fstat"));

	/* for a regular file, we are more interested in the ownership of its parent dir */
	if (S_ISREG(statbuf.st_mode))
		TST_LOGFUN(stat(get_ppath(fileName), &statbuf), -1, perror("stat"));

	file_mode = statbuf.st_mode;
#if defined(__linux__) && defined(_SYS_FSUID_H)
	fuid_proc = setfsuid(-1);
	fgid_proc = setfsgid(-1);
    s_perm = (fuid_proc == statbuf.st_uid) ? (S_IWUSR | S_IXUSR) & file_mode
		: fgid_proc == statbuf.st_gid ? (S_IWGRP | S_IXGRP) & file_mode : 0;
	if (s_perm)
		return s_perm;
#endif
	if (euid_proc != 0)
		s_perm = (euid_proc == statbuf.st_uid) ? (S_IWUSR | S_IXUSR) & file_mode
			: egid_proc == statbuf.st_gid ? (S_IWGRP | S_IXGRP) & file_mode : (S_IWOTH | S_IXOTH) & file_mode;
	else
	{
		/* if the process is priviledged, a directory must have an execute permission in any of the field before it is granted access */
		if (S_ISDIR(statbuf.st_mode) && !(file_mode & S_IXUSR || file_mode & S_IXGRP || file_mode & S_IXOTH))
			s_perm = 0;
		else s_perm = 1;
	}
	return s_perm;
}

void mv_directory_func(char *a, char *b)
{
	int fd;
	int file_mode;
	char n_path[PATH_MAX];
	int permis;
	struct stat statbuf;

	if (a == NULL || *a == 0)
	{
		puts("error empty path");
		exit(EXIT_FAILURE);
	}
	fd = open(a, O_PATH);
	TST_LOGFUN(fd, -1, perror("open"));
	TST_LOGFUN(fstat(fd, &statbuf), -1, perror("stat"));

	permis = faccesswx(fd, a);
/*	if (s_perm == 0)
	{
		puts("permission denied");
		exit(-1);
		} */
fs_mdfy:
	if (S_ISDIR(statbuf.st_mode))
	{
		/* if sticky bit is set on the dir and the dir doesnt belong to the process, then we dont have access */
		//if ((file_mode & S_ISVTX) && !(PROC_OWNED(statbuf, euid_proc)))
		//	{
		//	puts("no access");
		//	exit(-1);
			/* no access */
		//}
	}
/*	if (S_ISREG); */
	/* x - y */
	if (close(fd) == -1)
	{
		perror("close");
		exit(-1);
	}
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
	mv_directory_func("new", "./new2");
	return (0);
}
	/* also change timestamp with utimes in mv*/
/* add umask builtin */
