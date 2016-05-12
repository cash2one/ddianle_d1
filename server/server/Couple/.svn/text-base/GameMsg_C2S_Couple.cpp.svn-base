#include "GameMsg_C2S_Couple.h"
#include "../../socket/ParamPool.h"
#include "CoupleMsgDef.h"
#include "CoupleMacroDef.h"

///////////////////////////////////////////////////////
GameMsg_C2S_SendCoupleRequest::GameMsg_C2S_SendCoupleRequest(  )
:GameMsg_Base(MSG_C2S_SendCoupleRequest)
,m_nTargetID(0)
{

}

GameMsg_C2S_SendCoupleRequest::~GameMsg_C2S_SendCoupleRequest(  )
{

}

bool GameMsg_C2S_SendCoupleRequest::doDecode( CParamPool &IOBuff )
{
	m_nTargetID = IOBuff.GetUInt();

	return true;
}

GameMsg_C2S_AcceptCoupleRequest::GameMsg_C2S_AcceptCoupleRequest()
:GameMsg_Base(MSG_C2S_AcceptCoupleRequest)
,m_nTargetID(0)
{

}

GameMsg_C2S_AcceptCoupleRequest::~GameMsg_C2S_AcceptCoupleRequest()
{

}

bool GameMsg_C2S_AcceptCoupleRequest::doDecode( CParamPool &IOBuff )
{
	m_nTargetID = IOBuff.GetUInt();	

	return true;
}

GameMsg_C2S_RefuseCoupleRequest::GameMsg_C2S_RefuseCoupleRequest()
:GameMsg_Base(MSG_C2S_RefuseCoupleRequest)
,m_nTargetID(0)
,m_strReason("")
{

}

GameMsg_C2S_RefuseCoupleRequest::~GameMsg_C2S_RefuseCoupleRequest()
{

}

bool GameMsg_C2S_RefuseCoupleRequest::doDecode( CParamPool &IOBuff )
{
	m_nTargetID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strReason);

	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_CoupleSeparateRequest::GameMsg_C2S_CoupleSeparateRequest()
:GameMsg_Base(MSG_C2S_CoupleSeparateRequest)
,m_nTargetID(0)
{

}

GameMsg_C2S_CoupleSeparateRequest::~GameMsg_C2S_CoupleSeparateRequest()
{

}

