#include "ExtraExpActivity.h"
#include "../util/CommonFunc.h"
#include "../../socket/Log.h"
#include "GlobalActivityData.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "GameMsg_S2C_UpdateExtraExpActivityState.h"

extern void BroadcastMsg(LPGameMsg_Base pMsg);

CExtraExpActivity::CExtraExpActivity()
{
	m_nActivityID = -1;
	m_nRewardExpRate = 0;
}
CExtraExpActivity::~CExtraExpActivity()
{
}
//初始化
void CExtraExpActivity::Init(int nActivityID, const char * szBeginTime, const char * szEndTime,
							 const char * szStartTime, const char * szStopTime, unsigned short nReewardExpRate)
{
	int nStartHour = 0;
	int nStartMinute = 0;
	sscanf(szStartTime, "%2d:%2d", &nStartHour, &nStartMinute);
	int nStopHour = 0;
	int nStopMinute = 0;
	sscanf(szStopTime, "%2d:%2d", &nStopHour, &nStopMinute);

	m_nActivityID = nActivityID;
	m_nRewardExpRate = nReewardExpRate;
	CDayTimeBlockActivity::Init(nStartHour, nStartMinute, nStopHour, nStopMinute, SetTime(szBeginTime), SetTime(szEndTime));
}
//活动开始运营
void CExtraExpActivity::OnStart()
{
	WriteLog(LOGLEVEL_DEBUG, "Debug: CExtraExpActivity::OnStart. ActivityID(%d)", m_nActivityID);
	CGlobalActivityData::Instance().IncreaseExtraExpRate(m_nRewardExpRate);
	GameMsg_S2C_UpdateExtraExpActivityState updateStateMsg(m_nActivityID, true, m_nStartHour, m_nStartMinute, m_nStopHour, m_nStopMinute, m_nRewardExpRate);
	BroadcastMsg(&updateStateMsg);
}
//活动结束运营
void CExtraExpActivity::OnStop()
{
	WriteLog(LOGLEVEL_DEBUG, "Debug: CExtraExpActivity::OnStop. ActivityID(%d)", m_nActivityID);
	CGlobalActivityData::Instance().DecreaseExtraExpRate(m_nRewardExpRate);
	GameMsg_S2C_UpdateExtraExpActivityState updateStateMsg(m_nActivityID, false, m_nStartHour, m_nStartMinute, m_nStopHour, m_nStopMinute, m_nRewardExpRate);
	BroadcastMsg(&updateStateMsg);
}
//发送活动状态到客户端
void CExtraExpActivity::UpdateStateToClient(CEntityNetComponent & rNetCom)
{
	if (IsRunning())
	{
		GameMsg_S2C_UpdateExtraExpActivityState updateStateMsg(m_nActivityID, true, m_nStartHour, m_nStartMinute, m_nStopHour, m_nStopMinute, m_nRewardExpRate);
		rNetCom.SendPlayerMsg(&updateStateMsg);
	}
}

