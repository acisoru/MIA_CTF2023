FLAGS := -Wall
CC := gcc

SOURCES := src/main.c

.PHONY: all docker

all: $(SOURCES)
	$(CC) $(SOURCES) -o bin/monolith $(FLAGS)

docker:
	$(CC) /main.c -o /monolith $(FLAGS)
