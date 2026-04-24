#include "../include/ring_allocator.h"
#include "../include/internal.h"


 size_t align_up(size_t size) {
    return (size + alignof(max_align_t) - 1) & ~(alignof(max_align_t) - 1);
}


static Header *last = NULL;

void *ring_alloc(size_t n) {
    if (n == 0)
        return NULL;

  Header *p, *q;
  n = align_up(n + sizeof(Header));
  if (!last) {
      last = morecore(align_up(n + BLOCKSIZE));
      if (!last) {
          return NULL;
      }
  }

  for (p = last, q = p->next;;
       p = p->next, q = q->next) {
    if (q->size >= n) {
      if (q->size == n) {
        p->next = q->next;
        q->size -= n;
      } else {
          q->size -= n;
          q = (Header *)((char *)q + q->size);
          q->size = n;
      }
      break;
    }
    if (q == last) {
      Header *new = morecore(align_up(n + BLOCKSIZE));
      if (!new) {
        return NULL;
      }
      ring_free(new + 1);
      last = new;
    }
  }
  return (void *)(q + 1);
}

void ring_free(void *ptr) {
    if (!ptr) {
        return;
    }
    Header *block = (Header *)ptr - 1;
    Header *p = last;

    for (; !(block > p && block < p->next); p = p->next) {
        if (p >= p->next && (p > block || block < p->next)) {
            break;
        }
    }
    if ((char *)p + p->size == (char *)block) {
        p += block->size;
        block = p;
    }
    if ((char *)p->next == (char *)block + block->size) {
        block->size += p->next->size;
        block->next = p->next->next;
    } else {
        block->next = p->next;
        p->next = block;
    }
}

void print_debug() {
    Header *p = last;
    while(p) {
        printf("[ Block: %p | Size: %zu | Next: %p ]\n", p, p->size, p->next);
        p = p->next;
        if (p == last) {
            break;
        }
    }
}
