//myshell.c
//Alexander Meijer
//February 18, 2013
//CSCI 315

/*

In this file, I have added 2 extra features to myshell.

Feature 1: Exit
This command simply exits the myshell program. When called, it 
simply prints a goodbye message and exits the program. To use this command,
simply type exit at the prompt.
example usage:
myshell> exit

Feature 2: qnote
Ever need to quickly save a note when you are in your shell? Qnote makes it easy!
Qnote saves whatever text you enter to a file in the current working directory. The 
notes are organized by date in the file. Everything entered after the qnote command
is saved on a single line in the file. Nothing is printed to the shell-this is 
designed to be a minimally invasive note taking program.
example usage:
myshell> qnote This line is a note and will be saved.

(then open up mynotes.txt later on to read the notes) 

*/

#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_TOK_SIZE 128
#define SHELL_BUFFER_SIZE 256
#define ASCII_SPACE 32
#define ASCII_TAB 9
#define ASCII_NEWLINE 10
#define MAX_WHICH 128
#define MAX_LINE_LENGTH 256
#define QNOTE_OUT_FILE "mynotes.txt"


int qnoter(char *input_args[]){
  char templine[SHELL_BUFFER_SIZE];
  char formdate[100];
  struct tm *tmstruct;
  time_t curtime;
  int founddate = 0;
  int q;

  //get current date
  curtime = time(NULL);
  tmstruct = localtime(&curtime);
  
  //store in a string for later use
  strftime(formdate, SHELL_BUFFER_SIZE, "%A %B %d, %Y:\n", tmstruct);

  //read from beginning of file, but always append to end of file
  FILE *notefile = fopen(QNOTE_OUT_FILE, "a+");
  if (NULL == notefile) { //error
    perror("File I/O");
    return -1;
  }
  //scan file to determine if other notes from today are in there
  while(fgets(templine, SHELL_BUFFER_SIZE, notefile) != NULL) {
    if((strstr(templine, formdate)) != NULL) {
      //match found, date is already in file
      founddate = 1;
      break;
    }
  }

  //if no notes from today, add new date section
  if(!founddate){
    fprintf(notefile, "\n%s", formdate);
  }

  q = 1;
  //write new notes to file
  while(input_args[q+1] != NULL){
    fprintf(notefile, "%s ", input_args[q]);
    q++;
  }
  fprintf(notefile, "%s", input_args[q]);

  //close and return an 'OK' status
  fclose(notefile);
  return 0;
}



int main(int argc, char *argv[]){

  char *rawinput = malloc(SHELL_BUFFER_SIZE);
  int numtokens;
  int j;
  int stat;
  int status;
  char *whichin;
  char *whichout;
  FILE *fileptr;
  pid_t pid;

  for(;;) {

    //print prompt
    printf("myshell> ");
    fflush(stdout);
    fflush(stdin);

    //get input as raw sting into buffer
    while(1) {
      if( fgets(rawinput, SHELL_BUFFER_SIZE, stdin) != NULL){
	break;
      }
    }
    
    //tokenize input into array of strings
    if ((rawinput[0] == ASCII_SPACE) || (rawinput[0] == ASCII_TAB) || (rawinput[0] == ASCII_NEWLINE)){
      printf("Invalid command\n");
      fflush(stdout);
      continue;
    }
    
    
    //get size for token arrray
    numtokens = 0;
    for (int i = 1; rawinput[i] != '\0'; i++){
      if(((int) rawinput[i] == ASCII_SPACE) || ((int) rawinput[i] == ASCII_TAB) || ((int) rawinput[i] == ASCII_NEWLINE)){
	numtokens++;

	//dont count repeated whitespace
	if((rawinput[i-1] == ASCII_SPACE) || (rawinput[i-1] == ASCII_TAB) || (rawinput[i-1] == ASCII_NEWLINE)){
	  numtokens--;
	}
      }
    }
    
#ifdef DEBUG
    printf("Number of tokens: %d\n", numtokens);
    fflush(stdout);
#endif

    char *argarray[numtokens];

    //allocate an array for the arguments
    *argarray = calloc(numtokens, MAX_TOK_SIZE);

    //use strtok to tokenize the input string into an array of arguments
    //save first argument-we know it will be there
    argarray[0] = strtok(rawinput," \t\0" );
    
#ifdef DEBUG
    printf("First token: %s\n", argarray[0]); 
#endif
    
    j = 1;
    while(NULL != (argarray[j] = strtok(NULL," \t\0"))) {
      j++;
    }

#ifdef DEBUG
    for(int k = 0; k < numtokens; k++){
      printf("Token #%d: %s\n", k, argarray[k]); 
    }    
#endif


    //at this point, each token is stored in order in the array

    /*EXTRA FUNCTIONALITY: Shell exit command
      This allows the user to exit the shell in a friendly way, instead of forcing the shell to exit with UNIX hotkeys like ctrl+C
    */
    if(!strncmp("exit\n", argarray[0], 5)){
      printf("Exiting myshell. Goodbye!\n");
      fflush(stdout);
      exit(0);
    } 
    
    /*EXTRA FUNCTIONALITY: qnote command
      The qnote command allows the user to types small notes using the shell. These notes are then stored in a file in the current working directory. The notes are organized by date to make it easier to note when things happened.
    */

    if(!strncmp("qnote ", argarray[0], 5)){
      stat = qnoter(argarray);
      if (stat < 0){
    	perror("qnote problem. file not written.");
      }
      continue;
    }

    whichout = malloc(MAX_WHICH);
    whichin = malloc(MAX_WHICH);
    strcpy(whichin, "which ");
    strcat(whichin, argarray[0]);

    //now call which to determine exact path to executable
    fileptr = popen(whichin, "r");
    fgets(whichout, MAX_WHICH, fileptr);
    pclose(fileptr);
#ifdef DEBUG
    printf("ouput of which call: %s", whichout);
#endif

    //re-load the first element with the complete path from which
    argarray[0] = whichout;

    //strip newlines from all arguments. This is done so the system recognizes the command and the arguments
    for (int h = 0; argarray[h] != NULL; h++ ){
      for (int k = 0; k < strlen(argarray[h]);k++)
        {
	  if ( argarray[h][k] == ASCII_NEWLINE)
	    argarray[h][k] = '\0';
        }
    }

    //fork off a child process, and have that execute the process
    if ((pid = fork()) < 0) {
      perror("forking error");
      exit(-1);
    } else if (pid == 0) { //CHILD
      stat = execv(argarray[0], argarray);
      if (stat < 0) {
	perror("execv error: ");
	exit(-1);
      }
    } else {//PARENT
      //have parent wait for child to finish before resuming
      waitpid(pid, &status, 0);
    }
    
    

  }



}
