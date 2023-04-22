#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define FLAG_FILE "flag.txt"

#define BUF_SIZE          32
#define BASE_BUF_SIZE     256
#define MONOLITH_BUF_SIZE (BASE_BUF_SIZE + BUF_SIZE)

int get_flag(char * flag, size_t size)
{
	int ret = -1;

	int fd = open(FLAG_FILE, O_RDONLY);
	if (-1 == fd)
	{
		printf("open '%s' failed: %s\n", FLAG_FILE, strerror(errno));
		return -1;
	}

	ssize_t rc = read(fd, flag, size);
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

int ask(char * monolith)
{
	printf("What can you wish for the Great Monolith?\n> ");

	ssize_t rc = read(0, monolith, MONOLITH_BUF_SIZE);
	if ((-1 == rc) || (rc > MONOLITH_BUF_SIZE))
	{
		printf("read failed: %s\n", strerror(errno));
		return -1;
	}

	printf("The Great Monolith is grateful for your wish: %s\n", monolith);

	return 0;
}

int main(int argc, char ** argv)
{
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	char monolith[MONOLITH_BUF_SIZE] = { 0 };

	if (get_flag(monolith + BASE_BUF_SIZE, MONOLITH_BUF_SIZE - BASE_BUF_SIZE))
	{
		printf("get_flag failed\n");
		return -1;
	}

	if (ask(monolith))
	{
		printf("saving failed\n");
		return -1;
	}

	return 0;
}
