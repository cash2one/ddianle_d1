/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\PhotoRoomConfig.h
    author:     

    purpose:    PhotoStudioInfo.bytes��Ӧ�������ݽṹ
*********************************************************************/

#ifndef     __PHOTOROOMCONFIG_H__
#define     __PHOTOROOMCONFIG_H__

#include <string>
#include <map>
#include "../DataStruct_Base.h"

class CFileStream;

//Ļ����������
class PhotoRoomConfig
{
public:
    PhotoRoomConfig();
    ~PhotoRoomConfig();

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned int    m_nID;
    std::string     m_strName;
    std::string     m_strTexture;
    itemtype_t      m_nRequiredItemType;
    unsigned short  m_nRequiredItemCount;
};

//Ļ�����ñ�������
class PhotoThemeBGConfig
{
public:
    PhotoThemeBGConfig();
    ~PhotoThemeBGConfig();

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned int    m_nID;
    unsigned char   m_ThemeID;
    unsigned int    m_nBGID;
};

//���˶�������
class PhotoRoomSingleAction
{
public:
    PhotoRoomSingleAction();
    ~PhotoRoomSingleAction();

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned int m_nID;
    std::string  m_strName;
    std::string  m_strAnimationName;
};

//���嶯������
class PhotoRoomGroupAction
{
public:
    PhotoRoomGroupAction();
    ~PhotoRoomGroupAction();

public:
    bool Save(CFileStream&file);
    bool Load(CFileStream&file);

public:
    unsigned int    m_nID;
    std::string     m_strPos1Ani;
    std::string     m_strPos2Ani;
    std::string     m_strPos3Ani;
    std::string     m_strPos4Ani;
    std::string     m_strIcon;
    std::string     m_strAtlas;
    itemtype_t      m_nRequiredItemType;
    unsigned short  m_nRequiredItemCount;
};

//���嶯�����ó�������
class PhotoRoomThemeGroupAction
{
public:
    PhotoRoomThemeGroupAction();
    ~PhotoRoomThemeGroupAction();

public:
    bool Save(CFileStream&file);
    bool Load(CFileStream&file);

public:
    unsigned int    m_nID;
    unsigned char   m_ThemeID;
    unsigned int    m_GroupActionID;
};

//���������
class PhotoRoomTheme
{
public:
    PhotoRoomTheme();
    ~PhotoRoomTheme();

public:
    bool Save(CFileStream&file);
    bool Load(CFileStream&file);

public:
    unsigned char ThemeID() const;
    unsigned char MaxPlayerCount() const;
    unsigned char InviteType() const;
    unsigned int  DefaultThemeBG() const;
    unsigned short RequiredLevel() const;
    itemtype_t RequiredItemType() const;
    unsigned short RequiredItemCount() const;
    unsigned int DefaultExistedTime() const;
    bool CanRenew() const;
    itemtype_t RenewItemType() const;
    unsigned short RenewItemCount() const;
    unsigned int RenewTime() const;

public:
    unsigned char       m_nID;
    std::string         m_strName;
    unsigned char       m_nMaxPlayerCount;
    unsigned char       m_nInviteType;
    unsigned int        m_nDefaultBGID;
    unsigned short      m_nRequiredLevel;
    itemtype_t          m_nRequiredItemType;
    unsigned short      m_nRequiredItemCount;
    unsigned int        m_nDefaultExistedTime;
    bool                m_bCanRenew;
    itemtype_t          m_nRenewItemType;
    unsigned short      m_nRenewItemCount;
    unsigned int        m_nRenewTime;
};

#endif


