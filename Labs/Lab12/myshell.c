//myshell.c
//Alexander Meijer
//April 25, 2013
//CSCI 315


//MODDED FOR LAB 12

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


/*************ADDITIONAL CHALLENGE COMPLETED***************************
Hitting the enter key after each tab is lame. With the code below you dont have to.
 ********************************************************************/

#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termio.h>
#include <termios.h>

#define MAX_TOK_SIZE 128
#define SHELL_BUFFER_SIZE 256
#define ASCII_SPACE 32
#define ASCII_TAB 9
#define ASCII_NEWLINE 10
#define MAX_WHICH 128
#define MAX_LINE_LENGTH 256
#define QNOTE_OUT_FILE "mynotes.txt"


//prototypes
void runner(char *rawinput);
int qnoter(char *input_args[]);

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

  char *rawinput = calloc(SHELL_BUFFER_SIZE, 1);
  struct termios old_tio, new_tio;
  unsigned char c;
  int index = 0;
  int tabcount = 0;
  for(;;) {
    //print prompt
    printf("myshell> ");
    fflush(stdout);
    fflush(stdin);

    //this code adapted from http://shtrom.ssji.net/skb/getc.html

    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO,&old_tio);

    /* we want to keep the old setting to restore them a the end */
    new_tio=old_tio;

    /* disable canonical mode (buffered i/o) and local echo */
    new_tio.c_lflag &=(~ICANON & ~ECHO);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

    //read the new command in, noting any tabs entered
    index = 0;
    tabcount = 0;
    do {
      c=getchar();
      rawinput[index] = c;
      printf("%c", c);
      if ('\t' == c){
	tabcount++;
		   
      }

      if(tabcount == 2){
	break;
      }
		 
      index++;
    } while(c!='\n');
	
    //add null char to end to replicate behavior of fgets
    rawinput[index] = '\0';

    /* restore the former settings */
    tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

    if(tabcount == 2){
      printf("\n");
      fflush(stdout);
      rawinput[0] = 'l';
      rawinput[1] = 's';
      rawinput[2] = '\n';
      rawinput[3] = '\0';
    }
    

    //tokenize input into array of strings
    if ((rawinput[0] == ASCII_SPACE) || (rawinput[0] == ASCII_TAB) || (rawinput[0] == ASCII_NEWLINE)){
      printf("Invalid command\n");
      fflush(stdout);
      continue;
    }
    
    //use pipe redirection
    int numpipes = 0;
    for(int i = 0; i < SHELL_BUFFER_SIZE; i++){
      if(rawinput[i] == '|'){
	numpipes++;
      }
    }
    
    if (numpipes > 1){
      printf("Error: too many pipes\n");
      continue;
    }
  
    if (numpipes == 1){ 

      int fd[2];
      int j;
      int status;
      pid_t pid;
      
      char *cmd1;
      char *cmd2;
    
      cmd1 = strtok(rawinput,"|" );
      cmd2 = strtok(NULL, "|");
 

      //now tokenzie each sub command

      //get size for token arrray
      int numtokens1 = 1;
      for (int i = 1; cmd1[i] != '\0'; i++){
	if(((int) cmd1[i] == ASCII_SPACE) || ((int) cmd1[i] == ASCII_TAB) || ((int) cmd1[i] == ASCII_NEWLINE)){
	  numtokens1++;

	  //dont count repeated whitespace
	  if((cmd1[i-1] == ASCII_SPACE) || (cmd1[i-1] == ASCII_TAB) || (cmd1[i-1] == ASCII_NEWLINE)){
	    numtokens1--;
	  }
	}
      }
    
#ifdef DEBUG
      printf("raw cmd1: %s\n", cmd1);
      printf("Number of tokens in first command: %d\n", numtokens1);
      fflush(stdout);
#endif


       int numtokens2 = 1; 
      for (int i = 1; cmd2[i] != '\0'; i++){
      	if(((int) cmd2[i] == ASCII_SPACE) || ((int) cmd2[i] == ASCII_TAB) || ((int) cmd2[i] == ASCII_NEWLINE)){
      	  numtokens2++;

      	  //dont count repeated whitespace
      	  if((cmd2[i-1] == ASCII_SPACE) || (cmd2[i-1] == ASCII_TAB) || (cmd2[i-1] == ASCII_NEWLINE)){
      	    numtokens2--;
      	  }
      	}
      }
    
#ifdef DEBUG
      printf("raw cmd2: %s\n", cmd2);fflush(stdout);
      printf("Number of tokens in second command: %d\n", numtokens2);
      fflush(stdout);
#endif

      char *argarray1[numtokens1];
      char *argarray2[numtokens2];

      //allocate an array for the arguments
      *argarray1 = calloc(numtokens1, MAX_TOK_SIZE);
      *argarray2 = calloc(numtokens2, MAX_TOK_SIZE);

      //use strtok to tokenize the input string into an array of arguments
      //save first argument-we know it will be there
      argarray1[0] = strtok(cmd1," \t\0" );
    
#ifdef DEBUG
      printf("First token of cmd 1: %s\n", argarray1[0]); 
#endif
    
      j = 1;
      while(NULL != (argarray1[j] = strtok(NULL," \t\0"))) {
	j++;
      }


      argarray2[0] = strtok(cmd2," \t\0" );
    
#ifdef DEBUG
      printf("First token of cmd 2: %s\n", argarray2[0]); 
#endif
    
      j = 1;
      while(NULL != (argarray2[j] = strtok(NULL," \t\0"))) {
	j++;
      }


      //strip newlines from all arguments. This is done so the system recognizes the command and the arguments
      for (int h = 0; argarray1[h] != NULL; h++ ){
	for (int k = 0; k < strlen(argarray1[h]);k++)
	  {
	    if ( argarray1[h][k] == ASCII_NEWLINE)
	      argarray1[h][k] = '\0';
	  }
      }
      //strip all quotations from string literals in 1
      for (int h = 0; argarray1[h] != NULL; h++ ){
	for (int k = 0; k < strlen(argarray1[h]);k++)
	  {
	    if ( argarray1[h][k] == '"')
	      for(int q = k; q < strlen(argarray1[h]); q++){
		argarray1[h][q] = argarray1[h][q+1];
	      }
	  }
      }


      for (int h = 0; argarray2[h] != NULL; h++ ){
	for (int k = 0; k < strlen(argarray2[h]);k++)
	  {
	    if ( argarray2[h][k] == ASCII_NEWLINE)
	      argarray2[h][k] = '\0';
	  }
      }

 //strip all quotations from string literals in 2
      for (int h = 0; argarray2[h] != NULL; h++ ){
	for (int k = 0; k < strlen(argarray2[h]);k++)
	  {
	    if ( argarray2[h][k] == '"')
	      for(int q = k; q < strlen(argarray2[h]); q++){
		argarray2[h][q] = argarray2[h][q+1];
	      }
	  }
      }

#ifdef DEBUG
      for(int k = 0; k < numtokens1; k++){
	printf("Token #%d of cmd1: %s\n", k, argarray1[k]); fflush(stdout);
      }    
      for(int k = 0; k < numtokens2; k++){
	printf("Token #%d of cmd2: %s\n", k, argarray2[k]); fflush(stdout);
      } 
#endif


   
 //fork off a child process, and have that execute the process
  if ((pid = fork()) < 0) {
    perror("forking error");
    exit(-1);
  } else if (pid == 0) { //CHILD

    //input redirection code segment below adapted from http://stackoverflow.com/questions/2191414/implement-piping-using-c-fork-used
    pid_t childpid;
    pipe(fd);
    childpid=fork();
    if (childpid == -1)
    {
        perror("Error forking...");
        exit(1);
    }
    if (childpid)   /*parent proces*/ 
    {
        wait(&childpid);        //waits till the child send output to pipe
        close(fd[1]);
        close(0);       //stdin closed
        dup2(fd[0],0);
        execvp(argarray2[0], argarray2);
       

    }
    if (childpid==0)
    {
        close(fd[0]);   /*Closes read side of pipe*/
        close(1);       //STDOUT closed
        dup2(fd[1],1);
        execvp(argarray1[0], argarray1);
	
    }

    //above segment adapted from http://stackoverflow.com/questions/2191414/implement-piping-using-c-fork-used
  }else {//PARENT
    //have parent wait for child to finish before resuming
    waitpid(pid, &status, 0);

    //loop back to the top for next command
    continue;
  } 

    
    }
  
    //split input up based on semicolon
    

    //count semicolons:
    int numsemis = 0;
    for(int i = 0; i < SHELL_BUFFER_SIZE; i++){
      if(rawinput[i] == ';'){
	numsemis++;
      }
    }

    char **cmds = calloc(numsemis + 1, sizeof(SHELL_BUFFER_SIZE));
    
    cmds[0] = strtok(rawinput,";" );
    int k = 1;
    while(NULL != (cmds[k] = strtok(NULL,";"))) {
      k++;
    }
    for(int j = 0; j < numsemis + 1; j++){
      runner(cmds[j]);
    }
  
  }
}


void runner(char *rawinput){

  int numtokens;
  int j;
  int stat;
  int status;
  char *whichin;
  char *whichout;
  FILE *fileptr;
  pid_t pid;

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


  /*     at this point, each token is stored in order in the array */


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
    return;
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

   } else { //PARENT
    //have parent wait for child to finish before resuming
    waitpid(pid, &status, 0);
  }

}


  
