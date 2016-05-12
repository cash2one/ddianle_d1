#include "./ExpConfigManager.h"
#include "FileStream.h"


ExpConfigManager::ExpConfigManager()
{

}

ExpConfigManager::~ExpConfigManager()
{
    Destroy() ;
}

bool ExpConfigManager::LoadExpInfo(const char* pszFileName)
{
    bool bRes = true ;
    Destroy();

    CFileStream file;
    file.open(pszFileName,"rb");
    if(file.IsOpen())
    {
        unsigned short ulNumber = 0;
        file.ReadUShort(ulNumber);
        unsigned short i;
        if(ulNumber>0){
            for( i=0;i<ulNumber;i++){
                ExpConfig pInfo;
                pInfo.Load(file);
                if(pInfo.m_nExp > 0)
                {
                    m_mapExp.insert(std::make_pair((unsigned short)(i+1), pInfo.m_nExp)) ;
                }
            }
        }
        file.close();
    }
    else
    {
        bRes = false;
    }
    return bRes ;
}

void ExpConfigManager::Destroy()
{
    m_mapExp.clear() ;
}

int ExpConfigManager::GetByLev(unsigned short nlev)
{
    if (nlev > 0){
        std::map<unsigned short,int>::const_iterator it= m_mapExp.find(nlev);
        if(it == m_mapExp.end()) {
            return 0 ;
        }
        else {
            return it->second;
        }
    }
    return 0;
}


