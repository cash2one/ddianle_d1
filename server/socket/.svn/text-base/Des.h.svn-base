#ifndef __DES_H__
#define __DES_H__

#include "commondefine.h"

enum    {ENCRYPT,DECRYPT};

// Type—ENCRYPT:加密,DECRYPT:解密
// 输出缓冲区(Out)的长度 >= ((datalen+7)/8)*8,即比datalen大的且是8的倍数的最小正整数
// In 可以= Out,此时加/解密后将覆盖输入缓冲区(In)的内容
// 当keylen>8时系统自动使用3次DES加/解密,否则使用标准DES加/解密.超过16字节后只取前16字节

bool Des_Go(char *Out,char *In,long datalen,const char *Key,int keylen,bool Type = ENCRYPT);


INLINE unsigned int EncryptProcDES(char* pData, unsigned int nSize, char* pKey, unsigned int nSizeKey);
INLINE unsigned int DecryptProcDES(char* pData, unsigned int nSize, char* pKey, unsigned int nSizeKey);

//////////////////////////////////////////////////////////////////////////

#endif//__DES_H__

