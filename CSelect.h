


#ifndef CSELECT_H
#define CSELECT_H
#include <stdint.h>

#include <sys/select.h>

#include <set>

enum EWaitFor {
  EWFDataToRead,   /// Wait for data to read in the socket
  EWFDataToWrite,  /// Wait the socket to become ready to write some data to
  EWFException     /// Wait for an exception
};

class Iselect {
  public :
    /// @return the filedescriptor
    virtual uint32_t Getfd(void) = 0;

    /// This callback is called when the corresponding socket
    /// has data to read.
    virtual void DataToRead(void) = 0;
};

class Cselect {
  public :
    Cselect();

    /// This method has to run in its own thread
    /// callbacks are executed on this thread
    void SelectThread(void);

    /// Register the Filedescriptor to be watched
    void Register(Iselect* p_Consumer, EWaitFor p_SockType);

  private :

    void maxNdfs(uint32_t p_val) { if (p_val > m_ndfs) m_ndfs = p_val + 1;}
    fd_set             m_rdfds;  // Read fds
    //fd_set           m_wrfds;  // Write fds
    uint32_t           m_ndfs;   // max of all fds plus one
    std::set<Iselect*> m_IfcSet; // a set of consumers
    bool               m_abort;  // abort flag
};

Cselect::Cselect() : m_abort(false) {

//  FD_ZERO(&m_wrfds);
}

void Cselect::Register(Iselect* p_Consumer, EWaitFor p_SockType) {
  switch (p_SockType) {
    case EWFDataToRead : m_IfcSet.insert(p_Consumer);  break;
    //case EWFDataToWrite : FD_SET(fd, &m_wrfds); break;
    default : break;
  }
}

void Cselect::SelectThread() {
  while (!m_abort) {
    struct timeval tv;
    m_ndfs = 0;
    FD_ZERO(&m_rdfds);
    for (auto& Ifc : m_IfcSet) {
      maxNdfs(Ifc->Getfd());
      FD_SET(Ifc->Getfd(), &m_rdfds);
    }
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    select(m_ndfs, &m_rdfds, NULL, NULL, &tv);
    for (auto& Ifc : m_IfcSet) {
      if (FD_ISSET(Ifc->Getfd(), &m_rdfds)) {
        Ifc->DataToRead();
      }
    }
  }
}



#endif // CSELECT_H

