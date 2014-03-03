//user.c
//Alexander Meijer
//February 14, 2013
//CSCI 315

/*
 * Copyright (c) 2012 Bucknell University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: L. Felipe Perrone (perrone@bucknell.edu), modified by Alexander Meijer
 */

#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

extern char *strsignal(int sig);
struct timeval *tval;

struct timezone *tzone;
time_t curtime;

//new handler
void user_handler(int a)
{
  // handle signal, update clock
  if (-1 == gettimeofday(tval, tzone)){
    perror("gettimeofday:");
  }
  curtime = tval->tv_sec;
  printf("recieved signal 10\n");
}

void timer_handler(int a)
{
  // handle signal, update clock
  if (-1 == gettimeofday(tval, tzone)){
    perror("gettimeofday:");
  }
  curtime = tval->tv_sec;
}

int main(int argc, char* argv[])
{
  int retval;
  int stat;
  struct itimerval timerValue;
  struct itimerval oldTimerValue;
  struct sigaction sigact;
  struct sigaction sigact2;

  tzone = malloc(sizeof(struct timezone));
  tval = malloc(sizeof(struct timeval));

 memset(&sigact2, 0, sizeof(struct sigaction));
  memset(&sigact, 0, sizeof(struct sigaction));

  sigact.sa_handler = timer_handler;
  sigact2.sa_handler = user_handler;

  // initialize timer parameters: expires in 5 seconds
  timerValue.it_interval.tv_sec = 5;
  timerValue.it_interval.tv_usec = 0;

  timerValue.it_value.tv_sec = 5;
  timerValue.it_value.tv_usec = 0;
 
  stat = 0;
  // install signal handler to catch SIGALRM
  stat = sigaction(SIGALRM, &sigact, NULL);
  if (stat != 0){
    perror("sigact1");
  }
  //new signal handler for SIGUSR1
  stat = sigaction(SIGUSR1, &sigact2, NULL);
if (stat != 0){
    perror("sigact1");

 }
  retval = setitimer(ITIMER_REAL, &timerValue, &oldTimerValue);
  


  if (-1 == gettimeofday(tval, tzone)){

    perror("gettimeofday:");
  }

  

  curtime = tval->tv_sec;
 
  

  if (-1 == retval)
    perror("Could not set timer");
 
  while(1){
    //print results
    printf("%s", ctime(&curtime));
    fflush(stdout);
    sleep(1);
  }

  return 0;
}
