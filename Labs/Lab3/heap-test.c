//heap-test.c
//Alexander Meijer
//January 31, 2013
//CSCI 315

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_NUM 9999

int main (int argc, char *argv[]){
  pid_t pid;

  int *data = (int *) malloc(sizeof(int));
  
  *data = TEST_NUM;

  //at this point, parent has an integer in the heap

  //fork the process
  if ((pid = fork()) < 0){
    printf("%s","FORKING ERROR");
  } else if (pid == 0){ /* child */
		
    if ( *data == TEST_NUM ){
      printf("The heap memory is inherited by the child process. Contents of heap (child): %d. Expected: %d\n", *data, TEST_NUM);  
    } else {
      printf("The heap memory is NOT inherited by the child process. Contents of heap (child): %d. Expected: TEST_NUM\n", *data);  
    }

		
  } else { /* parent */
    
  }


}
