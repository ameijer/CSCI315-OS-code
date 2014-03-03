//circular-list.c
//Alexander Meijer
//Feb 27, 2012
//CSCI 315

#include <stdlib.h>
#include "circular-list.h" 
#include <stdio.h>
#include <pthread.h>

int circular_list_create(struct circular_list *l, int size) {
  int stats = 0;

  //initialize the mutex to protect data access
  mutex = malloc(sizeof(pthread_mutex_t ));
  stats = pthread_mutex_init(mutex, NULL);
  if (stats){
    perror("pthread_mutex_init");
    exit(-1);
  }

  //init full mutex to queue producers if list is full
  full = malloc(sizeof(sem_t));
  stats = sem_init(full,0,size);
  if (stats){
    perror("pthread_mutex_init");
    exit(-1);
  }

 //init empty mutex to queue consumers if list is empty
  empty = malloc(sizeof(sem_t));
  stats = sem_init(empty,0, 0);
  if (stats){
    perror("pthread_mutex_init");
    exit(-1);
  }

  l->buffer = calloc(size, sizeof(item));
  l->start = -1;
  l->end = -1;
  l->elems = 0;
  l->size = size;
  return 0;
}

int circular_list_insert(struct circular_list *l, item i) {

#ifdef ENABLE_BLOCKING  // CC wrapper added by grader
  //queue up if full
  sem_wait(full);
#else
  if(-1 == sem_trywait(full)){
      return -1;
  }
#endif

  //lock mutex
  pthread_mutex_lock(mutex);
  //list is full
  if ((l->elems) >= l->size){
    printf("Error: list is full\n");
    return -1;
  }
 
  l->elems++;

  //element is first
  if ((l->elems) == 1){
    l->start = 0;
    l->end = 0;
    l->buffer[l->end] = i;

    //unlock mutex & semaphore
    pthread_mutex_unlock(mutex);
    sem_post(empty);
    return 0;
  } else {
    //element is not first
    l->end++;
    l->end = (l->end) % l->size;
    l->buffer[l->end] = i;
  
  }

  //unlock access, indicate ready for consumer
  pthread_mutex_unlock(mutex);
  sem_post(empty);
  return 0;

}

int circular_list_remove(struct circular_list *l, item *i) {

#ifdef ENABLE_BLOCKING  // CC wrapper added by grader
  //wait in empty queue, lock access to list
  sem_wait(empty);
#else
  if(-1 == sem_trywait(empty)){
    return -1;
  }
#endif

  pthread_mutex_lock(mutex);

  //list is empty
  if ((l->elems) <= 0){
    printf("Error: list is empty\n");
    return -1;
  }

  
  l->elems--;

  //element is last
  if ((l->elems) == 0){
    *i = l->buffer[l->start];
    l->start = -1;
    l->end = -1;

    //unlock mutex & semaphore
    pthread_mutex_unlock(mutex);
    sem_post(full);
    return 0;
  } else {
 
    //element is not last
    *i = l->buffer[l->start];
    l->start++;
    l->start = (l->start) % l->size;
   
  }

  //unclock buffer, signal producer sem
  pthread_mutex_unlock(mutex);
  sem_post(full);
  return 0;
}

