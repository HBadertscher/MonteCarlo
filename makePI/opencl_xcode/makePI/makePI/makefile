#
# Makefile -- build the makePI example
#
# (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
#
CC = gcc
CFLAGS = -std=c99 -g -O2 -Wall -I../common -I/opt/AMDAPP/include

makePI:	makePI.c
	$(CC) $(CFLAGS) -o makePI makePI.c -L/opt/AMDAPP/lib/x86_64 -lOpenCL
