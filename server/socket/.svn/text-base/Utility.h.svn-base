//  [2003-5-12]
//Utility.h
//实现windows和linux的通用的一些常用功能函数，如 TRACE，得到本地Ip地址，得到执行路径等
//实现了windows下有linux下没有的一些常用的功能函数，如 GetTickCount，GetCurrentProcessId等
//为消除windows和linux的一些差别做了功能类，如 格式化字符串类CFormatString，得到本地机器信息类CHost等

#ifndef __UTILITY_H__
#define __UTILITY_H__

#ifdef WIN32
#pragma warning(disable:4786)
#else
#endif
#include <string>
#include <list>
#include <vector>
#include <deque>
#include <string.h>
using namespace std;

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX   256
#endif

#ifndef MAX_PATH
#define MAX_PATH        260
#endif

#ifndef MAX_TRACE_STRING
#define MAX_TRACE_STRING	2048
#endif

class CHost
{
public:
    CHost();
    //获得本机的地址，返回字符串
    string GetHostAddr(unsigned int *addr = NULL, unsigned int nEth = 0);
    //获得本机的机器名
    string GetHostName();
    //机器名是否等同于指定字符串（不区分大小写）
    bool HostNameEquals(const char* pszHostName);
    //获得地址数量
    int GetAddrCount(){
        return (int)m_aAddr.size();
    };
protected:
private:
    char m_name[HOST_NAME_MAX];
    deque< unsigned int >m_aAddr;
};


//得到运行目录
string GetExecPath(char** argv);

//解析由;分割的字符串
bool ParseString(char* arg, list<string> &result);

void _Debug_Trace(const char* lpszFmt, ...);
#ifndef TRACE
#ifdef WIN32
#define TRACE		_Debug_Trace
#define TRACE_CORE	_Debug_Trace
#else
#include "Log.h"
#ifdef _DEBUG
#define TRACE		WriteLog
#else
#define TRACE		_Debug_Trace
#endif
#endif
#endif

//得到8倍的TickCount，最长支持397天
unsigned int GetOctupleTickCount();

#ifdef WIN32
#else
unsigned int GetTickCount();
void init_daemon(void);
unsigned int GetCurrentProcessId();
void sig_empty(int nSignal);
#endif

extern CHost s_Host;

class CFormatString
{
public:
    CFormatString(){};
    CFormatString(const char* pszStr){
        *this = pszStr;
    };
    const CFormatString& operator =(const char* pszStr)
    {
        m_str = pszStr;
#ifdef WIN32
        find_replace("%lld", "%I64d");
#endif
        return *this;
    };
    const char* c_str(){
        return m_str.c_str();
    };
    void operator += (const char* pszStr)
    {
        m_str += pszStr;
#ifdef WIN32
        find_replace("%lld", "%I64d");
#endif
    }
protected:
    void find_replace(const char* szFind, const char* szReplace)
    {
        for (;;)
        {
            string::size_type nPos = m_str.find(szFind);
            if (nPos == string::npos)break;
            m_str.replace(nPos, strlen(szFind), szReplace);
        }
    }
    string m_str;

};

void CopyString2Buffer(string & src, char* DesBuffer, int nDesBufferLen);
char * WideChar2MultiChar(wchar_t * pwcSrc);
wchar_t * MultiChar2WideChar(const char * pszSrc);
void safememcpy(char * des, unsigned int  Deslen, const char* src, unsigned int SrcLen);

//整数范围内的随机数
unsigned int Random();
//区间[nStart, nEnd]随机整数
int Random(int nStart, int nEnd);
//根据vector中的rate随机相应的vector index
int RateRandom(const std::vector<int> & rRateVec);
//随机串
string CodeRandom(int nLength);

//二进制数据转串
string BinaryData2String(const unsigned char *src, unsigned int srcLen);


#ifdef WIN32
// 多字节编码转为UTF8编码
bool MBToUTF8(unsigned int CodePage, vector<char>& pu8, const char* pmb, int mLen);
// UTF8编码转为多字节编码
bool UTF8ToMB(unsigned int CodePage, vector<char>& pmb, const char* pu8, int utf8Len);
// 多字节编码转为Unicode编码
bool MBToUnicode(unsigned int CodePage, vector<wchar_t>& pun, const char* pmb, int mLen);
//Unicode编码转为多字节编码
bool UnicodeToMB(unsigned int CodePage, vector<char>& pmb, const wchar_t* pun, int uLen);
// UTF8编码转为Unicode
bool UTF8ToUnicode(vector<wchar_t>& pun, const char* pu8, int utf8Len);
// Unicode编码转为UTF8
bool UnicodeToUTF8(vector<char>& pu8, const wchar_t* pun, int uLen);
#endif

#endif//__UTILITY_H__

