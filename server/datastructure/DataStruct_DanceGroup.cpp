#include "DataStruct_DanceGroup.h"
#include "../socket/ParamPool.h"
#include "../socket/Log.h"
#include "DanceGroupDataMgr.h"
#include "DanceGroupDataMgr.h"
#include "../server/util/CommonFunc.h"
#include "../server/util/Split.h"
#include "datainfor/ServerConfig.h"

//根据等级获取舞团最大人数上限
unsigned int g_nMinContributionForDemise = 0;		//舞团团长传位时新团长需要的最小贡献度, 这里的默认值是0表示GroupServer不再对这个进行判断
unsigned int GetDanceGroupMaxMemberCount(unsigned int nLevel)
{
    return g_ServerConfig.m_nMaxDanceGroupBaseMemberCount + g_ServerConfig.m_nMaxDanceGroupLevelUpAddition * ( nLevel - 1 );
}


CChangeDanceGroupNameQuery::CChangeDanceGroupNameQuery()
    : m_nGroupID(0)
    , m_strNewName("")
    , m_nChangeTime(0)
{
}

CDanceGroupMember::CDanceGroupMember()
{
	m_nTitle = 0;
	m_nRoleID = 0;
	m_strRoleName = "";
	m_nSex = 0;
	m_nLevel = 0;
	m_nState = 0;
	m_nContribution = 0;
	m_nTotalContribution = 0;
	m_bIsVIP = false;
	m_nVIPLevel = 0;
	m_nNormalCount = 0;
	m_nMoneyCount = 0;
	m_nPointCount = 0;
	m_strPacketUnlockList = "";
	m_nLastProcessTime = 0;
	m_nProperty = 0; //
}
CDanceGroupMember::~CDanceGroupMember()
{
}

void CDanceGroupMember::doEncodeToClient(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nTitle);
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddStringW(m_strRoleName.c_str());
	IOBuff.AddUChar(m_nSex);
	IOBuff.AddUShort(m_nLevel);
	IOBuff.AddUChar(m_nState);
	IOBuff.AddUInt(m_nContribution);
	IOBuff.AddUInt( m_nTotalContribution );
	int nDeltaTime = (unsigned int)time(NULL) - m_nLastOffLineTime;
	nDeltaTime = nDeltaTime > 0 ? nDeltaTime : 0;
	IOBuff.AddUInt(nDeltaTime);
	IOBuff.AddBool(m_bIsVIP);
	IOBuff.AddUShort(m_nVIPLevel);
	IOBuff.AddUChar(m_nProperty);
}
void CDanceGroupMember::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nTitle);
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddStringW(m_strRoleName.c_str());
	IOBuff.AddUChar(m_nSex);
	IOBuff.AddUShort(m_nLevel);
	IOBuff.AddUChar(m_nState);
	IOBuff.AddUInt(m_nContribution);
	IOBuff.AddUInt( m_nTotalContribution );
	IOBuff.AddUInt(m_nLastOffLineTime);
	IOBuff.AddBool(m_bIsVIP);
	IOBuff.AddUShort(m_nVIPLevel);
	IOBuff.AddUInt( m_nInTime );
	IOBuff.AddUInt( m_nLastProcessTime );
	IOBuff.AddUChar(m_nProperty);

	//
	IOBuff.AddUShort( m_nNormalCount );
	IOBuff.AddUShort( m_nMoneyCount );
	IOBuff.AddUShort( m_nPointCount );
	IOBuff.AddStringW( m_strPacketUnlockList.c_str() );

	unsigned short nLen = (unsigned short)m_mShopExchangeCount.size();
	IOBuff.AddUShort( nLen );
	std::map<unsigned short, unsigned short>::iterator itor = m_mShopExchangeCount.begin(), itorEnd = m_mShopExchangeCount.end();
	for( ; itor != itorEnd; ++itor )
	{
		IOBuff.AddUShort( itor->first );
		IOBuff.AddUShort( itor->second );
	}	

}
void CDanceGroupMember::doDecode(CParamPool &IOBuff)
{
	m_nTitle = IOBuff.GetUChar();
	m_nRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strRoleName);
	m_nSex = IOBuff.GetUChar();
	m_nLevel = IOBuff.GetUShort();
	m_nState = IOBuff.GetUChar();
	m_nContribution = IOBuff.GetUInt();
	m_nTotalContribution = IOBuff.GetUInt();
	m_nLastOffLineTime = IOBuff.GetUInt();
	m_bIsVIP = IOBuff.GetBool();
	m_nVIPLevel = IOBuff.GetUShort();
	m_nInTime = IOBuff.GetUInt();
	m_nLastProcessTime = IOBuff.GetUInt();
	m_nProperty = IOBuff.GetUChar(); 
	//
	m_nNormalCount = IOBuff.GetUShort();
	m_nMoneyCount = IOBuff.GetUShort();
	m_nPointCount = IOBuff.GetUShort();
	IOBuff.GetStringW( m_strPacketUnlockList );

	unsigned short nLen = IOBuff.GetUShort();
	for( unsigned short i = 0;i < nLen; ++i )
	{
		unsigned short nIndex= IOBuff.GetUShort();
		unsigned short nCount = IOBuff.GetUShort();
		m_mShopExchangeCount.insert( make_pair( nIndex, nCount ) );
	}
}

void CDanceGroupMember::ChangeContribution( int nValue )
{
	m_nContribution += nValue;
	m_nContribution = MIN(m_nContribution, cMAX_ROLE_CONTRIBUTION);
	if( nValue > 0 )
	{
		m_nTotalContribution += nValue;
		m_nTotalContribution = MIN(m_nTotalContribution, cMAX_ROLE_CONTRIBUTION);
	}
}//end

//////////////////////////////////////////////////////////////////////////

CRequestDanceGroupInfo::CRequestDanceGroupInfo()
    : m_nRoleID(0)
    , m_strRoleName("")
    , m_nSex(0)
    , m_nLevel(0)
    , m_nExpiredTime(0)
    , m_bIsVIP(false)
    , m_nVIPLevel(0)
{
}

CRequestDanceGroupInfo::CRequestDanceGroupInfo(unsigned int nRoleID, std::string strRoleName, unsigned char nSex, unsigned short nLevel, unsigned int nExpiredTime, bool bIsVIP, unsigned short nVIPLevel)
    : m_nRoleID(nRoleID)
    , m_strRoleName(strRoleName)
    , m_nSex(nSex)
    , m_nLevel(nLevel)
    , m_nExpiredTime(nExpiredTime)
    , m_bIsVIP(bIsVIP)
    , m_nVIPLevel(nVIPLevel)
{
}

void CRequestDanceGroupInfo::doEncodeToClient(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName.c_str());
    IOBuff.AddUChar(m_nSex);
    IOBuff.AddUShort(m_nLevel);
    int nDeltaTime = m_nExpiredTime - (unsigned int)time(NULL);
    nDeltaTime = nDeltaTime > 0 ? nDeltaTime : 0;
    IOBuff.AddUInt(nDeltaTime);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
}

void CRequestDanceGroupInfo::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRoleName);
    m_nSex = IOBuff.GetUChar();
    m_nLevel = IOBuff.GetUShort();
    m_nExpiredTime = IOBuff.GetUInt();
    m_bIsVIP = IOBuff.GetBool();
    m_nVIPLevel = IOBuff.GetUShort();
}

void CRequestDanceGroupInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName.c_str());
    IOBuff.AddUChar(m_nSex);
    IOBuff.AddUShort(m_nLevel);
    IOBuff.AddUInt(m_nExpiredTime);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
}

//////////////////////////////////////////////////////////////////////////

CDanceGroupRecordsInfo::CDanceGroupRecordsInfo()
    : m_nGroupId(0)
    , m_nRoleId(0)
    , m_strContent("")
    , m_nRecordTime(0)
    , m_nDay(0)
{
}

CDanceGroupRecordsInfo::CDanceGroupRecordsInfo(unsigned int nGroupid, unsigned int nRoleId, string strContent, unsigned int nRecordTime, unsigned short nDay)
    : m_nGroupId(nGroupid)
    , m_nRoleId(nRoleId)
    , m_strContent(strContent)
    , m_nRecordTime(nRecordTime)
    , m_nDay(nDay)
{
}

const CDanceGroupRecordsInfo& CDanceGroupRecordsInfo::operator=(const CDanceGroupRecordsInfo &rRecordInfo)
{
    m_nGroupId = rRecordInfo.m_nGroupId;
    m_nRoleId = rRecordInfo.m_nRoleId;
    m_strContent = rRecordInfo.m_strContent;
    m_nRecordTime = rRecordInfo.m_nRecordTime;
    m_nDay = rRecordInfo.m_nDay;

    return *this;
}

void CDanceGroupRecordsInfo::doDecode(CParamPool &IOBuff)
{
    m_nGroupId = IOBuff.GetUInt();
    m_nRoleId = IOBuff.GetUInt();
    IOBuff.GetStringW( m_strContent );
    m_nRecordTime = IOBuff.GetUInt();
    m_nDay = IOBuff.GetUShort();
}

void CDanceGroupRecordsInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupId);
    IOBuff.AddUInt(m_nRoleId);
    IOBuff.AddStringW(m_strContent.c_str());
    IOBuff.AddUInt(m_nRecordTime);
    IOBuff.AddUShort(m_nDay);
}

//////////////////////////////////////////////////////////////////////////

CDanceGroupBaseInfo::CDanceGroupBaseInfo()
{
	m_nGroupID = 0;
	m_strGroupName = "";
	m_nLeaderRoleID = 0;
	m_strLeaderRoleName = "";
	m_nColor = 0;
	m_nBadge = 0;
	m_nResA = 0;
	m_nResB = 0;
	m_nResC = 0;
	m_nHonor = 0;
	m_strProfile = "";
	m_strAnnouncement = "";
	m_nCreateTime = 0;
	m_bCanRequestEnter = true;
	m_nLastDismissTime = 0;
	m_nLastCancelDismissTime = 0;
	m_nLastChangeGroupNameTime = 0;
	m_nTitleName1 = "";
	m_nTitleName2 = "";
	m_nTitleName3 = "";
	m_nTitleName4 = "";
	m_nTitleName5 = "";
    m_nCeremonyStamp = 0;
    m_nCeremonyTimes = 0;
    m_nChallengeActive = 0;
}
CDanceGroupBaseInfo::~CDanceGroupBaseInfo()
{
}

void CDanceGroupBaseInfo::doEncodeAllInfoToClient(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nGroupID);
	IOBuff.AddStringW(m_strGroupName.c_str());
	IOBuff.AddUInt(m_nLeaderRoleID);
	IOBuff.AddStringW(m_strLeaderRoleName.c_str());
	IOBuff.AddUShort(m_nLevel);
	IOBuff.AddUShort(m_nColor);
	IOBuff.AddUShort(m_nBadge);
	IOBuff.AddUShort( m_nEffect );
	IOBuff.AddUInt(m_nResA);
	IOBuff.AddUInt(m_nResB);
	IOBuff.AddUInt(m_nResC);
	unsigned int nResA = 0;
	unsigned int nResB = 0;
	unsigned int nResC = 0;
	CDGLevelUpRequireRes * pResInfo = CDanceGroupDataMgr::Instance().GetLevelUpRequireResBy(m_nLevel);
	if (pResInfo != NULL) {
		nResA = pResInfo->m_nResA;
		nResB = pResInfo->m_nResB;
		nResC = pResInfo->m_nResC;
	}
	IOBuff.AddUInt(nResA);
	IOBuff.AddUInt(nResB);
	IOBuff.AddUInt(nResC);
	IOBuff.AddUInt(m_nHonor);
	IOBuff.AddStringW(m_strProfile.c_str());
	IOBuff.AddStringW(m_strAnnouncement.c_str());
	IOBuff.AddUInt(m_nCreateTime);
	IOBuff.AddBool(m_bCanRequestEnter);

	unsigned int nNow = (unsigned int)time(NULL);
	int nDeltaTime = nNow - m_nLastDismissTime;
	nDeltaTime = nDeltaTime > 0 ? nDeltaTime : 0;
	IOBuff.AddUInt(nDeltaTime);
	nDeltaTime = nNow - m_nLastCancelDismissTime;
	nDeltaTime = nDeltaTime > 0 ? nDeltaTime : 0;
	IOBuff.AddUInt(nDeltaTime);

	IOBuff.AddStringW(m_nTitleName1.c_str());
	IOBuff.AddStringW(m_nTitleName2.c_str());
	IOBuff.AddStringW(m_nTitleName3.c_str());
	IOBuff.AddStringW(m_nTitleName4.c_str());
	IOBuff.AddStringW(m_nTitleName5.c_str());
	IOBuff.AddUInt(GetDanceGroupMaxMemberCount(m_nLevel));
