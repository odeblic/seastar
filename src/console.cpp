/**
 * \file console.cpp
 *
 * \brief Module for console use
 *
 * \author Olivier de BLIC
 */



// Standard headers
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <signal.h>
#include <typeinfo>
#include <time.h>

// Project headers
#include "console.h"
#include "exception.h"

// Constant values
#define CODE_GRA                  "0"
#define CODE_RED                  "1"
#define CODE_GRE                  "2"
#define CODE_YEL                  "3"
#define CODE_BLU                  "4"
#define CODE_MAG                  "5"
#define CODE_CYA                  "6"
#define CODE_WHI                  "7"
#define CODE_BLK                  "9"

// ANSI escape sequences for console text coloration
#define ESC_SEQ(FG_COL, BG_COL)  "\e[1;3" FG_COL ";4" BG_COL "m"
#define ESC_GRA                   ESC_SEQ(CODE_GRA, CODE_BLK)
#define ESC_RED                   ESC_SEQ(CODE_RED, CODE_BLK)
#define ESC_GRE                   ESC_SEQ(CODE_GRE, CODE_BLK)
#define ESC_YEL                   ESC_SEQ(CODE_YEL, CODE_BLK)
#define ESC_BLU                   ESC_SEQ(CODE_BLU, CODE_BLK)
#define ESC_MAG                   ESC_SEQ(CODE_MAG, CODE_BLK)
#define ESC_CYA                   ESC_SEQ(CODE_CYA, CODE_BLK)
#define ESC_WHI                   ESC_SEQ(CODE_WHI, CODE_BLK)
#define ESC_BLK                   ESC_SEQ(CODE_BLK, CODE_BLK)
#define ESC_STD                   "\e[0m"



/**
 * \brief          Console constructor
 */
CONSOLE::CONSOLE()
: _Stream(std::cout)
{
  pthread_mutex_init(&_Lock, NULL);
}



/**
 * \brief          Console destructor
 */
CONSOLE::~CONSOLE()
{
  pthread_mutex_destroy(&_Lock);
}



/**
 * \brief          Configuration setter for colored mode
 *
 * \param Enabled  true for enable, false for disable
 */
void CONSOLE::SetColors(bool Enabled)
{
  _Colors = Enabled;
}



/**
 * \brief          Configuration setter for verbose mode
 *
 * \param Enabled  true for enable, false for disable
 */
void CONSOLE::SetVerbose(bool Enabled)
{
  _Verbose = Enabled;
}



/**
 * \brief          Configuration setter for fullpath display mode
 *
 * \param Enabled  true for enable, false for disable
 */
void CONSOLE::SetFullPath(bool Enabled)
{
  _FullPath = Enabled;
}



#ifdef DEBUG
/**
 * \brief            Text log (debug only)
 *
 * \param Text       Text to log
 * \param Source     Source line from where the function is call
 */
void CONSOLE::LogText(std::string Text, std::string Source)
{
  PrintLogLine(LOG_DEBUG, Text, Source);
}



/**
 * \brief            Number log (debug only)
 *
 * \param Number     Number to log
 * \param Source     Source line from where the function is call
 */
template<typename T>
void LogNumber(T Number, std::string Desc = "", std::string Source = "")
{
}



/**
 * \brief            Constructor call log (debug only)
 *
 * \param ClassName  Name of the constructed class
 * \param Source     Source line from where the function is call
 */
void CONSOLE::LogCtor(std::string ClassName, std::string Source)
{
  PrintLogLine(LOG_CTOR, ClassName, Source);
}



/**
 * \brief            Destructor call log (debug only)
 *
 * \param ClassName  Name of the destroyed class
 * \param Source     Source line from where the function is call
 */
void CONSOLE::LogDtor(std::string ClassName, std::string Source)
{
  PrintLogLine(LOG_DTOR, ClassName, Source);
}



/**
 * \brief            Pointer log (debug only)
 *
 * \param Pointer    Pointer to log
 * \param Source     Source line from where the function is call
 */
template<typename T>
void CONSOLE::LogPointer(T* Pointer, std::string Source)
{
  _Stream << "typeid(T).name() : " << typeid(T).name() << std::endl;
}



/**
 * \brief            Symbol log (debug only)
 *
 * \param Addr       Symbol address to log
 * \param Source     Source line from where the function is call
 */
void CONSOLE::LogSymbol(void* Addr, std::string Source)
{
  _Stream << "Address : " << Addr << std::endl;
}
#endif



/**
 * \brief            Blank log
 */
void CONSOLE::LogBlank()
{
  PrintLogLine(LOG_BLANK);
}



/**
 * \brief            Information log
 *
 * \param Msg        Information message
 * \param Source     Source line from where the function is call
 */
void CONSOLE::LogInfo(std::string Msg, std::string Source)
{
  if(_Verbose)
  {
    PrintLogLine(LOG_INFO, Msg, Source);
  }
}



