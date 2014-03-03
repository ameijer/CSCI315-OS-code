//traverse.c
//alexander Meijer
//CSCI 315
//4/17/2013

#include "read_dir.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>


// function prototypes
void process(char *name);
void enqueue(char *name, size_t siz, time_t tim, mode_t mode, que_t *q);
void dequeue(que_t *q);
void peek_front(char *name, size_t *siz, time_t *tim, mode_t *mode, que_t q);
bool queue_empty(que_t q);
void initq(que_t *q);
void analyzer(void);
void queueprinter(void);
que_t nameq;
  que_t dirque;
  que_t fileque;

// main function
int main(int argc,char *argv[]) {

  if (2 != argc) {
    printf(" usage: %s dirpath\n", argv[0]);
    exit(-1);
  }
  else {
    // pass in the starting directory
    process(argv[1]); 
    analyzer();
  }
  
  return 0;
}

//used for debugging
void queueprinter(void){
  int ctr = 1;
  while(!queue_empty(dirque)){
    printf("queue item #%d:\n", ctr++);
    printf(" name: %s\n", dirque.head->name);
    printf(" size: %d\n", (int) dirque.head->size);
    printf(" modification time: %ld\n", (long int) dirque.head->modtime);
 printf(" mode: %d\n", dirque.head->mode);
    dequeue(&dirque);
  }
  printf("==========FILES=============\n");
  ctr = 0;
while(!queue_empty(fileque)){
    printf("queue item #%d:\n", ctr++);
    printf(" name: %s\n", fileque.head->name);
    printf(" size: %d\n", (int) fileque.head->size);
    printf(" modification time: %ld\n", (long int) fileque.head->modtime);
 printf(" mode: %d\n", fileque.head->mode);
    dequeue(&fileque);
  }
}

//iterate through both queues, calculate and print desired values
void analyzer(void){
  int smallestfile = 10000000;
  int largestfile = 0;
  int numdirs = 0;
  int numregfiles = 0;
  char mostrecent[256];
  char leastrecent[256];

  //a quick time calculation-it wont be too hard to be older than this
  time_t curtime;
  time( &curtime);

  long int oldesttime = (long int) curtime;
  long int newesttime = 0; //wont be too hard to be newer than this
  long int sumsizes = 0;
  char *tempname = calloc(1, 256);
  size_t tempsize;
  long int temptime = 0;
  mode_t tempmode; 
  int numtot = 0;

  //first iterate through all the files
  while (!queue_empty(fileque)){
    peek_front(tempname, &tempsize, &temptime, &tempmode, fileque);
    dequeue(&fileque);
    numtot++;
    sumsizes += tempsize;

    if(temptime > newesttime){
      //found most recently modified file
      strcpy(mostrecent, tempname);
      newesttime = temptime;
    }
    if(temptime < oldesttime){
      //found least recently modified file

      strcpy(leastrecent, tempname);
      oldesttime = temptime;
    }
    
    if(tempsize > largestfile){
      //found new largest file
      largestfile = tempsize;
    }
    if(tempsize < smallestfile){
      //found new smallest file
      smallestfile = tempsize;
    }
    if(S_ISREG(tempmode)){
      //found a regular file
      numregfiles++;
    }
  }

  //next iterate through the directories. Note that here I interpret directories as a type of file, so they are included in the calculations, as well as most/least recently modified. 
  while (!queue_empty(dirque)){
    numtot++;

    
    peek_front(tempname, &tempsize, &temptime, &tempmode, dirque);
    dequeue(&dirque);
    if( S_ISDIR(tempmode)){
      numdirs++;
  }
    sumsizes += tempsize;

if(temptime > newesttime){
      //found most recently modified file
  strcpy(mostrecent, tempname);
      newesttime = temptime;
    }
    if(temptime < oldesttime){
      //found least recently modified file
      strcpy(leastrecent, tempname);
      oldesttime = temptime;
    }
    
    if(tempsize > largestfile){
      //found new largest file
      largestfile = tempsize;
    }
    if(tempsize < smallestfile){
      //found new smallest file
      smallestfile = tempsize;
    }

  }
  
  //print results:
  printf("\n===============RESULTS===============\n");
  printf("\nValue of largest file: %d\n", largestfile);
  printf("Value of smallest file: %d\n", smallestfile);
  printf("Average file size: %f\n", (double)sumsizes / (double)numtot);
  printf("Number of directories: %d\n", numdirs);
  printf("Total number of regular files: %d\n", numregfiles);
  printf("File that was most recently modfied: %s\n", mostrecent);
  printf("File that was least recently modified: %s\n", leastrecent);
}



