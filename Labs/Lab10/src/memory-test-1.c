//memory-test-1.c
//Alexander Meijer
//mar 26, 2013
//CSCI 315

#include "allocator.h"
#include <stdio.h>

//tests the allocator functionality
int main(int argc, char *argv[]){
  allocator_init(6);
  void *num1 = allocate(4);
  *(int*)num1 = 12345678;
  printf("Number in allocated memory: %d @ %ld. Expected 12345678.\n", *(int*)num1, (long int) num1);

  void *char1 = allocate(1);
  *(char*)char1 = 'c';
  printf("Number in allocated memory: %c @ %ld. Expected c.\n", *(char*)char1, (long int) char1);

  void *char2 = allocate(1);
  *(char*)char2 = 'H';
  printf("Number in allocated memory: %c @ %ld. Expected H.\n", *(char*)char2, (long int) num1);

  //this should fail
  void *num2 = allocate(4);
  
  if(!num2){
    printf("no more room for num2. test passed.\n");fflush(stdout);
  }else{
    *(int*)num2 = 167;
    printf("Number in allocated memory: %d. Expected 167.\n", *(int*)num2); fflush(stdout);
  }

  void *char1old = (void*)char1;
  void *num1old = (void*)num1;

  deallocate((void*)char1);

  printf("1 byte deallocated @ %ld\n", (void*)char1old);

  deallocate((void*)num1);

  printf("4 bytes deallocated @ %ld\n", (void*)num1old);


  //for best/worst fit, this should be allocated differently
  void *char3 = allocate(1);
  printf("allocated new memory for 1 byte @ %ld\n", (long int) char3);

  *(char*)char3 = 'I';
  printf("Number in allocated memory: %c. Expected I.\n", *(char*)char3);

  void *char4 = allocate(1);

  if(!char4){
    printf("no more room.\n");
  }else{
    *(int*)char4 = 'Q';
    printf("Number in allocated memory: %c. Expected Q.\n", *(int*)char4);
  }


}

