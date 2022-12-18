#include "copyright.h"
#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "synch.h"
#include "consoledriver.h"
#include "syscall.h"

static void StartUserProg(){
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();

    machine->Run();
}

int
do_ForkExec(const char* filename){
    DEBUG('s',"executed file --> %s\n",filename);
    OpenFile *executable = fileSystem->Open(filename);
    if (executable == NULL)
      {
          SetColor (stdout, ColorRed);
          SetBold (stdout);
          printf ("Unable to open file %s\n", filename);
          ClearColor (stdout);
          return -1;
      }
    AddrSpace *space;

    // definir space
    try
    {
        space = new AddrSpace (executable);
    }
    catch(NotEnoughAvailablePagesException &e)
    {
        return -1;
    }
    delete executable;
    // lancer une nouvelle fonction StartUserProg
    Thread* monThread = new Thread("ForkExecThread");
    monThread->space = space;
    monThread->Start((VoidFunctionPtr)StartUserProg,nullptr);
    return 0;
    // éventuellement augmenter NumPhysPages
}