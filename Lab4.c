/* COP 3502C Lab 4
This program is written by: Tyler Carpenter

*/
//-----Summary of the code-------------------------------------
// check the balance of parenthesis and brackets in an expression
// Convert infix to postfix

//---------------------------------------------------------------NOTES TO HELP
// UNDERSTAND HOW TO CHECK BALANCE OF PARENTHESIS AND BRACKETS ALONG WITH INFIX
// AND POSTIFX PROCESS---------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// TO CHECK FOR PARENTHESIS AND BRACKETS IF THEY ARE BALANCED
/*
    Read the string char by char left to right,
    If you see a (, [, or {, push it on the stack
    If you see a ), ], or }, pop the stack and check whether you got the
   corresponding (, [, or { When you reach the end, check that the stack is
   empty. If it is empty, it is balanced. Otherwise it is not empty
*/

// INFIX TO POSTFIX PROCESS
/*
        Start reading the Infix expression from left to right
        If we encounter an operand we will append it to the Postfix string,
   [Operand doesn’t go to stack] If we encounter an operator let’s say current
   operator: The current operator is compared to the operator that is at the top
   of the stack If the priority of the current operator is HIGHER than the
   priority of the operator at the top of the stack, then the current operator
   is pushed at the top of the stack. If the priority of the current operator is
   same or lower than the priority of the operator at the top of the stack, we
   keep POPing operators from the stack until the priority of the current
   operator is higher than the operator at the top of the stack. POPed operators
   are appended to the Postfix in the order they are POPed. Finally, the current
            operator is PUSHED to Stack.
        If we encounter an open parenthesis: immediately pushed in the stack
        If we  encounter a closing parenthesis: POP until an open parenthesis
   (inclusive) found in the stack. Append the popped items to the Postfix except
   the parenthesis.

 */

// pre-processor directives
#include <ctype.h> // needed for isdigit built-in function to check if we have a digit or no
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// global variables:

// The array will store the items in the stack, first in
// index 0, then 1, etc. top will represent the index
// to the top element in the stack. If the stack is
// empty top will be -1.
#define SIZE 100
#define EMPTY -1

// stack struct
struct stack {
  char items[SIZE]; // where the items will be stored
  int top;          // holds the index of the top element
};

// function prototypes
void initialize(struct stack *stackPtr);
int full(struct stack *stackPtr);
int push(struct stack *stackPtr, char value);
int empty(struct stack *stackPtr);
char pop(struct stack *stackPtr);
int peek(struct stack *stackPtr);
void print_stack(struct stack *stackPtr);
int checkBalance(char exp[]);
int priority(char ch);
int isOperator(char ch);
char *infixToPostfix(char infix[]);
int isParentheses(char ch1);

// driver function
int main(void) {
  // variables declaration
  char exp[SIZE]; // hold the expresssion
  int valid;      // holds the state of validity of our expression

  // prompt the user for an expression
  printf("\nEnter Expression: ");
  scanf("%[^\n]s", exp); // includes spaces if written in this format

  // print the input expression
  printf("Your input expression: %s\n", exp);

  // get the balance status
  valid = checkBalance(exp);

  // if the expression is balanced
  // process the postfix and free the memory afterwards
  if (valid) {
    char *postfix = infixToPostfix(exp);
    printf("The postfix is: %s\n", postfix);

    free(postfix);
  }

  // end of the main program
  return 0;
}

// display the stack components
void print_stack(struct stack *stackPtr) {
  for (int i = 0; i <= stackPtr->top; i++)
    printf("%c-->", stackPtr->items[i]);

  printf("\n");
}

// initialize our stack -> simply set top at the start which is -1
void initialize(struct stack *stackPtr) { stackPtr->top = -1; }

// If the push occurs, 1 is returned. If the
// stack is full and the push can't be done, 0 is
// returned.
int push(struct stack *stackPtr, char value) {

  // Check if the stack is full.
  if (full(stackPtr))
    return 0;

  // Add value to the top of the stack and adjust the value of the top.
  stackPtr->items[stackPtr->top + 1] = value;
  (stackPtr->top)++;
  return 1;
}
// The stack pointed to by stackPtr is NOT empty.
// The value on the top of the stack is popped and returned.
// Note: If the stack pointed to by stackPtr is empty, -1 is returned.
char pop(struct stack *stackPtr) {

  char retval;

  // Check the case that the stack is empty.
  if (empty(stackPtr))
    return 'I';

  // Retrieve the item from the top of the stack, adjust the top and return
  // the item.
  retval = stackPtr->items[stackPtr->top];
  (stackPtr->top)--;
  return retval;
}

// The stack pointed to by stackPtr is NOT empty.
// The value on the top of the stack is returned.
// Note: If the stack pointed to by stackPtr is empty, -1 is returned.
int peek(struct stack *stackPtr) {

  // Take care of the empty case.
  if (empty(stackPtr))
    return EMPTY;

  // Return the desired item.
  return stackPtr->items[stackPtr->top];
}
// Returns true iff the stack pointed to by stackPtr is full.
int full(struct stack *stackPtr) { return (stackPtr->top == SIZE - 1); }

// Returns true iff the stack pointed to by stackPtr is empty.
int empty(struct stack *stackPtr) { return (stackPtr->top == -1); }

