#include "CeremonyBriefManager.h"
#include "../../socket/Log.h"
#include "../../socket/GameMsg_Map.h"
#include "GameMsg_CeremonyRoom.h"
#include "../dancegroup/DanceGroupMsgDef.h"
#include "../group/Global.h"
#include "ceremonyRoomHead.h"
#include "../dancegroup/DanceGroupProcessor.h"
#include "../../socket/QueryManagerBase.h"
#include "../libServerFrame/QueryManager.h"
#include "../util/CommonFunc.h"
#include "../../datastructure/DanceGroupDataMgr.h"
#include "../../socket/Formatter.h"

extern CQueryManager* g_pQueryMgr;

CeremonyBrief::CeremonyBrief()
    : m_nDanceGroupID(0)
    , m_state(0) //
    , m_line(0)				// 所在线的编号
    , m_createTime(0)		// 仪式举办时间
{

}

CeremonyBrief::~CeremonyBrief()
{

}

CeremonyApplyInfo::CeremonyApplyInfo()
    : m_nDanceGroupID(0)
    , m_nCreaterID(0)
    , m_applyTime(0) // 需要做倒计时
{

}

CeremonyApplyInfo::~CeremonyApplyInfo()
{

}

CeremonyBriefManager::CeremonyBriefManager()
	: GameMsg_Processor(true, true)
    , m_bLoadLogComplete(false)
{

}

CeremonyBriefManager::~CeremonyBriefManager()
{

}

CeremonyBriefManager& CeremonyBriefManager::Instance()
{
	static CeremonyBriefManager mgr;
	return mgr;
}

// 跟舞团用一个共同的loadcomplete.确保在舞团数据加载之前加载
void CeremonyBriefManager::Initialize()
{
    RegisterMessageBeforeLoad();
    LoadDbInfo();
	return ;
}

void CeremonyBriefManager::InitializeAfterLoadDanceGroup()
{
    RegisterMessageAfterLoad(); 
}

void CeremonyBriefManager::OnPlayerLogin(CGroupRoleEntity & role)
{
    SendCeremonyState(role);
    return ;
}

void CeremonyBriefManager::OnPlayerEnterDanceGroup(CGroupRoleEntity & role)
{
    SendCeremonyState(role);
    return ;
}

void CeremonyBriefManager::SendCeremonyState(CGroupRoleEntity & role)
{
    GameMsg_S2C_SyncCeremonyState msg;

    CeremonyBriefMap::iterator iter = m_ceremonyBriefMap.find(role.GetDanceGroupID());
    if (m_ceremonyBriefMap.end() == iter)
    { // 没有舞团
        msg.m_nCeremonyState = ECeremonyState_None;
        msg.m_nLineID = 0;
        msg.m_strLineName = "";
    }
    else
    {
        msg.m_nCeremonyState = iter->second.m_state;
        msg.m_nLineID = iter->second.m_line;
        msg.m_strLineName = iter->second.m_strLineName;
    }

    role.SendPlayerMsg(&msg);

    return ;
}


// 更新
void CeremonyBriefManager::OnUpdate(time_t tNow)
{
    static unsigned int nLastCheckTime = 0;
    const unsigned int kUpdateInterval = 3; // 三秒钟检测一次
    do
    {
        if (tNow - nLastCheckTime < kUpdateInterval) // 流逝时间不足三秒，不检测
            break;

        // 检测申请队列，去除超时的申请（理论上不存在）
        while (!m_ceremonyApplyInfo.empty())
        {
            if (tNow > (time_t)m_ceremonyApplyInfo[0].m_applyTime)
            {
                WriteLog(LOGLEVEL_ERROR, "dancegoup[%u] ceremony apply is time out...",
                    m_ceremonyApplyInfo[0].m_nDanceGroupID); // 记录超时
                m_ceremonyApplyInfo.erase(m_ceremonyApplyInfo.begin()); // 清理掉第一个元素
            }
            else
            {
                break; // 没有超时的
            }
        }

        unsigned int nOldTime = nLastCheckTime;
        nLastCheckTime = (unsigned int)tNow;

        if (IsSameDay(nOldTime, nLastCheckTime)) // 当前时间和上次检测时间是同一天，不处理
            break;

        WriteLog(LOGLEVEL_DEBUG, "reset ceremony applyTimes");

        // 切天了。需要广播。不写数据库哦
        CDanceGroupProcessor::Instance().ZeroResetCermonyApplyTimes(tNow);
    } while (0);

    return ;
}

