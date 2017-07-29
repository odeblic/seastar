/**
 * \file socket.cpp
 *
 * \brief Module for sockets use
 *
 * \author Olivier de BLIC
 */



// Standard headers
#include <socket.h>
#include <poll.h>
#include <string.h>
#include <sstream>

// Project headers
#include "socket.h"
#include "application.h"
#include "exception.h"

// Constant values



/**
 * \brief          Socket constructor
 */
SOCKET::SOCKET()
: OBJECT("SOCKET")
{
#ifdef DEBUG
  App().Console.LogCtor(_ObjName);
#endif

  _SocketId = socket(AF_INET, SOCK_STREAM, 0);

  if(_SocketId == -1)
  {
    throw EXCEPTION("Error creating socket");
  }
  else
  {
    App().Console.LogInfo("Socket now created");
  }

  int SockOption = 1;

  if((setsockopt(_SocketId, SOL_SOCKET, SO_REUSEADDR, (char*)&SockOption, sizeof(SockOption)) == -1 ) ||
     (setsockopt(_SocketId, SOL_SOCKET, SO_KEEPALIVE, (char*)&SockOption, sizeof(SockOption)) == -1 ) )
  {
    throw EXCEPTION("Error setting socket options");
  }
  else
  {
    App().Console.LogInfo("Socket options set");
  }
}



/**
 * \brief          Socket constructor (reserved for accept method)
 *
 * \param SocketId Already existing socket identifier (opened with accept)
 */
SOCKET::SOCKET(int SocketId)
: OBJECT("MANAGER"), _SocketId(SocketId)
{
#ifdef DEBUG
  App().Console.LogDtor(_ObjName);
#endif

  if(! IsConnected())
  {
    throw EXCEPTION("Error creating accepted socket");
  }
}



/**
 * \brief          Socket destructor
 */
SOCKET::~SOCKET()
{

  /// \warning A call to 'close()' indeed close the communication in only one direction

  /// @todo Use the function shutdown() instead of close()

  close(_SocketId);
}



/**
 * \brief          Local IP address getter
 *
 * \return         Local IP address
 */
std::string SOCKET::GetLocalAddr() const
{
  std::ostringstream Buffer;

  sockaddr_in SockAddr;
  socklen_t SockAddrLen = sizeof(sockaddr_in);

  if(getsockname(_SocketId, (sockaddr*)&SockAddr, &SockAddrLen) != 0)
  {
    throw EXCEPTION("Error reading socket local address");
  }

  Buffer << inet_ntoa(SockAddr.sin_addr) << ":" << SockAddr.sin_port;

  return Buffer.str();
}



/**
 * \brief          Remote IP address getter
 *
 * \return         Remote IP address
 */
std::string SOCKET::GetRemoteAddr() const
{
  std::ostringstream Buffer;

  sockaddr_in SockAddr;
  socklen_t SockAddrLen = sizeof(sockaddr_in);

  if(getpeername(_SocketId, (sockaddr*)&SockAddr, &SockAddrLen) != 0)
  {
    throw EXCEPTION("Error reading socket remote address");
  }

  Buffer << inet_ntoa(SockAddr.sin_addr) << ":" << SockAddr.sin_port;

  return Buffer.str();
}



/**
 * \brief          Binds the socket to local address (server mode)
 *
 * \param PortNum  Local port number
 */
void SOCKET::Bind(unsigned short int PortNum)
{
  struct sockaddr_in LocalAddr;

  if(PortNum < 0 || PortNum > 65535)
  {
    throw EXCEPTION("Port number out of range");
  }

  // Definition of address
  LocalAddr.sin_family = AF_INET;
  LocalAddr.sin_port = htons(PortNum);
  LocalAddr.sin_addr.s_addr = INADDR_ANY;

  // Padding with zeros
  memset(&(LocalAddr.sin_zero), 0, 8);

  if(bind(_SocketId, (sockaddr*)&LocalAddr, sizeof(LocalAddr)) == -1 )
  {
    throw EXCEPTION("Error binding socket");
  }
  else
  {
    App().Console.LogInfo("Socket now bound");
  }
}



/**
 * \brief          Listens the socket (server mode)
 */
void SOCKET::Listen()
{
  if(listen(_SocketId, 5) == -1 )
  {
    throw EXCEPTION("Impossible to listen");
  }
  else
  {
    App().Console.LogInfo("Socket now listening");
  }
}



/**
 * \brief          Connect the socket to remote address (client mode)
 *
 * \param IpAddr   Remote IP address
 *
 * \param PortNum  Remote port number
 */
void SOCKET::Connect(std::string IpAddr, unsigned short int PortNum)
{
  struct sockaddr_in RemoteAddr;

  // Definition of address
  RemoteAddr.sin_family = AF_INET;
  RemoteAddr.sin_port = htons(PortNum);
  RemoteAddr.sin_addr.s_addr = inet_addr(IpAddr.c_str());

  // Padding with zeros
  memset(&(RemoteAddr.sin_zero), 0, 8);

  if(connect(_SocketId, (struct sockaddr*)&RemoteAddr, sizeof(RemoteAddr)) < 0)
  {
    throw EXCEPTION("Impossible to connect the socket");
  }
}



