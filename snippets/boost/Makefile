#----------------------------------------------------------------------------------------
# Makefile
# by Hannes Badertscher
# Licensed under CC-BY-NC-SA

CXX=g++
CC=gcc
CXXFLAGS=-c
CFLAGS=-c
RM=rm -f

SRCS=boost.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
EXECUTABLE=boost

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(OBJS) -o $(EXECUTABLE)


clean:
	$(RM) $(OBJS)