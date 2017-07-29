/**
 * \file connection.h
 *
 * \brief Header for host connection use
 *
 * \author Olivier de BLIC
 */



#ifndef CONNECTION_H
#define CONNECTION_H

// Standard headers

// Project headers
#include "object.h"
#include "manager.h"
#include "socket.h"
#include "thread.h"



// Forward declaration (needed because of cross-references)
class MANAGER;



/**
 * \brief Host connection with socket and thread embedded
 */
class CONNECTION : public OBJECT
{
public:

  /**
   * \brief          Connection constructor
   *
   * \param Manager  Reference to the owner manager
   * \param Socket   Reference to the opened socket
   * \param HostID   Host identifier
   */
  CONNECTION(MANAGER& Manager, SOCKET& Socket, int HostID);



  /**
   * \brief          Connection destructor
   */
  virtual ~CONNECTION();



  /**
   * \brief          Getter for host identifier
   *
   * \return         Host identifier
   */
  int GetHostID() const;



private:

  /**
   * \brief          Task for the connection management
   *
   * \param  Arg     Pointer to the concerned connection
   *
   * \return         Host identifier
   */
  static void* RunTask(void* Arg);



  // Size by default of I/O buffer ?

  const int  _HostId;

  MANAGER&   _Manager;
  SOCKET&    _Socket;
  THREAD&    _Thread;
};



#endif
