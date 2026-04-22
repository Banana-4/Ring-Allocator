#include "../include/internal.h"

void *morecore(size_t n) {
  Header *block = mmap(NULL, n, PROT_WRITE | PROT_READ,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (block == MAP_FAILED) {
    return NULL;
  }
  block->size = n;
  block->next = block;
  return block;
}
