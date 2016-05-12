#ifndef __LIBSERVERFRAME_GAMEMSGROOMBROADCAST_H__
#define __LIBSERVERFRAME_GAMEMSGROOMBROADCAST_H__

#include <string>
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../../datastructure/ItemBase.h"
#include "../../datastructure/ImageData.h"

class CParamPool;

class GameMsg_S2C_RefreshItemForOther :
    public GameMsg_Base
{
    typedef  GameMsg_Base _baseClass;
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RefreshItemForOther);
    GameMsg_S2C_RefreshItemForOther(void);
    ~GameMsg_S2C_RefreshItemForOther(void);

    unsigned int   nRoleID;
    itemtype_t nItemType;
    unsigned short nColumn;
    unsigned short nIndex;

//    ItemBase * pItem;
    ItemEquip itemEquip;

public:
    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);
};


/////////////////////////////////////////////////
class GameMsg_S2C_RefreshEffectChangeToOthers :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RefreshEffectChangeToOthers);
    GameMsg_S2C_RefreshEffectChangeToOthers();
    ~GameMsg_S2C_RefreshEffectChangeToOthers();

public:
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    itemtype_t            m_nItemType;
    unsigned int          m_nEffectID;
};


/////////////////////////////////////////////////
class GameMsg_S2C_RefreshColorProgressToOthers :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RefreshColorProgressToOthers);
    GameMsg_S2C_RefreshColorProgressToOthers();
    ~GameMsg_S2C_RefreshColorProgressToOthers();

public:
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned short        m_nSuitID;
    unsigned char         m_nColor;
};


enum ChangeRoleNameResult
{
    CHANGEROLENAME_SUCCESS = 0,
    CHANGEROLENAME_INVADROLENAME,	// 含有非法字符
    CHANGEROLENAME_DUPILICATED,		// 角色名称已存在
    CHANGEROLENAME_CD				// 道具CD，暂时不能修改道具
};

class GameMsg_S2C_ChangeRoleNameResult :
    public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeRoleNameResult);
public:
    GameMsg_S2C_ChangeRoleNameResult(unsigned char nResult = CHANGEROLENAME_SUCCESS);
    ~GameMsg_S2C_ChangeRoleNameResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned char m_nResult;
    unsigned int m_nRoleID;
    std::string m_strNewName;
};



class GameMsg_S2C_NotifyRoleVIPInfoChanged :
    public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_NotifyRoleVIPInfoChanged);
public:
    GameMsg_S2C_NotifyRoleVIPInfoChanged();
    ~GameMsg_S2C_NotifyRoleVIPInfoChanged();
public:
    virtual bool doEncode(CParamPool & IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);
public:
    unsigned int m_nRoleID;
    bool m_bIsVIP;
    unsigned short m_nVIPLevel;
};


//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_UpdateDanceGroupName : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_UpdateDanceGroupName);
public:
    GameMsg_S2C_UpdateDanceGroupName();
    ~GameMsg_S2C_UpdateDanceGroupName() {}

public:
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int                m_nRoleID;
    std::string                 m_strNewName;
};


//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_UpdateDanceGroupTitle : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_UpdateDanceGroupTitle);
public:
    GameMsg_S2C_UpdateDanceGroupTitle();
    ~GameMsg_S2C_UpdateDanceGroupTitle() {}

public:
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int                m_nRoleID;              // 角色ID
    char                        m_nTitle;               // 舞团职位
};


//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew);
    GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew();
    ~GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew() {}

public:
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned short              m_nErrorCode;
    unsigned int                m_nRoleID;
    unsigned short              m_nBadge;
    unsigned short              m_nEffect;
};




//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_SyncMedalInfoToOthers : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_SyncMedalInfoToOthers);

public:
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nRoleID;
    unsigned char       m_nSceneID;
    MedalDisplayInfo    m_displayInfo;
};


//////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_SyncHorseData : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_SyncHorseData);

    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

    int m_nHorseOwnerId;
    int m_nHorseSeatIdx;
    int m_nHorseTypeId;
    int m_nSelfRoleId;
    int m_nSceneId;
};

class GameMsg_S2C_AddGene :
    public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_AddGene);

    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool & IOBuff);

public:
    unsigned int m_nRoleID;
    GeneData m_GeneData;
};

class GameMsg_S2C_DelGene :
    public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_DelGene);

public:
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int m_nRoleID;
    unsigned short m_nGeneID;
};

class GameMsg_S2C_ResetGene :
    public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_ResetGene);

public:
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int m_nRoleID;
    unsigned short m_nGeneID;
    int m_nParam2;
};


#endif // __LIBSERVERFRAME_GAMEMSGROOMBROADCAST_H__