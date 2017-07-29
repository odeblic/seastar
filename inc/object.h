/**
 * \file object.h
 *
 * \brief Header for objects using \ref APPLICATION
 *
 * \author Olivier de BLIC
 */



#ifndef OBJECT_H
#define OBJECT_H

// Standard headers
#include <string>

// Project headers

// Constant values



// Forward declaration (needed because of cross-references)
class APPLICATION;



/**
 * \brief Base class to give objects a name and an access to the object \ref APPLICATION
 */
class OBJECT
{
  // The main() function must be allowed to initialize \ref _AppPtr
  friend int main(int argc, char *argv[]);

// This class is an abstract class and cannot being instanciated directly
protected:

  /**
   * \brief          Object constructor
   *
   * \param          MyName Name of the object
   */
  OBJECT(std::string MyName = "<unnamed>");



  /**
   * \brief          Object destructor
   */
  virtual ~OBJECT();



  /**
   * \brief          Getter for owning application
   *
   * \return         Reference to application
   *
   * \note           This is the solution chosen to give to any object a way to access the master APPLICATION object which owns it.
   */
  static APPLICATION& App();



  /// Name of current object (for logs and debug)
  const std::string  _ObjName;



private:

  /// Pointer to owning application object (unique for all objects)
  static APPLICATION*  _AppPtr;
};


#endif
