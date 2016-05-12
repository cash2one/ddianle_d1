#include "../../socket/ParamPool.h"
#include "AllActivitiesConfigMgr.h"


CAllActivitiesConfigMgr::CAllActivitiesConfigMgr()
:m_nXmasActivityStartTime(0)
, m_nXmasActivityEndTime(0)
{

}

CAllActivitiesConfigMgr::~CAllActivitiesConfigMgr()
{

}

CAllActivitiesConfigMgr & CAllActivitiesConfigMgr::Instance()
{
    static CAllActivitiesConfigMgr s_allActivitiesConfigMgr;

    return s_allActivitiesConfigMgr;
}

void CAllActivitiesConfigMgr::AddXmasActivityTime(unsigned int nStartTime, unsigned int nEndTime)
{
    m_nXmasActivityStartTime = nStartTime;
    m_nXmasActivityEndTime = nEndTime;
}

const char * CAllActivitiesConfigMgr::SerializeName()
{
    return "AllActivitiesConfig";
}

void CAllActivitiesConfigMgr::Serialize(CParamPool & IOBuff)
{
    unsigned int nTimeNow = (unsigned int)time(NULL);

    int nSerilizeNum = 0;
    if (nTimeNow > m_nXmasActivityStartTime && nTimeNow < m_nXmasActivityEndTime)
    {
        nSerilizeNum = 0;
    }
    else if (nTimeNow >= m_nXmasActivityEndTime)
    {
        nSerilizeNum = -1;
    }
    else
    {
        nSerilizeNum = m_nXmasActivityStartTime - nTimeNow;
    }

    IOBuff.AddInt(nSerilizeNum);
}

