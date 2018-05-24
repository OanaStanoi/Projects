# Makefile for "HackerNewStatistics" application
# Created by Oana Stanoi 18/05/2018

PROG = HackerNewsStatistics
CC = g++
CPPFLAGS = -w -g -Wall

OBJS = main.o hn_statistics.o performance.o

$(PROG) : $(OBJS)
	$(CC) $(CPPFLAGS) -o $(PROG) $(OBJS)

main.o :
	$(CC) $(CPPFLAGS) -c main.cpp

hn_statistics.o : hn_statistics.h
	$(CC) $(CPPFLAGS) -c hn_statistics.cpp

performance.o : performance.h
	$(CC) $(CPPFLAGS) -c performance.cpp

clean:
	rm -f core $(PROG) $(OBJS)

