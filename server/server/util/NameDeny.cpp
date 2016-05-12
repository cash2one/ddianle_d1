#include "NameDeny.h"
#include "../../socket/Ini.h"
#include "../../socket/conv.h"
#include <vector>
#include <fstream>
#include <iterator>


//////////////////////////////////////////////////////////////////////////

bool IsValidChar(char ch)
{
    return (ch >= 0x21 && ch <= 0x7E && ch != '\\');
}

bool IsValidShort(char hh, char ll)
{
    unsigned char h = (unsigned char)hh;
    unsigned char l = (unsigned char)ll;

    if (h == 0xA1 && l == 0xA1)
    {
        return false;
    }

    //双字节1区 0xA1 0xA3 0xA4 0xA5
    if (h == 0xA1)
    {
        if (l >= 0xA1 && l <= 0xFE)
        {
            return true;
        }
    }
    if (h == 0xA3)
    {
        if (l >= 0xA1 && l <= 0xFE)
        {
            return true;
        }
    }
    if (h == 0xA4)
    {
        if (l >= 0xA1 && l <= 0xF3)
        {
            return true;
        }
    }
    if (h == 0xA5)
    {
        if (l >= 0xA1 && l <= 0xF6)
        {
            return true;
        }
    }

    //双字节2区 0xB0到0xF7
    if (h >= 0xB0 && h <= 0xF7 && l >= 0xA1 && l <= 0xFE)
    {
        return true;        //BGK2
    }
    if (h >= 0x81 && h <= 0xA0 && l >= 0x40 && (l <= 0xFE && l != 0x7F))
    {
        return true;        //BGK3
    }
    if (h >= 0xAA && h <= 0xFE && l >= 0x40 && (l <= 0xA0 && l != 0x7F))
    {
        return true;        //BGK4
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////

class FileLineData
{
public:
    FileLineData() {}
    ~FileLineData() {}

    operator const std::string& () const
    {
        return m_strData;
    }

    friend std::istream& operator >> (std::istream &stream, FileLineData &ref)
    {
        std::getline(stream, ref.m_strData);

        TrimCR(ref.m_strData);
        TrimLeft(ref.m_strData);
        TrimRight(ref.m_strData);

        return stream;
    }

private:
    std::string m_strData;
};

//////////////////////////////////////////////////////////////////////////

NameDeny::NameDeny(void)
{
}

NameDeny::~NameDeny(void)
{
}

bool NameDeny::ReadInfoFromFile(const std::string &rFileName)
{
    if (rFileName.empty())
        return false;

    std::ifstream infile(rFileName.c_str());
    std::istream_iterator<FileLineData> input(infile);
    std::istream_iterator<FileLineData> eof;
    std::vector<std::string> denyText(input, eof);

    std::vector<std::string>::iterator itr;
    unsigned int nLength;

    for (itr = denyText.begin(); itr != denyText.end(); ++itr)
    {
        if (itr->empty())
        {
            continue;
        }

        MakeLower(*itr);

        nLength = CalcGBKStringLength(*itr);
#ifndef UTFDB
        *itr = GBKToUTF8(*itr);
#endif

        m_mapDenies.insert(std::make_pair(*itr, nLength));
    }

    return true;
}

bool NameDeny::IsValidText(const std::string &rText) const
{
    if (rText.empty())
        return false;

#ifndef UTFDB
    std::string tmpName(UTF8ToGBK(rText));
#else
    std::string tmpName(rText);
#endif

    //名字中不能有空格和其他一些非显示字符
    const char *ptr = tmpName.c_str();
    while (*ptr != '\0')
    {
        if ((*ptr) & 0x80)
        {
            if (*(ptr + 1) == 0)
            {
                return false;
            }
            if (!IsValidShort(*ptr, *(ptr + 1)))
            {
                return false;
            }

            ptr += 2;
        }
        else
        {
            if (!IsValidChar(*ptr))
            {
                return false;
            }

            ptr += 1;
        }
    }

    tmpName = rText;
    MakeLower(tmpName);

    std::map<std::string, unsigned int>::const_iterator itr;
    for (itr = m_mapDenies.begin(); itr != m_mapDenies.end(); ++itr)
    {
        if (tmpName.find(itr->first) != std::string::npos)
        {
            return false;
        }
    }

    return true;
}

void NameDeny::FilterInvalidText(std::string &rText) const
{
    unsigned int nSize = rText.size();

    if (nSize == 0)
        return;

    std::string tmpText(rText);
    MakeLower(tmpText);

    std::string::size_type nPos = 0;
    std::map<std::string, unsigned int>::const_iterator itr;

    for (itr = m_mapDenies.begin(); itr != m_mapDenies.end(); ++itr)
    {
        if (itr->first.size() > nSize)
        {
            continue;
        }

        nPos = 0;

        while (nPos < tmpText.size())
        {
            nPos = tmpText.find(itr->first, nPos);

            if (nPos != std::string::npos)
            {
                tmpText.replace(nPos, itr->first.size(), itr->second, '*');
                rText.replace(nPos, itr->first.size(), itr->second, '*');
                nPos += itr->second;
            }
            else
            {
                break;
            }
        }
    }
}

void NameDeny::MakeLower(std::string &rText)
{
    unsigned int i = 0;
    unsigned int nSize = rText.size();

    while (i < nSize)
    {
        if ((rText[i] & 0x80) == 0 && isalpha(rText[i]))
        {
            rText[i] = (char)tolower(rText[i]);
        }

        ++i;
    }
}

unsigned int NameDeny::CalcGBKStringLength(const std::string &rText)
{
    if (rText.empty())
        return 0;

    unsigned int i = 0;
    unsigned int nSize = rText.size();
    unsigned int nLength = 0;

    while (i < nSize)
    {
        if (rText[i] < 0)
        {
            i += 2;
        }
        else
        {
            ++i;
        }

        ++nLength;
    }

    return nLength;
}

