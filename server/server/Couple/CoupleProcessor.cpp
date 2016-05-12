#include "CoupleProcessor.h"
#include "../group/Global.h"
#include "../../socket/GameMsg_Map.h"
#include "../libServerFrame/QueryManager.h"
#include "CoupleMsgDef.h"
#include "GameMsg_S2G_Couple.h"
#include "GameMsg_G2S_Couple.h"
#include "../group/Global.h"
//#include "../../../datastructure/Localization.h"

#include "../Rank/GlobalRankListManager.h"
#include "../attribute/WAttributeManager.h"
#include "../player/GameMsg_S2G_PlayerInfo.h"

extern CQueryManager		*g_pQueryMgr;

CCoupleProcessor::CCoupleProcessor()
:GameMsg_Processor(true, true)
,m_bLoadOfflineCoupleInfoSuc(false)
{

}

CCoupleProcessor::~CCoupleProcessor()
{

}

CCoupleProcessor & CCoupleProcessor::Instance()
{
	static CCoupleProcessor s_WMarriageManager;

	return s_WMarriageManager;
}

void CCoupleProcessor::Initialize()
{
	_RegisterMessage();

	// 加载排行榜信息
	m_BlessingValueRankMgr.LoadRankInfo();

	// 加载offlinecouple info
	g_pQueryMgr->AddQuery( QUERY_Couple_LoadOfflineCoupleInfo, 0, &m_OfflineCoupleInfoTable );
}

void CCoupleProcessor::_RegisterMessage()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_AcceptCoupleRequest);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CoupleSeprate);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetMarried);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_Divorced);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_WeddingRoomStartingNofication);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateCoupleIntimacy);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_AutoStartWeddingFail);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_BlessingCouple);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetWeddingRecordList);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_AddWeddingRecord);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateWeddingRecordState);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetBlessingRankList);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateWeddingRecord);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_RedEnvelope);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetCoupleInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetBlessingValue);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetPersonInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetDivorceAnimationPersonInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_WeddingNotice);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_AddRedEnvelopeBlessingValue);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_RefreshDivorceAction);

	
	RegMsgProcessor(MSG_S2G_AcceptCoupleRequst, &CCoupleProcessor::_OnAcceptCoupleRequest);
	RegMsgProcessor(MSG_S2G_CoupleSeparate, &CCoupleProcessor::_OnCoupleSeparated);
	RegMsgProcessor(MSG_S2G_GetMarried, &CCoupleProcessor::_OnGetMarried);
	RegMsgProcessor(MSG_S2G_Divorce, &CCoupleProcessor::_OnDivorced);
	RegMsgProcessor(MSG_S2G_WeddingRoomStartingNofication, &CCoupleProcessor::_OnWeddingRoomStartNofication);
	RegMsgProcessor(MSG_S2G_UpdateCoupleIntimacy, &CCoupleProcessor::_OnUpdateCoupleIntimacy);
	RegMsgProcessor(MSG_S2G_AutoStartWeddingRoomFail, &CCoupleProcessor::_OnAutoStartWeddingRoomFail);
    RegMsgProcessor(MSG_S2G_BlessingPair, &CCoupleProcessor::OnBlessingCouple);
	RegMsgProcessor(MSG_S2G_GetWeddingRecordList, &CCoupleProcessor::OnGetWeddingRecordList);
	RegMsgProcessor(MSG_S2G_AddWeddingRecord, &CCoupleProcessor::OnAddWeddingRecord);
	RegMsgProcessor(MSG_S2G_UpateWeddingRecordState, &CCoupleProcessor::OnUpdateWeddingRecordRunningState);
	RegMsgProcessor(MSG_S2G_GetBlessingRankList, &CCoupleProcessor::OnGetBlessingRankList);
    RegMsgProcessor(MSG_S2G_UpdateWeddingRecord, &CCoupleProcessor::OnUpdateWeddingRecord);
	RegMsgProcessor(MSG_S2G_MoneyBlessingPair, &CCoupleProcessor::OnRedEnvelope);
	RegMsgProcessor(MSG_S2G_GetCoupleInfo, &CCoupleProcessor::OnGetCoupleInfo);
	RegMsgProcessor(MSG_S2G_GetBlessingValue, &CCoupleProcessor::OnGetBlessingValue);
	RegMsgProcessor(MSG_S2G_GetPersonInfo, &CCoupleProcessor::OnGetPersonInfo);
	RegMsgProcessor(MSG_S2G_GetDivorceAnimationPersonInfo, &CCoupleProcessor::OnGetPersonInfoFromDivorce);
	RegMsgProcessor(MSG_S2G_WeddingNotice, &CCoupleProcessor::OnWeddingNotice);
	RegMsgProcessor(MSG_S2G_AddRedEnvelopeBlessingValue, &CCoupleProcessor::OnAddRedEnvelopeBlessingValue);
	RegMsgProcessor(MSG_S2G_RefreshDivorceAction, &CCoupleProcessor::OnRefreshDivorceAction);


	// 数据库
    RegQueryProcessor(QUERY_Couple_LoadBlessingValueRank,&CCoupleProcessor::OtLoadRankInfoSuc);
	RegQueryProcessor(QUERY_Couple_LoadOfflineCoupleInfo,&CCoupleProcessor::OnLoadOfflineCoupleInfoSuc);


}

