/**
 * \file exception.h
 *
 * \brief Header for exceptions use
 *
 * \author Olivier de BLIC
 */



#ifndef EXCEPTION_H
#define EXCEPTION_H

// Standard headers
#include <exception>
#include <string>

// Project headers



/**
 * \brief Generic exception dedicated for the application
 */
class EXCEPTION : public std::exception
{
public:

  /**
   * \brief          Exception constructor
   *
   * \param          Description  Short description of the cause which has thrown the exception
   */
  explicit EXCEPTION(const std::string& Description);



  /**
   * \brief          Exception destructor
   */
  virtual ~EXCEPTION() throw();



  /**
   * \brief          Getter for exception description (customized)
   *
   * \return         Description of the exception (C++ string)
   */
  const std::string& GetDescription() const throw();



  /**
   * \brief          Getter for exception description (standard)
   *
   * \return         Description of the exception (ANSI C string)
   */
  virtual const char* what() const throw();



private:

  /// Exception description
  std::string _Description;
};



#endif
