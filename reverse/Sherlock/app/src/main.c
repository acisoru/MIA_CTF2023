#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <inttypes.h>

#include "helpers.h"

#define FLAG  "files/flag.txt"
#define ELF   "files/elf"
#define ZIP   "files/zip"
#define TARGZ "files/targz"

#define FILE_SIZE_MAX 256

#define FILES_MAX 444

#define FLAG_SIZE 32

#define ANSWER_YES 'y'
#define ANSWER_NO  'n'

static int8_t get_file_data(file_t * file)
{
	int8_t rcode = -1;

	FILE * f = fopen(file->name, "rb");
	if (NULL == f)
	{
		MSG_ERR("<%s> fopen(%s) failed: %s\n", __func__, file->name, strerror(errno));
		goto exit;
	}

	int rc = fseek(f, 0, SEEK_END);
	if (-1 == rc)
	{
		MSG_ERR("<%s:%d> fseek() failed: %s\n", __func__, __LINE__, strerror(errno));
		goto exit;
	}

	file->size = ftell(f);
	if (0 == file->size)
	{
		MSG_ERR("<%s> ftell() failed: %s\n", __func__, strerror(errno));
		goto exit;
	}

	rc = fseek(f, 0, SEEK_SET);
	if (-1 == rc)
	{
		MSG_ERR("<%s:%d> fseek() failed: %s\n", __func__, __LINE__, strerror(errno));
		goto exit;
	}

	if (file->size > FILE_SIZE_MAX)
	{
		MSG_ERR("<%s> large size '%s' (%zu)\n", __func__, file->name, file->size);
		goto exit;
	}

	file->data = malloc(FILE_SIZE_MAX);
	if (NULL == file->data)
	{
		MSG_ERR("<%s> malloc() failed: %s\n", __func__, strerror(errno));
		goto exit;
	}

	rc = fread(file->data, sizeof(uint8_t), file->size, f);
	if ((rc <= 0) || (rc > file->size))
	{
		MSG_ERR("<%s> fread() failed: %s\n", __func__, strerror(errno));
		goto exit;
	}

	size_t byte = (file->size < FILE_SIZE_MAX) ? (file->size + 1) : file->size;
	for (; byte < FILE_SIZE_MAX; byte++)
	{
		file->data[byte] = rand() % 256;
	}

	rcode = 0;

exit:
	if (f) fclose(f);

	return rcode;
}

static int8_t create_files_order(uint8_t * files_order, const size_t files_count)
{
	for (int idx = 0; idx < FILES_MAX; idx++)
	{
		files_order[idx] = rand() % files_count;
	}

	return 0;
}

static int8_t start_game(
		const file_t * files,
		const size_t   files_count,
		const char   * flag)
{
	int files_max = FILES_MAX;

	SEND_DATA(&files_max, sizeof(int));

	uint8_t files_order[FILES_MAX];

	if (create_files_order(files_order, files_count))
	{
		MSG_ERR("<%s> create_levels() failed\n", __func__);
		return -1;
	}

	for (int idx = 0; idx < sizeof(files_order); idx++)
	{
		const file_t * file = files + files_order[idx];

		SEND_DATA(file->data, FILE_SIZE_MAX);

		char answer;

		READ_ANSWER(&answer);

		if (Elf == file->type)
		{
			if (answer != ANSWER_YES)
			{
				MSG_INFO("incorrect");
				return 0;
			}
		}
		else
		{
			if (answer == ANSWER_YES)
			{
				MSG_INFO("incorrect");
				return 0;
			}
		}

		MSG_INFO("ok");
	}

	MSG_INFO("%s", flag);

	return 0;
}

static int8_t init_files(file_t * files, const size_t count)
{
	for (int idx = 0; idx < count; idx++)
	{
		if (get_file_data(&files[idx]))
		{
			MSG_ERR("<%s> get_file_data() failed\n", __func__);
			return -1;
		}
	}

	return 0;
}

static void free_files(file_t * files, const size_t files_count)
{
	for (int idx = 0; idx < files_count; idx++)
	{
		free(files[idx].data);
	}
}

static int8_t init_flag(char * flag)
{
	file_t flag_file = { .name = FLAG };

	if (get_file_data(&flag_file))
	{
		MSG_ERR("<%s> get_file_data() failed\n", __func__);
		return -1;
	}

	memcpy(flag, flag_file.data, FLAG_SIZE);

	free(flag_file.data);

	return 0;
}

int main(int argc, char ** argv)
{
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	srand(time(NULL));

	file_t files[] = {
		{ .name = ELF, .type = Elf, .data = NULL, .size = 0 },
		{ .name = ZIP, .type = Zip, .data = NULL, .size = 0 },
		{ .name = TARGZ, .type = Targz, .data = NULL, .size = 0 },
	};

	const size_t files_count = sizeof(files) / sizeof(files[0]);

	if (init_files(files, files_count))
	{
		MSG_ERR_MAIN();
		goto exit;
	}

	char flag[FLAG_SIZE] = { 0 };

	if (init_flag(flag))
	{
		MSG_ERR_MAIN();
		goto exit;
	}

	if (start_game(files, files_count, flag))
	{
		MSG_ERR_MAIN();
		goto exit;
	}

exit:
	free_files(files, files_count);

	return 0;
}
