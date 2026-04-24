#include "../include/ring_allocator.h"

int main() {
    int *i = ring_alloc(20);
    int *j = ring_alloc(40);
    int *d = ring_alloc(sizeof(int) * 2);
    ring_free(d);
    print_debug();
}
