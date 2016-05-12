#include "GameMsgNewRoom.h"
#include "../share/ServerMsgDef.h"


GameMsg_C2S_NewCreateRoom::GameMsg_C2S_NewCreateRoom(void)
    : GameMsg_Base(MSG_C2S_NewCreateRoom)
    , m_chRoomType(0)
    , m_chPhoneOS(0)
{

}
GameMsg_C2S_NewCreateRoom::~GameMsg_C2S_NewCreateRoom(void)
{

}

bool GameMsg_C2S_NewCreateRoom::doDecode(CParamPool& IOBuff)
{
    IOBuff.GetStringW(m_strRoomName);
    IOBuff.GetStringW(m_strRoomPwd);
    m_chRoomType = IOBuff.GetUChar();
    m_chPhoneOS = IOBuff.GetUChar();

    return true;
}

bool GameMsg_C2S_NewCreateRoom::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddStringW(m_strRoomName);
    IOBuff.AddStringW(m_strRoomPwd);
    IOBuff.AddUChar(m_chRoomType);
    IOBuff.AddUChar(m_chPhoneOS);

    return true;
}


GameMsg_C2S_NewTryEnterRoom::GameMsg_C2S_NewTryEnterRoom(void)
    : GameMsg_Base(MSG_C2S_NewTryEnterRoom)
    , m_nRoleRoomType(0)
    , m_chPhoneOS(0)
    , m_nRoomID(0)
    , m_nRoomType(0)
{

}

GameMsg_C2S_NewTryEnterRoom::~GameMsg_C2S_NewTryEnterRoom(void){}

bool GameMsg_C2S_NewTryEnterRoom::doDecode(CParamPool& IOBuff)
{
    m_nRoleRoomType = IOBuff.GetUChar();
    m_chPhoneOS = IOBuff.GetUChar();
    m_nRoomID = IOBuff.GetUInt();
    m_nRoomType = IOBuff.GetUChar();

    return true;
}

bool GameMsg_C2S_NewTryEnterRoom::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_nRoleRoomType);
    IOBuff.AddUChar(m_chPhoneOS);
    IOBuff.AddUInt(m_nRoomID);
    IOBuff.AddUChar(m_nRoomType);

    return true;
}



GameMsg_C2S_NewQuitRoom::GameMsg_C2S_NewQuitRoom(void)
    : GameMsg_Base(MSG_C2S_NewQuitRoom)
    , m_chQuitTo(0)
{

}
GameMsg_C2S_NewQuitRoom::~GameMsg_C2S_NewQuitRoom(void)
{

}

bool GameMsg_C2S_NewQuitRoom::doDecode(CParamPool& IOBuff)
{
    m_chQuitTo = IOBuff.GetUChar();

    return true;
}
bool GameMsg_C2S_NewQuitRoom::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_chQuitTo);

    return true;
}



GameMsg_C2S_NewPromoteRoomHost::GameMsg_C2S_NewPromoteRoomHost()
    : GameMsg_Base(MSG_C2S_NewPromoteRoomHost)
    , m_chNewHostRoleRoomType(0)
    , m_chNewHostRoleRoomPos(0)
{

}
GameMsg_C2S_NewPromoteRoomHost::~GameMsg_C2S_NewPromoteRoomHost()
{

}

bool GameMsg_C2S_NewPromoteRoomHost::doDecode(CParamPool& IOBuff)
{
    m_chNewHostRoleRoomType = IOBuff.GetUChar();
    m_chNewHostRoleRoomPos = IOBuff.GetUChar();

    return true;
}

bool GameMsg_C2S_NewPromoteRoomHost::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_chNewHostRoleRoomType);
    IOBuff.AddUChar(m_chNewHostRoleRoomPos);

    return true;
}


GameMsg_C2S_NewKickPlayer::GameMsg_C2S_NewKickPlayer(void)
    : GameMsg_Base(MSG_C2S_NewKickPlayer)
    , m_chRoleRoomType(0)
    , m_chRoleRoomPos(0)
    , m_nRoleID(0)
{

}
GameMsg_C2S_NewKickPlayer::~GameMsg_C2S_NewKickPlayer(void)
{

}

