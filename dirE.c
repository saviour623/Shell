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

int faccesswx(int fd)
{
}
char *path_ncpy(char *dest, const char *src)
{
	register int c;

	if (src == NULL)
	{
		errno = EINVAL;
		return NULL;
	}
	if (dest == NULL && *src != 0)
	{
		dest = malloc(strlen(src) * sizeof (char));
		if (dest == NULL)
		{
			errno == ENOMEM;
			return NULL;
		}
	}
	for (; len && (c = *src); len--, *src++)
	{
		if ((c == '/') && (*(src + 1) == '/'))
			continue;
		*dest = *src;
	}
	return dest;
}
__attribute__((nonnull)) static char *cpy_ppath(char *path)
{
	static char paren_path[PATH_MAX];
	register int oo = 0, c = 0;
	register size_t len = strlen(path), tmlen = len - 1;

	if (len > PATH_MAX || *path == 0)
		return NULL;

	for (; (c = path[tmlen--]) && c != '/'; oo++)
		;;
	len -= oo;

	if 
	strncpy(paren_path, path, len);
	return paren_path;
}
void mv_directory_func(char *a, char *b)
{
	/* also change timestamp with utimes */
	struct stat statbuf;
	//char fName = info->cmd;
	int fd;
	mode_t file_mode, s_perm;
	char n_path[PATH_MAX];

	seteuid(0);
	setegid(0);
	uid_t euid_proc = geteuid();
	gid_t egid_proc = getegid();

	fd = open("../empty_test/", O_PATH);
	if (fd == -1)
	{
		perror("open");
		exit(-1);
	}
	if (fstat(fd, &statbuf) == -1)
	{
		perror("stat");
		exit(-1);
	}
	file_mode = statbuf.st_mode;
#if defined(__linux__) && defined(_SYS_FSUID_H)
    s_perm = (setfsuid(-1) == statbuf.st_uid) ? (S_IWUSR | S_IXUSR) & file_mode
		: setfsgid(-1) == statbuf.st_gid ? (S_IWGRP | S_IXGRP) & file_mode : 0;
	if (s_perm)
		goto fs_mdfy;
#endif
	if (euid_proc != 0)
		s_perm = (euid_proc == statbuf.st_uid) ? (S_IWUSR | S_IXUSR) & file_mode
			: egid_proc == statbuf.st_gid ? (S_IWGRP | S_IXGRP) & file_mode : (S_IWOTH | S_IXOTH) & file_mode;
	else
		s_perm = 1;

	if (s_perm == 0)
	{
		puts("permission denied");
		exit(-1);
	}
fs_mdfy:
	if (S_ISDIR(statbuf.st_mode))
	{
		/* if sticky bit is set on the dir and the dir doesnt belong to the process, then we dont have access */
		if ((file_mode & S_ISVTX) && !(PROC_OWNED(statbuf, euid_proc)))
		{
			puts("no access");
			exit(-1);
			/* no access */
		}
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
	char path[PATH_MAX];
	char *p = cpy_ppath("/h/p/k/p/g/my_path");
	if (p != NULL)
		puts(p);
//	printf("%s\n", realpath("/h/b/t/y", path));
	mv_directory_func("./new", "./new2");
	return (0);
}

/* add umask builtin */
