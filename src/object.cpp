/**
 * \file object.cpp
 *
 * \brief Module for objects using \ref APPLICATION
 *
 * \author Olivier de BLIC
 */



// Standard headers
#include <string>

// Project headers
#include "object.h"

// Constant values



  /**
   * \brief          Object constructor
   *
   * \param          MyName Name of the object
   */
OBJECT::OBJECT(std::string MyName)
: _ObjName(MyName)
{
}



/**
 * \brief          Object destructor
 */
OBJECT::~OBJECT()
{
}



/**
 * \brief          Getter for owning application
 *
 * \return         Reference to application
 *
 * \note           This is the solution chosen to give to any object a way to access the master APPLICATION object which owns it.
 */
APPLICATION& OBJECT::App()
{
  return *_AppPtr;
}



/// Pointer to owning application object (unique for all objects)
APPLICATION* OBJECT::_AppPtr = NULL;
