#include "GameMsg_S2C_AmuseMsg.h"
#include "AmuseParam.h"
#include "AmuseEnum.h"
#include "../../datastructure/Macro_Define.h"
#include "../../server/share/ServerMsgDef.h"
#include "../Pet/PetService.h"
#include "../Medal/EntityMedalComponent.h"
#include "../Horse/EntityHorseComponent.h"


///////////////////////////////////////////////////////
CAmuseRoomPlayerInfo::CAmuseRoomPlayerInfo()
: m_nRoleID(0)
, m_bIsHost(false)
, m_nRoleSex(ESexType_No)
, m_nRoleSkin(1)
, m_nDanceGroupTitle(-1)
, m_nDanceGroupBadgeId(0)
, m_nDanceGroupEffectId(0)
, m_bIsVIP(false)
, m_nVIPLevel(0)
, m_nMoveType(0)
, m_nTransFormID(0)
, m_fOrient(0)
, m_fPosX(0)
, m_fPosY(0)
, m_fPosZ(0)
, m_pItemCOM(NULL)
, m_pGeneCOM(NULL)
, m_pPetCOM(NULL)
, m_pMedalCOM(NULL)
, m_pHorseCOM(NULL)
{
}

CAmuseRoomPlayerInfo::~CAmuseRoomPlayerInfo()
{
}

void CAmuseRoomPlayerInfo::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName.c_str());
    IOBuff.AddBool(m_bIsHost);
    IOBuff.AddUChar(m_nRoleSex);
    IOBuff.AddUChar(m_nRoleSkin);
    IOBuff.AddChar(m_nDanceGroupTitle);
    IOBuff.AddStringW(m_strDanceGroupName.c_str());
    IOBuff.AddUShort(m_nDanceGroupBadgeId);
    IOBuff.AddUShort(m_nDanceGroupEffectId);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddUChar(m_nMoveType);
    IOBuff.AddUShort(m_nTransFormID);
    IOBuff.AddFloat(m_fOrient);
    IOBuff.AddFloat(m_fPosX);
    IOBuff.AddFloat(m_fPosY);
    IOBuff.AddFloat(m_fPosZ);

    if (m_pItemCOM != NULL)
    {
        m_pItemCOM->PackItemForOther(IOBuff);
    }

    if (m_pGeneCOM != NULL)
    {
        m_pGeneCOM->PackGeneForOther(IOBuff);
    }

    if (m_pPetCOM != NULL)
    {
        Pet *pet = m_pPetCOM->GetCarriedPet();
        IOBuff.AddBool(pet != NULL ? true : false);
        if (pet != NULL)
        {
            PlayerPet petInfo;
            PetService::Instance().EncodePetBrief(*pet, petInfo);
            petInfo.doEncode(IOBuff);
        }
    }

    if (m_pMedalCOM != NULL)
    {
        m_pMedalCOM->SyncAllDisplayMedalsForOther(IOBuff);
    }

    if (m_pHorseCOM != NULL)
    {
        m_pHorseCOM->SyncAllDisplayHorse2Other(IOBuff);
    }
}


///////////////////////////////////////////////////////
CAmuseRoomDeviceInfo::CAmuseRoomDeviceInfo()
:m_nDeviceID(0)
, m_nInviteType(eAmuseInvite_None)
{
}

CAmuseRoomDeviceInfo::~CAmuseRoomDeviceInfo()
{
}

void CAmuseRoomDeviceInfo::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_nDeviceID);
    IOBuff.AddUChar(m_nInviteType);
}


///////////////////////////////////////////////////////
CAmuseRoomBriefInfo::CAmuseRoomBriefInfo()
:m_nRoomID(0)
, m_bHasPwd(false)
, m_chRoomTheme(false)
, m_chRoomScene(eAmuseTheme_None)
, m_chRoomColor(RoomColor_None)
, m_bIsTop(false)
{
    memset(m_szPlayerSex, (char)ESexType_No, MAX_AMUSEROOM_PLAYER_COUNT);
}