void CCoupleProcessor::_OnAcceptCoupleRequest( GameMsg_Base &msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_AcceptCoupleRequest &msgAccept = (GameMsg_S2G_AcceptCoupleRequest &)msg;
	
	COfflineCoupleInfo offlinecoupleinfo;
	offlinecoupleinfo.m_nPairID = msgAccept.m_nPairID;
	offlinecoupleinfo.m_nMaleRoleID = msgAccept.m_nMaleID;
	offlinecoupleinfo.m_nFemaleRoleID = msgAccept.m_nFemaleID;
	offlinecoupleinfo.m_nCoupleDate = msgAccept.m_nCoupleDate;
    offlinecoupleinfo.m_nSmallRoleID = msgAccept.m_nSmallRoleID;

	CGroupRoleEntity *pMale = CGroupPlayerManager::Instance().SearchRoleByID(msgAccept.m_nMaleID);
	if (pMale != NULL) 
    {
		pMale->SetPairID(msgAccept.m_nPairID);
		pMale->SetCoupleID(msgAccept.m_nFemaleID);
		pMale->SetCoupleDate(msgAccept.m_nCoupleDate);
        pMale->SetCoupleSmallRoleID(msgAccept.m_nSmallRoleID);

		offlinecoupleinfo.m_strMaleRoleName = pMale->GetName();
	}

	CGroupRoleEntity *pFemale = CGroupPlayerManager::Instance().SearchRoleByID(msgAccept.m_nFemaleID);
	if (pFemale != NULL) 
    {
		pFemale->SetPairID(msgAccept.m_nPairID);
		pFemale->SetCoupleID(msgAccept.m_nMaleID);	
		pFemale->SetCoupleDate(msgAccept.m_nCoupleDate);
        pFemale->SetCoupleSmallRoleID(msgAccept.m_nSmallRoleID);

		offlinecoupleinfo.m_strFemaleRoleName = pFemale->GetName();
	}

	AddOfflineCoupleInfo( offlinecoupleinfo );
}

void CCoupleProcessor::_OnCoupleSeparated(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_CoupleSeprate & msgSeparated = (GameMsg_S2G_CoupleSeprate &)msg;
	CGroupRoleEntity *pSeparate = CGroupPlayerManager::Instance().SearchRoleByID(msgSeparated.m_nSeparateID);		
	if (pSeparate != NULL) {
		pSeparate->SetPairID(0);
		pSeparate->SetCoupleID(0);
		pSeparate->SetCoupleDate(0);
	}
	
	CGroupRoleEntity *pToBeSeparated = CGroupPlayerManager::Instance().SearchRoleByID(msgSeparated.m_nToBeSeparateID);
	if (pToBeSeparated != NULL) {
		pToBeSeparated->SetPairID(0);
		pToBeSeparated->SetCoupleID(0);
		pToBeSeparated->SetCoupleDate(0);
		if (msgSeparated.m_bNeedNotify) {
			GameMsg_G2S_NotifyHasBeenSeparated msgNotify;
			msgNotify.m_nSeparatedBy = msgSeparated.m_nSeparateID;
			if (pSeparate != NULL) {
				msgNotify.m_strSeparatedBy = pSeparate->GetName();
			}
			pToBeSeparated->SendPlayerMsg(&msgNotify);
		}
	}

    // 删除婚礼动态记录
    m_WeddingRecord.DeleteRecord( msgSeparated.m_nPairID );
}

void CCoupleProcessor::_OnGetMarried(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_GetMarried msgGetMarried = (GameMsg_S2G_GetMarried &)msg;	

	CGroupRoleEntity *pMarried = CGroupPlayerManager::Instance().SearchRoleByID(msgGetMarried.m_nMaleRoleID);		
	if (pMarried != NULL && pMarried->MarriageDate() == 0 ) 
	{
		pMarried->SetMarriageDate(msgGetMarried.m_nMarriageDate);
	}

	CGroupRoleEntity *pBeMarried = CGroupPlayerManager::Instance().SearchRoleByID(msgGetMarried.m_nFemaleRoleID);		
	if (pBeMarried != NULL && pBeMarried->MarriageDate() == 0 ) 
	{
		pBeMarried->SetMarriageDate(msgGetMarried.m_nMarriageDate);
	}

	// 更新排行榜数据
	CBlessingValueRankInfo rankinfo;
	rankinfo.m_nPairID = msgGetMarried.m_nPairID;
	rankinfo.m_nMaleRoleID = msgGetMarried.m_nMaleRoleID;
	rankinfo.m_strMaleRoleName = msgGetMarried.m_strMaleRoleName;
	rankinfo.m_nFemaleRoleID = msgGetMarried.m_nFemaleRoleID;
	rankinfo.m_strFemaleRoleName = msgGetMarried.m_strFemaleRoleName;
	rankinfo.m_nBlessingValue = msgGetMarried.m_nBlessingValue;
	m_BlessingValueRankMgr.UpdateBlessingValueInfo( rankinfo );

	// 离线信息 
	CGroupOffLinePlayerManager::Instance().LoadOfflineRoleImageData( msgGetMarried.m_nMaleRoleID );
	CGroupOffLinePlayerManager::Instance().LoadOfflineRoleImageData( msgGetMarried.m_nFemaleRoleID );

	// coupleinfo
	UpdateCoupleInfoMarriageData( msgGetMarried.m_nPairID, msgGetMarried.m_nMarriageDate );
	UpdateCoupleInfoBlessingValue( msgGetMarried.m_nPairID, msgGetMarried.m_nBlessingValue );

}

