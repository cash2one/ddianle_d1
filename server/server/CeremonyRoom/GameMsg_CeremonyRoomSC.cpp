#include "GameMsg_CeremonyRoomSC.h"
#include "../dancegroup/DanceGroupMsgDef.h"
#include "../../socket/ParamPool.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "../../socket/inc/SocketLib.h"
#include "../item/EntityItemComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../Pet/PetComponent.h"
#include "../Pet/PetService.h"
#include "../Medal/EntityMedalComponent.h"
#include "../Horse/EntityHorseComponent.h"
#include "ceremonyRoomHead.h"

CeremonyPlayerBaseInfo::CeremonyPlayerBaseInfo()
    : m_nPlayerID(0)
    , m_nPlayerSex(0)
    , m_bIsVIP(false)
    , m_nVIPLevel(0)
    , m_nDanceGroupPos(0)
    , m_nDanceGroupProperty(0)
    , m_nPlayerType(0)
    , m_nRookieIndex(0)
{

}

bool CeremonyPlayerBaseInfo::DoEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nPlayerID);
    IOBuff.AddStringW(m_strPlayerName);
    IOBuff.AddUChar(m_nPlayerSex);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddUChar(m_nDanceGroupPos);
    IOBuff.AddUChar(m_nDanceGroupProperty);
    IOBuff.AddUChar(m_nPlayerType);
    IOBuff.AddInt(m_nRookieIndex);

    return true;
}


CeremonyPlayerShowInfo::CeremonyPlayerShowInfo()
    : CeremonyPlayerBaseInfo()
    , m_nDanceGroupBadgeId(0)
    , m_nDanceGroupEffectId(0)
    , m_nMoveType(0)
    , m_fOrient(.0f)
    , m_fPosX(.0f)
    , m_fPosY(.0f)
    , m_fPosZ(.0f)
    , m_nPlayerSkin(0)
    , m_nTransformId(0)
    , m_pItemCOM(NULL)
    , m_pGeneCOM(NULL)
    , m_pPetCOM(NULL)
    , m_pMedalCOM( NULL )
    , m_pHorseCOM( NULL )
{

}

bool CeremonyPlayerShowInfo::DoEncode(CParamPool &IOBuff)
{
    CeremonyPlayerBaseInfo::DoEncode(IOBuff);

    if (m_nPlayerType != ECeremonyPlayerType_None)
    {
        IOBuff.AddUShort(m_nDanceGroupBadgeId);
        IOBuff.AddUShort(m_nDanceGroupEffectId);
        IOBuff.AddStringW(m_strDanceGroupName);

        IOBuff.AddUChar(m_nMoveType);
        IOBuff.AddFloat(m_fOrient);
        IOBuff.AddFloat(m_fPosX);
        IOBuff.AddFloat(m_fPosY);
        IOBuff.AddFloat(m_fPosZ);
        IOBuff.AddUChar(m_nPlayerSkin);
        IOBuff.AddUShort(m_nTransformId);

        if( m_pItemCOM != NULL )
        {
            m_pItemCOM->PackItemForOther( IOBuff );
        }

        if ( m_pGeneCOM != NULL )
        {
            m_pGeneCOM->PackGeneForOther( IOBuff );
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

        if ( m_pMedalCOM != NULL )
        {
            m_pMedalCOM->SyncAllDisplayMedalsForOther( IOBuff );
        }

        if (m_pHorseCOM != NULL)
        {
            m_pHorseCOM->SyncAllDisplayHorse2Other(IOBuff);
        }
    }

    return true;
}


CeremonyRookieInfo::CeremonyRookieInfo()
    : m_nRoleID(0)
    , m_nRookieIndex(0)
{

}
bool CeremonyRookieInfo::DoEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddInt(m_nRookieIndex);
    IOBuff.AddStringW(m_strIntruduction);

    return true;
}


CeremonyRoomInfo::CeremonyRoomInfo()
    : m_nCeremonyState(0)
    , m_nCeremontyCD(.0f)
{

}

void CeremonyRoomInfo::DoEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nCeremonyState);
    IOBuff.AddFloat(m_nCeremontyCD);

    IOBuff.AddUShort((unsigned short)m_vecCeremonyPlayers.size());
    for (size_t i = 0; i < m_vecCeremonyPlayers.size(); ++i)
    {
        m_vecCeremonyPlayers[i].DoEncode(IOBuff);
    }
}

GameMsg_C2S_RequestApplyCeremony::GameMsg_C2S_RequestApplyCeremony() 
    : GameMsg_Base(MSG_C2S_RequestApplyCeremony)
{
}

bool GameMsg_C2S_RequestApplyCeremony::doDecode(CParamPool &IOBuff)
{
    return true;
}



GameMsg_S2C_RequestApplyCeremonyRes::GameMsg_S2C_RequestApplyCeremonyRes() 
    : GameMsg_Base(MSG_S2C_RequestApplyCeremonyRes)
    , m_nErrorType(0)
{
}

