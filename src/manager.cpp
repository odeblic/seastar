/**
 * \file manager.cpp
 *
 * \brief Module for connections management
 *
 * \author Olivier de BLIC
 */



// Standard headers
#include <set>
#include <pthread.h>

// Project headers
#include "manager.h"
#include "application.h"
#include "connection.h"
#include "socket.h"
#include "console.h"

// Constant values



/**
 * \brief          Manager constructor
 */
MANAGER::MANAGER()
: OBJECT("MANAGER")
{
  pthread_mutex_init(&_Lock, NULL);
}



/**
 * \brief          Manager destructor
 */
MANAGER::~MANAGER()
{
  CONTAINER::iterator it;

  for(it = _Container.begin(); it != _Container.end(); it++)
  {
    delete (*it);
    _Container.erase(it);
  }

  pthread_mutex_destroy(&_Lock);
}



/**
 * \brief          Creation of new connection
 *
 * \param  Socket  Socket already opened
 *
 * \return         Host identifier of newly created connection
 */
int MANAGER::Create(SOCKET& Socket)
{
  // Actually, new ID generator reuses socket ID
  int HostId = Socket.GetId();

  // In the future, possible use of a dedicated ID generator
  //int HostId = NewHostID();

  CONNECTION* Connection = new CONNECTION(*this, Socket, HostId);

  Add(Connection);

  return HostId;
}



/**
 * \brief          Destruction of a connection
 *
 * \param  HostId  Host identifier of connection to destroy
 */
void MANAGER::Destroy(int HostId)
{
  CONTAINER::iterator it;

  for(it = _Container.begin(); it != _Container.end(); it++)
  {
    CONNECTION& Connection = *(CONNECTION*)(*it);

    if(Connection.GetHostID() == HostId)
    {
      Remove(&Connection);
      delete &Connection;
      return;
    }
  }

  throw EXCEPTION("Object not removed (ID not found)");
}



/**
 * \brief          Add a connection to the container
 *
 * \param Connection Pointer to the connection
 */
void MANAGER::Add(CONNECTION* Connection)
{
  App().Console.LogInfo("Adding object", SOURCE_LINE);

  std::pair<CONTAINER::iterator, bool> Ret;

  pthread_mutex_lock(&_Lock);
  Ret = _Container.insert(Connection);
  pthread_mutex_unlock(&_Lock);

  if(! Ret.second)
  {
    throw EXCEPTION("Object not added (ID already exists)");
  }

  App().Console.LogInfo("Object added", SOURCE_LINE);
}



/**
 * \brief          Remove a connection from the container
 *
 * \param Connection Pointer to the connection
 */
void MANAGER::Remove(CONNECTION* Connection)
{
  App().Console.LogInfo("Removing object", SOURCE_LINE);

  CONTAINER::iterator it;

  it = _Container.find(Connection);

  if(it != _Container.end())
  {
    pthread_mutex_lock(&_Lock);
    _Container.erase(it);
    pthread_mutex_unlock(&_Lock);
  }
  else
  {
    throw EXCEPTION("Object not removed (ID not found)");
  }

  App().Console.LogInfo("Object removed", SOURCE_LINE);
}



/**
 * \brief          Getter for the current number of connections
 *
 * \return         Number of connections in the container
 */
int MANAGER::Count() const
{
  return _Container.size();
}



/**
 * \brief          Generator for new host identifer
 *
 * \return         New host identifer
 */
int MANAGER::NewHostID()
{
  /*
  static int CurrentHostId = 0;

  if(CurrentHostId <= 1000)
  {
    Debug("CurrentHostId");
    CurrentHostId++;
    //_HostId = CurrentHostId;
  }
  else
  {
    Debug("too many hosts");
    throw "too many hosts";
    // Vérifier si un client a libéré de la place
  }
  */

  throw EXCEPTION("Should not be called because not implemented");
}
