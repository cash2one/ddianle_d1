#include "GameMsg_S2C_Couple.h"
#include "../../socket/ParamPool.h"
#include "../../datastructure/Macro_Define.h"
#include "CoupleMacroDef.h"
#include "CoupleMsgDef.h"
#include "../../datastructure/DataStruct_Couple.h"
#include "../Pet/PetService.h"
#include <algorithm>


///////////////////////////////////////////////////////
GameMsg_S2C_SendCoupleRequestSuc::GameMsg_S2C_SendCoupleRequestSuc()
:GameMsg_Base(MSG_S2C_SendCoupleRequestSuc)
{

}

GameMsg_S2C_SendCoupleRequestSuc::~GameMsg_S2C_SendCoupleRequestSuc()
{

}

bool GameMsg_S2C_SendCoupleRequestSuc::doEncode(CParamPool &IOBuff)
{
    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_SendCoupleRequestFail::GameMsg_S2C_SendCoupleRequestFail()
:GameMsg_Base(MSG_S2C_SendCoupleRequestFail)
, m_nError(0)
, m_strError("")
{
}

GameMsg_S2C_SendCoupleRequestFail::~GameMsg_S2C_SendCoupleRequestFail()
{

}

bool GameMsg_S2C_SendCoupleRequestFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nError);
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_StartWedding::GameMsg_S2C_StartWedding()
:GameMsg_Base(MSG_S2C_StartWedding)
, m_nBlessingValue(0)
, m_nGroomID(0)
, m_BrideID(0)
, m_nMarriedTime(0)
{

}

GameMsg_S2C_StartWedding::~GameMsg_S2C_StartWedding()
{

}

bool GameMsg_S2C_StartWedding::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nBlessingValue);
    IOBuff.AddUInt(m_nGroomID);
    IOBuff.AddUInt(m_BrideID);
    IOBuff.AddUInt(m_nMarriedTime);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_WeddingRoomStartingNofication::GameMsg_S2C_WeddingRoomStartingNofication()
:GameMsg_Base(MSG_S2C_WeddingRoomBeginNotification)
, m_nLine(-1)
, m_nRoomID(0)
, m_nStartTime(0)
{

}

GameMsg_S2C_WeddingRoomStartingNofication::~GameMsg_S2C_WeddingRoomStartingNofication()
{

}

bool GameMsg_S2C_WeddingRoomStartingNofication::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nLine);
    IOBuff.AddStringW(m_strLineName);
    IOBuff.AddUInt(m_nRoomID);
    IOBuff.AddUInt(m_nStartTime);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_ReceiveCoupleRequest::GameMsg_S2C_ReceiveCoupleRequest()
:GameMsg_Base(MSG_S2C_ReceiveCoupleRequest)
, m_nSenderID(0)
, m_strSenderName("")
{

}

GameMsg_S2C_ReceiveCoupleRequest::~GameMsg_S2C_ReceiveCoupleRequest()
{

}

bool GameMsg_S2C_ReceiveCoupleRequest::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUInt(m_nSenderID);
    IOBuff.AddStringW(m_strSenderName.c_str());

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_AcceptCoupleRequestSuc::GameMsg_S2C_AcceptCoupleRequestSuc()
:GameMsg_Base(MSG_S2C_AcceptCoupleRequestSuc)
, m_nPairID(0)
, m_nCoupleID(0)
, m_strCoupleName("")
, m_nCoupleDate(0)
, m_nSmallRoleID(0)
{
}

GameMsg_S2C_AcceptCoupleRequestSuc::~GameMsg_S2C_AcceptCoupleRequestSuc()
{

}

bool GameMsg_S2C_AcceptCoupleRequestSuc::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUInt(m_nPairID);
    IOBuff.AddUInt(m_nCoupleID);
    IOBuff.AddStringW(m_strCoupleName.c_str());
    IOBuff.AddUInt(m_nCoupleDate);
    IOBuff.AddUInt(m_nSmallRoleID);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_AcceptCoupleRequestFail::GameMsg_S2C_AcceptCoupleRequestFail()
:GameMsg_Base(MSG_S2C_AcceptCoupleRequestFail)
, m_strError("")
{

}

GameMsg_S2C_AcceptCoupleRequestFail::~GameMsg_S2C_AcceptCoupleRequestFail()
{

}

bool GameMsg_S2C_AcceptCoupleRequestFail::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_CoupleRequestReplay::GameMsg_S2C_CoupleRequestReplay()
: GameMsg_Base(MSG_S2C_CoupleRequestReply)
{
}

GameMsg_S2C_CoupleRequestReplay::~GameMsg_S2C_CoupleRequestReplay()
{
}

