#----------------------------------------------------------------------------------------
# Makefile
# by Hannes Badertscher
# Licensed under CC-BY-NC-SA

CXX=g++
CC=gcc
CXXFLAGS=-c
CFLAGS=-c
RM=rm -f

SRCS=dev_rand.c
OBJS=$(subst .c,.o,$(SRCS))
EXECUTABLE=dev_rand

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) -o $(EXECUTABLE)


clean:
	$(RM) $(OBJS)