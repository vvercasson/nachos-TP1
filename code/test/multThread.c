#include "syscall.h"

/*
    On attend comme sortie sur ce programme abcabcabc
*/

void dislayFor() {
    int i;
    for(i=0; i < 3; i++) {
        PutChar('a' + i);
    }
    ThreadExit();
}

int main() 
{
    ThreadCreate(dislayFor,0);
    ThreadCreate(dislayFor,0);
    ThreadCreate(dislayFor,0);
    ThreadExit();
    return -1;
}