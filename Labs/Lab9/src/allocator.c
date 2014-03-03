//allocator.c
//Alexander Meijer
//MAR 26, 2013
//CSCI 315

#include "allocator.h"
#include "dlist.h"
#include "dnode.h"
#include <stdio.h>

//allocation policies
//#define FIRST_FIT
//#define BEST_FIT
#define WORST_FIT
//#define DEBUG

typedef struct dlist dlist;

dlist *free_list = NULL;
dlist *allocated_list = NULL;
size_t avail = 0;

int allocator_init(size_t size) {

  //create full nd empty lists
  free_list = dlist_create();
  allocated_list = dlist_create();

  //get the memory space
  void* freespace = malloc(size);
  avail = size;
  if(freespace == NULL)
    return -1;

  //all of the space in the memory is free, so it can be added to the free space list
  dlist_add_front(free_list, freespace, size);
  printf("Newly allocated address: %ld @ %ld\n", free_list->front->data, freespace);
  //return 0 on success
  return 0;
}




void *allocateFF(size_t size, void *targetspace){
  //we know that there is an eligible space somewhere in free_list
  size_t thisspace = dlist_get_datsize(free_list);
  targetspace = free_list->iter->data;
  
  
  //if the size of the node is exactly the size needed, the pointer can be eliminated.
  if (thisspace == size){
#ifdef DEBUG
    printf("exact match found. wiping node completely\n");
    printf("Target addy passed to find_remove: %ld\n", (long int) targetspace); fflush(stdout);
    printf("data addy of first element: %ld\n", (long int) free_list->front->data);
#endif
   
    void* tobeallcd =  dlist_find_remove(free_list, targetspace); //FIX THIS
    //printf("dlist-find-remove deleted node with data: %ld", (long int) tobeallcd);
    dlist_add_front(allocated_list, tobeallcd ,thisspace);
    return tobeallcd;

  } else { //available space is bigger
   

    //now change the pointer of availspace to point to the new fragment
#ifdef DEBUG
    printf("reducing availspace to fit the new data. Previous size of availspace: %d, at %ld\n", dlist_get_datsize(free_list), (long int) free_list->iter->data);
#endif
    //some pointer arithmetic
    //printf("adding %ld to targetspace: %ld\n", (long int) size, (long int) targetspace);
    targetspace = targetspace + (long int) size;
    //print statement here
    //printf(" new target space calculated to be: %ld\n", (long int) targetspace);
    //reflect the new size
    dlist_set_datsize(free_list, dlist_get_datsize(free_list) - size);
    free_list->iter->data =  targetspace;
    // printf("alloc, larger than: target data set to %ld\n", free_list->iter->data);
  
#ifdef DEBUG
    printf("subtracting %d from the availspace.\n", (int)size);
    printf("New size of availspace: %d\n", (int)dlist_get_datsize(free_list));
    printf("data addy of first element when node not deleted: %ld\n", (long int) free_list->iter->data);
#endif

    //now add the old avaliable space to the allocated list    
    dlist_add_front(allocated_list, targetspace, size);
    // printf("data ptr: %d, size: %d\n", allocated_list->front->data, allocated_list->front->size);
    return targetspace;
  }

}

