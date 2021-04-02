#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define HEAP_END 65336
#define MAX_TOTAL_MEMORY 65536   // Total Memory Size (64kB)
#define align4(x) (((((x) -1) >>2) <<2)+4)

#define SIZE   2
#define OFFSET 2

typedef unsigned short RELP; //Relative pointer

char  *myMemory;

void initialMemory(void) {
    myMemory = sbrk(MAX_TOTAL_MEMORY); /* Allocate 64kb for heap */
    assert(myMemory != NULL);            /* Check if malloc was successful */

    /* Set the initial memory as one big chunk */
    RELP *f_size = (RELP *)(myMemory);
    RELP *f_offset = (RELP *)(myMemory + SIZE);
    RELP *e_size = (RELP *)(myMemory + SIZE + OFFSET + MAX_TOTAL_MEMORY - 10);
    RELP *e_offset = (RELP *)(myMemory + SIZE + OFFSET + MAX_TOTAL_MEMORY - 10 + SIZE);
    
    /* Assign initial values to fsize and esize for the chunk */
    *f_size = MAX_TOTAL_MEMORY - 2;
    *e_size = MAX_TOTAL_MEMORY - 2;
    *f_offset = 0;
    *e_offset = 0;
}
/* ---------------------------------- */
/* Assign a block of memory to user */
void assign_block(RELP initial_ptr, size_t size) {
    /*
     * Variables -
     * 1. f_size -> pointer to the f_size of the chunk
     * 2. e_size -> pointer to the e_size of the chunk
     */
    RELP *f_size = (RELP *)((char *)myMemory + initial_ptr);
    RELP *e_size = (RELP *)((char *)myMemory + initial_ptr + size - 4);

    /* Assign f_size value to size of the chunk */
    *f_size = size;
    /* Assign e_size value to size of chunk + 1 (to show that it is in use) */
    *e_size = size + 1;
    printf("E_SIZE: %u\n", *(RELP*)e_size);
    return;
}

/* ---------------------------------- */
/* Shift free block of memory */
/* Useful in cases, where program needs to split a bigger chunk of memory */
/* shift_free_block moves bigger chunk of memory */
void shift_free_block(RELP initial_ptr, unsigned short num_bytes) {
    /*
     * Variables -
     * 1. prev_fsize -> fsize of the current chunk which will be changed to new chunk
     * 2. prev_esize -> esize of the current chunk which will be changed to new chunk
     */
    RELP *prev_fsize = (RELP *)((char *)myMemory + initial_ptr);
    RELP *prev_esize = (RELP *)((char *)myMemory + initial_ptr + *prev_fsize - 4);

    /* Move the bigger chunk by "num_bytes" bytes */
    /* Set fsize and esize values of the split chunk */
    *(RELP *)((char *)prev_fsize + num_bytes) = *prev_fsize - num_bytes;
    *(prev_esize) = *prev_esize - num_bytes;

    return;
}
void assign_fe_sizes(RELP initial_ptr, RELP *f_size, RELP *e_size) {
    /* initial_ptr - Relative Pointer where the chunk of memory begins */
    
    /* f_size is at the initial pointer */
    RELP *f_size_value = (RELP *)((char *)myMemory + initial_ptr);
    *f_size = *f_size_value;
    
    //RELP *f_offset_value = (RELP *)((char *)myMemory + initial_ptr + SIZE);
    //*f_offset = *f_offset_value;

    /* e_size = initial pointer to the chunk + size of the chunk - 2 (value is stored 2 bytes before the end of the chunk) */
    RELP *e_size_value = (RELP *)((char *)myMemory + initial_ptr + *f_size - 4);
    *e_size = *e_size_value;

    // RELP *e_offset_value = (RELP *)((char *)myMemory + initial_ptr + SIZE + OFFSET);
    //*e_offset = *e_offset_value;

    return;
}

/* ---------------------------------- */
/*
 * Check if the chunk is free or not.
 */