/**
 * \brief            Warning log
 *
 * \param Msg        Warning message
 * \param Source     Source line from where the function is call
 */
void CONSOLE::LogWarn(std::string Msg, std::string Source)
{
  PrintLogLine(LOG_WARN, Msg, Source);
}



/**
 * \brief            Error log
 *
 * \param Msg        Error message
 * \param Source     Source line from where the function is call
 */
void CONSOLE::LogError(std::string Msg, std::string Source)
{
  PrintLogLine(LOG_ERROR, Msg, Source);
}



/**
 * \brief            Exception occurrence log
 *
 * \param Exception  Exception object
 * \param Source     Source line from where the function is call
 */
void CONSOLE::LogExcept(EXCEPTION Exception, std::string Source)
{
  PrintLogLine(LOG_EXCEPT, Exception.GetDescription(), Source);
}



/**
 * \brief            Signal occurrence log
 *
 * \param SigNum     Signal number
 */
void CONSOLE::LogSignal(int SigNum)
{
  std::ostringstream Buffer;

  Buffer << "Signal n° " << SigNum;

  switch(SigNum)
  {
    case SIGABRT:
      Buffer << " (abort)";
    break;

    case SIGINT:
      Buffer << " (interrupt)";
    break;

    case SIGSEGV:
      Buffer << " (segmentation fault)";
    break;

    case SIGTERM:
      Buffer << " (terminate)";
    break;

    default:
    break;
  }

  PrintLogLine(LOG_SIGNAL, Buffer.str());
}



/**
 * \brief            Print the splashscreen at start
 */
void CONSOLE::PrintSplashScreen()
{
  InitLogger();

  _Buffer << ColCya() <<
  "                               ***                         \n"
  "                             *******                       \n"
  "                            *********                      \n"
  "                           **********                      \n"
  "                           ***********                     \n"
  "                          ************                     \n"
  "                         *************                     \n"
  " ***********            *************                      \n"
  "****************        *************                      \n"
  " ************************************                      \n"
  "  ***********************************                      \n"
  "    *****************************************************  \n"
  "      *****************************************************\n"
  "         ************ " << ColYel() << "SEA STAR v0.0.0.1" << ColCya() << " *******************\n"
  "            *********************************************  \n"
  "              ******* " << ColYel() << "by Olivier de BLIC" << ColCya() << " **************    \n"
  "               ***********************************         \n"

#ifdef DEBUG
  "              *********** " << ColRed() << "DEBUG !!!" << ColCya() << " *********              \n"
#else
  "              *******************************              \n"
#endif

  "             ****************************                  \n"
  "           *****************************                   \n"
  "         *******************************                   \n"
  "        *********************************                  \n"
  "       *****************   **************                  \n"
  "      ***************       **************                 \n"
  "     *************           *************                 \n"
  "     ***********               ************                \n"
  "      *******                   ***********                \n"
  "                                 **********                \n"
  "                                  *********                \n"
  "                                    ******                 \n"
  << ColStd();

  ReleaseLogger();
}



/**
 * \brief            Print the help
 */
void CONSOLE::PrintHelp()
{
  InitLogger();

  _Buffer <<
  "Use :      seastar [-h] [-s] [-v] [-c] [-p portnum]              \n"
  "                                                                 \n"
  "Options :  -h  print help                                        \n"
  "           -s  display the splash screen at start                \n"
  "           -v  use the verbose mode                              \n"
  "           -c  use colored text in output                        \n"
  "           -p  set server port for listening (default is 1101)   \n"
  ;

  ReleaseLogger();
}



/**
 * \brief            Signal occurrence log
 *
 * \param Type       Type of log
 * \param LogData    Undefined
 * \param Source     Source line from where the function is call
 */
