/**
 * \file parameters.cpp
 *
 * \brief Module for parameters use
 *
 * \author Olivier de BLIC
 */



// Standard headers
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

// Project headers
#include "parameters.h"
#include "exception.h"
#include "application.h"

// Constant values
#define DEFLT_SERV_PORT  1101



/**
 * \brief          Parameters constructor
 */
PARAMETERS::PARAMETERS()
: _AlreadyParsed(false), _Splashscreen(false), _Colors(false), _Help(false), _Verbose(false), _PortNum(DEFLT_SERV_PORT)
{
}



/**
 * \brief          Parameters destructor
 */
PARAMETERS::~PARAMETERS()
{
}



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
void PARAMETERS::Parse(int ArgCnt, char *ArgVal[])
{
  // Parsing arguments a second time is a non-sense and thus forbidden
  if(_AlreadyParsed)
  {
    throw EXCEPTION("Arguments has been already parsed");
  }

  int Character;

  do
  {
    /// @todo Modify the option to disable colors

    Character = getopt(ArgCnt, ArgVal, ":schvp:");

    switch(Character)
    {
      case 's':
        _Splashscreen = true;
      break;

      case 'c':
        _Colors = true;
      break;

      case 'h':
        _Help = true;
      break;

      case 'v':
        _Verbose = true;
      break;

      case 'p':
      {
        int Port = atoi(optarg);

        if(Port > 0 && Port <= 65535)
        {
          _PortNum = Port;
        }
        else
        {
          throw EXCEPTION("Port number is out of range");
        }
      }
      break;

      case '?':
        throw EXCEPTION("Unknow option in command line");
      break;

      case ':':
        throw EXCEPTION("Missing parameter in command line");
      break;

      case -1:
        // End of options
      break;

      default:
        std::cout << ">>> " << Character << std::endl;
        throw EXCEPTION("Unexpected character" + Character);
      break;
    }
  }
  while(Character != -1);

  _AlreadyParsed = true;
}



/**
 * \brief          Getter for splashscreen display
 *
 * \return         \b true if enabled
 * \return         \b false if disabled
 */
bool PARAMETERS::GetSplashscreen() const
{
  return _Splashscreen;
}



/**
 * \brief          Getter for colors use
 *
 * \return         \b true if enabled
 * \return         \b false if disabled
 */
bool PARAMETERS::GetColors() const
{
  return _Colors;
}



/**
 * \brief          Getter for help query
 *
 * \return         \b true if enabled
 * \return         \b false if disabled
 */
bool PARAMETERS::GetHelp() const
{
  return _Help;
}



/**
 * \brief          Getter for verbose mode
 *
 * \return         \b true if enabled
 * \return         \b false if disabled
 */
bool PARAMETERS::GetVerbose() const
{
  return _Verbose;
}



/**
 * \brief          Getter for port number
 *
 * \return         Port number
 */
unsigned short PARAMETERS::GetServerPort() const
{
  return _PortNum;
}
