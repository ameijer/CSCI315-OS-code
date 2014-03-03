//time.c
//Alexander Meijer
//CSCI315
//2013
#include <time.h>
#include <stdio.h>
 
int main(void)
{
   time_t ltime;

   time(&ltime); 
   printf("%d", ltime);
   printf("the time is %s", ctime(&ltime));
}
