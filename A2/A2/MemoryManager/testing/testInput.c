// NAME : Rushabh Prajapati
// ASSIGNMENT 2
// INSTRUCTOR : Calin Anton
// LAB INSTRUCTOR: Saleh Hanan
// PURPOSE: Testing file

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "../memManager.h"

int main(void){
    RELP P1, P2, P3, P4, P5, P6,P7;
    initialize_myMemory();
    P1 = myMalloc(3);
    P2 = myMalloc(6);
    P3 = myMalloc(21);
    P4 = myMalloc(23);
    P5 = myMalloc(15);

    // printf("%u\n", *((RELP*)((char*)myMemory + 0)));
    // printf("%u\n", *((RELP*)((char*)myMemory + 12)));
    // printf("%u\n", *((RELP*)((char*)myMemory + 28)));
    // printf("%u\n", *((RELP*)((char*)myMemory + 56)));

    printf("P1: %u\n", P1);
    printf("P2: %u\n", P2);
    printf("P3: %u\n", P3);
    printf("P4: %u\n", P4);
    printf("P5: %u\n", P5);
    
    outputMemory();
    
    myFree(P1);
    myFree(P2);
    myFree(P3);
    
    outputMemory();
    P6 = myMalloc(7);
    P7 = myMalloc(12); 
    printf("P6: %u\n", P6);
    printf("P7: %u\n", P7);
    outputMemory();   
    return 0;

}