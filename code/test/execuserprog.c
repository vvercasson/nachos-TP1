#include "syscall.h"
main()
{
    ForkExec("../test/userpages0");
    ForkExec("../test/userpages1");
}