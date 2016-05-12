#include "ceremonyRoomHead.h"
#include "../../socket/ParamPool.h"


CeremonyLogRoomInfo::CeremonyLogRoomInfo()
    : m_nLogID(0)
    , m_nCreatorID(0)
    , m_nCreatorVip(0)
    , m_logTime(0)
{

}

CeremonyLogRoomInfo::~CeremonyLogRoomInfo()
{

}

void CeremonyLogRoomInfo::DoDecode(CParamPool &IOBuff)
{
    m_nLogID = IOBuff.GetUInt();
    m_nCreatorID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strCreatorName);
    m_nCreatorVip = IOBuff.GetUChar();
    m_logTime = IOBuff.GetUInt();

    return ;
}

void CeremonyLogRoomInfo::DoEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nLogID);
    IOBuff.AddUInt(m_nCreatorID);
    IOBuff.AddStringW(m_strCreatorName);
    IOBuff.AddUChar(m_nCreatorVip);
    IOBuff.AddUInt(m_logTime);

    return ;
}


CeremonyLogNewerInfo::CeremonyLogNewerInfo()
    : m_nRoleID(0)
{

}

CeremonyLogNewerInfo::~CeremonyLogNewerInfo(){}

void CeremonyLogNewerInfo::DoEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName);
    IOBuff.AddUChar(m_nVipLv);
    return ;
}

void CeremonyLogNewerInfo::DoDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRoleName);
    m_nVipLv = IOBuff.GetUChar();

    return ;
}


CeremonyLogVoterInfo::CeremonyLogVoterInfo()
    : m_nPraiserID(0)
    , m_nPraiserVipLv(0)
    , m_nPraiserCount(0)
    , m_logTime(0)
{

}
CeremonyLogVoterInfo::~CeremonyLogVoterInfo(){}

void CeremonyLogVoterInfo::DoEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nPraiserID);
    IOBuff.AddStringW(m_strPraiserName);
    IOBuff.AddUChar(m_nPraiserVipLv);
    IOBuff.AddUShort(m_nPraiserCount);
    IOBuff.AddUInt(m_logTime);

    return ;
}

void CeremonyLogVoterInfo::DoDecode(CParamPool &IOBuff)
{
    m_nPraiserID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strPraiserName);
    m_nPraiserVipLv = IOBuff.GetUChar();
    m_nPraiserCount = IOBuff.GetUShort();
    m_logTime = IOBuff.GetUInt();

    return ;
}

void CeremonyLogVoterInfo::Set(unsigned int nPraiserID, std::string strName, unsigned char vipLev, unsigned short count, unsigned int now)
{
    m_nPraiserID = nPraiserID;
    m_strPraiserName = strName;
    m_nPraiserVipLv = vipLev;
    m_nPraiserCount = count;
    m_logTime = now;
}

CeremonyPraiseLogInfo::CeremonyPraiseLogInfo()
{

}

CeremonyPraiseLogInfo::~CeremonyPraiseLogInfo()
{

}

// 存在则更新，否则添加
void CeremonyPraiseLogInfo::AddVoterInfo(CeremonyLogVoterInfo &voterInfo)
{
    for (size_t i = 0; i < m_vecVoterInfo.size(); ++i)
    {
        if (m_vecVoterInfo[i].m_nPraiserID == voterInfo.m_nPraiserID)
        {
            m_vecVoterInfo[i].m_nPraiserCount += voterInfo.m_nPraiserCount;
            m_vecVoterInfo[i].m_logTime = voterInfo.m_logTime;
            return ; // 添加完毕
        }
    }

    m_vecVoterInfo.push_back(voterInfo); // 如果原先没有投这个新人，则添加一个新的信息
}

void CeremonyPraiseLogInfo::DoEncode(CParamPool &IOBuff)
{
    m_newerInfo.DoEncode(IOBuff);
    IOBuff.AddUShort((unsigned short)m_vecVoterInfo.size());
    for (size_t i = 0; i < m_vecVoterInfo.size(); ++i)
    {
        m_vecVoterInfo[i].DoEncode(IOBuff);
    }
    
    return ;
}

void CeremonyPraiseLogInfo::DoDecode(CParamPool &IOBuff)
{
    m_newerInfo.DoDecode(IOBuff);
    unsigned short len = IOBuff.GetUShort();
    for (unsigned short i = 0; i < len; ++i)
    {
        CeremonyLogVoterInfo v;
        v.DoDecode(IOBuff);
        m_vecVoterInfo.push_back(v);
    }

    return ;
}



DanceGroupCeremonyLog::DanceGroupCeremonyLog(){}
DanceGroupCeremonyLog::~DanceGroupCeremonyLog(){}

