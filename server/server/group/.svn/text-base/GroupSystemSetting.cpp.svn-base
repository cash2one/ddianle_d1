#include "GroupSystemSetting.h"


CGroupSystemSetting::CGroupSystemSetting(void)
{
}

CGroupSystemSetting& CGroupSystemSetting::Instance()
{
    static CGroupSystemSetting s_GroupSystemSetting;

    return s_GroupSystemSetting;
}

CGroupSystemSetting::~CGroupSystemSetting(void)
{
}

void CGroupSystemSetting::SetFunctionSwitch(unsigned short nFunction, bool bOpen)
{
    m_mapFunctionSwitch[nFunction] = bOpen;
}

bool CGroupSystemSetting::IsFunctionOpen(unsigned short nFunction) const
{
    std::map<unsigned short, bool>::const_iterator itr = m_mapFunctionSwitch.find(nFunction);

    if (itr == m_mapFunctionSwitch.end() )
        return true;

    return itr->second;
}

