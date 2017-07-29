/**
 * \file socket.h
 *
 * \brief Header for sockets use
 *
 * \author Olivier de BLIC
 */



#ifndef SOCKET_H
#define SOCKET_H

// Standard headers
#include <sys/socket.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Project headers
#include "object.h"



/**
 * \brief Wrapper for sockets use in C++
 */
class SOCKET : public OBJECT
{
public:



  /**
   * \brief          Socket constructor
   */
  SOCKET();



  /**
   * \brief          Socket destructor
   */
  virtual ~SOCKET();



  /**
   * \brief          Local IP address getter
   *
   * \return         Local IP address
   */
  std::string GetLocalAddr() const;



  /**
   * \brief          Remote IP address getter
   *
   * \return         Remote IP address
   */
  std::string GetRemoteAddr() const;



  /**
   * \brief          Binds the socket to local address (server mode)
   *
   * \param PortNum  Local port number
   */
  void Bind(unsigned short PortNum);



  /**
   * \brief          Listens the socket (server mode)
   */
  void Listen();



  /**
   * \brief          Connect the socket to remote address (client mode)
   *
   * \param IpAddr   Remote IP address
   *
   * \param PortNum  Remote port number
   */
  void Connect(std::string IpAddr, unsigned short PortNum);



  /**
   * \brief          Accepts a connection (server mode)
   *
   * \return         Accepted socket
   */
  SOCKET& Accept();


  /**
   * \brief          Tells if the socket is connected
   *
   * \return         \b true if connected
   * \return         \b false if not connected
   */
  bool IsConnected();



  /**
   * \brief          Tells if data is waiting in the buffer
   *
   * \return         \b true if data is waiting
   * \return         \b false if no data is waiting
   */
  bool IsDataWaiting();



  bool WaitData();



  /**
   * \brief          Sends data to socket
   *
   * \param Data     Data to send
   */
  void Send(const std::string& Data);



  /**
   * \brief          Sends data to socket
   *
   * \return         Received data
   */
  std::string Receive();



  /**
   * \brief          Socket identifier getter
   *
   * \return         Socket identifier
   */
  int GetId() const;



private:

  /**
   * \brief          Socket constructor (reserved for accept method)
   *
   * \param SocketId Already existing socket identifier (opened with accept)
   */
  SOCKET(int SocketId);



  /// Socket identifier
  int  _SocketId;
};



#endif
