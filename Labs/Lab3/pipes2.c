//pipes2.c
//Alexander Meijer
//January 31, 2012
//CSCI 315 Lab

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 5
#define READ_END 0
#define WRITE_END 1

int main(void) {
	char write_msg[BUFFER_SIZE] = "hello";
	char read_msg;
	int fd[2];
	pid_t pid;
	int status;
	int fd2[2];
	char upper;

	/* create the pipe */
	if (pipe(fd) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}
	
	//create second pipe
	if (pipe(fd2) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}

	/* fork a child process */
	pid = fork();

	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed");
		return 1;
	}

	if (pid > 0) { /* parent process */
		/* close the unused end of the pipes */
		close(fd[READ_END]);
		close(fd2[WRITE_END]);

		/* write to the pipe */
		for (int i = 0; i < BUFFER_SIZE; i++) {
	     
		  //write 1 char to the pipe
		  status = write(fd[WRITE_END], &write_msg[i], 1);
		  
		  fflush(stdout);
		  //check for error
		  if(status < 0){
		    perror("read: ");
		    exit(-1);
		  }

		}

		close(fd[WRITE_END]);
        	/* read from the pipe */
		while (0 < (status = read(fd2[READ_END], &read_msg, 1))){
		  
		  //print 1 char to stdout
		printf("%c\n", read_msg);
		fflush(stdout);
		
		}
		
		//check for error
		if(status < 0){
		    perror("read: ");
		    exit(-1);
		  }

		/* close the write end of the pipe */
	       
		close(fd2[READ_END]);
		
	}
	else { /* child process */
		/* close the unused end of the pipes */
	        close(fd[WRITE_END]);
		close(fd2[READ_END]);

		/* read from the pipe */
		while (0 < (status = read(fd[READ_END], &read_msg, 1))){
		  
		  //print 1 char to stdout
		printf("%c\n", read_msg);
		fflush(stdout);
		upper = (char) toupper(read_msg);
		
		  //write 1 char to the pipe
		  status = write(fd2[WRITE_END], &upper, 1);
		  
		  //check for error
		  if(status < 0){
		    perror("read: ");
		    exit(-1);
		  }
		}

	       
		//check for error
		if(status < 0){
		    perror("read: ");
		    exit(-1);
		  }
	
		printf("\n");

		/* close the write end of the pipe */
		close(fd[READ_END]);
		close(fd2[WRITE_END]);
	     
	}

	return 0;
}
