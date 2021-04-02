/*********************************************************************
Name           : Rushabh Prajapati
Lab Section    : CMPT-201-X02L(1)
Lab Instructor : Hanan Saleh
Instructor     : Calin Anton
Purpose        : Main file containing testing of the helper files.
**********************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "memManager.h"  // Helper file containing functions

// Helper function containing to initialize block of memory
void initial_memory(void);

int main(void) {
/*
I have used a different approach, I din't knew how to get to the block 
of memory so I tried making two pointers in the memory block.
*/
    RELP initial = 0, last = MAX_TOTAL_MEMORY - 4;
    RELP P1, P2, P3;   // Pointer returned as RELP
    initial_memory();  // Initial block of memory containing F_SIZE,E_SIZE

    P1 = myMalloc(3, &initial, &last); // Malloc calls
    P2 = myMalloc(6, &initial, &last);
    P3 = myMalloc(20, &initial, &last);
   
    printf("%d\n", P1);
    printf("%d\n", P2);
    printf("%d\n", P3);
}

void initial_memory(void) {
/*
 My myMemory =  sbrk(0) was giving me warnings while conversion, that's 
 why I went with malloc but I will change to sbrk in final submission
*/
    myMemory = malloc(MAX_TOTAL_MEMORY);
    assert(myMemory != NULL); // Allocation check

    // Initializing the components F_SIZE, F_OFFSET, E_SIZE, E_OFFSET.
    RELP *f_size   = (RELP *)(myMemory); 
    RELP *f_offset = (RELP *)(myMemory + SIZES);
    RELP *e_size   = (RELP *)(myMemory + SIZES + OFFSETS + MAX_TOTAL_MEMORY - 8);
    RELP *e_offset = (RELP *)(myMemory + SIZES + OFFSETS + MAX_TOTAL_MEMORY - 8 + SIZES);
    
    // Unsigned short range(0, 65535) at 65536 the value turns to 0.
    *f_size   = MAX_TOTAL_MEMORY - 1;
    *e_size   = MAX_TOTAL_MEMORY - 1;
    *f_offset = 0;
    *e_offset = 0;
}
