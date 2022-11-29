#include "system.h"

PageProvider::PageProvider(unsigned int size,TranslationEntry * currentPageTable) {
    b = new BitMap(size);
    taille = size;
    pageTable = currentPageTable;
}

unsigned int
PageProvider::GetEmptyPage() {
    unsigned int pos = b->Find();
    DEBUG('p', "Asked for page %d\n", pos);

    // Create Table + memset
    memset(&pageTable[pos],0,taille);

    // Return table pos
    return pos;
}

void
PageProvider::ReleasePage(int pageToClear) {
    DEBUG('p', "Asked to clear page %d\n", pageToClear);
    b->Clear(pageToClear);
    // Should we set it back to 0 ?
}

unsigned int
PageProvider::NumAvailPage() {
    return b->NumClear();
}

PageProvider::~PageProvider() {

}