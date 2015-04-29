#include <iostream>
#include <stdio.h>
using namespace std;

#include "CECCFChannel.h"
#include "CSelect.h"
#include "CClientSocket.h"

const uint32_t cdi = 0x100000;


CBuffer buf;
CClientSocket Sock;


int main()
{
    CECCFChannel chan(cdi);

    //buf.PutUint32(0x12345678);
    buf.PutUint8(0xFF);
    buf.PutUint8(0x1);
    chan.SendBuffer(buf);
    printf("nr 2 ------------------------------------------------------------------------------\n\n\n");
    buf.Clear();

    buf.PutUint8(0xFF);
    buf.PutUint8(0x2);
    chan.SendBuffer(buf);
    printf("nr 2 ------------------------------------------------------------------------------\n\n\n");
    buf.Clear();

    sleep(1);
    return 0;
    for (uint16_t i = 0; i< 123; i++ ) {
      buf.PutUint8((i & 0x0F) | 0xA0);
    }
    chan.SendBuffer(buf);
    cout << "Hello world!" << endl;


    Sock.Open(0x100000,CClientSocket::eSTBCI);
    Sock.Close();

    return 0;
}
