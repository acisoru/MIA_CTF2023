#define _XOPEN_SOURCE 500
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <ftw.h>
#include <unistd.h>

#define SHERLOCK_LOGO              \
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n" \
"⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⣾⣿⣿⣷⣶⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n" \
"⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n" \
"⠀⠀⠀⠀⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n" \
"⠀⠀⠀⠀⠀⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀\n" \
"⠀⠀⠀⠀⠾⠿⠿⠟⠛⠛⠛⠛⠛⣛⣛⣛⣛⣛⡛⠛⠛⠛⠂⠀⠀⠀⠀⠀⠀⠀\n" \
"⠀⠀⠀⠀⠀⠀⠰⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⢀⣤⣤⡀⠀⠀\n" \
"⠀⠀⠀⠀⢀⣴⡄⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠀⢀⣾⠋⡈⢿⡄⠀\n" \
"⠀⠀⢠⣾⣿⣿⣿⣦⡀⠻⢿⣿⣿⣿⣿⣿⣿⠛⠛⠃⠀⠀⠀⣼⡇⠀⠁⢸⡇⠀\n" \
"⠀⣠⣤⣤⣌⣉⠙⠻⢿⣦⣄⠙⠻⠿⣿⡿⠃⠰⣦⠀⠀⠀⠀⣿⡄⠀⠀⣼⠇⠀\n" \
"⠀⣿⣿⣿⣿⣿⣿⣶⣤⣈⠛⢿⣶⣄⠀⠀⠀⠀⢸⠇⠀⠀⠀⠸⣧⣀⣰⠏⠀⠀\n" \
"⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡈⠛⢷⠀⠀⠀⣾⠀⠀⠀⠀⠀⢸⡿⠁⠀⠀⠀\n" \
"⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣄⠀⠀⢸⣿⣿⣷⣦⠀⠀⢸⡇⠀⠀⠀⠀\n" \
"⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⠘⠿⣿⠿⠋⠀⠀⣸⡇⠀⠀⠀⠀\n" \
"⠀⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠀⠀⠀⠀⠀⠀⠀⠀⠛⠁⠀⠀⠀⠀\n" \

#define SERVER_PORT 9004

#define BUF_SIZE 64

#define FILES_DIR "./sherlock_files"

#define FILE_PREFIX        FILES_DIR "/Sherlock-"
#define FILE_SIZE          256
#define FILE_POSTFIX_SIZE  7
#define FILE_POSTFIX_IDENT (sizeof(FILE_PREFIX) - 1)
#define FILE_NAME_LEN      (FILE_POSTFIX_IDENT + FILE_POSTFIX_SIZE)

#define ANSWER_OK      "ok"
#define ANSWER_OK_SIZE sizeof(ANSWER_OK)

#define INPUT_SIZE 2

static int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
	return remove(fpath);
}

int rmrf(const char *path)
{
	return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

char* generate_file_name(void)
{
	static char fname[BUF_SIZE] = { FILE_PREFIX };

	static const char charset[] = "ABCDEFGHIJK";

	for (int i = FILE_POSTFIX_IDENT; i < FILE_NAME_LEN; i++)
	{
		int key = rand() % (int) (sizeof(charset - 1));
		fname[i] = charset[key];
	}

	return fname;
}

int save_file(const uint8_t* file, const size_t size, const char* fname)
{
	int fd = open(fname, O_WRONLY | O_CREAT, 0666);
	if (fd == -1)
	{
		printf("<%s> open '%s' failed: %s\n", __func__, fname, strerror(errno));
		return -1;
	}

	if (write(fd, file, size) == -1)
	{
		printf("<%s> write failed: %s\n", __func__, strerror(errno));
		return -1;
	}

	return 0;
}

int sherlock_work(const int sockfd)
{
	int steps = 0;

	if (recv(sockfd, &steps, sizeof(int), 0) == -1)
	{
		printf("<%s> recv failed: %s\n", __func__, strerror(errno));
		return -1;
	}

	steps++;

	for (int step = 1; step < steps; step++)
	{
		printf("Step %d.\n", step);

		uint8_t file[FILE_SIZE];

		ssize_t bytes = recv(sockfd, file, sizeof(file), 0);
		if (bytes == -1)
		{
			printf("<%s> recv failed: %s\n", __func__, strerror(errno));
			return -1;
		}

		const char* fname = generate_file_name();

		if (save_file(file, sizeof(file), fname))
		{
			printf("<%s> save file '%s' failed\n", __func__, fname);
			return -1;
		}

		printf("Get file '%s'. Is it Elf?\n", fname);
		printf("[y/n] > ");

		char input[INPUT_SIZE];

		read(0, input, sizeof(input));

		if (send(sockfd, &input[0], sizeof(char), 0) == -1)
		{
			printf("<%s> send '%s' failed: %s\n", __func__, input, strerror(errno));
			return -1;
		}

		char answer[ANSWER_OK_SIZE];
		if (recv(sockfd, answer, sizeof(answer) - 1, 0) == -1)
		{
			printf("<%s> recv failed: %s\n", __func__, strerror(errno));
			return -1;
		}

		if (!strncmp(answer, ANSWER_OK, sizeof(answer))) continue;

		printf("Wrong answer :(\n");

		return 0;
	}

	char flag[BUF_SIZE] = { 0 };

	if (recv(sockfd, flag, sizeof(flag), 0) == -1)
	{
		printf("<%s> recv failed: %s", __func__, strerror(errno));
		return -1;
	}

	printf("Sherlock, you found all the Elfs, thanks!\n");

	printf("%s", flag);

	return 0;
}

int init_socket(const in_addr_t ip_addr)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("<%s> create socket failed: %s\n", __func__, strerror(errno));
		return -1;
	}

	struct sockaddr_in servaddr = { 0 };

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = ip_addr;
	servaddr.sin_port = htons(SERVER_PORT);

	if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)))
	{
		printf("<%s> connection to server failed: %s\n", __func__, strerror(errno));
		return -1;
	}

	return sockfd;
}

int init_files_dir()
{
	rmrf(FILES_DIR);

	if (mkdir(FILES_DIR, 0777))
	{
		printf("<%s> mkdir '%s' failed: %s\n", __func__, FILES_DIR, strerror(errno));
		return -1;
	}

	return 0;
}

void print_logo()
{
	printf(SHERLOCK_LOGO);
	printf("Hey sherlock! Could you help us to find all the Elfs?\n");
}

int main(int argc, char** argv)
{
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	srand(time(NULL));

	if (argc != 2)
	{
		printf("Usage: ./client <server_ip_address>\n");
		return 0;
	}

	in_addr_t ip_addr = inet_addr(argv[1]);
	if (ip_addr == -1)
	{
		printf("Invalid IP address: '%s'\n", argv[1]);
		return -1;
	}

	print_logo();

	int sockfd = init_socket(ip_addr);
	if (sockfd == -1)
	{
		printf("Init socket failed\n");
		return -1;
	}

	if (init_files_dir())
	{
		printf("Initialize files directory failed\n");
		return -1;
	}

	if (sherlock_work(sockfd))
	{
		printf("Main Sherlock's work failed\n");
		return -1;
	}

	close(sockfd);

	return 0;
}
