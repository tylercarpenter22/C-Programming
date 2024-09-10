// Tyler Carpenter made this program
// COP 3502c- Dr Ahmed

// This program calculates the runtime of the different sorting algorithms

// Pre-processor directives
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Constant definition
#define MAXVAL 1000000

// Function prototypes
void randArray(int A[], int size, int maxval);
void bubbleSort(int A[], int n);
void swap(int *a, int *b);
void arrayCopy(int from[], int to[], int size);
void selectionSort(int arr[], int n);
void mergeSort(int arr[], int l, int r);
void merge(int arr[], int l, int m, int r);
int partition(int arr[], int l, int r);
long timediff(clock_t t1, clock_t t2);

// bubble sort function
void bubbleSort(int arr[], int n)
{

    // i and j are counters
    int i, j;

    // for loops used to iterate through each index and compare it to the following one
    // if the current is smaller than the following nothing happens
    // if the following is smaller than the current value at the current index we swap
    for (i = 0; i < n - 1; i++)
    {

        for (j = 0; j < n - i - 1; j++)
        {

            if (arr[j] > arr[j + 1])
            {

                // call the swap function
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }

    // end of the bubble sort function
}

// Selection sort function
void selectionSort(int arr[], int n)
{

    // variables and counters declaration
    int i, j, min_idx, temp;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++)
    {

        // Find the minimum element in unsorted array
        min_idx = i;

        for (j = i + 1; j < n; j++)

            if (arr[j] < arr[min_idx])

                min_idx = j;

        // Swap the found minimum element with the first element
        temp = arr[i];

        // update the i th index (what it contains)
        // update what is at the min_idx
        arr[i] = arr[min_idx];
        arr[min_idx] = temp;
    }

    // end of the selection sort function
}

void insertionSort(int arr[], int n)
{
    int i, item, j;
    for (i = 1; i < n; i++)
    {
        item = arr[i];

        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        for (j = i - 1; j >= 0; j--)
        {
            if (arr[j] > item)
                arr[j + 1] = arr[j];
            else
                break;
        }
        arr[j + 1] = item;
    }
}

void mergeSortinsertionsort(int arr[], int l, int r) {
    /*
    merge insertion sort(combining merge and insertion sort by modifying 
    the merge sort function and insertion sort function.You should call the 
    insertion sort from the merge sort function when the array size reaches 
    to threshold 25)
    */
    const int THRESHOLD = 25;

    if (r - l + 1 <= THRESHOLD) {
        // call insertion sort
    }
    else
        if (l < r) {
            int m = l + (r - l) / 2;
    
            mergeSort(arr, l, m);
            mergeSort(arr, m + 1, r);
    
            merge(arr, l, m, r);
        }
}

// merge sort function
void mergeSort(int arr[], int l, int r)
{

        if (l < r)
    {

        // get the mid point
        int m = (l + r) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r)
{

    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)

        L[i] = arr[l + i];

    for (j = 0; j < n2; j++)

        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray

    while (i < n1 && j < n2)
    {

        if (L[i] <= R[j])
        {

            arr[k] = L[i];
            i++;
        }

        else
        {

            arr[k] = R[j];
            j++;
        }

        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {

        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {

        arr[k] = R[j];
        j++;
        k++;
    }

    // free both L and R
    free(L);
    free(R);
}

//quick sort function
void quicksort(int* numbers, int low, int high) {
    // Only have to sort if we are sorting more than one number
    if (low < high) {
        int split = partition(numbers,low,high);
        quicksort(numbers,low,split-1);
        quicksort(numbers,split+1,high);
    }
}

//partition function
int partition(int* vals, int low, int high) {
    int temp;
    int i, lowpos;
    // Pick a random partition element and swap it into index low.
    i = low + rand()%(high-low+1);
    swap(&vals[low], &vals[i]);
    // Store the index of the partition element.
    lowpos = low;
    // Update our low pointer.
    low++;
    // Run the partition so long as the low and high counters don't cross.
    while (low <= high) {
    // Move the low pointer until we find a value too large for this side.
        while (low <= high && vals[low] <= vals[lowpos]) low++;
        // Move the high pointer until we find a value too small for this side.
        while (high >= low && vals[high] > vals[lowpos]) high--;
        // Now that we've identified two values on the wrong side, swap them.
        if (low < high)
            swap(&vals[low], &vals[high]);
    }
    // Swap the partition element into it's correct location.
    swap(&vals[lowpos], &vals[high]);
    
    return high; // Return the index of the partition element.
}

// timediff function calculates the duration of the sorting process depending on
// the start and end time for each sorting passed to it
long timediff(clock_t t1, clock_t t2)
{

    // elapsed represents the time that the sorting took
    // from start to finish
    long elapsed;

    // Calculate elapsed for each t1 and t2 passed at each time to the function
    // CLOCKS_PER_SEC defines the number of clock ticks per second for a particular machine.
    elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000;

    // return elapsed
    return elapsed;
}

// swap function swaps two elements pointed to
// by the pointers a and b
void swap(int *a, int *b)
{

    // temp will hold what is pointed to by a
    int temp = *a;

    // swap what's in a to what is in b
    *a = *b;

    // swap what's in b to what was in a through the use of temp
    *b = temp;
}

// main function
int main(void)
{

    // variables declaration
    int sizes[] = {10000, 20000, 30000, 40000, 50000, 60000};

    int *originalArray;
    int *sortedArray;
    int i, j;
    clock_t start, end;
    long elapsed;

    // for loop used to iterate through each index of the sizes array.
    // the process will be repeated for each sorting method accordingly.
    // then we will print each sorting runtime size by size.
    // for each sorting t1 will represent the start time and t2 the end time for each
    // iteration. Elapsed time will be updated accordingly case by case.
    for (i = 0; i < 6; i++)
    {
        originalArray = malloc(sizeof(int) * sizes[i]);
        sortedArray = malloc(sizeof(int) * sizes[i]);

        randArray(originalArray, sizes[i], MAXVAL);

        //bubble sort
        arrayCopy(originalArray, sortedArray, sizes[i]);
        start = clock();
        bubbleSort(sortedArray, sizes[i]);
        end = clock();

        printf("Sorting %d values took %ld milliseconds for Bubble sort.\n", sizes[i], timediff(start, end));

        //selection sort
        arrayCopy(originalArray, sortedArray, sizes[i]);
        start = clock();
        selectionSort(sortedArray, sizes[i]);
        end = clock();

        printf("Sorting %d values took %ld milliseconds for Selection sort.\n", sizes[i], timediff(start, end));

        //merge sort
        arrayCopy(originalArray, sortedArray, sizes[i]);
        start = clock();
        mergeSort(sortedArray, 0, sizes[i] -1);
        end = clock();
        
        printf("Sorting %d values took %ld milliseconds for Merge sort.\n", sizes[i], timediff(start, end));

        //quick sort
        arrayCopy(originalArray, sortedArray, sizes[i]);
        start = clock();
        quicksort(sortedArray, 0, sizes[i] - 1);
        end = clock();

        printf("Sorting %d values took %ld milliseconds for Quick sort.\n", sizes[i], timediff(start, end));

        //insertion sort
        arrayCopy(originalArray, sortedArray, sizes[i]);
        start = clock();
        insertionSort(sortedArray, sizes[i]);
        end = clock();

        printf("Sorting %d values took %ld milliseconds for Insertion sort.\n", sizes[i], timediff(start, end));

        //seperates cases
        printf("\n");
        
        free(originalArray);
        free(sortedArray);
        
        /*

        another method:

        start = time(0);
        bubbleSort(sortedArray, sizes[i]);
        end = time(0);

         printf("Sorting %d values took %d seconds for Bubble sort.\n", sizes[i], end-start);

        //the sorted array got sorted. So, we are copying the data from original array to sorted array again
        arrayCopy(originalArray, sortedArray, sizes[i]);

        */
    }

    // End of the main function
    return 0;
}

// arrayCopy function copies the elements in order from the array
// "from" to the array "to"
void arrayCopy(int from[], int to[], int size)
{

    // i is a counter
    int i;

    // copy each element accordingly from the array "from" to the array "to"
    for (i = 0; i < size; i++)
    {

        to[i] = from[i];
    }
}

// ranArray function creates an array of size: size
// with random values from 0 to maxval included
void randArray(int A[], int size, int maxval)
{

    // i is a counter
    int i;

    // iterate through each index of the array and put a random value inside
    // each index of the array
    for (i = 0; i < size; i++)
    {

        A[i] = rand() % MAXVAL + 1;
    }
}
