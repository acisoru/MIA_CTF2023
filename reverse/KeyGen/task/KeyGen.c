#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_SEED_SIZE 22

#define KEY_PADDING_SIZE 10
#define KEY_SIZE (MAX_SEED_SIZE + KEY_PADDING_SIZE)
#define KEY_LENGTH (KEY_SIZE - 1)

#define DEV_RANDOM "/dev/random"

const char* help = "Usage: ./key_gen <seed>";

typedef struct
{
	char* data;
	size_t size;
	uint16_t rand;
} seed_t;

static int init_rand(uint16_t* value)
{
	int fd = open(DEV_RANDOM, O_RDONLY);
	if (!fd)
	{
		printf("open '%s' failed: %s\n", DEV_RANDOM, strerror(errno));
		return -1;
	}

	if (read(fd, value, sizeof(uint16_t)) == -1)
	{
		printf("read from '%s' failed: %s\n", DEV_RANDOM, strerror(errno));
		return -1;
	}

	return 0;
}

static void key_gen(seed_t* seed)
{
	char key[KEY_SIZE] = { 0 };

	int idx = 0;

	for (idx = 0; idx < seed->size; idx++)
	{
		char ch = seed->data[idx];

		key[idx] = (ch & 1) ? (ch & 0xFE) : (ch | 1);
	}

	srand(seed->rand);

	for (; idx < KEY_LENGTH; idx++)
	{
		key[idx] = (rand() % (0x7E - 0x30)) + 0x30;
	}

	printf("%s\n", key);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("%s\n", help);
		return -1;
	}

	seed_t seed = { argv[1], strlen(argv[1]) };

	if (seed.size > MAX_SEED_SIZE)
	{
		printf("Seed length so long (max %d)\n", MAX_SEED_SIZE);
		return -1;
	}

	if (init_rand(&seed.rand))
	{
		printf("init_rand failed\n");
		return -1;
	}

	key_gen(&seed);

	return 0;
}