bool GameMsg_S2C_RequestApplyCeremonyRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);

    return true;
}


GameMsg_C2S_RequestEnterCeremony::GameMsg_C2S_RequestEnterCeremony() 
	: GameMsg_Base(MSG_C2S_RequestEnterCeremony)
{
}

bool GameMsg_C2S_RequestEnterCeremony::doDecode(CParamPool &IOBuff)
{
    return true;
}


GameMsg_S2C_EnterCeremonyRes::GameMsg_S2C_EnterCeremonyRes() 
	: GameMsg_Base(MSG_S2C_EnterCeremonyRes)
	, m_nErrorType(0)
{
}

bool GameMsg_S2C_EnterCeremonyRes::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nErrorType);
	m_CeremonyRoomInfo.DoEncode(IOBuff);

	return true;
}


GameMsg_C2S_RequestQuitCeremony::GameMsg_C2S_RequestQuitCeremony() 
    : GameMsg_Base(MSG_C2S_RequestQuitCeremony)
{
}

bool GameMsg_C2S_RequestQuitCeremony::doDecode(CParamPool &IOBuff)
{
    m_nQuitTo = IOBuff.GetUChar();
    return true;
}


GameMsg_S2C_QuitCeremonyRes::GameMsg_S2C_QuitCeremonyRes() 
    : GameMsg_Base(MSG_S2C_QuitCeremonyRes)
    , m_nErrorType(0)
    , m_nQuitType(0)
    , m_nKickerID(0)
    , m_nQuitTo(0)
{
}

bool GameMsg_S2C_QuitCeremonyRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);
    IOBuff.AddUChar(m_nQuitType);
    IOBuff.AddUInt(m_nKickerID);
    IOBuff.AddChar(m_nQuitTo);
    return true;
}


GameMsg_C2S_RequestRefreshCeremony::GameMsg_C2S_RequestRefreshCeremony() 
    : GameMsg_Base(MSG_C2S_RequestRefreshCeremony)
{
}

bool GameMsg_C2S_RequestRefreshCeremony::doDecode(CParamPool &IOBuff)
{
    return true;
}


GameMsg_S2C_RefreshCeremonyRes::GameMsg_S2C_RefreshCeremonyRes() 
    : GameMsg_Base(MSG_S2C_RefreshCeremonyRes)
    , m_nErrorType(0)
{
}

bool GameMsg_S2C_RefreshCeremonyRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);
    m_CeremonyRoomInfo.DoEncode(IOBuff);

    return true;
}

GameMsg_C2S_RequestStartCeremony::GameMsg_C2S_RequestStartCeremony() 
    : GameMsg_Base(MSG_C2S_RequestStartCeremony)
{
}

bool GameMsg_C2S_RequestStartCeremony::doDecode(CParamPool &IOBuff)
{
    return true;
}


GameMsg_S2C_StartCeremonyRes::GameMsg_S2C_StartCeremonyRes() 
    : GameMsg_Base(MSG_S2C_StartCeremonyRes)
    , m_nErrorType(0)
{
}

bool GameMsg_S2C_StartCeremonyRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);

    IOBuff.AddUShort((unsigned short)m_vecRookieInfos.size());
    for (size_t i = 0; i < m_vecRookieInfos.size(); ++i)
    {
        m_vecRookieInfos[i].DoEncode(IOBuff);
    }

    return true;
}

GameMsg_S2C_OtherPlayerEnterCeremony::GameMsg_S2C_OtherPlayerEnterCeremony() 
    : GameMsg_Base(MSG_S2C_OtherPlayerEnterCeremony)
{
}

bool GameMsg_S2C_OtherPlayerEnterCeremony::doEncode(CParamPool &IOBuff)
{
    m_PlayerInfo.DoEncode(IOBuff);

    return true;
}

GameMsg_S2C_OtherPlayerQuitCeremony::GameMsg_S2C_OtherPlayerQuitCeremony() 
    : GameMsg_Base(MSG_S2C_OtherPlayerQuitCeremony)
    , m_nPlayerID(0)
{
}

bool GameMsg_S2C_OtherPlayerQuitCeremony::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nPlayerID);

    return true;
}

GameMsg_C2S_SetCeremonyRookie::GameMsg_C2S_SetCeremonyRookie() 
    : GameMsg_Base(MSG_C2S_SetCeremonyRookie)
    , m_nRoleId(0)
    , m_nRookieIndex(0)
{
}

bool GameMsg_C2S_SetCeremonyRookie::doDecode(CParamPool &IOBuff)
{
    m_nRoleId = IOBuff.GetUInt();
    m_nRookieIndex = IOBuff.GetInt();

    return true;
}


GameMsg_S2C_SetCeremonyRookieRes::GameMsg_S2C_SetCeremonyRookieRes() 
    : GameMsg_Base(MSG_S2C_SetCeremonyRookieRes)
    , m_nErrorType(0)
{
}

