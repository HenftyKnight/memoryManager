#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "memManager.h"


int main(void){
    RELP P1, P2, P3, P4, P5;
    initialize_myMemory();
    P1 = myMalloc(3);
    P2 = myMalloc(6);
    P3 = myMalloc(20);
    P4 = myMalloc(23);
    P5 = myMalloc(15);

    printf("%u\n", P1);
    printf("%u\n", P2);
    printf("%u\n", P3);
    printf("%u\n", P4);
    printf("%u\n", P5);
    myFree(P3);
    myFree(P4);
    output_memory();
    return 0;
}