void CeremonyBriefManager::RegisterMessageBeforeLoad()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CeremonySyncRoomInfo);
    RegMsgProcessor(Msg_S2G_CeremonySyncRoomInfo, &CeremonyBriefManager::OnCeremonySyncRoomInfo); //

    RegQueryProcessor(QUERY_DanceGroup_CeremonyLoadLog, &CeremonyBriefManager::OnCeremonyLoadLog);
}


void CeremonyBriefManager::RegisterMessageAfterLoad()
{
//    WriteLog(LOGLEVEL_DEBUG, "Group: RegisterMessageAfterLoad");

    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CeremonyApplyCreate);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CeremonyCreateRoomNotify);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CeremonyApplyCreateCancel);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CeremonyCloseRoomNotify);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CeremonyRoomStateChange);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CeremonyRoomNewMember);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CeremonyRoomLogsStart);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CeremonyRoomLogsItem);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CeremonyRoomLogsEnd);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestCeremonyRecord);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestCeremonyPraise);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CeremonyRoomStatics);

    RegMsgProcessor(MSG_S2G_CeremonyApplyCreate, &CeremonyBriefManager::OnCeremonyApplyCreate);
    RegMsgProcessor(MSG_S2G_CeremonyCreateRoomNotify, &CeremonyBriefManager::OnCeremonyCreateRoomNotify);
    RegMsgProcessor(MSG_S2G_CeremonyApplyCreateCancel, &CeremonyBriefManager::OnCeremonyApplyCreateCancel);
    RegMsgProcessor(MSG_S2G_CeremonyCloseRoomNotify, &CeremonyBriefManager::OnCeremonyCloseRoomNotify);
    RegMsgProcessor(MSG_S2G_CeremonyRoomStateChange, &CeremonyBriefManager::OnCeremonyRoomStateChange);
    RegMsgProcessor(MSG_S2G_CeremonyRoomNewMember, &CeremonyBriefManager::OnCeremonyRoomNewMember);
    RegMsgProcessor(MSG_S2G_CeremonyRoomLogsStart, &CeremonyBriefManager::OnCeremonyRoomLogsStart);
    RegMsgProcessor(MSG_S2G_CeremonyRoomLogsItem, &CeremonyBriefManager::OnCeremonyRoomLogsItem);
    RegMsgProcessor(MSG_S2G_CeremonyRoomLogsEnd, &CeremonyBriefManager::OnCeremonyRoomLogsEnd);
    RegMsgProcessor(MSG_C2S_RequestCeremonyRecord, &CeremonyBriefManager::OnRequestCeremonyRecord);
    RegMsgProcessor(MSG_C2S_RequestCeremonyPraise, &CeremonyBriefManager::OnRequestCeremonyPraise);
    RegMsgProcessor(MSG_S2G_CeremonyRoomStatics, &CeremonyBriefManager::OnCeremonyRoomStatics);

    RegQueryProcessor(QUERY_DanceGroup_CeremonyWriteLog, &CeremonyBriefManager::OnCeremonyWriteLog);
}



// 日志加载
void CeremonyBriefManager::LoadDbInfo()
{
    g_pQueryMgr->AddQuery(QUERY_DanceGroup_CeremonyLoadLog, 0, &m_logMap); // 把所有日志加载进来
}

