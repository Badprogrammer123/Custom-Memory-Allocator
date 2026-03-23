# Custom-Memory-Allocator
A custom memory allocator in C with mmap, inspired by the behavior of malloc. Designed and implemented block metadata structures, allocation, deallocation, block splitting, coalescing, and reallocation without relying on external libraries.

### In Progress
- Robust coalescing (merge logic)
- Edge case handling
- Stability improvements

# What I learned

I developed a better understading of low level memory management in C, including how dynamic allocation works under the hood. I learned how to design and mantain a memory allocator using mmap, as well as how the logic for allocation, reallocation, and dealloation are constructed. A major part of this learning process was debugging memory corruption issues such as, incorrect pointer arithmetic, overwriting data, incorrect interpretation of a freed block, improper linked list updates, engaging with bitwise operations for the 8 byte alignemnt.
