/**
 * \file application.cpp
 *
 * \brief Module for main application
 *
 * \author Olivier de BLIC
 */



// Standard headers
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>

// Project headers
#include "application.h"
#include "object.h"
#include "socket.h"
#include "connection.h"
#include "exception.h"

// Constant values



/**
 * \brief          Application constructor
 */
APPLICATION::APPLICATION()
: OBJECT("APPLICATION"), _Running(true)
{
}



/**
 * \brief          Application destructor
 */
APPLICATION::~APPLICATION()
{
}



/**
 * \brief          Entry point of the application
 *
 * \param  ArgCnt  Number of arguments
 * \param  ArgVal  Values of arguments
 *
 * \return         EXIT_SUCCESS if in case of success
 * \return         EXIT_FAILURE if an error occurred
 */
int APPLICATION::Main(int ArgCnt, char *ArgVal[])
{
  try
  {
    Param.Parse(ArgCnt, ArgVal);

    Console.SetColors(Param.GetColors());

    Console.SetVerbose(Param.GetVerbose());

    if(Param.GetHelp())
    {
      Console.PrintHelp();
    }
    else
    {
      SetSignalConfig();

      if(Param.GetSplashscreen())
      {
        Console.PrintSplashScreen();
      }

      /// @todo Separate the source code for a generic APPLICATION object and for the server

      RunServer(Param.GetServerPort());
    }
  }

  /// @todo Use the exception handler \b catch(std::exception e) instead of \b catch(EXCEPTION e)

  catch(EXCEPTION Exception)
  {
#ifdef DEBUG
    Console.LogExcept(Exception);
#endif

    return EXIT_FAILURE;
  }

  catch(...)
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}



/**
 * \brief          Server function
 *
 * \param PortNum  Port number
 */
void APPLICATION::RunServer(int PortNum)
{
  SOCKET ListeningSocket;

  ListeningSocket.Bind(PortNum);

  ListeningSocket.Listen();

  while(_Running)
  {
    Console.LogInfo("Now waiting for a new connection");

    SOCKET& ConnectedSock = ListeningSocket.Accept();

    int HostId = Manager.Create(ConnectedSock);

    std::ostringstream Text;

    Text << "A new client is connected with host ID " << HostId;

    Console.LogInfo(Text.str());
  }
}



/**
 * \brief         Signal configuration setup
 */
void APPLICATION::SetSignalConfig()
{
  int Ret;

  struct sigaction SigAction;

  SigAction.sa_restorer = NULL;
  SigAction.sa_flags = 0;
  SigAction.sa_handler = SignalHandler;

  sigemptyset(&SigAction.sa_mask);

  /// @todo Improve the signals management

  /*
  sigaddset(&SigAction.sa_mask, SIGINT);
  sigaddset(&SigAction.sa_mask, SIGSEGV);
  sigaddset(&SigAction.sa_mask, SIGABRT);
  sigaddset(&SigAction.sa_mask, SIGTERM);
  */

  //sigprocmask(SIG_SETMASK, &SigAction.sa_mask, NULL);

  pthread_sigmask(SIG_SETMASK, &SigAction.sa_mask, NULL);

  Ret = sigaction(SIGINT,  &SigAction, NULL);
  Ret = sigaction(SIGSEGV, &SigAction, NULL);
  Ret = sigaction(SIGABRT, &SigAction, NULL);
  Ret = sigaction(SIGTERM, &SigAction, NULL);

  if(Ret != 0)
  {
    throw EXCEPTION("Error setting signal configuration");
  }
}



/**
 * \brief         Signal management function
 *
 * \param         SigNum Signal occured
 */
void APPLICATION::SignalHandler(int SigNum)
{
  App().Console.LogSignal(SigNum);

  switch(SigNum)
  {
    case SIGINT:
    case SIGTERM:
    App()._Running = false;
    break;

    case SIGSEGV:
    case SIGABRT:
    PrintBackTrace();
    exit(EXIT_FAILURE);
    break;

    default:
    break;
  }
}



/**
 * \brief         Backtrace display
 */
void APPLICATION::PrintBackTrace()
{
  void* Array[10];
  size_t Size;
  char** Strings;

  Size = backtrace(Array, 10);

  Strings = backtrace_symbols(Array, Size);

  printf("Obtained %zd stack frames.\n", Size);

  for(size_t Index = 0; Index < Size; Index++)
  {
    printf("%s\n", Strings[Index]);
  }

  free(Strings);
}
