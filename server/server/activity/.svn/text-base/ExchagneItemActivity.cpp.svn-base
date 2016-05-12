#include "EntityActivityComponent.h"
#include "ExchangeItemActivity.h"
#include "ExchangeItemInfoMgr.h"
#include "GameMsg_Activity.h"
#include "../logic/PlayerManager.h"
#include "../../datastructure/Localization.h"
#include "../chat/GameMsg_S2C_Chat.h"


extern void BroadcastMsg(LPGameMsg_Base pMsg);


CExchangeItemActivity::CExchangeItemActivity()
{
	SetActivityType(EActivity_ExchangeItem);
}

CExchangeItemActivity::~CExchangeItemActivity()
{

}

void CExchangeItemActivity::Init( time_t tBeginTime, time_t tEndTime )
{
	CTimeActivity::Init(tBeginTime, tEndTime);
}

void CExchangeItemActivity::OnActivityBegin()
{
	CExchangeItemInfoMgr::Instance().SetIsOpening(true);
	//if (CExchangeItemInfoMgr::Instance().ActivityStartAnnouce().size() > 0) {
	//	GameMsg_S2C_Chat chatAdmine(CLocalization::Instance().GetString("AdminAnnounce").c_str(),0, eChatAdmin, CHATCOLOR_ADMINCHANNEL, 
	//		CExchangeItemInfoMgr::Instance().ActivityStartAnnouce().c_str(), 0, EChatExtraType_Default); 

	//	BroadcastMsg(&chatAdmine);
	//}

	//std::list< CRoleEntity * > roleList;
	//CPlayerManager::Instance().GetAllPlayers(roleList);
	//for (std::list< CRoleEntity * >::iterator itList = roleList.begin(); itList != roleList.end(); ++itList) {
	//	CRoleEntity * pRoleEntity = *itList;
	//	if (pRoleEntity != NULL) {
	//		CRoleActivity *pRoleActivity = pRoleEntity->GetComponent< CRoleActivity >();
	//		if (pRoleActivity != NULL) {
	//			//pRoleActivity->();
	//		}
	//	}
	//}
}

void CExchangeItemActivity::OnActivityEnd()
{
	//if (CExchangeItemInfoMgr::Instance().ActivityEndAnnouce().size() > 0) {
	//	GameMsg_S2C_Chat chatAdmine(CLocalization::Instance().GetString("AdminAnnounce").c_str(),0, eChatAdmin, CHATCOLOR_ADMINCHANNEL, 
	//							CExchangeItemInfoMgr::Instance().ActivityEndAnnouce().c_str(), 0, EChatExtraType_Default); 
	//	BroadcastMsg(&chatAdmine);
	//}

	CExchangeItemInfoMgr::Instance().SetIsOpening(false);
}


