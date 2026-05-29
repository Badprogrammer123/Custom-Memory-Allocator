#include <stdlib.h>
#include "block.h"
struct block_meta *allocator(int length);
struct block_meta *split_block(size_t length); 
struct block_meta *createNewBlock(struct block_meta *new_block, struct block_meta *curr, int length);
struct block_meta *reallocate(struct block_meta *ptr, int length);
void allocateMoreMemory(int length);
void block_merge(struct block_meta *freedblock);

extern block_meta *head;

