#ifndef MEMMANAGER_H
#define MEMMANAGER_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_TOTAL_MEMORY 65536   // Total Memory Size (64kB)


typedef unsigned short RELP; //Relative pointer 

char * myMemory; //A pointer to a memory section of size MAX_TOTAL_MEMORY
                 //You are to get a valid value for this pointer
                 //You can assign this pointer either statically or dynamically

void initialize_myMemory();

/*
  myMalloc(size_t size)
  Allocates the requested bytes, size, and returns a realtive pointer (RELP) to the 
  beginning  of the allocated memory. 
  If the space cannot be allocated, then 0 is returned.
  PARAMS: size - number of bytes to allocate
  RETURN: a relative pointer (RELP) to a newly allocated memory region of the desired  
          size or 0 if allocation fails
  PRE: 0 < size < MAX_TOTAL_MEMORY - 76
  POST: region is allocated, and valid relative pointer is returned 
        OR region is not allocated, and 0 is returned
*/

RELP  myMalloc(size_t size);

/*
  myFree(RELP region)
  Frees the memory region at the given relative pointer (RELP), region, that was 
  previously allocated using myMalloc. 
  It gives the chunk of memory containing region back to the allocator, and allows for  
  it to be reused in the future.
  PARAMS: region - a relative pointer (RELP) to an allocated memory region 
  RETURN: void
  PRE: 74 <= region < MAX_TOTAL_MEMORY-6
  POST: the chunk containing region is added to the available memory

*/
void myFree(RELP region);

/*
  outputBins()
  Outputs all information regarding the bins of unused chuncks available for allocation.
  PARAMS: none
  RETURN: void
  PRE: myMemory is initialized
  POST: prints on stdout information about each bin in the format:
        +----------+------------+----------+
        | Bin Size | Total Bins | Sum      |
        |          |            |          |
        where: 
            Bin Size-is the size of the bin
            Total Bins-is the number of chuncks in the bin
            Sum-is the sum of the sizes of all chuncks in the bin  
 
*/

unsigned short is_left_free(RELP ptr);
unsigned short is_right_free(RELP ptr);

RELP coalesce_left(RELP ptr);
void coalesce_right(RELP ptr);

void outputMemory(void);
#endif
