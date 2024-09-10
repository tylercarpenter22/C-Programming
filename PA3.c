/* COP 3502C Assignment 3
This program is written by: Tyler Carpenter */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STORES 16
#define MAX_NAME_LENGTH 20

typedef struct store {
    int x, y; // coordinates
    char name[MAX_NAME_LENGTH];
} store;

// declare global variables
store stores[MAX_STORES];
int bestPermutation[MAX_STORES];

// function prototypes
double calculateDistance(store store1, store store2);
void calculateMinDistance(int *current_Permutation, int *used, int numStores, int level, double currentSum, double *minDistance, double (*distanceMatrix)[MAX_STORES]);

int main(void) {
    int testCases;
    scanf("%d", &testCases);
    for (int i = 0; i < testCases; i++) {
        // process for each store
        int numStores;
        scanf("%d", &numStores);
        numStores *= 2; // convert number of pairs to number of stores
        for (int j = 0; j < numStores; j++) {
            scanf("%d %d %s", &stores[j].x, &stores[j].y, stores[j].name);
        }

        // initialize variables
        int current_Permutation[MAX_STORES];
        int used[MAX_STORES] = {0};
        double minDistance = INFINITY;
        double distanceMatrix[MAX_STORES][MAX_STORES];

        // precompute distances
        for (int m = 0; m < numStores; m++) {
            for (int n = 0; n < numStores; n++) {
                distanceMatrix[m][n] = calculateDistance(stores[m], stores[n]);
            }
        }

        // call recursive function to find min sum of distances
        calculateMinDistance(current_Permutation, used, numStores, 0, 0, &minDistance, distanceMatrix);

        // print the results
        printf("%.3f\n", minDistance);
        for (int k = 0; k < numStores; k += 2) {
            printf("(%s, %s, %.3f)\n", stores[bestPermutation[k]].name, stores[bestPermutation[k + 1]].name,
                   distanceMatrix[bestPermutation[k]][bestPermutation[k + 1]]);
        }
    }
    return 0;
}

double calculateDistance(store store1, store store2) {
    int distanceX = store1.x - store2.x;
    int distanceY = store1.y - store2.y;
    return sqrt(distanceX * distanceX + distanceY * distanceY);
}

void calculateMinDistance(int *current_Permutation, int *used, int numStores, int level, double currentSum, double *minDistance, double (*distanceMatrix)[MAX_STORES]) {
    // base case: all pairs are formed
    if (level == numStores) {
        // check if current permutation gives a lower distance
        if (currentSum < *minDistance) {
            *minDistance = currentSum;
            // update bestPermutation with current_Permutation
            for (int i = 0; i < numStores; i++) {
                bestPermutation[i] = current_Permutation[i];
            }
        }
        return;
    }

    // recursive case with pruning
    if (currentSum >= *minDistance) {
        return; // prune branches that exceed the current minimum
    }

    for (int i = 0; i < numStores; i++) {
        if (!used[i]) {
            used[i] = 1; // mark store i as used
            current_Permutation[level] = i;
            for (int j = i + 1; j < numStores; j++) {
                if (!used[j]) {
                    used[j] = 1; // mark store j as used
                    current_Permutation[level + 1] = j;

                    //distance between store i and store j
                    double distance = distanceMatrix[i][j];

                    // recursively call with the next level and updated current sum
                    calculateMinDistance(current_Permutation, used, numStores, level + 2, currentSum + distance, minDistance, distanceMatrix);

                    used[j] = 0; // backtrack: unmark store j as used
                }
            }
            used[i] = 0; // backtrack: unmark store i as used
        }
    }
}
