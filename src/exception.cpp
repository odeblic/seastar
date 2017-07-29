/**
 * \file exception.cpp
 *
 * \brief Module for exceptions use
 *
 * \author Olivier de BLIC
 */



// Standard headers
#include <string>

// Project headers
#include "exception.h"

// Constant values



/**
 * \brief          Exception constructor
 *
 * \param          Description  Short description of the cause which has thrown the exception
 */
EXCEPTION::EXCEPTION(const std::string& Description)
: _Description(Description)
{
  /// @todo Add the source code reference to localize the thrown exception
}



/**
 * \brief          Exception destructor
 */
EXCEPTION::~EXCEPTION() throw()
{
}



/**
 * \brief          Getter for exception description (customized)
 *
 * \return         Description of the exception (C++ string)
 */
const std::string& EXCEPTION::GetDescription() const throw()
{
  return _Description;
}



/**
 * \brief          Getter for exception description (standard)
 *
 * \return         Description of the exception (ANSI C string)
 */
const char* EXCEPTION::what() const throw()
{
  return _Description.c_str();
}
