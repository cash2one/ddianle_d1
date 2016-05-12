#include "ExtraActivityInfoMgr.h"

CExtraActivityInfoMgr::CExtraActivityInfoMgr()
{
	m_nAmuseSceneID = 0;
}

CExtraActivityInfoMgr::~CExtraActivityInfoMgr()
{

}

CExtraActivityInfoMgr & CExtraActivityInfoMgr::Instance()
{
	static CExtraActivityInfoMgr m;
	return m;
}

int CExtraActivityInfoMgr::GetAmuseSceneID()
{
	return m_nAmuseSceneID;
}

void CExtraActivityInfoMgr::SetAmuseSceneID(int nID)
{
	m_nAmuseSceneID = nID;
}

