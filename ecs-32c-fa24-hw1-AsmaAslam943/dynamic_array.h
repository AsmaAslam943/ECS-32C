#ifndef _DYNAMIC_ARRAY_H
#define _DYNAMIC_ARRAY_H

#include <stdbool.h>

// The structure definition for a 
// dynamically sized array.  

// Capacity is the number of elements this array can
// potentially hold before being resized, while
// elements is the number of elements that are currently
// in use in the array
typedef struct {
  unsigned int capacity;
  unsigned int elements;
  int *internal_array;
} DynamicIntArray;


// This should return an array with 0 entries, capable
// of holding only 1 entry
DynamicIntArray *allocate_int_array();

// This should return -1 (NOT ZERO) if the index
// is out of range
int get_int(unsigned int index, DynamicIntArray *data);

// This should return NULL if the index is out of range,
// Or a pointer to the element in question if it is in range.
// IMPORTANT NOTE: This pointer becomes invalid (accessing it
// is undefined behavior) if append is called on this dynamic
// array.
int *set_int(unsigned int index, DynamicIntArray *data);

// This should free all the memory in the dynamic array
void deallocate_int_array(DynamicIntArray *data);

// This should add on an element at the end, resizing the
// array if necessary.  Important note: This should be
// amortized constant time: doubling the size of the internal
// data storage as necessary.
void append(int element, DynamicIntArray *data);


bool present(unsigned int index, DynamicIntArray *data);

#endif
