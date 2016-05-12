#include "ceremonyRoomManager.h"
#include "ceremonyRoomHead.h"
#include "ceremonyRoom.h"
#include "../../socket/Log.h"
#include "../../socket/GameMsg_Map.h"
#include "GameMsg_CeremonyRoom.h"
#include "../dancegroup/DanceGroupMsgDef.h"
#include "../../datastructure/RoleEntity.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../logic/LogicCirculator.h"
#include "../../datastructure/DanceGroupDataMgr.h"
#include "../mall/CurrencyType.h"
#include "../mall/Currency.h"
#include "../dancegroup/GameMsg_G2S_DanceGroup.h"

extern int g_nLine;
extern CLogicCirculator* g_pCirculator;
CeremonyRoomManager::CeremonyRoomManager()
	: GameMsg_Processor(true, true)
{

}

CeremonyRoomManager::~CeremonyRoomManager()
{

}

CeremonyRoomManager& CeremonyRoomManager::Instance()
{
	static CeremonyRoomManager mgr;
	
	return mgr;
}

void CeremonyRoomManager::Initialize()
{
	RegMessageMap(); // 注册消息啊
}

void CeremonyRoomManager::RegMessageMap()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_G2S_CeremonyApplyCreateRet);

    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_RequestCeremonyRecordRes);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_RequestCeremonyPraiseRes);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_SyncCeremonyState);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_BroadcastMemberKicked);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_BroadcastMemberQuit);

    

	RegMsgProcessor(MSG_G2S_CeremonyApplyCreateRet, &CeremonyRoomManager::OnCeremonyApplyCreateRet);

    RegMsgProcessor(MSG_S2C_RequestCeremonyRecordRes, &CeremonyRoomManager::OnDirectToClient);
    RegMsgProcessor(MSG_S2C_RequestCeremonyPraiseRes, &CeremonyRoomManager::OnDirectToClient);
    RegMsgProcessor(MSG_S2C_SyncCeremonyState, &CeremonyRoomManager::OnDirectToClient);
    RegMsgProcessor(MSG_G2S_BroadcastMemberKicked, &CeremonyRoomManager::OnBroadcastMemberKicked);
    RegMsgProcessor(MSG_G2S_BroadcastMemberQuit, &CeremonyRoomManager::OnBroadcastMemberQuit);
}

void CeremonyRoomManager::OnUpdate(const unsigned long &lTimeElapsed)
{
    const unsigned long kUpdateInterval = 1000; // 间隔时间，毫秒
    static unsigned long lastCheckElapsed = 0;
    static unsigned int nApplyDuration = CDanceGroupDataMgr::Instance().m_nCeremonyApplyDuration;
    static unsigned int nStartDuration = CDanceGroupDataMgr::Instance().m_nCeremonyStartDuration;
    static std::vector<unsigned int> vecCloseRoom;
    static std::vector<unsigned int> vecErrorRoom;

    lastCheckElapsed += lTimeElapsed;
    if (lastCheckElapsed < kUpdateInterval)
        return ;// 不用检测

    lastCheckElapsed = 0; // 继续累计

    unsigned int now = (unsigned int)time(NULL);

    for (CeremonyRoomMap::iterator iter = m_mapCeremonyRoom.begin(); iter != m_mapCeremonyRoom.end(); ++iter)
    {
        if (iter->second.GetState() == ECeremonyState_Prepare)
        {
            if (iter->second.GetApplyTime() + nApplyDuration > now)
                continue;

            GameMsg_S2C_StartCeremonyRes brodMsg;

            brodMsg.m_nErrorType = (unsigned char)iter->second.Play(brodMsg.m_vecRookieInfos); // 举行仪式
            if (ECeremonyErrType_OK == brodMsg.m_nErrorType)
            {
                iter->second.RoomBroadcast(brodMsg);
            }
            else
            { // 开启不了就直接进入关闭流程
                GameMsg_S2C_CeremonyRoomStartFail fail_msg;
                fail_msg.m_nFailReason = brodMsg.m_nErrorType;
                iter->second.RoomBroadcast(fail_msg);
                iter->second.ComitRoomLog();
                vecCloseRoom.push_back(iter->first);
            }
        }
        else if (iter->second.GetState() == ECeremonyState_Start)
        {
            if (iter->second.GetStartTime() + nStartDuration > now)
                continue; 
            iter->second.ComitRoomLog();
            iter->second.MailRoomPrize(); // 发送房间奖励
            vecCloseRoom.push_back(iter->first);
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "error ceremonyRoom state[%u] of danceGroup[%u]", iter->second.GetState(), iter->first);
            // 关闭这个房间
            vecErrorRoom.push_back(iter->first);
        }
    }

    if (!vecCloseRoom.empty())
    {
        for (size_t i = 0; i < vecCloseRoom.size(); ++i)
        {
            CloseRoom(vecCloseRoom[i], 0);
        }

        vecCloseRoom.clear();
    }

    if (!vecErrorRoom.empty())
    {
        for (size_t i = 0; i < vecErrorRoom.size(); ++i)
        {
            CloseRoom(vecErrorRoom[i], 0);
        }
        vecErrorRoom.clear();
    }
}

