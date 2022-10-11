#include "syscall.h"

int main() 
{
    char buffer[32];
    GetString(buffer,32);
    PutString(buffer);
    return 1;
}