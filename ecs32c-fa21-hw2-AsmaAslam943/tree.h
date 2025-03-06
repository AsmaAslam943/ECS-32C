#ifndef _TREE_H
#define _TREE_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct tree_node
{
    void *key;
    void *data;
    struct tree_node *left;
    struct tree_node *right;
} tree_node;

typedef struct tree {
    struct tree_node *root;
    int (*comparison_fn)(const void*, const void*);
} tree;

// Allocates a new tree with the specified comparison function.
tree * new_tree(int (*comparison_fn)(const void*, const void*));

// Frees the tree and all its nodes, but does not free the keys 
// or data.
void free_tree(tree *t);

// Returns true if the key (comparison == 0) is in the tree
bool contains(tree *t, const void *key);

// Returns the data or NULL if the data is not in the tree.
void *find(tree *t, const void *key);

// Inserts the element into the tree
void insert(tree *t, void *key, void *data);

// This visits every node in an in-order traversal,
// calling f on key, data, context.  Context is
// so that f has an ability to maintain its own state
// between calls.
void traverse(tree *t, void (*f)(void *, void *, void *), void *context);





#endif