CC = gcc
CFLAGS = -Wall -ansi -std=c99 -g


test.o : test.c
	$(CC) $(CFLAGS) -c test.c

memManager.o : memManager.c memManager.h
	$(CC) $(CFLAGS) -c memManager.c memManager.h

test: test.o memManager.o
	$(CC) $(CFLAGS) -o test test.o memManager.o

clean : *.o test
	rm $^

tar: test.c memManager.c memManager.h makefile Milestone1.5_RP.txt
	tar -czvf assignment2.tar.gz $^
