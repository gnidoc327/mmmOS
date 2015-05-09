#
# Makefile for SThreads
#

CC=gcc
#CFLAGS=-g -I/usr/src/kernels/2.6.29.5-191.fc11.i686.PAE/include/ -O2 -Wall
LDFLAGS=-g
CFLAGS=-DX86 -m32 -msse2 -mfpmath=sse -O2

#.c.s:
#	$(CC) $(CFLAGS) $(CPPFLAGS) -S $<

sthread.o: sthread.c 
	$(CC) $(CFLAGS) -c -o $@ sthread.c

sync.o: sync.c sync.h
	$(CC) $(CFLAGS) -c -o $@ sync.c

sample.o: sample.c
	$(CC) $(CFLAGS) -c -o $@ sample.c

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c -o $@ st_queue.c

libsthread.a: sthread.o sync.o
	$(AR) cr $@ $^

sample: sample.o libsthread.a
	$(CC) -o $@ sample.o -L. -lsthread

clean:
	$(RM) libsthread.a sample *.o *~
