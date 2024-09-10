/* COP 3502C Lab 3
This program is written by: Tyler Carpenter

More Linked List
This code implements some basic operations of singly linked list like inserting
in the beginning and end, delete operation, and display operation

*/


// pre-processor directives
#include <stdio.h>
#include <stdlib.h>
// include the header file
#include "main.h"

// typedef struct node{
// int data;
// struct node *next;
// }node;

int main()
{
  node *root=NULL; //very important line. Otherwise all function will fail
  node *t;
  int ch,ele,v, del;
  while(1){
    printf("\nMenu: 1. insert at front, 2. reverse list 3. Insert to place 0. exit: ");
    scanf("%d",&ch);
    if(ch==0){
      printf("\nGOOD BYE>>>>\n");
      break;
    }
    if(ch==1){
      printf("\nEnter data (an integer): ");
      scanf("%d",&ele);
      root = insert_front(root, ele);
      display(root);
    }
    if(ch==2){
      root = reverse(root);
      printf("List reversed.\n");
      display(root);
    }
    if(ch==3){
    int place;
      printf("\nEnter data (an integer) and place (>1) separated by space: ");
      scanf("%d %d",&ele, &place);
      insertToPlace(root, ele, place);
      display(root);
    }
  }
return 0;
}


// this function takes an item and insert it in the linked list pointed by root.
node* insert_front(node *root, int item){
  // create a new node and fill-up the node
  node *temp = (node*)malloc(sizeof(node));
  temp->data = item;
  temp->next = NULL;

  // check if there is a node in the list or not
  if(root==NULL){
    return temp;
  }
  else{
  // if there is an existing linked list, so put existing root after temp
    // put the existing root after temp
    temp->next = root;
    // make the temp as the root!
    root = temp;
  }
  // next we return the new root as the type of the function is node* so we need a return statement!!
  return root;
}


// take the head of the linked list and print the list
void display(node* t){
  printf("\nPrinting your linked list.......");
  // iterate and print
  while(t != NULL){
    printf("%d ", t->data);
    t = t->next;
  }
}


//Write a function that takes the head of a linked list and then
//reverse the list. Finally, it returns the new head of the linked list. 
//For example, if you passthe following linked list to the function: 10->20->15->17->NULL,
//the linked list will beconverted into this: 17->15->20->10->NULL.
node* reverse(node* head){
  // no need to reverse if head is null
  // or there is only 1 node.
  if (head == NULL || head->next == NULL){
    return head;
  }
  // keep track of the nodes first with one that points to the main list and another to the reveresed list
  // also the reversed list will have its next pointing to null

  //mainlist =  4 NULL
  //reversedList: 3 2 1 NULL
  //temp: 3 2 1 NULL
  
  node * main_list = head;
  node * reversed_list = head;
  main_list = head->next;
  reversed_list->next = NULL;
  
  // loop through the list and reverse the list
  while (main_list != NULL) {
    // keep track of the current node in the main list
    node * temp = main_list;
    
    // move the main list pointer itself to the next one
    main_list = main_list->next;
    
    // reverse the current node and point it to the reversed list, the current node being held by temp
    temp->next = reversed_list;
    
    // move the reversed list pointer to the current node held by the temp node
    reversed_list = temp;
  }
  // return the reversed list
  return reversed_list;
}


//Write a function that takes in a pointer to the head of a linked list, 
//a value to insert into the list, val, and a location in the list in which to
//insert it, place, which is guaranteed to be greater than 1, and does the insertion. If place
//number of items aren’t in the list, just insert the item in the back of the list. You can assume
//that the linked list into which the inserted item is being added is not empty.
void insertToPlace(node* list, int val, int place){
// take the head of the linked list, a value and then a place where this valus
//needs to be inserted
  
  // if the place suggested is less or equal to 1 or if the list doesn't exist
  // then we simply print that the list is empty or the placement is invalid
  if(place <= 1 || list == NULL){
    printf("\nList is empty or place is not valid");
    return;
  }
  // after that if, we actually have now a valid place and list so we can proceeed
  // first Make the new node.
  node * temp = (node*)malloc(sizeof(node));
  temp->data = val;
  
  // Iterate to the spot BEFORE place, the NULL check
  // ensures we dont go off the list if place is too high.
  int cnt = 1;
  while (list->next != NULL &&cnt < place - 1){ //stop before the place
    list = list->next;
    cnt++;
  }
  // Patch in the node.
  temp->next = list->next;
  list->next = temp;
}