void DanceGroupCeremonyLog::DoEncode(CParamPool &IOBuff)
{
    CeremonyLogRoomInfo m_roomBaseLog;             // 
    std::vector<CeremonyPraiseLogInfo> m_voteInfo; // 

    m_roomBaseLog.DoEncode(IOBuff);
    IOBuff.AddUShort((unsigned short)m_voteInfo.size());
    for (size_t i = 0; i < m_voteInfo.size(); ++i)
    {
        m_voteInfo[i].DoEncode(IOBuff);
    }

    return ;
}

void DanceGroupCeremonyLog::DoDecode(CParamPool &IOBuff)
{
    m_roomBaseLog.DoDecode(IOBuff);
    unsigned short len = IOBuff.GetUShort();

    for (unsigned short i = 0; i < len; ++i)
    {
        CeremonyPraiseLogInfo v;
        v.DoDecode(IOBuff);
        m_voteInfo.push_back(v);
    }

    return ;
}


DanceGroupCeremonyLogs::DanceGroupCeremonyLogs(){}
DanceGroupCeremonyLogs::~DanceGroupCeremonyLogs(){}


DanceGroupCeremonyLogsProcessor::DanceGroupCeremonyLogsProcessor(){}
DanceGroupCeremonyLogsProcessor::~DanceGroupCeremonyLogsProcessor(){}

// 把一条日志(一次仪式的所有日志)添加到整个map中
void DanceGroupCeremonyLogsProcessor::InsertCeremonyLog(unsigned int groupID, DanceGroupCeremonyLog &log, 
    std::map<unsigned, DanceGroupCeremonyLogs> &logMap)
{
    // 查询是否存在这个群组的
    DanceGroupCeremonyLogs *groupLogs = NULL;
    std::map<unsigned, DanceGroupCeremonyLogs>::iterator iter = logMap.find(groupID);
    if (iter != logMap.end())    
    {
        groupLogs = &(iter->second);
    }
    else
    {
        iter = logMap.insert(std::make_pair(groupID, DanceGroupCeremonyLogs())).first;
        groupLogs = &(iter->second);
        groupLogs->m_nGroupID = groupID;
    }

    // 按照时间，最新的插入进来：
    groupLogs->m_vecLog.insert(groupLogs->m_vecLog.begin(), log); // 把日志添加在最前边

    return ;
}

void DanceGroupCeremonyLogsProcessor::InsertCeremonyPraiseItem(DanceGroupCeremonyLog &roomLog, CeremonyPraiseLogInfoDb &info)
{
    // 先根据新人找到对应的信息，
    CeremonyPraiseLogInfo *pNewerInfo = NULL;
    for (size_t i = 0; i < roomLog.m_voteInfo.size(); ++i)
    {
        if (roomLog.m_voteInfo[i].m_newerInfo.m_nRoleID == info.m_newerInfo.m_nRoleID)
        {
            pNewerInfo = &(roomLog.m_voteInfo[i]);
            break;
        }
    }
    if (NULL == pNewerInfo)
    {
        roomLog.m_voteInfo.push_back(CeremonyPraiseLogInfo());
        pNewerInfo = &(roomLog.m_voteInfo[roomLog.m_voteInfo.size()-1]);
        pNewerInfo->m_newerInfo = info.m_newerInfo;
    }

    if (0 == info.m_voterInfo.m_nPraiserCount)
    { // 点赞数目为0：
        return ;
    }

    // 找到此人对应的信息，累加
    CeremonyLogVoterInfo *pVoteInfo = NULL;
    for (size_t i = 0; i < pNewerInfo->m_vecVoterInfo.size(); ++i)
    {
        if (pNewerInfo->m_vecVoterInfo[i].m_nPraiserID == info.m_voterInfo.m_nPraiserID)
        {
            pVoteInfo = &(pNewerInfo->m_vecVoterInfo[i]);
            break;
        }
    }
    if (NULL == pVoteInfo)
    {
        pNewerInfo->m_vecVoterInfo.push_back(CeremonyLogVoterInfo());
        pVoteInfo = &(pNewerInfo->m_vecVoterInfo[pNewerInfo->m_vecVoterInfo.size() - 1]);
        *pVoteInfo = info.m_voterInfo;
    }
    else
    {
        pVoteInfo->m_nPraiserCount += info.m_voterInfo.m_nPraiserCount;
    }

    return ;
}


CeremonyNewMember::CeremonyNewMember()
    : m_nRoleID(0)
    , m_nSeq(0)
    , m_nPraisedNum(0)
{

}

CeremonyNewMember::~CeremonyNewMember(){}



CeremonyNormalMember::CeremonyNormalMember()
    : m_nRoleID(0)
    , m_nPraiseNum(0)
{

}

CeremonyNormalMember::~CeremonyNormalMember(){}

