#----------------------------------------------------------------------------------------
# Makefile for MonteCarlo
# by Hannes Badertscher
# Licensed under CC-BY-NC-SA

CXX=g++
CC=gcc
CXXFLAGS=-c
CFLAGS=-c
RM=rm -f

SRCS=makePI.c
OBJS=$(subst .c,.o,$(SRCS))
EXECUTABLE=makePI

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) -o $(EXECUTABLE)


clean:
	$(RM) $(OBJS)