//    m_nCeremonyStamp; // 这个不需要
    IOBuff.AddInt(m_nCeremonyTimes);
    IOBuff << m_nChallengeActive;
}

void CDanceGroupBaseInfo::doEncodeToClientData(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nGroupID);
	IOBuff.AddStringW(m_strGroupName.c_str());
	IOBuff.AddUInt(m_nLeaderRoleID);
	IOBuff.AddStringW(m_strLeaderRoleName.c_str());
	IOBuff.AddUShort(m_nLevel);
	IOBuff.AddUShort(m_nColor);
	IOBuff.AddUShort(m_nBadge);
	IOBuff.AddUShort( m_nEffect );
	IOBuff.AddUInt(m_nResA);
	IOBuff.AddUInt(m_nResB);
	IOBuff.AddUInt(m_nResC);
	unsigned int nResA = 0;
	unsigned int nResB = 0;
	unsigned int nResC = 0;
	CDGLevelUpRequireRes * pResInfo = CDanceGroupDataMgr::Instance().GetLevelUpRequireResBy(m_nLevel);
	if (pResInfo != NULL) {
		nResA = pResInfo->m_nResA;
		nResB = pResInfo->m_nResB;
		nResC = pResInfo->m_nResC;
	}
	IOBuff.AddUInt(nResA);
	IOBuff.AddUInt(nResB);
	IOBuff.AddUInt(nResC);
	IOBuff.AddUInt(m_nHonor);
	IOBuff.AddUInt(m_nCreateTime);
	IOBuff.AddBool(m_bCanRequestEnter);

	unsigned int nNow = (unsigned int)time(NULL);
	int nDeltaTime = nNow - m_nLastDismissTime;
	nDeltaTime = nDeltaTime > 0 ? nDeltaTime : 0;
	IOBuff.AddUInt(nDeltaTime);
	nDeltaTime = nNow - m_nLastCancelDismissTime;
	nDeltaTime = nDeltaTime > 0 ? nDeltaTime : 0;
	IOBuff.AddUInt(nDeltaTime);

	IOBuff.AddStringW(m_nTitleName1.c_str());
	IOBuff.AddStringW(m_nTitleName2.c_str());
	IOBuff.AddStringW(m_nTitleName3.c_str());
	IOBuff.AddStringW(m_nTitleName4.c_str());
	IOBuff.AddStringW(m_nTitleName5.c_str());
	IOBuff.AddUInt(GetDanceGroupMaxMemberCount(m_nLevel));
    //    m_nCeremonyStamp; // 这个不需要
    IOBuff.AddInt(m_nCeremonyTimes);
    IOBuff << m_nChallengeActive;
}

void CDanceGroupBaseInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nGroupID);
	IOBuff.AddStringW(m_strGroupName.c_str());
	IOBuff.AddUInt(m_nLeaderRoleID);
	IOBuff.AddStringW(m_strLeaderRoleName.c_str());
	IOBuff.AddUShort(m_nLevel);
	IOBuff.AddUShort(m_nColor);
	IOBuff.AddUShort(m_nBadge);
	IOBuff.AddUShort( m_nEffect );
	IOBuff.AddUInt(m_nResA);
	IOBuff.AddUInt(m_nResB);
	IOBuff.AddUInt(m_nResC);
	IOBuff.AddUInt(m_nHonor);
	IOBuff.AddStringW(m_strProfile.c_str());
	IOBuff.AddStringW(m_strAnnouncement.c_str());
	IOBuff.AddUInt(m_nCreateTime);
	IOBuff.AddBool(m_bCanRequestEnter);
	IOBuff.AddUInt(m_nLastDismissTime);
	IOBuff.AddUInt(m_nLastCancelDismissTime);
	IOBuff.AddUInt(m_nLastChangeGroupNameTime);
	IOBuff.AddStringW(m_nTitleName1.c_str());
	IOBuff.AddStringW(m_nTitleName2.c_str());
	IOBuff.AddStringW(m_nTitleName3.c_str());
	IOBuff.AddStringW(m_nTitleName4.c_str());
	IOBuff.AddStringW(m_nTitleName5.c_str());
	IOBuff.AddUInt(GetDanceGroupMaxMemberCount(m_nLevel));
	IOBuff.AddStringW( m_strBadgeEffectLock.c_str() );
    IOBuff.AddUInt(m_nCeremonyStamp);
    IOBuff.AddInt(m_nCeremonyTimes);
    IOBuff << m_nChallengeActive;
}

void CDanceGroupBaseInfo::doDecode(CParamPool &IOBuff)
{
	m_nGroupID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strGroupName);
	m_nLeaderRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strLeaderRoleName);
	m_nLevel = IOBuff.GetUShort();
	m_nColor = IOBuff.GetUShort();
	m_nBadge = IOBuff.GetUShort();
	m_nEffect = IOBuff.GetUShort();
	m_nResA = IOBuff.GetUInt();
	m_nResB = IOBuff.GetUInt();
	m_nResC = IOBuff.GetUInt();
	m_nHonor = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strProfile);
	IOBuff.GetStringW(m_strAnnouncement);
	m_nCreateTime = IOBuff.GetUInt();
	m_bCanRequestEnter = IOBuff.GetBool();
	m_nLastDismissTime = IOBuff.GetUInt();
	m_nLastCancelDismissTime = IOBuff.GetUInt();
	m_nLastChangeGroupNameTime = IOBuff.GetUInt();
	IOBuff.GetStringW(m_nTitleName1);
	IOBuff.GetStringW(m_nTitleName2);
	IOBuff.GetStringW(m_nTitleName3);
	IOBuff.GetStringW(m_nTitleName4);
	IOBuff.GetStringW(m_nTitleName5);
	/*unsigned int nUnUsedMaxMemberCount = */IOBuff.GetUInt();
	IOBuff.GetStringW( m_strBadgeEffectLock );
	m_nCeremonyStamp = IOBuff.GetUInt();
	m_nCeremonyTimes = IOBuff.GetInt();
    IOBuff >> m_nChallengeActive;
}

void CDanceGroupBaseInfo::ResetCeremonyStamp(time_t now)
{
    if (!IsSameDay(now, m_nCeremonyStamp))
    {
        m_nCeremonyStamp = (unsigned int)now;
        m_nCeremonyTimes = 0;
    }

    return ;
}


///////////////
void CDanceGroupActivityInfo::doEncodeToClient(CParamPool &IOBuff)
{
	IOBuff.AddUShort( m_nDayPacket );
	IOBuff.AddUShort( m_nStarLevel );
	IOBuff.AddUInt( m_nCurrentPowerValue );
}//end

void CDanceGroupActivityInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUShort( m_nDayPacket );
	IOBuff.AddUShort( m_nStarLevel );
	IOBuff.AddUInt( m_nCurrentPowerValue );
	//IOBuff.AddUInt( m_nProcessTime );
}//end

void CDanceGroupActivityInfo::doDecode(CParamPool &IOBuff)
{
	m_nDayPacket = IOBuff.GetUShort();
	m_nStarLevel = IOBuff.GetUShort();
	m_nCurrentPowerValue = IOBuff.GetUInt();
	//m_nProcessTime = IOBuff.GetUInt();
}


DanceGroupQuitInfo::DanceGroupQuitInfo()
    : m_nRoleID(0)
    , m_nDanceGroupID(0)
    , m_nQuitStamp(0)
{

}

DanceGroupQuitInfo::~DanceGroupQuitInfo()
{

}

void DanceGroupQuitInfo::Set(unsigned int nRoleID, unsigned int nDanceGroupID, unsigned int nQuitStamp)
{
    m_nRoleID = nRoleID;
    m_nDanceGroupID = nDanceGroupID;
    m_nQuitStamp = nQuitStamp;

    return ;
}

CDanceGroupInfo::CDanceGroupInfo()
{
}

CDanceGroupInfo::CDanceGroupInfo(const CDanceGroupInfo &rInfo)
{
    m_DGBaseInfo = rInfo.m_DGBaseInfo;
    m_RequestMap = rInfo.m_RequestMap;
    m_recordsMap = rInfo.m_recordsMap;
    m_DGActivityInfo = rInfo.m_DGActivityInfo;

    CDanceGroupMember *pMember = NULL;
    for (std::map<unsigned int, CDanceGroupMember *>::const_iterator bIt = rInfo.m_MemberMap.begin(), eIt = rInfo.m_MemberMap.end(); 
        bIt != eIt; ++bIt)
    {
        pMember = new CDanceGroupMember();
        *pMember = *bIt->second;

        if (NULL != pMember)
            m_MemberMap.insert(std::make_pair(pMember->m_nRoleID, pMember) );
    }
}

CDanceGroupInfo::~CDanceGroupInfo()
{
    for (std::map<unsigned int, CDanceGroupMember *>::iterator bIt = m_MemberMap.begin(), eIt = m_MemberMap.end(); 
         bIt != eIt; ++bIt)
    {
        delete bIt->second;
        bIt->second = NULL;
    }

    m_RequestMap.clear();
    m_MemberMap.clear();
    m_recordsMap.clear();
}

void CDanceGroupInfo::doEncodeAllInfoToClient(CParamPool &IOBuff)
{
	m_DGBaseInfo.doEncodeAllInfoToClient(IOBuff);

	IOBuff.AddUShort((unsigned short)m_RequestMap.size());
	for (std::map< unsigned int, CRequestDanceGroupInfo >::iterator it = m_RequestMap.begin(); it != m_RequestMap.end(); ++it)
	{
		CRequestDanceGroupInfo & rRequestInfo = it->second;
		rRequestInfo.doEncodeToClient(IOBuff);
	}

	IOBuff.AddUShort((unsigned short)m_MemberMap.size());
	for (std::map< unsigned int, CDanceGroupMember * >::iterator it = m_MemberMap.begin(); it != m_MemberMap.end(); ++it)
	{
		CDanceGroupMember * pMember = it->second;
		pMember->doEncodeToClient(IOBuff);
	}

	m_DGActivityInfo.doEncodeToClient( IOBuff );
}

void CDanceGroupInfo::doEncode(CParamPool &IOBuff)
{
	m_DGBaseInfo.doEncode(IOBuff);

	IOBuff.AddUShort((unsigned short)m_RequestMap.size());
	for (std::map< unsigned int, CRequestDanceGroupInfo >::iterator it = m_RequestMap.begin(); it != m_RequestMap.end(); ++it)
	{
		CRequestDanceGroupInfo & rRequestInfo = it->second;
		rRequestInfo.doEncode(IOBuff);
	}

	IOBuff.AddUShort((unsigned short)m_MemberMap.size());
	for (std::map< unsigned int, CDanceGroupMember * >::iterator it = m_MemberMap.begin(); it != m_MemberMap.end(); ++it)
	{
		CDanceGroupMember * pMember = it->second;
		pMember->doEncode(IOBuff);
	}

	m_DGActivityInfo.doEncode( IOBuff );

    const unsigned int kMaxSendRecordCount = 30;
    unsigned int nRecordCount = m_recordsMap.size();

    if (nRecordCount > kMaxSendRecordCount)
        nRecordCount = kMaxSendRecordCount;

    IOBuff.AddUShort((unsigned short)nRecordCount);
    for (std::map<unsigned int, CDanceGroupRecordsInfo>::iterator bIt = m_recordsMap.begin(), eIt = m_recordsMap.end(); 
         bIt != eIt && nRecordCount > 0; ++bIt, --nRecordCount)
    {
        bIt->second.doEncode(IOBuff);
    }
}

