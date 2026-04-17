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
 The linked list is always in ascending sorted order based on the memory address it holds.

The allocator is built around the concept of free memory blocks. 
Internally it is a ring data structure built from linked lists.
The nodes of the list represent free memory blocks and have the following members:
  1. The pointer to the next node.
  2. The size of the memory block.
  3. The pointer to the free memory.
The tail node points to the head and creates a full cycle linked list.

#### Allocation:
Allocation of memory of size n is done by linear searching for a free block that has the size >= n.
If the size == n the block is detached from the ring and given to the requester else the block is divided into a block of size n and remainder. 
The remainder is reattached to the list and the block of size n is returned to the requester. 
If no suitable size is found a request for a memory block is made to the OS and is attached to the ring and the search is continued.

#### Freeing:
A memory block is freed by giving it back to the allocator. The memory block is reattached to the list so that it does not breaks the sorted order.
If the address of the memory block is adjacent to the previous or next block in the list it is merged with them to reduce fragmentation.

