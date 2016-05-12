#include "InvitationActivity.h"
#include "InvitationActivityMgr.h"

CInvitationActivity::CInvitationActivity()
{
	SetActivityType(EActivity_Invitation);
}

CInvitationActivity::~CInvitationActivity()
{
}

void CInvitationActivity::Init(time_t tBeginTime, time_t tEndTime)
{
	CTimeActivity::Init(tBeginTime, tEndTime);
}

void CInvitationActivity::OnActivityBegin()
{
	CInvitationActivityMgr::Instance().SetIsOpening();
}

void CInvitationActivity::OnActivityEnd()
{
	CInvitationActivityMgr::Instance().SetIsOpening(false);
}

