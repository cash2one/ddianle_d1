#include "GameMsgRoomBroadcast.h"
#include "../share/ServerMsgDef.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_RefreshItemForOther
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RefreshItemForOther::GameMsg_S2C_RefreshItemForOther(void) :_baseClass(MSG_S2C_RefreshItemForOtherNew)
{
    nRoleID = 0;
    nItemType = 0;
    nColumn = 0;
    nIndex = 0;
//    pItem = NULL;
}

GameMsg_S2C_RefreshItemForOther::~GameMsg_S2C_RefreshItemForOther(void)
{
}

bool GameMsg_S2C_RefreshItemForOther::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(nRoleID);
    IOBuff.AddUInt(nItemType);
    IOBuff.AddUShort(nColumn);
    IOBuff.AddUShort(nIndex);
    IOBuff.AddUChar(nItemType == 0 ? 0 : 1);

    if (nItemType != 0){
        itemEquip.Encode(&IOBuff);
    }
    return true;
}

bool GameMsg_S2C_RefreshItemForOther::doDecode(CParamPool &IOBuff)
{
    nRoleID = IOBuff.GetUInt();
    nItemType = IOBuff.GetUInt();
    nColumn = IOBuff.GetUShort();
    nIndex = IOBuff.GetUShort();
    unsigned char ub = IOBuff.GetUChar();
    if (0 != ub)
    {
        itemEquip.Decode(&IOBuff);
    }

    return true;
}



GameMsg_S2C_RefreshEffectChangeToOthers::GameMsg_S2C_RefreshEffectChangeToOthers()
    :GameMsg_Base(MSG_S2C_RefreshEffectChangeToOthersNew)
    , m_nItemType(0)
    , m_nEffectID(0)
{

}

GameMsg_S2C_RefreshEffectChangeToOthers::~GameMsg_S2C_RefreshEffectChangeToOthers()
{

}

bool GameMsg_S2C_RefreshEffectChangeToOthers::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nItemType);
    IOBuff.AddUInt(m_nEffectID);
    return true;
}

bool GameMsg_S2C_RefreshEffectChangeToOthers::doDecode(CParamPool &IOBuff)
{
    m_nItemType = IOBuff.GetUInt();
    m_nEffectID = IOBuff.GetUInt();
    return true;
}



GameMsg_S2C_RefreshColorProgressToOthers::GameMsg_S2C_RefreshColorProgressToOthers()
    :GameMsg_Base(MSG_S2C_RefreshColorProgressToOthersNew)
    , m_nSuitID(0)
    , m_nColor(0)
{

}

GameMsg_S2C_RefreshColorProgressToOthers::~GameMsg_S2C_RefreshColorProgressToOthers()
{

}

bool GameMsg_S2C_RefreshColorProgressToOthers::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nSuitID);
    IOBuff.AddUChar(m_nColor);
    return true;
}

bool GameMsg_S2C_RefreshColorProgressToOthers::doDecode(CParamPool &IOBuff)
{
    m_nSuitID = IOBuff.GetUShort();
    m_nColor = IOBuff.GetUChar();
    return true;
}


//////////////////////////////////////////////////////////////////////////
// GameMsg_S2C_ChangeRoleNameResult
GameMsg_S2C_ChangeRoleNameResult::GameMsg_S2C_ChangeRoleNameResult(unsigned char nResult)
    :GameMsg_Base(MSG_S2C_ChangeRoleNameResultNew)
    , m_nResult(nResult)
    , m_nRoleID(0)
{
}

GameMsg_S2C_ChangeRoleNameResult::~GameMsg_S2C_ChangeRoleNameResult()
{
}

bool GameMsg_S2C_ChangeRoleNameResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nResult);
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strNewName);

    return true;
}

bool GameMsg_S2C_ChangeRoleNameResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUChar();
    m_nRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strNewName);

    return true;
}

////////////////////////////////////////////////////////////
// GameMsg_S2C_NotifyRoleVIPInfoChanged
GameMsg_S2C_NotifyRoleVIPInfoChanged::GameMsg_S2C_NotifyRoleVIPInfoChanged()
    :GameMsg_Base(MSG_S2C_NotifyRoleVIPInfoChangedNew)
    , m_nRoleID(0)
    , m_bIsVIP(false)
    , m_nVIPLevel(false)
{

}

GameMsg_S2C_NotifyRoleVIPInfoChanged::~GameMsg_S2C_NotifyRoleVIPInfoChanged()
{

}

bool GameMsg_S2C_NotifyRoleVIPInfoChanged::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);

    return true;
}

bool GameMsg_S2C_NotifyRoleVIPInfoChanged::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_bIsVIP = IOBuff.GetBool();
    m_nVIPLevel = IOBuff.GetUShort();

    return true;
}

// ----------------------------------------------- GameMsg_S2C_UpdateDanceGroupName

GameMsg_S2C_UpdateDanceGroupName::GameMsg_S2C_UpdateDanceGroupName()
    : GameMsg_Base(MSG_S2C_UpdateDanceGroupNameNew)
    , m_nRoleID(0)
    , m_strNewName("")
{
}

bool GameMsg_S2C_UpdateDanceGroupName::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strNewName.c_str());

    return true;
}

