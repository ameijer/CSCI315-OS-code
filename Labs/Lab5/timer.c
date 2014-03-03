//timer.c
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
#include <string.h>

extern char *strsignal(int sig);

void timer_handler(int a)
{
  // handle signal
  printf(">>>> signal caught\n");
  printf(">>>>    int parameter = %s\n", (char*) strsignal(a));
}

int main(int argc, char* argv[])
{
  int retval;
  struct itimerval timerValue;
  struct itimerval oldTimerValue;
  struct sigaction sigact;
  memset(&sigact, 0, sizeof(struct sigaction));
  sigact.sa_handler = timer_handler;

  // initialize timer parameters: expires in 5 seconds
  timerValue.it_interval.tv_sec = 5;
  timerValue.it_interval.tv_usec = 0;

  timerValue.it_value.tv_sec = 5;
  timerValue.it_value.tv_usec = 0;

  // install signal handler to catch SIGALRM
  sigaction(SIGALRM, &sigact, NULL);
  retval = setitimer(ITIMER_REAL, &timerValue, &oldTimerValue);
 
  if (-1 == retval)
    perror("Could not set timer");
 
  while(1);

  return 0;
}
