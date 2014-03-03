//frag-eval.c
//Alexander Meijer
//apr 10, 2013
//CSCI 315

#include "allocator.h"
#include <stdio.h>

//tests the allocator functionality
int main(int argc, char *argv[]){
  if(argc != 4){
    printf("error: wrong number of args!");
    exit(-1);
  }

  int requests = atoi(argv[3]);
  int alg = atoi(argv[1]);
  int seed = atoi(argv[2]);

  srand(seed);

  int r = 0;
  int s = 0;
  void *p = NULL;
allocator_init(10240);
  while (r < requests){
    s = (rand() % 900) + 100;

    p = allocate(alg, s);

    deallocate(p);
  }

  double frag = average_frag();
  
  printf("The average fragmentation in free_list: %f\n", frag);
 

}

