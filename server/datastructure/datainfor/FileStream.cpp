#include "FileStream.h"

#ifdef WIN32
#pragma warning(disable:4996) // È¥³ý¾¯¸æfopen
#endif

bool CFileStream::open(const char *pszFilename, const char *pszMode)
{
    if (m_pFile)
        close();
    m_pFile = fopen(pszFilename, pszMode);
    return m_pFile != NULL;
}

void CFileStream::close()
{
    if (m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

unsigned short CFileStream::ReadString(string& strOut, unsigned short nSize)
{
    if (m_pFile)
    {
        if (nSize > 0)
        {
            char* chBuffer = new char[nSize + 1];
            fread(chBuffer, sizeof(char), nSize, m_pFile);
            chBuffer[nSize] = '\0';
            strOut = chBuffer;
            delete[]chBuffer;
        }
        else
        {
            strOut = "";
        }
    }
    return nSize;
}

unsigned short CFileStream::ReadString2(string &strOut)
{
    unsigned short nSize = 0;
    ReadUShort(nSize);

    if (nSize > 0)
    {
        char *chBuffer = new char[nSize + 1];
        fread(chBuffer, sizeof(char), nSize, m_pFile);
        chBuffer[nSize] = '\0';
        strOut = chBuffer;
        delete[]chBuffer;
    }
    else
    {
        strOut = "";
    }

    return nSize;
}

