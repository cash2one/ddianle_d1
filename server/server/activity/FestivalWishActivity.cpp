#include "FestivalWishActivity.h"
#include "FestivalWishDataMgr.h"



CFestivalWishActivity::CFestivalWishActivity()
{
	SetActivityType(EActivity_FestivalWish);
}

CFestivalWishActivity::~CFestivalWishActivity()
{

}

void CFestivalWishActivity::Init( time_t tBeginTime, time_t tEndTime )
{
	CTimeActivity::Init(tBeginTime, tEndTime);
}

void CFestivalWishActivity::OnActivityBegin()
{
	CFestivalWishDataMgr::Instance().SetIsOpening(true);
}

void CFestivalWishActivity::OnActivityEnd()
{
	CFestivalWishDataMgr::Instance().SetIsOpening(false);
}


