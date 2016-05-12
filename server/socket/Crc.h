#ifndef __CRC_H__
#define __CRC_H__
#include "SocketLibCommonDef.h"

//���һ�����ݵ�CRC16
INLINE unsigned short GetCRC16(const unsigned char* pData, unsigned short nLength, unsigned short fcs = 0xFFFF);

//���һ�����ݵ�CRC32
INLINE unsigned int GetCRC32(unsigned char* pData, unsigned int len, unsigned int fcs = 0xFFFFFFFF);



#endif//__CRC_H__