void CDanceGroupInfo::doDecode(CParamPool &IOBuff)
{
    unsigned short nCount = 0;
    unsigned short i = 0;

    m_DGBaseInfo.doDecode(IOBuff);

    nCount = IOBuff.GetUShort();
    for (i = 0; i < nCount; ++i)
    {
        CRequestDanceGroupInfo requestInfo;
        requestInfo.doDecode(IOBuff);

        m_RequestMap.insert(std::make_pair(requestInfo.m_nRoleID, requestInfo) );
    }

    nCount = IOBuff.GetUShort();
    for (i = 0; i < nCount; ++i)
    {
        CDanceGroupMember * pMember = new CDanceGroupMember();
        pMember->doDecode(IOBuff);

        m_MemberMap.insert(std::make_pair(pMember->m_nRoleID, pMember) );
    }

    m_DGActivityInfo.doDecode( IOBuff );

    nCount = IOBuff.GetUShort();
    for (i = 0; i < nCount; ++i)
    {
        CDanceGroupRecordsInfo recordsInfo;
        recordsInfo.doDecode(IOBuff);

        m_recordsMap.insert(std::make_pair(recordsInfo.m_nRecordTime, recordsInfo) );
    }
}

void CDanceGroupInfo::Upgrade()
{
	++m_DGBaseInfo.m_nLevel;
}

//更新基本信息
void CDanceGroupInfo::UpdateBaseInfo(const CDanceGroupBaseInfo & rDGBaseInfo)
{
	m_DGBaseInfo = rDGBaseInfo;
}
//添加成员信息
bool CDanceGroupInfo::AddMemberInfo(const CDanceGroupMember & rDGMemberInfo)
{
	CDanceGroupMember * pMemberInfo = GetGroupMember(rDGMemberInfo.m_nRoleID);
	if (pMemberInfo != NULL)
	{
		return false;
	}
	pMemberInfo = new CDanceGroupMember();
	*pMemberInfo = rDGMemberInfo;
	m_MemberMap[rDGMemberInfo.m_nRoleID] = pMemberInfo;
	return true;
}
//更新成员信息
bool CDanceGroupInfo::UpdateMemberInfo(const CDanceGroupMember & rDGMemberInfo)
{
	CDanceGroupMember * pMemberInfo = GetGroupMember(rDGMemberInfo.m_nRoleID);
	if (pMemberInfo == NULL)
	{
		return false;
	}
	*pMemberInfo = rDGMemberInfo;
	return true;
}
//移除成员信息
void CDanceGroupInfo::RemoveMemberInfo(unsigned int nRoleID)
{
	std::map< unsigned int, CDanceGroupMember * >::iterator it = m_MemberMap.find(nRoleID);
	if (it != m_MemberMap.end())
	{
		delete it->second; it->second = NULL;
		m_MemberMap.erase(it);
	}
}
//添加申请加入舞团信息
void CDanceGroupInfo::AddRequestInfo(const CRequestDanceGroupInfo & rNewRequestInfo)
{
	std::map< unsigned int, CRequestDanceGroupInfo >::iterator it = m_RequestMap.find(rNewRequestInfo.m_nRoleID);
	if (it != m_RequestMap.end())
	{
		CRequestDanceGroupInfo & rRequestInfo = it->second;
		rRequestInfo.m_strRoleName = rNewRequestInfo.m_strRoleName;
		rRequestInfo.m_nSex = rNewRequestInfo.m_nSex;
		rRequestInfo.m_nLevel = rNewRequestInfo.m_nLevel;
		rRequestInfo.m_nExpiredTime = rRequestInfo.m_nExpiredTime < rNewRequestInfo.m_nExpiredTime ? rNewRequestInfo.m_nExpiredTime : rRequestInfo.m_nExpiredTime;
	}
	else
	{
		m_RequestMap[rNewRequestInfo.m_nRoleID] = rNewRequestInfo;
	}
}
//移除玩家的请求信息
unsigned int CDanceGroupInfo::RemoveRequestInfo(unsigned int nRoleID)
{
    unsigned int nExpiredTime = 0;

	std::map< unsigned int, CRequestDanceGroupInfo >::iterator it = m_RequestMap.find(nRoleID);
	if (it != m_RequestMap.end())
	{
        nExpiredTime = it->second.m_nExpiredTime;
		m_RequestMap.erase(it);
	}

    return nExpiredTime;
}

void CDanceGroupInfo::ChangeDanceGroupHonor(int nChangeValue,EChangeDanceGroupHonorCause eCause, int nPara)
{
	if (nChangeValue != 0) {
		__int64 nResult = (int)m_DGBaseInfo.m_nHonor + nChangeValue;
		if (nResult > 0) {
			nResult = MIN(nResult, cMAX_DANCE_GROUP_HONOR); 
		} else {
			nResult = 0;
		} 

		m_DGBaseInfo.m_nHonor = (unsigned int)nResult;
	}
}

//获取成员信息
CDanceGroupMember * CDanceGroupInfo::GetGroupMember(unsigned int nRoleID)
{
	std::map< unsigned int, CDanceGroupMember * >::iterator it = m_MemberMap.find(nRoleID);
	if (it != m_MemberMap.end())
	{
		return it->second;
	}
	return NULL;
}

const CDanceGroupMember* CDanceGroupInfo::GetGroupMember( unsigned int nRoleID ) const
{
    std::map<unsigned int, CDanceGroupMember *>::const_iterator itr = m_MemberMap.find( nRoleID );
    if ( itr != m_MemberMap.end() )
        return itr->second;

    return NULL;
}
//获取申请玩家信息
const CRequestDanceGroupInfo & CDanceGroupInfo::GetRequestDanceGroupInfo(unsigned int nRoleID)
{
	static CRequestDanceGroupInfo s_RequestDGInfo;
	std::map< unsigned int, CRequestDanceGroupInfo >::iterator it = m_RequestMap.find(nRoleID);
	if (it != m_RequestMap.end())
	{
		return it->second;
	}
	return s_RequestDGInfo;
}
//查看职位人数
unsigned int CDanceGroupInfo::GetTitleMemberCount(EGroupTitle eTitle)
{
	unsigned int nCount = 0;
	std::map< unsigned int, CDanceGroupMember * >::iterator it = m_MemberMap.begin();
	for (; it != m_MemberMap.end(); ++it)
	{
		CDanceGroupMember * pMember = it->second;
		if (pMember && pMember->m_nTitle == eTitle)
		{
			nCount++;
		}
	}
	return nCount;
}

void CDanceGroupInfo::GetAllMember(std::list<unsigned int> &memberList)
{
	std::map< unsigned int, CDanceGroupMember * >::iterator it = m_MemberMap.begin();
	for (; it != m_MemberMap.end(); ++it)
	{
		CDanceGroupMember * pMember = it->second;
		if (pMember != NULL)
		{
			memberList.push_back(pMember->m_nRoleID);
		}
	}
}

void CDanceGroupInfo::GetManagerMember(std::list<unsigned int> &rListManager)
{
    rListManager.clear();

    for (std::map<unsigned int, CDanceGroupMember *>::const_iterator bIt = m_MemberMap.begin(), eIt = m_MemberMap.end(); 
         bIt != eIt; ++bIt)
    {
        if (NULL != bIt->second && HasAddMemberAuthority(bIt->second->m_nTitle))
        {
            rListManager.push_back(bIt->second->m_nRoleID);
        }
    }
}

//是否还存在
bool CDanceGroupInfo::IsValid(time_t tNow)
{
	return (!IsDismissed() || m_DGBaseInfo.m_nLastDismissTime + cDANCE_GROUP_DISMISS_RETENTION_TIME > (unsigned int)tNow);
}
//是否已经解散
bool CDanceGroupInfo::IsDismissed()
{
	return (m_DGBaseInfo.m_nLastDismissTime > m_DGBaseInfo.m_nLastCancelDismissTime);
}
//是否在解散CD中
bool CDanceGroupInfo::IsInDismissCD()
{
	return ((unsigned int)time(NULL) < m_DGBaseInfo.m_nLastCancelDismissTime + cDANCE_GROUP_DISMISS_CD_TIME);
}
//玩家是否已经在申请队列中
bool CDanceGroupInfo::IsInRequestQueue(unsigned int nRoleID)
{
	std::map< unsigned int, CRequestDanceGroupInfo >::iterator it = m_RequestMap.find(nRoleID);
	return (it != m_RequestMap.end());
}

CDanceGroupManager::CDanceGroupManager()
{
}

CDanceGroupManager::~CDanceGroupManager()
{
	Reset();
}

//添加舞团信息
bool CDanceGroupManager::AddDanceGroupInfo(CDanceGroupInfo * pDGInfo)
{
	if (pDGInfo == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CDanceGroupManager::AddDanceGroupInfo. pDGInfo == NULL");
		return false;
	}
	if (GetDanceGroupInfo(pDGInfo->m_DGBaseInfo.m_nGroupID) != NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CDanceGroupManager::AddDanceGroupInfo. DanceGroup is exist. GroupID(%u)", pDGInfo->m_DGBaseInfo.m_nGroupID);
		return false;
	}
	m_GroupMap[pDGInfo->m_DGBaseInfo.m_nGroupID] = pDGInfo;
	m_GroupList.push_back(pDGInfo);

    if (pDGInfo->IsDismissed())
        m_multimapRemovedGroup.insert(std::make_pair(pDGInfo->m_DGBaseInfo.m_nLastDismissTime, pDGInfo->m_DGBaseInfo.m_nGroupID));

	return true;
}

//重置所有数据
void CDanceGroupManager::Reset()
{
	{
		std::map< unsigned int, CDanceGroupInfo * >::iterator it = m_GroupMap.begin();
		for (; it != m_GroupMap.end(); ++it)
		{
			delete it->second; it->second = NULL;
		}
		m_GroupMap.clear();
	}
	m_RoleRequestGroupMap.clear();
	m_GroupList.clear();
    m_multimapRemovedGroup.clear();
    m_mapRequestTime.clear();
}
//获取舞团信息
CDanceGroupInfo * CDanceGroupManager::GetDanceGroupInfo(unsigned int nGroupID)
{
	std::map< unsigned int, CDanceGroupInfo * >::iterator it = m_GroupMap.find(nGroupID);
	if (it == m_GroupMap.end())
	{
		return NULL;
	}
	else
	{
		return it->second;
	}
}

CDanceGroupInfo * CDanceGroupManager::GetDanceGroupInfo(const std::string &strDanceGroupName)
{
    std::map< unsigned int, CDanceGroupInfo * >::iterator it = m_GroupMap.begin();

    for (; it != m_GroupMap.end(); ++it)
    {
        if (it->second->m_DGBaseInfo.m_strGroupName.compare(strDanceGroupName) == 0)
            return it->second;
    }

    return NULL;
}


std::map< unsigned int, CDanceGroupInfo * > & CDanceGroupManager::GetDanceGroupInfoList()
{
	return m_GroupMap;
}//end

//获取角色的申请加入舞团列表
const std::set< unsigned int > & CDanceGroupManager::GetRequestGroupList(unsigned int nRoleID)
{
	static std::set< unsigned int > emptyList;

	std::map< unsigned int, std::set< unsigned int > >::iterator it = m_RoleRequestGroupMap.find(nRoleID);
	if (it != m_RoleRequestGroupMap.end())
	{
		return it->second;
	}
	else
	{
		return emptyList;
	}
}

