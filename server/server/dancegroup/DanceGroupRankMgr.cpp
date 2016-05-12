#include "DanceGroupRankMgr.h"
#include "../../datastructure/DataStruct_DanceGroup.h"

CDanceGroupRankInfo::CDanceGroupRankInfo(CDanceGroupInfo & rDGInfo)
: CRankInfo(rDGInfo.m_DGBaseInfo.m_nGroupID, rDGInfo.m_DGBaseInfo.m_nHonor)
, m_pDanceGroupInfo(&rDGInfo)
{

}
CDanceGroupRankInfo::~CDanceGroupRankInfo()
{

}

CDanceGroupRankMgr::CDanceGroupRankMgr()
{
}

CDanceGroupRankMgr::~CDanceGroupRankMgr()
{
}

