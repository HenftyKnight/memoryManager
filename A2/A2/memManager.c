#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "memManager.h"

//#define HEAP_END 65334
//#define MAX_TOTAL_MEMORY 65536   // Total Memory Size (64kB)

#define align4(x) (((((x) -1) >>2) <<2)+4)

#define SIZE   2
#define OFFSET 2

typedef unsigned short RELP; //Relative pointer

char  *myMemory;

void initialize_myMemory(){

myMemory = sbrk(0);

if (sbrk(MAX_TOTAL_MEMORY - 1) == (void*)-1)
  return;

// Use pointer arithmetic to initialize the whole block each and every part!
// Initializing the values where upto + 1 is Fsize and offset and then
// user specified size and Esize and offset,,,,

RELP *F_size     = (RELP*)(myMemory);
RELP *F_offset   = (RELP*)(myMemory + SIZE);
RELP *E_size     = (RELP*)(myMemory + SIZE + OFFSET + MAX_TOTAL_MEMORY - 8);
RELP *E_offset   = (RELP*)(myMemory + SIZE + OFFSET + MAX_TOTAL_MEMORY - 8 + SIZE);

    *F_size   = MAX_TOTAL_MEMORY - 1;
    *E_size   = MAX_TOTAL_MEMORY - 1;
    *F_offset = 0;
    *E_offset = 0;

}

RELP myMalloc(size_t size){

size_t align_size;
RELP *initial_ptr;
align_size = align4(size);

// Memory Block + F_size + E_size.
 RELP *f_size   = (RELP *)(myMemory + *initial_ptr);
 RELP *e_size   = (RELP *)(myMemory + *initial_ptr + SIZE + OFFSET + align_size);
 RELP chunk_size = (2 * SIZE) + (2 * OFFSET) + align_size;

// Return Pointer would be F_SIZE + 4
*f_size = chunk_size;
*e_size = chunk_size + 1;

RELP ret_ptr = *initial_ptr + 4;
//printf("Ret: %d\n", ret_ptr);	
//printf("Chunk size: %u\n", chunk_size);
//Initial Pointer = F_SIZE + USER_SIZE

*initial_ptr += chunk_size;
//*end_ptr = *initial_ptr - 4;

//printf("Initial_ptr : %u\n", *initial_ptr);
//printf("End PTr: %u\n", *end_ptr);

//*e_size = chunk_size + 1;

//printf("E_size: %u\n", *e_size);

return ret_ptr; // Returning the memory pointer.
}

/*
void myFree(RELP ret_ptr)
{

RELP metadata = ret_ptr - 4;
printf("MetaData: %u\n", metadata);
RELP *f_size = (RELP*)((char *)myMemory + metadata);
RELP *e_size = (RELP*)((char *)myMemory + metadata + *f_size - 4);

printf("Before Free F_size: %u\n", *(RELP*) f_size);
printf("Before Free E_size: %u\n", *(RELP*) e_size);

*e_size -= 1;

//printf("F_size: %u\n", *((RELP*)(myMemory + ret_ptr - 4)));
printf("After Free :E_size: %u\n", *(RELP*) e_size);
printf("After Free :F_size: %u\n", *(RELP*) f_size);
}

*/
/* ---------------------------------- */
/* Check if the chunk to the left of the current chunk is free */
unsigned short is_left_free(RELP ptr) {
    /* If the relative pointer is at the beginning of the memory, then program cannot access that memory */
    if (ptr == 0)
        return 0;   /* 0 (false), left chunk is not free */
     
    /* Otherwise, */
    /* esize of left region will be exactly 2 bytes before the ptr to the current region */
    RELP *left_esize = (RELP *)((char *)myMemory + ptr - 4);
    
    /* If left_esize is divisible by 2 (last bit is 0) */
    if (*left_esize % 2 == 0)
        return 1;   /* 1 (true), left_chunk is free */

    return 0;       /* 0 (false), left chunk is not free */
}

