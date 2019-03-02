C=gcc
CFLAGS=-std=c11 -O3 -g -pedantic-errors

clean:
	rm *.o

all: lab7

lab7: *.asm *.c
	nasm -f elf64 image_sepia_sse.asm
	$(CC) $(CFLAGS) -c *.c
	$(CC) -o lab7  *.o -no-pie
