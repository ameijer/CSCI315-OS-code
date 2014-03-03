//char-threads.c
//Alexander Meijer
//Febuary 3, 2013
//CSCI 315

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define NUM_THREADS	2

//adapted from https://computing.llnl.gov/tutorials/pthreads/#PthreadsAPI

void *numberator(void *threadid)
{

  //this function cycles through all the letters 
  int number = 0;
  for(;;){
    printf("%d", number); fflush(stdout);
   
    if (9 == number){
      number = 0;
    }else {
      number++;
    }
    sleep(1);
  }
}

//this function cycles through all the letters
void *letterizer(void *threadid)
{
  char letter = 'a';
  for(;;){
    printf("%c",letter); fflush(stdout);
   
    if (letter == 'z'){
      letter = 'a';
    }else {
      letter++;
    }
    sleep(1);
  }
}

int main(int argc, char *argv[]){
  

  pthread_t threads[NUM_THREADS];
  int rc;
  long t;
   

  //create the two threads
  rc = pthread_create(&threads[t], NULL, letterizer, (void *)t);
  if (rc){
    printf("ERROR; return code from pthread_create() is %d\n", rc);
    exit(-1);
  }
  rc = pthread_create(&threads[t], NULL, numberator, (void *)t);
  if (rc){
    printf("ERROR; return code from pthread_create() is %d\n", rc);
    exit(-1);
  }
   
   
  //exit threading
  pthread_exit(NULL);

}
