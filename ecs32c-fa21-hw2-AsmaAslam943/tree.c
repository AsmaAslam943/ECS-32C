#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

#define HERE fprintf(stderr, "YOU NEED TO IMPLEMENT THIS!\n");

// A useful helper function for contains/find/insert.
// This returns the pointer to the node that matches the
// key or NULL if nothing matches.
tree_node *find_node(tree_node *t, const void *key, int (*comparison_fn)(const void *, const void *))
{
    if (t == NULL) {
        return NULL;
    }
    int cmp = comparison_fn(key, t->key); 

    if (cmp == 0){
        return t; //This means that we have found the node 
    } else if (cmp < 0){
        return find_node(t->left,key,comparison_fn); //Here we initiate the recursive call on find node so that we can go to the left subtree 
    } else {
        return find_node(t->right, key, comparison_fn); //Here, we go to the right subtree 
    }
}

// Allocates a new tree with the specified comparison function.
tree *new_tree(int (*comparison_fn)(const void *, const void *))
{
    tree *t = (tree *)malloc(sizeof(tree)); 
    if (t == NULL) { 
        return NULL; //This means that the memory allocation has failed
    }
    t->root = NULL;
    t-> comparison_fn = comparison_fn; 
    return t;
}

// Frees the the nodes, but does not free the keys
// or data (deliberately so).
void free_node(tree_node *t)
{
    if (t == NULL){ //This simply checks if the node is empty and if so we just press return to exit 
        return; 
    }
    free_node(t->left); //This call returns the left node and the following one returns the right one 
    free_node(t->right); 
    free(t); //This recursive call returns the node itself 
}

// And frees the entire tree and the nodes
// but again, not the data or keys.
void free_tree(tree *t)
{
    if (t==NULL){ //Checking if the tree is null else it will return out of the loop
        return; 
    }
    free_node(t->root); //Here, we call the free node function on the root so we can free the root node 
    free(t); //This frees the tree structure 
}

// Returns true if the key (comparison == 0) is in the tree
bool contains(tree *t, const void *key)
{
    return find_node(t->root, key, t->comparison_fn) != NULL; 
}

// Returns the data or NULL if the data is not in the tree.
void *find(tree *t, const void *key)
{
    tree_node *node = find_node(t->root,key,t->comparison_fn);
    if (node == NULL) { //checking if the node is null 
        return NULL; 
    }
    return node->data; //otherwise we just return the data within node 
}

// Inserts the element into the tree
void insert(tree *t, void *key, void *data)
{
    tree_node *parent = NULL; //initialize the tree's parent node to be null 
    tree_node *current = t -> root; //but we make the current node our "root"
    int cmp; 

    while (current != NULL){ //when current is not null then we initialize cmp to take over comparisons 
        cmp = t->comparison_fn(key,current->key); 

        if (cmp == 0){ //as long as cmp is 0 then we just return the data 
            current->data = data; 
            return; 
        }
        parent = current; //we make the parent node to be the current one 
        if (cmp < 0){ //we set up the tree structure here, if we are less than 0 then current is left node or else right 
            current = current->left; 
        } else {
            current = current->right; 
        }
    }

    tree_node *new_node = (tree_node *)malloc(sizeof(tree_node)); 
    new_node->key = key; 
    new_node->data = data; 
    new_node->left = NULL; 
    new_node->right = NULL; 

    if (parent == NULL){ //here, we check if the parent is null then the root is just the new node 
        t->root = new_node; 
    } else if (cmp<0){ //designing the tree structure once again 
        parent ->left = new_node; 
    } else {
        parent->right = new_node;
    }

}

// This visits every node in an in-order traversal,
// calling f on key, data, context.  Context is
// so that f has an ability to maintain its own state
// between calls.  This is a useful helper function for implemneting
// traverse
void traverse_node(tree_node *t, void (*f)(void *, void *, void *), void *context)
{
    if (t == NULL) { //we exit if the node is null 
        return; 
    }
    traverse_node(t->left,f,context); //otherwise we call on traverse_node to traverse through contents of left 
    f(t->key,t->data,context); 
    traverse_node(t->right,f,context); //or else right 
}

void traverse(tree *t, void (*f)(void *, void *, void *), void *context)
{
    traverse_node(t->root,f,context); //to traverse through the tree then we call back on traverse_node to traverse starting from the root 
}