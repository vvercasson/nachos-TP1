#include "userthread.h"
#include "stdint.h"
#include "system.h"

int tab[2];

static void StartUserThread(void *schmurtz) {
    DEBUG('s',"Entering StartUserThread\n");
    int* tab = (int*) schmurtz;
}

int do_ThreadCreate(int f, int arg) {
    DEBUG ('s', "Creating a Thread\n");
    Thread *t = new Thread ("New Thread");

    tab[0] = f; // TODO: Trouver une meilleure solution car plusieurs Thread = non fonctionnel
    tab[1] = arg;

    t->Start(StartUserThread,tab);
    currentThread->Yield();

    return 0;
}