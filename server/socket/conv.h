#ifndef _CONV_H__
#define _CONV_H__

#include <string>

std::string GBKToUTF8(const std::string& strGBK);
std::string UTF8ToGBK(const std::string& strUTF8);

std::string GBKToUTF8(const char * strGBK);
std::string UTF8ToGBK(const char * strUTF8);
std::string GBKToUTF8(const char * strGBK,int nLength);
std::string UTF8ToGBK(const char * strUTF8,int nLength);

#endif
