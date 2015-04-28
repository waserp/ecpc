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


#include <iostream>
#include "CBuffer.h"

using namespace std;

CBuffer::CBuffer()
{
  m_rp = 0;
  m_wp = 0;
  m_OverRead = false;
  m_Overflow = false;
  m_creator = 0;
  m_owner = 0;
}

CBuffer::CBuffer(const uint8_t* data, uint32_t count)
{
  m_rp = 0;
  m_wp = 0;
  m_OverRead = false;
  m_Overflow = false;
  m_creator = 0;
  m_owner = 0;
  while(count--) {
    PutUint8(*data++);
  }
}

CBuffer::~CBuffer()
{
}

void CBuffer::PutUint8(uint8_t p_b)
{
 if (m_wp >= ecpcbuffersize) {
	 m_Overflow = true;
	 return;
 }
 m_data[m_wp++]=p_b;
}

void CBuffer::PutUint8(uint8_t p_b, uint32_t p_pos)
{
 if (p_pos >= ecpcbuffersize) {
	 m_Overflow = true;
	 return;
 }
 if (p_pos > m_wp) {
   m_wp = p_pos + 1;
 }
 m_data[p_pos]=p_b;
}

void CBuffer::PutUint16(uint16_t p_w, uint32_t p_pos)
{
  PutUint8(p_w & 0xFF, p_pos);
  PutUint8((p_w >> 8) & 0xFF, (p_pos + 1));
}

void CBuffer::PutUint16(uint16_t p_w)
{
  PutUint8(p_w & 0xFF);
  PutUint8((p_w >> 8) & 0xFF);
}

void CBuffer::PutUint32(uint32_t p_l)
{
  PutUint16(p_l & 0xFFFF);
  PutUint16((p_l >> 16) & 0xFFFF);
}

void CBuffer::PutFloat(float p_f)
{
  union { float f; uint32_t ui;} v;
  v.f = p_f;
  PutUint32(v.ui);
}

uint8_t CBuffer::GetUint8()
{
  if (m_rp<m_wp) {
    return m_data[m_rp++];
  }
  m_OverRead = true;
  return 0;
}

uint16_t CBuffer::GetUint16()
{
  return (GetUint8() + (GetUint8() << 8));
}

uint32_t CBuffer::GetUint32(void)
{
  return (GetUint8() + (GetUint8() << 8) + (GetUint8() << 16) + (GetUint8() << 24));
}

float CBuffer::GetFloat(void)
{
  union { float f; uint32_t ui;} v;
  v.ui = GetUint32();
  return v.f;
}

#ifdef DEBUGFUN
void CBuffer::dump()
{
  uint32_t i;
  cout << "dump buffer: " << hex;
  for (i=0;i<m_wp;i++) {
    cout << " " << (unsigned int) m_data[i];
  }
  cout << endl;
}

void CBuffer::dumpptr()
{
//  cout <<  hex << "this " << ((int) this) << " next : " << ((int) m_next) << " prev " << ((int)   m_prev) << endl;
}
#endif

uint32_t CBuffer::GetRemaining()
{
  return (m_wp-m_rp);
}

void CBuffer::Rewind()
{
  m_rp = 0;
}

bool CBuffer::GetOverReadFlag()
{
  return m_OverRead;
}

void CBuffer::Clear()
{
  m_rp=0;
  m_wp=0;
  m_OverRead = false;
}

bool CBuffer::IsReadPointerZero()
{
  return (m_rp == 0);
}