CAmuseRoomBriefInfo::~CAmuseRoomBriefInfo()
{
}

void CAmuseRoomBriefInfo::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoomID);
    IOBuff.AddStringW(m_strRoomName.c_str());
    IOBuff.AddBool(m_bHasPwd);
    IOBuff.AddUChar(m_chRoomTheme);
    IOBuff.AddUChar(m_chRoomScene);
    IOBuff.AddUChar(m_chRoomColor);
    IOBuff.AddBool(m_bIsTop);

    for (int i = 0; i < MAX_AMUSEROOM_PLAYER_COUNT; ++i)
    {
        IOBuff.AddUChar((char)m_szPlayerSex[i]);
    }
}


///////////////////////////////////////////////////////
CAmuseRoomWholeInfo::CAmuseRoomWholeInfo()
:m_nRoomID(0)
, m_bHasPwd(false)
, m_bIsHost(false)
, m_chRoomTheme(eAmuseTheme_None)
, m_chRoomScene(0)
, m_chRoomCapacity(0)
, m_chRoomType(0)
{
}

CAmuseRoomWholeInfo::~CAmuseRoomWholeInfo()
{
    for (std::list<CAmuseActionParam_Base*>::iterator it_Action = m_lstPlayerAction.begin(); it_Action != m_lstPlayerAction.end(); ++it_Action)
    {
        delete (*it_Action);
        (*it_Action) = NULL;
    }

    for (std::list<CAmuseEventParam_Base*>::iterator it_Event = m_lstRoomEvent.begin(); it_Event != m_lstRoomEvent.end(); ++it_Event)
    {
        delete (*it_Event);
        (*it_Event) = NULL;
    }

    m_lstPlayerAction.clear();
    m_lstRoomEvent.clear();
}

void CAmuseRoomWholeInfo::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoomID);
    IOBuff.AddStringW(m_strRoomName.c_str());
    IOBuff.AddStringW(m_strRoomPwd.c_str());
    IOBuff.AddBool(m_bHasPwd);
    IOBuff.AddBool(m_bIsHost);
    IOBuff.AddUChar(m_chRoomTheme);
    IOBuff.AddUChar(m_chRoomScene);
    IOBuff.AddUChar(m_chRoomCapacity);
    IOBuff.AddUChar(m_chRoomType);

    IOBuff.AddUShort((unsigned short)m_lstAmusePlayer.size());
    for (std::list<CAmuseRoomPlayerInfo>::iterator it_Player = m_lstAmusePlayer.begin(); it_Player != m_lstAmusePlayer.end(); ++it_Player)
    {
        (*it_Player).doEncode(IOBuff);
    }

    IOBuff.AddUShort((unsigned short)m_lstAmuseDevice.size());
    for (std::list<CAmuseRoomDeviceInfo>::iterator it_Device = m_lstAmuseDevice.begin(); it_Device != m_lstAmuseDevice.end(); ++it_Device)
    {
        (*it_Device).doEncode(IOBuff);
    }

    IOBuff.AddUShort((unsigned short)m_lstPlayerAction.size());
    for (std::list<CAmuseActionParam_Base*>::iterator it_Action = m_lstPlayerAction.begin(); it_Action != m_lstPlayerAction.end(); ++it_Action)
    {
        (*it_Action)->doEncode(IOBuff);
    }

    IOBuff.AddUShort((unsigned short)m_lstRoomEvent.size());
    for (std::list<CAmuseEventParam_Base*>::iterator it_Event = m_lstRoomEvent.begin(); it_Event != m_lstRoomEvent.end(); ++it_Event)
    {
        (*it_Event)->doEncode(IOBuff);
    }
}


