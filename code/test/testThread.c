#include "syscall.h"

void display() {
    PutChar('a');
    PutChar('b');
    PutChar('c');
    PutChar('\n');
    ThreadExit();
}

int main() 
{
    ThreadCreate(display,0);
    return 0;
}