// traverse a directory tree applying a function when a file is found
void process(char *root)
{
  int numOfFiles = 0;
  
  char dname[MAXLENGTH];
  char cname[MAXLENGTH];
  char prefix[MAXLENGTH];
  size_t junk;
  time_t junk2;
  mode_t junk3;
  struct dirent *dp;

  DIR *dirp;
	struct stat file_info;
    initq(&nameq);
    initq(&dirque);
    initq(&fileque);
    enqueue(root, junk, junk2, junk3, &nameq);

    

    while (true != queue_empty(nameq)) {
      peek_front(dname, &junk, &junk2, &junk3,  nameq); 
      dequeue(&nameq);
      
     

      dirp = opendir(dname);
      if (dirp != NULL) { // it is a directory

	if(lstat(dname, &file_info) != 0){
	  perror("Fstat on a dir: ");
	  exit(-1);
	}
	
	//load the dir into the queue for later processing.
	enqueue(dname, file_info.st_size, file_info.st_mtime, file_info.st_mode, &dirque);

	printf("directory : %s\n",dname);
	strncpy(prefix, dname, MAXLENGTH);
	strncat(prefix,"/", MAXLENGTH);
	
	for (dp = readdir(dirp); NULL != dp; dp = readdir(dirp)) {
	  if ((strcmp(dp->d_name,"..") != 0) && 
	      (strcmp(dp->d_name,".") != 0)) {
	    
	    // prevent from infinite loop
	    strncpy(cname, prefix, MAXLENGTH);
	    
	    // concatenate the prefix
	    strncat(cname, dp->d_name, MAXLENGTH); 
	    if(!S_ISLNK(file_info.st_mode)){
	    enqueue(cname,junk, junk2, junk3, &nameq);
	    }
	  }
	}
	closedir (dirp);
      } else { 
	// test if it is a regular file and not a device or link -- TO-DO
	// if this is a regular file, then process it -- TO-DO
	struct stat file_info2;
	if(lstat(dname, &file_info2) != 0){
	  perror("stat on a file: ");
	  exit(-1);
	}

	//load the file into the queue for later processing
	enqueue(dname, file_info2.st_size, file_info2.st_mtime, file_info2.st_mode, &fileque);




	printf(" processing file: %s\n", dname);
	numOfFiles++;
      }      
    } // while
    
    printf(" a total of %d files were counted\n",numOfFiles);
}

// initialize queue data structure
void initq(que_t *q) {
  q->head = q->tail = NULL;
}

// test whether queue data structure is empty
bool queue_empty(que_t q) {
  if (NULL == q.head) {
     return true;
  } else {
    return false;
  }
}

// add an element to queue
void enqueue(char *name, size_t siz, time_t tim, mode_t mode, que_t *q) {
  item_t *temp;
  
  temp = (item_t *)malloc(sizeof(item_t));
  strncpy(temp->name,name,MAXLENGTH);
  temp->size = siz;
  temp->modtime = tim;
  temp->mode = mode;
  temp->next = NULL;
  
  if (true == queue_empty(*q)) {
    q->head = temp;
    q->tail = temp;
  } else {
    q->tail->next = temp;
    q->tail = q->tail->next;
  }
}

// remove an element from the front of the queue
void dequeue(que_t *q) {
  item_t *temp;
  
  if (true == queue_empty(*q)) {
    printf(" error in dequeue \n");
    exit(-1);
  } else {
    temp = q->head;
    q->head = q->head->next;
    free(temp);
  }
}

// find element at front of queue without removing
void peek_front(char *name, size_t *siz, time_t *tim, mode_t *mode, que_t q) {
  if (true == queue_empty(q)) {
    printf(" error in peek \n");
     exit(-1);
  } else {
    strcpy(name, q.head->name);
    *siz = q.head->size;
    *tim = q.head->modtime;
    *mode = q.head->mode;
  }
} 