bool GameMsg_S2C_CoupleRequestReplay::doEncode(CParamPool &IOBuff)
{
    m_requestReplay.doEncode(IOBuff);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_CoupleSeparateRequestSuc::GameMsg_S2C_CoupleSeparateRequestSuc()
:GameMsg_Base(MSG_S2C_CoupleSeparateRequestSuc)
{

}

GameMsg_S2C_CoupleSeparateRequestSuc::~GameMsg_S2C_CoupleSeparateRequestSuc()
{

}

bool GameMsg_S2C_CoupleSeparateRequestSuc::doEncode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_CoupleSeparateRequestFail::GameMsg_S2C_CoupleSeparateRequestFail()
:GameMsg_Base(MSG_S2C_CoupleSeparateRequestFail)
, m_strError("")
{

}

GameMsg_S2C_CoupleSeparateRequestFail::~GameMsg_S2C_CoupleSeparateRequestFail()
{

}

bool GameMsg_S2C_CoupleSeparateRequestFail::doEncode(CParamPool &IOBuff)
{
    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_NotifydHasBeenSeparated::GameMsg_S2C_NotifydHasBeenSeparated()
:GameMsg_Base(MSG_S2C_NotifyHasBeenSeparated)
, m_nSeparatedBy(0)
, m_strSeparatedBy("")
{

}

GameMsg_S2C_NotifydHasBeenSeparated::~GameMsg_S2C_NotifydHasBeenSeparated()
{

}

bool GameMsg_S2C_NotifydHasBeenSeparated::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nSeparatedBy);
    IOBuff.AddStringW(m_strSeparatedBy.c_str());

    return true;
}

///////////////////////////////////////////////////////
//GameMsg_S2C_RequestMarrySuc::GameMsg_S2C_RequestMarrySuc()
//:GameMsg_Base(MSG_S2C_RequestGotMarriedSuc)
//{
//
//}
//
//GameMsg_S2C_RequestMarrySuc::~GameMsg_S2C_RequestMarrySuc()
//{
//
//}
//
//bool GameMsg_S2C_RequestMarrySuc::doEncode(CParamPool &IOBuff)
//{
//	return true;
//}
///////////////////////////////////////////////////////
GameMsg_S2C_RequestMarryFail::GameMsg_S2C_RequestMarryFail()
:GameMsg_Base(MSG_S2C_RequestGotMarriedFail)
, m_nError(0)
, m_strError("")
{

}

GameMsg_S2C_RequestMarryFail::~GameMsg_S2C_RequestMarryFail()
{

}

bool GameMsg_S2C_RequestMarryFail::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUChar(m_nError);
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_AutoStartWeddingRoomFail::GameMsg_S2C_AutoStartWeddingRoomFail()
:GameMsg_Base(MSG_S2C_AutoStartWeddingRoomFail)
, m_nError(0)
, m_strError("")
{

}

GameMsg_S2C_AutoStartWeddingRoomFail::~GameMsg_S2C_AutoStartWeddingRoomFail()
{

}

bool GameMsg_S2C_AutoStartWeddingRoomFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nError);
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}

unsigned char m_nError;
std::string m_strError;

///////////////////////////////////////////////////////
GameMsg_S2C_RequestDivorceSuc::GameMsg_S2C_RequestDivorceSuc()
:GameMsg_Base(MSG_S2C_RequestDivorceSuc)
, m_nSmallRoleID(0)
{

}

GameMsg_S2C_RequestDivorceSuc::~GameMsg_S2C_RequestDivorceSuc()
{

}

bool GameMsg_S2C_RequestDivorceSuc::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nSmallRoleID);
    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_RequestDivorceFail::GameMsg_S2C_RequestDivorceFail()
:GameMsg_Base(MSG_S2C_RequestDivorceFail)
, m_nError(0)
{

}

GameMsg_S2C_RequestDivorceFail::~GameMsg_S2C_RequestDivorceFail()
{

}

bool GameMsg_S2C_RequestDivorceFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nError);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_NotifyHasBeenDivorced::GameMsg_S2C_NotifyHasBeenDivorced()
:GameMsg_Base(MSG_S2C_NotifyHasBeenDivorced)
, m_nDivorcedBy(0)
, m_strDivorcedBy("")
{

}

GameMsg_S2C_NotifyHasBeenDivorced::~GameMsg_S2C_NotifyHasBeenDivorced()
{

}

bool GameMsg_S2C_NotifyHasBeenDivorced::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUInt(m_nDivorcedBy);
    IOBuff.AddStringW(m_strDivorcedBy.c_str());

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_GetWeddingRoomListRes::GameMsg_S2C_GetWeddingRoomListRes()
:GameMsg_Base(MSG_S2C_GetWeddingRoomRes)
, m_nCurPage(0)
, m_nTotalCount(0)
{

}

GameMsg_S2C_GetWeddingRoomListRes::~GameMsg_S2C_GetWeddingRoomListRes()
{

}

