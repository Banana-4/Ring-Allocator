#pragma once
#include <stdalign.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct Header {
    struct Header *next;
    size_t size;
} Header;

void *ring_alloc(size_t n);
void ring_free(Header *ptr);
