#include "EntityTimerComponent.h"


CEntityTimerComponent::CEntityTimerComponent():
CEntityComponent(false, false)
{
	m_nElapsedTime = 0;
}

CEntityTimerComponent::~CEntityTimerComponent()
{

}

void CEntityTimerComponent::Start()
{
}


void CEntityTimerComponent::RegComponentNetMsgMap()
{
}

void CEntityTimerComponent::OnEvent(CComponentEvent& refEvent )
{
}

void CEntityTimerComponent::SerializeComponent(CParamPool &IOBuff)
{
}

bool CEntityTimerComponent::CreateFromDB(ROLEINFO_DB* pRoleInfoDB)
{
	return true;
}
void CEntityTimerComponent::OnLogin()
{
}
void CEntityTimerComponent::OnLogout()
{
}


bool CEntityTimerComponent::AddTimer(unsigned int nEndTime,int nType,unsigned short nData)
{
	unsigned int now = (unsigned int)time(NULL);
	if (nEndTime <= now){
		return false;
	}
	TimerMap::iterator it = m_aMapTimer.find(nEndTime);
	if (it != m_aMapTimer.end()){
		std::vector<CTimerEvent> & vecTimerEvent = it->second;
		std::vector<CTimerEvent>::iterator iterVec = vecTimerEvent.begin();
		while (iterVec != vecTimerEvent.end())
		{
			CTimerEvent & aEvent = *iterVec;
			if (aEvent.nID == nType && aEvent.Data == nData){
				return false;
			}
			iterVec++;
		}

		CTimerEvent aNewEvent(nType,nData);
		vecTimerEvent.push_back(aNewEvent);
	}
	else{
		std::vector<CTimerEvent> vecTimerEvent;
		CTimerEvent aNewEvent(nType,nData);
		vecTimerEvent.push_back(aNewEvent);
		m_aMapTimer.insert(TimerMap::value_type(nEndTime,vecTimerEvent));
		m_qTimer.push(nEndTime);
	}
	return true;
}

void CEntityTimerComponent::OnUpdate(const unsigned long &nTimeElapsed)
{
	m_nElapsedTime += nTimeElapsed;
	if (m_nElapsedTime > 1000){
		m_nElapsedTime -= 1000;
		unsigned int now = (unsigned int)time(NULL);
		while (!m_qTimer.empty() && m_qTimer.top() <= now){
			SendEvent(m_qTimer.top());
			m_qTimer.pop();
		}
	}
}

void CEntityTimerComponent::SendEvent(unsigned int nEndTime)
{
	TimerMap::iterator it = m_aMapTimer.find(nEndTime);
	if (it != m_aMapTimer.end()){
		std::vector<CTimerEvent> & vecTimerEvent = it->second;
		std::vector<CTimerEvent>::iterator iterVec = vecTimerEvent.begin();
		while (iterVec != vecTimerEvent.end())
		{
			CTimerEvent & aEvent = *iterVec;
			Entity()->SendEvent(aEvent.nID,&aEvent.Data);
			iterVec++;
		}
		m_aMapTimer.erase(it);
	}
}

//end file

