/**
 * \file thread.cpp
 *
 * \brief Module for threads use
 *
 * \author Olivier de BLIC
 */



// Standard headers
#include <pthread.h>
#include <signal.h>

// Project headers
#include "thread.h"
#include "exception.h"
#include "application.h"

// Constant values



/**
 * \brief        Thread constructor
 *
 * \param Proc   Pointer to function used to run thread
 *
 * \param Arg    Argument to pass to the function
 */
THREAD::THREAD(void* (*Proc)(void*), void* Arg)
: OBJECT("THREAD"), _Procedure(Proc), _Argument(Arg)
{
#ifdef DEBUG
  App().Console.LogCtor(_ObjName);
#endif

  // Initialize the thread attributes
  if(pthread_attr_init(&_Attr) != 0)
  {
    throw EXCEPTION("Error creating thread attributes");
  }

  // Set the attributes with detached proprety at thread creation
  if(pthread_attr_setdetachstate(&_Attr, PTHREAD_CREATE_DETACHED) != 0)
  {
    throw EXCEPTION("Error setting thread attributes");
  }

  // Blocks all signals except SEGV, ABORT, INT, TERM
  if(sigfillset(&_SigMask) != 0          ||
     sigdelset(&_SigMask, SIGSEGV) != 0  ||
     sigdelset(&_SigMask, SIGABRT) != 0  ||
     sigdelset(&_SigMask, SIGINT)  != 0  ||
     sigdelset(&_SigMask, SIGTERM) != 0  )
  {
    throw EXCEPTION("Error setting thread signal mask");
  }
}



/**
 * \brief        Thread destructor
 */
THREAD::~THREAD()
{
#ifdef DEBUG
  App().Console.LogDtor(_ObjName);
#endif

  if(pthread_cancel(_ThreadId) != 0)
  {
    throw EXCEPTION("Error cancelling thread");
  }

  if(pthread_attr_destroy(&_Attr) != 0)
  {
    throw EXCEPTION("Error destroying thread attributes");
  }
}



/**
 * \brief        Start the thread
 */
void THREAD::Run()
{
  if(pthread_sigmask(SIG_SETMASK, &_SigMask, NULL) != 0)
  {
    throw EXCEPTION("Error setting signal mask for thread");
  }

  if(pthread_create(&_ThreadId, &_Attr, ThreadFunction, this) != 0)
  {
    throw EXCEPTION("Error creating thread");
  }
}



/**
 * \brief        Stop the thread
 */
void THREAD::Cancel()
{

  if(pthread_cancel(_ThreadId) != 0)
  {
    throw EXCEPTION("Error cancelling thread");
  }
}



/**
 * \brief        Thread function
 *
 * \param Arg    Generic pointer to current thread
 *
 * \return       Generic pointer to get thread return
 */
void* THREAD::ThreadFunction(void* Arg)
{
  THREAD& ThisThread = *(THREAD*)Arg;

  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

  return ThisThread._Procedure(ThisThread._Argument);
}
