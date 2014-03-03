//problem3-1.c
//Alexander Meijer
//3/19/2013
//CSCI 315

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define NUM_PHILO 5
#define MAX_NAP_T1 2 //define t
#define MAX_NAP_T2 1 //define t

int seed;
pthread_mutex_t chopsticks[NUM_PHILO];

void napping(int t){
  seed++;
  int thisseed  = seed;
  
  //threadsafe rand_r call
  int sleeptime = rand_r(&thisseed);
  int temp = (int) t * 1000000 *  ((double) sleeptime) / (double)RAND_MAX;
  
  //sleep for the required amt of time
  usleep(temp);
  
}

//philosopher
void *philosopher(void *id) {
  
  for(;;){
  //extract the id passed to the philosopher function
  int idt = (long long int)id;

  //print out required text
  printf("Philosopher %d is is thinking\n", idt);fflush(stdout);
  napping(MAX_NAP_T1);
  printf("Philosopher %d is is hungry\n", idt);fflush(stdout);

  if(!(idt%2)){

   //acquire chopstick to left
  pthread_mutex_lock(&chopsticks[idt]);
  printf("Philosopher %d picking up chopstick %d\n", idt, idt);fflush(stdout);
  napping(MAX_NAP_T1);

  //acquire chopstick to right
  pthread_mutex_lock(&chopsticks[(idt+1) % 5]);
  printf("Philosopher %d picking up chopstick %d\n", idt, (idt+1) % 5); fflush(stdout);

  } else {

//acquire chopstick to right
  pthread_mutex_lock(&chopsticks[(idt+1) % 5]);
  printf("Philosopher %d picking up chopstick %d\n", idt, (idt+1) % 5); fflush(stdout);
  napping(MAX_NAP_T1);
//acquire chopstick to left
  pthread_mutex_lock(&chopsticks[idt]);
  printf("Philosopher %d picking up chopstick %d\n", idt, idt);fflush(stdout);


  }
  
  printf("Philosopher %d is is starting to eat\n", idt);fflush(stdout);
  napping(MAX_NAP_T2);
 
  printf("Philosopher %d is is done eating\n", idt); fflush(stdout);
  //release chopstick to left
  pthread_mutex_unlock(&chopsticks[idt]);
  printf("Philosopher %d putting down chopstick %d\n", idt, idt); fflush(stdout);
 napping(MAX_NAP_T1);
  //release chopstick to right
  pthread_mutex_unlock(&chopsticks[(idt+1) % 5]);
  printf("Philosopher %d putting down chopstick %d\n", idt, (idt+1) % 5); fflush(stdout);
  }
}

int shared;

int main (int argc, char *argv[]) {
  
  seed = 1;
  pthread_attr_t attr;
  pthread_t *tidp; // array for thread ids
  int ret_val;

  int num_threads = NUM_PHILO;

	
  tidp = (pthread_t *) calloc(num_threads, sizeof(pthread_t));
  long long int *param = calloc(num_threads, sizeof(long long int)); 
  // initialize attr variable
  pthread_attr_init(&attr);

  // initialize thread detached state to joinable
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
  //init chopstick
  int stats = 0;
  for (int i = 0; i<num_threads; i++) {
    stats = pthread_mutex_init(&chopsticks[i],NULL);
    if (stats){
      perror("pthread_mutex_init");
      exit(-1);
    }
  }

  //create 5 philosopher threads
  for (int i = 0; i<num_threads; i++) {

    param[i] = i;
    //pass addr of ID
    ret_val = pthread_create(&tidp[i], &attr, philosopher, (void*) param[i]);
    if (ret_val) {
      printf("ERROR in pthread_create for thread %d: return value= %d\n", 
	     i, ret_val);
      exit(-1);
    }
  }

  // free attribute variable
  pthread_attr_destroy(&attr);

  // main thread waits for spawned threads to terminate
  for(long long int i=0; i<num_threads; i++) {
    
    ret_val = pthread_join(tidp[i], NULL);
    if (ret_val) {
      printf("ERROR in pthread_join() for thread %d = return value= %d\n",
	     i, ret_val);
      exit(-1);
    }
  }

  pthread_exit(NULL);
  printf("still here\n");
}
