#ifndef __CRC_H__
#define __CRC_H__
#include "SocketLibCommonDef.h"

//获得一段数据的CRC16
INLINE unsigned short GetCRC16(const unsigned char* pData, unsigned short nLength, unsigned short fcs = 0xFFFF);

//获得一段数据的CRC32
INLINE unsigned int GetCRC32(unsigned char* pData, unsigned int len, unsigned int fcs = 0xFFFFFFFF);



#endif//__CRC_H__

