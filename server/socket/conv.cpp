#include "conv.h"
#include <string>
#include <string.h>
#include <stdio.h>
#include "Log.h"
using std::string;

#ifdef WIN32
#pragma warning(disable:4996) // 去除警告sprintf/strncpy
#endif

#ifdef WIN32
#ifndef NTDDI_WIN7SP1
#define NTDDI_WIN7SP1 0x06010100
#endif
#include <windows.h>
//gbk ת utf8
string GBKToUTF8(const string& strGBK)
{
	string strOutUTF8 = "";
	WCHAR * str1;
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
}

//utf-8 ת gbk
string UTF8ToGBK(const string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP,0, (LPCWCH)wszGBK, -1, szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}

#else

#include <iconv.h>

string GBKToUTF8(const string& strGBK)
{
	iconv_t cd; 
	size_t inlen = strGBK.size();
	char * gbkChar = new char[inlen+1];
	gbkChar[inlen] = 0;
	memcpy(gbkChar,strGBK.c_str(),inlen);
	char **pin = &gbkChar;

	cd = iconv_open("utf-8//IGNORE","gb18030"); 
	if (cd==0)
	{
		delete[] gbkChar;
		return ""; 
	}

	size_t outLen = strGBK.size() * 2 + 1;
	char * outbuf = new char [outLen];
	char **pout = &outbuf; 

	memset(outbuf,0,outLen); 
	char* pInBuf = gbkChar;
	char* pOutBuf = outbuf;
	int nRet = iconv(cd,pin,&inlen,pout,&outLen);
	if (nRet ==-1)
	{
		LOG_ERROR << "GBKToUTF8 Conv Error : " << strGBK << std::endl;
	}
	string strTemp = pOutBuf;
	iconv_close(cd); 

	delete[] pInBuf;
	delete[] pOutBuf;

	return strTemp; 
}

string UTF8ToGBK(const string& strUTF8)
{
	iconv_t cd; 
	size_t inlen = strUTF8.size();
	char * utfChar = new char[inlen+1];
	utfChar[inlen] = 0;
	memcpy(utfChar,strUTF8.c_str(),inlen);
	char **pin = &utfChar;

	cd = iconv_open("gb18030//IGNORE","utf-8"); 
	if (cd==0)
	{
		delete[] utfChar;
		return ""; 
	}
	size_t outLen = strUTF8.size() * 2 + 1;
	char * outbuf = new char [outLen];
	char **pout = &outbuf; 

	memset(outbuf,0,outLen); 
	char* pInBuf = utfChar;
	char* pOutBuf = outbuf;
	int nRet = iconv(cd,pin,&inlen,pout,&outLen);
	if (nRet ==-1)
	{
		LOG_ERROR << "UTF8ToGBK Conv Error : " << strUTF8 << std::endl;
	}
	string strTemp = pOutBuf;
	iconv_close(cd); 


	delete[] pInBuf;
	delete[] pOutBuf;

	return strTemp; 
}

#endif

std::string GBKToUTF8(const char * strGBK,int nLength)
{
	char* szBuff = new char[nLength+1];
	szBuff[nLength] = 0;
	strncpy(szBuff,strGBK,nLength);
	std::string szGBK = szBuff;
	delete[] szBuff;
	return GBKToUTF8(szGBK);
}

std::string UTF8ToGBK(const char * strUTF8,int nLength)
{
	char* szBuff = new char[nLength+1];
	szBuff[nLength] = 0;
	strncpy(szBuff,strUTF8,nLength);
	std::string szUTF8 = szBuff;
	delete[] szBuff;
	return UTF8ToGBK(szUTF8);
}

std::string GBKToUTF8(const char * strGBK)
{
	std::string szGBK = strGBK;
	return GBKToUTF8(szGBK);
}

std::string UTF8ToGBK(const char * strUTF8)
{
	std::string szUTF8 = strUTF8;
	return UTF8ToGBK(szUTF8);
}
