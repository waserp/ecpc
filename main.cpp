#include <iostream>
#include <stdio.h>
using namespace std;

#include "CECCFChannel.h"
#include "CSelect.h"
const uint32_t cdi = 0x101605;


CBuffer buf;

int main()
{
    CECCFChannel chan(cdi);

    buf.PutUint32(0x12345678);
    chan.SendBuffer(buf);
    printf("nr 2 ------------------------------------------------------------------------------\n\n\n");
    buf.Clear();
    for (uint16_t i = 0; i< 123; i++ ) {
      buf.PutUint8((i & 0x0F) | 0xA0);
    }
    chan.SendBuffer(buf);
    cout << "Hello world!" << endl;
    return 0;
}