bool GameMsg_S2C_GetWeddingRoomListRes::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUShort(m_nCurPage);
    IOBuff.AddUShort(m_nTotalCount);
    IOBuff.AddUShort((unsigned short)m_listRoomInfo.size());
    for (std::list<CWeddingRoomBriefInfo>::iterator itr = m_listRoomInfo.begin(); itr != m_listRoomInfo.end(); ++itr) {
        itr->doEncode(IOBuff);
    }

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_CreateWeddingRoomFail::GameMsg_S2C_CreateWeddingRoomFail()
:GameMsg_Base(MSG_S2C_CreateWeddingRoomFail)
, m_nError(0)
, m_strError("")
, m_nNeedCount(0)
{

}

GameMsg_S2C_CreateWeddingRoomFail::~GameMsg_S2C_CreateWeddingRoomFail()
{

}

bool GameMsg_S2C_CreateWeddingRoomFail::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUChar(m_nError);
    IOBuff.AddStringW(m_strError.c_str());
    IOBuff.AddInt(m_nNeedCount);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_EnterWeddingRoomSuc::GameMsg_S2C_EnterWeddingRoomSuc()
:GameMsg_Base(MSG_S2C_EnterWeddingRoomSuc)
{
}

GameMsg_S2C_EnterWeddingRoomSuc::~GameMsg_S2C_EnterWeddingRoomSuc()
{

}

bool GameMsg_S2C_EnterWeddingRoomSuc::doEncode(CParamPool & IOBuff)
{
    m_roomWholeInfo.doEncode(IOBuff);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_EnterWeddingPlayer::GameMsg_S2C_EnterWeddingPlayer()
:GameMsg_Base(MSG_S2C_EnterWeddingPlayer)
{

}

GameMsg_S2C_EnterWeddingPlayer::~GameMsg_S2C_EnterWeddingPlayer()
{

}

bool GameMsg_S2C_EnterWeddingPlayer::doEncode(CParamPool & IOBuff)
{
    m_roomPlayer.doEncode(IOBuff);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_TryEnterWeddingRoomFail::GameMsg_S2C_TryEnterWeddingRoomFail()
:GameMsg_Base(MSG_S2C_TryEnterWeddingRoomFail)
, m_nRoomID(0)
, m_nError(0)
, m_strError("")
{

}

GameMsg_S2C_TryEnterWeddingRoomFail::~GameMsg_S2C_TryEnterWeddingRoomFail()
{

}

bool GameMsg_S2C_TryEnterWeddingRoomFail::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUInt(m_nRoomID);
    IOBuff.AddUChar(m_nError);
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_EnterWeddingRoomFail::GameMsg_S2C_EnterWeddingRoomFail()
:GameMsg_Base(MSG_S2C_EnterWeddingRoomFail)
, m_nError(0)
, m_strError("")
{

}

GameMsg_S2C_EnterWeddingRoomFail::~GameMsg_S2C_EnterWeddingRoomFail()
{

}

bool GameMsg_S2C_EnterWeddingRoomFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nError);
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_EnterMyWeddingRoomFail::GameMsg_S2C_EnterMyWeddingRoomFail()
:GameMsg_Base(MSG_S2C_EnterMyWeddingRoomFail)
, m_nError(0)
, m_strError("")
{

}

GameMsg_S2C_EnterMyWeddingRoomFail::~GameMsg_S2C_EnterMyWeddingRoomFail()
{

}

bool GameMsg_S2C_EnterMyWeddingRoomFail::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUChar(m_nError);
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_ExitWeddingRoomSuc::GameMsg_S2C_ExitWeddingRoomSuc()
:GameMsg_Base(MSG_S2C_ExitWeddingRoomSuc)
, m_nQuitTo(0)
{

}

GameMsg_S2C_ExitWeddingRoomSuc::~GameMsg_S2C_ExitWeddingRoomSuc()
{

}

bool GameMsg_S2C_ExitWeddingRoomSuc::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddChar(m_nQuitTo);
    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_ExitWeddingRoomFail::GameMsg_S2C_ExitWeddingRoomFail()
:GameMsg_Base(MSG_S2C_ExitWeddingRoomFail)
, m_nError(0)
, m_strError("")
{

}

GameMsg_S2C_ExitWeddingRoomFail::~GameMsg_S2C_ExitWeddingRoomFail()
{

}

bool GameMsg_S2C_ExitWeddingRoomFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nError);
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_PlayerExitedRoom::GameMsg_S2C_PlayerExitedRoom()
:GameMsg_Base(MSG_S2C_PlayerExitedRoom)
, m_nExitID(0)
{

}

GameMsg_S2C_PlayerExitedRoom::~GameMsg_S2C_PlayerExitedRoom()
{

}

