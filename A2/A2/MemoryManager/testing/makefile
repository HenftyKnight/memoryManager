CC = gcc
CFLAGS = -Wall -ansi -std=c99 -g

testInput.o : testInput.c
	$(CC) $(CFLAGS) -c testInput.c

memManager.o : ../memManager.c
	$(CC) $(CFLAGS) -c ../memManager.c

test: testInput.o memManager.o
	$(CC) $(CFLAGS) -o test testInput.o memManager.o

clean : *.o test
	rm $^

.PHONY:
text:
	./test > testOutput
	diff correctOutput testOutput