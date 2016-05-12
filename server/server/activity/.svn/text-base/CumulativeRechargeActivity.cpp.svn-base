#include "EntityActivityComponent.h"
#include "CumulativeRechargeActivity.h"
#include "CumulativeRechargeMgr.h"
#include "GameMsg_Activity.h"
#include "../logic/PlayerManager.h"


CCummulativeRechargeActivity::CCummulativeRechargeActivity()
{
	SetActivityType(EActivity_CumulativeRecharge);
}

CCummulativeRechargeActivity::~CCummulativeRechargeActivity()
{

}

void CCummulativeRechargeActivity::Init( time_t tBeginTime, time_t tEndTime )
{
	CTimeActivity::Init(tBeginTime, tEndTime);
}

void CCummulativeRechargeActivity::OnActivityBegin()
{
	CCumulativeRechargeMgr::Instance().SetIsOpening(true);

	std::list< CRoleEntity * > roleList;
	CPlayerManager::Instance().GetAllPlayers(roleList);
	for (std::list< CRoleEntity * >::iterator itList = roleList.begin(); itList != roleList.end(); ++itList) {
		CRoleEntity * pRoleEntity = *itList;
		if (pRoleEntity != NULL) {
			CRoleActivity *pRoleActivity = pRoleEntity->GetComponent< CRoleActivity >();
			if (pRoleActivity != NULL) {
				pRoleActivity->OnCumulativeRechargeActivityStart();
			}
		}
	}
}

void CCummulativeRechargeActivity::OnActivityEnd()
{
	CCumulativeRechargeMgr::Instance().SetIsOpening(false);
	//std::list< CRoleEntity * > roleList;
	//CPlayerManager::Instance().GetAllPlayers(roleList);
	//for (std::list< CRoleEntity * >::iterator itList = roleList.begin(); itList != roleList.end(); ++itList) {
	//	CRoleEntity * pRoleEntity = *itList;
	//	if (pRoleEntity != NULL) {
	//		CRoleActivity *pRoleActivity = pRoleEntity->GetComponent< CRoleActivity >();
	//		if (pRoleActivity != NULL) {
	//			pRoleActivity->OnCumulativeRechargeActivityEnd();
	//		}
	//	}
	//}
}