void CCoupleProcessor::_OnDivorced(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_Divorced msgDivorced = (GameMsg_S2G_Divorced &)msg;

	CGroupRoleEntity *pDivorce = CGroupPlayerManager::Instance().SearchRoleByID(msgDivorced.m_nDivorceID);		
	if (pDivorce != NULL) 
	{
		pDivorce->ResetCoupleInfo();
	}

	CGroupRoleEntity *pBeDivorced = CGroupPlayerManager::Instance().SearchRoleByID(msgDivorced.m_nBeDivorcedID);		
	if (pBeDivorced != NULL) 
	{
		pBeDivorced->ResetCoupleInfo();
		if (msgDivorced.m_bNeedNotify) 
		{
			GameMsg_G2S_NotifyHasBeenDivorced msgNotify;
			msgNotify.m_nDivorcedBy = msgDivorced.m_nDivorceID;
			if (pDivorce != NULL) 
			{
				msgNotify.m_strDivorcedBy = pDivorce->GetName();
			}
			pBeDivorced->SendPlayerMsg(&msgNotify);
		}
	}

	// 删除排行榜
	m_BlessingValueRankMgr.DelBlessingValueInfo( msgDivorced.m_nPairID );

    CGlobalRankListManager::Instance().RemoveFromRank(ERankDataType_Intimacy, msgDivorced.m_nPairID);

	// 删除婚礼动态记录
	m_WeddingRecord.DeleteRecord( msgDivorced.m_nPairID );

	// 删除coupleinfo
	DeleteOfflineCoupleInfo( msgDivorced.m_nPairID );
}

void CCoupleProcessor::_OnWeddingRoomStartNofication( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_WeddingRoomStartingNofication & msgReceived = (GameMsg_S2G_WeddingRoomStartingNofication & )msg;

	GameMsg_G2S_WeddingRoomStartingNofication msgNotification;
	msgNotification.m_nLine = msgReceived.m_nLine;
	msgNotification.m_nRoomID = msgReceived.m_nRoomID;
	msgNotification.m_nStartTime = msgReceived.m_nStartTime;

	CGroupRoleEntity *pGroom =  CGroupPlayerManager::Instance().SearchRoleByID(msgReceived.m_nGroomID);
	if (pGroom != NULL) {
		pGroom->SendPlayerMsg(&msgNotification);
	}

	CGroupRoleEntity *pBride =  CGroupPlayerManager::Instance().SearchRoleByID(msgReceived.m_nBrideID);
	if (pBride != NULL) {
		pBride->SendPlayerMsg(&msgNotification);
	}
}

void CCoupleProcessor::_OnAutoStartWeddingRoomFail(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_AutoStartWeddingFail & msgReceived = (GameMsg_S2G_AutoStartWeddingFail &)msg;

	GameMsg_G2S_AutoStartWeddingFail msgNotification;
	msgNotification.m_nError = msgReceived.m_nError;
	msgNotification.m_strError = msgReceived.m_strError;

	CGroupRoleEntity *pGroom =  CGroupPlayerManager::Instance().SearchRoleByID(msgReceived.m_nGroomID);
	if (pGroom != NULL) {
		pGroom->SendPlayerMsg(&msgNotification);
	}

	CGroupRoleEntity *pBride =  CGroupPlayerManager::Instance().SearchRoleByID(msgReceived.m_nBrideID);
	if (pBride != NULL) {
		pBride->SendPlayerMsg(&msgNotification);
	}
}

