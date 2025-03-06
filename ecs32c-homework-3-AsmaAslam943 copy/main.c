#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sorter.h"

/* This simple main should work as follows:
   For each filename passed as a command line argument:
   
   1)  Open the file.  If the file fails to open, exit with code 42
   2)  Load the file into a loaded_file structure using load_file
   3)  When the file is loaded, sort the file with sort_file
   4)  When the file is sorted, print the file to standard output with
       print_file
   5)  Free the loaded_file structure with free_file, and close
       the FILE pointer itself

   If there are no files specified it should simply exit.
*/


int main(int argc, char **argv) {

  if (argc < 2){ 
    return 0; 
  }
  for (int i=1; i < argc; i++ ){ //We need a for loop and i++ iterator to iterate within the arguments 
    FILE *f = fopen(argv[i], "rb"); //We use this argument to open the file in a binary mode since one of the test cases is in binary 
    if (f == NULL){
      return 42; 
    }
    loaded_file *file = load_file(f); //We need to load the file again 
    if (!file){ //If the file is empty then we close the file and return a random value to the user 
      fclose(f); 
      return 42; 
    }
    sort_file(file); //We call on this function to sort, print, free and close the file that the user initially inputted 
    print_file(file); 
    free_file(file); 
    fclose(f); 
  }
  return 0; 


}