//检查申请进入舞团的条件
ERequestEnterDanceGroupResult CDanceGroupManager::CheckRequestEnterDanceGroupCond(unsigned int nRoleID, unsigned nGroupID)
{
	const std::set< unsigned int > & rRequestList = GetRequestGroupList(nRoleID);
	//玩家同时申请数限制
	if (rRequestList.size() >= cMAX_REQUEST_ENTER_DANCE_GROUP_COUNT)
	{
		return ERequestEnterDanceGroupResult_MaxRequestCount;
	}

	CDanceGroupInfo * pGroupInfo = GetDanceGroupInfo(nGroupID);
	if (pGroupInfo == NULL)
	{
		return ERequestEnterDanceGroupResult_GroupNotExist;
	}
	//舞团是否开启申请
	if (!pGroupInfo->m_DGBaseInfo.m_bCanRequestEnter)
	{
		return ERequestEnterDanceGroupResult_DisableRequest;
	}
	//是否已经向该舞团申请
	if (pGroupInfo->IsInRequestQueue(nRoleID))
	{
		return ERequestEnterDanceGroupResult_HaveRequested;
	}
	//舞团最大申请数限制
	if (pGroupInfo->m_RequestMap.size() >= cMAX_REQUEST_ENTER_DANCE_GROUP_QUEUE_NUM)
	{
		return ERequestEnterDanceGroupResult_RequestQueueFull;
	}
	return ERequestEnterDanceGroupResult_Success;
}
//检查取消申请进入舞团的条件
ECancelRequestEnterDanceGroupResult CDanceGroupManager::CheckCancelRequestEnterDanceGroupCond(unsigned int nRoleID, unsigned int nGroupID)
{
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo == NULL)
	{
		return ECancelRequestEnterDanceGroupResult_GroupNotExist;
	}
	const CRequestDanceGroupInfo & rRequestDGInfo = pDGInfo->GetRequestDanceGroupInfo(nRoleID);
	if (rRequestDGInfo.m_nRoleID != nRoleID)
	{
		return ECancelRequestEnterDanceGroupResult_NoRequestInfo;
	}
	return ECancelRequestEnterDanceGroupResult_Success;
}
//检查批复条件
EReplyRequestEnterDanceGroupResult CDanceGroupManager::CheckReplyRequestEnterDanceGroupCond(unsigned int nRoleID, unsigned int nGroupID, unsigned int nRequestRoleID)
{
	if (nGroupID == 0)
	{
		return EReplyRequestEnterDanceGroupResult_NotInGroup;
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo == NULL)
	{
		return EReplyRequestEnterDanceGroupResult_GroupNotExist;
	}
	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleID);
	if (pMember == NULL)
	{
		return EReplyRequestEnterDanceGroupResult_NotInGroupErr;
	}
	if (!HasAddMemberAuthority(pMember->m_nTitle))
	{
		return EReplyRequestEnterDanceGroupResult_NoAuthority;
	}
	if (pDGInfo->GetMemberCount() >= GetDanceGroupMaxMemberCount(pDGInfo->m_DGBaseInfo.m_nLevel))
	{
		return EReplyRequestEnterDanceGroupResult_MaxCount;
	}
	//玩家已经没有在申请队列中
	if (!pDGInfo->IsInRequestQueue(nRequestRoleID))
	{
		return EReplyRequestEnterDanceGroupResult_NoInRequestQueue;
	}
	return EReplyRequestEnterDanceGroupResult_Success;
}
//检查退出舞团条件
EExitDanceGroupResult CDanceGroupManager::CheckExitDanceGroupCond(unsigned int nRoleID, unsigned int nGroupID)
{
	if (nGroupID == 0)
	{
		return EExitDanceGroupResult_NotInGroup;
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo == NULL)
	{
		return EExitDanceGroupResult_GroupNotExist;
	}
	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleID);
	if (pMember == NULL)
	{
		return EExitDanceGroupResult_NotInGroupErr;
	}
	if (pMember->m_nTitle == EGroupTitle_Leader)
	{
		return EExitDanceGroupResult_Leader;
	}
	return EExitDanceGroupResult_Success;
}
//检查舞团传位条件
EChangeDanceGroupLeaderResult CDanceGroupManager::CheckChangeDanceGroupLeaderCond(unsigned int nLeaderID, unsigned int nGroupID, unsigned int nNewLeaderID)
{
	if (nGroupID == 0)
	{
		return EChangeDanceGroupLeaderResult_NotInGroup;
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo == NULL)
	{
		return EChangeDanceGroupLeaderResult_GroupNotExist;
	}
	CDanceGroupMember * pLeaderMember = pDGInfo->GetGroupMember(nLeaderID);
	if (pLeaderMember == NULL)
	{
		return EChangeDanceGroupLeaderResult_LeaderNotMember;
	}
	CDanceGroupMember * pNewLeaderMember = pDGInfo->GetGroupMember(nNewLeaderID);
	if (pNewLeaderMember == NULL)
	{
		return EChangeDanceGroupLeaderResult_NewLeaderNotMember;
	}
	if (pLeaderMember->m_nTitle != EGroupTitle_Leader)
	{
		return EChangeDanceGroupLeaderResult_LeaderIsNotLeader;
	}
	if (pNewLeaderMember->m_nContribution < g_nMinContributionForDemise)
	{
		return EChangeDanceGroupLeaderResult_ContributionNotEnough;
	}
    if (EMemberProperty_MengXin == pNewLeaderMember->m_nProperty)
    {
        return EChangeDanceGroupLeaderResult_CanNotMengXin;
    }
	return EChangeDanceGroupLeaderResult_Success;
}
//检查解散舞团条件
EDismissDanceGroupResult CDanceGroupManager::CheckDismissDanceGroupCond(unsigned int nRoleID, unsigned int nGroupID)
{
	if (nGroupID == 0)
	{
		return EDismissDanceGroupResult_NotInGroup;
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo == NULL)
	{
		return EDismissDanceGroupResult_GroupNotExist;
	}
	if (pDGInfo->IsDismissed())
	{
		return EDismissDanceGroupResult_IsDismissed;
	}
	if (pDGInfo->IsInDismissCD())
	{
		return EDismissDanceGroupResult_IsInDismissCD;
	}
	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleID);
	if (pMember == NULL)
	{
		return EDismissDanceGroupResult_NotGroupMember;
	}
	if (!CanDismiss(pMember->m_nTitle))
	{
		return EDismissDanceGroupResult_NotLeader;
	}
	return EDismissDanceGroupResult_Success;
}

