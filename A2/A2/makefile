CC = gcc
CFLAGS = -Wall -ansi -std=c99 -g

main.o : main.c
	$(CC) $(CFLAGS) -c main.c

memManager.o : memManager.c
	$(CC) $(CFLAGS) -c memManager.c

main: main.o memManager.o
	$(CC) $(CFLAGS) -o main main.o memManager.o

clean : *.o main
	rm $^

tar: main.c memManager.c memManager.h makefile design.txt README testing
	tar -czvf assignment2.tar.gz $^

