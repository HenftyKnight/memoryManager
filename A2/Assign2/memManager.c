/*******************************************************
Name           : Rushabh Prajapati
Lab Section    : CMPT-201-X02L(1)
Lab Instructor : Hanan Saleh
Instructor     : Calin Anton
Purpose        : Memanager file contiaining the memory 
                 allocation and free fucntions.
********************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "memManager.h"

RELP myMalloc(size_t size, RELP *initial_ptr, RELP *last_ptr) {
/*
Returns a RELP pointer, I am using an helper function to get to the 
block of memory.
*/
    if((*initial_ptr - *last_ptr) < size)
	return 0;
    RELP ptr = assign_memory(initial_ptr, last_ptr, align_4(size));
    return ptr;
}

/*
To stay with the same data type I created this function instead of using 
a MACRO
*/
RELP align_4(RELP num) {
    for (int i = 0; i <= 4; i++)
        if ((num + i) % 4 == 0)
            return num + i;
    return 0;
}

/*
Helper function which returns a pointer to the memory block, which 
malloc returns.
*/

RELP assign_memory(RELP *initial_ptr, RELP *end_ptr, RELP memory_size) {

    //Setting the particualar space for each piece in the memory block
    RELP *f_size   = (RELP *)(myMemory + *initial_ptr);
    RELP *f_offset = (RELP *)(myMemory + *initial_ptr + SIZES);
    RELP *e_size   = (RELP *)(myMemory + *initial_ptr + SIZES + OFFSETS + memory_size);
    RELP *e_offset = (RELP *)(myMemory + *initial_ptr + SIZES + OFFSETS + memory_size + SIZES);

    // Making the chuck with F_SIZE + OFFSET + E_SIZE + OFFSET + MEMORY BLOCK 
    RELP chunk_size = (2 * SIZES) + (2 * OFFSETS) + memory_size;

    // Moving to the block of memory
    RELP *initial_move = (RELP *)(myMemory + *initial_ptr + chunk_size);
    RELP *end_move     = (RELP *)(myMemory + *end_ptr);
    
    // Moving the initial_move pointer to the memory location  
    *initial_move = *f_size - chunk_size;
    *end_move     = *f_size - chunk_size;
    
    // Changing the F_SIZE equal to memory chunk, then setting the offset and E_SIZE
    *f_size   = chunk_size;
    *f_offset = chunk_size - 2;
    *e_size   = chunk_size + 1;

    // Empty check
    if (*initial_ptr == 0)
        *e_offset = 0;
    else
        *e_offset = chunk_size - 2;
    
    // Return Pointer would be F_SIZE + 4
    RELP ret_ptr = *initial_ptr + 4;	
       //Initial Pointer = F_SIZE + USER_SIZE
    *initial_ptr += chunk_size;

    return ret_ptr; // Returning the memory pointer.
}

