// LAB 7 - COP 3502 - LL with Recursion
// Author: Tyler  Carpenter
// This code shows performing various operations on linked list recursively.
// insert, delete, free, display, copy
// pre-processor directives

#include <stdio.h>
#include <stdlib.h>
#include "leak_detector_c.h"

// struct to build our nodes
struct node{
  int data;
  struct node *next;
};
// function prototypes
struct node *insert(struct node *list, int d);
struct node *del(struct node *list, int d);
void print(struct node *list);
void freeList(struct node *list);
void copy(struct node *q, struct node **s);

// driver function
int main(){
  
  atexit(report_mem_leak); // for leak detector
  int number = 0, choice = 0;
  struct node *pList = NULL;
  struct node *nList = NULL;
  
  // Let the user add values until they enter -1.
  while (choice != 4){
    // Get the operation.
    printf("\nDo you want to (1)insert, (2)delete, (3)Copy (4)quit.\n");
    scanf("%d", &choice);
    
    printf("Your choice is %d\n", choice);
    
    // Execute the operation.
    if (choice == 1){
      // Get the number.
      printf("Enter the value to insert\n");
      scanf("%d", &number);
      pList = insert(pList, number);
      // Look at the list.
      printf("Items in linked list: ");
      print(pList);
      // printf("\n");
    }
    else if (choice == 2){ // Get the number.
      printf("Enter the value to delete.\n");
      scanf("%d", &number);
      pList = del(pList, number);
      // Look at the list.
      printf("Items in linked list: ");
      print(pList);
      // printf("\n");
    }
    else if (choice == 3){
      if (nList)
        freeList(nList);
      
      copy(pList, &nList); // passing reference of nList as it is not returning anything
                          // Look at the list.
      printf("Items in NEW linked list: ");
      print(nList);
      // printf("\n");
    }
    else{
      break;
    }
  }
  freeList(nList);
  freeList(pList);
  printf("\nBye..\n");
  return 0;
}


/* copies a linked list into another */
void copy(struct node *source, struct node **dest){
  if (source != NULL){
    *dest = malloc(sizeof(struct node));
    (*dest)->data = source->data;
    (*dest)->next = NULL;

    //Think about the repetitive process of the other nodes
    copy(source->next, &((*dest)->next));
  }
}


// Recursively inserts item into the LL pointed to by list.
struct node *insert(struct node *list, int item){
  // Node is at front, insert and return.
  if (list == NULL || item <= list->data){
    struct node *pNew = malloc(sizeof(struct node));
    pNew->data = item;
    pNew->next = list;
    return pNew;
  }
  // Recursively insert and return the front of the list.
  list->next = insert(list->next, item);
  return list;
}


// Recursively deletes the first node storing item in the list pointed to
// by list. If no such node exists, no changes are made. a pointer to the front
// of the resulting list is returned.
struct node *del(struct node *list, int item){
  if(list == NULL){
    return list;
  }
  // Free this node and return a pointer to the rest of the list.
  if (list->data == item){
    struct node *temp = list->next;
    free(list);
    return temp;
  }
  // Recursively delete and return the front of the list.
  list->next = del(list->next, item);
  return list;
}


// display
void print(struct node *list){
  // Iterate through each item and print it!
  // first check that the list is not Null
  if (list != NULL){
    // access the data part and print it
    printf("->%d", list->data); 
    // call the function recursively and send the next node to be printed
    print(list->next);
  }
  
}


// Frees each node in the LL pointed to by list.
void freeList(struct node *list){
  // check If null then just return
  if (list == NULL) 
    return;
  // if there exists a next element then call the function recursively by sending the next
  // element as a parameter
  if (list->next != NULL)
    freeList(list->next);
  // free the head at the end :)
  free(list);
}
