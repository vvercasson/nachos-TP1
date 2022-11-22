#include "bitmap.h"

class PageProvider
{
private:
    BitMap* b;
public:
    PageProvider(unsigned int size);
    ~PageProvider();
    unsigned int GetEmptyPage();
};