/**
 * \file application.h
 *
 * \brief Header for main application
 *
 * \author Olivier de BLIC
 */



#ifndef APPLICATION_H
#define APPLICATION_H

// Standard headers

// Project headers
#include "object.h"
#include "parameters.h"
#include "console.h"
#include "manager.h"



/**
 * \brief Running program and all its components as subobjects
 */
class APPLICATION : public OBJECT
{
private:

public:

  /**
   * \brief          Application constructor
   */
  APPLICATION();



  /**
   * \brief          Application destructor
   */
  virtual ~APPLICATION();



  /**
   * \brief          Entry point of the application
   *
   * \param  ArgCnt  Number of arguments
   * \param  ArgVal  Values of arguments
   *
   * \return         EXIT_SUCCESS if in case of success
   * \return         EXIT_FAILURE if an error occurred
   */
  int Main(int ArgCnt, char *ArgVal[]);



  /// Parameters management
  PARAMETERS Param;



  /// Console outputs management
  CONSOLE    Console;



  /// Clients connections management
  MANAGER    Manager;



private:

  /**
   * \brief          Server function
   *
   * \param PortNum  Port number
   */
  void RunServer(int PortNum);



  /**
   * \brief         Signal configuration setup
   */
  void SetSignalConfig();



  /**
   * \brief         Signal management function
   *
   * \param         SigNum Signal occured
   */
  static void SignalHandler(int SigNum);



  /**
   * \brief         Backtrace display
   */
  static void PrintBackTrace();



  /// Flag for state of appplication
  bool _Running;
};



#endif
