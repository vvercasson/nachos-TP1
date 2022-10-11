#include "syscall.h"

void display() {
    int i;
    for (i = 0; i < 5; i++)
    {
        PutChar(i);
    }
}

int main() 
{
    ThreadCreate(display,0);
    return 0;
}