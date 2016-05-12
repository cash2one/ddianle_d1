#include "GiftCodeActivityShow.h"
#include "GiftCodeInfoMgr.h"
#include "../../datastructure/Macro_Define.h"

CGiftCodeActivityShow::CGiftCodeActivityShow()
{
	SetActivityType(EActivity_GiftCodeAcitivity);
}

CGiftCodeActivityShow::~CGiftCodeActivityShow()
{

}

void CGiftCodeActivityShow::Init( time_t tBeginTime, time_t tEndTime )
{
	CTimeActivity::Init(tBeginTime, tEndTime);
}

void CGiftCodeActivityShow::OnActivityBegin()
{
	CGiftCodeInfoMgr::Instance().SetIsShowing(true);
}

void CGiftCodeActivityShow::OnActivityEnd()
{
	CGiftCodeInfoMgr::Instance().SetIsShowing(false);
}

