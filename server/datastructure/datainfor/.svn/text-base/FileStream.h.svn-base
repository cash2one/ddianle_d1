#ifndef __FILESTREAM_H__
#define __FILESTREAM_H__

#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
using namespace std;

#ifdef WIN32
#else//WIN32
typedef long long         __int64;
#endif

#define Buffer_Size (1024*4) 

class CFileStream
{
public:
    CFileStream(void)
    {
        m_pFile = NULL;
    }

    virtual ~CFileStream(void)
    {
        close();
    }

    bool open(const char *pszFilename, const char *pszMode);
    void close();

    bool IsOpen()
    {
        return m_pFile != NULL;
    }

    unsigned short ReadString(string& strOut, unsigned short nSize);
    unsigned short ReadString2(string &strOut);

    void WriteString(const char *buffer, unsigned short nSize)
    {
        fwrite(buffer, sizeof(char), nSize, m_pFile);
    }

    void  WriteString(std::string & str)
    {
        WriteString(str.c_str(), (unsigned short)str.length());
    }

    void WriteWString(const wchar_t buffer[], unsigned short nSize)
    {
        fwrite(buffer, sizeof(wchar_t), nSize, m_pFile);
    }

    void WriteString2(const char *buff, unsigned short nSize)
    {
        if (nSize > 0)
        {
            WriteUShort(nSize);
            fwrite(buff, sizeof(char), nSize, m_pFile);
        }
        else
        {
            WriteUShort(0);
        }
    }

    void WriteString2(std::string &str)
    {
        WriteString2(str.c_str(), (unsigned short)str.size());
    }

    void WriteString2(const wchar_t buff[], unsigned short nSize)
    {
        if (nSize > 0)
        {
            WriteUShort(nSize);
            fwrite(buff, sizeof(wchar_t), nSize, m_pFile);
        }
        else
        {
            WriteShort(0);
        }
    }

    void WriteMemory(const char chBuffer[], long nSize)
    {
        if (m_pFile)
            fwrite(chBuffer, sizeof(char), nSize, m_pFile);
    }

    void ReadMemory(char chBuffer[], long nSize)
    {
        if (!m_pFile || (fread(chBuffer, sizeof(char), nSize, m_pFile) == 0))
            memset(chBuffer, 0, nSize);
    }

    void ReadShort(short & nValue)
    {
        if (!m_pFile || (fread(&nValue, sizeof(short), 1, m_pFile) == 0))
            nValue = 0;
    }

    void WriteShort(const short nValue)
    {
        if (m_pFile)
            fwrite(&nValue, sizeof(short), 1, m_pFile);
    }

    void ReadUShort(unsigned short & nValue)
    {
        if (!m_pFile || (fread(&nValue, sizeof(unsigned short), 1, m_pFile) == 0))
            nValue = 0;
    }

    void WriteUShort(const unsigned short nValue)
    {
        if (m_pFile)
            fwrite(&nValue, sizeof(unsigned short), 1, m_pFile);
    }

    void ReadInt(int &nValue)
    {
        if (!m_pFile || (fread(&nValue, sizeof(int), 1, m_pFile) == 0))
            nValue = 0;
    }

    void WriteInt(const int nValue)
    {
        if (m_pFile)
            fwrite(&nValue, sizeof(int), 1, m_pFile);
    }

    void ReadUInt(unsigned int &nValue)
    {
        if (!m_pFile || (fread(&nValue, sizeof(unsigned int), 1, m_pFile) == 0))
            nValue = 0;
    }

    void WriteUInt(const unsigned int nValue)
    {
        if (m_pFile)
            fwrite(&nValue, sizeof(unsigned int), 1, m_pFile);
    }

    void ReadLong(long &nValue)
    {
        if (!m_pFile || (fread(&nValue, sizeof(long), 1, m_pFile) == 0))
            nValue = 0;
    }

    void WriteLong(const long nValue)
    {
        if (m_pFile)
            fwrite(&nValue, sizeof(long), 1, m_pFile);
    }

    void ReadDWORD(unsigned long &dwValue)
    {
        if (!m_pFile || (fread(&dwValue, sizeof(unsigned long), 1, m_pFile) == 0))
            dwValue = 0;
    }

    void WriteDWORD(const unsigned long dwValue)
    {
        if (m_pFile)
            fwrite(&dwValue, sizeof(unsigned long), 1, m_pFile);
    }

    void ReadLongLong(__int64 &nValue)
    {
        if (!m_pFile || (fread(&nValue, sizeof(__int64), 1, m_pFile) == 0))
            nValue = 0;
    }

    void WriteLongLong(const __int64 nValue)
    {
        if (m_pFile)
            fwrite(&nValue, sizeof(unsigned long), 1, m_pFile);
    }

    void  ReadBool(bool &bValue)
    {
        char chValue = 0;
        ReadChar(chValue);
        bValue = (chValue != 0);
    }

    void  WriteBool(const bool bValue)
    {
        char chValue = 0;
        if (bValue)
            chValue = 1;
        WriteChar(chValue);
    }

    void ReadFloat(float &fValue)
    {
        if (!m_pFile || (fread(&fValue, sizeof(float), 1, m_pFile) == 0))
            fValue = 0;
    }

    void WriteFloat(const float fValue)
    {
        if (m_pFile)
            fwrite(&fValue, sizeof(float), 1, m_pFile);
    }

    void ReadChar(char &cValue)
    {
        if (!m_pFile || (fread(&cValue, sizeof(char), 1, m_pFile) == 0))
            cValue = 0;
    }

    void ReadUChar(unsigned char &cValue)
    {
        if (!m_pFile || (fread(&cValue, sizeof(unsigned char), 1, m_pFile) == 0))
            cValue = 0;
    }

    void WriteChar(const char cValue)
    {
        if (m_pFile)
            fwrite(&cValue, sizeof(char), 1, m_pFile);
    }

    void WriteUChar(const unsigned char cValue)
    {
        if (m_pFile)
            fwrite(&cValue, sizeof(unsigned char), 1, m_pFile);
    }

    FILE *GetFileHandle() const
    {
        return m_pFile;
    }

protected:
    FILE * m_pFile;
};

#endif//__FILESTREAM_H__