void CCoupleProcessor::OnPlayerLogin(CGroupRoleEntity & role)
{
	// 离线形象
	CGroupOffLinePlayerManager::Instance().LoadOfflineRoleImageData( role.CoupleID() );
	CGroupOffLinePlayerManager::Instance().LoadOfflineRoleImageData( role.GetDivorecAgreementOtherRoleID() );

	// coupleinfo
	const CCoupleInfo & coupleinfo = role.GetCoupleInfo();
	COfflineCoupleInfo offlinecoupleinfo;
	offlinecoupleinfo.m_nPairID = coupleinfo.m_nPairID;
	if ( role.GetSex() == ESexType_Male )
	{
		offlinecoupleinfo.m_nMaleRoleID = role.GetRoleID();
		offlinecoupleinfo.m_strMaleRoleName = role.GetName();
		offlinecoupleinfo.m_nFemaleRoleID = coupleinfo.m_nCoupleID;
		offlinecoupleinfo.m_strFemaleRoleName = coupleinfo.m_strCoupleName;
	}
	else
	{
		offlinecoupleinfo.m_nFemaleRoleID = role.GetRoleID();
		offlinecoupleinfo.m_strFemaleRoleName = role.GetName();
		offlinecoupleinfo.m_nMaleRoleID = coupleinfo.m_nCoupleID;
		offlinecoupleinfo.m_strMaleRoleName = coupleinfo.m_strCoupleName;
	}

	offlinecoupleinfo.m_nIntimacy = coupleinfo.m_nIntimacy;
	offlinecoupleinfo.m_nMarriageDate = coupleinfo.m_nMarriageDate;
	offlinecoupleinfo.m_nCoupleDate = coupleinfo.m_nCoupleDate;
	offlinecoupleinfo.m_strLoveDeclaration = coupleinfo.m_strLoveDeclaration;
	offlinecoupleinfo.m_nBlessingValue = coupleinfo.m_nBlessingValue;
    offlinecoupleinfo.m_nSmallRoleID = coupleinfo.m_nSmallRoleID;
	AddOfflineCoupleInfo( offlinecoupleinfo );
}

void CCoupleProcessor::_OnUpdateCoupleIntimacy(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_UpdateCoupleIntimacy & msgFrom = (GameMsg_S2G_UpdateCoupleIntimacy &)msg;

	CGroupRoleEntity *pTrigger =  CGroupPlayerManager::Instance().SearchRoleByID(msgFrom.m_nTriggerID);
	if (pTrigger != NULL) 
	{
		pTrigger->SetCoupleIntimacy(msgFrom.m_nChangedValue);

		const CCoupleInfo & coupleinfo = pTrigger->GetCoupleInfo();
		UpdateCoupleInfoIntimacy( coupleinfo.m_nPairID, coupleinfo.m_nIntimacy );
	}

	CGroupRoleEntity *pCouple =  CGroupPlayerManager::Instance().SearchRoleByID(msgFrom.m_nCoupleID);
	if (pCouple != NULL) {
		pCouple->SetCoupleIntimacy(msgFrom.m_nChangedValue);
		GameMsg_G2S_UpdateCoupleIntimacy msgToGame;
		msgToGame.m_nChangedValue = msgFrom.m_nChangedValue;
		pCouple->SendPlayerMsg(&msgToGame);
	}

    if (msgFrom.m_bMarried)
    {
        unsigned int nMaleRoleID = msgFrom.m_nTriggerID;
        unsigned int nFemaleRoleID = msgFrom.m_nCoupleID;

        if (!msgFrom.m_bMale)
        {
            nMaleRoleID = msgFrom.m_nCoupleID;
            nFemaleRoleID = msgFrom.m_nTriggerID;
        }

        CGlobalRankListManager::Instance().ChangeCoupleRankValue( msgFrom.m_nPairID,
            msgFrom.m_nChangedValue, nMaleRoleID, nFemaleRoleID, msgFrom.m_nSmallRoleID );
    }
}

void CCoupleProcessor::OtLoadRankInfoSuc( QueryBase &pQuery )
{
	m_BlessingValueRankMgr.LoadRankInfoSuc();
}

void CCoupleProcessor::OnLoadOfflineCoupleInfoSuc( QueryBase &pQuery )
{
	m_bLoadOfflineCoupleInfoSuc = true;
}

void CCoupleProcessor::OnBlessingCouple( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_BlessingCouple & s2gMsg = ( GameMsg_S2G_BlessingCouple &)msg;

	// 判断是否还在进行中
	WeddingRecordInfo * pRecord = m_WeddingRecord.FindWeddingRecord( s2gMsg.m_nPairID );
	if ( pRecord != NULL && pRecord->m_nIsRunning )
	{
		// 是否已经祝福过
		if ( pRecord->IsInBlessingRoleList( s2gMsg.m_nRoleID ) )
		{
			CGroupRoleEntity *pRole =  CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nRoleID );
			if ( pRole != NULL )
			{
				GameMsg_G2S_BlessingError errMsg;
				errMsg.m_strError = "ECoupleMsgError_HaveBlessing";
				::SendMsg2GameServer( &errMsg, pRole->GetLine());
			}
		}
		else
		{
			// 转发到pair所在线
			GameMsg_G2S_BlessingCouple g2sMsg;
			g2sMsg.m_nRoleID = s2gMsg.m_nRoleID;
			g2sMsg.m_nPairID = s2gMsg.m_nPairID;
			g2sMsg.m_nSex    = s2gMsg.m_nSex;

			::SendMsg2GameServer( &g2sMsg, pRecord->m_nLine );
		}
	}
	else
	{
		// 祝福的pair婚礼已经结束
		CGroupRoleEntity *pRole =  CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nRoleID );
		if ( pRole != NULL )
		{
			GameMsg_G2S_BlessingError errMsg;
			errMsg.m_strError = "ECoupleMsgError_BlessingPairNot";

			::SendMsg2GameServer( &errMsg, pRole->GetLine());
		}
	}
}

