



#ifndef CCLIENTSOCKET_H
#define CCLIENTSOCKET_H

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "CBuffer.h"

class CClientSocket : public Iselect {
  public :
    /// constructor
    CClientSocket(void);
    ~CClientSocket(void);
    /// @return the filedescriptor
    virtual int Getfd(void) { return m_SocketFD; }
    /// This callback is called when the corresponding socket
    /// has data to read.
    virtual void DataToRead(void);
    void Open(void);
    void Close(void);
    void Send(CBuffer& p_Buffer);
  private :
    int         m_SocketFD;
    sockaddr_un m_remote;    /// the socket address
    bool        m_isopen;

};

CClientSocket::CClientSocket(void) : m_isopen(false) {

}
CClientSocket::~CClientSocket(void) {
  if (m_isopen) Close();
}
/// open the socket
void CClientSocket::Open(void) {
  assert((m_SocketFD = socket(AF_UNIX, SOCK_STREAM, 0)) != -1);  // create Socket FD
  memset(&m_remote, 0, sizeof(m_remote));               // clean address structure
  m_remote.sun_family = AF_UNIX;
  snprintf(m_remote.sun_path, sizeof(m_remote.sun_path), "/tmp/BCI.sock");
  if(connect(m_SocketFD,(struct sockaddr *) &m_remote, sizeof(m_remote)) != 0) {
    printf("connect() failed\n");
    return;
  }
  m_isopen = true;
}

void CClientSocket::DataToRead(void) {
  //nbytes = read(socket_fd, buffer, 256);
}

void CClientSocket::Send(CBuffer& p_Buffer) {
  write(m_SocketFD, p_Buffer.GetDataPointer(), p_Buffer.GetRemaining());
}

void CClientSocket::Close(void) {
  close(m_SocketFD);
}


#endif  // CCLIENTSOCKET_H


