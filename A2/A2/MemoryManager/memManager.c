// NAME : Rushabh Prajapati
// ASSIGNMENT 2
// INSTRUCTOR : Calin Anton
// LAB INSTRUCTOR: Saleh Hanan
// PURPOSE: Functions myMalloc(), myFree() and outputMemory(), for allocating, freeing and show status of memory

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "memManager.h"

#define MAX_TOTAL_MEMORY 65536   // Total Memory Size (64kB)

#define SIZE   2   // F_SIZE AND E_SIZE
#define OFFSET 2   // F_OFFSET AND E_OFFSET

typedef unsigned short RELP;    //Relative pointer

char  *myMemory;    //Initial pointer to the start of memory

void initialize_myMemory(void) {
    myMemory = sbrk(0);   /* Allocate 64kb for heap */
    // assert(myMemory != NULL);            /* if sbrk was successful */
    if (sbrk(MAX_TOTAL_MEMORY) == (void*)-1)
       return;
    /* Initializing Memory as one big chunk containing f_size, e_size, f_offset and e_offset */
    RELP *f_size    = (RELP *)(myMemory); 
    RELP *f_offset  = (RELP *)(myMemory + SIZE);
    RELP *e_size    = (RELP *)(myMemory + SIZE + OFFSET + MAX_TOTAL_MEMORY - 10);
    RELP *e_offset  = (RELP *)(myMemory + SIZE + OFFSET + MAX_TOTAL_MEMORY - 10 + SIZE);
    
    /* Assign initial values to fsize and esize for the chunk */
    *f_size = MAX_TOTAL_MEMORY - 2;
    *e_size = MAX_TOTAL_MEMORY - 2;
    *f_offset = 0;
    *e_offset = 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Assigning the memory block for the requested size */
void assign_requested_block(RELP initial_ptr, size_t size) {
    
    /* f_size & e_size -> pointer to the f_size and e_size of the chunk */
    
    RELP *f_size = (RELP *)((char *)myMemory + initial_ptr);
    RELP *e_size = (RELP *)((char *)myMemory + initial_ptr + size - 4);

    /* Assigning f_size value to size of the chunk, as it is the initial chunk
       and after each malloc call it will be updaated depending on the size  */
    *f_size = size;

    /* Incrementing and making e_size value to size of chunk + 1 
       Which means chunk / memory region is in use                          */
    *e_size = size + 1;

    return;
}

/*******************************************************************************/
/* Shifting the free block of memory */
/* moves bigger chunk of memory */
void shifting_freed_block(RELP initial_ptr, unsigned short num_bytes) {
    /* prevchunk_fsize -> fsize && prevchunk_esize -> esize of the current chunk which will be changed to new chunk */
    RELP *prevchunk_fsize = (RELP *)((char *)myMemory + initial_ptr);
    RELP *prevchunk_esize = (RELP *)((char *)myMemory + initial_ptr + *prevchunk_fsize - 4);

    /* Move the bigger chunk by number of bytes of the chunk */
    /* Set fsize and esize values of the chunk after splitting */
    *(RELP *)((char *)prevchunk_fsize + num_bytes) = *prevchunk_fsize - num_bytes;
    *(prevchunk_esize) = *prevchunk_esize - num_bytes;

    return;
}
/* Initializes the values of f_size and e_size */
void initial_FEsizes(RELP initial_ptr, RELP *f_size, RELP *e_size) {
    /* initial_ptr - pointer at the beginning of the memory */

    /* f_size is at mymemory + the initial pointer i.e the initial pointer */
    RELP *f_size_ptr = (RELP *)((char *)myMemory + initial_ptr);
    *f_size = *f_size_ptr;

    /* e_size = initial pointer to 
       the chunk + size of the chunk - 4 */
    RELP *e_size_ptr = (RELP *)((char *)myMemory + initial_ptr + *f_size - 4);
    *e_size = *e_size_ptr;
    
    return;
}

/* Check if the chunk is free or not. */
unsigned short isFree(RELP ptr) {
    /* Initializing f_size, e_size */
    RELP f_size, e_size;
        
    /* Assign f_size and e_size values of the chunk at the pointer. */
       initial_FEsizes(ptr, &f_size, &e_size);

    /* if e_size is divisible by 2, chunk is free, because it will 
       always be an even number if memory is not assigned */
    if ((e_size % 2) == 0) // (true), if chunk is free
        return 1;
    
    // if it is not free
    return 0;
}
/*
 to get the nearest integer of 4
*/
unsigned short align4(unsigned short num) {
    int i=0;
    for (i = 0; i < 4; i++) {
        if ((num + i) % 4 == 0)     /* If number is divisible by 4 */
            return (num + i);
    }    
    return 0;
}
/*****************************************************************************************/
RELP myMalloc(size_t size) {
    /* f_size && e_size of chunk, fsize_ptr  - initial relative pointer to chunk */
    RELP f_size = 0, e_size = 0;
    RELP fsize_ptr = 0;
    
    // can allocate Max Meomory - 8, f & e_Size and f & e_offsets
    if (size > 65528) {
        printf("Could Not Allocate Memory.\n");
        return 0;
    }

    /* size of chunk to allocate */
    /* it should be nearest multiple of 4 + 8 bytes for fsize and esize */
    size = 8 + align4((unsigned short)size);

    /* While pointer is not at the end of memory */
    /* Iterate while they both are not equal */
    while ((RELP *)(myMemory + fsize_ptr - 4) != (RELP *)((char *)myMemory + MAX_TOTAL_MEMORY)) {
        initial_FEsizes(fsize_ptr, &f_size, &e_size);
        /* If chunk is free and the size of chunk requested is in the chunk */
        if (isFree(fsize_ptr) && ((f_size - size) < 8)) {
            /* return ptr to where memory begins */
            assign_requested_block(fsize_ptr, f_size);
            return (fsize_ptr + 4);
        }

        /* if the chunk is big than requested chunk, so split the chunk */
        /* And then give the relative ptr to the user */
        else if (isFree(fsize_ptr) && (f_size - 4) >= size) {
            shifting_freed_block(fsize_ptr, size);     /* Shifting the big chunk */
            assign_requested_block(fsize_ptr, size);     /* Assign the smaller chunk */
            return (fsize_ptr + 4);                  /* return RELP to beginning of memory */
        }

        fsize_ptr += f_size; // Incrementing pointer to f_size for next chunk
    }

    /*if allocation fails*/
    printf("No Space for Memory Allocation\n");
    return 0;
}
/* ---------------------------------- */
/* if the chunk to the left of the current chunk is free */
unsigned short is_leftchunk_free(RELP ptr) {
    /*Cannot access memory at the beginning*/
    if (ptr == 0)
        return 0;   /*left chunk is not free */

    /* esize of left region will be - 4 bytes*/
    RELP *leftchunk_esize = (RELP *)((char *)myMemory + ptr - 4);
    
    /* if last bit is 0 */
    if (*leftchunk_esize % 2 == 0)
        return 1;   /* free left */

    return 0;       /* left not free */
}

/* ---------------------------------- */
/* Merging with the left chunk */
RELP merging_left(RELP ptr) {
    /* f_size && e_size pointer to the esize of current chunk */
    RELP *f_size = (RELP *)((char *)myMemory + ptr);
    RELP *e_size = (RELP *)((char *)myMemory + ptr + *f_size - 4);
    
    /*
    prevchunk_esize -> pointer to the esize of left chunk
    prevchunk_fsize -> pointer to the fsize of left chunk
    */
    RELP *prevchunk_esize = (RELP *)((char *)myMemory + ptr - 4);
    RELP *prevchunk_fsize = (RELP *)((char *)myMemory + ptr - *prevchunk_esize);

    /* left chunk's fsize incremented to current chunk */
    /* right chunk's esize incremented to left chunk */
    *prevchunk_fsize += *f_size;
    *e_size += *prevchunk_esize;    
    /* Forming a big chunk */
    /* return initial pointer to the left chunk */
    return ptr - *prevchunk_esize;
}

/* if the chunk to the right of current chunk is free */
unsigned short is_rightchunk_free(RELP ptr) {
    /* current chunk f_size */
    RELP *current_fsize = (RELP *)((char *)myMemory + ptr);
    
    /* ptr + *current_fsize = relative pointer to the fsize of the right chunk */
    if ((ptr + *current_fsize) >= MAX_TOTAL_MEMORY)
        return 0;   /*Not free*/
    
    /* If relative pointer to right chunk shows that it's free */
    if (isFree(ptr + *current_fsize))
        return 1;   /* Free chunk */

    return 0;       /* Not free */
}

/* ---------------------------------- */
/* Given Relative pointer to the current chunk, coalesce this chunk with the right chunk */
void merging_right(RELP ptr) {
    /* pointer to the fsize of current chunk */
    RELP *current_fsize = (RELP *)((char *)myMemory + ptr);

    /* fsize and esize of the right chunk */
    RELP *next_fsize = (RELP *)((char *)myMemory + ptr + *current_fsize);
    RELP *next_esize = (RELP *)((char *)myMemory + ptr + *current_fsize + *next_fsize - 4);

    /* esize of the right chunk incremnetd by the current chunk */
    *next_esize += *current_fsize;
    /* fsize of the current chunk incremneted by the next chunk */
    *current_fsize += *next_fsize;

    return;
}

void myFree(RELP region) {
    /* 
     initial_ptr -> memory region - 4 
     left        -> equal to initial_ptr to get to the left chunk
     */
    RELP initial_ptr = region - 4, left = initial_ptr;
    RELP *f_size = (RELP *)((char *)myMemory + initial_ptr);
    RELP *e_size = (RELP *)((char *)myMemory + initial_ptr + *f_size - 4);

    /* Free this chunk*/
    *e_size -= 1; 

    /* Check if the left chunk is free then merge */
    if (is_leftchunk_free(initial_ptr))
       left = merging_left(initial_ptr); 
    
    /* Check if the right chunk is free then merge */
    if (is_rightchunk_free(left))
        merging_right(left);
}
/* Printing the status of the all the allocated, free spaces */

void outputMemory(void){
 /* Initial pointers for traversing through memory */ 
    RELP *f_size; //*e_size;
    RELP fsize_ptr = 0;

    printf("+---------+-------+------------+-------+\n");
    printf("|  Status | Start | Block Size | End   |\n");
    printf("+---------+-------+------------+-------+\n");

    /* Looping till the fsize_ptr --> RELP for traversing is not equal to MAX_TOTAL_MEMORY - 2 */
    while(fsize_ptr != 65534){
        f_size = (RELP*)(myMemory + fsize_ptr);
        // e_size = (RELP*)(myMemory + fsize_ptr + *f_size - 4);

        /* If the chunk free, print Free, else In Use*/
        isFree(fsize_ptr) ? printf(" Free    ") : printf(" In Use  ");

        /* RELP to the start of user data space*/
        printf("|");
        printf(" %5d ", fsize_ptr + 4);

        /* Size of the user data space in bytes*/
        printf("|");
        printf("      %5d ", *f_size - 8);

        
        /* RELP to the end of user data space*/
        printf("|");
        printf(" %5d ", fsize_ptr + *f_size - 4);
        
        printf("|\n");

        // Incrementing the fsize_ptr with f_size to get to next chunk */
        fsize_ptr += *f_size;
    }
    printf("+---------+-------+------------+-------+\n");
}
