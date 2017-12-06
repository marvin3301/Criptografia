CFLAGS = -g -Wall
LDFLAGS = -g
CC = gcc
LIBS_PATH = -L.
LDLIBS = $(LIBS_PATH) -lm

test: test.o auto.a rsa.h

auto.a: rsa.o
	ar rc auto.a rsa.o
	ranlib auto.a

rsa.o: rsa.c rsa.h
	gcc -c rsa.c

.PHONY: clean, all

clean:
	rm -f *.o a.out rsa.o rsa

all: clean rsa
