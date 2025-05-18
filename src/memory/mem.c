#include "mem.h"
#include <stdint.h>
#include <sys/mman.h>

static struct block_header *heap_start = NULL;

void *
heap_init (size_t size)
{
  if (NULL != heap_start)
    return NULL;
  size_t sz = size < MIN_BLOCK_SIZE ? MIN_BLOCK_SIZE : size;
  sz += sizeof (struct block_header);

  void *heap_addr = mmap (HEAP_START, sz, PROT_READ | PROT_WRITE,
                          MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (MAP_FAILED == heap_addr)
    {
      return NULL;
    }

  struct block_header header = {
    .length = sz - sizeof (struct block_header),
    .is_free = true,
    .next = NULL,
  };

  *(struct block_header *)heap_addr = header;
  heap_start = (struct block_header *)heap_addr;

  return heap_addr;
}

static void
add_block_to_list (const struct block_header *const block)
{
  struct block_header *cur;
  for (cur = heap_start; cur->next != NULL; cur = cur->next)
    {
      ;
      ;
    }

  cur->next = (struct block_header *)block;
}

static struct block_header *
split_block (struct block_header *const header, size_t size)
{
  header->length -= size + sizeof (struct block_header);
  struct block_header splitted
      = { .length = size, .is_free = false, .next = header->next };

  uintptr_t block_header_addr
      = (uintptr_t)header + sizeof (struct block_header) + header->length;
  *(struct block_header *)block_header_addr = splitted;
  header->next = (struct block_header *)block_header_addr;

  return (struct block_header *)block_header_addr;
}

void *
_alloc (size_t size)
{
  size_t total_sz = size + sizeof (struct block_header);
  if (NULL == heap_start && NULL == heap_init (size))
    {
      return NULL;
    }

  for (struct block_header *cur = heap_start; NULL != cur; cur = cur->next)
    {
      if (cur->is_free
          && size <= cur->length - sizeof (struct block_header)
                         - MIN_BLOCK_SIZE)
        {
          struct block_header *block_header_addr = split_block (cur, size);
          return (void *)((uintptr_t)block_header_addr
                          + sizeof (struct block_header));
        }
    }

  if (total_sz < MIN_BLOCK_SIZE + sizeof (struct block_header))
    {
      total_sz = MIN_BLOCK_SIZE + sizeof (struct block_header);
    }

  void *block_addr = mmap (NULL, total_sz, PROT_READ | PROT_WRITE,
                           MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (MAP_FAILED == block_addr)
    {
      return NULL;
    }

  struct block_header free_header = {
    .length = total_sz - sizeof (struct block_header),
    .is_free = true,
    .next = NULL,
  };
  *(struct block_header *)block_addr = free_header;
  add_block_to_list ((struct block_header *)block_addr);
  uintptr_t block_header_addr
      = (uintptr_t)split_block ((struct block_header *)block_addr, size);

  return (void *)(block_header_addr + sizeof (struct block_header));
}

/*
static void merge_blocks(struct block_header* a, struct block_header* b) {
  // Not implemented
}
*/

void
_free (void *ptr)
{
  uintptr_t addr = (uintptr_t)ptr;
  for (struct block_header *cur = heap_start; NULL != cur->next;
       cur = cur->next)
    {
      uintptr_t base = (uintptr_t)cur + sizeof (struct block_header);
      uintptr_t bounds = base + cur->length;
      if (addr >= base && addr < bounds)
        {
          cur->is_free = true;
          return;
        }
    }
}

void
print_block_header (FILE *f, const struct block_header *const block)
{
  fprintf (f, "block at %p\n", (void *)block);
  fprintf (f, "length %zu\n", block->length);
  fprintf (f, "is_free %i\n", block->is_free);
  fprintf (f, "next %p\n", (void *)block->next);
  fprintf (f, "\n");
}

void
debug_heap_blocks (FILE *f)
{
  for (const struct block_header *cur = heap_start; cur; cur = cur->next)
    {
      print_block_header (f, cur);
    }
}