void CCoupleProcessor::OnGetWeddingRecordList( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_GetWeddingRecordList & s2gMsg = ( GameMsg_S2G_GetWeddingRecordList &)msg;

	CGroupRoleEntity *pRole =  CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nRoleID );
	if (pRole != NULL) 
	{
		GameMsg_G2S_GetWeddingRecordList g2sMsg;
		m_WeddingRecord.GetRecordList( g2sMsg.m_RecordList );
		
		pRole->SendPlayerMsg( &g2sMsg );
	}
}

void CCoupleProcessor::OnAddWeddingRecord( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_AddWeddingRecord & s2gMsg = ( GameMsg_S2G_AddWeddingRecord &)msg;
	
	m_WeddingRecord.AddRecord( s2gMsg.m_Record );

	CGroupOffLinePlayerManager::Instance().LoadOfflineRoleImageData( s2gMsg.m_Record.m_nMaleID );
	CGroupOffLinePlayerManager::Instance().LoadOfflineRoleImageData( s2gMsg.m_Record.m_nFemaleID );


	// offline coupleinfo
	CGroupRoleEntity * pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_Record.m_nMaleID);
	if ( pRoleEntity == NULL )
	{
		pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_Record.m_nFemaleID );
		if ( pRoleEntity != NULL )
		{
			const CCoupleInfo & coupleinfo = pRoleEntity->GetCoupleInfo();
			COfflineCoupleInfo offlinecoupleinfo;
			offlinecoupleinfo.m_nPairID = s2gMsg.m_Record.m_nPairID;
			offlinecoupleinfo.m_nMaleRoleID = s2gMsg.m_Record.m_nMaleID;
			offlinecoupleinfo.m_strMaleRoleName = s2gMsg.m_Record.m_strMaleName;
			offlinecoupleinfo.m_nFemaleRoleID = s2gMsg.m_Record.m_nFemaleID;
			offlinecoupleinfo.m_strFemaleRoleName = s2gMsg.m_Record.m_strFemaleName;
			offlinecoupleinfo.m_nIntimacy = coupleinfo.m_nIntimacy;
			offlinecoupleinfo.m_nMarriageDate = coupleinfo.m_nMarriageDate;
			offlinecoupleinfo.m_nCoupleDate = coupleinfo.m_nCoupleDate;
			offlinecoupleinfo.m_strLoveDeclaration = coupleinfo.m_strLoveDeclaration;
			offlinecoupleinfo.m_nBlessingValue = coupleinfo.m_nBlessingValue;
            offlinecoupleinfo.m_nSmallRoleID = coupleinfo.m_nSmallRoleID;

			AddOfflineCoupleInfo( offlinecoupleinfo );
		}
	}
	else
	{
		const CCoupleInfo & coupleinfo = pRoleEntity->GetCoupleInfo();
		COfflineCoupleInfo offlinecoupleinfo;
		offlinecoupleinfo.m_nPairID = s2gMsg.m_Record.m_nPairID;
		offlinecoupleinfo.m_nMaleRoleID = s2gMsg.m_Record.m_nMaleID;
		offlinecoupleinfo.m_strMaleRoleName = s2gMsg.m_Record.m_strMaleName;
		offlinecoupleinfo.m_nFemaleRoleID = s2gMsg.m_Record.m_nFemaleID;
		offlinecoupleinfo.m_strFemaleRoleName = s2gMsg.m_Record.m_strFemaleName;
		offlinecoupleinfo.m_nIntimacy = coupleinfo.m_nIntimacy;
		offlinecoupleinfo.m_nMarriageDate = coupleinfo.m_nMarriageDate;
		offlinecoupleinfo.m_nCoupleDate = coupleinfo.m_nCoupleDate;
		offlinecoupleinfo.m_strLoveDeclaration = coupleinfo.m_strLoveDeclaration;
		offlinecoupleinfo.m_nBlessingValue = coupleinfo.m_nBlessingValue;
        offlinecoupleinfo.m_nSmallRoleID = coupleinfo.m_nSmallRoleID;

		AddOfflineCoupleInfo( offlinecoupleinfo );
	}
}