void CeremonyBriefManager::OnCeremonySyncRoomInfo(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
//    WriteLog(LOGLEVEL_DEBUG, "Group: OnCeremonySyncRoomInfo");
    // 接受同步请求：
    GameMsg_S2G_CeremonySyncRoomInfo &rmsg = (GameMsg_S2G_CeremonySyncRoomInfo &)msg;

    std::vector<unsigned int> vecRemoveRoom;
    for (CeremonyBriefMap::iterator iter = m_ceremonyBriefMap.begin(); iter != m_ceremonyBriefMap.end(); ++iter)
    {
        if (iter->second.m_line == rmsg.m_nLine)
            vecRemoveRoom.push_back(iter->second.m_nDanceGroupID);
    }

    // 清除掉本线所有
    for (size_t i = 0; i < vecRemoveRoom.size(); ++i)
    {
        m_ceremonyBriefMap.erase(vecRemoveRoom[i]); 
    }

    // 添加：
    for (std::vector<CeremonyRoomSyncInfo>::iterator iter = rmsg.m_vecRooms.begin(); iter != rmsg.m_vecRooms.end(); ++iter)
    {
        if (m_ceremonyBriefMap.find(iter->m_nRoomID) != m_ceremonyBriefMap.end())
        {
            WriteLog(LOGLEVEL_ERROR, "dumplicated ceremony room[%u] on sync roomInfo.", iter->m_nRoomID);
            continue; // 重复的就不添加了。
        }

        CeremonyBrief brief;
        brief.m_nDanceGroupID = iter->m_nRoomID;
        brief.m_state = iter->m_state;
        brief.m_line = rmsg.m_nLine;
        brief.m_createTime = iter->m_createTime;
        brief.m_strLineName = rmsg.m_strLineName;
        m_ceremonyBriefMap.insert(std::make_pair(iter->m_nRoomID, brief));
    }

    return ;
}


void CeremonyBriefManager::OnCeremonyApplyCreate(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    ENSURE_WITH_LOG_CMD(m_bLoadLogComplete, return, "pls wait laod ceremony log complete.");

	GameMsg_S2G_CeremonyApplyCreate &rmsg = (GameMsg_S2G_CeremonyApplyCreate &)msg;

	GameMsg_G2S_CeremonyApplyCreateRet retMsg;
	retMsg.m_nErrorType = ECeremonyErrType_OK;
    retMsg.m_nRoleID = rmsg.m_nRoleID;
    retMsg.m_nDanceGroupID = rmsg.m_nDanceGroupID;

    WriteLog(LOGLEVEL_DEBUG, "group[%u] open ceremony on group check, slot:%d", rmsg.m_nDanceGroupID, slotPeer.GetSlot());

	do 
	{
		if (HasGroupCeremonyApply(rmsg.m_nDanceGroupID))
		{
			retMsg.m_nErrorType = ECeremonyErrType_OnApply; // 有人正在申请仪式
			break; 
		}

		if (HasGroupCeremony(rmsg.m_nDanceGroupID))
		{
			retMsg.m_nErrorType = ECeremonyErrType_Running; // 仪式正在举行
			break;
		}

		CeremonyApplyInfo applyInfo;
		applyInfo.m_applyTime = (unsigned int)time(NULL) + 60; // 超时时间
		applyInfo.m_nDanceGroupID = rmsg.m_nDanceGroupID;
		applyInfo.m_nCreaterID = rmsg.m_nRoleID;
		m_ceremonyApplyInfo.push_back(applyInfo);

	} while (0);

	SendMsg2GameServer(&retMsg, rmsg.m_nLine);

	return;
}

void CeremonyBriefManager::OnCeremonyCreateRoomNotify(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_CeremonyCreateRoomNotify &rmsg = (GameMsg_S2G_CeremonyCreateRoomNotify &)msg;

	// 删除申请锁定
	RemoveFromApply(rmsg.m_nDanceGroupID);

	// 创建新建房间列表
	if (m_ceremonyBriefMap.find(rmsg.m_nDanceGroupID) != m_ceremonyBriefMap.end())
	{
		WriteLog(LOGLEVEL_ERROR, "dumplicate dancegroup[%u] ceremony", rmsg.m_nDanceGroupID);
			
		return ;
	}
	else
	{
		CeremonyBrief brief;
		brief.m_createTime = rmsg.m_nCreateTime; // 创建时间
		brief.m_state = ECeremonyState_Prepare;
		brief.m_nDanceGroupID = rmsg.m_nDanceGroupID;
		brief.m_line = rmsg.m_nLine;
        brief.m_strLineName = rmsg.m_strLineName;
		m_ceremonyBriefMap.insert(std::make_pair(rmsg.m_nDanceGroupID, brief));

        CDanceGroupProcessor::Instance().AddGroupCeremonyTime(rmsg.m_nDanceGroupID, rmsg.m_nOldCeremonyTimes + 1 <= CDanceGroupDataMgr::Instance().m_nCeremonyFreeApplyTimes);
	}
	

	// 广播通知所有舞团玩家
    BroadcastSyncCeremonyState(rmsg.m_nDanceGroupID, ECeremonyState_Prepare, rmsg.m_nLine, rmsg.m_strLineName);

	return ;
}

