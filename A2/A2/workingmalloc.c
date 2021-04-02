
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define MAX_TOTAL_MEMORY 65536   // Total Memory Size (64kB)
#define align4(x) (((((x) -1) >>2) <<2)+4)

#define SIZE   2
#define OFFSET 2

typedef unsigned short RELP; //Relative pointer

char  *myMemory;

void initialize_myMemory(){

myMemory = sbrk(0);
//assert(myMemory != NULL);

if (sbrk(MAX_TOTAL_MEMORY - 1) == (void*)-1)
  return;

// Use pointer arithmetic to initialize the whole block each and every
//part!
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
RELP total_size, *initial_ptr, *end_ptr;

align_size = align4(size);

// Memory Block + F_size + E_size.
 RELP *f_size   = (RELP *)(myMemory + *initial_ptr);
 RELP *f_offset = (RELP *)(myMemory + *initial_ptr + SIZE);
 RELP *e_size   = (RELP *)(myMemory + *initial_ptr + SIZE + OFFSET + align_size);
 RELP *e_offset = (RELP *)(myMemory + *initial_ptr + SIZE + OFFSET + align_size + SIZE);

 RELP chunk_size = (2 * SIZE) + (2 * OFFSET) + align_size;
 RELP *initial_move = (RELP *)(myMemory + chunk_size); 

// Return Pointer would be F_SIZE + 4
RELP ret_ptr = *initial_ptr + 4;
//printf("Ret: %d\n", ret_ptr);	
   //Initial Pointer = F_SIZE + USER_SIZE
*initial_ptr += chunk_size;
return ret_ptr; // Returning the memory pointer.

}


void initialize_myMemory();
RELP myMalloc(size_t size);

int main(void){
    int P1, P2, P3, P4;
    initialize_myMemory();

    P1 = myMalloc(3);
    P2 = myMalloc(16);
    P3 = myMalloc(20);
    P4 = myMalloc(10);
    printf("%d\n", P1); 
    printf("%d\n", P2);
    printf("%d\n", P4);
    return 0;
}