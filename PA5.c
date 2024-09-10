/* COP 3502C Assignment 5
This program is written by: Tyler Carpenter */

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 25

typedef struct tree_node{
  int points;
  char name[MAX_LENGTH];
  int depth;
  struct tree_node *left;
  struct tree_node *right;
}tree_node;

//function prototypes
tree_node* create_node(char *name, int points);
void free_tree(tree_node *root);
tree_node* insert(tree_node *root, tree_node *temp, int depth);
int height(tree_node *root);
tree_node* search(tree_node *root, char target[]);
tree_node* sub(tree_node *node, int points);
tree_node* delete_node(tree_node *root, tree_node *delnode);
//delete_node auxilliary functions
tree_node* parent(struct tree_node *root, struct tree_node *node);
int isLeaf(struct tree_node *node);
int hasOnlyLeftChild(struct tree_node *node);
int hasOnlyRightChild(struct tree_node *node);
tree_node* maxVal(struct tree_node *root);
void update_depths(tree_node* root, int depth);
tree_node* delete_and_update(tree_node* root, tree_node* delnode);



int main(void){
  //Declare variables
  int num_commands, points, i;
  char name[MAX_LENGTH], command[MAX_LENGTH];
  tree_node *root = NULL, *temp_node;

  //read commands
  scanf("%d", &num_commands);
  for(i = 0; i < num_commands; i++){
    scanf("%s", command);

    //add command
    if(strcmp(command, "add") == 0){
      scanf("%s %d", name, &points);
      temp_node = create_node(name, points);
      root = insert(root, temp_node, 0);
    }
    //sub command
    if(strcmp(command, "sub") == 0){
      scanf("%s %d", name, &points);
      sub(search(root, name), points);
    }
    //delete command
    if (strcmp(command, "del") == 0) {
      scanf(" %s", name);
      temp_node = search(root, name);
      //print deletion results
      if(temp_node == NULL){
        printf("%s not found\n", name);
      }
      else{
        printf("%s deleted\n", name);
      }
      root = delete_and_update(root, temp_node);
    }
    //search command
    if (strcmp(command, "search") == 0) {
      scanf(" %s", name);
      temp_node = search(root, name);
      if(temp_node == NULL){
        printf("%s not found\n", name);
      } else {
        printf("%s %d %d\n", temp_node->name, temp_node->points, temp_node->depth);
      }
    }
    //height balance command
    if(strcmp(command, "height_balance") == 0){
      int left_height = height(root->left);
      int right_height = height(root->right);
      //check and display balance
      if(left_height == right_height){
        printf("left height = %d right height = %d balanced\n", left_height, right_height);
      }
      else{
        printf("left height = %d right height = %d not balanced\n", left_height, right_height);
      }
    }
  }
  //free memory
  free_tree(root);
}

//this function creates a new node with name and points as input
tree_node* create_node(char *name, int points){
  //allocate space for node and fill data
  tree_node* temp = (tree_node*)malloc(sizeof(tree_node));
  strcpy(temp->name, name);
  temp->points = points;
  temp->left = NULL;
  temp->right = NULL;
  temp->depth = -1; //initial depth value

  return temp;
}

//this function frees all of the memory allocated by the tree
void free_tree(tree_node *root){
  if(root ==  NULL){
    return;
  }
  free_tree(root->left);
  free_tree(root->right);
  free(root);
}

//this function takes a node and inserts it in the tree rooted at root in ALPHABETICAL order
struct tree_node* insert(struct tree_node *root, struct tree_node *temp, int depth) {
    //inserting into empty tree
    if (root == NULL) {
      temp->depth = depth;
      printf("%s %d %d\n", temp->name, temp->points, temp->depth);
      return temp;
    } 
    //check if the node with the same name already exists
    if (strcmp(root->name, temp->name) == 0) {
        // Update points of the existing node
        root->points += temp->points;
        printf("%s %d %d\n", root->name, root->points, root->depth);
        free(temp);
        return root;
    } 

    //compare names to decide whether to insert the new node to the left or right
    if (strcmp(temp->name, root->name) > 0) {
        //temp should be inserted to the right if its name is greater than root's name
        root->right = insert(root->right, temp, depth + 1);
    } else {
        //temp should be inserted to the left if its name is less than or equal to root's name
        root->left = insert(root->left, temp, depth + 1);
    }

    return root;
}

//this function calculates the height of a subtree
int height(tree_node *root) {
  if (root == NULL) {
    return -1; //height of an empty subtree is -1
  }
  //recursively calculate the height of left and right subtrees
  int left_height = height(root->left);
  int right_height = height(root->right);

  //return the greater height between left and right subtrees plus one for the current node
  if (left_height > right_height) {
    return left_height + 1;
  } 
  else {
    return right_height + 1;
  }
}