bool GameMsg_S2C_UpdateDanceGroupName::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strNewName);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_UpdateDanceGroupTitle

GameMsg_S2C_UpdateDanceGroupTitle::GameMsg_S2C_UpdateDanceGroupTitle()
    : GameMsg_Base(MSG_S2C_UpdateDanceGroupTitleNew)
    , m_nRoleID(0)
    , m_nTitle(-1)
{
}

bool GameMsg_S2C_UpdateDanceGroupTitle::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddChar(m_nTitle);

    return true;
}

bool GameMsg_S2C_UpdateDanceGroupTitle::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nTitle = IOBuff.GetUChar();

    return true;
}

// ----------------------------------------------- GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew

GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew::GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew()
    : GameMsg_Base(MSG_S2C_ChangeDanceGroupBadgeOrEffectResultNew)
    , m_nErrorCode(0)
    , m_nRoleID(0)
    , m_nBadge(0)
    , m_nEffect(0)
{
}

bool GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew::doDecode(CParamPool &IOBuff)
{
    m_nErrorCode = IOBuff.GetUShort();
    m_nRoleID = IOBuff.GetUInt();
    m_nBadge = IOBuff.GetUShort();
    m_nEffect = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nErrorCode);
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUShort(m_nBadge);
    IOBuff.AddUShort(m_nEffect);

    return true;
}



//////////////////////////////////////////////////////////////////////////
// GameMsg_S2C_SyncMedalInfoToOthers
GameMsg_S2C_SyncMedalInfoToOthers::GameMsg_S2C_SyncMedalInfoToOthers()
    : GameMsg_Base(MSG_S2C_SyncMedalInfoToOthersNew)
    , m_nRoleID(0)
    , m_nSceneID(0)
{
}

GameMsg_S2C_SyncMedalInfoToOthers::~GameMsg_S2C_SyncMedalInfoToOthers()
{
}

bool GameMsg_S2C_SyncMedalInfoToOthers::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_nRoleID << m_nSceneID << m_displayInfo;
    return true;
}

bool GameMsg_S2C_SyncMedalInfoToOthers::doDecode(CParamPool &IOBuff)
{
    IOBuff >> m_nRoleID >> m_nSceneID >> m_displayInfo;
    return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_SyncHorseData::GameMsg_S2C_SyncHorseData()
    :GameMsg_Base(MSG_S2C_SYNC_HorseDataNew)
    , m_nHorseOwnerId(0)
    , m_nHorseSeatIdx(0)
    , m_nHorseTypeId(0)
    , m_nSelfRoleId(0)
{

}

GameMsg_S2C_SyncHorseData::~GameMsg_S2C_SyncHorseData()
{

}

bool GameMsg_S2C_SyncHorseData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nSelfRoleId);
    IOBuff.AddUInt(m_nHorseOwnerId);
    IOBuff.AddUInt(m_nHorseTypeId);
    IOBuff.AddInt(m_nHorseSeatIdx);
    IOBuff.AddUShort((unsigned short)m_nSceneId);

    return true;
}

bool GameMsg_S2C_SyncHorseData::doDecode(CParamPool &IOBuff)
{
    m_nSelfRoleId = IOBuff.GetUInt();
    m_nHorseOwnerId = IOBuff.GetUInt();
    m_nHorseTypeId = IOBuff.GetUInt();
    m_nHorseSeatIdx = IOBuff.GetInt();
    m_nSceneId = IOBuff.GetUShort();

    return true;
}

GameMsg_S2C_AddGene::GameMsg_S2C_AddGene()
    :GameMsg_Base(MSG_S2C_AddGeneNew)
, m_nRoleID(0)
{
}

GameMsg_S2C_AddGene::~GameMsg_S2C_AddGene()
{
}

bool GameMsg_S2C_AddGene::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    m_GeneData.doEncode(IOBuff);

    return true;
}

bool GameMsg_S2C_AddGene::doDecode(CParamPool & IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_GeneData.doDecode(IOBuff);

    return true;
}


GameMsg_S2C_DelGene::GameMsg_S2C_DelGene()
    :GameMsg_Base(MSG_S2C_DelGeneNew)
, m_nRoleID(0)
, m_nGeneID(0)
{
}

GameMsg_S2C_DelGene::~GameMsg_S2C_DelGene()
{
}

bool GameMsg_S2C_DelGene::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUShort(m_nGeneID);

    return true;
}

bool GameMsg_S2C_DelGene::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nGeneID = IOBuff.GetUShort();

    return true;
}


GameMsg_S2C_ResetGene::GameMsg_S2C_ResetGene()
    :GameMsg_Base(MSG_S2C_ResetGeneNew)
, m_nRoleID(0)
, m_nGeneID(0)
, m_nParam2(0)
{
}

GameMsg_S2C_ResetGene::~GameMsg_S2C_ResetGene()
{
}

bool GameMsg_S2C_ResetGene::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUShort(m_nGeneID);
    IOBuff.AddInt(m_nParam2);

    return true;
}

bool GameMsg_S2C_ResetGene::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nGeneID = IOBuff.GetUShort();
    m_nParam2 = IOBuff.GetInt();

    return true;
}
