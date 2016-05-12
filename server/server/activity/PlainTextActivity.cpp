#include "PlainTextActivity.h"
#include "PlainTextActivityInfoMgr.h"

CPlainTextActivityShow::CPlainTextActivityShow()
{
	SetActivityType(EActivity_PlainTextAcitivity);
}

CPlainTextActivityShow::~CPlainTextActivityShow()
{

}

void CPlainTextActivityShow::Init( time_t tBeginTime, time_t tEndTime )
{
	CTimeActivity::Init(tBeginTime, tEndTime);
}

void CPlainTextActivityShow::OnActivityBegin()
{
	CPlainTextActivityInfoMgr::Instance().SetIsOpening(true);
}

void CPlainTextActivityShow::OnActivityEnd()
{
	CPlainTextActivityInfoMgr::Instance().SetIsOpening(false);
}

