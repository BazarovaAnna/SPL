#ifndef _CUS_ALL_H_
#define _CUS_ALL_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define MIN_BLOCK_SIZE 32

typedef struct chunk_header chunk_header;
char* start;

struct __attribute__((packed)) chunk_header {
  chunk_header* next;
  size_t capacity;
  bool is_free;
};

void* customalloc(size_t size);

#endif