bool GameMsg_S2C_PlayerExitedRoom::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nExitID);

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_KickWeddingPlayerSuc::GameMsg_S2C_KickWeddingPlayerSuc()
:GameMsg_Base(MSG_S2C_KickWeddingPlayerSuc)
, m_nTargetID(0)
{

}

GameMsg_S2C_KickWeddingPlayerSuc::~GameMsg_S2C_KickWeddingPlayerSuc()
{

}

bool GameMsg_S2C_KickWeddingPlayerSuc::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nTargetID);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_KickWeddingPlayerFail::GameMsg_S2C_KickWeddingPlayerFail()
:GameMsg_Base(MSG_S2C_KickWeddingPlayerFail)
, m_nError(0)
, m_strError("")
{

}

GameMsg_S2C_KickWeddingPlayerFail::~GameMsg_S2C_KickWeddingPlayerFail()
{

}

bool GameMsg_S2C_KickWeddingPlayerFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nError);
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_SetWeddingPlayerTypeSuc::GameMsg_S2C_SetWeddingPlayerTypeSuc()
:GameMsg_Base(MSG_S2C_SetWeddingPlayerTypeSuc)
, m_nTargetID(0)
, m_RoomRoleType(0)
{

}

GameMsg_S2C_SetWeddingPlayerTypeSuc::~GameMsg_S2C_SetWeddingPlayerTypeSuc()
{

}

bool GameMsg_S2C_SetWeddingPlayerTypeSuc::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nTargetID);
    IOBuff.AddUChar(m_RoomRoleType);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_SetWeddingPlayerTypeFail::GameMsg_S2C_SetWeddingPlayerTypeFail()
:GameMsg_Base(MSG_S2C_SetWeddingPlayerTypeFail)
, m_strError("")
{

}

GameMsg_S2C_SetWeddingPlayerTypeFail::~GameMsg_S2C_SetWeddingPlayerTypeFail()
{

}

bool GameMsg_S2C_SetWeddingPlayerTypeFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_NotifyChangeWeddingPlayerType::GameMsg_S2C_NotifyChangeWeddingPlayerType()
:GameMsg_Base(MSG_S2C_NotifyChangeWeddingPlayerType)
, m_nTargetID(0)
, m_nRoomRoleType(0)
{

}

GameMsg_S2C_NotifyChangeWeddingPlayerType::~GameMsg_S2C_NotifyChangeWeddingPlayerType()
{

}

bool GameMsg_S2C_NotifyChangeWeddingPlayerType::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nTargetID);
    IOBuff.AddUChar(m_nRoomRoleType);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_ChangeWeddingRoomPwdSuc::GameMsg_S2C_ChangeWeddingRoomPwdSuc()
:GameMsg_Base(MSG_S2C_ChangeWeddingRoomPwdSuc)
, m_strNewPwd("")
{

}

GameMsg_S2C_ChangeWeddingRoomPwdSuc::~GameMsg_S2C_ChangeWeddingRoomPwdSuc()
{

}

bool GameMsg_S2C_ChangeWeddingRoomPwdSuc::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strNewPwd.c_str());

    return true;
}
///////////////////////////////////////////////////////
GameMsg_S2C_ChangeWeddingRoomPwdFail::GameMsg_S2C_ChangeWeddingRoomPwdFail()
:GameMsg_Base(MSG_S2C_ChangeWeddingRoomPwdFail)
, m_strError("")
{

}

GameMsg_S2C_ChangeWeddingRoomPwdFail::~GameMsg_S2C_ChangeWeddingRoomPwdFail()
{

}

bool GameMsg_S2C_ChangeWeddingRoomPwdFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_UpdateCoupleIntimacy::GameMsg_S2C_UpdateCoupleIntimacy()
:GameMsg_Base(MSG_S2C_UpdateCoupleIntimacy)
, m_nIntimacy(0)
{

}

GameMsg_S2C_UpdateCoupleIntimacy::~GameMsg_S2C_UpdateCoupleIntimacy()
{

}

bool GameMsg_S2C_UpdateCoupleIntimacy::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nIntimacy);

    return true;
}

////////////////////////////////////////////////////
GameMsg_S2C_WeddingRoomStateChanged::GameMsg_S2C_WeddingRoomStateChanged()
:GameMsg_Base(MSG_S2C_WeddingRoomStateChanged)
, m_bValid(false)
{

}

GameMsg_S2C_WeddingRoomStateChanged::~GameMsg_S2C_WeddingRoomStateChanged()
{

}

bool GameMsg_S2C_WeddingRoomStateChanged::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bValid);

    return true;
}

////////////////////////////////////////////////////
GameMsg_S2C_UpdateCoupleName::GameMsg_S2C_UpdateCoupleName()
:GameMsg_Base(MSG_S2C_UpdateCoupleName)
, m_strNewName("")
{

}

