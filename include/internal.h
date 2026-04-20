#pragma once
#include "ring_allocator.h"
#define BLOCKSIZE 128 * sizeof(Header)

inline size_t nunits(size_t n) {
  return 1 + ((n * sizeof(Header) - 1) / sizeof(Header));
}

void *morecore(size_t n);
