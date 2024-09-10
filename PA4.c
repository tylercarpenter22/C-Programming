/* COP 3502C Programming Assignment 4
This program is written by: Tyler Carpenter */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct point {
  int x;
  int y;
} point;

typedef struct store {
  point location;
  double distance;
} store;

// declare global variables
point myPoint;

// function prototypes
store *ReadData(point *searches, int numShops, int numSearches);
void display(store *stores, point *searches, int numShops, int numSearches);
void calcDistance(store *stores, int numShops);
int compareTo(const void *a, const void *b);
void mergeInsertionSort(store *arr, int threshold, int l, int r);
void insertionSort(store *arr, int l, int r);
void mergeSort(store *arr, int l, int r);
void merge(store *arr, int l, int m, int r);
void binarySearch(store *arr, int numShops, point target);

int main(void) {
  // declare variables
  int numShops, numSearches, threshold;

  // get first line of input
  if (scanf("%d %d %d %d %d", &myPoint.x, &myPoint.y, &numShops, &numSearches,
            &threshold) != 5) {
    printf("Error reading input.\n");
    return 1;
  }
  // declare arrays
  point searches[numSearches];
  store *stores;

  // read data and get the array of stores to be sorted
  stores = ReadData(searches, numShops, numSearches);

  if (stores == NULL) {
    free(stores);
    return 1;
  }
  // calculate distances for stores
  calcDistance(stores, numShops);

  // sort the stores by distance
  mergeInsertionSort(stores, threshold, 0, numShops - 1);

  // display output
  display(stores, searches, numShops, numSearches);

  // free memory
  free(stores);
  return 0;
}

// this function reads the required inputs and returns the array of stores
store *ReadData(point *searches, int numShops, int numSearches) {
  // create array of store locations
  store *stores = malloc(numShops * sizeof(store));
  if (stores == NULL) {
    printf("Error: malloc failed for stores.\n");
    return NULL;
  }
  // get input for each store
  for (int i = 0; i < numShops; i++) {
    scanf("%d %d", &stores[i].location.x, &stores[i].location.y);
  }
  // get input for each search
  for (int i = 0; i < numSearches; i++) {
    scanf("%d %d", &searches[i].x, &searches[i].y);
  }
  // return array of stores
  return stores;
}

// this function displays the current contents of the arrays
void display(store *stores, point *searches, int numShops, int numSearches) {
  for (int i = 0; i < numShops; i++) {
    printf("%d %d\n", stores[i].location.x, stores[i].location.y);
  }
  for (int i = 0; i < numSearches; i++) {
    binarySearch(stores, numShops, searches[i]);
  }
}

// this function calculates the distance between each store and the user
void calcDistance(store *stores, int numShops) {
  for (int i = 0; i < numShops; i++) {
    int distanceX = myPoint.x - stores[i].location.x;
    int distanceY = myPoint.y - stores[i].location.y;
    stores[i].distance = sqrt(distanceX * distanceX + distanceY * distanceY);
  }
}

// compareTo function to compare two stores
int compareTo(const void *a, const void *b) {
  store *store1 = (store *)a;
  store *store2 = (store *)b;

  if (store1->distance < store2->distance) {
    return -1;
  } else if (store1->distance > store2->distance) {
    return 1;
  } else {
    // tie-breaking
    if (store1->location.x < store2->location.x) {
      return -1;
    } else if (store1->location.x > store2->location.x) {
      return 1;
    } else {
      if (store1->location.y < store2->location.y) {
        return -1;
      } else if (store1->location.y > store2->location.y) {
        return 1;
      } else {
        return 0;
      }
    }
  }
}

// mergeInsertionSort function to sort the stores using merge sort
void mergeInsertionSort(store *arr, int threshold, int l, int r) {
  if (r - l + 1 <= threshold) {
    insertionSort(arr, l, r);
  } else if (l < r) {
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
  }
}

// insertion sort function
void insertionSort(store *arr, int l, int r) {
  for (int i = l + 1; i <= r; i++) {
    store key = arr[i];
    int j = i - 1;
    while (j >= l && compareTo(&arr[j], &key) > 0) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

// this function merges two sorted subarrays and is called recursively and in
// the mergeInsertionSort function
void mergeSort(store *arr, int l, int r) {
  if (l < r) {
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
  }
}

// this function merges the sorted subarrays and is called in mergeSort
void merge(store *arr, int l, int m, int r) {
  int n1 = m - l + 1;
  int n2 = r - m;

  store *L = (store *)malloc(n1 * sizeof(store));
  store *R = (store *)malloc(n2 * sizeof(store));

  for (int i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (int j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  int i = 0, j = 0, k = l;
  while (i < n1 && j < n2) {
    if (compareTo(&L[i], &R[j]) <= 0) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }

  free(L);
  free(R);
}

// this function uses binary search to look through the sorted list of stores
// and see if one exists at the searched point.
void binarySearch(store *arr, int numShops, point target) {
  int left = 0, right = numShops - 1;
  int found = 0;

  // create temp store to use as search target
  store temp;
  temp.location.x = target.x;
  temp.location.y = target.y;
  int distanceX = myPoint.x - temp.location.x;
  int distanceY = myPoint.y - temp.location.y;
  temp.distance = sqrt(distanceX * distanceX + distanceY * distanceY);

  while (left <= right) {
    int mid = left + (right - left) / 2;

    if (compareTo(&arr[mid], &temp) == 0) {
      printf("%d %d found at rank %d\n", target.x, target.y, mid + 1);
      found = 1;
      break;
    } else {
      if (compareTo(&arr[mid], &temp) < 0) {
        left = mid + 1;
      } else {
        right = mid - 1;
      }
    }
  }

  if (!found) {
    printf("%d %d not found\n", target.x, target.y);
  }
}