GameMsg_S2C_UpdateCoupleName::~GameMsg_S2C_UpdateCoupleName()
{

}

bool GameMsg_S2C_UpdateCoupleName::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strNewName.c_str());

    return true;
}

////////////////////////////////////////////////////
GameMsg_S2C_BlessingNotify::GameMsg_S2C_BlessingNotify()
:GameMsg_Base(MSG_S2C_BlessingNotify),
m_strManName(""),
m_strWomanName(""),
m_nPointIndex(0)
{

}

bool GameMsg_S2C_BlessingNotify::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strManName.c_str());
    IOBuff.AddStringW(m_strWomanName.c_str());
    IOBuff.AddInt(m_nPointIndex);

    return true;
}

////////////////////////////////////////////////////
GameMsg_S2C_GetBlessingRankListResult::GameMsg_S2C_GetBlessingRankListResult()
:GameMsg_Base(MSG_S2C_GetBlessingRankListResult)
, m_nCurrentPage(0)
, m_nPageCount(0)
, m_nRankTotalCount(0)
{

}

GameMsg_S2C_GetBlessingRankListResult::~GameMsg_S2C_GetBlessingRankListResult()
{

}


bool GameMsg_S2C_GetBlessingRankListResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nCurrentPage);
    IOBuff.AddUShort(m_nRankTotalCount);

    IOBuff.AddUShort((unsigned short)m_BlessingRankVec.size());
    for (size_t i = 0; i < m_BlessingRankVec.size(); ++i)
    {
        IOBuff.AddUInt(m_nCurrentPage*m_nPageCount + i + 1);
        m_BlessingRankVec[i].doEncode(IOBuff);
    }

    return true;
}

////////////////////////////////////////////////////
GameMsg_S2C_GetCoupleInfoResult::GameMsg_S2C_GetCoupleInfoResult()
:GameMsg_Base(MSG_S2C_GetCoupleInfoResult)
{

}


bool GameMsg_S2C_GetCoupleInfoResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRank);
    m_CoupleInfo.doEncode(IOBuff);
    return true;
}


////////////////////////////////////////////////////////////
GameMsg_S2C_GetWeddingRecordListResult::GameMsg_S2C_GetWeddingRecordListResult()
:GameMsg_Base(MSG_S2C_GetWddingRecordListResult)
, m_nRoleID(0)
{

}


bool GameMsg_S2C_GetWeddingRecordListResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_WeddingRecord.size());
    for (size_t i = 0; i < m_WeddingRecord.size(); ++i)
    {
        IOBuff.AddUInt(m_WeddingRecord[i].m_nPairID);
        IOBuff.AddBool(m_WeddingRecord[i].m_nIsRunning > 0 ? true : false);
        IOBuff.AddUInt(m_WeddingRecord[i].m_nMaleID);
        IOBuff.AddStringW(m_WeddingRecord[i].m_strMaleName.c_str());
        IOBuff.AddUInt(m_WeddingRecord[i].m_nFemaleID);
        IOBuff.AddStringW(m_WeddingRecord[i].m_strFemaleName.c_str());
        IOBuff.AddBool(IsBlessed(m_WeddingRecord[i].m_BlessingRoleIDList, m_nRoleID));
    }

    return true;
}

