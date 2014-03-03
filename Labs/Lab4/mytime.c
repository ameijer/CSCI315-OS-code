//mytime.c
//Alexander Meijer
//Febuary 3, 2013
//CSCI 315


#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
  struct timeval *tval = malloc(sizeof(struct timeval));

 struct timezone *tzone = malloc(sizeof(struct timezone));


  if (-1 == gettimeofday(tval, tzone)){
     perror("gettimeofday:");
    }

  time_t time;

  time = tval->tv_sec;
 
  //print results
  printf("%s", ctime(&time));


 
}
