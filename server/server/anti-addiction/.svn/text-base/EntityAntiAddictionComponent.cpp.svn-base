#include "EntityAntiAddictionComponent.h"
#include "../logic/EntityTimerComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../logic/EntityComponentEventID.h"
#include "GameMsg_S2C_AntiAddiction.h"
#include "../player/GameMsg_C2S_RealNameAuth.h"
#include "../../socket/GameMsg_Map.h"

CEntityAntiAddictonComponent::CEntityAntiAddictonComponent():
CEntityComponent(true, false)
{
	m_pRoleNet = NULL;
	m_pRoleTimer = NULL;
	m_pCurConfigItem = NULL;
	m_nConfigItemIndex = -1;
	m_nAddictionTime = 0;
	m_nAddictionOfflineTime = 0;
	m_nTipCount = 0;
	m_nPID = 0;
	m_bAdult = true;
	m_bActive = CheckActive();
	m_bAddictionLogin = false;
}

CEntityAntiAddictonComponent::~CEntityAntiAddictonComponent()
{

}

void CEntityAntiAddictonComponent::Start()
{
	m_pRoleNet = GetComponent<CRoleNet>();
	m_pRoleTimer = GetComponent<CRoleTimer>();
}

bool CEntityAntiAddictonComponent::CreateFromDB(ROLEINFO_DB* pRoleInfoDB)
{
	CRoleEntity* pRoleEntity = (CRoleEntity*)(Entity());
	m_nPID = pRoleEntity->PID();
	m_bActive = CheckActive();

	int nIndex = 0;
	AntiAddictionConfigItem* pConfigItem = CAntiAddictionDataMgr::Instance().FindConfig(m_nPID, pRoleInfoDB->m_nAddictionTime/60, nIndex);
	if (pConfigItem != NULL)
	{
		m_bAddictionLogin = true;
		unsigned int nNowTime = (unsigned int)time(NULL);
		unsigned int nOfflineTime = nNowTime - pRoleInfoDB->m_nLastLogoutTime;
		pRoleInfoDB->m_nAddictionOfflineTime += nOfflineTime;
		if (pRoleInfoDB->m_nAddictionOfflineTime > (unsigned int)CAntiAddictionDataMgr::Instance().GetOfflineMinutes()*60)
		{
			pRoleInfoDB->m_nAddictionTime = 0;
			pRoleInfoDB->m_nAddictionOfflineTime = 0;
		}
	}
	else
	{
		pRoleInfoDB->m_nAddictionOfflineTime = 0;
	}
	m_nAddictionTime = pRoleInfoDB->m_nAddictionTime;
	m_nAddictionOfflineTime = pRoleInfoDB->m_nAddictionOfflineTime;
	return true;
}

bool CEntityAntiAddictonComponent::PacketToDB( ROLEINFO_DB *pRoleInforDB ) const
{
	if (m_bActive)
	{
		unsigned int nNowTime = (unsigned int)time(NULL);
		pRoleInforDB->m_nAddictionTime = m_nAddictionTime + (nNowTime - pRoleInforDB->m_nLastLoginTime);
		pRoleInforDB->m_nAddictionOfflineTime = m_nAddictionOfflineTime;
		//登陆时未沉迷登出时处于沉迷则开始累计下线时间
		if (!m_bAddictionLogin && m_pCurConfigItem != NULL)
		{
			pRoleInforDB->m_nAddictionOfflineTime = 0;
		}
	}
	else
	{
		pRoleInforDB->m_nAddictionTime = 0;
		pRoleInforDB->m_nAddictionOfflineTime = 0;
	}
	return true;
}

void CEntityAntiAddictonComponent::OnLogin()
{
	if (m_bActive)
	{
		unsigned int nNowTime = (unsigned int)time(NULL);
		CreateNextTimer(nNowTime, m_nAddictionTime);
	}
}
void CEntityAntiAddictonComponent::OnLogout()
{
}

void CEntityAntiAddictonComponent::OnEvent(CComponentEvent& refEvent)
{
	if (!m_bActive)
	{
		return;
	}
	if (refEvent.nID == eComponentEvent_AntiAddiction)
	{
		unsigned int nNowTime = (unsigned int)time(NULL);
		unsigned short nOnlineTime = *(unsigned short*)(refEvent.m_pData);
		CreateNextTimer(nNowTime, nOnlineTime*60);
	}
	else if (refEvent.nID == eComponentEvent_AntiAddictionTip)
	{
		if (m_pCurConfigItem != NULL)
		{
			SendTipMsg(m_pCurConfigItem->tipText);
		}
		unsigned int nNowTime = (unsigned int)time(NULL);
		if (m_nTipCount > 0)
		{
			CreateNextTipTimer(nNowTime);
			--m_nTipCount;
		}
	}
}

void CEntityAntiAddictonComponent::SerializeComponent(CParamPool &IOBuff)
{
}

void CEntityAntiAddictonComponent::OnUpdate( const unsigned long &nTimeElapsed )
{

}

void CEntityAntiAddictonComponent::SendExtraStaticData()
{

}

AntiAddictionConfigItem* CEntityAntiAddictonComponent::GetConfigItem()
{
	return m_pCurConfigItem;
}

void CEntityAntiAddictonComponent::SendTipMsg( const string& strText )
{
	GameMsg_S2C_AntiAddictionTip msg;
	msg.m_strTipText = strText;
	SendPlayerMsg(&msg);
}

