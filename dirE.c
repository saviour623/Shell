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
#define IS_RDWRXTE(PERM) (PERM & (S_IRUSR | S_IWUSR | S_IXUSR))
void mv_directory_func(char *a, char *b)
{
	/* also change timestamp with utimes */
	struct stat statbuf;
	//char fName = info->cmd;
	int fd;
	mode_t file_mode, s_perm;

	uid_t euid_proc = geteuid();
	gid_t gid_proc = getegid();

	fd = open("./bugs", O_PATH);
	if (fstat(fd, &statbuf) == -1)
	{
		perror("stat");
	}
	file_mode = statbuf.st_mode;
#ifdef __LINUX__
	/* compare against file_uid/gid instead of effective */
#endif
	if (eid_proc != 0)
		s_perm = (euid_proc == statbuf.st_uid) ? (S_IWUSR | S_IXUSR) & file_mode
			: gid_proc == statbuf.st_gid ? (S_IWGRP | S_IXGRP) & file_mode : (S_IWOTH | S_IXOTH) & file_mode;
	else s_perm = 1;

	if (s_perm == 0)
	{
		puts("permission denied");
		exit(-1);
	}
	if (S_ISDIR(statbuf.st_mode))
	{
		/* if sticky bit is set on the dir and the dir doesnt belong to the process, then we dont have access */
		if ((file_mode & S_ISVTX) && !(PROC_OWNED(statbuf)))
		{
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
	mv_directory_func("./new", "./new2");
	return (0);
}

/* add umask builtin */
