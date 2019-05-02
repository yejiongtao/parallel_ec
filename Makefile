DEBUG=0
#CC=gcc
CC=/afs/andrew.cmu.edu/usr17/jiongtay/private/gcc/bin/gcc
OMP=-fopenmp -DOMP
CFLAGS=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=gnu99
LDFLAGS= -lm

CFILES = benchmark.c coding_loop.c cycletimer.c galois.c matrix.c reed_solomon.c
HFILES = coding_loop.h cycletimer.h galois.h matrix.h reed_solomon.h

all: $(CFILES) $(HFILES)
	$(CC) $(CFLAGS) $(OMP) -o benchmark $(CFILES) $(LDFLAGS)