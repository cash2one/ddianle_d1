#include "WordCoupleManager.h"
#include "../group/Global.h"
#include "../../socket/GameMsg_Map.h"
#include "../libServerFrame/QueryManager.h"
#include "../CoupleMsgDef.h"
#include "GameMsg_S2G_Couple.h"
#include "GameMsg_G2S_Couple.h"
//#include "../../../datastructure/Localization.h"

extern CQueryManager		*g_pQueryMgr;

CWCoupleManager::CWCoupleManager()
:GameMsg_Processor(true, true)
{

}

CWCoupleManager::~CWCoupleManager()
{

}

CWCoupleManager & CWCoupleManager::Instance()
{
	static CWCoupleManager s_WMarriageManager;

	return s_WMarriageManager;
}

void CWCoupleManager::Init()
{
	_RegisterMessage();
}

void CWCoupleManager::_RegisterMessage()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_AcceptCoupleRequest);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CoupleSeprate);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetMarried);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_Divorced);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_WeddingRoomStartingNofication);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateCoupleIntimacy);
	GAMEMSG_REGISTERCREATOR(GameMsg_S2G_AutoStartWeddingFail);
	
	RegMsgProcessor(MSG_S2G_AcceptCoupleRequst, &CWCoupleManager::_OnAcceptCoupleRequest);
	RegMsgProcessor(MSG_S2G_CoupleSeparate, &CWCoupleManager::_OnCoupleSeparated);
	RegMsgProcessor(MSG_S2G_GetMarried, &CWCoupleManager::_OnGetMarried);
	RegMsgProcessor(MSG_S2G_Divorce, &CWCoupleManager::_OnDivorced);
	RegMsgProcessor(MSG_S2G_WeddingRoomStartingNofication, &CWCoupleManager::_OnWeddingRoomStartNofication);
	RegMsgProcessor(MSG_S2G_UpdateCoupleIntimacy, &CWCoupleManager::_OnUpdateCoupleIntimacy);
	RegMsgProcessor(MSG_S2G_AutoStartWeddingRoomFail, &CWCoupleManager::_OnAutoStartWeddingRoomFail);
}

void CWCoupleManager::_OnAcceptCoupleRequest( GameMsg_Base &msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_AcceptCoupleRequest &msgAccept = (GameMsg_S2G_AcceptCoupleRequest &)msg;
	
	CGroupRoleEntity *pMale = CGroupPlayerManager::Instance().SearchRoleByID(msgAccept.m_nMaleID);
	if (pMale != NULL && pMale->GetSex() == SexType_Male) {
		pMale->SetPairID(msgAccept.m_nPairID);
		pMale->SetCoupleID(msgAccept.m_nFemaleID);
		pMale->SetCoupleDate(msgAccept.m_nCoupleDate);
	}

	CGroupRoleEntity *pFemale = CGroupPlayerManager::Instance().SearchRoleByID(msgAccept.m_nFemaleID);
	if (pFemale != NULL && pFemale->GetSex() == SexType_Female) {
		pFemale->SetPairID(msgAccept.m_nPairID);
		pFemale->SetCoupleID(msgAccept.m_nMaleID);	
		pFemale->SetCoupleDate(msgAccept.m_nCoupleDate);
	}
}

void CWCoupleManager::_OnCoupleSeparated(GameMsg_Base & msg, CSlotPeer & slotPeer)
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
}

void CWCoupleManager::_OnGetMarried(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_GetMarried msgGetMarried = (GameMsg_S2G_GetMarried &)msg;	

	CGroupRoleEntity *pMarried = CGroupPlayerManager::Instance().SearchRoleByID(msgGetMarried.m_nMarrieID);		
	if (pMarried != NULL) {
		pMarried->SetMarriageDate(msgGetMarried.m_nMarriageDate);
	}

	CGroupRoleEntity *pBeMarried = CGroupPlayerManager::Instance().SearchRoleByID(msgGetMarried.m_nBeMarriedID);		
	if (pBeMarried != NULL) {
		pBeMarried->SetMarriageDate(msgGetMarried.m_nMarriageDate);
	}
}

void CWCoupleManager::_OnDivorced(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_Divorced msgDivorced = (GameMsg_S2G_Divorced &)msg;

	CGroupRoleEntity *pDivorce = CGroupPlayerManager::Instance().SearchRoleByID(msgDivorced.m_nDivorceID);		
	if (pDivorce != NULL) {
		pDivorce->ResetCoupleInfo();
	}

	CGroupRoleEntity *pBeDivorced = CGroupPlayerManager::Instance().SearchRoleByID(msgDivorced.m_nBeDivorcedID);		
	if (pBeDivorced != NULL) {
		pBeDivorced->ResetCoupleInfo();
		if (msgDivorced.m_bNeedNotify) {
			GameMsg_G2S_NotifyHasBeenDivorced msgNotify;
			msgNotify.m_nDivorcedBy = msgDivorced.m_nDivorceID;
			if (pDivorce != NULL) {
				msgNotify.m_strDivorcedBy = pDivorce->GetName();
			}
			pBeDivorced->SendPlayerMsg(&msgNotify);
		}
	}
}

void CWCoupleManager::_OnWeddingRoomStartNofication( GameMsg_Base & msg, CSlotPeer & slotPeer )
{
	GameMsg_S2G_WeddingRoomStartingNofication & msgReceived = (GameMsg_S2G_WeddingRoomStartingNofication & )msg;

	GameMsg_G2S_WeddingRoomStartingNofication msgNotification;
	msgNotification.m_nLine = msgReceived.m_nLine;
	msgNotification.m_strLineName = msgReceived.m_strLineName;
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

void CWCoupleManager::_OnAutoStartWeddingRoomFail(GameMsg_Base & msg, CSlotPeer & slotPeer)
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

void CWCoupleManager::_OnUpdateCoupleIntimacy(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_UpdateCoupleIntimacy & msgFrom = (GameMsg_S2G_UpdateCoupleIntimacy &)msg;

	CGroupRoleEntity *pTrigger =  CGroupPlayerManager::Instance().SearchRoleByID(msgFrom.m_nTriggerID);
	if (pTrigger != NULL) {
		pTrigger->SetCoupleIntimacy(msgFrom.m_nChangedValue);
	}

	CGroupRoleEntity *pCouple =  CGroupPlayerManager::Instance().SearchRoleByID(msgFrom.m_nCoupleID);
	if (pCouple != NULL) {
		pCouple->SetCoupleIntimacy(msgFrom.m_nChangedValue);
		GameMsg_G2S_UpdateCoupleIntimacy msgToGame;
		msgToGame.m_nChangedValue = msgFrom.m_nChangedValue;
		pCouple->SendPlayerMsg(&msgToGame);
	}
}



