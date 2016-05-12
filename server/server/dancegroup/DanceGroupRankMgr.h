#ifndef	__DANCE_GROUP_MGR_H__
#define __DANCE_GROUP_MGR_H__

#include "../Rank/RankMgr.h"

class CDanceGroupInfo;

class CDanceGroupRankInfo : public CRankInfo
{
public:
	CDanceGroupRankInfo(CDanceGroupInfo & rDGInfo);
	virtual ~CDanceGroupRankInfo();

	CDanceGroupInfo * m_pDanceGroupInfo;
};

//ŒËÕ≈π‹¿Ì∆˜
class CDanceGroupRankMgr : public CRankMgr
{
public:
	CDanceGroupRankMgr();
	virtual ~CDanceGroupRankMgr();
};

#endif


