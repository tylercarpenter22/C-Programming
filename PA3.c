/* COP 3502C Assignment 3
This program is written by: Tyler Carpenter */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_STORES 8
#define MAX_NAME_LENGTH 20

typedef struct store{
  int x, y; //coordinates
  char name[MAX_NAME_LENGTH];
}store;

//declare global variables
store stores[MAX_STORES];
int bestPermutation[MAX_STORES];

//function prototypes
double calculateDistance(store store1, store store2);
void calculateMinDistance(int* current_Permutation, int* used, int storeNum, int level, double currentSum);

int main(void) {
  //get input
  //proccess for each test case
  int testCases;
  scanf("%d", &testCases);
  for(int i = 0; i < testCases; i++){
    //proccess for each store
    int numStores;
    scanf("%d", &numStores);
    numStores *= 2;
    for(int j = 0; j < numStores; j++){
      scanf("%d %d", &stores[j].x, &stores[j].y);
      scanf("%s", stores[j].name);
    }
    
    //initalize variables
    int current_Permutation[MAX_STORES];
    int used[MAX_STORES] = {0};

    //call recursive function to find min sum of distances
    calculateMinDistance(current_Permutation, used, numStores, 0, 0);
    
    // print the results
    printf("Test Case %d:\n", i+1);printf("%.3f\n", calculateDistance(stores[bestPermutation[0]], stores[bestPermutation[1]]));
    for (int k = 0; k < numStores; k += 2) {
      printf("(%s, %s, %.3f)\n", stores[bestPermutation[k]].name, stores[bestPermutation[k + 1]].name, calculateDistance(stores[bestPermutation[k]], stores[bestPermutation[k + 1]]));
    }
  }
  return 0;
}

double calculateDistance(store store1, store store2){
  int distanceX = store1.x - store2.x;
  int distanceY = store1.y - store2.y;
  return sqrt(distanceX * distanceX + distanceY * distanceY);
}


void calculateMinDistance(int* current_Permutation, int* used, int numStores, int level, double currentSum){
  //base case
  //ends when all coordinate pairs are made
  static double minDistance = INFINITY; 

  // base case: all pairs are formed
  if (level == numStores) {
      // check if current permutation gives a lower distance
      if (currentSum < minDistance) {
          minDistance = currentSum;
          // update bestPermutation with current_Permutation
          for (int i = 0; i < numStores; i++) {
              bestPermutation[i] = current_Permutation[i];
          }
      }
      return;
  }
  //recursive case
  for(int i = 0; i < numStores; i++){ //store i
    //mark store i as used
    if(!used[i]){
      used[i] = 1; 
      current_Permutation[level] = i;
    }
    for(int j = i + 1; j < numStores; j++){ //store j
      //mark store j as used
      if(!used[j]){
        used[j] = 1;
        current_Permutation[level + 1] = j;

        //calculate distance
        double distance =calculateDistance(stores[i], stores[j]);

        //call rescuersive function
        calculateMinDistance(current_Permutation, used, numStores, level + 2, currentSum + distance);
        
        //unmark store 2 
        used[j] = 0; 
      }
    }
    //unmark store 1
    used[i] = 0;
    break;
  }
}