//检查取消解散舞团条件
ECancelDismissDanceGroupResult CDanceGroupManager::CheckCancelDismissDanceGroupCond(unsigned int nRoleID, unsigned int nGroupID)
{
	if (nGroupID == 0)
	{
		return ECancelDismissDanceGroupResult_NotInGroup;
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo == NULL)
	{
		return ECancelDismissDanceGroupResult_GroupNotExist;
	}
	if (!pDGInfo->IsDismissed())
	{
		return ECancelDismissDanceGroupResult_NotDismissed;
	}
	if (!pDGInfo->IsValid(time(NULL)))
	{
		return ECancelDismissDanceGroupResult_InValid;
	}
	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleID);
	if (pMember == NULL)
	{
		return ECancelDismissDanceGroupResult_NotGroupMember;
	}
	if (!CanDismiss(pMember->m_nTitle))
	{
		return ECancelDismissDanceGroupResult_NotLeader;
	}

	return ECancelDismissDanceGroupResult_Success;
}
//检查改变舞团职位条件
EChangeDanceGroupTitleResult CDanceGroupManager::CheckChangeDanceGroupTitleCond(unsigned int nOperatorRoleID, unsigned int nGroupID, unsigned int nTargetRoleID, unsigned char nNewTitle)
{
	if (nGroupID == 0)
	{
		return EChangeDanceGroupTitleResult_NotInGroup;
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo == NULL)
	{
		return EChangeDanceGroupTitleResult_GroupNotExist;
	}
	CDanceGroupMember * pOperatorMember = pDGInfo->GetGroupMember(nOperatorRoleID);
	if (pOperatorMember == NULL)
	{
		return EChangeDanceGroupTitleResult_NotGroupMember;	//操作者不是舞团成员
	}
	CDanceGroupMember * pTargeMember = pDGInfo->GetGroupMember(nTargetRoleID);
	if (pTargeMember == NULL)
	{
		return EChangeDanceGroupTitleResult_TargetNotGroupMember;	//操作对象不是舞团成员
	}
	if (!CanChangeTitle(pOperatorMember->m_nTitle))
	{
		return EChangeDanceGroupTitleResult_NoAuthority;	//没有操作权限
	}
    if (EMemberProperty_MengXin == pTargeMember->m_nProperty)
    {
        return EChangeDanceGroupTitleResult_CanNotMengxin;
    }
	if (pOperatorMember->m_nTitle <= pTargeMember->m_nTitle)
	{
		return EChangeDanceGroupTitleResult_LowTitle;		//不能对同级及上级进行操作
	}
	if (nNewTitle > EGroupTitle_Leader)
	{
		return EChangeDanceGroupTitleResult_TitleError;		//职位错误
	}
	if (nNewTitle >= pOperatorMember->m_nTitle)
	{
		return EChangeDanceGroupTitleResult_Limited;		//操作职位比自己职位高
	}
	if (nNewTitle == pTargeMember->m_nTitle)
	{
		return EChangeDanceGroupTitleResult_TitleSame;		//职位没变动
	}
// 	if (nNewTitle == EGroupTitle_Assistant)
// 	{
// 		//人数限制
// 		if (pDGInfo->GetTitleMemberCount(EGroupTitle_Assistant) >= cMAX_DANCE_GROUP_ASSISTANT_NUM)
// 		{
// 			return EChangeDanceGroupTitleResult_Full;		//该职位已经满员
// 		}
// 	}
	if (nNewTitle == EGroupTitle_ViceLeader)
	{
		if (pDGInfo->GetTitleMemberCount(EGroupTitle_ViceLeader) >= cMAX_DANCE_GROUP_VICE_LEADER_NUM)
		{
			return EChangeDanceGroupTitleResult_Full;		//该职位已经满员
		}
	}

	return EChangeDanceGroupTitleResult_Success;
}
//检查开除舞团成员条件
EKickOutDanceGroupMemberResult CDanceGroupManager::CheckKickOutDanceGroupMemberCond(unsigned int nOperatorRoleID, unsigned int nGroupID, unsigned int nTargetRoleID)
{
	if (nGroupID == 0)
	{
		return EKickOutDanceGroupMemberResult_NotInGroup;
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo == NULL)
	{
		return EKickOutDanceGroupMemberResult_GroupNotExist;
	}
	CDanceGroupMember * pOperatorMember = pDGInfo->GetGroupMember(nOperatorRoleID);
	if (pOperatorMember == NULL)
	{
		return EKickOutDanceGroupMemberResult_NotGroupMember;	//操作者不是舞团成员
	}
	CDanceGroupMember * pTargeMember = pDGInfo->GetGroupMember(nTargetRoleID);
	if (pTargeMember == NULL)
	{
		return EKickOutDanceGroupMemberResult_TargetNotGroupMember;	//操作对象不是舞团成员
	}
	if (!CanKickOut(pOperatorMember->m_nTitle))
	{
		return EKickOutDanceGroupMemberResult_NoAuthority;	//没有操作权限
	}
	if (pOperatorMember->m_nTitle <= pTargeMember->m_nTitle)
	{
		return EKickOutDanceGroupMemberResult_LowTitle;		//不能对同级及上级进行操作
	}

	return EKickOutDanceGroupMemberResult_Success;
}
//检查修改舞团颜色条件
EChangeDanceGroupColorResult CDanceGroupManager::CheckChangeDanceGroupColorCond(unsigned int nRoleID, unsigned int nGroupID)
{
	if (nGroupID == 0)
	{
		return EChangeDanceGroupColorResult_NotInGroup;		//未加入任何舞团
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo == NULL)
	{
		return EChangeDanceGroupColorResult_GroupNotExist;	//舞团不存在
	}
	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleID);
	if (pMember == NULL)
	{
		return EChangeDanceGroupColorResult_NotGroupMember;	//不是舞团成员
	}
	if (!CanUpdateColor(pMember->m_nTitle))
	{
		return EChangeDanceGroupColorResult_NoAuthority;	//没有操作权限
	}
	return EChangeDanceGroupColorResult_Success;
}
//检查修改舞团徽章条件
EChangeDanceGroupBadgeResult CDanceGroupManager::CheckChangeDanceGroupBadgeCond(unsigned int nRoleID, unsigned int nGroupID)
{
	if (nGroupID == 0)
	{
		return EChangeDanceGroupBadgeResult_NotInGroup;		//未加入任何舞团
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo == NULL)
	{
		return EChangeDanceGroupBadgeResult_GroupNotExist;	//舞团不存在
	}
	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleID);
	if (pMember == NULL)
	{
		return EChangeDanceGroupBadgeResult_NotGroupMember;	//不是舞团成员
	}
	if (!CanUpdateBadge(pMember->m_nTitle))
	{
		return EChangeDanceGroupBadgeResult_NoAuthority;	//没有操作权限
	}
	return EChangeDanceGroupBadgeResult_Success;
}
//检查修改舞团职位名称条件
EChangeDanceGroupTitleNameResult CDanceGroupManager::CheckChangeDanceGroupTitleNameCond(unsigned int nRoleID, unsigned int nGroupID)
{
	if (nGroupID == 0)
	{
		return EChangeDanceGroupTitleNameResult_NotInGroup;		//未加入任何舞团
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo == NULL)
	{
		return EChangeDanceGroupTitleNameResult_GroupNotExist;	//舞团不存在
	}
	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleID);
	if (pMember == NULL)
	{
		return EChangeDanceGroupTitleNameResult_NotGroupMember;	//不是舞团成员
	}
	if (!CanUpdateTitleName(pMember->m_nTitle))
	{
		return EChangeDanceGroupTitleNameResult_NoAuthority;	//没有操作权限
	}
	return EChangeDanceGroupTitleNameResult_Success;
}
//检查修改舞团简介条件
EChangeDanceGroupProfileResult CDanceGroupManager::CheckChangeDanceGroupProfileCond(unsigned int nRoleID, unsigned int nGroupID)
{
	if (nGroupID == 0)
	{
		return EChangeDanceGroupProfileResult_NotInGroup;		//未加入任何舞团
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo == NULL)
	{
		return EChangeDanceGroupProfileResult_GroupNotExist;	//舞团不存在
	}
	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleID);
	if (pMember == NULL)
	{
		return EChangeDanceGroupProfileResult_NotGroupMember;	//不是舞团成员
	}
	if (!CanUpdateProfile(pMember->m_nTitle))
	{
		return EChangeDanceGroupProfileResult_NoAuthority;	//没有操作权限
	}
	return EChangeDanceGroupProfileResult_Success;
}
//检查修改舞团公告条件
EChangeDanceGroupAnnouncementResult CDanceGroupManager::CheckChangeDanceGroupAnnouncementCond(unsigned int nRoleID, unsigned int nGroupID)
{
	if (nGroupID == 0)
	{
		return EChangeDanceGroupAnnouncementResult_NotInGroup;		//未加入任何舞团
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo == NULL)
	{
		return EChangeDanceGroupAnnouncementResult_GroupNotExist;	//舞团不存在
	}
	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleID);
	if (pMember == NULL)
	{
		return EChangeDanceGroupAnnouncementResult_NotGroupMember;	//不是舞团成员
	}
	if (!CanUpdateAnnouncement(pMember->m_nTitle))
	{
		return EChangeDanceGroupAnnouncementResult_NoAuthority;	//没有操作权限
	}
	return EChangeDanceGroupAnnouncementResult_Success;
}
//检查舞团捐赠资源条件
EDonateDanceGroupResourceResult CDanceGroupManager::CheckDonateDanceGroupResourceCond(unsigned int nRoleID, unsigned int nGroupID, unsigned int nResA, unsigned int nResB, unsigned int nResC)
{
	if (nGroupID == 0) {
		return EDonateDanceGroupResourceResult_NoInGroup;		//未加入任何舞团
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);	
	if (pDGInfo == NULL)
	{
		return EDonateDanceGroupResourceResult_GroupNotExist;	//舞团不存在
	}
	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleID);
	if (pMember == NULL)
	{
		return EDonateDanceGroupResourceResult_NotGroupMember;	//不是舞团成员
	}
	if (pDGInfo->m_DGBaseInfo.m_nResA + nResA > cMAX_GROUP_RES_A || pDGInfo->m_DGBaseInfo.m_nResB + nResB > cMAX_GROUP_RES_B || pDGInfo->m_DGBaseInfo.m_nResC + nResC > cMAX_GROUP_RES_C) {
		return EDonateDanceGroupResourceResult_DanceGroupResourceExceedLimit;
	}
	return EDonateDanceGroupResourceResult_Success;
}
//检查升级舞团公告条件
EUpgradeGroupDanceResult CDanceGroupManager::CheckUpgradeDanceGroupCond(unsigned int nRoleID, unsigned int nGroupID)
{
	if (nGroupID == 0) {
		return EUpgradeGroupDanceResult_NoInGroup;		//未加入任何舞团
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);	
	if (pDGInfo == NULL)
	{
		return EUpgradeGroupDanceResult_GroupNotExist;	//舞团不存在
	}
	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleID);
	if (pMember == NULL)
	{
		return EUpgradeGroupDanceResult_NotGroupMember;	//不是舞团成员
	}
	if (pDGInfo->m_DGBaseInfo.m_nLeaderRoleID != nRoleID &&  pMember->m_nTitle <= EGroupTitle_Assistant) 
    {
		return EUpgradeGroupDanceResult_NoAuthority;	//没有权限
	}
	if (pDGInfo->m_DGBaseInfo.m_nLevel >= CDanceGroupDataMgr::Instance().MaxDanceGroupLevel()) {
		return EUpgradeGroupDanceResult_GroupDanceIsMaxLevel;//舞团等级已经是或超过最高等级
	}
	CDGLevelUpRequireRes *pLevelUpRes = CDanceGroupDataMgr::Instance().GetLevelUpRequireResBy(pDGInfo->m_DGBaseInfo.m_nLevel);
	if (pLevelUpRes == NULL) {
		return EUpgradeGroupDanceResult_Unkonw;					//未知原因
	}
	if (pDGInfo->m_DGBaseInfo.m_nResA < pLevelUpRes->m_nResA || pDGInfo->m_DGBaseInfo.m_nResB < pLevelUpRes->m_nResB || pDGInfo->m_DGBaseInfo.m_nResC < pLevelUpRes->m_nResC) {
		return EUpgradeGroupDanceResult_NotEnoughLevelUpRequireResource;
	}

	return EUpgradeGroupDanceResult_Success;
}

//检查修改舞团名字条件
EChangeDanceGroupNameResult CDanceGroupManager::CheckChangeDanceGroupNameCond(unsigned int nRoleID, unsigned int nGroupID, time_t tTimeNow, unsigned int & nTimeCoolDown)
{
	if (nGroupID == 0) {
		return EChangeDanceGroupNameResult_NoInGroup;		//未加入任何舞团
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);	
	if (pDGInfo == NULL)
	{
		return EChangeDanceGroupNameResult_GroupNotExist;	//舞团不存在
	}
	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleID);
	if (pMember == NULL)
	{
		return EChangeDanceGroupNameResult_NotGroupMember;	//不是舞团成员
	}
	if (pDGInfo->m_DGBaseInfo.m_nLeaderRoleID != nRoleID) {
		return EChangeDanceGroupNameResult_NoAuthority;	//没有权限
	}
	if (tTimeNow <= (time_t)(pDGInfo->m_DGBaseInfo.m_nLastChangeGroupNameTime + cCHANGE_DANCE_GROUP_NAME_CD_TIME)) {
		nTimeCoolDown = (unsigned int)(pDGInfo->m_DGBaseInfo.m_nLastChangeGroupNameTime + cCHANGE_DANCE_GROUP_NAME_CD_TIME - tTimeNow);
		return EChangeDanceGroupNameResult_CD;
	}

	return EChangeDanceGroupNameResult_Success;
}

EDanceGroupBadgeResult CDanceGroupManager::CheckDanceGroupBadge( unsigned int nRoleId, unsigned int nGroup, bool bLeader )
{
	if (nGroup == 0) {
		return EDanceGroupBadgeResult_NoInDG;		//未加入任何舞团
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroup);	
	if (pDGInfo == NULL)
	{
		return EDanceGroupBadgeResult_DGNotExist;	//舞团不存在
	}
	if (pDGInfo->m_DGBaseInfo.m_nLeaderRoleID != nRoleId && bLeader ) {
		return EDanceGroupBadgeResult_NoPrivilege;	//没有权限
	}
	return EDanceGroupBadgeResult_Success;
}

EDanceGroupActivityStarResult CDanceGroupManager::CheckDanceGroupActivityStar( unsigned int nRoleId, unsigned int nGroupId )
{
	if (nGroupId == 0) {
		return EDanceGroupActivityStar_NoInDG;		//未加入任何舞团
	}
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupId);	
	if (pDGInfo == NULL)
	{
		return EDanceGroupActivityStar_DGNotExist;	//舞团不存在
	}
    else if (pDGInfo->m_DGActivityInfo.m_nStarLevel > DANCEGROUP_ACTIVITY_STAR_MAX_NUM)
    {
        return EDanceGroupActivityStar_MaxStar;
    }
	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleId);
	if (pMember == NULL)
	{
		return EDanceGroupActivityStar_NotGroupMember;	//不是舞团成员
	}
    if (EMemberProperty_MengXin == pMember->m_nProperty)
    {
        return EDanceGroupActivityStar_CanNotMengxin;
    }
	//当天加入舞团无法参加舞团活动
	int nDiffDays = DiffDays( time(NULL), pMember->m_nInTime );
	if( nDiffDays == 0 )
	{
		return EDanceGroupActivityStar_JoinDGSameDay; //当前加入舞团
	}

	return EDanceGroupActivityStar_Success;
}//end

EDanceGroupShopResult CDanceGroupManager::CheckDanceGroupShop( unsigned int nRoleId, unsigned int nGroupId )
{
	if( nGroupId == 0 )
		return EDanceGroupShopResult_NoInDG;
	
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupId);
	if( pDGInfo == NULL )
		return EDanceGroupShopResult_DGNotExist;

	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleId);
	if( pMember == NULL )
		return EDanceGroupShopResult_NotGroupMember;

	return EDanceGroupShopResult_Success;
}//end

EDanceGroupRecrodsResult CDanceGroupManager::CheckDanceGroupRecrods( unsigned int nRoleId, unsigned int nGroupId )
{
	if( nGroupId == 0 )
		return EDanceGroupRecrodsResult_NoInDG;

	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupId);
	if( pDGInfo == NULL )
		return EDanceGroupRecrodsResult_DGNotExist;

	CDanceGroupMember * pMember = pDGInfo->GetGroupMember(nRoleId);
	if( pMember == NULL )
		return EDanceGroupRecrodsResult_NotGroupMember;

	return EDanceGroupRecrodsResult_Success;
}//end

EAcceptDanceGroupMemberInvitationResult CDanceGroupManager::CheckAcceptInvitationCond(unsigned int nGroupID, unsigned int nSelfGroupID)
{
    if (nGroupID == 0)
        return EAcceptDanceGroupMemberInvitationResult_GroupNotExist;

    CDanceGroupInfo *pDGInfo = GetDanceGroupInfo(nGroupID);
    if (pDGInfo == NULL)
        return EAcceptDanceGroupMemberInvitationResult_GroupNotExist;
    else if (pDGInfo->GetMemberCount() >= GetDanceGroupMaxMemberCount(pDGInfo->m_DGBaseInfo.m_nLevel) )
        return EAcceptDanceGroupMemberInvitationResult_MaxCount;
    else if (nSelfGroupID != 0)
        return EAcceptDanceGroupMemberInvitationResult_HasGroup;

    return EAcceptDanceGroupMemberInvitationResult_Success;
}

