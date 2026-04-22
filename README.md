# Ring Allocator

## Overview
This project is a reimplementation of the allocator from Chapter 8 of K&R 1978 edition.
The purpose of the project is studying memory allocators and learning how to build projects.
This allocator is not a solution for any problems that arise in real world memory management and as such is not designed to be thread safe or secure.

The allocator uses a one way carousel for free memory blocks storage. The blocks in the carousel are sorted by the memory addresses in ascending order, the next pointers point to higher memory address except 
the last it points to the smallest memory address.
This organization solves the problem of how to coalescence freed memory blocks. 

### File organization:
``` te

ring-allocator/
│
├── include/
│   └── ring_alloc.h
│
├── src/
│   ├── ring_alloc.c
│   ├── morecore.c
│   └── internal.h
│
├── tests/
│   ├── test_basic.c
│   ├── test_fragmentation.c
│   └── test_stress.c
│
├── examples/
│   └── demo.c
│
├── Makefile
└── README.md


### Constraints:
  1. The allocator needs to limit fragmentation of memory.
  2. The complexity of allocation should be minimal.
  3. The memory is aligned.
  4. Time complexity:
    - Allocating O(n) 
    - Freeing O(n)

### Invariant:
The free memory blocks are always structured as a cyclic sorted linked list in ascending order by the free blocks memory address.
The last variable always holds a pointer to the last OS allocated free memory block.

### Data Structure:
The allocator is built around the concept of free memory blocks. 
Internally it is a full cycle singly linked list.
The nodes of the list are OS allocated free memory blocks. At the start of each memory block a header struct is inserted, that is the metadata for the memory block.
The header is a struct that contains:
1. the size of the memory block in bytes including the header size, the minimum size is size of header.
2. The pointer to the next free block.
The cycle is created by pointing the tail node to the head.

### Allocation:
Interface:
void *ring_alloc(size_t n)

The allocate function returns a pointer to the free memory of the requested size.

The passed size is converted to header sized units that are ceiled to the nearest integer.
Allocation of memory of size n is done by linear searching for a free block that has the size >= n.
If the size == n the block is detached from the ring and given to the requester else the block is divided into a block of size n and remainder. 
The remainder is reattached to the list and the block of size n is returned to the requester. 
If no suitable size is found a request for a memory block is made to the OS and is attached to the ring and the search is continued.

### Freeing:
Interface:
void ring_free(void *ptr)

A memory block is freed by giving it back to the allocator. The memory block is reattached to the list so that it does not breaks the sorted order.
If the address of the memory block is adjacent to the previous or next block in the list it is merged with them to reduce fragmentation.


