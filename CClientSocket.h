



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
    /// destructor
    ~CClientSocket(void);
    /// @return the filedescriptor
    virtual int Getfd(void) { return m_SocketFD; }
    /// This callback is called when the corresponding socket
    /// has data to read.
    virtual void DataToRead(void);
    /// Open the socket
    void Open(void);
    /// Close the socket
    void Close(void);

    /// @param[in] p_Buffer buffer with data to send
    void Send(CBuffer& p_Buffer);
  private :

    bool ReadnBytes(uint32_t p_n);

    uint32_t ReadLength();      /// reads big endian!


    int         m_SocketFD;     /// File descriptor
    sockaddr_un m_remote;       /// the socket address
    bool        m_isopen;       /// is the socket connected
    uint32_t    m_HeaderLength; /// number of bytes in header
    uint32_t    m_LengthPos;    /// first byte of length
    uint32_t    m_LengthSize;   /// number of length bytes

    CBuffer     m_RecBuffer;    /// Receive Buffer
};

CClientSocket::CClientSocket(void) : m_isopen(false), m_State(ERSinactive) {
  m_RecBuffer.Clear();
}

CClientSocket::~CClientSocket(void) {
  if (m_isopen) Close();
}

void CClientSocket::Open(void) {
  assert((m_SocketFD = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0)) != -1);  // create Socket FD
  memset(&m_remote, 0, sizeof(m_remote));               // clean address structure
  m_remote.sun_family = AF_UNIX;
  snprintf(m_remote.sun_path, sizeof(m_remote.sun_path), "/tmp/BCI.sock");
  if(connect(m_SocketFD,(struct sockaddr *) &m_remote, sizeof(m_remote)) != 0) {
    printf("connect() failed\n");
    return;
  }
  m_isopen = true;
}

bool CClientSocket::ReadnBytes(uint32_t p_n)
{
  uint8_t* dptr = m_RecBuffer.GetDataPointerWritePos();
  int32_t nbytes = read(m_SocketFD, dptr, p_n);
  if (nbytes < 0) {
    // throw
  }
  if (nbytes == p_n) return true;
  return false;
}

uint32_t CClientSocket::ReadLength()  /// reads big endian!
{
  uint32_t val = 0;
  for (uint32_t pos = m_LengthPos; pos < (m_LengthPos + m_LengthSize); pos++) {
    val = val << 8;
    val += m_RecBuffer.GetUint8(pos);
  }
  return val;
}

void CClientSocket::DataToRead(void) {
  if ((uint32_t curcnt = m_RecBuffer.GetRemaining()) < m_HeaderLength) {
    if (ReadnBytes(m_HeaderLength - curcnt) == false) return;
  }
  if (ReadnBytes(ReadLength() - m_RecBuffer.GetRemaining()) == false) return;
  /// process data in buffer
  m_RecBuffer.Clear();
}

void CClientSocket::Send(CBuffer& p_Buffer) {
  write(m_SocketFD, p_Buffer.GetDataPointer(), p_Buffer.GetRemaining());
}

void CClientSocket::Close(void) {
  close(m_SocketFD);
}


#endif  // CCLIENTSOCKET_H


