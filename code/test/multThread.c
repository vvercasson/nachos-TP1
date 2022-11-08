#include "syscall.h"

void display() {
    PutChar('a');
    PutChar('b');
    PutChar('c');
    PutChar('\n');
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
    ThreadCreate(display2,0);
    PutChar('h');
    PutChar('j');
    PutChar('\n');
    ThreadExit();
}