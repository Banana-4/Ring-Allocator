#include "../include/ring_allocator.h"
#include "../include/internal.h"

inline size_t align_up(size_t size) {
    return (size + alignof(max_align_t) - 1) & ~(alignof(max_align_t) - 1);
}


static Header *last = NULL;

void *ring_alloc(size_t n) {
    if (n == 0)
        return NULL;

  Header *p, *q;
  n = align_up(n + sizeof(Header));
  if (!last) {
      last = morecore(n >= BLOCKSIZE ? n : align_up(n + BLOCKSIZE) );
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
        q->next = NULL;
        q->size = n;
      }
      break;
    }
    if (q == last) {
      Header *new = morecore(n >= BLOCKSIZE ? n : align_up(n + BLOCKSIZE) );
      if (!new) {
        return NULL;
      }
      last = new;
      ring_free(last);
    }
  }
  return (void *)(q + 1);
}

void ring_free(Header *ptr) {
  if (!ptr) {
    return;
  }
  Header *q, *p;
  for (p = last, q = last->next;;
       p = p->next, q = q->next) {
    if (p <= ptr && q >= ptr) {
      if (((char *)p + p->size) == (char *)ptr) {
        p->size += ptr->size;
        ptr = p;
      }
      if ((char *)q == (char *)ptr + ptr->size) {
        ptr->size += q->size;
        ptr->next = q->next;
        if (ptr != p) {
          p->next = ptr;
        }
      }

      if (p->next != ptr && ptr != p) {
        p->next = ptr;
        ptr->next = q;
      }
      break;
    }
  }
}