unsigned short is_free(RELP ptr) {
    /* Initialize f_size, e_size */
    RELP f_size, e_size, f_offset, e_offset;        
    /* Assign f_size and e_size values of the chunk at the pointer. */
    assign_fe_sizes(ptr, &f_size, &e_size);

    /* if e_size is divisible by 2, chunk is free. */
    if ((e_size % 2) == 0) // return 1 (true), if chunk is free
        return 1;
    
    // 0 (false), if it is not free
    return 0;
}

unsigned short find_nearest_4(unsigned short num) {
    int i=0;
    for (i = 0; i < 4; i++) {
        if ((num + i) % 4 == 0)     /* If number is divisible by 4 */
            return (num + i);       /* return number */
    }    
    return 0;
}
/* ---------------------------------- */
RELP myMalloc(size_t size) {
    /*
     * Variables -
     * 1. f_size - fsize of chunk
     * 2. e_size - esize of chunk
     * 3. f_ptr  - initial relative pointer to chunk
     */
    RELP f_size = 0, e_size = 0, f_offset = 0, e_offset = 0;
    RELP f_ptr = 0;
    
    /* Maximum memory assignable is 65534 bytes, because of design choice */
    /* 65528 is the highest multiple of 4 before 65534, which can be assigned a block */
    /* 2 + 65528 + 2 = 65532 */
    /* After 65528, e.g 65529 */
    /* 2 + 65532 (round to multiple of 4) + 2 = 65536 */
    /* Program doesnot have enough memory. */
    if (size > 65528) {
        printf("Could Not Allocate Memory.\n");
        return 0;
    }

    /* size of chunk to allocate */
    /* it should be nearest multiple of 4 + 4 extra bytes for fsize and esize of the chunk */
    size = 8 + find_nearest_4((unsigned short)size);

    /* While pointer is not at the end of memory */
    /* Relative Pointer at the end -> f_ptr - 2 = 65534 */
    /* Relative Pointer            -> HEAP_END  = 65534 */
    /* Iterate while they both are not equal */
    while ((RELP *)(myMemory + f_ptr - 4) != (RELP *)((char *)myMemory + HEAP_END)) {
        
        assign_fe_sizes(f_ptr, &f_size, &e_size);

        /* If chunk is free and the size of chunk requested is within 8 (4 + 4 extra for metadata) bytes of the chunk */
        if (is_free(f_ptr) && ((f_size - size) < 8)) {
            /* return ptr to user dataspace */
            assign_block(f_ptr, f_size);
            return (f_ptr + 4);
        }

        /* Else, the chunk is much bigger than requested chunk, so split the chunk */
        /* And then give the relative ptr to the user */
        else if (is_free(f_ptr) && (f_size - 4) >= size) {
            shift_free_block(f_ptr, size);      /* Shift the bigger chunk */
            assign_block(f_ptr, size);          /* Assign the smaller chunk */
            return (f_ptr + 4);                 /* return RELP to the user */
        }
             
        /* Increment the Relative pointer to the next chunk */
        f_ptr += f_size;
    }

    /* If the function did not found a chunk then shows the warning */
    printf("Could Not Allocate Memory.\n");
    return 0;
}
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

/* ---------------------------------- */
/* Check if the chunk to the right of current chunk is free */
unsigned short is_right_free(RELP ptr) {
    /* fsize of the current chunk */
    RELP *current_fsize = (RELP *)((char *)myMemory + ptr);
    
    /* ptr + *current_fsize = relative pointer to the fsize of the right chunk */
    /* If fsize of the right chunk is the end of the heap, then program cannot access that memory region  */
    if ((ptr + *current_fsize) >= HEAP_END)
        return 0;   /* 0 (false), right chunk is not free */
    
    /* If relative pointer to right chunk shows that it's free */
    if (is_free(ptr + *current_fsize))
        return 1;   /* 0 (true),  right chunk is free */

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

    /* Free this chunk, by reducing e_size value by 1 */
    *e_size -= 1; 

    printf("E_SIZE: %u\n", *(RELP*)e_size);
    /* Check if the left chunk is free */
    /* If it is free, coalesce left chunk with current chunk */
    /* Set the initial pointer at beginning of left chunk */
    if (is_left_free(initial_ptr))
       left = coalesce_left(initial_ptr); 
    
    /* Similarly, check if the right chunk is free, and coalesce chunks */
    if (is_right_free(left))
        coalesce_right(left);
}

