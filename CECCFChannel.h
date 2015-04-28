


#ifndef CECCFCHANNEL_H
#define CECCFCHANNEL_H


#include <stdint.h>
#include "CBuffer.h"
#include "BCFConstants.h"
//#include <stdio.h>

/// This class creates BCF frames from MSCF data and vice versa for one channel

class CECCFChannel {
  public:
    /// @param[in] p_cdi CDI of the communication channel
    CECCFChannel(uint32_t p_cdi);

    /// @param[in] p_buffer buffer to send
    void SendBuffer(CBuffer& p_buffer);

    /// this method takes BCF messages and calls a callback method if a complete
    /// mscf message is available.
    ///@param[in] p_buffer which contains a complete BCF message
    void ExtractBCF(CBuffer& p_buffer);

  private:

    /// @param[in] p_buffer source buffer
    /// @param[in] p_OutBuffer destination buffer
    /// @return if there are more data in p_buffer to be written to OutBuffer
    bool CreateECCFs(CBuffer& p_buffer, CBuffer& p_OutBuffer);

    /// unpack the recombination buffer
    void UnpackECCFs(void);

    /// @return the node id of this object
    uint8_t GetNode(){return ((m_cdi >> 8) & 0xFF);}

    /// @return the device id of this object
    uint8_t GetDevice(){return (m_cdi & 0xFF);}

    /// this method always swaps the endiannes in contrast to the ntoh i.e.
    /// @param[in] p_val value
    /// @return p_val with swaped endiannes
    uint16_t SwapEndiann(uint16_t p_val) {return (((p_val >> 8) & 0xFF) | ((p_val << 8) & 0xFF00));}

    void SendToSocket(CBuffer& p_SocketData);

    uint32_t m_cdi;
    bool     m_connected;
    uint8_t  m_ChannelType;
    uint8_t  m_ECCFframing;

    CBuffer  m_ECCFRecoBuf;
    CBuffer  m_MSCFRecoBuf;

};



CECCFChannel::CECCFChannel(uint32_t p_cdi) :
 m_cdi(p_cdi) , m_connected(false), m_ChannelType(ECCFTypeMSCFLo), m_ECCFframing(0) {}


void CECCFChannel::UnpackECCFs(void) {
  while (m_ECCFRecoBuf.GetRemaining() > MSCFHeaderLength) {
    uint8_t eccflen = m_ECCFRecoBuf.GetUint8();
    uint8_t eccfFraming = m_ECCFRecoBuf.GetUint8();
    if (eccfFraming & CommFrameStartFlag) {
      m_MSCFRecoBuf.Clear();
    }
    (void) m_ECCFRecoBuf.GetUint8(); // type
    (void) m_ECCFRecoBuf.GetUint8(); // nodeid
    (void) m_ECCFRecoBuf.GetUint8(); // device id
    while (eccflen-- > 0) {
      m_MSCFRecoBuf.PutUint8(m_ECCFRecoBuf.GetUint8());
    }
    if (eccfFraming & CommFrameEndFlag) {
      m_MSCFRecoBuf.dump();
    }
  }
}

void CECCFChannel::ExtractBCF(CBuffer& p_buffer) {
  (void) p_buffer.GetUint8();  // ignore type for now
  uint16_t bcflen = SwapEndiann(p_buffer.GetUint16()); // read length
  uint8_t bcfFram = p_buffer.GetUint8(); // BCF Framing
  if ((bcfFram & CommFrameStartFlag) == CommFrameStartFlag) {
    m_ECCFRecoBuf.Clear();
  }
  printf("bcflen %i bcffrm %x \n",bcflen, (uint32_t) bcfFram);
  while (bcflen-- > 0) {
    m_ECCFRecoBuf.PutUint8(p_buffer.GetUint8());
  }
  if (bcfFram & CommFrameEndFlag) {
    printf("unpack eccf \n");
    UnpackECCFs();
  }
}


bool CECCFChannel::CreateECCFs(CBuffer& p_buffer, CBuffer& p_OutBuffer){
  uint8_t nxtpkglen = 0;
  if (p_buffer.IsReadPointerZero()) m_ECCFframing = CommFrameStartFlag;
  while ( 0 != ( nxtpkglen = p_buffer.GetRemaining())) {
    //p_OutBuffer.dump();
    if ( ECCFMaxLength < nxtpkglen ) {
      nxtpkglen = ECCFMaxLength;
    } else {
      m_ECCFframing |= CommFrameEndFlag;
    }

    uint32_t spaceleftOut = BCFMaxLength - p_OutBuffer.GetRemaining();
    //printf("nxt %i spcotu %i\n",nxtpkglen, spaceleftOut);
    if ( nxtpkglen <= spaceleftOut ) {
      p_OutBuffer.PutUint8(nxtpkglen);
      p_OutBuffer.PutUint8(m_ECCFframing);
      p_OutBuffer.PutUint8(m_ChannelType);
      p_OutBuffer.PutUint8(GetNode());
      p_OutBuffer.PutUint8(GetDevice());
      while(nxtpkglen--) {
        p_OutBuffer.PutUint8(p_buffer.GetUint8());
      }
      m_ECCFframing = (m_ECCFframing + 1) & 0xF;
    } else {
      return true;
    }
  }
  return false;
}


void CECCFChannel::SendBuffer(CBuffer& p_buffer) {
  CBuffer OutBuffer;
  bool moredata = false;
  uint8_t BCFframing = CommFrameStartFlag;
  do {
    OutBuffer.Clear();
    OutBuffer.PutUint32(0); // reserve space
    if (false == (moredata = CreateECCFs(p_buffer,OutBuffer))) {
      BCFframing |= CommFrameEndFlag;
    }
    OutBuffer.PutUint8(BCFType,BCFPosType);
    OutBuffer.PutUint16(SwapEndiann(OutBuffer.GetRemaining() - BCFHeaderLength),BCFPosLength); // write BCI Framelength to buffer
    OutBuffer.PutUint8(BCFframing,BCFPosFraming);
    // send OutBuffer to socket
    //OutBuffer.dump();
    printf("decode\n");
    //ExtractBCF(OutBuffer); // just testing

    BCFframing = (BCFframing + 1) & 0xF;
  } while(moredata);
}



#endif
