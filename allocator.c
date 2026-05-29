#include <sys/mman.h> //mmap and the PROT_READ MAP_PRIVATE etc
#include <errno.h> //Error codes with mmap failure
#include "allocator.h"
#include <stdio.h>
#include <string.h> //memcpy

#define NON_USABLE_BLOCK -1


#define BLOCK_META_SIZE sizeof(block_meta)

struct block_meta *head = NULL; //Checking if there is allocated space already



// Alllcates the head of the memory block first allocation
struct block_meta *allocator(int length){	
	//creating memory pool
	head = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1 ,0);
	if(head == MAP_FAILED) return NULL;

	head = (block_meta *) (((unsigned long)head + 7) & ~ 7); 
	head->free = NON_USABLE_BLOCK;
	head->next = NULL;
	head->prev = NULL; 
	head->size = BLOCK_SIZE;
	return head; 
}	



//function to create a new block if there no special case
struct block_meta *createNewBlock(struct block_meta *new_block, struct block_meta *curr, int length){
	new_block = (block_meta*)( ( (size_t *) (curr + 1) ) + curr->size);
	new_block->prev = curr;
        new_block->next = NULL;
     	new_block->size = length;
        new_block->free = 1;
       	new_block->data = new_block + 1;
	curr->next = new_block;
	return new_block;

		
}

//merges adjacent blocks together
void block_merge(struct block_meta *freedblock){

	block_meta *neighborBlockPrev = freedblock->prev; 
	block_meta *neighborBlockNext = freedblock->next;  
	if(neighborBlockPrev != NULL && neighborBlockPrev->free == 0 && neighborBlockPrev != head){
		neighborBlockPrev->next = freedblock->next;
		neighborBlockNext->prev = neighborBlockPrev; 
		neighborBlockPrev->size+= freedblock->size;
		freedblock = neighborBlockPrev; 
	
	}
	if(neighborBlockPrev != NULL && neighborBlockNext->free == 0 && neighborBlockNext!= NULL){
		freedblock->size += neighborBlockNext->size; 
		freedblock->next = neighborBlockNext->next;
       		neighborBlockNext->free = 0;		
	}
    	head->size += BLOCK_META_SIZE + freedblock->size;
	

}

void allocateMoreMemory(int length){
	   if(head->size <= BLOCK_META_SIZE + length ){
                struct block_meta *curr = head;
                while(curr->next != NULL){
                        curr = curr->next;
		}
                curr->next = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1 ,0);
                curr->next = (block_meta *) (((unsigned long)curr->next + 7) & ~ 7);
                curr->next->prev = curr;
                curr = curr->next;
                curr->next = NULL;
                curr->data = curr + 1;
                curr->size = length;
                curr->size += BLOCK_SIZE;
                head->size += curr->size;
                printf("TOTAL SPACE NOW: %ld\n", curr->size);
        }
}

// Causing segmenttion fault issue and also you need to check for realloc and thers not enough memory pool space then we need to mmap more
struct block_meta *reallocate(struct block_meta *ptr, int length){
	allocateMoreMemory(length);		
	struct block_meta *curr = head;
       	while(curr->next != NULL){
		if(curr->free == 0 && curr->size  > length){
			memcpy(curr->data, ptr->data, ptr->size);	
			ptr->free = 0;
			curr->size = length;
			printf("NEW ADDRESS: %p\n", curr->data);
			return curr; 
		}
		curr = curr->next;
	}	
	
	ptr->free = 0;
	struct block_meta *new_block = createNewBlock(new_block, curr, length); 
	memcpy(new_block->data, ptr->data, ptr->size);
	printf("NEW ADDRESS: %p\n", new_block);	
	return new_block;
}

struct block_meta *split_block(size_t length){
	//Allocating bigger memory pool if head is full
	allocateMoreMemory(length);	
	//First BLOCK other than head memory allocation 
	if(head->next == NULL){
		struct block_meta *new_block = head + 1;
		new_block->size = length;
		new_block->free = 1;
		new_block->next = NULL;
		new_block->prev = head; 
		new_block->data = new_block + 1;
		head->next = new_block;
		head->size -= BLOCK_META_SIZE + new_block->size;
		return new_block;
		
	}

	//when head has already more than one allocation
	else if(head->next != NULL){
		struct block_meta *curr = head->next;
		struct block_meta *new_block = NULL; 

		
		while(curr->next != NULL){
			if(curr->free == 0 && curr->size >= length && curr->size < length + BLOCK_META_SIZE + 8){
			       	new_block = curr;
				new_block->free = 1;
				return curr;	
			}
			
			//8 represents minimum requiremnt while also keeping bit alignment with other bits that needs to be satisfied in order for it to make sense splitting a block ebcause there is no reason to split a block if that block will only be able to store one byte, that is waste of space
			//this is for splitting a block that could be used for another request so creating a new blcok from a bigger blcok
			else if(curr->free == 0 && curr->size >= BLOCK_META_SIZE + 8){
				new_block = (block_meta *) (( (size_t *) (curr + 1) ) + length) ;
				curr->size = curr->size - length - BLOCK_META_SIZE;
				block_meta *old_next = curr->next;
				new_block->next = old_next;
				new_block->prev = curr;
				if(old_next != NULL){
					old_next->prev = new_block;
				}

				curr->next = new_block; 
				
				new_block->free = 0; 
				new_block->data = new_block + 1; 
				
				curr->free = 1; 
				
				curr->data = curr + 1;	
				return curr;
			}
			curr = curr->next;
		} 
			
			new_block = createNewBlock(new_block, curr, length);
			head->size -= BLOCK_META_SIZE + new_block->size;

			return new_block;
	}
}





