#include "PhotoRoomConfig.h"
#include "./FileStream.h"

PhotoRoomConfig::PhotoRoomConfig()
    :m_nID(0)
    ,m_strName("")
    ,m_strTexture("")
    ,m_nRequiredItemType(0)
    ,m_nRequiredItemCount(0)
{

}

PhotoRoomConfig::~PhotoRoomConfig()
{

}

bool PhotoRoomConfig::Save( CFileStream&file )
{
    file.WriteUInt(m_nID) ;

    file.WriteUShort((unsigned short)m_strName.size());
    file.WriteString(m_strName);

    file.WriteUShort((unsigned short)m_strTexture.size());
    file.WriteString(m_strTexture);


    file.WriteUInt(m_nRequiredItemType);
    file.WriteUShort(m_nRequiredItemCount);

    return true;
}

bool PhotoRoomConfig::Load( CFileStream&file )
{
    file.ReadUInt(m_nID) ;

    unsigned short nSize = 0 ;
    file.ReadUShort(nSize) ;
    file.ReadString(m_strName, nSize) ;

    file.ReadUShort(nSize) ;
    file.ReadString(m_strTexture, nSize) ;

    file.ReadUInt(m_nRequiredItemType);
    file.ReadUShort(m_nRequiredItemCount);

    return true;
}

///////////////////////////////////////////////////////////////
PhotoThemeBGConfig::PhotoThemeBGConfig()
    :m_nID(0)
    ,m_ThemeID(0)
    ,m_nBGID(0)
{
}

PhotoThemeBGConfig::~PhotoThemeBGConfig()
{
}

bool PhotoThemeBGConfig::Save(CFileStream&file)
{
    file.WriteUInt(m_nID) ;
    file.WriteUChar(m_ThemeID);
    file.WriteUInt(m_nBGID);

    return true;
}

bool PhotoThemeBGConfig::Load(CFileStream&file)
{
    file.ReadUInt(m_nID);
    file.ReadUChar(m_ThemeID);
    file.ReadUInt(m_nBGID);

    return true;
}

////////////////////////////////////////////////////////////////
PhotoRoomSingleAction::PhotoRoomSingleAction()
    :m_nID(0)
    ,m_strName("")
    ,m_strAnimationName("")
{

}

PhotoRoomSingleAction::~PhotoRoomSingleAction()
{

}

bool PhotoRoomSingleAction::Save( CFileStream&file )
{
    file.WriteUInt(m_nID);
    file.WriteString2(m_strName);
    file.WriteString2(m_strAnimationName);

    return true;
}

bool PhotoRoomSingleAction::Load( CFileStream&file )
{
    file.ReadUInt(m_nID);
    file.ReadString2(m_strName);
    file.ReadString2(m_strAnimationName);

    return true;
}

/////////////////////////////////////////////////////
PhotoRoomGroupAction::PhotoRoomGroupAction()
    :m_nID(0)
    ,m_strPos1Ani("")
    ,m_strPos2Ani("")
    ,m_strPos3Ani("")
    ,m_strPos4Ani("")
    ,m_strIcon("")
    ,m_strAtlas("")
    ,m_nRequiredItemType(0)
    ,m_nRequiredItemCount(0)
{
}
PhotoRoomGroupAction::~PhotoRoomGroupAction()
{
}

bool PhotoRoomGroupAction::Save( CFileStream&file )
{
    file.WriteUInt(m_nID);
    file.WriteString2(m_strPos1Ani);
    file.WriteString2(m_strPos2Ani);
    file.WriteString2(m_strPos3Ani);
    file.WriteString2(m_strPos4Ani);
    file.WriteString2(m_strIcon);
    file.WriteString2(m_strAtlas);
    file.WriteUInt(m_nRequiredItemType);
    file.WriteUShort(m_nRequiredItemCount);

    return true;
}

bool PhotoRoomGroupAction::Load( CFileStream&file )
{
    file.ReadUInt(m_nID);
    file.ReadString2(m_strPos1Ani);
    file.ReadString2(m_strPos2Ani);
    file.ReadString2(m_strPos3Ani);
    file.ReadString2(m_strPos4Ani);
    file.ReadString2(m_strIcon);
    file.ReadString2(m_strAtlas);
    file.ReadUInt(m_nRequiredItemType);
    file.ReadUShort(m_nRequiredItemCount);

    return true;
}