//移除舞团信息
void CDanceGroupManager::RemoveDanceGroupInfo(unsigned int nGroupID)
{
	std::map< unsigned int, CDanceGroupInfo * >::iterator it = m_GroupMap.find(nGroupID);
	if (it != m_GroupMap.end())
	{
		delete it->second;
		m_GroupMap.erase(it);
	}
}

//更新舞团基本信息，返回当前被更新的舞团
CDanceGroupInfo * CDanceGroupManager::UpdateDanceGroupBaseInfo(CDanceGroupBaseInfo & rDGBaseInfo)
{
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(rDGBaseInfo.m_nGroupID);
	if (pDGInfo)
	{
		pDGInfo->UpdateBaseInfo(rDGBaseInfo);
	}
	return pDGInfo;
}
//添加舞团成员信息，返回当前添加的舞团
CDanceGroupInfo * CDanceGroupManager::AddDanceGroupMemberInfo(unsigned int nGroupID, CDanceGroupMember & rDGMemberInfo)
{
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo)
	{
		pDGInfo->AddMemberInfo(rDGMemberInfo);
	}
	return pDGInfo;
}
//更新舞团成员信息，返回当前被更新的舞团
CDanceGroupInfo * CDanceGroupManager::UpdateDanceGroupMemberInfo(unsigned int nGroupID, CDanceGroupMember & rDGMemberInfo)
{
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo)
	{
		pDGInfo->UpdateMemberInfo(rDGMemberInfo);
	}
	return pDGInfo;
}
//移除舞团成员信息，返回当前被操作的舞团
CDanceGroupInfo * CDanceGroupManager::RemoveDanceGroupMember(unsigned int nGroupID, unsigned int nRoleID)
{
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo)
	{
		pDGInfo->RemoveMemberInfo(nRoleID);
	}
	return pDGInfo;
}
//添加舞团申请信息，返回当前被操作的舞团
CDanceGroupInfo * CDanceGroupManager::AddRequestDanceGroupInfo(unsigned int nGroupID, CRequestDanceGroupInfo & rRequestDGInfo)
{
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo)
	{
		pDGInfo->AddRequestInfo(rRequestDGInfo);
		_AddRequestEnterDanceGroupInfo(rRequestDGInfo.m_nRoleID, nGroupID, rRequestDGInfo.m_nExpiredTime);
	}
	return pDGInfo;
}



//移除或更新舞团申请信息，返回当前被操作的舞团
CDanceGroupInfo * CDanceGroupManager::RemoveRequestDanceGroupInfo(unsigned int nGroupID, unsigned int nRoleID, bool bRemoveFromDG)
{
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupID);
	if (pDGInfo)
	{
		unsigned int nExpiredTime = 0;

        if (bRemoveFromDG)
            nExpiredTime = pDGInfo->RemoveRequestInfo(nRoleID);
        else
        {
            std::map<unsigned int, CRequestDanceGroupInfo>::const_iterator itr = pDGInfo->m_RequestMap.find(nRoleID);
            if (itr != pDGInfo->m_RequestMap.end())
                nExpiredTime = itr->second.m_nExpiredTime;
        }

		_RemoveRequestEnterDanceGroupInfo(nRoleID, nGroupID, nExpiredTime);
	}
	return pDGInfo;
}
//舞团升级
void CDanceGroupManager::DanceGroupUpgrade(CDanceGroupInfo *pDanceGroupInfo)
{
	if (pDanceGroupInfo != NULL) {
		CDGLevelUpRequireRes *pInfo = CDanceGroupDataMgr::Instance().GetLevelUpRequireResBy(pDanceGroupInfo->m_DGBaseInfo.m_nLevel);
		if (pInfo != NULL) {
			if (pDanceGroupInfo->m_DGBaseInfo.m_nResA >= pInfo->m_nResA && pDanceGroupInfo->m_DGBaseInfo.m_nResB >= pInfo->m_nResB && pDanceGroupInfo->m_DGBaseInfo.m_nResC >= pInfo->m_nResC) {
				pDanceGroupInfo->m_DGBaseInfo.m_nResA -= pInfo->m_nResA;
				pDanceGroupInfo->m_DGBaseInfo.m_nResB -= pInfo->m_nResB;
				pDanceGroupInfo->m_DGBaseInfo.m_nResC -= pInfo->m_nResC;

				pDanceGroupInfo->Upgrade();
			}
		}
	}
}

void CDanceGroupManager::AddDanceGroupRecord( unsigned int nGroupId, CDanceGroupRecordsInfo &rRecordsInfo )
{
	CDanceGroupInfo * pDGInfo = GetDanceGroupInfo(nGroupId);
	if( pDGInfo != NULL )
	{
		pDGInfo->m_recordsMap.insert( make_pair( rRecordsInfo.m_nRecordTime, rRecordsInfo ) );
	}
}

bool CDanceGroupManager::ProcessRemoveDanceGroup(unsigned int nGroupID)
{
    CDanceGroupInfo *pDGInfo = GetDanceGroupInfo(nGroupID);

    if (NULL != pDGInfo && !pDGInfo->IsDismissed())
    {
        unsigned int nNow = (unsigned int)time(NULL);
        pDGInfo->m_DGBaseInfo.m_nLastDismissTime = nNow;
        m_multimapRemovedGroup.insert(std::make_pair(nNow, nGroupID));

        return true;
    }

    return false;
}

bool CDanceGroupManager::ProcessCancelRemoveDanceGroup(unsigned int nGroupID)
{
    CDanceGroupInfo *pDGInfo = GetDanceGroupInfo(nGroupID);

    if (NULL != pDGInfo && pDGInfo->IsDismissed())
    {
        typedef std::multimap<unsigned int, unsigned int>::iterator Multimap_UI_UI_Itr;
        std::pair<Multimap_UI_UI_Itr, Multimap_UI_UI_Itr> range = 
            m_multimapRemovedGroup.equal_range(pDGInfo->m_DGBaseInfo.m_nLastDismissTime);

        for (Multimap_UI_UI_Itr itr = range.first; itr != range.second; ++itr)
        {
            if (nGroupID == itr->second)
            {
                m_multimapRemovedGroup.erase(itr);

                break;
            }
        }

        pDGInfo->m_DGBaseInfo.m_nLastCancelDismissTime = (unsigned int)time(NULL);

        return true;
    }

    return false;
}

void CDanceGroupManager::InsertRoleQuitInfo(DanceGroupQuitInfo &quitInfo)
{
    DanceGroupQuitInfoMap::iterator iter = m_quitInfoMap.find(quitInfo.m_nRoleID);
    if (m_quitInfoMap.end() != iter)
    {
        iter->second = quitInfo;
    }
    else
    {
        m_quitInfoMap[quitInfo.m_nRoleID] = quitInfo;
    }

    return ;
}

DanceGroupQuitInfo* CDanceGroupManager::GetQuitInfo(unsigned int nRoleID)
{
    DanceGroupQuitInfoMap::iterator iter = m_quitInfoMap.find(nRoleID);
    if (m_quitInfoMap.end() != iter)
        return &(iter->second);

    return NULL;
}


//添加角色申请记录
void CDanceGroupManager::_AddRequestEnterDanceGroupInfo(unsigned int nRoleID, unsigned int nGroupID, unsigned int nExpiredTime)
{
	std::map< unsigned int, std::set< unsigned int > >::iterator it = m_RoleRequestGroupMap.find(nRoleID);
	if (it != m_RoleRequestGroupMap.end())
	{
		std::set< unsigned int > & rGroupIDSet = it->second;
		rGroupIDSet.insert(nGroupID);
	}
	else
	{
		std::set< unsigned int > oNewSet;
		oNewSet.insert(nGroupID);
		m_RoleRequestGroupMap[nRoleID] = oNewSet;
	}

    std::map<unsigned int, std::map<unsigned int, unsigned int> >::iterator requestTimeItr = m_mapRequestTime.find(nExpiredTime);
    if (requestTimeItr == m_mapRequestTime.end())
    {
        std::map<unsigned int, unsigned int> newMap;
        newMap.insert(std::make_pair(nRoleID, nGroupID));

        m_mapRequestTime.insert(std::make_pair(nExpiredTime, newMap));
    }
    else
        requestTimeItr->second.insert(std::make_pair(nRoleID, nGroupID));
}
//移除角色申请记录
void CDanceGroupManager::_RemoveRequestEnterDanceGroupInfo(unsigned int nRoleID, unsigned int nGroupID, unsigned int nExpiredTime)
{
	std::map< unsigned int, std::set< unsigned int > >::iterator it = m_RoleRequestGroupMap.find(nRoleID);
	if (it != m_RoleRequestGroupMap.end())
	{
		std::set< unsigned int > & rGroupIDSet = it->second;
		rGroupIDSet.erase(nGroupID);
	}

    if (0 == nExpiredTime)
        return;

    std::map<unsigned int, std::map<unsigned int, unsigned int> >::iterator requestTimeItr = m_mapRequestTime.find(nExpiredTime);
    if (requestTimeItr != m_mapRequestTime.end())
        it->second.erase(nRoleID);
}

//////////////////////////////////////////////////////////////////////////

DanceGroupChallengeChanceEventDB::DanceGroupChallengeChanceEventDB()
    : m_nRoleID( 0 )
    , m_nValue( 0 )
    , m_nResetTime( 0 )
{
}

DanceGroupChallengeChanceEventDB::~DanceGroupChallengeChanceEventDB()
{
}

DanceGroupChallengeRanking::DanceGroupChallengeRanking()
    : m_nStorey( 0 )
    , m_nRanking( 0 )
    , m_nState( EDanceGroupChallengeRankingState_None )
{
}

DanceGroupChallengeRanking::DanceGroupChallengeRanking( int nStorey, short nRanking, EDanceGroupChallengeRankingState eState )
    : m_nStorey( nStorey )
    , m_nRanking( nRanking )
    , m_nState( (unsigned char)eState )
{
}

DanceGroupChallengeRanking::~DanceGroupChallengeRanking()
{
}

DanceGroupChallengeRecordRankingInfo::DanceGroupChallengeRecordRankingInfo( unsigned int nStartTime, int nStorey )
    : m_nStartTime( nStartTime )
    , m_nStorey( nStorey )
{
}

DanceGroupChallengeRecordRankingInfo::~DanceGroupChallengeRecordRankingInfo()
{
}

void DanceGroupChallengeRecordRankingInfo::SetAllRoleID( std::list<unsigned int> &rAll )
{
    m_listRoleID.swap( rAll );
}

DanceGroupChallengeOpendStoreyInfo::DanceGroupChallengeOpendStoreyInfo()
    : m_nStorey( 0 )
    , m_nScene( 0 )
    , m_nMusic( 0 )
    , m_nMusicMode( 0 )
    , m_nMusicLevel( 0 )
    , m_nTargetScore( 0 )
    , m_nCurrentScore( 0 )
{
}

DanceGroupChallengeOpendStoreyInfo::~DanceGroupChallengeOpendStoreyInfo()
{
}

bool DanceGroupChallengeOpendStoreyInfo::IsFinished() const
{
    return m_nTargetScore <= m_nCurrentScore;
}

void DanceGroupChallengeOpendStoreyInfo::AddScore( unsigned int nScore )
{
    m_nCurrentScore += nScore;
}

DanceGroupChallengeRoleInfo::DanceGroupChallengeRoleInfo()
    : m_nRoleID( 0 )
    , m_nScore( 0 )
    , m_nInspire( 0 )
{
}

DanceGroupChallengeRoleInfo::DanceGroupChallengeRoleInfo( unsigned int nRoleID, 
    const std::string &rName, unsigned int nScore, unsigned int nInspire )
    : m_nRoleID( nRoleID )
    , m_strRoleName( rName )
    , m_nScore( nScore )
    , m_nInspire( nInspire )
{
}

