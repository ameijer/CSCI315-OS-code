//echod.c
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
#include <sys/uio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE     512 // length of message buffer    
#define	QLEN		        6   // length of request queue

#define FOREVER         1

#define TRUE 1
#define FALSE 0
#define ASCII_SPACE 32
#define ASCII_NULL 0

int	num_requests = 0;  // tally of client requests

/*------------------------------------------------------------------------
 * Program:   echod - a server for echo requests
 *
 * Purpose:   repeatedly execute the following:
 *    (0) wait for a connection request from client
 *		(1) wait for a null-terminated string from client
 *		(2) send back the same string to client 
 *		(3) close the connection
 *		(4) go back to step (0)
 *
 * Usage:    echod [ port ]
 *
 *		 port  - a port number in user space
 *
 *------------------------------------------------------------------------
 */

int 
main (int argc, char* argv[]) {

	struct sockaddr_in sad;  // structure to hold server's address	
	struct sockaddr_in cad;  // structure to hold client's address	
	int sd, sd2;	           // socket descriptors			
	int port;		             // protocol port number		
	socklen_t alen;	         // length of address			
	char in_msg[BUFFER_SIZE];  // buffer for incoming message
	unsigned int in_index;   // index to incoming message buffer

	// prepare address data structure

	memset((char *)&sad,0,sizeof(sad)); // zero out sockaddr structure	
	sad.sin_family = AF_INET;	          // set family to Internet	        
	sad.sin_addr.s_addr = INADDR_ANY;   // set the local IP address	

	// verify usage

	if (argc > 1) {			
		port = atoi(argv[1]);	        
	}
	else {
		printf("Usage: %s [ port ]\n", argv[0]);
		exit(-1);
	}

	if (port > 0)	
		// test for illegal value	
		sad.sin_port = htons((u_short)port);
	else {				
		// print error message and exit	
		fprintf(stderr,"ECHOD: bad port number %s\n", argv[1]);
		exit(-1);
	}

	// create socket 

	sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sd < 0) {
		perror("ECHOD: socket creation failed");
		exit(-1);
	}

	// assign IP/port number to socket where connections come in 

	if (bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
		perror("ECHOD: bind failed");
		exit(-1);
	}

	// set up socket to receive incomming connections 

	if (listen(sd, QLEN) < 0) {
		perror("ECHOD: listen failed");
		exit(-1);
	}

	// main server loop - accept and handle requests 

	while (FOREVER) {
		alen = sizeof(cad);

		if ( (sd2 = accept(sd, (struct sockaddr *)&cad, &alen)) < 0) {
			perror("ECHOD: accept failed\n");
			exit(-1);
		}

		num_requests++;

		// receive the string sent by client
		int stats = recv(sd2, in_msg, BUFFER_SIZE, 0);
		if(stats == -1){
		  perror("echod recv");
		  exit(-1);
		}
		
		for(int i = 1; i < strlen(in_msg); i++){
 
		  if((ASCII_SPACE == in_msg[i]) && (ASCII_SPACE == in_msg[i -1])){
		    //i is index of repeated space
		    int j = i;
		    while(ASCII_NULL != in_msg[j]){
		      in_msg[j] = in_msg[j+1];
		      if(!in_msg[j]){
			break;
		      }
		      j++;
		    }
		   
		    i--;
		  }
		}

		// send the received string back to client
		stats = send(sd2, in_msg, BUFFER_SIZE, 0);
		if(stats == -1){
		  perror("echod send");
		  exit(-1);
		}
		close(sd2);
	}
}
