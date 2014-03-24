#----------------------------------------------------------------------------------------
# Makefile for MonteCarlo
# by Hannes Badertscher
# Licensed under CC-BY-NC-SA

CC=gcc
CFLAGS = -std=c99 -o -Wall -O2 -fopenmp
RM=rm -f

SRCS=makePI.c
EXECUTABLE=makePI


makePI:	makePI.c
	$(CC) $(CFLAGS) -o makePI makePI.c


clean:
	$(RM) $(OBJS)