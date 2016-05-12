#ifndef __NSYTDOC_H__
#define __NSYTDOC_H__

#include <string>

#include "../datastructure/datainfor/FileStream.h"
#include "../socket/Utility.h"
#include "../socket/Windefine.h"

typedef unsigned long		DWORD;
typedef void*				LPVOID;

#define BAD_ID 0

// œ–÷√√ª”–”√
class CNsytDoc
{
public:
    CNsytDoc();
    CNsytDoc(const CNsytDoc& data);
    virtual ~CNsytDoc();

    virtual const CNsytDoc& operator=(const CNsytDoc& data);

    virtual void ReadDoc(CFileStream* pFs)  = 0;
    virtual void WriteDoc(CFileStream* pFs) = 0;

    static void ReadData(CFileStream* pfs, void * lpData, int nDataSize);
    static void WriteData(CFileStream* pfs, void * lpData, int nDataSize); 

    static void ReadString(CFileStream* pfs, std::string& s);
    static void WriteString(CFileStream* pfs, std::string s);
};

#endif	//__NSYTDOC_H__


//end file