//--------------------------Important Functions that are not the same as our
// standard stack processing ones!!!---------------------------------
// I wrote this function to determine the priority of operators
// the degree of priority here is ascending.
int priority(char ch) {
  if (ch == '^') {
    return 3;
  }
  if (ch == '*' || ch == '/' || ch == '%') {
    return 2;
  }
  if (ch == '+' || ch == '-') {
    return 1;
  }
  if (ch == '(' || ch == '[' || ch == '{') {
    return 0;
  }
}

// wrote this function to make it easy to check if our current char is an
// operator
int isOperator(char ch) {
  if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' ||
      ch == '%')
    return 1;
  return 0;
}

// check if our current char is a parenthesis or no either opening or closing
// one
int isParentheses(char ch1) {
  if (ch1 == '{' || ch1 == '}' || ch1 == '(' || ch1 == ')' || ch1 == '[' ||
      ch1 == ']')
    return 1;
  return 0;
}

// check if the expression is balanced or no
int checkBalance(char exp[]) {
  // initially we will set the valid state to 1 then whenever in the checks we
  // find a problem set it back to 0
  int valid = 1;
  // create an instance of the struct
  struct stack mine;

  // initialize the stack
  initialize(&mine);

  // small prompt here :)
  printf("\nChecking balance...\n");

  // loop through the whole expression
  for (int i = 0; exp[i] != '\0'; i++) {
    // if it is an opening bracket/parenthesis then push it to the stack
    if (exp[i] == '(' || exp[i] == '{' || exp[i] == '[')
      push(&mine, exp[i]);

    // if we have a closing sign we need to pop, yet if we do not find the
    // matching closing bracket/parenthesis in that case set the balance check
    // to 0
    else if (exp[i] == ')') {
      char a = pop(&mine);
      if (a == 'I' || a != '(') {
        valid = 0;
        printf("INVALID for )!!!\n");
        return valid;
      }
    } else if (exp[i] == '}') {
      char a = pop(&mine);
      if (a == 'I' || a != '{') {
        valid = 0;
        printf("INVALID for }!!!\n");
        return valid;
      }
    }
    // this part is for you guys, yet lets try and leave some hints
    else if (exp[i] == ']') {
      char a = pop(&mine);
      if (a == 'I' || a != '[') {
        valid = 0;
        printf("INVALID for ]!!!\n");
        return valid;
      }
    }
  }

  // not empty we still have something in the stack sadly :(
  // if so we set valid back to 0 and print
  if (pop(&mine) != 'I') {
    valid = 0;
    printf("INVALID. You have extra!!!\n");
  }
  // otherwise success
  if (valid == 1)
    printf("VALID\n");

  // return the balance check status
  return valid;
}

// infix to postfix (you can add more explanatory comments here)
char *infixToPostfix(char infix[]) {
  // declare a struct instance
  // and initialize it
  struct stack operators;
  initialize(&operators);

  int count = 0; // keep track of postfix index

  // allocating memory for postfix
  // allocate enough memory to handle all characters including space and null terminator
  char *postfix = (char *)malloc((strlen(infix) + 1) * sizeof(char));
  if (postfix == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(1);
  }

  // iterating through the infix and evaluating each char
  for (int i = 0; i < strlen(infix); i++) {
    // if the char is a digit puts it into the postfix
    if (isdigit(infix[i])) {
      while (isdigit(infix[i])) {
        postfix[count] = infix[i];
        count++;
        i++;
      }
      i--; // adjust i after the inner loop
      postfix[count] = ' '; // append space after the number
      count++;
    }
    // if the char is an operator, and the stack is empty, pushing it. if the
    // stack isn't empty, checking for priority.
    else if (isOperator(infix[i])) {
      if (empty(&operators))
        push(&operators, infix[i]);

      // if the priority is higher than the top of the stack, pushing it to
      // operators stack
      else if (priority(infix[i]) > priority(peek(&operators))) {
        push(&operators, infix[i]);
      }

      // if priority is lower or equal, popping the stack into the postfix
      // until it is empty or the priority of the char is higher than the one
      // from the top of stack
      else {
        while (!empty(&operators) &&
               priority(infix[i]) <= priority(peek(&operators)) &&
               !isParentheses(peek(&operators))) {
          postfix[count] = pop(&operators);
          count++;
          postfix[count++] = ' ';
        }
        push(&operators, infix[i]);
      }
    }
    // if the char is an opening parenthesis, push it onto the stack
    else if (infix[i] == '(') {
      push(&operators, infix[i]);
    }
    // if the char is a closing parenthesis, pop from the stack until an opening parenthesis is encountered
    else if (infix[i] == ')') {
      while (!empty(&operators) && peek(&operators) != '(') {
        postfix[count++] = pop(&operators);
        postfix[count++] = ' ';
      }
      pop(&operators); // remove the '(' from the stack
    }
  } // end of iterating through the the infix

  // pops all the operators left in the stack and adds white spaces between them.
  while (!empty(&operators)) {
    // use count as your counter as it was pre-declared and first you need to
    // store a space so use ' '
    postfix[count++] = ' ';

    // pop the operator stack here and store it
    postfix[count] = pop(&operators);

    // increment count
    count++;
  }
  // remember we are storing a string so at the end what do we store ???
  postfix[count] = '\0';
  // end return the postfix
  return postfix;
}
