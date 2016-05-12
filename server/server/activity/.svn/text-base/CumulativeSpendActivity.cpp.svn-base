#include "EntityActivityComponent.h"
#include "CumulativeSpendActivity.h"
#include "CumulativeSpendMgr.h"
#include "GameMsg_Activity.h"
#include "../logic/PlayerManager.h"


CCummulativeSpendActivity::CCummulativeSpendActivity()
{
	SetActivityType(EActivity_CumulativeSpend);
}

CCummulativeSpendActivity::~CCummulativeSpendActivity()
{

}

void CCummulativeSpendActivity::Init( time_t tBeginTime, time_t tEndTime )
{
	CTimeActivity::Init(tBeginTime, tEndTime);
}

void CCummulativeSpendActivity::OnActivityBegin()
{
	CCumulativeSpendMgr::Instance().SetIsOpening(true);

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

void CCummulativeSpendActivity::OnActivityEnd()
{
	CCumulativeSpendMgr::Instance().SetIsOpening(false);
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


