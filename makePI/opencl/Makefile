#----------------------------------------------------------------------------------------
# Makefile for MonteCarlo
# by Hannes Badertscher
# Licensed under CC-BY-NC-SA

CC = gcc
CFLAGS = -std=c99 -g -O2 -Wall -I/opt/AMDAPP/include -I/opt/AMDAPP/include
RM = rm -f

SRCS=makePI.c

all:	makePI
makePI:	main.c 
	$(CC) main.c -o makePI -L/opt/AMDAPP/lib/x86_64 -lOpenCL
clean:
	$(RM) makePI
