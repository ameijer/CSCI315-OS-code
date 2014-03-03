//allocator.h
//Alexander Meijer
//March 26, 2012
//CSCI 315


#ifndef ALLOC_H
#define ALLOC_H

#include <stdlib.h>
#include <stddef.h>

/*
This function will create and initialize two memory management lists: one which keeps track of the memory that is available (call it free_list) and another which keeps track of memory which is already in use (call it allocated_list). All the memory manipulated by your allocator will reside in the heap of the process that uses it. When allocator_init starts, it will call the standard malloc to request a contiguous space of size bytes from the underlying operating system. The pointer received from malloc will be used to initialize a single node in your free_list; your allocated_list must start out empty. If the malloc call does not succeed, this function must return -1, otherwise, it must return 0.
*/

int allocator_init( size_t size); 

/*
Equivalent to malloc. This function will traverse the free_list and find a contiguous chunk of memory that can be used to satisfy the request of an area of size bytes. If the caller makes a request for memory that is larger than what your allocator can honor, this function must return NULL. Your allocate function must be flexible enough to allow for different allocation policies, namely first-fit, best-fit, and worst-fit. You should probably create three functions, one for each of these policies, which are used internally by allocate and not visible to the programmer using your custom memory allocator. Having those functions allows you to make a trivial modification to allocate any time you want to experiment with a different policy. The design of these functions’ API and their implementation (obviously) is up to you. You should describe all additional functions uses in your code in a file called designAPI.txt to turn in for this lab.
*/
void *allocate (int method, size_t size); 

/*
Equivalent to free. This function will use ptr to find the corresponding node in the allocated_list and free up that chunk of memory. If the caller attempts to deallocate a pointer that cannot be found in the allocated_list, this function must return -1, otherwise it must return 0. To make your development process easier, at first, you can simply move the deallocated memory from the allocated_list to the free_list. Once that functionality is working, you may want to tackle an extra credit problem: you can check if the deallocated memory is adjacent to another chunk of free memory and consolidate the two corresponding free_list nodes into a single node representing the sum of the sizes of the two original nodes.
*/
int deallocate(void *ptr); 

/**
 * This function computes the average fragmentation in the two lists
 * 
 * @return average fragementation of the memory
 */
double average_frag();

#endif


