/********************************************************************
	created:	2010/10/13
	author:		Mei jiuhua
	filename: 	DataStructure\ParamPool.h
	purpose:	这个文件序列化一些变量，
				读写不能交叉使用，写完之前不可能读，写完之后不能在写
				主要用接发消息
*********************************************************************/

#ifndef __PARAMPOOL_H__
#define __PARAMPOOL_H__

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <time.h>

#ifdef WIN32
#pragma warning(disable:4018 4100)
#pragma warning(disable: 4996)
#else
typedef long long         __int64;
#endif

using namespace std;

class CParamPool
{
public:

    CParamPool(char * pMemory, int nSize);
    CParamPool(const CParamPool& _src, unsigned int _nPosition, unsigned int _nDataSize);
    ~CParamPool(void);
    void Attach(char * pMemory, int nSize);
    //在开始读取之前，调用这个函数来重新设置当前位置
    void Reset();
    void ClearData();//清除所有数据

    bool AddBool(bool bValue);
    bool AddUChar(unsigned char chValue);
    bool AddChar(char chValue);
    bool AddShort(short nValue);
    bool AddUShort(unsigned short nValue);
    bool AddInt(int nValue);
    bool AddUInt(unsigned int nValue);
    bool AddFloat(float fValue);
    bool AddInt64(__int64 nValue);
    bool AddPerFixSizeMemory(const void * pchData, unsigned short nSize);
    bool AddMemory(const void * pchData, unsigned short nSize);
    bool GetMemory(void * pchData, unsigned short nSize);
    bool GetPerFixSizeMemory(void * pchData, unsigned short &nSize);
    bool GetPerFixSizeMemory(std::string &string);
    bool AddString(const char * pszValue);
    bool AddStringW(const char * pszValue);
    bool AddString(const std::string & strResult);
    bool AddStringW(const std::string & strResult);

    char* GetString();
    unsigned int GetValidDataSize();//打包数据的时候,得到打进去的数据长度
    unsigned int GetCurPosition();//得到当前位置相对于起始的位置
    void SetCurPosition(unsigned int Pos);
    const char* GetData();
    const unsigned int& GetUInt();
    const int& GetInt();
    const unsigned short& GetUShort();
    const short& GetShort();
    const unsigned char& GetUChar();
    const bool GetBool();
    const __int64& GetInt64();
    const float& GetFloat();

    bool HasRemaining();
    int Remaining();

    void Settle();

    char * CloneCurrentBuffer();
    void GetString(std::string & strresult);
    void GetStringW(std::string & strresult);
    bool ReplaceShort(short nValue);
    bool ReplaceUChar(unsigned char nValue);
    std::string GetBuffString();
protected:
    const void _SpaceCheck(int nSize);//throw(overflow_error);
private:
    char* m_pMemory;
    unsigned int m_nMemorySize;
    unsigned int m_nDataSize;
    char*m_pchCurPos;
    char*m_pMaxMemoryPtr;
    bool m_bNeedDelete;
};


//以下部分为序列化的模板方法，目的在于将各种类型的数据以统一的形式进行序列化
//stringw用于模板推导时区分宽字符串解析
class stringw : public string
{
public:
    stringw() {}
    stringw(const string& rhs) {
        *this = rhs;
    }
    stringw& operator=(const string& rhs) {
        assign(rhs.c_str()); return *this;
    }
};

template<class T>
bool doEncode(CParamPool &IOBuff, const T& e)
{
	return e.doEncode(IOBuff);
}
template<class T>
bool doDecode(CParamPool &IOBuff, T& e)
{
	return e.doDecode(IOBuff);
}
#define EncodeDecodeColl(Coll)\
template<class T>\
bool doEncode(CParamPool &IOBuff, const typename std::Coll<T>& collection)\
{\
	IOBuff.AddUShort((unsigned short)collection.size());\
	for (typename std::Coll<T>::const_iterator it = collection.begin(); it != collection.end(); ++it)\
	{\
		doEncode(IOBuff, *it);\
	}\
	return true;\
}\
template<class T>\
bool doDecode(CParamPool &IOBuff, typename std::Coll<T>& collection)\
{\
	unsigned short size = IOBuff.GetUShort();\
	if ( size > 0 )\
	{\
		for (unsigned short i = 0; i < size; ++i)\
		{\
			T elem;\
			doDecode(IOBuff, elem);\
			collection.insert(collection.end(), elem);\
		}\
	}\
	return true;\
}
EncodeDecodeColl(list)
EncodeDecodeColl(vector)
EncodeDecodeColl(set)
template<class K, class V>
bool doEncode(CParamPool &IOBuff, const typename std::map<K, V>& collection)
{
	IOBuff.AddUShort((unsigned short)collection.size());
	for (typename std::map<K, V>::const_iterator it = collection.begin(); it != collection.end(); ++it)
	{
		doEncode(IOBuff, it->first);
		doEncode(IOBuff, it->second);
	}
	return true;
}
template<class K, class V>
bool doDecode(CParamPool &IOBuff, typename std::map<K, V>& collection)
{
	unsigned short size = IOBuff.GetUShort();
	if ( size > 0 )
	{
		for (unsigned short i = 0; i < size; ++i)
		{
			K k;
			V v;
			doDecode(IOBuff, k);
			doDecode(IOBuff, v);
			collection[k] = v;
		}
	}
	return true;
}

#define EncodeDecode(T, EncodeMethod, DecodeMethod)\
	template<> inline bool doEncode(CParamPool &IOBuff, const T& e){return IOBuff.EncodeMethod(e);}\
	template<> inline bool doDecode(CParamPool &IOBuff, T& e){e = IOBuff.DecodeMethod();return true;}
#define EncodeDecodeRef(T, EncodeMethod, DecodeMethod)\
	template<> inline bool doEncode(CParamPool &IOBuff, const T& e){return IOBuff.EncodeMethod(e);}\
	template<> inline bool doDecode(CParamPool &IOBuff, T& e){IOBuff.DecodeMethod(e);return true;}

EncodeDecode(bool, AddBool, GetBool)
EncodeDecode(char, AddChar, GetUChar)
EncodeDecode(unsigned char, AddUChar, GetUChar)
EncodeDecode(short, AddShort, GetShort)
EncodeDecode(unsigned short, AddUShort, GetUShort)
EncodeDecode(int, AddInt, GetInt)
EncodeDecode(unsigned int, AddUInt, GetUInt)
EncodeDecode(__int64, AddInt64, GetInt64)
EncodeDecode(float, AddFloat, GetFloat)
EncodeDecodeRef(string, AddString, GetString)
EncodeDecodeRef(stringw, AddStringW, GetStringW)

template<class T>
CParamPool& operator << (CParamPool& IOBuff, const T& e){doEncode(IOBuff, e); return IOBuff;}
template<class T>
CParamPool& operator >> (CParamPool& IOBuff, T& e){doDecode(IOBuff, e); return IOBuff;}

#endif //__PARAMPOOL_H__
