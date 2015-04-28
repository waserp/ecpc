



#ifndef CCLIENTSOCKET_H
#define CCLIENTSOCKET_H

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

class CClientSocket {
  public :
    /// constructor
    CClientSocket(void);
  private :
    int         m_SocketFD;
    sockaddr_un m_remote;
};

CClientSocket::CClientSocket(void) {

}

CClientSocket::Open(void) {
  /// create Socket FD
  if ((m_SocketFD = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        //perror("socket");

  }
}


#endif  // CCLIENTSOCKET_H