void *allocateBF(size_t size) {
  //loop through to find the best fit
  void* bestspace = dlist_iter_begin(free_list);
  int bestfit = dlist_get_datsize(free_list) - size;
  int thisnode = 0;
  void* thisfree = NULL;
 
#ifdef DEBUG
  printf("initial size to beat: %d @ %ld\n", bestfit, (long int) bestspace);
#endif

  while(dlist_iter_has_next(free_list)) {
    thisfree = dlist_iter_next(free_list);
    thisnode = dlist_get_datsize(free_list);
    
    if(((thisnode - size) < bestfit) && ((thisnode - size) >= 0)) {
      //current node is best fit
      bestfit = thisnode - size;
      bestspace = thisfree;
#ifdef DEBUG
      printf("new size to beat: node size of %d yields frag %d @ %ld\n", thisnode, bestfit, (long int) bestspace);
#endif
	
    }
      
  }
      
  
  //remember to check if positive. if not, something has gone wrong
  if(bestfit < 0){
    printf("bestfit: invalid size\n");
    exit(-1);
  }
  
  //now perform some variable aliasing so that the old code from FF can be recycled
  void* curspace =  dlist_iter_begin(free_list);

  while(dlist_iter_has_next(free_list)) {
    if (bestspace == (curspace = dlist_iter_next(free_list))){
      break;
    }
  }

#ifdef DEBUG
  printf("data passed: current iter node has size %d @ %ld\n", dlist_get_datsize(free_list), (long int) curspace);
#endif

  size_t thisspace = dlist_get_datsize(free_list);
  void* targetspace = curspace;
    
 //if the size of the node is exactly the size needed, the pointer can be eliminated.
  if (thisspace == size){
#ifdef DEBUG
    printf("exact match found. wiping node completely\n");
    printf("Target addy passed to find_remove: %ld\n", (long int) targetspace); fflush(stdout);
    printf("data addy of first element: %ld\n", (long int) free_list->front->data);
#endif
   
    void* tobeallcd =  dlist_find_remove(free_list, targetspace); //FIX THIS
    //printf("dlist-find-remove deleted node with data: %ld", (long int) tobeallcd);
    dlist_add_front(allocated_list, tobeallcd ,thisspace);
    return tobeallcd;

  } else { //available space is bigger
   

    //now change the pointer of availspace to point to the new fragment
#ifdef DEBUG
    printf("reducing availspace to fit the new data. Previous size of availspace: %d, at %ld\n", dlist_get_datsize(free_list), (long int) free_list->iter->data);
#endif
    //some pointer arithmetic
    //printf("adding %ld to targetspace: %ld\n", (long int) size, (long int) targetspace);
    targetspace = targetspace + (long int) size;
    //print statement here
    //printf(" new target space calculated to be: %ld\n", (long int) targetspace);
    //reflect the new size
    dlist_set_datsize(free_list, dlist_get_datsize(free_list) - size);
    free_list->iter->data =  targetspace;
    // printf("alloc, larger than: target data set to %ld\n", free_list->iter->data);
  
#ifdef DEBUG
    printf("subtracting %d from the availspace.\n", (int)size);
    printf("New size of availspace: %d\n", (int)dlist_get_datsize(free_list));
    printf("data addy of first element when node not deleted: %ld\n", (long int) free_list->iter->data);
#endif

    //now add the old avaliable space to the allocated list    
    dlist_add_front(allocated_list, targetspace, size);
    // printf("data ptr: %d, size: %d\n", allocated_list->front->data, allocated_list->front->size);
    return targetspace;
  }

}




