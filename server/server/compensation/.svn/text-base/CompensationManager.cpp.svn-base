#include "CompensationManager.h"

CompensationMgr::CompensationMgr()
{

}

CompensationMgr::~CompensationMgr()
{

}

CompensationMgr& CompensationMgr::Instance()
{
	static	CompensationMgr compensationMgr;
	return compensationMgr;
}

bool CompensationMgr::InsertCompensation(CompensationInfo& cInfo)
{
	if (cInfo.m_nID != INVALID_COMPESSATION_ID && m_CompensationMap.find(cInfo.m_nID) == m_CompensationMap.end())
	{
		m_CompensationMap.insert(make_pair(cInfo.m_nID,cInfo));
		return true;
	}

	return false;
}

void CompensationMgr::ClearCompensation()
{
	m_CompensationMap.clear();
}

