// NAME : Rushabh Prajapati
// ASSIGNMENT 2
// INSTRUCTOR : Calin Anton
// LAB INSTRUCTOR: Saleh Hanan
// PURPOSE: Header file for myMalloc(), myFree() and outputMemory(), for allocating, freeing and show status of memory

#ifndef MEMMANAGER_H
#define MEMMANAGER_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_TOTAL_MEMORY 65536   // Total Memory Size (64kB)


typedef unsigned short RELP; //Relative pointer 

char * myMemory; //A pointer to a memory section of size MAX_TOTAL_MEMORY
                 //You are to get a valid value for this pointer
                 //You can assign this pointer either statically or dynamically

/*
  Initializes the block of memory with F_size, E_size, F_offset and E_offset
  Parameter: Nothing
  Returns : Nothing 
*/
void initialize_myMemory();

/*
  myMalloc(size_t size)
  Allocates the requested bytes, size, and returns a realtive pointer (RELP) to the 
  beginning  of the allocated memory. 
  If the space cannot be allocated, using the assigning_requested_block function, 
  then 0 is returned.
  We also used isFree() function to check if the block of memory is available from the previous 
  freed chunk, and if we find a free region we use shifing_freed_block to update the f_size and e_size
  for making our new requested block. 
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
initial_FEsizes(RELP initial_ptr, RELP *f_size, RELP *e_size)
--> Function for assining F_SIZE, E_SIZE, for using in each function to reduce code repertion,
Parameters: A relative pointer, and pointers to f_size and e_size for assing values traversing through memory
Return : Nothing
*/
void initial_FEsizes(RELP initial_ptr, RELP *f_size, RELP *e_size);

/*
Finding the nearest Multiple of 4
Parameter: An unsigned short integer
Returns : An unsigned short integer which is a multiple of 4 
*/
unsigned short align4(unsigned short num);

/*
Function for checking if the chunk is_free or not
parameter: A RELP pointer
returns: 1 if free i.e condition is true else 0 if not free
*/
unsigned short isFree(RELP ptr);


/*
assign_requested_block
Assigns a block of memory to the user, as a helper function for myMalloc.
after assigning f_size and e_size, this function updates f_size's and e_size's, every myMalloc call. 
Parameter : a RELP pointer for f_size and e_size, size_t size
returns : nothing
*/
void assign_requested_block(RELP initial_ptr, size_t size);

/*
shifting_freed_block
This function is helpful when we free a big chunk or colease small chunks to form a big chunk
where the f_size and e_size of those individual chunks need to be changed depending on the memory 
allocated
parameter: aa RELP pointer, and bytes which are the total bites of the memory to be allocated.
return : nothing
*/
void shifting_freed_block(RELP initial_ptr, unsigned short num_bytes);


// myFree and all the Free helper functions

/*
is_leftchunk_free(RELP ptr)
Function for checking if the chunk on the left of the currently freed
chunk is free or not, i.e if e_size's last bit = 0, means left chunk is free
and we can colease it with the current chunk to make a bigger chunk of memory
Parameter: a RELP pointer
returns: 0 if False chunk is not free or 1 i.e if TRUE chunk is free
*/
unsigned short is_leftchunk_free(RELP ptr);

/*
is_rightchunk_free(RELP ptr)
Function for checking if the chunk on the right of the currently freed
chunk is free or not, i.e if e_size's last bit = 0, means right chunk is free
and we can colease it with the current chunk to make a bigger chunk of memory
Parameter: a RELP pointer
returns: 0 if False chunk is not free or 1 i.e if TRUE chunk is free

*/
unsigned short is_rightchunk_free(RELP ptr);
/*
merging_left(RELP ptr)
merging the chunk to the left of the currect chunk.
parameter : a RELP ptr
return : initial pointer to the left chunk as both are merged.
*/
RELP merging_left(RELP ptr);

/*
merging_right(RELP ptr)
merging the pointer to the right of the current chunk.
parameter : a RELP ptr
return : nothing
*/
void merging_right(RELP ptr);

/*
  void outputMemory(void);
  Outputs all blocks of Memory and their status 
  PARAMS: none
  RETURN: void
  PRE: myMemory is initialized
  POST: prints on stdout information about all blocks of Memory in the format:
        +---------+-------+------------+-------+
        |  Status | Start | Block Size | End   |
        |         |       |            |       |
        where: 
           Status-is the status of the chunk containing the block
           Start-is the beginning of the block (the beginning of the user data space)
           Block Size-is the size of the block
           End-is the end of the block (the end of the user data space)
*/

void outputMemory(void);
#endif
