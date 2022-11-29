#include "bitmap.h"

class PageProvider
{
private:
    BitMap* b;
    unsigned int taille;
    TranslationEntry* pageTable;
public:
    PageProvider(unsigned int size,TranslationEntry * currentPageTable);
    ~PageProvider();
    unsigned int GetEmptyPage();
    void ReleasePage(int pageToClear);
    unsigned int NumAvailPage();
};