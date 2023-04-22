#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define FLAG_FILE "flag.txt"

#define HASH "24e5fc322c0c9fddea5b85e2f40c404e"

#define BUF_SIZE 33

int get_flag(char * flag, size_t size)
{
	int ret = -1;

	int fd = open(FLAG_FILE, O_RDONLY);
	if (-1 == fd)
	{
		printf("open '%s' failed: %s\n", FLAG_FILE, strerror(errno));
		return -1;
	}

	int rc = read(fd, flag, size);
	if ((-1 == rc) || (rc > size))
	{
		printf("read failed: %s\n", strerror(errno));
		goto error;
	}

	ret = 0;

error:
	close(fd);
	return ret;
}

int check(char * flag)
{
	char buf[BUF_SIZE] = { 0 };

	if (!fgets(buf, BUF_SIZE, stdin))
	{
		printf("fgets failed\n");
		return -1;
	}

	printf("%s\n", !strcmp(buf, HASH) ? flag : "NOP");

	return 0;
}

int main(int argc, char ** argv)
{
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	char flag[BUF_SIZE] = { 0 };

	if (get_flag(flag, BUF_SIZE))
	{
		printf("get_flag failed\n");
		return -1;
	}

	if (check(flag))
	{
		printf("check failed\n");
		return -1;
	}

	return 0;
}
