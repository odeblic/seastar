/**
 * \file console.h
 *
 * \brief Header for console use
 *
 * \author Olivier de BLIC
 */



#ifndef CONSOLE_H
#define CONSOLE_H

// Standard headers
#include <pthread.h>
#include <string>
#include <sstream>

// Project headers
#include "exception.h"

// Source line reference format
#define SSTR(NB)                  #NB
#define STR(MSG)                  SSTR(MSG)
#define SOURCE_LINE               __FILE__ ":" STR(__LINE__)



/// Enumeration of log types
typedef enum
{
  LOG_CTOR,
  LOG_DTOR,
  LOG_BLANK,
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_EXCEPT,
  LOG_SIGNAL,
} LOG_TYPE;



/**
 * \brief Interface for console output and formated logs
 */
class CONSOLE
{
public:

  /**
   * \brief          Console constructor
   */
  CONSOLE();



  /**
   * \brief          Console destructor
   */
  ~CONSOLE();



  /**
   * \brief          Configuration setter for colored mode
   *
   * \param Enabled  true for enable, false for disable
   */
  void SetColors(bool Enabled);



  /**
   * \brief          Configuration setter for verbose mode
   *
   * \param Enabled  true for enable, false for disable
   */
  void SetVerbose(bool Enabled);



  /**
   * \brief          Configuration setter for fullpath display mode
   *
   * \param Enabled  true for enable, false for disable
   */
  void SetFullPath(bool Enabled);



#ifdef DEBUG
  /**
   * \brief            Text log (debug only)
   *
   * \param Text       Text to log
   * \param Source     Source line from where the function is call
   */
  void LogText(std::string Text, std::string Source = "");



  /**
   * \brief            Number log (debug only)
   *
   * \param Number     Number to log
   * \param Source     Source line from where the function is call
   */
  template<typename T>
  void LogNumber(T Number, std::string Desc = "", std::string Source = "");



  /**
   * \brief            Constructor call log (debug only)
   *
   * \param ClassName  Name of the constructed class
   * \param Source     Source line from where the function is call
   */
  void LogCtor(std::string ClassName, std::string Source = "");



  /**
   * \brief            Destructor call log (debug only)
   *
   * \param ClassName  Name of the destroyed class
   * \param Source     Source line from where the function is call
   */
  void LogDtor(std::string ClassName, std::string Source = "");



  /**
   * \brief            Pointer log (debug only)
   *
   * \param Pointer    Pointer to log
   * \param Source     Source line from where the function is call
   */
  template<typename T>
  void LogPointer(T* Pointer, std::string Source = "");



  /**
   * \brief            Symbol log (debug only)
   *
   * \param Addr       Symbol address to log
   * \param Source     Source line from where the function is call
   */
  void LogSymbol(void* Addr, std::string Source = "");
#endif



  /**
   * \brief            Blank log
   */
  void LogBlank();



  /**
   * \brief            Information log
   *
   * \param Msg        Information message
   * \param Source     Source line from where the function is call
   */
  void LogInfo(std::string Msg, std::string Source = "");



  /**
   * \brief            Warning log
   *
   * \param Msg        Warning message
   * \param Source     Source line from where the function is call
   */
  void LogWarn(std::string Msg, std::string Source = "");



  /**
   * \brief            Error log
   *
   * \param Msg        Error message
   * \param Source     Source line from where the function is call
   */
  void LogError(std::string Msg, std::string Source = "");



  /**
   * \brief            Exception occurrence log
   *
   * \param Exception  Exception object
   * \param Source     Source line from where the function is call
   */
  void LogExcept(EXCEPTION Exception, std::string Source = "");



  /**
   * \brief            Signal occurrence log
   *
   * \param SigNum     Signal number
   */
  void LogSignal(int SigNum);



  /**
   * \brief            Print the splashscreen at start
   */
  void PrintSplashScreen();



  /**
   * \brief            Print the help
   */
  void PrintHelp();



  /**
   * \brief            Signal occurrence log
   *
   * \param Type       Type of log
   * \param LogData    Undefined
   * \param Source     Source line from where the function is call
   */
  void PrintLogLine(LOG_TYPE Type, std::string LogData = "", std::string Source = "");



private:

  /**
   * \brief            Begin of a new log operation
   */
  void InitLogger();



  /**
   * \brief            End of a new log operation
   */
  void ReleaseLogger();



  //std::string FixLenght(std::string Text, int Lenght);



  /**
   * \brief            Get the current timestamp
   *
   * \return           Current timestamp
   */
  std::string GetTimeStamp();



  /**
   * \brief            Terminal on the fly configuration (standard display)
   *
   * \return           ANSI escape sequence if colored mode enabled
   * \return           The "" string if colored mode disabled
   */
  const char* ColStd();



  /**
   * \brief            Terminal on the fly configuration (gray display)
   *
   * \return           ANSI escape sequence if colored mode enabled
   * \return           The "" string if colored mode disabled
   */
  const char* ColGra();



  /**
   * \brief            Terminal on the fly configuration (red display)
   *
   * \return           ANSI escape sequence if colored mode enabled
   * \return           The "" string if colored mode disabled
   */
  const char* ColRed();



  /**
   * \brief            Terminal on the fly configuration (green display)
   *
   * \return           ANSI escape sequence if colored mode enabled
   * \return           The "" string if colored mode disabled
   */
  const char* ColGre();



  /**
   * \brief            Terminal on the fly configuration (yellow display)
   *
   * \return           ANSI escape sequence if colored mode enabled
   * \return           The "" string if colored mode disabled
   */
  const char* ColYel();



  /**
   * \brief            Terminal on the fly configuration (blue display)
   *
   * \return           ANSI escape sequence if colored mode enabled
   * \return           The "" string if colored mode disabled
   */
  const char* ColBlu();



  /**
   * \brief            Terminal on the fly configuration (magenta display)
   *
   * \return           ANSI escape sequence if colored mode enabled
   * \return           The "" string if colored mode disabled
   */
  const char* ColMag();



  /**
   * \brief            Terminal on the fly configuration (cyan display)
   *
   * \return           ANSI escape sequence if colored mode enabled
   * \return           The "" string if colored mode disabled
   */
  const char* ColCya();



  /**
   * \brief            Terminal on the fly configuration (white display)
   *
   * \return           ANSI escape sequence if colored mode enabled
   * \return           The "" string if colored mode disabled
   */
  const char* ColWhi();



private:

  /// Mutex to prevent of lines cutting due to multi-threading
  pthread_mutex_t _Lock;

  /// Flag for color mode
  bool _Colors;

  /// Flag for verbose mode
  bool _Verbose;

  /// Flag for fullpath mode
  bool _FullPath;

  /// Buffer for log line construction
  std::ostringstream  _Buffer;

  /// Stream for log line printing
  std::ostream&       _Stream;
};



#endif
