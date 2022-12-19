#include "system.h"
#include "machine.h"
#include "synch.h"

PageProvider::PageProvider() {
    b = new BitMap(NumPhysPages);
    mutex = new Semaphore("mutex",1);
    reservedPages = 0;
}

unsigned int
PageProvider::GetEmptyPage() {
    mutex->P();
    unsigned int pos = b->Find();
    DEBUG('p', "Asked for page %d\n", pos);

    reservedPages--;

    // Create Table + memset
    memset(&machine->mainMemory[pos * PageSize],0,PageSize);

    // Return table pos
    mutex->V();
    return pos;
}

void
PageProvider::ReleasePage(int pageToClear) {
    ASSERT_MSG (pageToClear <= NumPhysPages && pageToClear >= 0, "PageToClear must be in a certain interval\n");
    mutex->P();
    DEBUG('p', "Asked to clear page %d\n", pageToClear);
    if(b->Test(pageToClear)) {
        b->Clear(pageToClear);
    }
    mutex->V();
}

unsigned int
PageProvider::NumAvailPage() {
    return b->NumClear() - reservedPages;
}

bool
PageProvider::ReserverPage(unsigned int nb) {
    if(NumAvailPage() >= nb) {
        reservedPages += nb;
        return true;
    }
    throw NotEnoughAvailablePagesException();
    return false;
}

PageProvider::~PageProvider() {

}