void *allocateWF(size_t size) {
  //loop through to find the worst fit
  void* bestspace = dlist_iter_begin(free_list);
  int bestfit = dlist_get_datsize(free_list) - size;
  int thisnode = 0;
  void* thisfree = NULL;
 
#ifdef DEBUG
  printf("initial size to beat: %d @ %ld\n", bestfit, (long int) bestspace);
#endif

  while(dlist_iter_has_next(free_list)) {
    thisfree = dlist_iter_next(free_list);
    thisnode = dlist_get_datsize(free_list);
    
    if(((thisnode - size) >  bestfit) && ((thisnode - size) >= 0)) {
      //current node is best fit
      bestfit = thisnode - size;
      bestspace = thisfree;
#ifdef DEBUG
      printf("new size to beat: node size of %d yields frag %d @ %ld\n", thisnode, bestfit, (long int) bestspace);
#endif
	
    }
      
  }
      
  
  //remember to check if positive. if not, something has gone wrong
  if(bestfit < 0){
    printf("worstfit: invalid size\n");
    exit(-1);
  }
  
  //now perform some variable aliasing so that the old code from FF can be recycled
  void* curspace =  dlist_iter_begin(free_list);

  while(dlist_iter_has_next(free_list)) {
    if (bestspace == (curspace = dlist_iter_next(free_list))){
      break;
    }
  }

#ifdef DEBUG
  printf("data passed: current iter node has size %d @ %ld\n", dlist_get_datsize(free_list), (long int) curspace);
#endif

  size_t thisspace = dlist_get_datsize(free_list);
  void* targetspace = curspace;
    
 //if the size of the node is exactly the size needed, the pointer can be eliminated.
  if (thisspace == size){
#ifdef DEBUG
    printf("exact match found. wiping node completely\n");
    printf("Target addy passed to find_remove: %ld\n", (long int) targetspace); fflush(stdout);
    printf("data addy of first element: %ld\n", (long int) free_list->front->data);
#endif
   
    void* tobeallcd =  dlist_find_remove(free_list, targetspace); //FIX THIS
    //printf("dlist-find-remove deleted node with data: %ld", (long int) tobeallcd);
    dlist_add_front(allocated_list, tobeallcd ,thisspace);
    return tobeallcd;

  } else { //available space is bigger
   

    //now change the pointer of availspace to point to the new fragment
#ifdef DEBUG
    printf("reducing availspace to fit the new data. Previous size of availspace: %d, at %ld\n", dlist_get_datsize(free_list), (long int) free_list->iter->data);
#endif
    //some pointer arithmetic
    //printf("adding %ld to targetspace: %ld\n", (long int) size, (long int) targetspace);
    targetspace = targetspace + (long int) size;
    //print statement here
    //printf(" new target space calculated to be: %ld\n", (long int) targetspace);
    //reflect the new size
    dlist_set_datsize(free_list, dlist_get_datsize(free_list) - size);
    free_list->iter->data =  targetspace;
    // printf("alloc, larger than: target data set to %ld\n", free_list->iter->data);
  
#ifdef DEBUG
    printf("subtracting %d from the availspace.\n", (int)size);
    printf("New size of availspace: %d\n", (int)dlist_get_datsize(free_list));
    printf("data addy of first element when node not deleted: %ld\n", (long int) free_list->iter->data);
#endif

    //now add the old avaliable space to the allocated list    
    dlist_add_front(allocated_list, targetspace, size);
    // printf("data ptr: %d, size: %d\n", allocated_list->front->data, allocated_list->front->size);
    return targetspace;
  }

  
}




void *allocate(size_t size){
  //check for enough empty space
  size_t temp;
  void *ptr;

  if(NULL == (ptr = dlist_iter_begin(free_list))){
#ifdef DEBUG
    printf("Error, no data found in free_list\n");
#endif
    return NULL;
  }

  temp = dlist_get_datsize(free_list);
#ifdef DEBUG
  printf("First node data size: %d\n", (int) temp);
#endif

  int fits = 0;

#ifdef DEBUG
  int iter = 1;
#endif
void *targetspace;
  if(temp >= size){
    fits = 1;
#ifdef DEBUG
    printf("Fit found in first node\n");
#endif
  }else{
    
    while(dlist_iter_has_next(free_list)){
      targetspace = dlist_iter_next(free_list);
      temp = dlist_get_datsize(free_list);
#ifdef DEBUG   
      printf("node num: %d. Size of node data: %d\n", (int) iter, (int)temp);
      iter++;
#endif
      if(temp >= size){
	fits = 1;
#ifdef DEBUG
	printf("Fit found in node %d\n", iter);
#endif
	break;
      }
    
    }
  }
  if(!fits){
#ifdef DEBUG
    printf("Not enough memory\n");
#endif
    return NULL;
  }
  //at this point, it is known that sufficient space can be allocated. call the appropriate allocator 

#ifdef FIRST_FIT
  return allocateFF(size,targetspace);
#elif defined BEST_FIT
  return allocateBF(size);
#elif defined WORST_FIT
  return allocateWF(size);
#else
#error "No allocation policy selected"
#endif

}



int deallocate(void *ptr){
  //printf("ptr before passed: %d\n", ptr);
#ifdef DEBUG
  printf("In dealloc\n");
#endif
  /*   void *ptr2; */
  /*   if(NULL == (ptr2 = dlist_iter_begin(allocated_list))){ */

  /* #ifdef DEBUG */
  /*     printf("Error, no data found in allocatedlist\n"); */
  /* #endif */

  /*     return -1; */
  /*   } */
  void *dealloced = NULL;
  int size = 0;
  //printf("ptr before rem: %d\n", ptr);
  if(NULL == (dealloced = dlist_find_remove2(allocated_list, ptr, &size))){
    printf("Error: data not found for deallocation\n");
    return -1;
  } else {
    printf("deallocating size: %d @ %ld\n", size, dealloced);
    dlist_add_front(free_list, dealloced, size);
    return 0;
  }
} 