DanceGroupChallengeRoleInfo::~DanceGroupChallengeRoleInfo()
{
}

DanceGroupChallengeContribution::DanceGroupChallengeContribution( int nStorey )
    : m_nStorey( nStorey )
    , m_nTotalInspire( 0 )
{
}

DanceGroupChallengeContribution::~DanceGroupChallengeContribution()
{
    for ( std::map<unsigned int, DanceGroupChallengeRoleInfo *>::iterator itr = m_mapRoleInfo.begin(); 
        itr != m_mapRoleInfo.end(); ++itr )
    {
        SAFE_DELETE( itr->second );
    }
}

unsigned int DanceGroupChallengeContribution::GetRoleScore( unsigned int nRoleID ) const
{
    std::map<unsigned int, DanceGroupChallengeRoleInfo *>::const_iterator itr = m_mapRoleInfo.find( nRoleID );
    if ( itr != m_mapRoleInfo.end() && itr->second != NULL )
        return itr->second->GetScore();

    return 0;
}

unsigned int DanceGroupChallengeContribution::GetRoleInspire( unsigned int nRoleID ) const
{
    std::map<unsigned int, DanceGroupChallengeRoleInfo *>::const_iterator itr = m_mapRoleInfo.find( nRoleID );
    if ( itr != m_mapRoleInfo.end() && itr->second != NULL )
        return itr->second->GetInspire();

    return 0;
}

void DanceGroupChallengeContribution::AddRoleScore( unsigned int nRoleID, const std::string &rName, unsigned int nAddScore )
{
    DanceGroupChallengeRoleInfo *pRoleInfo = m_mapRoleInfo[nRoleID];

    if ( pRoleInfo == NULL )
    {
        pRoleInfo = new DanceGroupChallengeRoleInfo( nRoleID, rName, 0, 0 );
        m_mapRoleInfo[nRoleID] = pRoleInfo;
    }

    unsigned int nOldScore = pRoleInfo->GetScore();
    unsigned int nNewScore = nOldScore + nAddScore;

    pRoleInfo->SetScore( nNewScore );
    UpdateRank( m_mapScore, nOldScore, nNewScore, pRoleInfo );
}

void DanceGroupChallengeContribution::AddRoleInspire( unsigned int nRoleID, const std::string &rName, unsigned int nAddInspire )
{
    DanceGroupChallengeRoleInfo *pRoleInfo = m_mapRoleInfo[nRoleID];

    if ( pRoleInfo == NULL )
    {
        pRoleInfo = new DanceGroupChallengeRoleInfo( nRoleID, rName, 0, 0 );
        m_mapRoleInfo[nRoleID] = pRoleInfo;
    }

    unsigned int nOldInspire = pRoleInfo->GetInspire();
    unsigned int nNewInspire = nOldInspire + nAddInspire;
    m_nTotalInspire += nAddInspire;

    pRoleInfo->SetInspire( nNewInspire );
    UpdateRank( m_mapInspire, nOldInspire, nNewInspire, pRoleInfo );
}

void DanceGroupChallengeContribution::UpdateRank( std::multimap<unsigned int, DanceGroupChallengeRoleInfo *, std::greater<unsigned int> > &rMap, 
    unsigned int nOldKey, unsigned int nNewKey, DanceGroupChallengeRoleInfo *pInfo )
{
    if ( pInfo == NULL || nOldKey == nNewKey )
        return;

    typedef std::multimap<unsigned int, DanceGroupChallengeRoleInfo *, std::greater<unsigned int> >::iterator ItrType;
    std::pair<ItrType, ItrType> pos = std::make_pair( rMap.end(), rMap.end() );

    if ( nOldKey != 0 ) // 不存在key为0的值
        pos = rMap.equal_range( nOldKey );

    ItrType itr = pos.first;
    for ( ; itr != pos.second; ++itr )
    {
        if ( itr->second == pInfo )
        {
            break;
        }
    }

    if ( nNewKey != 0 ) // 不存在key为0的值
        rMap.insert( std::make_pair( nNewKey, pInfo ) );

    if ( itr != pos.second )
        rMap.erase( itr );
}

DanceGroupChallengeUpdateRoleContribution::DanceGroupChallengeUpdateRoleContribution( 
    unsigned int nGroupID, unsigned int nStartTime, unsigned int nRoleID )
    : m_nType( EDanceGroupChallengeUpdateRoleContributionType_ALL )
    , m_nGroupID( nGroupID )
    , m_nStartTime( nStartTime )
    , m_nStorey( MIN_CHALLENGE_STOREY )
    , m_nRoleID( nRoleID )
    , m_nScore( 0 )
    , m_nInspire( 0 )
{
}

DanceGroupChallengeUpdateRoleContribution::~DanceGroupChallengeUpdateRoleContribution()
{
}

void DanceGroupChallengeUpdateRoleContribution::SetType( EDanceGroupChallengeUpdateRoleContributionType eType )
{
    unsigned char nType = (unsigned char)eType;
    if ( nType < EDanceGroupChallengeUpdateRoleContributionType_Max )
        m_nType = nType;
}

bool DanceGroupChallengeUpdateRoleContribution::IsValid() const
{
    if ( m_nGroupID == 0 || m_nStartTime == 0 || m_nRoleID == 0 || m_nStorey < MIN_CHALLENGE_STOREY )
        return false;

    return true;
}

DanceGroupChallengeInfo::DanceGroupChallengeInfo( unsigned int nGroupID, unsigned int nStartTime )
    : m_nGroupID( nGroupID )
    , m_nStartTime( nStartTime )
{
}

DanceGroupChallengeInfo::~DanceGroupChallengeInfo()
{
    for ( std::map<int, DanceGroupChallengeOpendStoreyInfo *>::iterator itrOpend = m_mapOpendStorey.begin(); 
        itrOpend != m_mapOpendStorey.end(); ++itrOpend )
    {
        SAFE_DELETE( itrOpend->second );
    }

    for ( std::map<int, DanceGroupChallengeContribution *>::iterator itr = m_mapStoreyContribution.begin(); 
        itr != m_mapStoreyContribution.end(); ++itr )
    {
        SAFE_DELETE( itr->second );
    }
}

DanceGroupChallengeOpendStoreyInfo* DanceGroupChallengeInfo::GetOpendStoreyInfo( int nStorey ) const
{
    std::map<int, DanceGroupChallengeOpendStoreyInfo *>::const_iterator itr = m_mapOpendStorey.find( nStorey );
    if ( itr != m_mapOpendStorey.end() )
        return itr->second;

    return NULL;
}

void DanceGroupChallengeInfo::AddOpendStoreyInfo( DanceGroupChallengeOpendStoreyInfo *pOpendInfo )
{
    if ( pOpendInfo == NULL )
        return;

    DanceGroupChallengeOpendStoreyInfo *pTmp = m_mapOpendStorey[pOpendInfo->GetStorey()];
    m_mapOpendStorey[pOpendInfo->GetStorey()] = pOpendInfo;

    SAFE_DELETE( pTmp );
}

bool DanceGroupChallengeInfo::IsStoreyOpened( int nStorey ) const
{
    std::map<int, DanceGroupChallengeOpendStoreyInfo *>::const_iterator itr = m_mapOpendStorey.find( nStorey );
    if ( itr != m_mapOpendStorey.end() && itr->second != NULL )
        return true;

    return false;
}

bool DanceGroupChallengeInfo::IsStoreyFinished( int nStorey ) const
{
    std::map<int, DanceGroupChallengeOpendStoreyInfo *>::const_iterator itr = m_mapOpendStorey.find( nStorey );
    if ( itr != m_mapOpendStorey.end() && itr->second != NULL && itr->second->IsFinished() )
        return true;

    return false;
}

bool DanceGroupChallengeInfo::IsPrevStoreyFinished( int nStorey ) const
{
    if ( nStorey == MIN_CHALLENGE_STOREY )
        return true;

    return IsStoreyFinished( nStorey - 1 );
}

int DanceGroupChallengeInfo::GetCurrentStorey( unsigned char *pState ) const
{
    std::map<int, DanceGroupChallengeOpendStoreyInfo *>::const_reverse_iterator ritr = m_mapOpendStorey.rbegin();
    if ( ritr != m_mapOpendStorey.rend() && ritr->second != NULL )
    {
        int nStorey = ritr->second->GetStorey();

        if ( ritr->second->IsFinished() )
        {
            ++nStorey;

            if ( pState != NULL )
            {
                *pState = EDanceGroupChallengeStoreyState_NotStart;
            }
        }
        else if ( pState != NULL )
        {
            *pState = EDanceGroupChallengeStoreyState_Challenging;
        }

        return nStorey;
    }

    if ( pState != NULL )
        *pState = EDanceGroupChallengeStoreyState_NotStart;

    return MIN_CHALLENGE_STOREY;
}

DanceGroupChallengeContribution* DanceGroupChallengeInfo::GetStoreyContribution( int nStorey )
{
    std::map<int, DanceGroupChallengeContribution *>::iterator itr = m_mapStoreyContribution.find( nStorey );
    if ( itr != m_mapStoreyContribution.end() )
        return itr->second;

    return NULL;
}

void DanceGroupChallengeInfo::AddStoreyContribution( DanceGroupChallengeContribution *pContribution )
{
    if ( pContribution != NULL )
        m_mapStoreyContribution.insert( std::make_pair( pContribution->GetStorey(), pContribution ) );
}

//////////////////////////////////////////////////////////////////////////
/// 舞团秘境功能 BEGIN
//////////////////////////////////////////////////////////////////////////

FairyLandRewardItem::FairyLandRewardItem()
    :m_nItemId(0)
    , m_nDuration(0)
    , m_nCount(0)
{}

Json::Value FairyLandRewardItem::toJson()
{
    Json::Value tv;
    tv[keyItemId] = m_nItemId;
    tv[keyDuration] = m_nDuration;
    tv[keyCount] = m_nCount;
    return tv;
}

void FairyLandRewardItem::Parse(const Json::Value &Jsval)
{
    m_nItemId = Jsval[keyItemId].asInt();
    m_nDuration = Jsval[keyDuration].asInt();
    m_nCount = (unsigned short)Jsval[keyCount].asInt();
}

bool FairyLandRewardItem::operator == (const FairyLandRewardItem& rOther)
{
    if (m_nItemId == rOther.m_nItemId && 
        m_nDuration == rOther.m_nDuration)
    {
        return true;
    }
    return false;
}


FairyLandChapterReward::FairyLandChapterReward()
    :m_nBindMB(0)
    , m_nMoney(0)
{}

FairyLandChapterReward::FairyLandChapterReward(const FairyLandChapterReward& other)
{
    m_nBindMB = other.m_nBindMB;
    m_listItems = other.m_listItems;
    m_nMoney = other.m_nMoney;
}

Json::Value FairyLandChapterReward::toJson()
{
    Json::Value tv;
    Json::Value item;
    for (std::list<FairyLandRewardItem>::iterator it = m_listItems.begin();
        it != m_listItems.end(); ++it)
    {
        item.append(it->toJson());
    }

    tv[keyRewardBindMB] = m_nBindMB;
    tv[keyRewardMoney] = m_nMoney;
    tv[keyRewardItems] = item;

    return tv;
}

void FairyLandChapterReward::Parse(const Json::Value &Jsval)
{
    m_nBindMB = Jsval[keyRewardBindMB].asInt();
    m_nMoney = Jsval[keyRewardMoney].asInt();

    const Json::Value array = Jsval[keyRewardItems];
    for (int i = 0; i < (int)array.size(); ++i)
    {
        FairyLandRewardItem cell;
        cell.Parse(array[i]);
        m_listItems.push_back(cell);
    }
}

