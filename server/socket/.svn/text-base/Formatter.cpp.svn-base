#include "Formatter.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#pragma warning(disable:4571)
#endif

Formatter::Formatter()
    : m_nDestBuffSize(0)
    , m_destBuff(NULL)
    , m_strDestBuff(NULL)
{
}

Formatter::Formatter(std::string &destBuff)
    : m_nDestBuffSize(0)
    , m_destBuff(NULL)
    , m_strDestBuff(&destBuff)
{
}

Formatter::Formatter(char *destBuff, int nBuffSize)
    : m_nDestBuffSize(nBuffSize)
    , m_destBuff(destBuff)
    , m_strDestBuff(NULL)
{
    if (m_nDestBuffSize < 0)
    {
        m_nDestBuffSize = 0;
    }
}

Formatter::~Formatter()
{
}

int Formatter::ToString(std::string &destBuff)
{
    destBuff.assign(m_ssBuff.str());
    return (int)destBuff.size();
}

int Formatter::ToString(char *destBuff, int nDestBuffSize)
{
    if (destBuff == NULL || nDestBuffSize <= 0)
        return 0;

    std::string strBuff(*this);
    if (strBuff.empty())
    {
        destBuff[0] = '\0';
        return 0;
    }

    int nCopySize = strBuff.length();
    if (nDestBuffSize <= nCopySize)
    {
        nCopySize = nDestBuffSize - 1;
    }

    memcpy(destBuff, strBuff.c_str(), nCopySize);
    destBuff[nCopySize] = '\0';

    return nCopySize;
}

Formatter::operator std::string ()
{
    return m_ssBuff.str();
}

void Formatter::ToString()
{
    if (m_strDestBuff != NULL)
        ToString(*m_strDestBuff);

    if (m_destBuff != NULL && m_nDestBuffSize > 0)
        ToString(m_destBuff, m_nDestBuffSize);
}

#ifdef WIN32
#pragma warning(disable:4996) // È¥³ý¾¯¸æ: 4996 vsnprintf
#endif

int Formatter::Format(std::string &destBuff, const char *format, ...)
{
    static const size_t kSmallBufferSize = 256;
    static const size_t kFormatBufferSize = 4 * 1024;

    int result = 0;
    char smallBuff[kSmallBufferSize] = { 0 };
    char *pBuff = smallBuff;
    size_t nSize = kSmallBufferSize;
    va_list list;

    while (format != NULL)
    {
        va_start(list, format);
        result = vsnprintf(pBuff, nSize, format, list);
        va_end(list);

        if (result > -1 && result < (int)nSize) // OK
        {
            destBuff.assign(pBuff);

            if (pBuff != smallBuff)
                delete[]pBuff;

            return result;
        }

        /* Else try again with more space. */
        if (result > -1)    /* glibc 2.1 */
            nSize = result + 1; /* precisely what is needed */
        else           /* glibc 2.0 and vs2010 */
            nSize = (2*nSize > kFormatBufferSize ? 2*nSize : kFormatBufferSize);  /* twice the old size */

        if (pBuff != smallBuff)
            delete[]pBuff;

        try
        {
            pBuff = new char[nSize];
        }
        catch (...)
        {
            return -1;
        }
    }

    return result;
}

int Formatter::Format(char *destBuff, int nDestBuffSize, const char *format, ...)
{
    int result = 0;
    if (destBuff != NULL && nDestBuffSize > 0 && format != NULL)
    {
        va_list list;
        va_start(list, format);
        result = vsnprintf(destBuff, nDestBuffSize-1, format, list);
        va_end(list);
    }

    return result;
}

