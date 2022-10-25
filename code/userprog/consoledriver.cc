#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

// initialiser mutex
static Semaphore *mutex;

static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }

ConsoleDriver::ConsoleDriver(const char *in, const char *out)
{
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    mutex = new Semaphore("mutex", 0);
    console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, NULL);
    mutex->V();

}

ConsoleDriver::~ConsoleDriver()
{
    delete console;
    delete writeDone;
    delete readAvail;
    delete mutex;
}

void ConsoleDriver::PutChar(int ch)
{
    //TODO: Mutex lock
    mutex->P();
    console->TX(ch);
    writeDone->P();
    mutex->V();
}

int ConsoleDriver::GetChar()
{
    mutex->P();
    readAvail->P ();
    mutex->V();      // wait for character to arrive
    return console->RX ();
}

void ConsoleDriver::PutString(const char *s)
{
    mutex->P();
    int i = 0;
    while(s[i] != '\0') {
        PutChar(s[i]);
        i++;
    }
    mutex->V();
}

void ConsoleDriver::GetString(char *s, int n)
{
    mutex->P();
    for(int i = 0; i < n; i++) {
        s[i] = GetChar();
        if (s[i] == '\0' || s[i] == EOF) {
            return;
        }
    }
    mutex->V();
}

#endif // CHANGED