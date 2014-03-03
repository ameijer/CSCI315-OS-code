//adt-test.c
//Alexander Meijer
//Feb 27, 2012
//CSCI 315

#include <stdlib.h>
#include <stdio.h>
#include <circular-list.h>

int main (int argc, char *argv[]){
  
  struct circular_list *list = malloc(sizeof(struct circular_list));


  //create list
  int stat =  circular_list_create(list, 5);
  if(stat){
    printf("error in circular_list_create");
    exit(-1);
  }

  //add two numbers to list
  stat = circular_list_insert(list, 1.1);
  if(stat){
    printf("error in circular_list_insert, first element");
    exit(-1);
  }
  stat = circular_list_insert(list, 2.2);
  if(stat){
    printf("error in circular_list_insert, second element");
    exit(-1);
  }
  //remove a number from the list
  item q;
  circular_list_remove(list, &q);
  printf("removed %.1f from list. expected 1.1\n", q);

  //add more numbers to circle around
  stat = circular_list_insert(list, 3.3);
  if(stat){
    printf("error in circular_list_insert, third element");
    exit(-1);
  }

  stat =  circular_list_insert(list, 4.4);
  if(stat){
    printf("error in circular_list_insert, fourth element");
    exit(-1);
  }
  stat =  circular_list_insert(list, 5.5);
  if(stat){
    printf("error in circular_list_insert, fifth element");
    exit(-1);
  }

  stat = circular_list_insert(list, 6.6);
  if(stat){
    printf("error in circular_list_insert, sixth element");
    exit(-1);
  }

  //add one too many numbers. This test should fail
  if (circular_list_insert(list, 7.7) != -1){
    printf("TEST PASSED. ERROR\n");
    exit(-1);
  }

  //remove another number
  stat = circular_list_remove(list, &q);
  if(stat){
    printf("error in circular_list_remove");
    exit(-1);
  }
  printf("removed %.1f from list. expected 2.2\n", q);

  //remove another number
  stat = circular_list_remove(list, &q);
  if(stat){
    printf("error in circular_list_remove");
    exit(-1);
  }
  printf("removed %.1f from list. expected 3.3\n", q);
  
  //remove another number
  stat = circular_list_remove(list, &q);
 if(stat){
    printf("error in circular_list_remove");
    exit(-1);
  }
  printf("removed %.1f from list. expected 4.4\n", q);

  //remove another number
  stat = circular_list_remove(list, &q);
 if(stat){
    printf("error in circular_list_remove");
    exit(-1);
  }
  printf("removed %.1f from list. expected 5.5\n", q);

  //remove another number
  stat = circular_list_remove(list, &q);
 if(stat){
    printf("error in circular_list_remove");
    exit(-1);
  }
  printf("removed %.1f from list. expected 6.6\n", q);

 


  //check that there are 0 elements in the empty list
  printf("There are %d elements in the list. Expected: 0\n", list->elems);

  //this test should fail
  if(circular_list_remove(list, &q) != -1){
    printf("TEST PASSED");
    exit(-1);
  }
}
