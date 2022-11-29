#include "bitmap.h"
#include "synch.h"

class PageProvider
{
private:
    BitMap* b;
    Semaphore* mutex;
    unsigned int reservedPages;
public:
    PageProvider();
    ~PageProvider();
    unsigned int GetEmptyPage();
    void ReleasePage(int pageToClear);
    unsigned int NumAvailPage();
    bool ReserverPage(unsigned int nb);
};