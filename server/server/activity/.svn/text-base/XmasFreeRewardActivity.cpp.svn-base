#include "XmasFreeRewardActivity.h"
#include "XmasFreeRewardDataMgr.h"



CXmasFreeRewardActivity::CXmasFreeRewardActivity()
{
	SetActivityType(EActivity_XmaxFreeReward);
}

CXmasFreeRewardActivity::~CXmasFreeRewardActivity()
{

}

void CXmasFreeRewardActivity::Init( time_t tBeginTime, time_t tEndTime )
{
	CTimeActivity::Init(tBeginTime, tEndTime);
}

void CXmasFreeRewardActivity::OnActivityBegin()
{
	CXmasFreeRewardDataMgr::Instance().SetIsOpening(true);
}

void CXmasFreeRewardActivity::OnActivityEnd()
{
	CXmasFreeRewardDataMgr::Instance().SetIsOpening(false);
}


