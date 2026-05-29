#include "allocator.h"
#include "block.h"
#include "fmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){	
	int *hello = fmalloc(4);
	int *hey = fmalloc(4);
	int *je = fmalloc(4);
	int *a = fmalloc(4);
	int *b = fmalloc(4);
	int *omg = fmalloc(4);
	fstats(a);	
	fstats(b);
	ffree(a);
	ffree(b);

	int *o = fmalloc(4);
	int *p = fmalloc(4);
	//ffree(p);
	fstats(o);
	fstats(p);
	p[0] = 1;
	printf("VISH %p %d\n", p, p[0]);
	int *yo = o;
	p = frealloc(p, 25);	
	printf("VISH %p %d\n", p, p[0]);
	fstats(p);
}
