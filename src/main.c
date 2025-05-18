#include "memory/mem.h"
#include <stdio.h>

int
main (void)
{
  printf ("sizeof struct block_header %zu\n", sizeof (struct block_header));
  int *ptr = _alloc (sizeof (int));
  int *ptr2 = _alloc (sizeof (int));
  *ptr = 2;
  *ptr2 = 3;
  printf ("%i + %i -> %i!\n", *ptr, *ptr2, *ptr + *ptr2);
  debug_heap_blocks (stdout);
  printf ("freeing memory\n");
  _free (ptr2);
  debug_heap_blocks (stdout);
  return 0;
}