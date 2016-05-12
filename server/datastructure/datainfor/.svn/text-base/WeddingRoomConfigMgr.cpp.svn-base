#include "./WeddingRoomConfigMgr.h"
#include "FileStream.h"


WeddingRoomConfigMgr::WeddingRoomConfigMgr()
{
}

WeddingRoomConfigMgr::~WeddingRoomConfigMgr()
{
    Destroy() ;
}

bool WeddingRoomConfigMgr::LoadWeddingRoomConfig(const char* pszFileName)
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

        if(ulNumber>0)
        {
            for( i=0;i<ulNumber;i++)
            {
                IntimacyLevelConfig info;
                info.Load(file);
            }
        }

        file.ReadUShort( ulNumber );
        if(ulNumber>0)
        {
            for( i=0;i<ulNumber;i++)
            {
                WeddingRoomConfig pInfo;
                pInfo.Load(file);

                m_WeddingRoomConfig[pInfo.m_nType] = pInfo;
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

void WeddingRoomConfigMgr::GetWeddingCostItemInfo(unsigned short nTypeID, itemtype_t & nItemID, unsigned short & nCount)
{
    std::map<unsigned short,WeddingRoomConfig>::iterator it = m_WeddingRoomConfig.find( nTypeID );
    if ( it != m_WeddingRoomConfig.end() )
    {
        nItemID = it->second.m_nItemID;
        nCount = it->second.m_nItemCount;
    }
}

void WeddingRoomConfigMgr::Destroy()
{
    m_WeddingRoomConfig.clear() ;
}