/*
void outputMemory(void) {
    /*
     * Variables -
     * 1. f_size -> fsize of chunk
     * 2. e_size -> esize of chunk
     * 3. f_ptr  -> Relative pointer to beginning of the chunk
    RELP f_size = 0, e_size = 0, f_offset = 0, e_offset = 0;
    RELP f_ptr = 0;
    
    printf("+---------+-------+------------+-------+\n");
    printf("|  Status | Start | Block Size | End   |\n");
    printf("+---------+-------+------------+-------+\n");
    
    /* While pointer is not at the end of memory */
    /* Relative Pointer at the end -> f_ptr - 2 = 65534 */
    /* Relative Pointer            -> HEAP_END  = 65534 */
    /* Iterate while they both are not equal

    RELP *mymem = (RELP*)((char*)myMemory + f_size);
    printf("Memory: %u\n",*((RELP*)mymem));
        /* Assign fsize and esize values to the chunk  
        assign_fe_sizes(f_ptr, &f_size, &e_size);
        printf("|");
        /* If the chunk is free, print Free, else In Use
        is_free(f_ptr) ? printf(" Free    ") : printf(" In Use  ");

        /* RELP to the start of user data space
        printf("|");
        printf(" %5d ", f_ptr + 4);

        /* Size of the user data space in bytes
        printf("|");
        printf("      %5d ", f_size - 8);

        
        /* RELP to the end of user data space
        printf("|");
        printf(" %5d ", f_ptr + f_size - 4);
        
        printf("|\n");

        f_ptr += f_size;
    printf("+---------+-------+------------+-------+\n");
}
*/

void outputMemory(void){
    RELP *f_size, *e_size;
    RELP f_ptr = 0;

    printf("+---------+-------+------------+-------+\n");
    printf("|  Status | Start | Block Size | End   |\n");
    printf("+---------+-------+------------+-------+\n");

    while(f_ptr != 65534){
        f_size = (RELP*)(myMemory + f_ptr);
        e_size = (RELP*)(myMemory + f_ptr + *f_size - 4);
        /* If the chunk is free, print Free, else In Use*/
        is_free(f_ptr) ? printf(" Free    ") : printf(" In Use  ");

        /* RELP to the start of user data space*/
        printf("|");
        printf(" %5d ", f_ptr + 4);

        /* Size of the user data space in bytes*/
        printf("|");
        printf("      %5d ", *f_size - 8);

        
        /* RELP to the end of user data space*/
        printf("|");
        printf(" %5d ", f_ptr + *f_size - 4);
        
        printf("|\n");

        // printf("%d: %d:\n", f_ptr, *((RELP*)f_size));
        // printf("%d\n", *((RELP*)e_size));
        f_ptr += *f_size;
    }
    printf("+---------+-------+------------+-------+\n");
}



int main(void){
    RELP P1, P2, P3;
    initialMemory();
    P3 = myMalloc(65536);
    // printf("%u\n", *((RELP*)((char*)myMemory + 0)));
    // printf("%u\n", *((RELP*)((char*)myMemory + 12)));
    // printf("%u\n", *((RELP*)((char*)myMemory + 28)));
    // printf("%u\n", *((RELP*)((char*)myMemory + 56)));

    // printf("P1: %u\n", P1);
    // printf("P2: %u\n", P2);
    printf("P3: %u\n", P3);
    //printf("P4: %u\n", P4);
    //printf("P5: %u\n", P5);
    // outputMemory();
    // myFree(P1);
    // myFree(P2);
    outputMemory();
    return 0;

}