void FairyLandChapterReward::operator += (const FairyLandChapterReward& rOther)
{
    m_nBindMB += rOther.m_nBindMB;
    m_nMoney += rOther.m_nMoney;

    for (std::list<FairyLandRewardItem>::const_iterator OtherIt = rOther.m_listItems.begin();
        OtherIt != rOther.m_listItems.end(); ++OtherIt)
    {
        std::list<FairyLandRewardItem>::iterator MyIt;
        for (MyIt = m_listItems.begin(); MyIt != m_listItems.end(); ++MyIt)
        {
            if (*MyIt == *OtherIt)
            {
                MyIt->m_nCount += OtherIt->m_nCount;
                break;
            }
        }

        if (MyIt == m_listItems.end())
        {
            m_listItems.push_back(*OtherIt);
        }
    }
}

/// <summary>
/// FairyLandRankCell
/// <summary>

FairyLandRankCell::FairyLandRankCell()
    :m_nRoleId(0)
    , m_nValue(0)
    , m_strName("")
{

}

FairyLandRankCell::FairyLandRankCell(int r, int v,const std::string & name)
{
    m_nRoleId = r;
    m_nValue = v;
    m_strName = name;
}

bool FairyLandRankCell::operator <(const FairyLandRankCell &rhs) const
{
    return m_nTotalSocre < rhs.m_nTotalSocre;
}

bool FairyLandRankCell::operator>(const FairyLandRankCell &rhs) const
{
    return m_nTotalSocre > rhs.m_nTotalSocre;
}


Json::Value FairyLandRankCell::toJson()
{
    Json::Value tv;
    tv[RankRoleId] = m_nRoleId;
    tv[RankName] = m_strName;
    tv[RankValue] = m_nValue;
    return tv;
}

void FairyLandRankCell::Parse(const Json::Value &Jsval)
{
    m_nRoleId = Jsval[RankRoleId].asInt();
    m_nValue = Jsval[RankValue].asInt();
    m_strName = Jsval[RankName].asString();
}

/// <summary>
/// PersonalInfo
/// <summary>
DanceGroupFairyLandPersonalInfo::DanceGroupFairyLandPersonalInfo()
    : m_nRoleId(0)
    , m_nGroupId(0)
    , m_nTotalScore(0)
    , m_nBlassTimes(0)
    , m_nLastChallengeChapterLvl(0)
    , m_nCurChapterChallengeTimes(0)
{}

void DanceGroupFairyLandPersonalInfo::AddPassedChapterReward(FairyLandChapterReward &cell)
{
    m_PassedChapterReward += cell;
}

void DanceGroupFairyLandPersonalInfo::GetPassedChapterReward(FairyLandChapterReward& ret)
{
    ret = m_PassedChapterReward;
}

void DanceGroupFairyLandPersonalInfo::ParseRewardFlag(const std::string &str, unsigned char &sex, set<int> &setRewardFlag)
{
    std::vector<int> vec;
    Split(str, "|", vec);

    if (!vec.empty())
        sex = (unsigned char)vec[0];

    for (size_t i = 0; i < vec.size(); ++i)
        setRewardFlag.insert(vec[i]);
}


void DanceGroupFairyLandPersonalInfo::ParsePassedReward(const std::string &JsonFmt)
{
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(JsonFmt, value))
    {
        const Json::Value array = value[keyPassedRewardRecord];
        m_PassedChapterReward.Parse(array);
    }
}

std::string DanceGroupFairyLandPersonalInfo::PassedRewardToJson()
{
    Json::Value tv;
    Json::Value cell;
    cell = m_PassedChapterReward.toJson();
    tv[keyPassedRewardRecord] = cell;
    
    Json::FastWriter js;
    return js.write(tv);
}

void DanceGroupFairyLandPersonalInfo::ParseStorageBox(const std::string& JsonFmt)
{
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(JsonFmt, value))
    {
        const Json::Value array = value[keyStorageBox];
        for (int i = 0; i < (int)array.size(); ++i)
        {
            FairyLandRewardItem cell;
            cell.Parse(array[i]);
            m_listStorageBoxItems.push_back(cell);
        }
    }
}

std::string DanceGroupFairyLandPersonalInfo::StorageBoxToJson()
{
    Json::Value tv;
    Json::Value cell;
    for (std::list<FairyLandRewardItem>::iterator it = m_listStorageBoxItems.begin();
        it != m_listStorageBoxItems.end(); ++it)
    {
        cell.append(it->toJson());
    }

    tv[keyStorageBox] = cell;

    Json::FastWriter writer;
    return writer.write(tv);
}

void DanceGroupFairyLandPersonalInfo::RewardFlagToString(unsigned char sex, const set<int> &setRewardFlag, std::string &str)
{
    std::vector<int> vec;
    vec.push_back(sex);
    vec.insert(vec.end(), setRewardFlag.begin(), setRewardFlag.end());
    ConcatToString(vec, "|", str);
}

void DanceGroupFairyLandPersonalInfo::SetCurChapterChallengeTimes(int nTimes) 
{ 
    m_nCurChapterChallengeTimes = nTimes; 
}

int DanceGroupFairyLandPersonalInfo::GetCurChapterChallengeTimes(int nChapter)
{ 
    return m_nLastChallengeChapterLvl == nChapter ? m_nCurChapterChallengeTimes : 0; 
}

void DanceGroupFairyLandPersonalInfo::AddStorageBoxItem(FairyLandRewardItem & item)
{ 
    for (std::list<FairyLandRewardItem>::iterator iter = m_listStorageBoxItems.begin(); iter != m_listStorageBoxItems.end(); ++iter)
    {
        if (*iter == item)
        {
            iter->m_nCount += item.m_nCount;
            return;
        }
    }

    m_listStorageBoxItems.push_back(item);
    return;
}

void DanceGroupFairyLandPersonalInfo::GetStorageBoxItems(std::list<FairyLandRewardItem>& listItems)
{ 
    listItems = m_listStorageBoxItems; 
}

void DanceGroupFairyLandPersonalInfo::ClearStorageBoxItems()
{ 
    m_listStorageBoxItems.clear(); 
}


bool DanceGroupFairyLandPersonalInfo::CheckIsHaveJoinReward(int nChapter)
{
    return m_setJoinRewardFalg.find(nChapter) != m_setJoinRewardFalg.end();
}
// 
// void DanceGroupFairyLandPersonalInfo::SetPassRewardFlag(int nChapter)
// {
//     m_setPassRewardFalg.insert(nChapter);
// }

void DanceGroupFairyLandPersonalInfo::SetJoinRewardFlag(int nChapter)
{
    m_setJoinRewardFalg.insert(nChapter);
}

/// <summary>
/// GroupInfo
/// <summary>
DanceGroupFairyLandGroupInfo::DanceGroupFairyLandGroupInfo()
    : RANKLEN(20)
    , m_nGroupId(0)
    , m_nCurChapterLvl(0)
    , m_nTotalScore(0)
    , m_nBlessBuffEndTimeStamp(0)
    , m_nLastBlessRoleID(0)
{}

bool DanceGroupFairyLandGroupInfo::TryAddTodayRank(int nRoleId, int nScore, const std::string & strName)
{
    return TryAddRank(nRoleId, nScore, strName, m_listTodayScoreRank);
}

void DanceGroupFairyLandGroupInfo::SetYestedayRank(std::list<FairyLandRankCell> &list)
{
    m_listYesterdayScoreRank = list;
}

bool DanceGroupFairyLandGroupInfo::TryAddBlessRank(int nRoleId, int nBless, const std::string & strName)
{
    return TryAddRank(nRoleId, nBless, strName, m_listBlessRank);
}

bool DanceGroupFairyLandGroupInfo::ParseScoreTodayRank(const std::string &strJsonFormat)
{
    return ParseRank(strJsonFormat, m_listTodayScoreRank);
}

std::string DanceGroupFairyLandGroupInfo::ScoreTodayRankToJson()
{
    return RanktoJson(m_listTodayScoreRank);
}

bool DanceGroupFairyLandGroupInfo::ParseScoreYesterdayRank(const std::string &strJsonFormat)
{
    return ParseRank(strJsonFormat, m_listYesterdayScoreRank);
}

std::string DanceGroupFairyLandGroupInfo::ScoreYesterdayToJson()
{
    return RanktoJson(m_listYesterdayScoreRank);
}

bool DanceGroupFairyLandGroupInfo::ParseBlessRank(const std::string &strJsonFormat)
{
    return ParseRank(strJsonFormat, m_listBlessRank);
}

std::string DanceGroupFairyLandGroupInfo::BlessRankToJson()
{
    return RanktoJson(m_listBlessRank);
}

bool DanceGroupFairyLandGroupInfo::ParseRank(const std::string &strJsonFormat, std::list<FairyLandRankCell>& listRank)
{
    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strJsonFormat, value))
    {
        const Json::Value array = value[Rank];
        for (int i = 0; i < (int)array.size(); ++i)
        {
            FairyLandRankCell cell;
            cell.Parse(array[i]);
            listRank.push_back(cell);
        }
        return true;
    }

    return false;
}

std::string DanceGroupFairyLandGroupInfo::RanktoJson(std::list<FairyLandRankCell>& listRank)
{
    Json::Value tv;
    Json::Value cell;
    for (std::list<FairyLandRankCell>::iterator it = listRank.begin();
        it != listRank.end(); ++it)
    {
        cell.append(it->toJson());
    }
    tv[Rank] = cell;

    return JValueToString(tv);
}


void DanceGroupFairyLandGroupInfo::SetCurChapterLvl(int nChatperLvl)
{
    m_nCurChapterLvl = nChatperLvl;
}

bool DanceGroupFairyLandGroupInfo::TryAddRank(int nRole, int nValue, const std::string &name, std::list<FairyLandRankCell> &destRank)
{
    const int RANKLEN = 20;

    for (std::list<FairyLandRankCell>::iterator it = destRank.begin();
        it != destRank.end(); ++it)
    {
        if (nRole == it->m_nRoleId)
        { // 原先有，直接添加
            if (it->m_nValue < nValue)
            {
                it->m_nValue = nValue;
                destRank.sort(std::greater<FairyLandRankCell>());
                return true;
            }
            return false;
        }
    }

    FairyLandRankCell cell(nRole, nValue, name);
    if ((int)destRank.size() > RANKLEN)
    {
        if (destRank.back().m_nValue > nValue)
        {
            return false;
        }
        else
        {
            destRank.pop_back();
            destRank.push_back(cell);
        }
    }
    else
    {
        destRank.push_back(cell);
    }
    destRank.sort(std::greater<FairyLandRankCell>());

    return true;
}

void DanceGroupFairyLandGroupInfo::SetLastBlessRole(unsigned int nRoleID, std::string strRoleName)
{
    m_nLastBlessRoleID = nRoleID;
    m_strLastBlessRoleName = strRoleName;

    return;
}

unsigned int DanceGroupFairyLandGroupInfo::GetLastBlessRoleID() const
{
    return m_nLastBlessRoleID;
}

std::string DanceGroupFairyLandGroupInfo::GetLastBlessRoleName() const
{
    return m_strLastBlessRoleName;
}


FairyLandLoadAllDB::FairyLandLoadAllDB()
    : m_pGInfo(NULL)
    , m_pGPInfo(NULL)
    , m_nNewDayStamp(0)
    , m_nPassRewardStamp(0)
    , m_nStorgeRewardStamp(0)
{

}

FairyLandLoadAllDB::~FairyLandLoadAllDB()
{

}

FairyLandLastBlessInfoDB::FairyLandLastBlessInfoDB(unsigned int nGroupID, unsigned int nLastRoleID, std::string strBlessRoleName, time_t endStamp)
    : m_nGroupID(nGroupID)
    , m_nBlessEndTimeStamp(endStamp)
    , m_nLastBlessRoleID(nLastRoleID)
    , m_strLastBlessRoleName(strBlessRoleName)
{
}




//////////////////////////////////////////////////////////////////////////
/// 舞团秘境功能 END
//////////////////////////////////////////////////////////////////////////