void CeremonyBriefManager::OnCeremonyApplyCreateCancel(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
	GameMsg_S2G_CeremonyApplyCreateCancel &rmsg = (GameMsg_S2G_CeremonyApplyCreateCancel &)msg;

	RemoveFromApply(rmsg.m_nDanceGroupID); // 取消仪式创建

	return ;
}

void CeremonyBriefManager::OnCeremonyCloseRoomNotify(GameMsg_Base & msg, CSlotPeer & slotPeer)
{ 
	GameMsg_S2G_CeremonyCloseRoomNotify &rmsg = (GameMsg_S2G_CeremonyCloseRoomNotify &)msg;

	// 把舞团记录关闭掉
	CeremonyBriefMap::iterator iter = m_ceremonyBriefMap.find(rmsg.m_nDanceGroupID);
	ENSURE_WITH_LOG_CMD(iter != m_ceremonyBriefMap.end(), return, "faild get danceGroupRoom[%u] room on close CeremonyRoom", rmsg.m_nDanceGroupID);

    BroadcastSyncCeremonyState(rmsg.m_nDanceGroupID, ECeremonyState_None, iter->second.m_line, iter->second.m_strLineName);
	m_ceremonyBriefMap.erase(iter); 

	return ;
}

void CeremonyBriefManager::OnCeremonyRoomStateChange(GameMsg_Base & msg, CSlotPeer & slotPeer)
{ 
	GameMsg_S2G_CeremonyRoomStateChange &rmsg = (GameMsg_S2G_CeremonyRoomStateChange &)msg;

	// 把舞团记录关闭掉
	CeremonyBriefMap::iterator iter = m_ceremonyBriefMap.find(rmsg.m_msg.m_nDanceGroupID);
	ENSURE_WITH_LOG_CMD(iter != m_ceremonyBriefMap.end(), return, "faild get danceGroupRoom[%u] room on state change", rmsg.m_msg.m_nDanceGroupID);

	iter->second.m_state = rmsg.m_msg.m_nNewState;

	// 广播所有玩家，状态变更。
    BroadcastSyncCeremonyState(rmsg.m_msg.m_nDanceGroupID, rmsg.m_msg.m_nNewState, iter->second.m_line, iter->second.m_strLineName);

	return ;
}

void CeremonyBriefManager::OnCeremonyRoomNewMember(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_CeremonyRoomNewMember &rmsg = (GameMsg_S2G_CeremonyRoomNewMember &)msg;

    for (size_t i = 0; i < rmsg.m_vecNewMember.size(); ++i)
    { // 
        CDanceGroupProcessor::Instance().ChangeMemberProperty(rmsg.m_vecNewMember[i], rmsg.m_nGroupID);
    }

    return ;
}


void CeremonyBriefManager::OnCeremonyRoomLogsStart(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    ENSURE_WITH_LOG_CMD(m_bLoadLogComplete, return, "pls wait laod ceremony log complete.");
    GameMsg_S2G_CeremonyRoomLogsStart &rmsg = (GameMsg_S2G_CeremonyRoomLogsStart &)msg;

    ENSURE_WITH_LOG_CMD(NULL == GetBuffCeremonyLog(rmsg.m_nLine, rmsg.m_nLogSeq), return, 
        "dumplicate to write dancegroup[%d] ceremony log", rmsg.m_nGroupID);

    BufferCeremonyLog log;
    log.m_nLine = rmsg.m_nLine;
    log.m_nLogSeq = rmsg.m_nLogSeq;
    log.m_nGroupID = rmsg.m_nGroupID;
    log.m_log.m_roomBaseLog = rmsg.m_roomInfo;

    InsertBuffCeremonyLog(log);

    return ;
}

void CeremonyBriefManager::OnCeremonyRoomLogsItem(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_CeremonyRoomLogsItem &rmsg = (GameMsg_S2G_CeremonyRoomLogsItem &)msg;

    BufferCeremonyLog *pLogBuff = GetBuffCeremonyLog(rmsg.m_nLine, rmsg.m_nLogSeq);
    ENSURE_WITH_LOG_CMD(NULL != pLogBuff, return, "not find dancegroup ceremony log to write item");

    pLogBuff->m_log.m_voteInfo.push_back(rmsg.m_logItem);

    return ;
}

