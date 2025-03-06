#include "sorter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * the C qsort() utility takes a comparison function that
 * is a little different.  Becaues it wants to work on data that
 * can be of arbitrary sized (so, eg, an array of structures) rather
 * than just void* items, it wants pointers-to-pointers.
 * 
 * Similarly we can't use strcmp in this because we actually need
 * to evaluate strings that can contain null pointers.  But we will
 * just do strict character comparison:  
 * 
 * for i from 0 to length of min(a/b), if a[i] < b[i] then its -1,
 * if a[i] > b[i] its +1, and if a[i] = b[i] we increment i.
 * Then its the shorter string is before, and 0 if the
 * strings are fully equal.
*/
int sorter_comp(const void *a, const void *b){
  const line *str_a = *(const line **)a; // I set str_a and str_b to be lines 
  const line *str_b = *(const line **)b; 
  size_t minimum = (str_a->length < str_b -> length) ? str_a -> length : str_b -> length; //Formed the minimum size_t to get a value of the smaller length of both strings
  for (size_t i = 0; i < minimum ; i++){ //Used a for loop to iterate through 
      if (str_a-> data[i]<str_b-> data[i]) return -1;  //If str_a is smaller than str_b in length then it returns -1 because str_a is supposed to come before str_b  
      if (str_a-> data[i]>str_b-> data[i]) return 1; //If str_a is larger than str_b then it returns 1 because str_b is supposed to come first 
  }
  return (int)(str_a->length - str_b->length); //Returend the difference of lengths of both strings 
}


/* You probably want to use qsort */
void sort_file(loaded_file *f) 
{
  qsort(f->lines, f->num_lines, sizeof(line *),sorter_comp); 
}


loaded_file *load_file(FILE*f)
{ 
  loaded_file*file = malloc(sizeof(loaded_file)); //We allocate the memory within the file here within the sizeof(loaded_file)
    if (file == NULL) return NULL; //We check if teh file is null and return that to the user 

    file -> lines = NULL; 
    file -> num_lines = 0; //We want to set both as empty 
    
    line *current_line = malloc(sizeof(line)); //We allocate the memory once again 
    if (current_line == NULL){//Check if the line is null and if so we have to free the file memory 
      free(file); 
      return NULL; 
    }
    current_line->data = NULL; //Next, we have to set everything to null 
    current_line->length = 0; 
    size_t current_size = 0; 

    int character; //We create an int for character 
    while ((character = fgetc(f))!= EOF){ //We use fgetc function so that we can read the file character by character until the END OF FILE
      unsigned char *t_data = realloc(current_line ->data, current_size +1); //We have to adjust the memory storage here that's why current_size + 1 exists
      if (t_data == NULL){ //Check if t_data is null and if so we have to free memory in current_line and the file 
        free(current_line->data); 
        free(current_line); 
        free_file(file);
        return NULL; 
      }
      current_line->data = t_data; 
      current_line->data[current_size++] = (unsigned char)character; 
      
      if (character== '\n'){ //IF chracter is a new line then we have to set length of current_line to current_size 
        current_line->length = current_size; 
        line **new_lines = realloc(file->lines, (file-> num_lines+1)*sizeof(line *)); //We reallocate the memory once again to adjust with the size 
        if (!new_lines){ //If new lines is null then we have to free memory once again 
          free(current_line->data); 
          free(current_line); 
          free_file(file);
          return NULL; 
        }
        file->lines = new_lines; 
        file->lines[file->num_lines++] = current_line; 
        current_line = malloc(sizeof(line)); 
        if (current_line == NULL){
          free_file(file); 
          return NULL; 
        }
        current_line->data = NULL; //Set all the data and length equal to null 
        current_line->length = 0; 
        current_size = 0; 
      }
 }
  if (current_size > 0){ //Next, we check if current_size > 0 and then adjust the size and memory accordingly 
      current_line->length = current_size; 
      line **new_lines = realloc(file->lines, (file->num_lines+1)* sizeof(line *));
      if (!new_lines){ //If new_lines is null then we have to obviously free the memory once again 
          free(current_line->data); 
          free(current_line); 
          free_file(file); 
          return NULL; 
      }
      file->lines = new_lines; 
      file->lines[file->num_lines++] = current_line; //And we continue adjusting the current_line in case it does not match the conditions 
  } else{
      free(current_line->data); 
      free(current_line); 
  }
    
  return file; 

}


void free_file (loaded_file *f)
{
  if(f) {
    for (size_t i = 0; i <f->num_lines; i++){ //We iterate through i and again using 
      free(f->lines[i]->data); //We free the data within the file->lines[i]
      free(f->lines[i]);  //Again, here we free the lines[i] within the file 
    } //This just frees the lines 
    free(f->lines); 
    free(f); //Lastly, we need to free the file within loaded_file *f
  }
}
/*
 * As a reminder the lines can include nulls, so C printing of string
 * routines can't work.  Instead we need to print the string a character
 * at a time to standard output using putchar()
*/
void print_file(loaded_file *f)
{
  for (size_t i = 0; i <f -> num_lines; i++ ){ //Here we have a for loop that iterates through size of i=0, f->num_lines and continues within the loop 
    for (size_t j =0; j<f->lines[i]->length; j++){ //J is the other iterator for this loop 
      putchar(f->lines[i]->data[j]); //We utilize putchar to print out the characters in file->lines[i] and retrieve data in j 
    }
  }
}