bool GameMsg_S2C_GetWeddingRecordListResult::IsBlessed(const std::vector<unsigned int> & BlessingRoleIDList, unsigned int nRoleID)
{
    std::vector<unsigned int>::const_iterator it = std::find(BlessingRoleIDList.begin(), BlessingRoleIDList.end(), nRoleID);
    if (it != BlessingRoleIDList.end())
    {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_GetBlessingValueResult::GameMsg_S2C_GetBlessingValueResult()
:GameMsg_Base(MSG_S2C_GetBlessingValueReslt)
, m_nBlessingValue(0)
, m_nLine(0)
{

}


bool GameMsg_S2C_GetBlessingValueResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nBlessingValue);
    IOBuff.AddUShort(m_nLine);

    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_GetPersonInfoResult::GameMsg_S2C_GetPersonInfoResult()
: GameMsg_Base(MSG_S2C_GetPersonInfoResult)
, m_pRoleItem(NULL)
, m_pRoleGene(NULL)
, m_pRolePet(NULL)
{

}


bool GameMsg_S2C_GetPersonInfoResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_OffLineData.m_nRoleID);
    IOBuff.AddStringW(m_OffLineData.m_strRoleName.c_str());
    IOBuff.AddUChar(m_OffLineData.m_nSex);
    IOBuff.AddUChar(m_OffLineData.m_nSkin);

    if (m_pRoleItem != NULL)
    {
        m_pRoleItem->PackItemForOther(IOBuff);
    }
    else
    {
        int nOldPos1 = IOBuff.GetCurPosition();
        IOBuff.AddUShort(0);

        int nOldPos2 = IOBuff.GetCurPosition();
        IOBuff.AddUShort(0);

        short nCount = 0;
        for (size_t i = 0; i < m_OffLineData.m_vecRoleItem.size(); ++i)
        {
            IOBuff.AddUInt(m_OffLineData.m_vecRoleItem[i].m_nTypeID);
            IOBuff.AddUShort(m_OffLineData.m_vecRoleItem[i].m_nColumn);
            IOBuff.AddUShort(m_OffLineData.m_vecRoleItem[i].m_nIndex);

            char szItemBuff[256] = { 0 };
            memset(szItemBuff, 0, sizeof(szItemBuff));
            CParamPool itemBuff(szItemBuff, sizeof(szItemBuff));
            itemBuff.AddUShort(0);
            itemBuff.AddUShort(1);
            itemBuff.AddInt(-1);
            itemBuff.AddUChar(1);

            // 衣服特效
            std::map<itemid_t, unsigned int>::iterator it = m_OffLineData.m_mapClothEffect.find(m_OffLineData.m_vecRoleItem[i].m_nItemID);
            if (it != m_OffLineData.m_mapClothEffect.end())
            {
                itemBuff.AddUInt(it->second);
            }
            else
            {
                itemBuff.AddUInt(0);
            }

            unsigned int nItemSize = itemBuff.GetValidDataSize();
            ASSERT(nItemSize == itemBuff.GetCurPosition());

            IOBuff.AddUChar((unsigned char)nItemSize);
            IOBuff.AddMemory(itemBuff.GetData(), (unsigned short)nItemSize);

            nCount++;
        }

        int nFinalPos = IOBuff.GetCurPosition();
        IOBuff.SetCurPosition(nOldPos2);
        IOBuff.ReplaceShort(nCount);
        IOBuff.SetCurPosition(nFinalPos);

        // 七色彩虹信息
        IOBuff.AddUShort((unsigned short)m_OffLineData.m_mapClothEffectSevenColor.size());
        for (std::map<unsigned short, std::set<ESevenColorType> >::iterator it = m_OffLineData.m_mapClothEffectSevenColor.begin();
            it != m_OffLineData.m_mapClothEffectSevenColor.end(); ++it)
        {
            IOBuff.AddUShort(it->first);
            IOBuff.AddUShort((unsigned short)it->second.size());
            for (std::set<ESevenColorType>::iterator itColor = it->second.begin();
                itColor != it->second.end(); ++itColor)
            {
                IOBuff.AddUChar((unsigned char)(*itColor));
            }
        }

        if (IOBuff.GetCurPosition() - nOldPos1 - sizeof(unsigned short) != 0)
        {
            nFinalPos = IOBuff.GetCurPosition();
            IOBuff.SetCurPosition(nOldPos1);
            IOBuff.ReplaceShort((short)(nFinalPos - nOldPos1 - sizeof(unsigned short)));
            IOBuff.SetCurPosition(nFinalPos);
        }
    }

    if (m_pRoleGene != NULL)
    {
        m_pRoleGene->PackGeneForOther(IOBuff);
    }
    else
    {
        IOBuff.AddUShort(0);
    }

    if (m_pRolePet != NULL && m_pRolePet->GetCarriedPet() != NULL)
    {
        Pet *pPet = m_pRolePet->GetCarriedPet();
        PlayerPet playerPet;
        PlayerPetSkill playerPetSkill;

        IOBuff.AddBool(true);
        PetService::Instance().EncodePetBrief(*pPet, playerPet);
        PetService::Instance().EncodePetSkill(*pPet, playerPetSkill);
        playerPet.doEncode(IOBuff);
        playerPetSkill.doEncode(IOBuff);
    }
    else
    {
        IOBuff.AddBool(m_OffLineData.m_bHasCarriedPet);

        if (m_OffLineData.m_bHasCarriedPet)
        {
            m_OffLineData.m_playerPet.doEncode(IOBuff);
            m_OffLineData.m_playerPetSkill.doEncode(IOBuff);
        }
    }

    return true;
}


////////////////////////////////////////////////////////////
GameMsg_S2C_BlessingError::GameMsg_S2C_BlessingError()
:GameMsg_Base(MSG_S2C_BlessingError)
{

}


bool GameMsg_S2C_BlessingError::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strError.c_str());
    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_BlessingCoupleSuc::GameMsg_S2C_BlessingCoupleSuc()
:GameMsg_Base(MSG_S2C_BlessingPairSuc)
{

}


