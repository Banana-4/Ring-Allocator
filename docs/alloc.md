# Allocation:
## Interface:
void *ring_allocator(size_t n)
## About:
Returns a pointer to a free memory block of size n.

## Keywords:
1. Free memory block
2. ring list
3. header
4. reattach
5. p and q pointers
6. ascending ordering
## Relationship:
ascending order -> ring list -> free memory block -> header

## Design:
This function searches for a memory block that is at least of n bytes size.
It does so by iterating over the free memory block ring until it finds a adequate block, when a block can not be found a new block of n * BLOCKSIZE is requested from the OS and attached to the ring and than the search is resumed. If the OS can not return a large enough block the function return NULL.
The found block can be in two states:
1. It is the same size as n. 
2. It is larger than n.
In the first case it is detached from the ring, the next block is attached to the previous one, 1 is added to the founds block pointer and it is returned to the calling function.
In the second case the block is divided. The previous blocks next pointer is set to the address of the found block + n as the new next. The new next block is cast to Header, size is set to the founds block size - n and the next pointer is set to the founds next. The founds block size is set to n and the same thing is done as for the first case block.


## Pipeline
0. Prepare the data, 
1. Search for a block
2. If not found request a new one and attached to the ring. If the request fails return NULL.
3. When found prepare it for returning.
4. return block.

## Header:
1. size - size of the block in bytes.
2. next - pointer to the next memory block.

## Edge cases
1. No free blocks.
2. OS can't allocate a large enough block.