bool GameMsg_S2C_SetCeremonyRookieRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);

    return true;
}


GameMsg_C2S_CancelCeremonyRookie::GameMsg_C2S_CancelCeremonyRookie() 
    : GameMsg_Base(MSG_C2S_CancelCeremonyRookie)
    , m_nRoleId(0)
{
}

bool GameMsg_C2S_CancelCeremonyRookie::doDecode(CParamPool &IOBuff)
{
    m_nRoleId = IOBuff.GetUInt();

    return true;
}

GameMsg_S2C_CancelCeremonyRookieRes::GameMsg_S2C_CancelCeremonyRookieRes() 
    : GameMsg_Base(MSG_S2C_CancelCeremonyRookieRes)
    , m_nErrorType(0)
{
}

bool GameMsg_S2C_CancelCeremonyRookieRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);

    return true;
}

GameMsg_S2C_SyncCeremonyRookie::GameMsg_S2C_SyncCeremonyRookie() 
    : GameMsg_Base(MSG_S2C_SyncCeremonyRookie)
    , m_nRoleId(0)
    , m_bIsRookie(false)
{
}

bool GameMsg_S2C_SyncCeremonyRookie::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleId);
    IOBuff.AddBool(m_bIsRookie);

    if (m_bIsRookie)
    {
        m_PlayerInfo.DoEncode(IOBuff);
    }

    return true;
}

GameMsg_C2S_KickCeremonyPlayer::GameMsg_C2S_KickCeremonyPlayer() 
    : GameMsg_Base(MSG_C2S_KickCeremonyPlayer)
    , m_nRoleId(0)
{
}

bool GameMsg_C2S_KickCeremonyPlayer::doDecode(CParamPool &IOBuff)
{
    m_nRoleId = IOBuff.GetUInt();

    return true ;
}


GameMsg_S2C_KickCeremonyPlayerRes::GameMsg_S2C_KickCeremonyPlayerRes() 
    : GameMsg_Base(MSG_S2C_KickCeremonyPlayerRes)
    , m_nErrorType(0)
    , m_nRoleId(0)
{
}

bool GameMsg_S2C_KickCeremonyPlayerRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);
    IOBuff.AddUInt(m_nRoleId);

    return true;
}

GameMsg_C2S_SaveCeremonyIntruduction::GameMsg_C2S_SaveCeremonyIntruduction() 
    : GameMsg_Base(MSG_C2S_SaveCeremonyIntruduction)
{
}

bool GameMsg_C2S_SaveCeremonyIntruduction::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_strIntruduction);

    return true ;
}

GameMsg_S2C_SaveCeremonyIntruductionRes::GameMsg_S2C_SaveCeremonyIntruductionRes()
    : GameMsg_Base(MSG_S2C_SaveCeremonyIntruductionRes)
    , m_nErrorType(0)
{

}

GameMsg_S2C_SaveCeremonyIntruductionRes::~GameMsg_S2C_SaveCeremonyIntruductionRes()
{

}
bool GameMsg_S2C_SaveCeremonyIntruductionRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);

    return true;
}


GameMsg_C2S_RequestPraiseRookie::GameMsg_C2S_RequestPraiseRookie() 
    : GameMsg_Base(MSG_C2S_RequestPraiseRookie)
    , m_nRookieIndex(0)
    , m_nRoleID(0)
    , m_nCount(0)
{
}

bool GameMsg_C2S_RequestPraiseRookie::doDecode(CParamPool &IOBuff)
{
    m_nRookieIndex = IOBuff.GetInt();
    m_nRoleID = IOBuff.GetUInt();
    m_nCount = IOBuff.GetInt();

    return true ;
}

GameMsg_S2C_RequestPraiseRookieRes::GameMsg_S2C_RequestPraiseRookieRes() 
    : GameMsg_Base(MSG_S2C_RequestPraiseRookieRes)
    , m_nErrorType(0)
{
}

bool GameMsg_S2C_RequestPraiseRookieRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);

    return true;
}

GameMsg_S2C_SyncPraiseInfo::GameMsg_S2C_SyncPraiseInfo()
    : GameMsg_Base(MSG_S2C_SyncPraiseInfo)
    , m_roleID(0)
    , m_praiseNum(0)
{
    
}

GameMsg_S2C_SyncPraiseInfo::~GameMsg_S2C_SyncPraiseInfo()
{

}

bool GameMsg_S2C_SyncPraiseInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_roleID);
    IOBuff.AddUInt(m_praiseNum);

    return true;
}

GameMsg_S2C_CeremonyRoomStartFail::GameMsg_S2C_CeremonyRoomStartFail()
    : GameMsg_Base(MSG_S2C_CeremonyRoomStartFail)
    , m_nFailReason(0)
{

}

GameMsg_S2C_CeremonyRoomStartFail::~GameMsg_S2C_CeremonyRoomStartFail()
{

}

bool GameMsg_S2C_CeremonyRoomStartFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nFailReason);

    return true;
}

