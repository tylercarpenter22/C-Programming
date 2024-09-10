/* COP 3502C Assignment 1
This program is written by: Tyler Carpenter


Given a list of possible smoothie ingredients, a list of smoothie recipes, and
lists of sales from several stores, determine how much of each ingredient each
store must order
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct item {
    int itemNo;
    int portions;
} item;

typedef struct recipe {
    int itemCount;
    item *itemList;
    int totalPortions;
} recipe;

char **readIngredients(int *numIngredients);
recipe *readRecipe();
recipe **readAllRecipes(int numRecipes);
double *calculateOrder(int ingredientCount, int numSmoothies, recipe **recipeList);
void printOrder(char **ingredientNames, double *orderList, int numIngredients);
void freeIngredients(char **ingredientList, int numIngredients);
void freeRecipes(recipe **allRecipes, int numRecipes);

int main(void) {
    // Declare variables
    int numIngredients, numStores, numSmoothies;

    // Get number of ingredients
    scanf("%d", &numIngredients);

    // Read ingredients
    char **ingredientList = readIngredients(&numIngredients);

    // Read number of recipes
    scanf("%d", &numSmoothies);

    // Read recipes
    recipe **recipeList = readAllRecipes(numSmoothies);

    // Get number of stores
    scanf("%d", &numStores);

    // Get, print, and free order for each store
    for (int i = 0; i < numStores; i++) {
        double *orderList = calculateOrder(numIngredients, numSmoothies, recipeList);
        printf("List of items for store %d:\n", i + 1);
        printOrder(ingredientList, orderList, numIngredients);
        free(orderList);
    }
    // Free ingredients
    freeIngredients(ingredientList, numIngredients);

    // Free recipes
    freeRecipes(recipeList, numSmoothies);
    
    return 0;
}

// Pre-condition: reference to a variable to store number of ingredients.
// Post-condition: Reads in numIngredients and that number of strings from
// the inputs, allocates an array of
// strings to store the input, and sizes each
// individual string dynamically to be the
// proper size (string length plus 1), and
// returns a pointer to the array.
char **readIngredients(int *numIngredients) {
    // Allocate space for 2D array of strings
    char **ingredients = (char **)malloc(*numIngredients * sizeof(char *));
    // Store ingredients
    for (int i = 0; i < *numIngredients; i++) {
        // Get string length
        char temp[256]; // temporary buffer
        scanf("%s", temp);
        int len = strlen(temp) + 1;
        // Allocate space for strings
        ingredients[i] = (char *)malloc(len * sizeof(char));
        strcpy(ingredients[i], temp);
    }
    return ingredients;
}

// Pre-condition: does not take any parameter
// Post-condition: Reads in details of a recipe such as numItems,
// Dynamically allocates space for a single
// recipe, dynamically allocates an array of
// item of the proper size, updates the
// numItems field of the struct, fills the
// array of items appropriately based on the
// input and returns a pointer to the struct
// dynamically allocated.
recipe *readRecipe() {
    // Allocate space for recipe
    recipe *newRecipe = (recipe *)malloc(sizeof(recipe));
    // Get number of ingredients
    scanf("%d", &newRecipe->itemCount);
    // Allocate space for itemList
    newRecipe->itemList = (item *)malloc(newRecipe->itemCount * sizeof(item));
    // Initialize totalPortions
    newRecipe->totalPortions = 0;
    // Store items in itemList for recipe
    for (int i = 0; i < newRecipe->itemCount; i++) {
        scanf("%d %d", &newRecipe->itemList[i].itemNo, &newRecipe->itemList[i].portions); // Store index and ratio of ingredient
        // Update totalPortions
        newRecipe->totalPortions += newRecipe->itemList[i].portions;
    }
    return newRecipe;
}

// Pre-condition: reference to a variable to store number of recipes.
// Post-condition: Read number of recipes. Dynamically allocates an array of
// pointers to recipes of size numRecipes, reads numRecipes
// number of recipes from standard input, creates
// structs to store each recipe and has the
// pointers point to each struct, in the order
// the information was read in. (Should call
// readRecipe in a loop.)
recipe **readAllRecipes(int numRecipes) {
    // Allocate space for array of recipe pointers
    recipe **smoothieList = (recipe **)malloc(numRecipes * sizeof(recipe *));
    // Store recipes
    for (int i = 0; i < numRecipes; i++) {
        smoothieList[i] = readRecipe();
    }
    return smoothieList;
}

// Pre-condition: 0 < numSmoothies <= 100000, recipeList is
// pointing to the list of all smoothie recipes and
// numIngredients equals the number of total ingredients (you have
// already read it in the first line of the input).
// Post-condition: Reads in information from input file
// about numSmoothies number of smoothie orders and dynamically
// allocates an array of doubles of size numIngredients such
// that index i stores the # of pounds of ingredient i
// needed to fulfill all smoothie orders and returns a pointer
// to the array.
double *calculateOrder(int ingredientCount, int numSmoothies, recipe **recipeList) {
    // Allocate space for array of ingredients needed for order
    double *orderList = (double *)calloc(ingredientCount, sizeof(double)); // Initialize to 0
    // Get number of recipes for each store's order
    int numRecipes;
    scanf("%d", &numRecipes);

    // Get recipe number and weight sold for each recipe
    for (int i = 0; i < numRecipes; i++) {
        int recipeNo;
        double weight;
        scanf("%d %lf", &recipeNo, &weight);
        // Check if recipeNo is valid
        if (recipeNo >= 0 && recipeNo < numSmoothies) {
            // Calculate amount of each ingredient needed for each recipe and store in orderList
            recipe *currentRecipe = recipeList[recipeNo];
            for (int j = 0; j < currentRecipe->itemCount; j++) {
                int itemNo = currentRecipe->itemList[j].itemNo;
                int portions = currentRecipe->itemList[j].portions;
                // Calculate amount needed
                double amountNeeded = portions * weight / currentRecipe->totalPortions;
                // Update orderList to store amount needed
                if (itemNo >= 0 && itemNo < ingredientCount) {
                    orderList[itemNo] += amountNeeded;
                } else {
                    printf("Error: itemNo out of bounds\n");
                }
            }
        } else {
            printf("Error: Invalid recipe number %d\n", recipeNo);
        }
    }
    return orderList;
}

// Pre-conditions: ingredientNames store the names of each
// ingredient and orderList stores the amount
// to order for each ingredient, and both arrays
// are of size numIngredients.
// Post-condition: Prints out a list, in ingredient order, of each
// ingredient, a space and the amount of that
// ingredient to order rounded to 6 decimal
// places. One ingredient per line.
void printOrder(char **ingredientNames, double *orderList, int numIngredients) {
    // Print name of ingredient and amount needed if amountNeeded is positive
    for (int i = 0; i < numIngredients; i++) {
        if (orderList[i] > 0) {
            printf("%s %.6f\n", ingredientNames[i], orderList[i]);
        }
    }
    printf("\n"); // Print newline after each store's list
}

// Pre-conditions: ingredientList is an array of char* of size
// numIngredients with each char* dynamically allocated.
// Post-condition: all the memory pointed to by ingredientList is freed.
void freeIngredients(char **ingredientList, int numIngredients) {
    for (int i = 0; i < numIngredients; i++) {
        free(ingredientList[i]);
    }
    free(ingredientList);
}

// Pre-conditions: allRecipes is an array of recipe* of size
// numRecipes with each recipe* dynamically allocated
// to point to a single recipe.
// Post-condition: all the memory pointed to by allRecipes is freed.
void freeRecipes(recipe **allRecipes, int numRecipes) {
    for (int i = 0; i < numRecipes; i++) {
        free(allRecipes[i]->itemList);
        free(allRecipes[i]);
    }
    free(allRecipes);
}
