#include "syscall.h"

void display() {
    PutChar('a');
    PutChar('b');
    PutChar('c');
    PutChar('\n');
    ThreadExit();
}

void dislayFor() {
    int i;
    for(i=0; i < 3; i++) {
        PutChar('a');
    }
    ThreadExit();
}


void display3() {
    PutChar('k');
    PutChar('y');
    PutChar('z');
    PutChar('\n');
    ThreadExit();
}

void display2() {
    PutChar('u');
    ThreadCreate(display,0);
    PutChar('m');
    ThreadCreate(display3,0);
    PutChar('p');
    PutChar('\n');
    ThreadExit();
}



int main() 
{
    ThreadCreate(dislayFor,0);
    ThreadCreate(dislayFor,0);
    ThreadCreate(dislayFor,0);
    // ThreadCreate(display2,0);
    // ThreadCreate(display3,0);
    ThreadExit();
}