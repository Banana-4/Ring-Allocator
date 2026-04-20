#include "../include/ring_allocator.h"
#include "../include/internal.h"

void *ring_alloc(size_t n) {
  Header *p, *q;
  static Header *last = NULL;
  if (!last) {
    last = morecore(n);
  }

  for (p = last, q = p->metadata.next;;
       p = p->metadata.next, q = q->metadata.next) {
    if (q->metadata.size >= n) {
      if (q->metadata.size == n) {
        p->metadata.next = q->metadata.next;
        last = p;
      } else {
        last = (void *)(q + 1) + n;
        last->metadata.size -= n;
        last->metadata.next = q->metadata.next;
        p->metadata.next = last;
        q->metadata.size = n;
      }
      q->metadata.next = NULL;
      break;
    }
    if (q == last) {
      last = morecore(n);
      if (!last) {
        return NULL;
      }
      ring_free(last);
    }
  }

  return q + 1;
}