bool GameMsg_C2S_NewKickPlayer::doDecode(CParamPool& IOBuff)
{
    m_chRoleRoomType = IOBuff.GetUChar();
    m_chRoleRoomPos = IOBuff.GetUChar();
    m_nRoleID = IOBuff.GetUInt();

    return true;
}
bool GameMsg_C2S_NewKickPlayer::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_chRoleRoomType);
    IOBuff.AddUChar(m_chRoleRoomPos);
    IOBuff.AddUInt(m_nRoleID);

    return true;
}


GameMsg_C2S_NewChangeRoleRoomState::GameMsg_C2S_NewChangeRoleRoomState(void)
    : GameMsg_Base(MSG_C2S_NewChangeRoleRoomState)
    , m_chRoomType(0)
    , m_chRoleRoomState(0)
{

}
GameMsg_C2S_NewChangeRoleRoomState::~GameMsg_C2S_NewChangeRoleRoomState(void)
{

}

bool GameMsg_C2S_NewChangeRoleRoomState::doDecode(CParamPool& IOBuff)
{
    m_chRoomType = IOBuff.GetUChar();
    m_chRoleRoomState = IOBuff.GetUChar();

    return true;
}
bool GameMsg_C2S_NewChangeRoleRoomState::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_chRoomType);
    IOBuff.AddUChar(m_chRoleRoomState);

    return true;
}


GameMsg_C2S_NewChangeRoomPosState::GameMsg_C2S_NewChangeRoomPosState(void)
    : GameMsg_Base(MSG_C2S_NewChangeRoomPosState)
    , m_chRoleRoomType(0)
    , m_chRoleRoomPos(0)
    , m_chRoomPosState(0)
{

}

GameMsg_C2S_NewChangeRoomPosState::~GameMsg_C2S_NewChangeRoomPosState(void)
{

}

bool GameMsg_C2S_NewChangeRoomPosState::doDecode(CParamPool& IOBuff)
{
    m_chRoleRoomType = IOBuff.GetUChar();
    m_chRoleRoomPos = IOBuff.GetUChar();
    m_chRoomPosState = IOBuff.GetUChar();

    return true;
}
bool GameMsg_C2S_NewChangeRoomPosState::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_chRoleRoomType);
    IOBuff.AddUChar(m_chRoleRoomPos);
    IOBuff.AddUChar(m_chRoomPosState);

    return true;
}



GameMsg_S2C_NewCreateRoomSuc::GameMsg_S2C_NewCreateRoomSuc(void)
    : GameMsg_Base(MSG_S2C_NewCreateRoomSuc)
    , m_nRoomType(0)
{

}
GameMsg_S2C_NewCreateRoomSuc::~GameMsg_S2C_NewCreateRoomSuc(void)
{}

bool GameMsg_S2C_NewCreateRoomSuc::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_nRoomType);
    m_RoomInfo.doEncode(IOBuff);
    
    return true;
}

bool GameMsg_S2C_NewCreateRoomSuc::doDecode(CParamPool& IOBuff)
{
    m_nRoomType = IOBuff.GetUChar();
    m_RoomInfo.doDecode(IOBuff);

    return true;
}

GameMsg_S2G_TransFromIDUpdate::GameMsg_S2G_TransFromIDUpdate(void)
    : GameMsg_Base(MSG_S2G_TransFromIDUpdate)
    , m_nTransformId(0)
{

}
GameMsg_S2G_TransFromIDUpdate::~GameMsg_S2G_TransFromIDUpdate(void)
{

}

bool GameMsg_S2G_TransFromIDUpdate::doDecode(CParamPool& IOBuff)
{
    m_nTransformId = IOBuff.GetInt();
    return true;
}

bool GameMsg_S2G_TransFromIDUpdate::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddInt(m_nTransformId);
    return true;
}


GameMsg_S2G_OnEquipNotify::GameMsg_S2G_OnEquipNotify(void)
    : GameMsg_Base(MSG_S2G_OnEquipNotify)
    , m_nEffectID(0)
{

}
GameMsg_S2G_OnEquipNotify::~GameMsg_S2G_OnEquipNotify(void)
{

}

