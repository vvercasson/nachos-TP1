#include "syscall.h"

int main(){
    ForkExec("../test/putstring");
    while(1);
}