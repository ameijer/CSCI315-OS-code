//forkwait.c
//Alexander Meijer
//Jan 24, 2013
//CSCI 315

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>

int fac(int toFac){
  if (toFac == 1){
    return 1;
  } else {
    return toFac * fac(toFac -1);
  }
}

int main(int argc, char *argv[]) {
 
        pid_t pid;
	pid_t pid2;
	int status;

        //fork the process
	if ((pid = fork()) < 0){
	  printf("%s","FORKING ERROR");
	} else if (pid == 0){ /* child1 */

	  for (int counter = 1; counter < 11; counter++){
	    printf("CHILD1: fact(%d) = %d\n",counter, fac(counter));
	  }	
		
	} else { /* parent */

	  //fork the process again 
	  if ((pid2 = fork()) < 0){
	    printf("%s","FORKING ERROR");
	  } else if (pid2 == 0){ /* child2 */
		
		  for (int counter = 1; counter < 11; counter++){
		  printf("CHILD2: fact(%d) = %d\n",counter, fac(counter));
	       	}
		
	  } else { /* parent */
	   waitpid(-1, &status, 0);	
	  }    	

	  waitpid(-1, &status, 0);
	}

	return 0;

}
