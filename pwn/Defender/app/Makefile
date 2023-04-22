#gcc -m32 main.c -o test -fno-stack-protector -w -no-pie

FLAGS := -fno-stack-protector -w -no-pie -m32
CC := gcc

SOURCES := src/main.c

.PHONY: all docker

all: $(SOURCES)
	$(CC) $(SOURCES) -o bin/defender $(FLAGS)

docker:
	$(CC) /main.c -o /defender $(FLAGS)
