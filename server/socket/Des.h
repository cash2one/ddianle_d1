#ifndef __DES_H__
#define __DES_H__

#include "commondefine.h"

enum    {ENCRYPT,DECRYPT};

// Type��ENCRYPT:����,DECRYPT:����
// ���������(Out)�ĳ��� >= ((datalen+7)/8)*8,����datalen�������8�ı�������С������
// In ����= Out,��ʱ��/���ܺ󽫸������뻺����(In)������
// ��keylen>8ʱϵͳ�Զ�ʹ��3��DES��/����,����ʹ�ñ�׼DES��/����.����16�ֽں�ֻȡǰ16�ֽ�

bool Des_Go(char *Out,char *In,long datalen,const char *Key,int keylen,bool Type = ENCRYPT);


INLINE unsigned int EncryptProcDES(char* pData, unsigned int nSize, char* pKey, unsigned int nSizeKey);
INLINE unsigned int DecryptProcDES(char* pData, unsigned int nSize, char* pKey, unsigned int nSizeKey);

//////////////////////////////////////////////////////////////////////////

#endif//__DES_H__

