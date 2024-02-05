# If OS is Windows set command to del else assume OS is Linux and set command to rm.
ifeq ($(OS), Windows_NT)
	command = del
else
	command = rm
endif

CC=g++
CFLAGS=-std=c++11 -Wall

convtoics: ConvToICS.cpp Makefile
	$(CC) $(CFLAGS) -o ConvToICS ConvToICS.cpp

clean:
	$(command) *.o
