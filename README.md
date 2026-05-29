# Custom-Memory-Allocator
A custom memory allocator in C with mmap, inspired by the behavior of malloc. The goal of this project was to better understand how dynamic memory allocation works internally by implementing my own versions of malloc, free, and realloc. 

## Features

- Uses `mmap()` to request memory from the operating system
- Maintains block metadata for each allocation
- Supports custom `fmalloc()`
- Supports custom `ffree()`
- Supports experimental `frealloc()`
- Splits larger free blocks when possible
- Attempts to merge adjacent free blocks
- Aligns allocation sizes to 8 bytes
- Includes `fstats()` for inspecting block metadata

## Known Issues / Future Improvements

- Improve `frealloc()` behavior when the memory pool does not have enough space
- Add stronger pointer validation before freeing or reallocating memory
- Improve boundary checking
- Clean up block free-state logic
- Add better error handling for `mmap()` failures
- Reduce fragmentation (I MIGHT WORK ON THAT)

## Takeaway

Building this project gave me a much better understanding of how memory allocation works under the hood. I got to work directly with memory blocks, metadata, pointer arithmetic, alignment, and raw bytes.

This project also helped me get more comfortable manipulating memory at a lower level. I had to think carefully about how pointers move through memory, how block headers are stored before user data, how sizes are aligned to 8 bytes, and how splitting or merging blocks affects the layout of the allocator. 

Even though this allocator is terrible, it was a valuable systems programming project because it forced me to research, debug, and understand concepts that are usually hidden behind standard library functions.
