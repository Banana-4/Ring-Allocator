#include <stdlib.h>

typedef int ALIGN;

typedef union Header {
  struct {
    union Header *next;
    size_t size;
  } metadata;
  ALIGN x;
} Header;

void *alloc(size_t n);
void free(void *ptr);
