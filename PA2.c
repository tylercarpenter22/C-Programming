/*COP 3502C Assignment 2
This program is written by: Tyler Carpenter

You will write a program that reads in information about customers: which line
they go to the back of (1 through 12), at what time (in seconds) they enter that
line, and the number of smoothies they will buy, and determines at what time
each customer will check out
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPTY -1
#define numLines 12

// Stores one customer
typedef struct customer {
    int time;
    int lineNum;
    char name[15];
    int numSmoothies;
} customer;

// Stores one node of the linked list.
struct node {
    customer data;
    struct node *next;
};

// Stores our queue.
struct queue {
    struct node *front;
    struct node *back;
};


// Function prototypes
customer *createCustomer();
customer *createCustomerArray(int numCustomers);
void init(struct queue *qPtr);
int enqueue(struct queue *qPtr, customer cust);
int dequeue(struct queue *qPtr);
customer peak(struct queue *qPtr);
int empty(struct queue *qPtr);
customer getFirstCustomer(struct queue *qPtr);
customer getNextCustomer(struct queue *qPtr);
int calculateCheckout(customer cust, int currentTime);
void clearQueue(struct queue *qPtr);
//void displayCustomers(customer *customers, int numCustomers);
//void displayLines(struct queue *lines);


int main() {
    // Get number of test cases
    int numTestCases;
    scanf("%d", &numTestCases);

    // Repeat for each test case
    for (int i = 0; i < numTestCases; i++) {
        // Get number of customers
        int numCustomers;
        scanf("%d", &numCustomers);

        // Create array of customers
        customer *customerArray = createCustomerArray(numCustomers);

        //Display list of customers
        //displayCustomers(customerArray, numCustomers);

        // Create array of queues
        struct queue *qPtr =
            (struct queue *)malloc(sizeof(struct queue) * numLines);

        // Initialize queues
        for (int q = 0; q < numLines; q++) {
            init(&qPtr[q]);
        }

        // Queue customers in corresponding line
        for (int j = 0; j < numCustomers; j++) {
            enqueue(&qPtr[customerArray[j].lineNum - 1], customerArray[j]);
        }
      
        //Display lines
        //displayLines(qPtr);
    
        // Calculate checkout time for customers
        int currentTime = 0;
        for (int t = 0; t < numCustomers; t++) {
            customer nextCustomer;
            if (t == 0) {
                nextCustomer = getFirstCustomer(qPtr);
		// Set current time to the arrival time of the first customer
                currentTime = nextCustomer.time;  
            } else {
                nextCustomer = getNextCustomer(qPtr);
            }
            currentTime = calculateCheckout(nextCustomer, currentTime);
        }
	// Clear and free queues
        for (int q = 0; q < numLines; q++) {
            clearQueue(&qPtr[q]);
	}
        //Free memory
        free(customerArray);
        free(qPtr);
    }

    return 0;
}

// This function initializes the queue
void init(struct queue *qPtr) {
    qPtr->front = NULL;
    qPtr->back = NULL;
}

// This function adds a customer to the queue
int enqueue(struct queue *qPtr, customer cust) {
    // Create new node
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    // Store data in new node
    newNode->data = cust;
    // Link the back node's next pointer to this new node.
    newNode->next = NULL;
    // If the queue is empty, set the front and back to the new node.
    if (qPtr->back == NULL) {
        qPtr->front = newNode;
        qPtr->back = newNode;
    } else {
        // Otherwise, add the new node to the end of the queue and change the back of the queue.
        qPtr->back->next = newNode;
        qPtr->back = newNode;
    }
    return 1;
}

// This function removes a customer from the queue
int dequeue(struct queue *qPtr) {
    // If the queue is empty, return EMPTY.
    if (qPtr->front == NULL) {
        return EMPTY;
    }
    // Store a temporary pointer to the beginning of the list
    struct node *tempNode = qPtr->front;
    // Move the front pointer to the next node
    qPtr->front = qPtr->front->next;
    // If dequeuing the last element, update back pointer to NULL.
    if (qPtr->front == NULL) {
        qPtr->back = NULL;
    }
    // Free the memory pointed to by the temporary pointer.
    free(tempNode);
    return 1;
}

// Takes the front of the queue and returns the customer at the front
customer peak(struct queue *qPtr) {
    // If the queue is empty, return an empty customer.
    if (qPtr->front == NULL) {
        customer empty_customer = {-1, -1, "", -1};
        return empty_customer;
    } else {
        // Otherwise, return the customer at the front of the queue.
        return qPtr->front->data;
    }
}

// This function checks if the queue is empty
int empty(struct queue *qPtr) {
    // If the queue is empty, return 1.
    if (qPtr->front == NULL) {
        return 1;
    } else {
        return 0;
    }
}

// This function creates a single customer
customer *createCustomer() {
    // Allocate space
    customer *cust = (customer *)malloc(sizeof(customer));
    // Get customer info
    scanf("%d", &cust->time);
    scanf("%d", &cust->lineNum);
    scanf("%s", cust->name);
    scanf("%d", &cust->numSmoothies);
    return cust;
}

// This function creates an array of customers
customer *createCustomerArray(int numCustomers) {
    // Allocate space for array
    customer *custArray = (customer *)malloc(sizeof(customer) * numCustomers);
    // Store customers
    for (int i = 0; i < numCustomers; i++) {
        custArray[i] = *createCustomer();
    }
    return custArray;
}

// This function determines and returns the first customer to check out
// This is determined by earliest arrival time.
customer getFirstCustomer(struct queue *qPtr) {
    // Declare variables
    customer currentCustomer;
    customer firstCustomer;
    int index;
    // Find first customer to arrive
    int earliestTime = EMPTY;
    for (int i = 0; i < numLines; i++) {
        if (!empty(&qPtr[i])) {
            currentCustomer = peak(&qPtr[i]);
            if (earliestTime == EMPTY || currentCustomer.time < earliestTime) {
                earliestTime = currentCustomer.time;
                firstCustomer = currentCustomer;
                index = i;
            }
        }
    }
    // Dequeue first customer
    dequeue(&qPtr[index]);
    return firstCustomer;
}

// This function determines and returns the next customer to check out.
// This is determined by smallest smoothie order.
customer getNextCustomer(struct queue *qPtr) {
    // Declare variables
    customer currentCustomer;
    customer nextCustomer;
    int minSmoothies = EMPTY;
    int index;
    // Find next customer to arrive
    for (int i = 0; i < numLines; i++) {
        if (!empty(&qPtr[i])) {
            currentCustomer = peak(&qPtr[i]);
            if (minSmoothies == EMPTY || currentCustomer.numSmoothies < minSmoothies) {
                minSmoothies = currentCustomer.numSmoothies;
                nextCustomer = currentCustomer;
                index = i;
            } else if (currentCustomer.numSmoothies == minSmoothies) {
                // Find customer with smallest line number
                if (currentCustomer.lineNum < nextCustomer.lineNum) {
                    nextCustomer = currentCustomer;
                    index = i;
                }
            }
        }
    }
    // Dequeue next customer
    dequeue(&qPtr[index]);
    return nextCustomer;
}

// This function calculates the checkout time for a customer and displays output.
int calculateCheckout(customer cust, int currentTime) {
    // Update currentTime to the customer's arrival time if it is greater
    if (cust.time > currentTime) {
        currentTime = cust.time;
    }
    // Calculate checkout time
    int checkoutTime = currentTime + (cust.numSmoothies * 5) + 30;
    // Display output
    printf("At time %d, %s left the counter from line %d.\n", checkoutTime, cust.name, cust.lineNum);
    return checkoutTime;
}

void clearQueue(struct queue *qPtr) {
    while (!empty(qPtr)) {
        dequeue(qPtr);
    }
}

//This function displays a list of customers
//This is for testing and will not be displayed in the final output.
/*
void displayCustomers(customer *customers, int numCustomers){
  for(int i = 0; i < numCustomers; i++){
    printf("\n%d %d %s %d\n", customers[i].time, customers[i].lineNum, customers[i].name, customers[i].numSmoothies);
  }
}
*/


//This function displays each line and the names of the customers in that line.
//This is for testing and will not be displayed in the final output.
/*
void displayLines(struct queue *lines){
  for (int i = 0; i < numLines; i++){
    printf("\nLine %d: ", i + 1);
    struct node *t = lines[i].front;
    while (t != NULL){
      printf("%s ", t->data.name);
      t = t->next;
    }
  }
}
*/