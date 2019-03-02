CC=gcc

CFLAGS=-std=c11 -g -Wall -Ofast -march=native



all: lab7



lab7: *.s *.c

	nasm -g -F dwarf -f elf64 image_sepia_sse.s

	nasm -g -F dwarf -f elf64 image_sepia_avx.s

	$(CC) $(CFLAGS) -c *.c

	$(CC) -ggdb -o lab7 *.o

	@rm *.o