///////////////////////////////////////////////////////
GameMsg_S2C_GetAmuseRoomListRes::GameMsg_S2C_GetAmuseRoomListRes(void)
:GameMsg_Base(MSG_S2C_GetAmuseRoomListRes)
, m_nCurPage(0)
, m_nTotalCount(0)
{
}

GameMsg_S2C_GetAmuseRoomListRes::~GameMsg_S2C_GetAmuseRoomListRes(void)
{
}

bool GameMsg_S2C_GetAmuseRoomListRes::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUShort(m_nCurPage);
    IOBuff.AddUShort(m_nTotalCount);
    IOBuff.AddUShort((unsigned short)m_vecRoomBriefInfo.size());

    for (size_t i = 0; i < m_vecRoomBriefInfo.size(); i++)
    {
        m_vecRoomBriefInfo[i].doEncode(IOBuff);
    }

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_CreateAmuseRoomSuc::GameMsg_S2C_CreateAmuseRoomSuc(void)
:GameMsg_Base(MSG_S2C_CreateAmuseRoomSuc)
{
}

GameMsg_S2C_CreateAmuseRoomSuc::~GameMsg_S2C_CreateAmuseRoomSuc(void)
{
}

bool GameMsg_S2C_CreateAmuseRoomSuc::doEncode(CParamPool& IOBuff)
{
    m_RoomWholeInfo.doEncode(IOBuff);

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_CreateAmuseRoomFail::GameMsg_S2C_CreateAmuseRoomFail(void)
:GameMsg_Base(MSG_S2C_CreateAmuseRoomFail)
, m_strError("")
{
}

GameMsg_S2C_CreateAmuseRoomFail::~GameMsg_S2C_CreateAmuseRoomFail(void)
{
}

bool GameMsg_S2C_CreateAmuseRoomFail::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_AmuseRoomRequriePwd::GameMsg_S2C_AmuseRoomRequriePwd()
:GameMsg_Base(MSG_S2C_AmuseRoomRequirePwd)
, m_nRoomID(0)
{

}

GameMsg_S2C_AmuseRoomRequriePwd::~GameMsg_S2C_AmuseRoomRequriePwd()
{

}

bool GameMsg_S2C_AmuseRoomRequriePwd::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUInt(m_nRoomID);

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_EnterAmuseRoomFail::GameMsg_S2C_EnterAmuseRoomFail(void)
:GameMsg_Base(MSG_S2C_EnterAmuseRoomFail)
, m_strError("")
{
}

GameMsg_S2C_EnterAmuseRoomFail::~GameMsg_S2C_EnterAmuseRoomFail(void)
{
}

bool GameMsg_S2C_EnterAmuseRoomFail::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_QuitAmuseRoomSuc::GameMsg_S2C_QuitAmuseRoomSuc(void)
:GameMsg_Base(MSG_S2C_QuitAmuseRoomSuc)
, m_chQuitType(eAmuseRoomQuit_Active)
, m_nKickerID(0)
, m_nQuitTo(0)
{
}

GameMsg_S2C_QuitAmuseRoomSuc::~GameMsg_S2C_QuitAmuseRoomSuc(void)
{
}

bool GameMsg_S2C_QuitAmuseRoomSuc::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_chQuitType);
    IOBuff.AddUInt(m_nKickerID);
    IOBuff.AddUChar(m_nQuitTo);

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_QuitAmuseRoomFail::GameMsg_S2C_QuitAmuseRoomFail(void)
:GameMsg_Base(MSG_S2C_QuitAmuseRoomFail)
, m_strError("")
{
}

GameMsg_S2C_QuitAmuseRoomFail::~GameMsg_S2C_QuitAmuseRoomFail(void)
{
}

bool GameMsg_S2C_QuitAmuseRoomFail::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_ChangeAmuseRoomInfoSuc::GameMsg_S2C_ChangeAmuseRoomInfoSuc(void)
:GameMsg_Base(MSG_S2C_ChangeAmuseRoomInfoSuc)
, m_strRoomName("")
, m_strRoomPwd("")
, m_bHasPwd(false)
{
}