/**
 * \brief          Accepts a connection (server mode)
 *
 * \return         Accepted socket
 */
SOCKET& SOCKET::Accept()
{
  int AcceptedSocketId = accept(_SocketId, NULL, NULL);

  if(AcceptedSocketId == -1)
  {
    throw EXCEPTION("Error accepting socket");
  }

  // Wrap the accepted socket in a new socket object
  SOCKET* AcceptedSocket = new SOCKET(AcceptedSocketId);

  App().Console.LogInfo("Socket accepted");

  return *AcceptedSocket;
}



/**
 * \brief          Tells if the socket is connected
 *
 * \return         \b true if connected
 * \return         \b false if not connected
 */
bool SOCKET::IsConnected()
{
  struct pollfd Checker = {_SocketId, POLLHUP | POLLERR, 0};
  //struct timespec Timer = {0, 0};

  int Ret = poll(&Checker, 1, 0);
  //int Ret = ppoll(&Checker, 1, &Timer, NULL);

  if((Ret >= 0) && ! (Checker.revents & (POLLHUP | POLLERR)))
  {
    return true;
  }
  else
  {
    return false;
  }
}



/**
 * \brief          Tells if data is waiting in the buffer
 *
 * \return         \b true if data is waiting
 * \return         \b false if no data is waiting
 */
bool SOCKET::IsDataWaiting()
{
  /// @bug poll() will not block as soon as the socket should block !

  /// @todo Should use SIGIO instead of polling ? (Busy waiting against sleeping...)

  struct pollfd Checker = {_SocketId, POLLIN, 0};
  //struct timespec Timer = {0, 0};

  int Ret = poll(&Checker, 1, 0);
  //int Ret = ppoll(&Checker, 1, &Timer, NULL);

  if((Ret >= 0) && (Checker.revents & POLLIN))
  {
    return true;
  }
  else
  {
    return false;
  }
}



bool SOCKET::WaitData()
{
  //struct fd_set Checker = {_SocketId, POLLIN | POLLHUP, 0};
  struct pollfd Checker = {_SocketId, POLLIN | POLLHUP, 0};
  struct timespec Timer = {1, 0};

  // Within one second, scan the reception buffer to get a possible new line character
  //while(Timer.tv_sec > 0 || Timer.tv_nsec > 0)
  {
    //printf(" > Looping...  s=%d, ns=%d\n", (int)(Timer.tv_sec), (int)(Timer.tv_nsec));

    int Ret = ppoll(&Checker, 1, &Timer, NULL);

    // Check if error
    if(Ret < 0)
    {
      throw EXCEPTION("Error on ppoll");
    }
    // Check if timed out
    else if(Ret == 0)
    {
      App().Console.LogInfo("Time out on ppoll", SOURCE_LINE);
      return false;
    }
    // Check if client has hangup
    else if(Checker.revents & POLLHUP)
    {
      throw EXCEPTION("Hangup on ppoll");
    }
    // Check if client sent data
    else if(Checker.revents & POLLIN)
    {
      App().Console.LogInfo("Data on ppoll", SOURCE_LINE);
      return true;
    }
    else
    {
      App().Console.LogInfo("Else", SOURCE_LINE);
    }
  }

  struct timespec Time = {1, 0};
  pthread_cond_timedwait(NULL, NULL, &Time);

  return false;
}



/**
 * \brief          Sends data to socket
 *
 * \param Data     Data to send
 */
void SOCKET::Send(const std::string& Data)
{
  ssize_t Count = send(_SocketId, Data.c_str(), Data.length(), 0);

  if(Count < 0)
  {
    throw EXCEPTION("Error sending data to socket");
  }
  else if(Count < Data.length())
  {
    throw EXCEPTION("Sending uncomplete data to socket");
  }
  else
  {
    App().Console.LogInfo("All data sent to socket");
  }
}



/**
 * \brief          Sends data to socket
 *
 * \return         Received data
 */
std::string SOCKET::Receive()
{
  char Buffer[1024];

  ssize_t Count = recv(_SocketId, Buffer, sizeof(Buffer), 0);

  if(Count < 0)
  {
    throw EXCEPTION("Error receiving data from socket");
  }
  else if(Count < sizeof(Buffer))
  {
    App().Console.LogInfo("Few data received from socket");
  }
  else
  {
    App().Console.LogInfo("All data received from socket");
  }

  return std::string((const char*)Buffer, Count);
}



/**
 * \brief          Socket identifier getter
 *
 * \return         Socket identifier
 */
int SOCKET::GetId() const
{
  return _SocketId;
}
