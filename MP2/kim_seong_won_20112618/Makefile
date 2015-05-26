#
# Makefile for SThreads
#

CC=gcc
#CFLAGS=-g -I/usr/src/kernels/2.6.29.5-191.fc11.i686.PAE/include/ -O2 -Wall
LDFLAGS=-g
CFLAGS=-DX86 -m32 -msse2 -mfpmath=sse -O2

#.c.s:
#	$(CC) $(CFLAGS) $(CPPFLAGS) -S $<

all: sample

sthread.o: sthread.c 
	${CC} ${CFLAGS} -c $<

sync.o: sync.c 
	${CC} ${CFLAGS} -c $<

libsthread.a: sthread.o sync.o 
	${AR} cr $@ $<

sample.o: sample.c
	${CC} ${CFLAGS} -c $<

sample: sample.o libsthread.a
	${CC} ${CFLAGS} -o $@ sample.o -L. -lsthread

clean:
	${RM} libsthread.a sample *.o 
