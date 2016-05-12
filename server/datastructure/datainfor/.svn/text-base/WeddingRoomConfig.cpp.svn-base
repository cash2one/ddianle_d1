#include "WeddingRoomConfig.h"
#include "./FileStream.h"


bool IntimacyLevelConfig::Load(CFileStream&file)
{
    file.ReadUShort(m_nLev) ;
    file.ReadUInt(m_nExp) ;
    unsigned short nSize = 0 ;
    file.ReadUShort(nSize) ;
    file.ReadString(m_strTitle, nSize) ;
    return true ;
}

bool IntimacyLevelConfig::Save(CFileStream&file)
{
    file.WriteUShort(m_nLev) ;
    file.WriteUInt(m_nExp) ;
    file.WriteUShort((unsigned short)m_strTitle.length());
    file.WriteString(m_strTitle);
    return true ;
}

WeddingRoomConfig::WeddingRoomConfig()
    :m_nType(0)
    ,m_strIcon("")
    ,m_strName("")
    ,m_nItemID(0)
    ,m_nItemCount(0)
{
}

WeddingRoomConfig::~WeddingRoomConfig()
{

}

bool WeddingRoomConfig::Load(CFileStream&file)
{
    file.ReadUChar(m_nType) ;

    unsigned short nSize = 0 ;
    file.ReadUShort(nSize) ;
    file.ReadString(m_strIcon, nSize) ;

    file.ReadUShort(nSize) ;
    file.ReadString(m_strName, nSize) ;

    file.ReadUShort(nSize) ;
    file.ReadString(m_strRoom, nSize) ;

    file.ReadUInt( m_nItemID );
    file.ReadUShort( m_nItemCount );

    return true ;
}

bool WeddingRoomConfig::Save(CFileStream&file)
{
    file.WriteUChar(m_nType) ;

    file.WriteUShort((unsigned short)m_strIcon.length());
    file.WriteString(m_strIcon);

    file.WriteUShort((unsigned short)m_strName.length());
    file.WriteString(m_strName);

    file.WriteUShort((unsigned short)m_strRoom.length());
    file.WriteString(m_strRoom);

    file.WriteUInt( m_nItemID );
    file.WriteUShort( m_nItemCount );

    return true ;
}


