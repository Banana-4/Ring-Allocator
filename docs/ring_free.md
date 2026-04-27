# Ring Free Function:

## About:
Ring free function inserts a free memory block into the carousale while preserving order.
This function takes a free memory block and transformes it into a node of the carousale.

## Pipeline:
1. Searching for the insertion position.
2. Insertion of the block.
The function is a linear scan that scans the ring for a place where to insert the free memory block.
The free memory block ptr positon for insertion is the one that does not break the sorted order of the ring.

### Searching:
The search is done linearly itearating fro p = last.
The states of pointer p that signal a valid insertion points are:
1. p < block and  p->next > block, the block is inserted between p and p->next. 
2. p > p->next and and block > p. End point insertion, at the tail.
3. p > p->next and block < p->next. End point insertion at the head.

### Inserting:
In ordere to minimize fragmentation block can be coalescenced with p or p->next:
1. If p + p->size == block, this condition means that the block was part of p. The joining is done by increasing p size by the blocks size.
2. if block + block->size == p->next. In this case p->next is a part of the block so the block size is increased by p->next size and p is set to point to the block and block is set to point to p->next->next.

If there is no coalescencing than the block is insert by block->next = p-<next and p->next = block.
