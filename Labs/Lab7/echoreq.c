//echoreq.c
//Alexander Meijer
//Feb 27, 2012
//CSCI 315

/*
 * CSCI 315 Operating Systems Design
 * Author: L. Felipe Perrone, modified by A. Meijer
 * Date: 2010-02-16
 * Copyright (c) 2011 Bucknell University
 *
 * Permission is hereby granted, free of charge, to any individual
 * or institution obtaining a copy of this software and associated
 * documentation files (the "Software"), to use, copy, modify, and
 * distribute without restriction, provided that this copyright
 * and permission notice is maintained, intact, in all copies and
 * supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL BUCKNELL UNIVERSITY BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 512
#define TRUE 1
#define FALSE 0

/*------------------------------------------------------------------------
 * Program:   echoreq
 *
 * Purpose:  allocate a socket, connect to a server, transfer requested
 *            file to local host, and print file contents to stdout
 *
 * Usage:    echoreq [ host ] [ port ] [ string ] 
 *
 *		 host  - name of a computer on which server is executing
 *		 port  - protocol port number server is using
 *     string - a string in double quotes
 *     
 *------------------------------------------------------------------------
 */

int
main(int argc, char* argv[]) {

	struct	hostent	 *ptrh;	 // pointer to a host table entry	
	struct	sockaddr_in sad; // structure to hold an IP address	

	int	sd;		                 // socket descriptor			
	int	port;		               // protocol port number		
	char *host;                // pointer to host name		
	char  in_msg[BUFFER_SIZE]; // buffer for incoming message

	unsigned int in_index;     // index to incoming message buffer
	int bytes_read;
	int no_zero;
	int ret_val;

	memset((char *)&sad,0,sizeof(sad)); // zero out sockaddr structure	
	sad.sin_family = AF_INET;	          // set family to Internet	

	// verify usage

	if (argc < 4) {
		printf("Usage: %s [ host ] [ port ] [ string ]\n", argv[0]);
		exit(-1);
	}

	host = argv[1];		
	port = atoi(argv[2]);	

	if (port > 0)	
		// test for legal value		
		sad.sin_port = htons((u_short)port);
	else {				
		// print error message and exit	
		printf("ECHOREQ: bad port number %s\n", argv[2]);
		exit(-1);
	}

	// convert host name to equivalent IP address and copy to sad 

	ptrh = gethostbyname(host);

	if ( ((char *)ptrh) == NULL ) {
		printf("ECHOREQ: invalid host: %s\n", host);
		exit(-1);
	}

	memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

	// create socket 

	sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sd < 0) {
		printf("ECHOREQ: socket creation failed\n");
		exit(-1);
	}

	// connect the socket to the specified server 

	if (connect(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
		perror("ECHOREQ: connect failed");
		exit(-1);
	}

	// send message to server
	int stats = send(sd, argv[3], BUFFER_SIZE, 0);
	if(stats == -1){
	  perror("echoreq send");
	  exit(-1);
	}
	// receive message echoed back by server

	stats = recv(sd, in_msg, BUFFER_SIZE, 0);
	if(stats == -1){
	  perror("echoreq recv");
	  exit(-1);
	}
	printf("ECHOREQ: from server= %s\n", in_msg);

	// close the socket   
	close(sd);

	// terminate the client program gracefully 
	return(0);
}