void CeremonyRoomManager::OnGroupServerConnected()
{
//    WriteLog(LOGLEVEL_DEBUG, "GameServer: OnGroupServerConnected");

    // 统计所有的房间信息，发送给group 同步。
    GameMsg_S2G_CeremonySyncRoomInfo syncMsg;
    syncMsg.m_nLine = g_nLine;
    syncMsg.m_strLineName = g_pCirculator->GetServerName();

    for (CeremonyRoomMap::iterator iter = m_mapCeremonyRoom.begin(); iter != m_mapCeremonyRoom.end(); ++iter)
    {
        CeremonyRoomSyncInfo roomInfo;
        roomInfo.m_nRoomID = iter->first;
        roomInfo.m_state = iter->second.GetState();
        roomInfo.m_createTime = iter->second.GetApplyTime();
        syncMsg.m_vecRooms.push_back(roomInfo);

    }

    SendMsg2GroupServer(&syncMsg);

    return ;
}


void CeremonyRoomManager::OnLogout(unsigned int roleID)
{
    // 取得舞团
    CRoleEntity* pRole = CPlayerManager::Instance().GetEntityByRoleID(roleID);
    if (NULL == pRole)
        return ; // 
    CRoleDanceGroup *pRoleGroup = pRole->GetComponent<CRoleDanceGroup>();
    if (NULL == pRoleGroup)
        return ; // 没有舞团

    // 取得入团仪式
    CeremonyRoom *pRoom = GetCeremonyRoom(pRoleGroup->GetDanceGroupID());
    if (NULL == pRoom)
        return ; // 舞团没有房间

    int ret = pRoom->LeaveRoom(roleID);

    if (ret == ECeremonyErrType_OK)
    { // 广播所有，告知：
        GameMsg_S2C_OtherPlayerQuitCeremony bmsg;
        bmsg.m_nPlayerID = roleID;
        pRoom->RoomBroadcast(bmsg); // 退出广播
    }

    return ; // 退出检测
}

void CeremonyRoomManager::OnQuitDanceGroup(unsigned int nRoleID, unsigned nDanceGroupID, int nReason)
{
    // 取得入团仪式
    CeremonyRoom *pRoom = GetCeremonyRoom(nDanceGroupID);
    if (NULL == pRoom)
        return ; // 舞团没有房间

    if (pRoom->IsRoomStart())
        return; // 房间已经开启，则不做处理

    // 申请人可以离开房间，所以需要做是否申请人的判断
    if (!pRoom->IsCreator(nRoleID) && !pRoom->HasMember(nRoleID))
        return ;


    if (pRoom->IsCreator(nRoleID))
    { // 创始人走了，先广播房间关闭，然后关闭房间
        GameMsg_S2C_CeremonyRoomStartFail bMsg;
        bMsg.m_nFailReason = ECeremonyErrType_CreatorIsKicked;
        pRoom->RoomBroadcast(bMsg); // 广播房间关闭
        pRoom->ComitRoomLog();
        CloseRoom(nDanceGroupID, 0); // 房间关闭
    }
    else
    {
        int ret = pRoom->LeaveRoom(nRoleID);
        if (ret == ECeremonyErrType_OK)
        { // 广播所有，告知：
            GameMsg_S2C_OtherPlayerQuitCeremony bmsg;
            bmsg.m_nPlayerID = nRoleID;
            pRoom->RoomBroadcast(bmsg); // 退出广播
        }

        CRoleEntity* pRole = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
        if (NULL != pRole)
        {
            // 通知被踢出的人
            GameMsg_S2C_QuitCeremonyRes kickedNotify; // 
            kickedNotify.m_nErrorType = ECeremonyErrType_OK;
            kickedNotify.m_nQuitType = (unsigned char)nReason;
            kickedNotify.m_nKickerID = 0; // 没人踢
            kickedNotify.m_nQuitTo = (char)ECeremonyRoomQuitTo_World;

            pRole->SendPlayerMsg(&kickedNotify);
        }
    }

    return ; // 退出检测
}