bool GameMsg_C2S_CoupleSeparateRequest::doDecode( CParamPool &IOBuff )
{
	m_nTargetID = IOBuff.GetUInt();

	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_RequestMarry::GameMsg_C2S_RequestMarry()
:GameMsg_Base(MSG_C2S_RequestMarry)
{

}

GameMsg_C2S_RequestMarry::~GameMsg_C2S_RequestMarry()
{

}

bool GameMsg_C2S_RequestMarry::doDecode(CParamPool & IOBuff)
{
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_RequestDivorce::GameMsg_C2S_RequestDivorce()
:GameMsg_Base(MSG_C2S_RequestDivorce)
,m_nDivorceID(0)
{

}

GameMsg_C2S_RequestDivorce::~GameMsg_C2S_RequestDivorce()
{

}

bool GameMsg_C2S_RequestDivorce::doDecode( CParamPool &IOBuff )
{
	m_nDivorceID = IOBuff.GetUInt();

	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_CreateWeddingRoom::GameMsg_C2S_CreateWeddingRoom()
:GameMsg_Base(MSG_C2S_CreateWeddingRoom)
,m_nWeddingType(EWeddingType_None)
,m_strRoomName("")
,m_strPassword("")
{

}

GameMsg_C2S_CreateWeddingRoom::~GameMsg_C2S_CreateWeddingRoom()
{

}

bool GameMsg_C2S_CreateWeddingRoom::doDecode( CParamPool &IOBuff )
{
	m_nWeddingType = IOBuff.GetUChar();
	IOBuff.GetStringW(m_strRoomName);
	IOBuff.GetStringW(m_strPassword);

	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_GetWeddingRoomList::GameMsg_C2S_GetWeddingRoomList()
:GameMsg_Base(MSG_C2S_GetWeddingRoomList)
,m_nCurPage(0)
,m_nPageShowNum(0)
{

}

GameMsg_C2S_GetWeddingRoomList::~GameMsg_C2S_GetWeddingRoomList()
{

}

bool GameMsg_C2S_GetWeddingRoomList::doDecode( CParamPool &IOBuff )
{
	m_nCurPage = IOBuff.GetUShort();
    m_nPageShowNum = IOBuff.GetUShort();
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_TryEnterWeddingRoom::GameMsg_C2S_TryEnterWeddingRoom()
:GameMsg_Base(MSG_C2S_TryEnterWeddingRoom)
,m_nRoomID(0)
{

}

GameMsg_C2S_TryEnterWeddingRoom::~GameMsg_C2S_TryEnterWeddingRoom()
{

}

bool GameMsg_C2S_TryEnterWeddingRoom::doDecode( CParamPool &IOBuff )
{
	m_nRoomID = IOBuff.GetUInt();

	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_EnterWeddingRoom::GameMsg_C2S_EnterWeddingRoom()
:GameMsg_Base(MSG_C2S_EnterWeddingRoom)
,m_nRoomID(0)
{

}

GameMsg_C2S_EnterWeddingRoom::~GameMsg_C2S_EnterWeddingRoom()
{

}

bool GameMsg_C2S_EnterWeddingRoom::doDecode( CParamPool & IOBuff )
{
	m_nRoomID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strRoomPassword);

	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_EnterMyWeddingRoom::GameMsg_C2S_EnterMyWeddingRoom()
:GameMsg_Base(MSG_C2S_RequestEnterMyWeddingRoom)
{

}

GameMsg_C2S_EnterMyWeddingRoom::~GameMsg_C2S_EnterMyWeddingRoom()
{
	
}

bool GameMsg_C2S_EnterMyWeddingRoom::doDecode( CParamPool &IOBuff )
{
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_ExitWeddingRoom::GameMsg_C2S_ExitWeddingRoom()
:GameMsg_Base(MSG_C2S_ExitWeddingRoom)
,m_nQuitTo(0)
{

}

GameMsg_C2S_ExitWeddingRoom::~GameMsg_C2S_ExitWeddingRoom()
{

}

bool GameMsg_C2S_ExitWeddingRoom::doDecode( CParamPool & IOBuff )
{
    m_nQuitTo = IOBuff.GetUChar();
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_KickWeddingRoomPlayer::GameMsg_C2S_KickWeddingRoomPlayer()
:GameMsg_Base(MSG_C2S_KickWeddingRoomPlayer)
,m_nTargetID(0)
{

}

GameMsg_C2S_KickWeddingRoomPlayer::~GameMsg_C2S_KickWeddingRoomPlayer()
{

}

bool GameMsg_C2S_KickWeddingRoomPlayer::doDecode( CParamPool & IOBuff )
{
	m_nTargetID = IOBuff.GetUInt();

	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_SetWeddingPlayerType::GameMsg_C2S_SetWeddingPlayerType()
:GameMsg_Base(MSG_C2S_SetWeddingPlayerType)
,m_nTargetID(0)
,m_nRoomRoleType(0)
{

}

GameMsg_C2S_SetWeddingPlayerType::~GameMsg_C2S_SetWeddingPlayerType()
{

}

bool GameMsg_C2S_SetWeddingPlayerType::doDecode( CParamPool & IOBuff )
{
	m_nTargetID = IOBuff.GetUInt();
	m_nRoomRoleType = IOBuff.GetUChar();
	
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_ChangeWeddingRoomPwd::GameMsg_C2S_ChangeWeddingRoomPwd()
:GameMsg_Base(MSG_C2S_ChangeWeddingRoomPwd)
,m_strNewPwd("")
{

}

GameMsg_C2S_ChangeWeddingRoomPwd::~GameMsg_C2S_ChangeWeddingRoomPwd()
{

}

bool GameMsg_C2S_ChangeWeddingRoomPwd::doDecode( CParamPool & IOBuff )
{
	IOBuff.GetStringW(m_strNewPwd);
	
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_GetBlessingRankList::GameMsg_C2S_GetBlessingRankList()
:GameMsg_Base(MSG_C2S_GetBlessingRankList)
,m_nCurrentPage(0)
,m_nPageCount(0)
{

}

GameMsg_C2S_GetBlessingRankList::~GameMsg_C2S_GetBlessingRankList()
{

}

bool GameMsg_C2S_GetBlessingRankList::doDecode( CParamPool & IOBuff )
{
	m_nCurrentPage = IOBuff.GetUShort();
    m_nPageCount = IOBuff.GetUShort();
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_BlessingCouple::GameMsg_C2S_BlessingCouple()
:GameMsg_Base(MSG_C2S_BlessingPair)
,m_nPairID( 0 )
{

}

bool GameMsg_C2S_BlessingCouple::doDecode( CParamPool & IOBuff )
{
	m_nPairID = IOBuff.GetUInt();

	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_GetWeddingRecordList::GameMsg_C2S_GetWeddingRecordList()
:GameMsg_Base( MSG_C2S_GetWeddingRecordList )
{

}

bool GameMsg_C2S_GetWeddingRecordList::doDecode( CParamPool & IOBuff )
{
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_MoneyBlessingPair::GameMsg_C2S_MoneyBlessingPair()
:GameMsg_Base( MSG_C2S_MoneyBlessingPair )
,m_nPairID( 0 )
,m_nRedEnvelopeCount( 0 )
{
}

bool GameMsg_C2S_MoneyBlessingPair::doDecode( CParamPool & IOBuff )
{
	m_nPairID = IOBuff.GetUInt();
	m_nRedEnvelopeCount  = IOBuff.GetUInt();
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_GetCoupleInfo::GameMsg_C2S_GetCoupleInfo()
:GameMsg_Base( MSG_C2S_GetCoupleInfo )
,m_nPairID( 0 )
{
}

bool GameMsg_C2S_GetCoupleInfo::doDecode( CParamPool & IOBuff )
{
	m_nPairID = IOBuff.GetUInt();
	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_GetBlessingValue::GameMsg_C2S_GetBlessingValue()
:GameMsg_Base( MSG_C2S_GetBlessingValue )
,m_nPairID( 0 )
{
}

bool GameMsg_C2S_GetBlessingValue::doDecode( CParamPool & IOBuff )
{
	m_nPairID = IOBuff.GetUInt();
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_GetPersonInfo::GameMsg_C2S_GetPersonInfo()
:GameMsg_Base( MSG_C2S_GetPersonInfo )
,m_nRoleID( 0 )
{

}

bool GameMsg_C2S_GetPersonInfo::doDecode( CParamPool & IOBuff )
{
	m_nRoleID = IOBuff.GetUInt();
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_GetCoupleRedEnvelopeGiftInfo::GameMsg_C2S_GetCoupleRedEnvelopeGiftInfo()
	:GameMsg_Base( MSG_C2S_GetCoupleRedEnvelopeGiftInfo )
	,m_nPairID( 0 )
{

}

bool GameMsg_C2S_GetCoupleRedEnvelopeGiftInfo::doDecode( CParamPool & IOBuff )
{
	m_nPairID = IOBuff.GetUInt();
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_CancelDivorce::GameMsg_C2S_CancelDivorce()
	:GameMsg_Base( MSG_C2S_RequestCancelDivorce )
{

}

bool GameMsg_C2S_CancelDivorce::doDecode( CParamPool & IOBuff )
{
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_CompulsoryDivorce::GameMsg_C2S_CompulsoryDivorce()
	:GameMsg_Base( MSG_C2S_RequestCompulsoryDivorce )
{

}

bool GameMsg_C2S_CompulsoryDivorce::doDecode( CParamPool & IOBuff )
{
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_AgreeDivorce::GameMsg_C2S_AgreeDivorce()
	:GameMsg_Base( MSG_C2S_AgreeDivorce )
	
{

}

bool GameMsg_C2S_AgreeDivorce::doDecode( CParamPool & IOBuff )
{
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_DivorceChat::GameMsg_C2S_DivorceChat()
	:GameMsg_Base( MSG_C2S_DivorceChat )
	,m_strChatContent("")
{

}

bool GameMsg_C2S_DivorceChat::doDecode( CParamPool & IOBuff )
{
	IOBuff.GetStringW( m_strChatContent );
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_DivorceGift::GameMsg_C2S_DivorceGift()
	:GameMsg_Base( MSG_C2S_DivorceSendGift )
	,m_nDestRoleID(0)
{

}

bool GameMsg_C2S_DivorceGift::doDecode( CParamPool & IOBuff )
{
	m_nDestRoleID = IOBuff.GetUInt();
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++)
	{
		PeddleGood aPeddleGood;
		aPeddleGood.doDecode(IOBuff);
		vecGoods.push_back(aPeddleGood);
	}

	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_DivorceWish::GameMsg_C2S_DivorceWish()
	:GameMsg_Base( MSG_C2S_RequestCoupleWish )
	,m_nDestRoleID(0)
{

}

bool GameMsg_C2S_DivorceWish::doDecode( CParamPool & IOBuff )
{
	m_nDestRoleID = IOBuff.GetUInt();
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_DivorceAnimation::GameMsg_C2S_DivorceAnimation()
	:GameMsg_Base( MSG_C2S_RequestEnterDivorceAnimation )
{

}

bool GameMsg_C2S_DivorceAnimation::doDecode( CParamPool & IOBuff )
{
	return true;
}