void CeremonyBriefManager::OnCeremonyRoomLogsEnd(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_CeremonyRoomLogsEnd &rmsg = (GameMsg_S2G_CeremonyRoomLogsEnd &)msg;
    BufferCeremonyLog *pLogBuff = GetBuffCeremonyLog(rmsg.m_nLine, rmsg.m_nLogSeq);
    ENSURE_WITH_LOG_CMD(NULL != pLogBuff, return, "not find dancegroup ceremony log to write item");

    
    DanceGroupCeremonyLog *pLog = new DanceGroupCeremonyLog;
    *pLog = pLogBuff->m_log; // 


    // addQuery
    g_pQueryMgr->AddQuery(QUERY_DanceGroup_CeremonyWriteLog, pLogBuff->m_nGroupID, pLog);

    RemoveBuffCeremonyLog(rmsg.m_nLine, rmsg.m_nLogSeq); //

    return ;
}

void CeremonyBriefManager::OnCeremonyLoadLog(QueryBase &rQuery)
{
    m_bLoadLogComplete = true; // 加载日志成功
}


// 数据库写入完毕之后，进入内存
void CeremonyBriefManager::OnCeremonyWriteLog(QueryBase &rQuery)
{
    DanceGroupCeremonyLog *pLog = (DanceGroupCeremonyLog *)rQuery.m_pAttachment;
    unsigned int nDanceGroupID = (unsigned int)rQuery.m_nAttachment;

    ENSURE_WITH_LOG_CMD(NULL != pLog, return, "error to get NULL DanceGroupCeremonyLog of group[%u]", nDanceGroupID);

    DanceGroupCeremonyLogsProcessor::InsertCeremonyLog(nDanceGroupID, *pLog, m_logMap); // 

    delete pLog;
    pLog = NULL;
    rQuery.m_pAttachment = NULL;

    return ;
}

void CeremonyBriefManager::OnRequestCeremonyRecord(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    ENSURE_WITH_LOG_CMD(m_bLoadLogComplete, return, "pls wait load ceremony log complete...");
    GameMsg_C2S_RequestCeremonyRecord &rmsg = (GameMsg_C2S_RequestCeremonyRecord &)msg;

    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pRoleEntity, return, "miss account[%u] CGroupRoleEntity in group server", msg.nAccount);

    GameMsg_S2C_RequestCeremonyRecordRes retMsg;
    retMsg.m_nCurIndex = rmsg.m_nRecordIndex;
    do 
    {
        DanceGroupCeremonyLogMap::iterator iter = m_logMap.find(pRoleEntity->GetDanceGroupID());
        
        if (m_logMap.end() == iter)
        {
            break; // 没有日志
        }

        DanceGroupCeremonyLogs &logs = iter->second;

        unsigned int count = 0;
        unsigned int startStamp = PrevMondayStamp(time(NULL));
        for (; count < (unsigned int)logs.m_vecLog.size(); count++)
        {
            if (logs.m_vecLog[count].m_roomBaseLog.m_logTime < startStamp)
                break;
        }
        retMsg.m_nTotleCount = count;
        if (rmsg.m_nRecordIndex >= 0 && count <= (unsigned int)rmsg.m_nRecordIndex)
        {
            break; // 没有日志
        }
            
        // 转换成客户端可以显示的日志：
        TransSrvLog2ClientLog(logs.m_vecLog[rmsg.m_nRecordIndex], retMsg.m_recordInfo);
    } while (0);


    pRoleEntity->SendPlayerMsg(&retMsg);

    return ; // 
}

