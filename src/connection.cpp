/**
 * \file connection.cpp
 *
 * \brief Module for host connection use
 *
 * \author Olivier de BLIC
 */



// Standard headers
#include <string>
#include <unistd.h>

// Project headers
#include "connection.h"
#include "object.h"
#include "application.h"

// Constant values
#define CYCLE_DURATION_MS       (1000)
#define SLICE_DURATION_MS       (50)
#define CYCLE_DURATION_US       ((CYCLE_DURATION_MS) * (1000))
#define SLICE_DURATION_US       ((SLICE_DURATION_MS) * (1000))



/**
 * \brief          Connection constructor
 *
 * \param Manager  Reference to the owner manager
 * \param Socket   Reference to the opened socket
 * \param HostID   Host identifier
 */
CONNECTION::CONNECTION(MANAGER& Manager, SOCKET& Socket, int HostID)
: OBJECT("CONNECTION"), _Manager(Manager), _HostId(HostID), _Socket(Socket), _Thread(*new THREAD(RunTask, (void*)this))
{
#ifdef DEBUG
  App().Console.LogCtor(_ObjName);
#endif

  _Thread.Run();
}



/**
 * \brief          Connection destructor
 */
CONNECTION::~CONNECTION()
{
#ifdef DEBUG
  App().Console.LogDtor(_ObjName);
#endif

  if(_Socket.IsConnected())
  {
    _Socket.Send("BYE\n");
  }

  _Thread.Cancel();
}


/**
 * \brief          Getter for host identifier
 *
 * \return         Host identifier
 */
int CONNECTION::GetHostID() const
{
  return _HostId;
}



/**
 * \brief          Task for the connection management
 *
 * \param  Arg     Pointer to the concerned connection
 *
 * \return         Host identifier
 */
void* CONNECTION::RunTask(void* Arg)
{
  // Equivalent of 'this' pointer in a non-static method
  CONNECTION& HostConn = *(CONNECTION*)Arg;

  std::ostringstream Buffer;

  //HostConn._Manager.Add(&HostConn);

  App().Console.LogInfo("Local address is " + HostConn._Socket.GetLocalAddr());
  App().Console.LogInfo("Remote address is " + HostConn._Socket.GetRemoteAddr());

  try
  {
    while(HostConn._Socket.IsConnected())
    {
      // Send the host ID
      Buffer << "ID=" << HostConn._HostId << std::endl;
      HostConn._Socket.Send(Buffer.str());

      // Clear the buffer
      Buffer.clear();
      Buffer.str("");

      for(__useconds_t ElapsedTime = CYCLE_DURATION_US; ElapsedTime > 0; ElapsedTime -= SLICE_DURATION_US)
      {
        usleep(SLICE_DURATION_US);

        if(HostConn._Socket.IsDataWaiting())
        {
          std::string Data = HostConn._Socket.Receive();

          /// \bug IsDataWaiting() just ensure that reading socket will not block ! Data is not waiting for sure !

          if(Data.length() > 0)
          {
            App().Console.LogInfo("Data received : '" + Data + "'", SOURCE_LINE);

#if 0
            // The parser will do something like (ignoring case and trailing blanks, taking parameters, etc.)

            if(Data == "play")
            {
              // Do something
            }
            else if(Data == "pause")
            {
              // Do something
            }
            else if(Data == "stop")
            {
              // Do something
            }
            else if(Data == "open")
            {
              // Do something
            }
            else if(Data == "kill")
            {
              // Do something
            }
            else
            {
              // Do nothing
            }
#endif

            if(Data.find('\n') != Data.npos)
            {
              Buffer << "COUNT=" << HostConn._Manager.Count() << std::endl;
              HostConn._Socket.Send(Buffer.str());

              Buffer.clear();
              Buffer.str("");
            }
          }
        }
      }
    }
  }

  catch(EXCEPTION Exception)
  {
    App().Console.LogExcept(Exception);
  }

  //HostConn._Manager.Remove(&HostConn);

  HostConn._Manager.Destroy(HostConn._HostId);

  return NULL;
}
