#ifndef _SORTER_H
#define _SORTER

#include <stdlib.h>
#include <stdio.h>


#define HERE {printf("Implement this here\n");}

/*
 * A structure for a "Line" of text, ending in a '\n'.  Unlike a standard C
 * string, this structure has its own length and therefore
 * can support '\0' as a character
 */
typedef struct line_struct {
    unsigned char *data;
    size_t length;
} line;

/*
  The structure we load the file into, which has an array of lines
*/
typedef struct loaded_file_struct {
    // Each line is a string ending in '\n' (with no '\n' if there is no trailing newline).
    line **lines;
    size_t num_lines;
} loaded_file;

loaded_file * load_file(FILE *f);

void free_file(loaded_file *l);

void sort_file(loaded_file *l);

void print_file(loaded_file *l);


#endif
