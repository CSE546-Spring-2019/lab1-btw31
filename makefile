# Makefile for count.c

CC = gcc
CFLAGS = -g -Wall

all: count

lab1: count.c
	$(CC) $(CFLAGS) -o count count.c

clean: 
	rm count
