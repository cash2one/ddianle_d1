/**
* Formatter
* Description: 格式化类，用于替代sprintf/sprintf_s/snprintf，同时支持格式化数据转换和流式数据转换
* Copyright (C) 2015 ddianle Inc. All rights reserved.
* Author: xieshaobin
* Create time: 2015-8-5 17:01:32
*/



#ifndef _FORMATTER_H_
#define _FORMATTER_H_

#include <string>
#include <sstream>

#ifdef WIN32
#pragma warning(disable:4625 4626)
#endif


class Formatter
{
public:
    Formatter();
    Formatter(std::string &destBuff);
    Formatter(char *destBuff, int nBuffSize);
    virtual ~Formatter();

public:
    int ToString(std::string &destBuff);
    int ToString(char *destBuff, int nDestBuffSize);

    operator std::string ();

    template <typename T>
    Formatter & operator + (const T &value);

    template <typename T>
    Formatter & operator << (const T &value);

public:
    static int Format(std::string &destBuff, const char *format, ...);
#define SafeFormat(dest, strFormat, ...) Formatter::Format((dest), (strFormat).c_str(), __VA_ARGS__)

    static int Format(char *destBuff, int nDestBuffSize, const char *format, ...);
#define SafeNFormat(dest, size, strFormat, ...) Formatter::Format((dest), (size), (strFormat).c_str(), __VA_ARGS__)

protected:
    void ToString();

protected:
    int m_nDestBuffSize;            // 用户的输出缓冲区长度
    char *m_destBuff;               // 用户的输出缓冲区
    std::string *m_strDestBuff;     // 用户的输出缓冲区
    std::stringstream m_ssBuff;     // 原始数据流
};

template <typename T>
Formatter & Formatter::operator + (const T &value)
{
    m_ssBuff << value;
    ToString();
    return *this;
}

template <typename T>
Formatter & Formatter::operator << (const T &value)
{
    return *this + value;
}

// 测试代码和使用示例
#ifdef _TEST_FORMATTER_
#include <iostream>
#define cout std::cout
#define endl std::endl
static inline void TestFormatter()
{
    // 1.Usage of class Formatter
    std::string hello = Formatter() << "hello world: " << 1234;
    cout << hello << endl;

    std::string world;
    Formatter(world) << std::string("hello world: ") << 1234;
    Formatter(world) + std::string("hello world: ") + 1234;
    cout << world << endl;

    // 2.Usage of Formatter::Format
    std::string strBuff;
    Formatter::Format(strBuff, "hello: %d,%f, %lf, %s, %u", 54, 54.1f, 54.2f, "54.3", 3);
    cout << "strBuff = " << strBuff << endl;

    char buff[256];
    Formatter::Format(buff, sizeof(buff), "hello: %d,%f, %lf, %s, %u", 54, 54.1f, 54.2f, "54.3", 3);
    cout << "buff = " << buff  << endl;

    // 3.Usage of SafeFormat/SafeNFormat
    SafeFormat(strBuff, std::string("hello: %d,%f, %lf, %s, %u"), 54, 54.1f, 54.2f, "54.3", 3);
    cout << "strBuff  = " << strBuff << endl;

    SafeNFormat(buff, sizeof(buff), std::string("hello: %d,%f, %lf, %s, %u"), 54, 54.1f, 54.2f, "54.3", 3);
    cout << "buff = " << buff  << endl;

    // 4. NULL
    Formatter(buff, sizeof(buff)) << NULL;
    cout << "buff = " << buff  << endl;
    Formatter::Format(buff, sizeof(buff), "%s", NULL);
    cout << "buff = " << buff  << endl;

    Formatter(strBuff) << NULL;
    cout << "strBuff = " << strBuff  << endl;
    Formatter::Format(strBuff, "%s", NULL);
    cout << "strBuff = " << strBuff  << endl;
}
#undef cout
#undef endl
#endif //_TEST_FORMATTER_

#endif //_FORMATTER_H_

