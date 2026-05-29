#include "allocator.h"
#include "fmalloc.h"
#include "block.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>


void *fmalloc(size_t length){
	struct block_meta *newblock = NULL;
	if(head == NULL) {
		head = allocator(((length + 7) & ~7));
      		newblock = split_block(((length + 7) & ~7));
		return newblock->data;
	}

	else if(head != NULL) {
	//	printf("SPLIT BLOCK: REMAINING SIZE OF PAGE %ld\n", head->size);
		newblock = split_block(((length + 7) & ~7)); 
		printf("BLOCK ALLOCATED MEMORY ADDRESS: %p\n", newblock);
		return newblock->data;
	}
}


void *frealloc(void *ptr, int length){
	block_meta *tmp = (block_meta *)ptr - 1;
	printf("REALLOCATED DATA PREVIOUS ADDRESS: %p\n", tmp);
	tmp = reallocate(tmp, ((length + 7) & ~7));
	return tmp->data;
}

void ffree(void *ptr){
	block_meta *tmp = (block_meta*)ptr - 1;
	tmp->free = 0;	
	block_merge(tmp);
	
}

void fstats(void *ptr){
	block_meta *tmp = (block_meta *)ptr-1;
	printf("-------------------------------------------------------------------\n");
	printf("TOTAL POOL SIZE REMAINING: %ld\n CURRENT ADDRESS: %p\n SIZE: %ld\n FREE: %d\n PREV BLOCK: %p\n NEXT BLOCK: %p\n", head->size, tmp,tmp->size, tmp->free, tmp->prev, tmp->next);
	printf("-------------------------------------------------------------------\n");

}
