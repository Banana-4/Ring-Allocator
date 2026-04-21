#include "../include/ring_allocator.h"
#include "../include/internal.h"

static Header *last = NULL;

void *ring_alloc(size_t n) {
  Header *p, *q;
  n += sizeof(Header);
  if (!last) {
    last = morecore(n);
  }

  for (p = last, q = p->metadata.next;;
       p = p->metadata.next, q = q->metadata.next) {
    if (q->metadata.size >= n) {
      if (q->metadata.size == n) {
        p->metadata.next = q->metadata.next;
      } else {
        q->metadata.size -= n;
        q += q->metadata.size;
        q->metadata.next = NULL;
        q->metadata.size = n;
      }
      break;
    }
    if (q == last) {
      Header *new = morecore(n);
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
  for (p = last, q = last->metadata.next;;
       p = p->metadata.next, q = q->metadata.next) {
    if (p < ptr && q > ptr) {
      if (((void *)p + p->metadata.size) == ptr) {
        p->metadata.size += ptr->metadata.size;
        ptr = p;
      }
      if (((void *)ptr + ptr->metadata.size) == q) {
        ptr->metadata.size += q->metadata.size;
        ptr->metadata.next = q->metadata.next;
        q = ptr;
      }
      if (ptr != p) {
        p->metadata.next = ptr;
      }
      if (q != ptr) {
        ptr->metadata.next = q;
      }
      break;
    }
  }
}