void CeremonyRoomManager::TryOpenCeremonyRoom(unsigned int roleID)
{
	// 根据ID取得component.
	CRoleEntity* pRole = CPlayerManager::Instance().GetEntityByRoleID(roleID);
	ENSURE_WITH_LOG_CMD(NULL != pRole, return, "roleID[%u] miss entity", roleID);
		
	CRoleAttribute* pAttr = pRole->GetComponent<CRoleAttribute>();
	ENSURE_WITH_LOG_CMD(NULL != pAttr, return, "roleID[%u] miss CRoleAttribute", roleID);

	CRoleDanceGroup* pDanceGroup = pRole->GetComponent<CRoleDanceGroup>();
	ENSURE_WITH_LOG_CMD(NULL != pDanceGroup, return, "roleID[%u] miss CRoleDanceGroup", roleID);

	ENSURE_WITH_LOG_CMD(NULL != pDanceGroup->GetDanceGroupInfo(), return, "role[%u] get groupInfo Failed", roleID);

	CDanceGroupMember* selfInfo = pDanceGroup->GetDanceGroupInfo()->GetGroupMember(roleID);
	ENSURE_WITH_LOG_CMD(NULL != selfInfo, return, "role[%u] get group member info Failed", roleID);

    WriteLog(LOGLEVEL_DEBUG, "group[%d] try open cremonyroom", pDanceGroup->GetDanceGroupID());

	unsigned char retType = ECeremonyErrType_OK;
	do 
	{
        if (pDanceGroup->GetDanceGroupInfo()->IsDismissed())
        {
            retType = ECeremonyErrType_GroupDismissed;
            break;
        }

        if (pRole->InSomeRoom())
        {
            retType = ECeremonyErrType_InOtherRoom;
            break;
        }

		if (!CanRunCeremony(selfInfo->m_nTitle))
		{ // 没有举办仪式的权利
			retType = ECeremonyErrType_RightNotEnough;
			break;
		}


		// 取得仪式举行次数
		CDanceGroupBaseInfo& baseInfo = pDanceGroup->GetDanceGroupInfo()->m_DGBaseInfo; // 检测次数
		// 当天已经举办的次数
		unsigned int runTimes = CeremonyTimes(baseInfo.m_nCeremonyTimes, baseInfo.m_nCeremonyStamp, time(NULL));
        int needcoin = CalcApplyCeremonyMoney(runTimes);
        if (needcoin > 0)
        {
            BUILD_CURRENCY_MAP_ONETYPE(currencyMap, CDanceGroupDataMgr::Instance().m_nCeremonyApplyCostType, needcoin);
            if (!pAttr->CanBill(currencyMap))
            {
                retType = ECeremonyErrType_ResNotEnough;
                break;
            }
        }

		// 检测当前是否正在申请
// 		if (m_applyRequest.find(pDanceGroup->GetDanceGroupID()) != m_applyRequest.end())
// 		{ // 已经有人在申请
// 			retType = ECeremonyErrType_OnApply; 
// 			break;
// 		}
//
//		m_applyRequest.insert(std::make_pair(pDanceGroup->GetDanceGroupID(), roleID)); // 记录起来
		// 执行申请：
		ApplyOpenRoom(pDanceGroup->GetDanceGroupID(), roleID);

        WriteLog(LOGLEVEL_DEBUG, "group[%d] try open cremonyroom send group apply sucess", pDanceGroup->GetDanceGroupID());

	} while (0);

	if (retType != ECeremonyErrType_OK)
	{ // 创建不成功
		GameMsg_S2C_RequestApplyCeremonyRes retClient;
		retClient.m_nErrorType = retType;

		pRole->SendPlayerMsg(&retClient);
	}

	return ; //
}

