#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define SEND_DATA(__data__, __size__) write(1, __data__, __size__)
#define READ_ANSWER(__buf__)          read(0, __buf__, sizeof(char))

#define MSG_INFO(__fmt__, ...) printf(__fmt__, ##__VA_ARGS__)
#define MSG_ERR(__fmt__, ...)  fprintf(stderr, __fmt__, ##__VA_ARGS__)
#define MSG_ERR_MAIN()         printf("Please, send error log to @bigger777\n");

typedef enum
{
	Elf,
	Zip,
	Targz,
} file_type_t;

typedef struct file_s
{
	char* name;
	uint8_t * data;
	size_t size;
	file_type_t type;
} file_t;

#endif // _HELPERS_H_