bool GameMsg_S2C_BlessingCoupleSuc::doEncode(CParamPool &IOBuff)
{
    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_RedEnvelopeSuc::GameMsg_S2C_RedEnvelopeSuc()
:GameMsg_Base(MSG_S2C_MoneyBlessingPairSuc)
{

}


bool GameMsg_S2C_RedEnvelopeSuc::doEncode(CParamPool &IOBuff)
{
    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_GetCoupleRedEnvelopeGiftInfoResult::GameMsg_S2C_GetCoupleRedEnvelopeGiftInfoResult()
:GameMsg_Base(MSG_S2C_GetCoupleRedEnvelopeGiftResult)
, m_nRedEnvelopeCount(0)
{

}


bool GameMsg_S2C_GetCoupleRedEnvelopeGiftInfoResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRedEnvelopeCount);
    IOBuff.AddUShort((unsigned short)m_RedEnvelopeInfo.size());

    std::map<unsigned int, itemtype_t>::iterator it = m_RedEnvelopeInfo.begin();
    for (; it != m_RedEnvelopeInfo.end(); ++it)
    {
        IOBuff.AddUInt(it->first);
        IOBuff.AddUInt(it->second);
    }
    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_CancelDivorceSuc::GameMsg_S2C_CancelDivorceSuc()
:GameMsg_Base(MSG_S2C_RequestCancelDivorceSuc)

{

}


bool GameMsg_S2C_CancelDivorceSuc::doEncode(CParamPool &IOBuff)
{
    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_CancelDivorceFail::GameMsg_S2C_CancelDivorceFail()
:GameMsg_Base(MSG_S2C_RequestCancelDivorceFail)
, m_cErrorType(0)
{

}


bool GameMsg_S2C_CancelDivorceFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddChar(m_cErrorType);
    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_MandatoryDivorceSuc::GameMsg_S2C_MandatoryDivorceSuc()
:GameMsg_Base(MSG_S2C_RequestCompulsoryDivorceSuc)

{

}


bool GameMsg_S2C_MandatoryDivorceSuc::doEncode(CParamPool &IOBuff)
{
    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_MandatoryDivorceFail::GameMsg_S2C_MandatoryDivorceFail()
:GameMsg_Base(MSG_S2C_RequestCompulsoryDivorceFail)
, m_cErrorType(0)
{

}


bool GameMsg_S2C_MandatoryDivorceFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddChar(m_cErrorType);
    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_AgreeDivorceSuc::GameMsg_S2C_AgreeDivorceSuc()
:GameMsg_Base(MSG_S2C_AgreeDivorceSuc)

{

}


bool GameMsg_S2C_AgreeDivorceSuc::doEncode(CParamPool &IOBuff)
{
    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_AgreeDivorceFail::GameMsg_S2C_AgreeDivorceFail()
:GameMsg_Base(MSG_S2C_AgreeDivorceFail)
, m_cErrorType(0)
{

}


bool GameMsg_S2C_AgreeDivorceFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddChar(m_cErrorType);
    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_GiftDivorceSuc::GameMsg_S2C_GiftDivorceSuc()
:GameMsg_Base(MSG_S2C_DivorceSendGiftSuc)

{

}


bool GameMsg_S2C_GiftDivorceSuc::doEncode(CParamPool &IOBuff)
{
    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_GiftDivorceFail::GameMsg_S2C_GiftDivorceFail()
:GameMsg_Base(MSG_S2C_DivorceSendGiftFail)
, m_cErrorType(0)
{

}


bool GameMsg_S2C_GiftDivorceFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddChar(m_cErrorType);
    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_RefreshAction::GameMsg_S2C_RefreshAction()
:GameMsg_Base(MSG_S2C_ReciveDivorceAction)
{
}


bool GameMsg_S2C_RefreshAction::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddChar((char)m_DivorceAgreementInfo.m_eState);
    IOBuff.AddBool(m_DivorceAgreementInfo.m_bMaleIsAgree);
    IOBuff.AddBool(m_DivorceAgreementInfo.m_bFemaleIsAgree);

    IOBuff.AddUInt(m_action.m_nTime);
    IOBuff.AddUInt(m_action.m_nProposerID);
    char cProposerSex = (char)(m_action.m_nProposerID == m_DivorceAgreementInfo.m_nMaleRoleID ? ESexType_Male : ESexType_Female);
    IOBuff.AddChar(cProposerSex);
    if (cProposerSex == ESexType_Male)
    {
        IOBuff.AddStringW(m_DivorceAgreementInfo.m_strMaleRoleName);
        IOBuff.AddUInt(m_DivorceAgreementInfo.m_nFemaleRoleID);
        IOBuff.AddChar(ESexType_Female);
        IOBuff.AddStringW(m_DivorceAgreementInfo.m_strFemaleRoleName);
    }
    else
    {
        IOBuff.AddStringW(m_DivorceAgreementInfo.m_strFemaleRoleName);
        IOBuff.AddUInt(m_DivorceAgreementInfo.m_nMaleRoleID);
        IOBuff.AddChar(ESexType_Male);
        IOBuff.AddStringW(m_DivorceAgreementInfo.m_strMaleRoleName);
    }
    IOBuff.AddChar((char)m_action.m_eType);
    IOBuff.AddStringW(m_action.m_strChatContent);
    m_action.m_SendItem.doEncode(IOBuff);

    return true;
}


////////////////////////////////////////////////////////////
GameMsg_S2C_DivorceWishItem::GameMsg_S2C_DivorceWishItem()
:GameMsg_Base(MSG_S2C_RequestCoupleWishRes)
{
}


bool GameMsg_S2C_DivorceWishItem::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_WishItem.size());
    std::vector<itemtype_t>::iterator it = m_WishItem.begin();
    for (; it != m_WishItem.end(); ++it)
    {
        IOBuff.AddUInt(*it);
    }
    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_DivorceAnimationSuc::GameMsg_S2C_DivorceAnimationSuc()
:GameMsg_Base(MSG_S2C_EnterDivorceAnimationSuc)
{
}


bool GameMsg_S2C_DivorceAnimationSuc::doEncode(CParamPool &IOBuff)
{
    m_MaleMsg.doEncode(IOBuff);
    m_FemaleMsg.doEncode(IOBuff);

    IOBuff.AddUShort((unsigned short)m_Actions.size());
    std::list<CDivorceAction>::iterator it = m_Actions.begin();
    for (; it != m_Actions.end(); ++it)
    {
        IOBuff.AddUInt(it->m_nTime);
        IOBuff.AddUInt(it->m_nProposerID);
        if (it->m_nProposerID == m_DivorceAgreementInfo.m_nMaleRoleID)
        {
            if (m_MaleMsg.nRoleID == m_DivorceAgreementInfo.m_nMaleRoleID)
            {
                IOBuff.AddChar(m_MaleMsg.cRoleSex);
                IOBuff.AddStringW(m_DivorceAgreementInfo.m_strMaleRoleName);
                IOBuff.AddUInt(m_DivorceAgreementInfo.m_nFemaleRoleID);
                IOBuff.AddUChar(m_FemaleMsg.cRoleSex);
                IOBuff.AddStringW(m_DivorceAgreementInfo.m_strFemaleRoleName);
            }
            else
            {
                IOBuff.AddChar(m_FemaleMsg.cRoleSex);
                IOBuff.AddStringW(m_DivorceAgreementInfo.m_strMaleRoleName);
                IOBuff.AddUInt(m_DivorceAgreementInfo.m_nFemaleRoleID);
                IOBuff.AddUChar(m_MaleMsg.cRoleSex);
                IOBuff.AddStringW(m_DivorceAgreementInfo.m_strFemaleRoleName);
            }
        }
        else
        {
            if (m_MaleMsg.nRoleID == m_DivorceAgreementInfo.m_nMaleRoleID)
            {
                IOBuff.AddChar(m_MaleMsg.cRoleSex);
                IOBuff.AddStringW(m_DivorceAgreementInfo.m_strFemaleRoleName);
                IOBuff.AddUInt(m_DivorceAgreementInfo.m_nMaleRoleID);
                IOBuff.AddUChar(m_FemaleMsg.cRoleSex);
                IOBuff.AddStringW(m_DivorceAgreementInfo.m_strMaleRoleName);
            }
            else
            {
                IOBuff.AddChar(m_FemaleMsg.cRoleSex);
                IOBuff.AddStringW(m_DivorceAgreementInfo.m_strFemaleRoleName);
                IOBuff.AddUInt(m_DivorceAgreementInfo.m_nFemaleRoleID);
                IOBuff.AddUChar(m_MaleMsg.cRoleSex);
                IOBuff.AddStringW(m_DivorceAgreementInfo.m_strMaleRoleName);
            }
        }

        IOBuff.AddChar((char)it->m_eType);
        IOBuff.AddStringW(it->m_strChatContent);
        it->m_SendItem.doEncode(IOBuff);
    }

    return true;
}

////////////////////////////////////////////////////////////
GameMsg_S2C_DivorceAnimationFail::GameMsg_S2C_DivorceAnimationFail()
:GameMsg_Base(MSG_S2C_EnterDivorceAnimationFail)
, m_cErrorType(0)
{

}


bool GameMsg_S2C_DivorceAnimationFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddChar(m_cErrorType);
    return true;
}


////////////////////////////////////////////////////////////
GameMsg_S2C_DivorceHaveNewCouple::GameMsg_S2C_DivorceHaveNewCouple()
:GameMsg_Base(MSG_S2C_DivorceHaveNewCouple)
{

}


bool GameMsg_S2C_DivorceHaveNewCouple::doEncode(CParamPool &IOBuff)
{
    return true;
}