void CeremonyRoomManager::OnCeremonyApplyCreateRet(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_G2S_CeremonyApplyCreateRet &rmsg = (GameMsg_G2S_CeremonyApplyCreateRet &)msg;
    CRoleEntity* pRole = CPlayerManager::Instance().GetEntityByRoleID(rmsg.m_nRoleID);

    WriteLog(LOGLEVEL_DEBUG, "group[%d] try open cremonyroom OnCeremonyApplyCreateRet[%d]", rmsg.m_nDanceGroupID, rmsg.m_nErrorType);

    // 分为两种情况进行考虑：如果groupServer 拒绝创建，则直接回复客户端。（不能回复groupServer（此时groupServer没有记录））
    // 如果groupServer 允许创建，则必须回复groupServer创建结果，如果同时role也在线，则回复role创建结果。
    if (rmsg.m_nErrorType != ECeremonyErrType_OK)
    { // 不能创建，直接回复客户端
        if (NULL != pRole)
        {
            GameMsg_S2C_RequestApplyCeremonyRes retClient; // 
            retClient.m_nErrorType = rmsg.m_nErrorType;
            pRole->SendPlayerMsg(&retClient);
        }
        return ;
    }
    else
    { // 可以创建：必须要回复group。 
        GameMsg_S2C_RequestApplyCeremonyRes retClient; // 
        retClient.m_nErrorType = ECeremonyErrType_Internal;

        int runTimes = 0;
        do 
        {
            if (NULL == pRole)
            {
                break;
            }

            CRoleAttribute *pAttr = pRole->GetComponent<CRoleAttribute>();
            if (NULL == pAttr)
            {
                WriteLog(LOGLEVEL_ERROR, "roleID[%u] miss CRoleAttribute", rmsg.m_nRoleID);
                break;
            }

            CRoleDanceGroup *pDanceGroup = pRole->GetComponent<CRoleDanceGroup>();
            if (NULL == pDanceGroup)
            {
                WriteLog(LOGLEVEL_ERROR, "roleID[%u] miss CRoleDanceGroup", rmsg.m_nRoleID);
                break;
            }

            if (NULL == pDanceGroup->GetDanceGroupInfo())
            {
                WriteLog(LOGLEVEL_ERROR, "roleID[%u] miss DanceGroupInfo", rmsg.m_nRoleID);
                break;
            }

            CDanceGroupBaseInfo& baseInfo = pDanceGroup->GetDanceGroupInfo()->m_DGBaseInfo; // 检测次数

            // 当天已经举办的次数
            runTimes = CeremonyTimes(baseInfo.m_nCeremonyTimes, baseInfo.m_nCeremonyStamp, time(NULL));
            int needcoin = CalcApplyCeremonyMoney(runTimes);
            if (needcoin > 0)
            {
                BUILD_CURRENCY_MAP_ONETYPE(currencyMap, CDanceGroupDataMgr::Instance().m_nCeremonyApplyCostType, needcoin);
                if (!pAttr->CanBill(currencyMap))
                {
                    retClient.m_nErrorType = ECeremonyErrType_ResNotEnough;
                    break;
                }

                pAttr->Bill(currencyMap, EChangeBillCause_ApplyCeremonyRoom, EChangeBindBillCause_ApplyCeremonyRoom, EChangeMoneyCause_ApplyCeremonyRoom);
            }

            // 创建成功, 
            CeremonyRoom room(rmsg.m_nDanceGroupID, pRole);
            m_mapCeremonyRoom.insert(std::make_pair(rmsg.m_nDanceGroupID, room));

            retClient.m_nErrorType = ECeremonyErrType_OK;

            WriteLog(LOGLEVEL_DEBUG, "group[%u] create ceremony room ok.", rmsg.m_nDanceGroupID);
        } while (0);
        
        if (NULL != pRole)
        {
            pRole->SendPlayerMsg(&retClient);
        }

        if (ECeremonyErrType_OK == retClient.m_nErrorType)
        {
            GameMsg_S2G_CeremonyCreateRoomNotify notifyMsg; // 成功创建房间消息（通知group房间创建成功）
            notifyMsg.m_nDanceGroupID = rmsg.m_nDanceGroupID;
            notifyMsg.m_nOldCeremonyTimes = runTimes;
            notifyMsg.m_nLine = g_nLine;
            notifyMsg.m_nCreateTime = (unsigned int)time(NULL);
            notifyMsg.m_strLineName = g_pCirculator->GetServerName();
            SendMsg2GroupServer(&notifyMsg);
        }
        else
        { // 创建没有成功
            CancelApplyCreateCeremony(rmsg.m_nRoleID, rmsg.m_nDanceGroupID); 
        }
    }

    return ;
}