void CONSOLE::PrintLogLine(LOG_TYPE Type, std::string LogData, std::string Source)
{
  InitLogger();

  _Buffer << GetTimeStamp();

  if(! _FullPath && Source != "")
  {
    size_t LastSlashPosition = Source.rfind('/');

    if(LastSlashPosition != Source.npos)
    {
      Source = Source.substr(LastSlashPosition + 1);
    }
  }

  switch(Type)
  {
    case LOG_CTOR:
      _Buffer << ColCya() << "[" << "CTOR" << "]   " << ColStd();
      _Buffer << "class " << ColWhi() << LogData << ColStd() << std::endl;
    break;

    case LOG_DTOR:
      _Buffer << ColCya() << "[" << "DTOR" << "]   " << ColStd();
      _Buffer << "class " << ColWhi() << LogData << ColStd() << std::endl;
    break;

    case LOG_DEBUG:
      _Buffer << ColMag() << "[" << "DEBUG" << "]  " << ColStd();
      _Buffer << "line " << ColWhi() << Source << ColStd() << " " << LogData << std::endl;
    break;

    case LOG_BLANK:
      _Buffer << std::endl;
    break;

    case LOG_INFO:
      _Buffer << ColBlu() << "[" << "INFO" << "]   " << ColStd();
      _Buffer << LogData << std::endl;
    break;

    case LOG_WARN:
      _Buffer << ColYel() << "[" << "WARN" << "]   " << ColStd();
      _Buffer << LogData << std::endl;
    break;

    case LOG_ERROR:
      _Buffer << ColRed() << "[" << "ERROR" << "]  " << ColStd();
      _Buffer << LogData << std::endl;
    break;

    case LOG_EXCEPT:
      _Buffer << ColGre() << "[" << "EXCEPT" << "] " << ColStd();
      _Buffer << LogData << std::endl;
    break;

    case LOG_SIGNAL:
      _Buffer << ColGre() << "[" << "SIGNAL" << "] " << ColStd();
      _Buffer << LogData << std::endl;
    break;

    default:
      _Buffer << ColGra() << "[" << "???" << "]    " << ColStd();
      _Buffer << LogData << std::endl;
    break;
  }

  ReleaseLogger();
}



/**
 * \brief            Begin of a new log operation
 */
void CONSOLE::InitLogger()
{
  pthread_mutex_lock(&_Lock);

  _Buffer.clear();
  _Buffer.str("");
}



/**
 * \brief            End of a new log operation
 */
void CONSOLE::ReleaseLogger()
{
  _Stream << _Buffer.str();

  pthread_mutex_unlock(&_Lock);
}



/*
std::string CONSOLE::FixLenght(std::string Text, int Lenght)
{
   if(Text.length() <= Lenght)
   {
     return Text + std::string(Lenght - Text.length(), ' ');
   }
   else
   {
     return Text.substr(Lenght - 3) + "...";
   }
}
*/



/**
 * \brief            Get the current timestamp
 *
 * \return           Current timestamp
 */
std::string CONSOLE::GetTimeStamp()
{
  struct timespec TimeValue;

  std::ostringstream Timestamp;

  clock_gettime(CLOCK_REALTIME, &TimeValue);

  Timestamp << "T:";
  Timestamp << TimeValue.tv_sec;
  Timestamp << ".";
  Timestamp << TimeValue.tv_nsec;
  Timestamp << " ";

  return Timestamp.str();
}



/**
 * \brief            Terminal on the fly configuration (standard display)
 *
 * \return           ANSI escape sequence if colored mode enabled
 * \return           The "" string if colored mode disabled
 */
const char* CONSOLE::ColStd() { return (_Colors ? ESC_STD : ""); }



/**
 * \brief            Terminal on the fly configuration (gray display)
 *
 * \return           ANSI escape sequence if colored mode enabled
 * \return           The "" string if colored mode disabled
 */
const char* CONSOLE::ColGra() { return (_Colors ? ESC_GRA : ""); }



/**
 * \brief            Terminal on the fly configuration (red display)
 *
 * \return           ANSI escape sequence if colored mode enabled
 * \return           The "" string if colored mode disabled
 */
const char* CONSOLE::ColRed() { return (_Colors ? ESC_RED : ""); }



/**
 * \brief            Terminal on the fly configuration (green display)
 *
 * \return           ANSI escape sequence if colored mode enabled
 * \return           The "" string if colored mode disabled
 */
const char* CONSOLE::ColGre() { return (_Colors ? ESC_GRE : ""); }



/**
 * \brief            Terminal on the fly configuration (yellow display)
 *
 * \return           ANSI escape sequence if colored mode enabled
 * \return           The "" string if colored mode disabled
 */
const char* CONSOLE::ColYel() { return (_Colors ? ESC_YEL : ""); }



/**
 * \brief            Terminal on the fly configuration (blue display)
 *
 * \return           ANSI escape sequence if colored mode enabled
 * \return           The "" string if colored mode disabled
 */
const char* CONSOLE::ColBlu() { return (_Colors ? ESC_BLU : ""); }



/**
 * \brief            Terminal on the fly configuration (magenta display)
 *
 * \return           ANSI escape sequence if colored mode enabled
 * \return           The "" string if colored mode disabled
 */
const char* CONSOLE::ColMag() { return (_Colors ? ESC_MAG : ""); }



/**
 * \brief            Terminal on the fly configuration (cyan display)
 *
 * \return           ANSI escape sequence if colored mode enabled
 * \return           The "" string if colored mode disabled
 */
const char* CONSOLE::ColCya() { return (_Colors ? ESC_CYA : ""); }



/**
 * \brief            Terminal on the fly configuration (white display)
 *
 * \return           ANSI escape sequence if colored mode enabled
 * \return           The "" string if colored mode disabled
 */
const char* CONSOLE::ColWhi() { return (_Colors ? ESC_WHI : ""); }