void CCoupleProcessor::OnUpdateWeddingRecordRunningState( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_UpdateWeddingRecordState & s2gMsg = (GameMsg_S2G_UpdateWeddingRecordState &)msg;

	if ( s2gMsg.m_nDoType == 1 )
	{
		m_WeddingRecord.UpdateRecordIsRunning( s2gMsg.m_nPairID, s2gMsg.m_bIsRunning );
	}
	else if ( s2gMsg.m_nDoType == 2 )
	{
		m_WeddingRecord.DeleteRecord( s2gMsg.m_nPairID );
	}	
}

void CCoupleProcessor::OnGetBlessingRankList( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
    GameMsg_S2G_GetBlessingRankList & s2gMsg = ( GameMsg_S2G_GetBlessingRankList &)msg;
	CGroupRoleEntity *pRole =  CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nRoleID );
	if (pRole != NULL) 
	{
		GameMsg_G2S_GetBlessingRankList g2sMsg;
		m_BlessingValueRankMgr.GetBlessingRankList(s2gMsg.m_nCurrentPage, s2gMsg.m_nPageCount, g2sMsg.m_BlessingValueRankVec);
		g2sMsg.m_nCurrentPage = s2gMsg.m_nCurrentPage;
		g2sMsg.m_nRankTotalCount =  m_BlessingValueRankMgr.GetRankTotalCount();
        g2sMsg.m_nPageCount = s2gMsg.m_nPageCount;

		pRole->SendPlayerMsg( &g2sMsg) ;
	}
}

void CCoupleProcessor::OnUpdateWeddingRecord( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_UpdateWeddingRecord & s2gMsg = (GameMsg_S2G_UpdateWeddingRecord &)msg;

	if ( s2gMsg.m_nBlessingType == GameMsg_S2G_UpdateWeddingRecord::EBlessingType_BlessingCouple )
	{
		m_WeddingRecord.AddBlessingRoleIDInList( s2gMsg.m_nPairID, s2gMsg.m_nBlessingRoleID );
	}

	m_WeddingRecord.UpdateBlessingValue( s2gMsg.m_nPairID, s2gMsg.m_nBlessingValue );

	CGroupRoleEntity *pRole =  CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nBlessingRoleID );
	if ( pRole != NULL )
	{
		if ( s2gMsg.m_nBlessingType == GameMsg_S2G_UpdateWeddingRecord::EBlessingType_BlessingCouple )
		{
			GameMsg_G2S_BlessingCoupleScuess Msg;
			Msg.nIndex = pRole->m_nIndex;
			Msg.SessionID = pRole->m_nSession;
			::SendMsg2GameServer( &Msg, pRole->GetLine());
		}
		else
		{
			GameMsg_G2S_RedEnvelopeScuess Msg;
			Msg.nIndex = pRole->m_nIndex;
			Msg.SessionID = pRole->m_nSession;
			::SendMsg2GameServer( &Msg, pRole->GetLine());
		}
	}
}

void CCoupleProcessor::OnRedEnvelope( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	//直接转发过去对couple操作
	GameMsg_S2G_RedEnvelope & s2gMsg = (GameMsg_S2G_RedEnvelope &)msg;

	CGroupRoleEntity *pRole =  CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nRoleID );
	if ( pRole != NULL )
	{
		WeddingRecordInfo * pRecord = m_WeddingRecord.FindWeddingRecord( s2gMsg.m_nPairID );
		if ( pRecord != NULL && pRecord->m_nIsRunning )
		{
			GameMsg_G2S_RedEnvelopeDelMoney g2sMsg;
			g2sMsg.m_nRedEnvelopeCount = s2gMsg.m_nRedEnvelopeCount;
			g2sMsg.m_nPairID = s2gMsg.m_nPairID;

			pRole->SendPlayerMsg( &g2sMsg );
		}
		else
		{
			GameMsg_G2S_BlessingError errMsg;
			errMsg.m_strError = "ECoupleMsgError_BlessingPairNot";
			::SendMsg2GameServer( &errMsg, pRole->GetLine());
		}
	}
}

void CCoupleProcessor::OnGetCoupleInfo( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_GetCoupleInfo & s2gMsg = ( GameMsg_S2G_GetCoupleInfo & )msg;

	std::map<unsigned int,COfflineCoupleInfo>::iterator it =  m_OfflineCoupleInfoTable.find( s2gMsg.m_nPairID );
	if ( it != m_OfflineCoupleInfoTable.end() )
	{
		CGroupRoleEntity *pRoleEntity =  CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nRoleID );
		if (pRoleEntity != NULL) 
		{
			GameMsg_G2S_GetCoupleInfo  g2sMsg;
			g2sMsg.m_nRank = m_BlessingValueRankMgr.GetSelfRank( s2gMsg.m_nPairID );
			g2sMsg.m_CoupleInfo = it->second;

			pRoleEntity->SendPlayerMsg( &g2sMsg) ;
		}
	}
}

