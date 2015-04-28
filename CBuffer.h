//$Id:  $
//**************************************************************************************************
// (C) Copyright Roche Diagnostics Ltd., 2008 - 2014
//**************************************************************************************************
//  Location:
//  $HeadURL:  $
//**************************************************************************************************
//  Last Update:
//  $LastChangedBy:  $
//  $LastChangedDate: $
//  $LastChangedRevision: $
//**************************************************************************************************


#ifndef CBuffer_h
#define CBuffer_h

#include <stdint.h>

#define DEBUGFUN

const uint32_t ecpcbuffersize = 300;

class CBuffer
{
  public :
    CBuffer();
    CBuffer(const uint8_t* data, uint32_t count);
    ~CBuffer();
    /// put byte into buffer
    /// @param[in] p_b byte to write
    void PutUint8(uint8_t p_b);
    /// put byte into buffer at given position
    /// @param[in] p_b byte to write
    /// @param[in] p_pos position
    void PutUint8(uint8_t p_b, uint32_t p_pos);
    /// put word into buffer
    /// @param[in] p_w word to write
    void PutUint16(uint16_t p_w);
     /// put word into buffer at given position
    /// @param[in] p_w word to write
    /// @param[in] p_pos position
    void PutUint16(uint16_t p_w, uint32_t p_pos);
    /// put long word into buffer
    /// @param[in] p_b long word to write
    void PutUint32(uint32_t p_l);
    /// put float into buffer
    /// @param[in] p_f to write
    void PutFloat(float p_f);
    /// read byte from buffer
    /// @return byte read
    uint8_t GetUint8();
    /// read word from buffer
    /// @return word read
    uint16_t GetUint16();
    /// read long word from buffer
    /// @return long word read
    uint32_t GetUint32();
    /// read single precision floating point from buffer
    /// @return the float read
    float GetFloat();
    /// dump the buffer content to std::out, debug only
    void dump();
    /// dump the pointers to std::out, debut only
    void dumpptr();
    /// Get number of bytes in buffer not yet read
    /// @return number of remaining bytes
    uint32_t GetRemaining();
    /// rewinds readpointer to begin of message
    void Rewind();
    /// clear buffer
    void Clear();
    /// @return true if buffer was overread and false if not
    bool GetOverReadFlag();
    /// @return if readpointer is at begin
    bool IsReadPointerZero();

    /// @return pointer to Data
    uint8_t* GetDataPointer();

    /// pointer to the next
    CBuffer* m_next;
    CBuffer* m_prev;
    CBuffer* m_nextcon;

    /// creator of the buffer
    uint32_t m_creator;
    /// current owner
    uint32_t m_owner;
    /// sender
    uint32_t m_sender;
    /// receiver
    uint32_t m_receiver;
    /// message Type
    uint32_t m_type;
  private:
    /// is true if read more data than available
    bool   m_OverRead;
	/// is true if tried to write beond buffer size
	bool   m_Overflow;
    /// read pointer, points to the next byte to read
    uint32_t m_rp;
    /// write pointer, points to the next byte to write to
    uint32_t m_wp;
    /// memory to store data
    uint8_t  m_data[ecpcbuffersize];
};

#endif

