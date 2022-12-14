// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "userthread.h"
#include "progtest.h"
#include "userfork.h"

unsigned int nb_proc = 1;

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}

unsigned copyStringFromMachine(int from,char *to,unsigned size){
  int tmp;
  unsigned int i;
  bool tailleMax = true;
  for(i = 0; i < size; i++)
  {
    machine->ReadMem(from+i,1,&tmp);
    to[i] = tmp;
    if(to[i] == '\0'){
      tailleMax = false;
      break;
    }
  }
  if(tailleMax)
    to[i-1] = '\0';
  return i;
}

unsigned copyStringToMachine(int to, char* from, unsigned size) {
  unsigned i;
  for (i = 0; i < size; i++)
  {
    machine->WriteMem(to + i,1, from[i]);
    if(from[i] == EOF){
      break;
    }
  }
  return i;
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);
    int address = machine->registers[BadVAddrReg];

    switch (which)
      {
        case SyscallException:
          {
            switch (type)
              {
                case SC_Halt:
                  {
                    DEBUG ('s', "Shutdown, initiated by user program.\n");
                    interrupt->Powerdown ();
                    break;
                  }
                #ifdef CHANGED
                case SC_PutChar:
                  {
                    DEBUG ('s',"Putchar\n");
                    consoledriver->PutChar(machine->ReadRegister(4));
                    break;
                  }
                case SC_GetChar:
                {
                  DEBUG('s',"GetChar\n");
                  int c = consoledriver->GetChar();
                  machine->WriteRegister(2,c);
                  break;
                }
                case SC_PutString:
                  {
                    DEBUG ('s',"PutString\n");
                    char buffer[MAX_STRING_SIZE];
                    copyStringFromMachine(machine->ReadRegister(4),buffer,MAX_STRING_SIZE);
                    consoledriver->PutString(buffer);
                    break;
                  }
                case SC_GetString:
                {
                  DEBUG ('s', "GetString\n");
                  char buffer[MAX_STRING_SIZE];
                  consoledriver->GetString(buffer,MAX_STRING_SIZE);
                  copyStringToMachine(machine->ReadRegister(2),buffer,MAX_STRING_SIZE);
                  break;
                }
                case SC_Exit:
                  {
                    DEBUG ('s', "Exiting the program : exit value %d\n", machine->ReadRegister(2));
                    // Si il n'y a plus qu'un seul processus vivant alors on le free completement
                    // Sinon powerdown
                    DEBUG('s',"Value of nb_proc entering SC_Exit is %d\n", nb_proc);
                    if(nb_proc > 1) {
                      // Must free
                      DEBUG('s',"Killing Processus\n");
                      nb_proc--;
                      currentThread->Finish();
                      free(currentThread->space); 

                    }
                    else {
                      DEBUG('s',"Killing Nachos\n");
                      interrupt->Powerdown();
                    }
                    break;
                  }
                case SC_ThreadCreate:
                  {
                    DEBUG ('s', "ThreadCreate\n");
                    do_ThreadCreate(machine->ReadRegister(4),machine->ReadRegister(5));
                    break;
                  }
                case SC_ForkExec:
                  {
                    DEBUG('s',"ForkExec\n");
                    char buffer[MAX_STRING_SIZE]; 
                    copyStringFromMachine(machine->ReadRegister(4),buffer,MAX_STRING_SIZE);
                    nb_proc++;
                    do_ForkExec(buffer);
                    break;
                  }
                case SC_ThreadExit:
                  {
                    unsigned int nb_thread_restant = currentThread->space->removeThread(currentThread->getSlot());
                    DEBUG('s',"slot : %d\nnb_thread_restant : %d\n",currentThread->getSlot(), nb_thread_restant);
                    if(nb_thread_restant <= 0 && nb_proc <= 1) {
                      interrupt->Powerdown();
                    }
                    currentThread->Finish();
                    DEBUG ('s', "ThreadExit\n");
                    break;
                  }
                #endif
                default:
                  {
                    ASSERT_MSG(FALSE, "Unimplemented system call %d\nMachine shutting down due to unknown syscall\n", type);
                  }
              }

            // Do not forget to increment the pc before returning!
            UpdatePC ();
            break;
          }

        case PageFaultException:
          if (!address) {
            ASSERT_MSG (FALSE, "NULL dereference at PC %x!\n", machine->registers[PCReg]);
          } else {
            // For now
            ASSERT_MSG (FALSE, "Page Fault at address %x at PC %x\n", address, machine->registers[PCReg]);
          }
          break;

        case ReadOnlyException:
          // For now
          ASSERT_MSG (FALSE, "Read-Only at address %x at PC %x\n", address, machine->registers[PCReg]);
          break;

        case BusErrorException:
          // For now
          ASSERT_MSG (FALSE, "Invalid physical address at address %x at PC %x\n", address, machine->registers[PCReg]);
          break;

        case AddressErrorException:
          // For now
          ASSERT_MSG (FALSE, "Invalid address %x at PC %x\n", address, machine->registers[PCReg]);
          break;

        case OverflowException:
          // For now
          ASSERT_MSG (FALSE, "Overflow at PC %x\n", machine->registers[PCReg]);
          break;

        case IllegalInstrException:
          // For now
          ASSERT_MSG (FALSE, "Illegal instruction at PC %x\n", machine->registers[PCReg]);
          break;

        default:
          ASSERT_MSG (FALSE, "Unexpected user mode exception %d %d %x at PC %x\n", which, type, address, machine->registers[PCReg]);
          break;
      }
}
