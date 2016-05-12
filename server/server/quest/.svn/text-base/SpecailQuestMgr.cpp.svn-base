#include "SpecailQuestMgr.h"


CPrivateQuest::CPrivateQuest()
:m_nQuestID(0)
{

}

CPrivateQuest::~CPrivateQuest()
{

}


CSpecailQuestMgr::CSpecailQuestMgr()
{

}

CSpecailQuestMgr::~CSpecailQuestMgr()
{

}

CSpecailQuestMgr & CSpecailQuestMgr::Instance()
{
	static CSpecailQuestMgr s_SpecialQuestMgr;

	return s_SpecialQuestMgr;
}

void CSpecailQuestMgr::AddPrivateQuest( const CPrivateQuest & prviateQuest )
{
	m_mapPrivateQuest.insert(std::make_pair(prviateQuest.m_nQuestID, prviateQuest));	
}

const std::map<unsigned short, CPrivateQuest> & CSpecailQuestMgr::PrivateQuestMap() const
{
	return m_mapPrivateQuest;
}