//this function searches for a target name in root and returns a pointer to the node if found
tree_node* search(tree_node *root, char target[]){
  //check if there are nodes in tree
  if(root != NULL){
    //compare target to current node
    int cmp = strcmp(root->name, target);

    //target is found at root
    if(cmp == 0){
      return root;
    }
    //search to left
    if(cmp > 0){
      return search(root->left, target);
    }
    //search to right
    else{
      return search(root->right, target);
    }
  }
  else{
    return NULL; //node not found
  }
}

//this function subtracts points from a node
tree_node* sub(tree_node *node, int points){
  //if fewer points than being sub, set to 0
  if(node->points < points){
    node->points = 0;
  }
  else{
    //subtract points
    node->points -= points;
  }
  printf("%s %d %d\n", node->name, node->points, node->depth);
  return node;
}

//this function deletes a node from the tree and returns a pointer to the root of the resulting tree.
tree_node* delete_node(tree_node* root, tree_node* delnode) {
    struct tree_node *new_del_node, *save_node, *par;
    char save_name[MAX_LENGTH];
    int save_points;

    //node not found
    if (delnode == NULL) {
        return root; 
    }

    // if deleting root node
    if (root == delnode) {
        par = NULL;
    } else {
        par = parent(root, delnode);
    }

    // case 1: leaf Node
    if (isLeaf(delnode)) {
        // deleting the only node in the tree
        if (par == NULL) {
            free(root); // free the memory for the node
            return NULL;
        }
        // deletes the node if it's a left child
        if (strcmp(delnode->name, par->name) < 0) {
            par->left = NULL;
        } else {
            // deletes the node if it's a right child
            par->right = NULL;
        }
        free(delnode);
        return root; // return the root of the new tree
    }

    // case 2: one child
    if (hasOnlyLeftChild(delnode)) {
        // deleting the root node of the tree
        if (par == NULL) {
            save_node = delnode->left;
            free(delnode);
            return save_node; // return the new root node of the resulting tree
        }
        // deletes the node if it's a left child
        if (strcmp(delnode->name, par->name) < 0) {
            par->left = delnode->left;
        } else {
            // Deletes the node if it's a right child
            par->right = delnode->left;
        }
        free(delnode);
        return root;
    }

    // only has a right child
    if (hasOnlyRightChild(delnode)) {
        // Deleting the root node of the tree
        if (par == NULL) {
            save_node = delnode->right;
            free(delnode);
            return save_node;
        }
        // deletes the node if it's a left child
        if (strcmp(delnode->name, par->name) < 0) {
            par->left = delnode->right;
        } else {
            //deletes the node if it's a right child
            par->right = delnode->right;
        }
        free(delnode);
        return root;
    }

    // case 3: node has two children
    // Find new node to replace the deleted node
    new_del_node = maxVal(delnode->left);
    new_del_node->depth = delnode->depth;
    strcpy(save_name, new_del_node->name);
    save_points = new_del_node->points;

    // delete the replacement node
    root = delete_node(root, new_del_node);

    // replace the data of the node to be deleted with the new node
    strcpy(delnode->name, save_name);
    delnode->points = save_points;

    return root;
}

//returns the parent of the node pointed to by node in the tree rooted at root.
struct tree_node* parent(struct tree_node *root, struct tree_node *node) {
  //take care of NULL cases.
  if (root == NULL || root == node) {
    return NULL;
  }
  //the root is the direct parent of the node if it matches either left or right child
  if (root->left == node || root->right == node) {
    return root;
  }
  //compare the names to decide whether to search the left or right subtree
  int cmp = strcmp(node->name, root->name);

  //look for node's parent in the left side of the tree if the node's name is less than root's name
  if (cmp < 0) {
    return parent(root->left, node);
  }
  //look for node's parent in the right side of the tree if the node's name is greater than root's name
  else {
    return parent(root->right, node);
  }
}

//returns 1 if node is a leaf node, 0 otherwise
int isLeaf(struct tree_node *node) {
  return (node->left == NULL && node->right == NULL);
}

//returns 1 if node has a left child and no right child
int hasOnlyLeftChild(struct tree_node *node) {
  return (node->left!= NULL && node->right == NULL);
}

// returns 1 iff node has a right child and no left child
int hasOnlyRightChild(struct tree_node *node) {
  return (node->left== NULL && node->right != NULL);
}

//returns a pointer to the node storing the maximum value in the tree
struct tree_node* maxVal(struct tree_node *root) {
  //root stores the maximal value.
  if (root->right == NULL)
    return root;
  //the right subtree of the root stores the maximal value.
  else
    return maxVal(root->right);
}

void update_depths(tree_node* root, int depth) {
    if (root == NULL) return;

    //update the depth of the current node
    root->depth = depth;

    //left and right subtrees
    update_depths(root->left, depth + 1);
    update_depths(root->right, depth + 1);
}

tree_node* delete_and_update(tree_node* root, tree_node* delnode) {
  // delete the node
  root = delete_node(root, delnode);
  // update depths after deletion
  update_depths(root, 0);
  
  return root;
}