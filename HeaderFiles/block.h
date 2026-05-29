#ifndef BLOCK_H
#define BLOCK_H


#define BLOCK_SIZE 4096

typedef struct block_meta {
        size_t size;
        struct block_meta *next;
	struct block_meta *prev;        
	char free;
	void *data; 

}block_meta;

#endif