void CCoupleProcessor::OnGetBlessingValue( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_GetBlessingValue & s2gMsg = ( GameMsg_S2G_GetBlessingValue &)msg;

	CGroupRoleEntity *pRole =  CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nRoleID );
	if ( pRole != NULL )
	{
		GameMsg_G2S_GetBlessingValue  g2sMsg;
		WeddingRecordInfo * pRecord = m_WeddingRecord.FindWeddingRecord( s2gMsg.m_nPairID );
		if ( pRecord != NULL )
		{
			g2sMsg.m_nBlessingValue = pRecord->m_nBlessingValue;	
            g2sMsg.m_nLine = (unsigned short)pRecord->m_nLine;
		}

		pRole->SendPlayerMsg( &g2sMsg) ;
	}
}

void CCoupleProcessor::OnGetPersonInfoFromDivorce( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_GetDivorceAnimationPersonInfo & s2gMsg = (GameMsg_S2G_GetDivorceAnimationPersonInfo &)msg;

	CGroupRoleEntity *pRole =  CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nCoupleRoleID );
	if (pRole == NULL) 
	{
		GameMsg_G2S_GetDivorceeAnimationPersonInfo g2sMsg;
		CGroupOffLineRoleEntity * pRolentity = CGroupOffLinePlayerManager::Instance().GetOfflineRoleImageData( s2gMsg.m_nCoupleRoleID );
		if ( pRolentity != NULL )
		{
			pRolentity->GetOfflineRoleImageData(g2sMsg.m_OffLineData);
			CGroupRoleEntity *pSelfRole =  CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nSelfRoleID );
			if ( pSelfRole != NULL )
			{
				pSelfRole->SendPlayerMsg( &g2sMsg) ;
			}		   
		}
		else
		{
			// error log
			WriteLog(LOGLEVEL_DEBUG, "<<<<<OnGetPersonInfoFromDivorce");
		}
	}
	else
	{
        CWAttributeManager::Instance().GetOfflinePlayerInfo( s2gMsg.m_nCoupleRoleID, s2gMsg.m_nSelfRoleID, EOfflinePlayerDataFrom_Divorce);
	}
}

void CCoupleProcessor::OnGetPersonInfo( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_GetPersonInfo & s2gMsg = (GameMsg_S2G_GetPersonInfo &)msg;

	CGroupRoleEntity *pRole =  CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nRoleID );
	if (pRole == NULL) 
	{
		GameMsg_G2S_GetPersonInfo g2sMsg;
		CGroupOffLineRoleEntity * pRolentity = CGroupOffLinePlayerManager::Instance().GetOfflineRoleImageData( s2gMsg.m_nRoleID );
		if ( pRolentity != NULL )
		{
			pRolentity->GetOfflineRoleImageData( g2sMsg.m_OffLineData  );
			CGroupRoleEntity *pSelfRole =  CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nSelfRoleID );
			if ( pSelfRole != NULL )
			{
				pSelfRole->SendPlayerMsg( &g2sMsg) ;
			}
			else
			{
				// error log
				WriteLog(LOGLEVEL_DEBUG, "<<<<<OnGetPersonInfo");
			}
		}
	}
	else
	{
        CWAttributeManager::Instance().GetOfflinePlayerInfo( s2gMsg.m_nRoleID, s2gMsg.m_nSelfRoleID, EOfflinePlayerDataFrom_Couple);
	}
}

void CCoupleProcessor::OnGetOnlinePersonInfo( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
    GameMsg_S2G_GetOfflinePlayerInfo & s2gMsg = (GameMsg_S2G_GetOfflinePlayerInfo &)msg;

    CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(s2gMsg.m_nRoleID);
    switch (s2gMsg.m_eFrom)
    {
    case EOfflinePlayerDataFrom_Couple:
        if (pRole != NULL)
        {
            GameMsg_G2S_GetPersonInfo g2sMsg;
            g2sMsg.m_OffLineData = s2gMsg.m_OffLineData;

            pRole->SendPlayerMsg(&g2sMsg);
        }
        break;
    case EOfflinePlayerDataFrom_Divorce:
        if (pRole != NULL)
        {
            GameMsg_G2S_GetDivorceeAnimationPersonInfo g2sMsg;
            g2sMsg.m_OffLineData = s2gMsg.m_OffLineData;

            pRole->SendPlayerMsg(&g2sMsg);
        }
        break;
    case EOfflinePlayerDataFrom_Logout:
    case EOfflinePlayerDataFrom_None:
    default:
        break;
    }
}

void CCoupleProcessor::OnWeddingNotice( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_WeddingNotice & s2gMsg = (GameMsg_S2G_WeddingNotice&)msg;
	
	CGroupRoleEntity *pRole =  CGroupPlayerManager::Instance().SearchRoleByID( s2gMsg.m_nRoleID );
	if ( pRole != NULL )
	{
		GameMsg_G2S_WeddingNotice g2sMsg;
		g2sMsg.m_strMaleRoleName = s2gMsg.m_strMaleRoleName;
		g2sMsg.m_strFemaleRoleName = s2gMsg.m_strFeMaleRoleName;

		pRole->SendPlayerMsg( &g2sMsg );
	}
}

