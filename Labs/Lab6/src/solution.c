//solution.c
//Alexander Meijer
//Feb 27, 2012
//CSCI 315

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "circular-list.h" 

// global variables -----------------------

struct circular_list mylist;
unsigned int seed = 100;
pthread_mutex_t *prodmutex = NULL;

// end of global variables ----------------

void *producer (void *param) {
  item i;

  //increment seed for a unique random value, after locking accesses to it
  pthread_mutex_lock(prodmutex);
  seed++;
  pthread_mutex_unlock(prodmutex);

  int temp = 0;
  while (true) {
    // sleep for random period of time
     temp = rand_r(&seed);
    printf("Number passed to usleep(producer): %d\n", temp);

    //this also had to be fixed
    usleep((int)( 2000000 * (  ((double) temp) / (double)RAND_MAX)) ); 
    
    // generate a random number
    i = (item) (((double) rand_r(&seed)) / RAND_MAX);

    if (circular_list_insert(&mylist, i) == -1) {
      printf("PRODUCER: error condition\n");
    } else {
      printf("PRODUCER: produced value %lf\n", i);
    }
  }
}

void *consumer (void *param) {
  item i = 0.0;
  unsigned int seed = 999;
  int temp = 0;
  while (true) {
    // sleep for random period of time
    temp = rand_r(&seed);
    printf("Number passed to usleep(consumer): %d\n", temp);
    usleep((int)(200000 * ( ( (double) temp) / (double) RAND_MAX)));

    if (circular_list_remove(&mylist, &i) == -1) {
      printf("CONSUMER: error condition\n");
    } else {
      printf("CONSUMER: consumed value %lf\n", i);
    }
  }
}

int main (int argc, char *argv[]) {

  // if error in command line argument usage, terminate with helpful
  // message
  // get command line arguments
  if(argc != 4){
    printf("Error: incorrect number of arguments\n");
    printf("correct usage: solution [num_prod] [num_cons] [sleep_time]\n");
    exit(-1);
  }
  
  //detect non-integer input before atoi is called
  for(int tok  = 1; tok < argc; tok++){
    for(int chr = 0; chr < strlen(argv[tok]); chr++){
      for(int ascii = 48; ascii < 58; ascii++){
	if(argv[tok][chr] == ascii){
	  break;
	}
	if(ascii == 57){
	  printf("Error: Incompatible characters in input: %d\n", argv[tok][chr]);
	  exit(-1);
	}
      }
    }

  }

  //get parameters
  int num_prod = atoi(argv[1]);
  int num_cons = atoi(argv[2]);
  int sleep_time = atoi(argv[3]);

  // initialize buffer
  circular_list_create(&mylist, 20);

  //init mutex to protect seed incrementing used by producers
  prodmutex = malloc(sizeof(pthread_mutex_t));
  int stats = pthread_mutex_init(prodmutex, NULL);
 if (stats){
    perror("pthread_mutex_init");
    exit(-1);
  }

  // create producer thread(s)
 pthread_t *tidprods = NULL; // array for thread ids
 tidprods = (pthread_t *) calloc(num_prod, sizeof(pthread_t));
  
  for (int i=0; i < num_prod; i++) {
    int ret_val = pthread_create(&tidprods[i], NULL, producer, NULL);
    if (ret_val) {
      printf("ERROR in pthread_create for thread %d: return value= %d\n",
	     i, ret_val);
      exit(-1);
    }
  }

  // create consumer thread(s)
    pthread_t *tidcons; // array for thread ids
  tidcons = (pthread_t *) calloc(num_cons, sizeof(pthread_t));

  for (int i=0; i < num_cons; i++) {
    int ret_val = pthread_create(&tidcons[i], NULL, consumer, NULL);
    if (ret_val) {
      printf("ERROR in pthread_create for thread %d: return value= %d\n",
	     i, ret_val);
      exit(-1);
    }
  }

  // sleep to give time for threads to run
  sleep(sleep_time);

  // exit
  return (0);
}