GameMsg_S2C_ChangeAmuseRoomInfoSuc::~GameMsg_S2C_ChangeAmuseRoomInfoSuc(void)
{
}

bool GameMsg_S2C_ChangeAmuseRoomInfoSuc::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddStringW(m_strRoomName.c_str());
    IOBuff.AddStringW(m_strRoomPwd.c_str());
    IOBuff.AddBool(m_bHasPwd);

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_ChangeAmuseRoomInfoFail::GameMsg_S2C_ChangeAmuseRoomInfoFail(void)
:GameMsg_Base(MSG_S2C_ChangeAmuseRoomInfoFail)
, m_strError("")
{
}

GameMsg_S2C_ChangeAmuseRoomInfoFail::~GameMsg_S2C_ChangeAmuseRoomInfoFail(void)
{
}

bool GameMsg_S2C_ChangeAmuseRoomInfoFail::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_AmuseKickPlayerFail::GameMsg_S2C_AmuseKickPlayerFail(void)
:GameMsg_Base(MSG_S2C_AmuseKickPlayerFail)
, m_strError("")
{
}

GameMsg_S2C_AmuseKickPlayerFail::~GameMsg_S2C_AmuseKickPlayerFail(void)
{
}

bool GameMsg_S2C_AmuseKickPlayerFail::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_AmuseEnterPlayer::GameMsg_S2C_AmuseEnterPlayer(void)
:GameMsg_Base(MSG_S2C_AmuseEnterPlayer)
{
}

GameMsg_S2C_AmuseEnterPlayer::~GameMsg_S2C_AmuseEnterPlayer(void)
{
}

bool GameMsg_S2C_AmuseEnterPlayer::doEncode(CParamPool& IOBuff)
{
    m_PlayerInfo.doEncode(IOBuff);

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_AmuseQuitPlayer::GameMsg_S2C_AmuseQuitPlayer(void)
:GameMsg_Base(MSG_S2C_AmuseQuitPlayer)
, m_chQuitType(eAmuseRoomQuit_Active)
, m_nQuitID(0)
, m_nNewHostID(0)
{
}

GameMsg_S2C_AmuseQuitPlayer::~GameMsg_S2C_AmuseQuitPlayer(void)
{
}

bool GameMsg_S2C_AmuseQuitPlayer::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_chQuitType);
    IOBuff.AddUInt(m_nQuitID);
    IOBuff.AddUInt(m_nNewHostID);
    IOBuff.AddStringW(m_strRoomPwd.c_str());

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_AmusePromoteHostSuc::GameMsg_S2C_AmusePromoteHostSuc()
:GameMsg_Base(MSG_S2C_AmusePromoteHostSuc)
, m_nOldHostID(0)
, m_nNewHostID(0)
, m_strRoomPwd("")
{

}

GameMsg_S2C_AmusePromoteHostSuc::~GameMsg_S2C_AmusePromoteHostSuc()
{

}

bool GameMsg_S2C_AmusePromoteHostSuc::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nOldHostID);
    IOBuff.AddUInt(m_nNewHostID);
    IOBuff.AddStringW(m_strRoomPwd.c_str());

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_AmusePromoteHostFail::GameMsg_S2C_AmusePromoteHostFail()
:GameMsg_Base(MSG_S2C_AmusePromoteHostFail)
, m_strError("")
{

}

GameMsg_S2C_AmusePromoteHostFail::~GameMsg_S2C_AmusePromoteHostFail()
{

}

bool GameMsg_S2C_AmusePromoteHostFail::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_AmuseInvitePlayerSuc::GameMsg_S2C_AmuseInvitePlayerSuc(void)
:GameMsg_Base(MSG_S2C_AmuseInvitePlayerSuc)
{
}

GameMsg_S2C_AmuseInvitePlayerSuc::~GameMsg_S2C_AmuseInvitePlayerSuc(void)
{
}

