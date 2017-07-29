/**
 * \file main.cpp
 *
 * \brief Module containing the main() function
 *
 * \author Olivier de BLIC
 */



// Standard headers

// Project headers
#include "object.h"
#include "application.h"

// Constant values



/**
 *  \todo Apply the \b const rules in the whole project
 *  \todo Use the \b namespace ss = sea_star; to declare identifiers dedicated to the project
 *
 *  \xrefitem test "heading1" "Test list" Client connection
 *  \xrefitem test "heading2" "Test list" Unique ID
 *  \xrefitem test "heading3" "Test list" Number of client sent by server
 *  \xrefitem test "heading4" "Test list" Client disconnection
 */



/**
 * \brief   Entry point of the program
 *
 * \param   argc      Number of arguments
 * \param   argv      Values of arguments
 *
 * \return            EXIT_SUCCESS if in case of success
 * \return            EXIT_FAILURE if an error occurred
 *
 * \remarks This function is just an interface between the caller and the objet APPLICATION which is the core of the program.
 */
int main(int argc, char *argv[])
{
  APPLICATION App;

  OBJECT::_AppPtr = &App;

  return App.Main(argc, argv);
}



/**
* \mainpage Multi-threaded TCP/IP server for simultaneous clients connections
*
* \section  SECTION_FOREWORD Foreword
*           The seastar is an beautiful aquatic animal whom several arms join the center of its body,
*           like several clients can connect to a server.
*           That's why I have chosen this mascot figuring as the logo for my project.
*           Every famous Open Source Project has one so why not mine ?
*
* \image    html    logo0.png
* \image    latex   logo0.png "Logo (seastar)" width=8cm
*
* \section  SECTION_INTRODUCTION Introduction
*           The goal of the project is the developpment of a TCP/IP server
*           which smultaneously deals with several connected clients.
*           Remember that it is only a demonstrative project that is not aimed
*           to be released within a long lifecycle...
*
* \section  SECTION_SUPPORT Support
*           For any question or bug reporting, please contact the author
*           \htmlonly <a href="mailto:odeblic@gmail.com?subject=Sea%20Star%20Project&body=Hi Olivier,%0A%0AI%20loved%20your%20project%20but...%0A%0ARegards,">Olivier de BLIC.</a> \endhtmlonly
*           \latexonly \href{mailto:odeblic@gmail.com?subject=Sea%20Star%20Project&body=Hi Olivier,%0A%0AI%20loved%20your%20project%20but...%0A%0ARegards,}{Olivier de BLIC}. \endlatexonly
*
* \page     PAGE_SPECIFICATIONS Specifications
*
* \brief    Here are the original basic specifications for the project
*
*           \par
*           Write a server in C++ language serving arbitrary number of TCP clients.
*           \li For every TCP client connected a new thread in the server will be spawned. The thread will send a server-wide unique binary 32 bit ID number every second. The ID will be ASCII coded before sending to client and terminated by a new line character. The mechanism for finding unique IDs is performance sensitive and cannot be pre-computed.
*           \li The server also responds new line character received from the client with number of clients connected.
*           \li Ctrl-C will send "Bye\n" to all the connected clients and immediately terminate the server cleanly.
*           \li Limit the usage of global variables.
*
* \page     PAGE_DEVELOPMENT Development
*
* \brief    Here are the detailed documentation of the software design and the source code.
*
* \section  SECTION_ARCHITECTURE Architecture
*           \par
*           The program is strongly inspired of a program in Java : everything is an object (no global variable out of any class).
*
* \section  SECTION_CODING_RULES Coding rules
*           \par
*           \li No pointer declared whithout initialization
*           \li Public members of builtin types are forbidden
*           \li No use of default constructors and destructors
*           \li Only one class definition/declaration per file
*
* \section  SECTION_NAMING_CONVENTION Naming convention
*           \par
*           \li Variables use CamelCase convention.
*           \li Every identifier starting with an underscore stands for a private member.
*           \li File names only use alphanumeric lowercase characters.
*
* \page     PAGE_HOW_BUILD Build chain
*
* \brief    Here is the description of the build chain for the outputs (program and docmentation)
*
* \section  SECTION_REQUIREMENTS Requirements
*           \par For the program
*           All the classic tools for builing Linux programs are needed (gcc, g++, ld, make, etc.)
*           \par For the documentation
*           Doxygen and texlive are needed for automatic documentation generation (in \b HTML, \b Latex and \b PDF).
*
* \section  SECTION_OUTPUTS Outputs
*           Generated files (objects and binary) are same for release and debug building mode, thus you need to rebuild all the project when changing mode (target \b clean and then \b all).
*           \par
*           Use the make command to generate the available targets which are :
*           \li \b all       debug or release version (release by default)
*           \li \b dep       dependencies generation
*           \li \b tarball   backup of the whole project in a tarball
*           \li \b clean     cleanup of the project directory
*           \par
*           For the debug or release version selection, use the following commands :
*           \li make \b clean && make \b all -DDEBUG
*           \li make \b clean && make \b all
*
* \page     PAGE_TEST Test and validation
*
* \brief    How to test the program to check the specifications matching.
*
* \section  SECTION_REQUIREMENTS Requirements
*           \b netcat (or other TCP client) is needed to connect to the server as client from a terminal.
*           Linux OS on a standard PC architecture.
*
* \section  SECTION_RUN_TIME Run time
*           Use the dedicated script :
*           \li sh test.sh
*
* \page     PAGE_USE User manual
*
* \brief    How to use the program.
*           \par
*           Run \b seastar to wait for incoming connections (options -s -v -c are so nice...) :
*           \li seastar -svc
*           \par
*           Use \b netcat to connect to the server as host :
*           \li nc localhost 1101
*
* \section  SECTION_PICTURE_HELP Screenshot of built-in help
*
* \image    html    screenshot_help.png
* \image    latex   screenshot_help.png "Help (screenshot)" width=16cm
*
* \section  SECTION_PICTURE_START Screenshot of start using options
*
* \image    html    screenshot_spash_and_colors.png
* \image    latex   screenshot_spash_and_colors.png "Use (screenshot)" width=16cm
*
* \section  SECTION_PICTURE_HOSTS Screenshot of connected hosts
*
* \image    html    screenshot_server.png
* \image    latex   screenshot_server.png "Server (screenshot)" width=16cm
*
* \image    html    screenshot_host_1.png
* \image    latex   screenshot_host_1.png "Host 1 (screenshot)" width=16cm
*
* \image    html    screenshot_host_2.png
* \image    latex   screenshot_host_2.png "Host 2 (screenshot)" width=16cm
*
*/
