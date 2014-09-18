CC = gcc
FLAGS = -ansi

test: test.c
	$(CC) $(FLAGS) test.c BSDsocket.c -o test
