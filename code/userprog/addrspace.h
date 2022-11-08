// addrspace.h
//      Data structures to keep track of executing user programs
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "translate.h"
#include "noff.h"
#include "list.h"
#include "bitmap.h"

#define MAX_THREAD 10 // max number of thread
#define UserStacksAreaSize  MAX_THREAD * 256	// increase this as necessary!

class Semaphore;

class AddrSpace:public dontcopythis
{
  private:
    NoffHeader noffH;           // Program layout

    TranslationEntry * pageTable; // Page table
    unsigned int numPages;      // Number of pages in the page table
    Semaphore *mutex;
    BitMap *bitmap;
    // TODO: Bitmap *bitmap;
    /* EXEMPLE 
      BITMAP
      MAX TRHREAD = 8
      USerStackAreaSize = MAX THREAD * 256
      (voir photo 8 nov)
    */
  public:

    AddrSpace (OpenFile * executable); // Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();              // De-allocate an address space

    void InitRegisters (void);  // Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState (void);      // Save/restore address space-specific
    void RestoreState (void);   // info on a context switch
    int AllocateUserStack(void); // Allocate the new thread its space

    unsigned Dump(FILE *output, unsigned addr_s, unsigned sections_x, unsigned virtual_x, unsigned virtual_width,
                    unsigned physical_x, unsigned virtual_y, unsigned y,
                    unsigned blocksize);
                                // Dump program layout as SVG
    unsigned NumPages(void) { return numPages; }

    unsigned int nb_thread;
    unsigned addThread(void);
    unsigned removeThread(void);
};

extern List AddrspaceList;

#endif // ADDRSPACE_H