/////////////////////////////////////////////////////

PhotoRoomThemeGroupAction::PhotoRoomThemeGroupAction()
    :m_nID(0)
    ,m_ThemeID(0)
    ,m_GroupActionID(0)
{
}

PhotoRoomThemeGroupAction::~PhotoRoomThemeGroupAction()
{
}

bool PhotoRoomThemeGroupAction::Save(CFileStream&file)
{
    file.WriteUInt(m_nID);
    file.WriteUChar(m_ThemeID);
    file.WriteUInt(m_GroupActionID);

    return true;
}

bool PhotoRoomThemeGroupAction::Load( CFileStream&file )
{
    file.ReadUInt(m_nID);
    file.ReadUChar(m_ThemeID);
    file.ReadUInt(m_GroupActionID);
    return true;
}

PhotoRoomTheme::PhotoRoomTheme()
    :m_nID(0)
    ,m_strName("")
    ,m_nMaxPlayerCount(0)
    ,m_nInviteType(0)
    ,m_nDefaultBGID(0)
    ,m_nRequiredLevel(0)
    ,m_nRequiredItemType(0)
    ,m_nRequiredItemCount(0)
    ,m_nDefaultExistedTime(0)
    ,m_bCanRenew(false)
    ,m_nRenewItemType(0)
    ,m_nRenewItemCount(0)
    ,m_nRenewTime(0)
{
}

PhotoRoomTheme::~PhotoRoomTheme()
{
}

bool PhotoRoomTheme::Save(CFileStream&file)
{
    file.WriteUChar(m_nID);
    file.WriteString2(m_strName);
    file.WriteUChar(m_nMaxPlayerCount);
    file.WriteUChar(m_nInviteType);
    file.WriteUInt(m_nDefaultBGID);
    file.WriteUShort(m_nRequiredLevel);
    file.WriteUInt(m_nRequiredItemType);
    file.WriteUShort(m_nRequiredItemCount);
    file.WriteUInt(m_nDefaultExistedTime);
    file.WriteBool(m_bCanRenew);
    file.WriteUInt(m_nRenewItemType);
    file.WriteUShort(m_nRenewItemCount);
    file.WriteUInt(m_nRenewTime);

    return true;
}

bool PhotoRoomTheme::Load( CFileStream&file )
{
    file.ReadUChar(m_nID);
    file.ReadString2(m_strName);
    file.ReadUChar(m_nMaxPlayerCount);
    file.ReadUChar(m_nInviteType);
    file.ReadUInt(m_nDefaultBGID);
    file.ReadUShort(m_nRequiredLevel);
    file.ReadUInt(m_nRequiredItemType);
    file.ReadUShort(m_nRequiredItemCount);
    file.ReadUInt(m_nDefaultExistedTime);
    file.ReadBool(m_bCanRenew);
    file.ReadUInt(m_nRenewItemType);
    file.ReadUShort(m_nRenewItemCount);
    file.ReadUInt(m_nRenewTime);

    return true;
}

unsigned char PhotoRoomTheme::ThemeID() const
{
    return m_nID;
}

unsigned char PhotoRoomTheme::MaxPlayerCount() const
{
    return m_nMaxPlayerCount;
}

unsigned char PhotoRoomTheme::InviteType() const
{
    return m_nInviteType;
}

unsigned int  PhotoRoomTheme::DefaultThemeBG() const
{
    return m_nDefaultBGID;
}

unsigned short PhotoRoomTheme::RequiredLevel() const
{
    return m_nRequiredLevel;
}

itemtype_t PhotoRoomTheme::RequiredItemType() const
{
    return m_nRequiredItemType;
}

unsigned short PhotoRoomTheme::RequiredItemCount() const
{
    return m_nRequiredItemCount;
}

unsigned int PhotoRoomTheme::DefaultExistedTime() const
{
    return m_nDefaultExistedTime;
}

bool PhotoRoomTheme::CanRenew() const
{
    return m_bCanRenew;
}

itemtype_t PhotoRoomTheme::RenewItemType() const
{
    return m_nRenewItemType;
}

unsigned short PhotoRoomTheme::RenewItemCount() const
{
    return m_nRenewItemCount;
}

unsigned int PhotoRoomTheme::RenewTime() const
{
    return m_nRenewTime;
}