/* ---------------------------------- */
/* Given Relative pointer to the current chunk, coalesce this chunk with the left chunk */
RELP coalesce_left(RELP ptr) {
    /*
     * Variables - 
     * 1. f_size -> absolute pointer to the fsize of current chunk
     * 2. e_size -> absolute pointer to the esize of current chunk
     */
    RELP *f_size = (RELP *)((char *)myMemory + ptr);
    RELP *e_size = (RELP *)((char *)myMemory + ptr + *f_size - 4);
    
    /*
     * 3. prev_esize -> absolute pointer to the esize of left chunk
     * 4. prev_fsize -> absolute pointer to the fsize of left chunk
     */
    RELP *prev_esize = (RELP *)((char *)myMemory + ptr - 4);
    RELP *prev_fsize = (RELP *)((char *)myMemory + ptr - *prev_esize);

    /* left  chunk's fsize is increased by size of current chunk */
    /* right chunk's esize is increased by size of left chunk */
    /* This makes the 2 chunks as one whole big chunk now */
    *prev_fsize += *f_size;
    *e_size += *prev_esize;    
    
    /* Now, initial pointer to the current chunk is switched, to initial pointer of the left chunk */
    /* return initial pointer to the left chunk */
    return ptr - *prev_esize;
}
/* To check if the E_SIze pointer is 0 or 1*/
RELP is_free(RELP ptr){
    RELP initial_ptr = ptr - 4;
    RELP *f_size = (RELP *)((char *)myMemory + initial_ptr);
    RELP *e_size = (RELP *)((char *)myMemory + initial_ptr + *f_size - 4);
    
    if (*e_size % 2 == 0)
        return 1; // chunk is free
    return 0; // not free
}

/* ---------------------------------- */
/* Check if the chunk to the right of current chunk is free */
unsigned short is_right_free(RELP ptr) {
    /* fsize of the current chunk */
    RELP *current_fsize = (RELP *)((char *)myMemory + ptr);
    
    /* ptr + *current_fsize = relative pointer to the fsize of the right chunk */
    /* If fsize of the right chunk is at the end of the heap, then program cannot access that memory region  */
    if ((ptr + *current_fsize) >= MAX_TOTAL_MEMORY)
        return 0;   /* 0 (false), right chunk is not free */
    /* If relative pointer to right chunk shows that it's free */ 
    if (is_free(ptr + *current_fsize))
        return 1;    /*0 (true),  right chunk is free */

    return 0;       /* 0 (false), right chunk is not free */
}

/* ---------------------------------- */
/* Given Relative pointer to the current chunk, coalesce this chunk with the right chunk */
void coalesce_right(RELP ptr) {
    /*
     * Variables -
     * 1. current_fsize -> absolute pointer to the fsize of current chunk 
     */
    RELP *current_fsize = (RELP *)((char *)myMemory + ptr);
    /*
     * 2. next_fsize -> fsize of the next chunk
     * 3. next_esize -> esize of the next chunk
     */ 
    RELP *next_fsize = (RELP *)((char *)myMemory + ptr + *current_fsize);
    RELP *next_esize = (RELP *)((char *)myMemory + ptr + *current_fsize + *next_fsize - 4);

    /* esize of the next chunk is increased by the size of the current chunk */
    *next_esize += *current_fsize;
    /* fsize of the current chunk is increased by the size of the next chunk */
    *current_fsize += *next_fsize;

    return;
}

 /* Given Relative pointer to a region of memory, It frees that region.
 */
void myFree(RELP region) {
    /* 
     * Variables -
     * 1. initial_ptr -> region (RELP of user data space) - 2 (subtract 2 to access f_size of the chunk)
     * 2. left        -> equal to initial_ptr
     * 3. *f_size     -> pointer to the fsize of the chunk 
     * 4. *e_size     -> pointer to the esize of the chunk
     */
    RELP initial_ptr = region - 4, left = initial_ptr;
    RELP *f_size = (RELP *)((char *)myMemory + initial_ptr);
    RELP *e_size = (RELP *)((char *)myMemory + initial_ptr + *f_size - 4);
     
     printf("Before Free --> E_size: %u\n", *((RELP *) e_size));
     printf("Before Free --> F_size : %u\n", *((RELP*)f_size));
    /* Free this chunk, by reducing e_size value by 1 */
    *e_size -= 1; 
     
     printf("After Free --> E_size: %u\n", *((RELP *) e_size));
     printf("After Free --> F_size : %u\n", *((RELP*)f_size));

    /* Check if the left chunk is free */
    /* If it is free, coalesce left chunk with current chunk */
    /* Set the initial pointer at beginning of left chunk */
    if (is_left_free(initial_ptr))
       left = coalesce_left(initial_ptr); 
    
    /* Similarly, check if the right chunk is free, and coalesce chunks */
    if (is_right_free(left))
        coalesce_right(left);
}

void output_memory(void){
    RELP *f_size = (RELP*)((char *)myMemory);
    printf("F_Status: %u\n", *((RELP*)f_size));
}