bool GameMsg_S2G_OnEquipNotify::doDecode(CParamPool& IOBuff)
{
    m_dbEquip.doDecode(IOBuff);
    m_nEffectID = IOBuff.GetUInt();
    return true;
}
bool GameMsg_S2G_OnEquipNotify::doEncode(CParamPool& IOBuff)
{
    m_dbEquip.doEncode(IOBuff);
    IOBuff.AddUInt(m_nEffectID);

    return true;
}

GameMsg_S2G_UnEquipNotify::GameMsg_S2G_UnEquipNotify(void)
    : GameMsg_Base(MSG_S2G_UnEquipNotify)
    , m_nItemID(0)
{

}

GameMsg_S2G_UnEquipNotify::~GameMsg_S2G_UnEquipNotify(void)
{

}

bool GameMsg_S2G_UnEquipNotify::doDecode(CParamPool& IOBuff)
{
    m_nItemID = IOBuff.GetInt64();
    m_nColumn = IOBuff.GetUShort();
    m_nIndex = IOBuff.GetUShort();
    return true;
}

bool GameMsg_S2G_UnEquipNotify::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddInt64(m_nItemID);
    IOBuff.AddUShort(m_nColumn);
    IOBuff.AddUShort(m_nIndex);

    return true;
}



GameMsg_S2G_EffectChangeNotify::GameMsg_S2G_EffectChangeNotify(void)
    : GameMsg_Base(MSG_S2G_EffectChangeNotify)
    , m_nItemID(0)
    , m_nEffectID(0)
{

}
GameMsg_S2G_EffectChangeNotify::~GameMsg_S2G_EffectChangeNotify(void)
{

}

bool GameMsg_S2G_EffectChangeNotify::doDecode(CParamPool& IOBuff)
{
    m_nItemID = IOBuff.GetInt64();
    m_nEffectID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_EffectChangeNotify::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddInt64(m_nItemID);
    IOBuff.AddUInt(m_nEffectID);

    return true;
}


GameMsg_S2G_RefreshSevenColor::GameMsg_S2G_RefreshSevenColor(void)
    : GameMsg_Base(MSG_S2G_RefreshSevenColor)
    , m_nSuitID(0)
    , m_nColor(0)
{

}

GameMsg_S2G_RefreshSevenColor::~GameMsg_S2G_RefreshSevenColor(void)
{

}

bool GameMsg_S2G_RefreshSevenColor::doDecode(CParamPool& IOBuff)
{
    m_nSuitID = IOBuff.GetUShort();
    m_nColor = IOBuff.GetUChar();

    return true;
}

bool GameMsg_S2G_RefreshSevenColor::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUShort(m_nSuitID);
    IOBuff.AddUChar(m_nColor);

    return true;
}

GameMsg_S2G_RefreshGeneNotify::GameMsg_S2G_RefreshGeneNotify(void)
    : GameMsg_Base(MSG_S2G_RefreshGeneNotify)
    , m_eAction(EGeneNotifyAction_None)
{

}
GameMsg_S2G_RefreshGeneNotify::~GameMsg_S2G_RefreshGeneNotify(void)
{

}

bool GameMsg_S2G_RefreshGeneNotify::doDecode(CParamPool& IOBuff)
{
    m_eAction = (EGeneNotifyAction)IOBuff.GetInt();
    m_GeneData.doDecode(IOBuff);

    return true;
}

bool GameMsg_S2G_RefreshGeneNotify::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddInt((int)m_eAction);
    m_GeneData.doEncode(IOBuff);

    return true;
}



GameMsg_S2G_AddMedals::GameMsg_S2G_AddMedals(void)
    : GameMsg_Base(MSG_S2G_AddMedals)
    , m_nSceneID(0)
    , m_nMedalID(0)
{

}

GameMsg_S2G_AddMedals::~GameMsg_S2G_AddMedals(void)
{

}


bool GameMsg_S2G_AddMedals::doDecode(CParamPool& IOBuff)
{
    m_nSceneID = IOBuff.GetUChar();
    m_nMedalID = IOBuff.GetInt();

    return true;
}

bool GameMsg_S2G_AddMedals::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_nSceneID);
    IOBuff.AddInt(m_nMedalID);

    return true;
}

