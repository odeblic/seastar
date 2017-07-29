/**
 * \file manager.h
 *
 * \brief Header for connections management
 *
 * \author Olivier de BLIC
 */



#ifndef MANAGER_H
#define MANAGER_H

// Standard headers
#include <pthread.h>
#include <set>

// Project headers
#include "object.h"



// Forward declarations (needed because of cross-references)
class CONNECTION;
class SOCKET;



/// Container for uniques values set (pointers to host connections)
typedef std::set<CONNECTION*> CONTAINER;



/**
 * \brief Connections manager
 */
class MANAGER : public OBJECT
{
  friend class APPLICATION;

private:

  /**
   * \brief          Manager constructor
   */
  MANAGER();



  /**
   * \brief          Manager destructor
   */
  virtual ~MANAGER();



public:

  /**
   * \brief          Creation of new connection
   *
   * \param  Socket  Socket already opened
   *
   * \return         Host identifier
   */
  int Create(SOCKET&);



  /**
   * \brief          Destruction of a connection
   *
   * \param  HostId  Host identifier of connection to destroy
   */
  void Destroy(int HostId);



  /**
   * \brief          Getter for the current number of connections
   *
   * \return         Number of connections in the container
   */

  int Count() const;



private:

  /**
   * \brief          Generator for new host identifer
   *
   * \return         New host identifer
   */
  int NewHostID();



  /**
   * \brief          Add a connection to the container
   *
   * \param Connection Pointer to the connection
   */
  void Add(CONNECTION* Connection);



  /**
   * \brief          Remove a connection from the container
   *
   * \param Connection Pointer to the connection
   */
  void Remove(CONNECTION* Connection);



  pthread_mutex_t _Lock;

  CONTAINER       _Container;
};



#endif
