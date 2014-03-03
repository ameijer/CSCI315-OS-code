//file-test.c
//Alexnder Meijer
//January 29, 2013
//CSCI 315

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 10

int main(void) {
	char write_msg[BUFFER_SIZE] = "0123456789";
       
	int fd = open("data.txt", O_WRONLY);

		/* write to the file */
		write(fd, write_msg, strlen(write_msg));

		/* close the write end of the file */
		close(fd);
	

	     
	

	return 0;
}
