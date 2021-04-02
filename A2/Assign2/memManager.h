/***************************************************************
Name           : Rushabh Prajapati
Lab Section    : CMPT-201-X02L(1)
Lab Instructor : Hanan Saleh
Instructor     : Calin Anton
Purpose        : memManager.h containing the function documentation.
****************************************************************/
#include <stdio.h>
#include <stdlib.h>

#define MAX_TOTAL_MEMORY 65536   // Total Memory Size (64kB)

#define SIZES   2
#define OFFSETS 2

typedef unsigned short RELP; //Relative pointer 

char * myMemory; //A pointer to a memory section of size MAX_TOTAL_MEMORY
                 //You are to get a valid value for this pointer
                 //You can assign this pointer either statically or dynamically

/*

RELP align_4(RELP num)
Changes the size requested to nearest nultiple of 4.
Parameter:  RELP num
returns a RELP number which is the nearest multiple of 4

*/

RELP align_4(RELP num);

/*

Parameter: an Initial pointer pointing to the start of the memory, 
and end pointer just for end of the block and the block_size
Return   : a RELP pointer to the memory
Moves though the whole initialized chunk and allocates memory for 
the block by adding F_SIZE + E_SIZE and returns the pointer of the memory.
 
*/
RELP assign_memory(RELP *initial_ptr, RELP *end_ptr, RELP memory_size);

/*
  myMalloc(size_t size)
  Allocates the requested bytes, size, and returns a realtive pointer (RELP) to the 
  beginning  of the allocated memory. 
  Region is not allocated, and 0 is returned
*/
RELP  myMalloc(size_t size, RELP *initial, RELP *end);

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
void outputBins(void);

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
