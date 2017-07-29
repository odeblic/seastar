/**
 * \file parameters.h
 *
 * \brief Header for parameters use
 *
 * \author Olivier de BLIC
 */



#ifndef PARAMETERS_H
#define PARAMETERS_H

// Standard headers
#include <string>

// Project headers



/**
 * \brief Parser to read parameters passed to the main() function
 */
class PARAMETERS
{
public:

  /**
   * \brief          Parameters constructor
   */
  PARAMETERS();



  /**
   * \brief          Parameters destructor
   */
  ~PARAMETERS();



  /**
   * \brief          Parser for arguments
   *
   * \param  ArgCnt  Number of arguments
   * \param  ArgVal  Values of arguments
   *
   * \pre The arguments should be ones passed to the main() function.
   *
   * \post The options of the program are defined according to the passed arguments.
   */
  void Parse(int ArgCnt, char* ArgVal[]);



  /**
   * \brief          Getter for splashscreen display
   *
   * \return         \b true if enabled
   * \return         \b false if disabled
   */
  bool GetSplashscreen() const;



  /**
   * \brief          Getter for colors use
   *
   * \return         \b true if enabled
   * \return         \b false if disabled
   */
  bool GetColors() const;



  /**
   * \brief          Getter for help query
   *
   * \return         \b true if enabled
   * \return         \b false if disabled
   */
  bool GetHelp() const;



  /**
   * \brief          Getter for verbose mode
   *
   * \return         \b true if enabled
   * \return         \b false if disabled
   */
  bool GetVerbose() const;



  /**
   * \brief          Getter for port number
   *
   * \return         Port number
   */
  unsigned short GetServerPort() const;



private:

  bool           _AlreadyParsed;

  bool           _Splashscreen;

  bool           _Colors;

  bool           _Help;

  bool           _Verbose;

  unsigned short _PortNum;
};



#endif
