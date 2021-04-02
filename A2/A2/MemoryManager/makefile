# NAME : Rushabh Prajapati
# ASSIGNMENT 2
# INSTRUCTOR : Calin Anton
# LAB INSTRUCTOR: Saleh Hanan
# PURPOSE: makefile for creating an executable

CC = gcc
CFLAGS = -Wall -ansi -std=c99 -g

all: main.o memManager.o
	$(CC) $(CFLAGS) -o main main.o memManager.o

main.o : main.c
	$(CC) $(CFLAGS) -c main.c

memManager.o : memManager.c
	$(CC) $(CFLAGS) -c memManager.c

clean : *.o main
	rm $^

test:
	(cd testing; make test; make text)

A2.tar.gz: main.c memManager.c memManager.h makefile testing README design.txt
	tar -czvf A2.tar.gz $^
