# Ring Allocator

## About
This project is a reimplementation of the allocator from Chapter 8 of K&R 1978 edition.
The goal of the project is to explore the concept of memory allocation and project development.
The ring allocator is not meant to be efficient or used for any real memory allocation needs.

## Design

### Constrains
The allocator is just a toy project, it handles no multithreading or security:
  1. The allocator needs to limit fragmentation of memory.
  2. The complexity of allocation should be minimal.
  3. The memory is aligned.
  4. Time complexity:
    - Allocating O(n) 
    - Freeing O(n)

### Overview

#### Invariant:
The free memory blocks in the linked list are always in sorted order based on the memory address.

#### Data Structure:
The allocator is built around the concept of free memory blocks. 
Internally it is a ring data structure built from a linked list.
The nodes of the list are free memory blocks that are built from header data and free memory. The header is a union placed at the start of a 
free memory block and is just metadata that contains:
  1. The metadata struct, that has a pointer to the next free memory block and the size of the memory block.
  2. A member variable x of type ALIGN that represents the most restrictive type of the machine. 
The tail node points to the head and creates a cyclic linked list.

#### Allocation:
Allocation of memory of size n is done by linear searching for a free block that has the size >= n.
If the size == n the block is detached from the ring and given to the requester else the block is divided into a block of size n and remainder. 
The remainder is reattached to the list and the block of size n is returned to the requester. 
If no suitable size is found a request for a memory block is made to the OS and is attached to the ring and the search is continued.

#### Freeing:
A memory block is freed by giving it back to the allocator. The memory block is reattached to the list so that it does not breaks the sorted order.
If the address of the memory block is adjacent to the previous or next block in the list it is merged with them to reduce fragmentation.


#### File organization:
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
