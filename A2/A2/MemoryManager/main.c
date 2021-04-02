// NAME : Rushabh Prajapati
// ASSIGNMENT 2
// INSTRUCTOR : Calin Anton
// LAB INSTRUCTOR: Saleh Hanan
// PURPOSE: Main file for several calls of myMalloc(), myfree() and outputMemory()

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "memManager.h"

int main(void){
    RELP P1, P2, P3;
    initialize_myMemory();
    P1 = myMalloc(3);
    P2 = myMalloc(6);
    P3 = myMalloc(21);

    // printf("%u\n", *((RELP*)((char*)myMemory + 0)));
    // printf("%u\n", *((RELP*)((char*)myMemory + 12)));
    // printf("%u\n", *((RELP*)((char*)myMemory + 28)));
    // printf("%u\n", *((RELP*)((char*)myMemory + 56)));

    printf("P1: %u\n", P1);
    printf("P2: %u\n", P2);
    printf("P3: %u\n", P3);
    //printf("P4: %u\n", P4);
    //printf("P5: %u\n", P5);
    outputMemory();
    myFree(P1);
    myFree(P2);
    myFree(P3);
    outputMemory();
    return 0;

}