void CCoupleProcessor::OnAddRedEnvelopeBlessingValue(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_AddRedEnvelopeBlessingValue &s2gMsg = (GameMsg_S2G_AddRedEnvelopeBlessingValue&)msg;

	WeddingRecordInfo * pRecord = m_WeddingRecord.FindWeddingRecord( s2gMsg.m_nPairID );
	if ( pRecord != NULL && pRecord->m_nIsRunning )
	{
		GameMsg_G2S_AddRedEnvelopeBlessingValue  g2sMsg;
		g2sMsg.m_nPairID     = s2gMsg.m_nPairID;
		g2sMsg.m_nRedEnvelopeCount      = s2gMsg.m_nRedEnvelopeCount;
		g2sMsg.m_nRoleID     = s2gMsg.m_nBlessingRoleID;
		g2sMsg.m_strRoleName = s2gMsg.m_strBlessingRoleName;

		::SendMsg2GameServer( &g2sMsg, pRecord->m_nLine );
	}	
}

void CCoupleProcessor::OnRefreshDivorceAction(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_RefreshDivorceAction &s2gmsg = (GameMsg_S2G_RefreshDivorceAction&)msg;

	CGroupRoleEntity * pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID( s2gmsg.m_nRoleID );
	if ( pRoleEntity != NULL )
	{
		GameMsg_G2S_RefreshDivorceAction g2smsg;
		g2smsg.m_Action = s2gmsg.m_action;
		g2smsg.m_DivorceAgreementInfo = s2gmsg.m_DivorceAgreementInfo;

		pRoleEntity->SendPlayerMsg( &g2smsg );
	}
}

void CCoupleProcessor::SyncChangeNickname( unsigned int nPairID, unsigned int nRoleID, std::string & strNickname )
{
	// rank
	m_BlessingValueRankMgr.UpdatePlayerNickname( nPairID, nRoleID, strNickname );

	// wedding record
	m_WeddingRecord.UpdatePlayerNickname( nPairID, nRoleID, strNickname );

	// offline couple info
	UpdateCoupleInfoPlayerNickname( nPairID, nRoleID, strNickname );
}

void CCoupleProcessor::AddOfflineCoupleInfo( const COfflineCoupleInfo & coupleinfo )
{
	std::map<unsigned int,COfflineCoupleInfo>::iterator it = m_OfflineCoupleInfoTable.find( coupleinfo.m_nPairID );
	if ( it == m_OfflineCoupleInfoTable.end() )
	{
		m_OfflineCoupleInfoTable.insert( make_pair(coupleinfo.m_nPairID, coupleinfo));
	}
}

void CCoupleProcessor::DeleteOfflineCoupleInfo( unsigned int nPairID )
{
	std::map<unsigned int,COfflineCoupleInfo>::iterator it = m_OfflineCoupleInfoTable.find( nPairID );
	if ( it != m_OfflineCoupleInfoTable.end() )
	{
		m_OfflineCoupleInfoTable.erase( it );
	}
}

void CCoupleProcessor::UpdateCoupleInfoPlayerNickname( unsigned int nPairID, unsigned int nRoleID, std::string & strNickName )
{
	std::map<unsigned int,COfflineCoupleInfo>::iterator it = m_OfflineCoupleInfoTable.find( nPairID );
	if ( it != m_OfflineCoupleInfoTable.end() )
	{
		if ( it->second.m_nMaleRoleID == nRoleID )
		{
			it->second.m_strMaleRoleName = strNickName;
		}
		else if ( it->second.m_nFemaleRoleID == nRoleID )
		{
			it->second.m_strFemaleRoleName = strNickName;
		}
	}
}

void CCoupleProcessor::UpdateCoupleInfoMarriageData( unsigned int nPairID, unsigned int nMarriageData )
{
	std::map<unsigned int,COfflineCoupleInfo>::iterator it = m_OfflineCoupleInfoTable.find( nPairID );
	if ( it != m_OfflineCoupleInfoTable.end() )
	{
        if ( it->second.m_nMarriageDate == 0 )
        {
            it->second.m_nMarriageDate = nMarriageData;
        }
	}
}

void CCoupleProcessor::UpdateCoupleInfoIntimacy( unsigned int nPairID, unsigned int nIntimacy )
{
	std::map<unsigned int,COfflineCoupleInfo>::iterator it = m_OfflineCoupleInfoTable.find( nPairID );
	if ( it != m_OfflineCoupleInfoTable.end() )
	{
		it->second.m_nIntimacy = nIntimacy;
	}
}

void CCoupleProcessor::UpdateCoupleInfoBlessingValue( unsigned int nPairID, unsigned int nBlessingValue )
{
	std::map<unsigned int,COfflineCoupleInfo>::iterator it = m_OfflineCoupleInfoTable.find( nPairID );
	if ( it != m_OfflineCoupleInfoTable.end() )
	{
		it->second.m_nBlessingValue = nBlessingValue;
	}
}