void CeremonyRoomManager::OnDirectToClient(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    CRoleEntity *pRole = CPlayerManager::Instance().GetPlayerByAccount(msg.nAccount);
    if (NULL != pRole)
        pRole->SendPlayerMsg(&msg);

    return ; 
}

void CeremonyRoomManager::OnBroadcastMemberKicked(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_G2S_BroadcastMemberKicked &rmsg = (GameMsg_G2S_BroadcastMemberKicked &)msg;

    OnQuitDanceGroup(rmsg.m_nKickedRoleID, rmsg.m_nGroupID, ECeremonyRoomQuitType_KickedFromDanceGroup);
    return ;
}

void CeremonyRoomManager::OnBroadcastMemberQuit(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_G2S_BroadcastMemberQuit &rmsg = (GameMsg_G2S_BroadcastMemberQuit &)msg;

    OnQuitDanceGroup(rmsg.m_nQuitRoleID, rmsg.m_nGroupID, ECeremonyRoomQuitType_ExitDanceGroup);
    return ;
}


void CeremonyRoomManager::CancelApplyCreateCeremony(unsigned int nRoleID, unsigned int nDanceGroupID)
{
    GameMsg_S2G_CeremonyApplyCreateCancel sendMsg;
    sendMsg.m_nCreateID = nRoleID;
    sendMsg.m_nDanceGroupID = nDanceGroupID;
    SendMsg2GroupServer(&sendMsg);

    return ;
}

// 计算次数
unsigned int CeremonyRoomManager::CeremonyTimes(unsigned int CeremonyTimes, unsigned int stamp, time_t now)
{
    return CeremonyTimes; // game server 不做时间戳的检测，只根据groupserver 切换
}

// 根据举办的次数计算需要的钱数
unsigned int CeremonyRoomManager::CalcApplyCeremonyMoney(unsigned int times)
{
    if (times < CDanceGroupDataMgr::Instance().m_nCeremonyFreeApplyTimes)
        return 0;
    else 
        return CDanceGroupDataMgr::Instance().m_nCeremonyApplyCostNum;
}



void CeremonyRoomManager::ApplyOpenRoom(unsigned int danceGroupID, unsigned int roleID)
{
	GameMsg_S2G_CeremonyApplyCreate sendMsg;
	sendMsg.m_nDanceGroupID = danceGroupID;
	sendMsg.m_nRoleID = roleID;
    sendMsg.m_nLine = g_nLine;

	SendMsg2GroupServer(&sendMsg); // 等待group 返回结果

	return ; 
}


CeremonyRoom* CeremonyRoomManager::GetCeremonyRoom(unsigned int danceGroupID)
{
	CeremonyRoomMap::iterator iter = m_mapCeremonyRoom.find(danceGroupID);
	if (m_mapCeremonyRoom.end() == iter)
		return NULL;

	return &(iter->second); 
}

void CeremonyRoomManager::CloseRoom(unsigned int danceGroupID, unsigned char reason)
{
	// 通知group所有人房间关闭
    GameMsg_S2G_CeremonyCloseRoomNotify groupMsg;
    groupMsg.m_nCloseReason = reason;
    groupMsg.m_nDanceGroupID = danceGroupID;
    SendMsg2GroupServer(&groupMsg);

    CeremonyRoomMap::iterator it = m_mapCeremonyRoom.find(danceGroupID);
    if (it == m_mapCeremonyRoom.end() )
    {
        return;
    }
    
    it->second.CloseRoom();

	// 关闭该房间
    m_mapCeremonyRoom.erase(danceGroupID); // 关闭了这个房间

    WriteLog(LOGLEVEL_DEBUG, "group[%u] ceremony room closed.", danceGroupID);

    return ;
}

int CeremonyRoomManager::CeremonyRoomNum() const
{
    return m_mapCeremonyRoom.size();
}


