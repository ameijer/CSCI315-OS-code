//pre-lab2.c
//Alexander Meijer
//January 23, 2012
//CSCI 315

#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/prctl.h>

int main(int argc, char *argv[]) {
  int pcount = 0;
  int ccount = 0;
  pid_t pid;

  //fork the process
	if ((pid = fork()) < 0){
	  printf("%s","FORKING ERROR");
	} else if (pid == 0){ /* child */
		
		for(;;) {
		  printf("%s%d %s", "child: ",ccount++, "\n") ;
		}

		
	} else { /* parent */
	       	for(;;) {
		  printf("%s%d%s", "parent: ",pcount++, "\n") ;
		} 
	}

}
