/**
 * \file thread.h
 *
 * \brief Header for threads use
 *
 * \author Olivier de BLIC
 */



#ifndef THREAD_H
#define THREAD_H

// Standard headers
#include <pthread.h>

// Project headers
#include "object.h"



/**
 * \brief Wrapper for POSIX threads use in C++
 */
class THREAD : public OBJECT
{
public:

  /**
   * \brief        Thread constructor
   *
   * \param Proc   Pointer to function used to run thread
   *
   * \param Arg    Argument to pass to the function
   */
  THREAD(void* (*Proc)(void*), void* Arg);



  /**
   * \brief        Thread destructor
   */
  ~THREAD();



  /**
   * \brief        Starts the thread
   */
  void Run();



  /**
   * \brief        Stop the thread
   */
  void Cancel();



private:

  /**
   * \brief        Thread function
   *
   * \param Arg    Generic pointer to current thread
   *
   * \return       Generic pointer to get thread return
   */
  static void* ThreadFunction(void* Arg);



  /// POSIX thread identifier
  pthread_t       _ThreadId;

  /// POSIX thread attributes
  pthread_attr_t  _Attr;

  /// POSIX signal thread mask
  sigset_t        _SigMask;

  /// Pointer to the function the thread has to run
  void*         (*_Procedure)(void*);

  /// Pointer to the argument the thread has to pass to its function
  void*           _Argument;
};



#endif
