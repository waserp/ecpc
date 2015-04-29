


#ifndef BCFCONSTANTS_H
#define BCFCONSTANTS_H

const uint8_t  CommFrameStartFlag = 0x80;
const uint8_t  CommFrameEndFlag = 0x40;
const uint32_t BCFMaxLength = 128;
const uint32_t BCFHeaderLength = 4;
const uint8_t  ECCFMaxLength = 14;

const uint8_t  ECCFHDRLEN = 5;

const uint8_t  ECCFTypeMSCFLo = 2;
const uint8_t  ECCFTypeMSCFHi = 3;

const uint32_t BCFPosLength = 1;
const uint32_t BCFPosType = 0;
const uint32_t BCFPosFraming = 3;

const uint32_t BCFLengthSize = 2;

const uint8_t  BCFType = 0;

const uint32_t MSCFHeaderLength = 5;

const uint32_t BCFChunckSize = 403;

const uint32_t BCFPadding = 0;

/// RTC3 WelcomMessage Constants


#endif // BCFCONSTANTS_H

