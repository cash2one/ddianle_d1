#ifndef __CRYPT_H__
#define __CRYPT_H__

#include <memory.h>
#include <string.h>

enum    {_OP_ENCRYPT, _OP_DECRYPT};

unsigned int EncryptProcXOR8(char* pData, unsigned int nSize, char* pKey, unsigned int nSizeKey);
unsigned int DecryptProcXOR8(char* pData, unsigned int nSize, char* pKey, unsigned int nSizeKey);
unsigned int EncryptProcXOR16(char* pData, unsigned int nSize, char* pKey, unsigned int nSizeKey);
unsigned int DecryptProcXOR16(char* pData, unsigned int nSize, char* pKey, unsigned int nSizeKey);
unsigned int EncryptProcXOR32(char* pData, unsigned int nSize, char* pKey, unsigned int nSizeKey);
unsigned int DecryptProcXOR32(char* pData, unsigned int nSize, char* pKey, unsigned int nSizeKey);


#define CryptKeyLen		16

class CCryptKey
{
public:
	//������Կ
	void SetKey(const char* pKey, unsigned int nSizeKey, unsigned int nStartPos = 0){for(unsigned int i = 0; i + nStartPos < CryptKeyLen && i < nSizeKey; i++)key[i+nStartPos] = pKey[i];};
	//������Կ
	void Update(){for(int i = 0; i < CryptKeyLen; i++)key[i] += (char)i;};
	//�õ���Կ
	char* GetKey(){return key;};
protected:
	char key[CryptKeyLen];
};

extern char IP_Table[64];
extern char IPR_Table[64];
extern char E_Table[48];
extern char P_Table[32];
extern char PC1_Table[56];
extern char PC2_Table[48];
extern char LOOP_Table[16];
extern char S_Box[8][4][16] ;
class CCryptDes
{
public:

	//////////////////////////////////////////////////////////////////////////

	typedef bool    (*PSubKey)[16][48];

	//////////////////////////////////////////////////////////////////////////

	//void DES(char Out[8], char In[8], const PSubKey pSubKey, bool Type);
	//void SetKey(const char* Key, int len);// ������Կ
	//void SetSubKey(PSubKey pSubKey, const char Key[8]);// ��������Կ
	//void F_func(bool In[32], const bool Ki[48]);// f ����
	//void S_func(bool Out[32], const bool In[48]);// S �д���
	//void Transform(bool *Out, bool *In, const char *Table, int len);// �任
	//void Xor(bool *InA, const bool *InB, int len);// ���
	//void RotateL(bool *In, int len, int loop);// ѭ������
	//void ByteToBit(bool *Out, const char *In, int bits);// �ֽ���ת����λ��
	//void BitToByte(char *Out, const bool *In, int bits);// λ��ת�����ֽ���

	//////////////////////////////////////////////////////////////////////////

	bool SubKey[2][16][48];// 16Ȧ����Կ
	bool Is3DES;// 3��DES��־
	char Tmp[256], deskey[16];


	// Type��ENCRYPT:����,DECRYPT:����
	// ���������(Out)�ĳ��� >= ((datalen+7)/8)*8,����datalen�������8�ı�������С������
	// In ����= Out,��ʱ��/���ܺ󽫸������뻺����(In)������
	// ��keylen>8ʱϵͳ�Զ�ʹ��3��DES��/����,����ʹ�ñ�׼DES��/����.����16�ֽں�ֻȡǰ16�ֽ�

	bool Des_Go(char *Out, char *In, long datalen, const char *Key, int keylen, bool Type)
	{
		if( !( Out && In && Key && (datalen=(datalen+7)&0xfffffff8)!=0 ) ) 
			return false;
		SetKey(Key, keylen);
		if( !Is3DES ) {   // 1��DES
			for(long i=0,j=datalen>>3; i<j; ++i,Out+=8,In+=8)
				DES(Out, In, &SubKey[0], Type);
		} else{   // 3��DES ����:��(key0)-��(key1)-��(key0) ����::��(key0)-��(key1)-��(key0)
			for(long i=0,j=datalen>>3; i<j; ++i,Out+=8,In+=8) {
				DES(Out, In,  &SubKey[0], Type);
				DES(Out, Out, &SubKey[1], !Type);
				DES(Out, Out, &SubKey[0], Type);
			}
		}
		return true;
	}

