#include "userthread.h"
#include "stdint.h"
#include "system.h"

// int tab[2];

// #define THREAD_START_ADDRESS 0x180

static void StartUserThread(void *schmurtz) {
    DEBUG('s',"Entering StartUserThread\n");
    int* t = (int*) schmurtz;
    // DEBUG('s', "valeur tab 1 : %d\nvaleur tab 2 : %d\n",tab[0],tab[1]);

    // INIT REGISTERS COPY
    int i;

    for (i = 0; i < NumTotalRegs; i++)
        machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, t[0]); // DOUTE
    DEBUG ('s', "Initializing PCReg register to 0x%x\n",
    t[0]);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    int valeurStackReg = currentThread->space->AllocateUserStack();

    machine->WriteRegister(StackReg, valeurStackReg);
    // END OF COPY

    machine->WriteRegister(4, t[1]);
    free(schmurtz);
    DEBUG ('s', "Initializing stack register to 0x%x\n",
           valeurStackReg);
    machine->DumpMem("threads.svg");
    machine->Run();
}

int do_ThreadCreate(int f, int arg) {
    DEBUG ('s', "Creating a Thread\n");
    Thread *t = new Thread ("New_Thread");

    // tab[0] = f; // TODO: Trouver une meilleure solution car plusieurs Thread = non fonctionnel
    // tab[1] = arg;
    int *tab;
    tab = (int*) malloc(2 * sizeof(int*));
    tab[0] = f;
    tab[1] = arg;

    t->space = currentThread->space;
    currentThread->space->addThread();

    t->Start(StartUserThread,tab);
    // currentThread->Yield();

    return 0;
}