void CeremonyBriefManager::OnRequestCeremonyPraise(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    ENSURE_WITH_LOG_CMD(m_bLoadLogComplete, return, "pls wait load ceremony log complete...");

    GameMsg_C2S_RequestCeremonyPraise &rmsg = (GameMsg_C2S_RequestCeremonyPraise &)msg;

    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pRoleEntity, return, "miss account[%u] CGroupRoleEntity in group server", msg.nAccount);

    GameMsg_S2C_RequestCeremonyPraiseRes retMsg;
    retMsg.m_logID = rmsg.m_nLogID;
    retMsg.m_nRoleID = rmsg.m_nRoleID;
    retMsg.m_nCurPage = rmsg.m_nPage;

    do 
    {
        DanceGroupCeremonyLogMap::iterator iter = m_logMap.find(pRoleEntity->GetDanceGroupID());

        if (m_logMap.end() == iter)
        {
            break; // 没有日志
        }

        DanceGroupCeremonyLogs &logs = iter->second;

        DanceGroupCeremonyLog *pLog = NULL;
        // 根据logID 定位
        for (size_t i = 0; i < logs.m_vecLog.size(); ++i)
        {
            if (rmsg.m_nLogID == logs.m_vecLog[i].m_roomBaseLog.m_nLogID)
            {
                pLog = &(logs.m_vecLog[i]);
                break;
            }
        }

        if (NULL == pLog)
        {
            break; // 没有找到
        }

        // 找到这个新人的记录：
        CeremonyPraiseLogInfo *pPraiseLog = NULL;
        for (size_t i = 0; i < pLog->m_voteInfo.size(); ++i)
        {
            if (pLog->m_voteInfo[i].m_newerInfo.m_nRoleID == retMsg.m_nRoleID)
            {
                pPraiseLog = &(pLog->m_voteInfo[i]);
                break;
            }
        }
        if (NULL == pPraiseLog)
            break;


        unsigned int countStart = (rmsg.m_nPage)*rmsg.m_nCount;
        unsigned int countEnd = (rmsg.m_nPage+1)*rmsg.m_nCount;

        if (pPraiseLog->m_vecVoterInfo.size() <= countStart)
            break; // 没有这页
        retMsg.m_nTotleCount = pPraiseLog->m_vecVoterInfo.size();
        
        TransSrvVoteLog2ClientLog(*pPraiseLog, countStart, countEnd, retMsg.m_vecPraiseInfo);
    } while (0);

    pRoleEntity->SendPlayerMsg(&retMsg);

    return ;
}

void CeremonyBriefManager::OnCeremonyRoomStatics(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
//    GameMsg_S2G_CeremonyRoomStatics &rmsg = (GameMsg_S2G_CeremonyRoomStatics &)msg;
    CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);
    ENSURE_WITH_LOG_CMD(NULL != pRole, return, "error to get account[%u] role", msg.nAccount);

    std::map<int, int> mapCeremonyNum;
    for (CeremonyBriefMap::iterator iter = m_ceremonyBriefMap.begin(); iter != m_ceremonyBriefMap.end(); ++iter)
    {
        if (mapCeremonyNum.find(iter->second.m_line) != mapCeremonyNum.end())
        {
            mapCeremonyNum[iter->second.m_line] += 1;
        }
        else
        {
            mapCeremonyNum[iter->second.m_line] = 1; // 一个
        }
    }

    GameMsg_G2S_CeremonyRoomStatics retMsg;
    retMsg.m_strStatics = "statics: ";
    
    for (std::map<int, int>::iterator iter = mapCeremonyNum.begin(); iter != mapCeremonyNum.end(); ++iter)
    {
        std::string tempString;
        Formatter::Format(tempString, "\nline:%d, num:%d", iter->first, iter->second);
        retMsg.m_strStatics += tempString;
    }

    pRole->SendPlayerMsg(&retMsg);
}


bool CeremonyBriefManager::HasGroupCeremonyApply(unsigned int danceGroupID)
{
    for (size_t i = 0; i < m_ceremonyApplyInfo.size(); ++i)
    {
        if (m_ceremonyApplyInfo[i].m_nDanceGroupID == danceGroupID)
            return true;
    }

    return false;
}

bool CeremonyBriefManager::HasGroupCeremony(unsigned int danceGroupID)
{
    return m_ceremonyBriefMap.find(danceGroupID) != m_ceremonyBriefMap.end();
}

// 
bool CeremonyBriefManager::RemoveFromApply(unsigned int danceGroupID)
{
    for (CeremonyApplyInfoVec::iterator iter = m_ceremonyApplyInfo.begin();
        iter != m_ceremonyApplyInfo.end(); ++iter)
    {
        if (iter->m_nDanceGroupID == danceGroupID)
        {
            m_ceremonyApplyInfo.erase(iter);
            break;
        }
    }

    return true;
}