	// ������Կ
	void SetKey(const char* Key, int len)
	{
		memset(deskey, 0, 16);
		memcpy(deskey, Key, len>16?16:len);
		SetSubKey(&SubKey[0], &deskey[0]);
		Is3DES = len>8 ? (SetSubKey(&SubKey[1], &deskey[8]), true) : false;
	}

	//��׼DES��/����
	void DES(char Out[8], char In[8], const PSubKey pSubKey, bool Type)
	{
		static bool M[64], tmp[32], *Li=&M[0], *Ri=&M[32];
		ByteToBit(M, In, 64);
		Transform(M, M, IP_Table, 64);
		if( Type == _OP_ENCRYPT ){
			for(int i=0; i<16; ++i) {
				memcpy(tmp, Ri, 32);
				F_func(Ri, (*pSubKey)[i]);
				Xor(Ri, Li, 32);
				memcpy(Li, tmp, 32);
			}
		}else{
			for(int i=15; i>=0; --i) {
				memcpy(tmp, Li, 32);
				F_func(Li, (*pSubKey)[i]);
				Xor(Li, Ri, 32);
				memcpy(Ri, tmp, 32);
			}
		}
		Transform(M, M, IPR_Table, 64);
		BitToByte(Out, M, 64);
	}
	
	// ��������Կ
	void SetSubKey(PSubKey pSubKey, const char Key[8])
	{
		static bool K[64], *KL=&K[0], *KR=&K[28];
		ByteToBit(K, Key, 64);
		Transform(K, K, PC1_Table, 56);
		for(int i=0; i<16; ++i) {
			RotateL(KL, 28, LOOP_Table[i]);
			RotateL(KR, 28, LOOP_Table[i]);
			Transform((*pSubKey)[i], K, PC2_Table, 48);
		}
	}

	// f ����
	void F_func(bool In[32], const bool Ki[48])
	{
		static bool MR[48];
		Transform(MR, In, E_Table, 48);
		Xor(MR, Ki, 48);
		S_func(In, MR);
		Transform(In, In, P_Table, 32);
	}

	// S �д���
	void S_func(bool Out[32], const bool In[48])
	{
		for(int i=0,j,k; i<8; ++i,In+=6,Out+=4) {
			j = (In[0]<<1) + In[5];
			k = (In[1]<<3) + (In[2]<<2) + (In[3]<<1) + In[4];
			ByteToBit(Out, &S_Box[i][j][k], 4);
		}
	}
	// �任
	void Transform(bool *Out, bool *In, const char *Table, int len)
	{
		for(int i=0; i<len; ++i)
			Tmp[i] = In[ Table[i]-1 ];
		memcpy(Out, Tmp, len);
	}
	// ���
	void Xor(bool *InA, const bool *InB, int len)
	{
		for(int i=0; i<len; ++i)
			InA[i] ^= InB[i];
	}

	// ѭ������
	void RotateL(bool *In, int len, int loop)
	{
		memcpy(Tmp, In, loop);
		memmove(In, In+loop, len-loop);
		memcpy(In+len-loop, Tmp, loop);
	}
	// �ֽ���ת����λ��
	void ByteToBit(bool *Out, const char *In, int bits)
	{
		for(int i=0; i<bits; ++i)
			Out[i] = (In[i>>3]>>(i&7)) & 1;
	}
	// λ��ת�����ֽ���
	void BitToByte(char *Out, const bool *In, int bits)
	{
		memset(Out, 0, bits>>3);
		for(int i=0; i<bits; ++i)
			Out[i>>3] |= In[i]<<(i&7);
	}

	unsigned int EncryptProcDES(char *Out, int dataLen, const char *Key, int keyLen)
	{
		char *In = Out; 
		dataLen = (dataLen + 7) & 0xFFFFFFF8;
		SetKey(Key, keyLen);
		for(long i=0,j=dataLen>>3; i<j; ++i,Out+=8,In+=8)
			DES(Out, In, &SubKey[0], _OP_ENCRYPT);

		return dataLen;
	}

	unsigned int DecryptProcDES(char *Out, unsigned int dataLen, const char *Key, unsigned int keyLen)
	{
		char *In = Out;
		dataLen = (dataLen + 7) & 0xFFFFFFF8;
		SetKey(Key, keyLen);
		for(long i=0,j=dataLen>>3; i<j; ++i,Out+=8,In+=8)
			DES(Out, In, &SubKey[0], _OP_DECRYPT);

		return dataLen;
	}

protected:
private:
};

#endif//__CRYPT_H__

