#----------------------------------------------------------------------------------------
# Makefile for MonteCarlo
# by Hannes Badertscher
# Licensed under CC-BY-NC-SA

CC = gcc
CCFLAGS = -std=c99 -g -O2 -Wall
RM = rm -f


linux:  makePI.linux
mac:    makePI.mac

makePI.linux:	main.c 
	$(CC) $(CCFLAGS) -I/opt/AMDAPP/include -I/opt/AMDAPP/include main.c -o makePI -lOpenCL -L/opt/AMDAPP/lib/x86_64

makePI.mac:	main.c 
	$(CC) $(CCFLAGS) main.c -o makePI -framework OpenCL

clean:
	$(RM) makePI %.mac %.linux