bool GameMsg_S2C_AmuseInvitePlayerSuc::doEncode(CParamPool& IOBuff)
{
    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_AmuseInvitePlayerFail::GameMsg_S2C_AmuseInvitePlayerFail(void)
:GameMsg_Base(MSG_S2C_AmuseInvitePlayerFail)
, m_strError("")
{
}

GameMsg_S2C_AmuseInvitePlayerFail::~GameMsg_S2C_AmuseInvitePlayerFail(void)
{
}

bool GameMsg_S2C_AmuseInvitePlayerFail::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_AmuseInviteeNotice::GameMsg_S2C_AmuseInviteeNotice(void)
:GameMsg_Base(MSG_S2C_AmuseInviteeNotice)
, m_nRoomID(0)
, m_nInviterRoleID(0)
, m_strInviterName("")
, m_bIsInviterVIP(false)
, m_nInviterVIPLevel(0)
{
}

GameMsg_S2C_AmuseInviteeNotice::~GameMsg_S2C_AmuseInviteeNotice(void)
{
}

bool GameMsg_S2C_AmuseInviteeNotice::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nRoomID);
    IOBuff.AddUInt(m_nInviterRoleID);
    IOBuff.AddStringW(m_strInviterName.c_str());
    IOBuff.AddBool(m_bIsInviterVIP);
    IOBuff.AddUShort(m_nInviterVIPLevel);

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_AmuseInviteeFail::GameMsg_S2C_AmuseInviteeFail(void)
:GameMsg_Base(MSG_S2C_AmuseInviteeFail)
, m_strError("")
{
}

GameMsg_S2C_AmuseInviteeFail::~GameMsg_S2C_AmuseInviteeFail(void)
{
}

bool GameMsg_S2C_AmuseInviteeFail::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_DoAmuseActionSuc::GameMsg_S2C_DoAmuseActionSuc(void)
:GameMsg_Base(MSG_S2C_DoAmuseActionSuc)
, m_pActionParam(NULL)
{
}

GameMsg_S2C_DoAmuseActionSuc::~GameMsg_S2C_DoAmuseActionSuc(void)
{
    if (m_pActionParam != NULL)
    {
        delete m_pActionParam;
        m_pActionParam = NULL;
    }
}

bool GameMsg_S2C_DoAmuseActionSuc::doEncode(CParamPool& IOBuff)
{
    m_pActionParam->doEncode(IOBuff);

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_DoAmuseActionFail::GameMsg_S2C_DoAmuseActionFail(void)
:GameMsg_Base(MSG_S2C_DoAmuseActionFail)
, m_strError("")
{
}

GameMsg_S2C_DoAmuseActionFail::~GameMsg_S2C_DoAmuseActionFail(void)
{
}

bool GameMsg_S2C_DoAmuseActionFail::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_AmuseEventNotice::GameMsg_S2C_AmuseEventNotice(void)
:GameMsg_Base(MSG_S2C_AmuseEventNotice)
, m_pEventParam(NULL)
{
}

GameMsg_S2C_AmuseEventNotice::~GameMsg_S2C_AmuseEventNotice(void)
{
    if (m_pEventParam != NULL)
    {
        delete m_pEventParam;
        m_pEventParam = NULL;
    }
}

bool GameMsg_S2C_AmuseEventNotice::doEncode(CParamPool& IOBuff)
{
    m_pEventParam->doEncode(IOBuff);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_PlayMakerBroadcast::GameMsg_S2C_PlayMakerBroadcast(void)
:GameMsg_Base(MSG_S2C_PlayMakerBroadcast)
{
}

GameMsg_S2C_PlayMakerBroadcast::~GameMsg_S2C_PlayMakerBroadcast(void)
{
}

bool GameMsg_S2C_PlayMakerBroadcast::doEncode(CParamPool& IOBuff)
{
    m_playerMakerInfo.doEncode(IOBuff);

    return true;
}

