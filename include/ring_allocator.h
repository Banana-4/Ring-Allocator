#pragma once
#include <stdlib.h>

typedef int ALIGN;

typedef union Header {
  struct {
    union Header *next;
    size_t size;
  } metadata;
  ALIGN x;
} Header;

void *ring_alloc(size_t n);
void ring_free(void *ptr);
