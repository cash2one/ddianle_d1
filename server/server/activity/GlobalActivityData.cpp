#include "GlobalActivityData.h"

CGlobalActivityData::CGlobalActivityData()
: m_nExtraExpRate(0)
{
}

CGlobalActivityData::~CGlobalActivityData()
{
}

CGlobalActivityData & CGlobalActivityData::Instance()
{
	static CGlobalActivityData s_Instance;
	return s_Instance;
}

//增加额外经验奖励比率
void CGlobalActivityData::IncreaseExtraExpRate(unsigned short nDeltaExpRate)
{
	m_nExtraExpRate += nDeltaExpRate;
}
//减少额外经验奖励比率
void CGlobalActivityData::DecreaseExtraExpRate(unsigned short nDeltaExpRate)
{
	if (m_nExtraExpRate >= nDeltaExpRate)
	{
		m_nExtraExpRate -= nDeltaExpRate;
	}
	else
	{
		m_nExtraExpRate = 0;
	}
}