void CeremonyBriefManager::BroadcastSyncCeremonyState(unsigned int groupID, unsigned char newState, int line, std::string strLineName)
{
    // 广播状态变更的消息
    GameMsg_S2C_SyncCeremonyState bmsg;
    bmsg.m_nCeremonyState = newState;
    bmsg.m_nLineID = line;
    bmsg.m_strLineName = strLineName;
    CDanceGroupProcessor::Instance().SendMsg2DanceGroupMember(groupID, 0, bmsg);

    return ;
}


BufferCeremonyLog* CeremonyBriefManager::GetBuffCeremonyLog(int nLine, unsigned int logSeq)
{
    for (size_t i = 0; i < m_bufferLogVec.size(); ++i)
    {
        if (m_bufferLogVec[i].m_nLine == nLine && m_bufferLogVec[i].m_nLogSeq == logSeq)
            return &(m_bufferLogVec[i]);
    }

    return NULL;
}

void CeremonyBriefManager::InsertBuffCeremonyLog(BufferCeremonyLog &log)
{
    m_bufferLogVec.push_back(log); // 直接插入进来

    return ;
}

void CeremonyBriefManager::RemoveBuffCeremonyLog(int nLine, unsigned int logSeq)
{
    for (BufferCeremonyLogVec::iterator iter = m_bufferLogVec.begin(); iter != m_bufferLogVec.end(); ++iter)
    {
        if (iter->m_nLine == nLine && iter->m_nLogSeq == logSeq)
        {
            m_bufferLogVec.erase(iter);
            return ;
        }
    }

    return ;
}


// 服务器存储的日志结构转换成客户端的日志结构
void CeremonyBriefManager::TransSrvLog2ClientLog(DanceGroupCeremonyLog &srvLog, CeremonyRecordInfo &clientLog)
{
    /// 仪式新人点赞记录
    clientLog.m_nLogID = srvLog.m_roomBaseLog.m_nLogID;
    clientLog.m_nApplicantID = srvLog.m_roomBaseLog.m_nCreatorID;
    clientLog.m_strApplicantName = srvLog.m_roomBaseLog.m_strCreatorName;
    clientLog.m_bIsVIP = (srvLog.m_roomBaseLog.m_nCreatorVip > 0 ? true : false);
    clientLog.m_nVIPLevel = srvLog.m_roomBaseLog.m_nCreatorVip;
    clientLog.m_nCeremonyDate = srvLog.m_roomBaseLog.m_logTime; // 日期

    for (size_t i = 0; i < srvLog.m_voteInfo.size(); ++i)
    {
        CeremonyPraiseLogInfo &logInfo = srvLog.m_voteInfo[i];
        CeremonyPraiseGainedInfo cltInfo;
        cltInfo.m_nRoleID = logInfo.m_newerInfo.m_nRoleID;
        cltInfo.m_strRoleName = logInfo.m_newerInfo.m_strRoleName;
        cltInfo.m_bIsVIP = (logInfo.m_newerInfo.m_nVipLv > 0 ? true : false);
        cltInfo.m_nVIPLevel = logInfo.m_newerInfo.m_nVipLv;
        cltInfo.m_nPraiseCount = 0;
        for (size_t j = 0; j < logInfo.m_vecVoterInfo.size(); ++j)
        {
            cltInfo.m_nPraiseCount += (logInfo.m_vecVoterInfo[j].m_nPraiserCount);
        }

        clientLog.m_vecPraiseInfo.push_back(cltInfo);
    }

    return ;
}

void CeremonyBriefManager::TransSrvVoteLog2ClientLog(CeremonyPraiseLogInfo &srvLog, unsigned int startIndex, unsigned int endIndex, std::vector<CeremonyPraiseSendInfo> &vecClientLog)
{
    for (size_t i = (size_t)startIndex; i < srvLog.m_vecVoterInfo.size() && i < (size_t)endIndex; ++i)
    {
        CeremonyPraiseSendInfo info;
        info.m_nRoleID = srvLog.m_vecVoterInfo[i].m_nPraiserID;
        info.m_strRoleName = srvLog.m_vecVoterInfo[i].m_strPraiserName;
        info.m_bIsVIP = (srvLog.m_vecVoterInfo[i].m_nPraiserVipLv > 0 ? true : false);
        info.m_nVIPLevel = srvLog.m_vecVoterInfo[i].m_nPraiserVipLv;
        info.m_nPraiseCount = srvLog.m_vecVoterInfo[i].m_nPraiserCount;
        vecClientLog.push_back(info); 
    }

    return ;
}