bool CEntityAntiAddictonComponent::CreateNextTimer( unsigned int nNowTime, unsigned int nOnlineTime )
{
	if (!m_bActive)
	{
		return false;
	}
	int nIndex = 0;
	AntiAddictionConfigItem* pConfigItem = CAntiAddictionDataMgr::Instance().FindConfig(m_nPID, nOnlineTime/60, nIndex);
	if (pConfigItem != NULL && m_pCurConfigItem != pConfigItem)
	{
		m_pCurConfigItem = pConfigItem;
		m_nConfigItemIndex = nIndex;

		if (m_pCurConfigItem != NULL)
		{
			SendTipMsg(m_pCurConfigItem->tipText);
			m_nTipCount = (m_pCurConfigItem->timeRange.second - m_pCurConfigItem->timeRange.first)/m_pCurConfigItem->tipDiffMinutes;
			CreateNextTipTimer(nNowTime);
		}
	}
	AntiAddictionConfigItem* pNextConfigItem = CAntiAddictionDataMgr::Instance().FindConfig(m_nPID, m_nConfigItemIndex+1);
	if (pNextConfigItem != NULL)
	{
		unsigned short nNextTime = (unsigned short)(pNextConfigItem->timeRange.first);
		int nDiffSeconds = nNextTime*60 - nOnlineTime;
		m_pRoleTimer->AddTimer(nNowTime + nDiffSeconds, eComponentEvent_AntiAddiction, nNextTime);
	}
	return false;
}

void CEntityAntiAddictonComponent::CreateNextTipTimer( unsigned int nNowTime )
{
	if (!m_bActive)
	{
		return;
	}
	if (m_pCurConfigItem)
	{
		m_pRoleTimer->AddTimer(nNowTime + m_pCurConfigItem->tipDiffMinutes*60, eComponentEvent_AntiAddictionTip, 0);
	}
}

void CEntityAntiAddictonComponent::RegComponentNetMsgMap()
{
	//实名认证
	RegMsgCreatorAndProcessor(GameMsg_C2S_SynAdult, &CEntityAntiAddictonComponent::OnSynAdult);
}

void CEntityAntiAddictonComponent::OnSynAdult( GameMsg_Base & Msg ,CSlotPeer & SlotPeer )
{
	GameMsg_C2S_SynAdult* pMsg = (GameMsg_C2S_SynAdult*)&Msg;
	WriteLog(LOGLEVEL_DEBUG, "nAccount=%u OnSynAdult m_nAdult=%d", pMsg->nAccount, (int)pMsg->m_nAdult);
	SetAdult(pMsg->m_nAdult!=0);
}

int CEntityAntiAddictonComponent::GetAddicEffectByExpCause( CRoleExpLog::EAddExpCause eCause )
{
	static std::map<int, int> mapCause2Effect;
	if (mapCause2Effect.empty())
	{
		mapCause2Effect[CRoleExpLog::EAddExpCause_Quest] = eAddicEffect_QuestExp;
		mapCause2Effect[CRoleExpLog::EAddExpCause_Match] = eAddicEffect_DanceExp;
		mapCause2Effect[CRoleExpLog::EAddExpCause_AmuseRoom] = eAddicEffect_AmuseExp;
	}
	std::map<int, int>::iterator it = mapCause2Effect.find(eCause);
	if (it != mapCause2Effect.end())
	{
		return it->second;
	}
	else
	{
		return -1;
	}
}

int CEntityAntiAddictonComponent::GetAddicEffectByMoneyCause( EChangeMoneyCause eCause )
{
	static std::map<int, int> mapCause2Effect;
	if (mapCause2Effect.empty())
	{
		mapCause2Effect[EChangeMoneyCause_Add_Quest] = eAddicEffect_QuestGoldTick;
	}
	std::map<int, int>::iterator it = mapCause2Effect.find(eCause);
	if (it != mapCause2Effect.end())
	{
		return it->second;
	}
	else
	{
		return -1;
	}
}

bool CEntityAntiAddictonComponent::IsAdult() const
{
	return m_bAdult;
}

void CEntityAntiAddictonComponent::SetAdult( bool val )
{
	if (m_bAdult != val)
	{
		m_bAdult = val;
		WriteLog(LOGLEVEL_DEBUG, "RoleID=%u SetAdult to %d", Entity()->GetEntityID(), (int)m_bAdult);
		bool bLastActive = m_bActive;
		m_bActive = CheckActive();

		//组件激活状态变更
		if (bLastActive != m_bActive)
		{
			if (!m_bActive)//激活转换为未激活
			{
				m_nConfigItemIndex = -1;
				m_pCurConfigItem = NULL;
				m_nAddictionTime = 0;
				m_nTipCount = 0;
			}
			else//未激活转换为激活
			{
				unsigned int nNowTime = (unsigned int)time(NULL);
				CreateNextTimer(nNowTime, m_nAddictionTime);
			}
		}
	}
}

void CEntityAntiAddictonComponent::SendPlayerMsg(GameMsg_Base *pMsg)
{
	if (m_pRoleNet != NULL && pMsg != NULL	)
	{
		m_pRoleNet->SendPlayerMsg(pMsg);
	}
}

bool CEntityAntiAddictonComponent::IsActive() const
{
	return m_bActive;
}

bool CEntityAntiAddictonComponent::CheckActive()
{
	if (CAntiAddictionDataMgr::Instance().IsOpenAntiaddiction())
	{
		if (CAntiAddictionDataMgr::Instance().HasConfig(m_nPID) && !m_bAdult)
		{
			return true;
		}
	}
	return false;
}

//end file

