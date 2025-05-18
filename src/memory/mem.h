#ifndef MEM_H
#define MEM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define HEAP_START ((void *)0x404000)
#define MIN_BLOCK_SIZE 4096

#pragma pack(push, 1)
struct block_header
{
  size_t length;
  bool is_free;
  struct block_header *next;
};
#pragma pack(pop)

void *heap_init (size_t size);
void *_alloc (size_t size);
void _free (void *ptr);

void print_block_header_info (FILE *f,
                              const struct block_header *const header);
void debug_heap_blocks (FILE *f);

#endif // MEM_H
