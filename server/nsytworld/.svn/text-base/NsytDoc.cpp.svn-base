#include "nsytdoc.h"

CNsytDoc::CNsytDoc()
{
}

CNsytDoc::CNsytDoc(const CNsytDoc& data) 
{ 
    *this = data; 
}

CNsytDoc::~CNsytDoc()
{
}

const CNsytDoc& CNsytDoc::operator=(const CNsytDoc& data)
{
    return *this;
}

void CNsytDoc::ReadData(CFileStream* pfs, void * lpData, int nDataSize) 
{ 
    pfs->ReadMemory((char*)lpData, nDataSize); 
}

void CNsytDoc::WriteData(CFileStream* pfs, void * lpData, int nDataSize) 
{ 
    pfs->WriteMemory((char*)lpData, nDataSize); 
}

void CNsytDoc::ReadString(CFileStream* pfs, std::string& s)
{
    int nLength;
    pfs->ReadMemory((char*)&nLength, sizeof(nLength));
    if (nLength > 0)
    {
        char *pText = new char[nLength + 1];
        pfs->ReadMemory(pText, nLength);
        *(pText + nLength) = '\0';
        s = pText;
        delete pText;
    }
    else
    {
        //s.clear();
        s = "";
    }
}

void CNsytDoc::WriteString(CFileStream* pfs, std::string s)
{
    int nLength = int(s.length());
    pfs->WriteMemory((char*)&nLength, sizeof(nLength));
    if (nLength > 0)
    {
        pfs->WriteMemory(s.c_str(), nLength);
    }
}


//end file


