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
	RegMessageMap(); // ע����Ϣ��
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
    const unsigned long kUpdateInterval = 1000; // ���ʱ�䣬����
    static unsigned long lastCheckElapsed = 0;
    static unsigned int nApplyDuration = CDanceGroupDataMgr::Instance().m_nCeremonyApplyDuration;
    static unsigned int nStartDuration = CDanceGroupDataMgr::Instance().m_nCeremonyStartDuration;
    static std::vector<unsigned int> vecCloseRoom;
    static std::vector<unsigned int> vecErrorRoom;

    lastCheckElapsed += lTimeElapsed;
    if (lastCheckElapsed < kUpdateInterval)
        return ;// ���ü��

    lastCheckElapsed = 0; // �����ۼ�

    unsigned int now = (unsigned int)time(NULL);

    for (CeremonyRoomMap::iterator iter = m_mapCeremonyRoom.begin(); iter != m_mapCeremonyRoom.end(); ++iter)
    {
        if (iter->second.GetState() == ECeremonyState_Prepare)
        {
            if (iter->second.GetApplyTime() + nApplyDuration > now)
                continue;

            GameMsg_S2C_StartCeremonyRes brodMsg;

            brodMsg.m_nErrorType = (unsigned char)iter->second.Play(brodMsg.m_vecRookieInfos); // ������ʽ
            if (ECeremonyErrType_OK == brodMsg.m_nErrorType)
            {
                iter->second.RoomBroadcast(brodMsg);
            }
            else
            { // �������˾�ֱ�ӽ���ر�����
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
            iter->second.MailRoomPrize(); // ���ͷ��佱��
            vecCloseRoom.push_back(iter->first);
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "error ceremonyRoom state[%u] of danceGroup[%u]", iter->second.GetState(), iter->first);
            // �ر��������
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

    // ͳ�����еķ�����Ϣ�����͸�group ͬ����
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
    // ȡ������
    CRoleEntity* pRole = CPlayerManager::Instance().GetEntityByRoleID(roleID);
    if (NULL == pRole)
        return ; // 
    CRoleDanceGroup *pRoleGroup = pRole->GetComponent<CRoleDanceGroup>();
    if (NULL == pRoleGroup)
        return ; // û������

    // ȡ��������ʽ
    CeremonyRoom *pRoom = GetCeremonyRoom(pRoleGroup->GetDanceGroupID());
    if (NULL == pRoom)
        return ; // ����û�з���

    int ret = pRoom->LeaveRoom(roleID);

    if (ret == ECeremonyErrType_OK)
    { // �㲥���У���֪��
        GameMsg_S2C_OtherPlayerQuitCeremony bmsg;
        bmsg.m_nPlayerID = roleID;
        pRoom->RoomBroadcast(bmsg); // �˳��㲥
    }

    return ; // �˳����
}

void CeremonyRoomManager::OnQuitDanceGroup(unsigned int nRoleID, unsigned nDanceGroupID, int nReason)
{
    // ȡ��������ʽ
    CeremonyRoom *pRoom = GetCeremonyRoom(nDanceGroupID);
    if (NULL == pRoom)
        return ; // ����û�з���

    if (pRoom->IsRoomStart())
        return; // �����Ѿ���������������

    // �����˿����뿪���䣬������Ҫ���Ƿ������˵��ж�
    if (!pRoom->IsCreator(nRoleID) && !pRoom->HasMember(nRoleID))
        return ;


    if (pRoom->IsCreator(nRoleID))
    { // ��ʼ�����ˣ��ȹ㲥����رգ�Ȼ��رշ���
        GameMsg_S2C_CeremonyRoomStartFail bMsg;
        bMsg.m_nFailReason = ECeremonyErrType_CreatorIsKicked;
        pRoom->RoomBroadcast(bMsg); // �㲥����ر�
        pRoom->ComitRoomLog();
        CloseRoom(nDanceGroupID, 0); // ����ر�
    }
    else
    {
        int ret = pRoom->LeaveRoom(nRoleID);
        if (ret == ECeremonyErrType_OK)
        { // �㲥���У���֪��
            GameMsg_S2C_OtherPlayerQuitCeremony bmsg;
            bmsg.m_nPlayerID = nRoleID;
            pRoom->RoomBroadcast(bmsg); // �˳��㲥
        }

        CRoleEntity* pRole = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
        if (NULL != pRole)
        {
            // ֪ͨ���߳�����
            GameMsg_S2C_QuitCeremonyRes kickedNotify; // 
            kickedNotify.m_nErrorType = ECeremonyErrType_OK;
            kickedNotify.m_nQuitType = (unsigned char)nReason;
            kickedNotify.m_nKickerID = 0; // û����
            kickedNotify.m_nQuitTo = (char)ECeremonyRoomQuitTo_World;

            pRole->SendPlayerMsg(&kickedNotify);
        }
    }

    return ; // �˳����
}


void CeremonyRoomManager::TryOpenCeremonyRoom(unsigned int roleID)
{
	// ����IDȡ��component.
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
		{ // û�оٰ���ʽ��Ȩ��
			retType = ECeremonyErrType_RightNotEnough;
			break;
		}


		// ȡ����ʽ���д���
		CDanceGroupBaseInfo& baseInfo = pDanceGroup->GetDanceGroupInfo()->m_DGBaseInfo; // ������
		// �����Ѿ��ٰ�Ĵ���
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

		// ��⵱ǰ�Ƿ���������
// 		if (m_applyRequest.find(pDanceGroup->GetDanceGroupID()) != m_applyRequest.end())
// 		{ // �Ѿ�����������
// 			retType = ECeremonyErrType_OnApply; 
// 			break;
// 		}
//
//		m_applyRequest.insert(std::make_pair(pDanceGroup->GetDanceGroupID(), roleID)); // ��¼����
		// ִ�����룺
		ApplyOpenRoom(pDanceGroup->GetDanceGroupID(), roleID);

        WriteLog(LOGLEVEL_DEBUG, "group[%d] try open cremonyroom send group apply sucess", pDanceGroup->GetDanceGroupID());

	} while (0);

	if (retType != ECeremonyErrType_OK)
	{ // �������ɹ�
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

    // ��Ϊ����������п��ǣ����groupServer �ܾ���������ֱ�ӻظ��ͻ��ˡ������ܻظ�groupServer����ʱgroupServerû�м�¼����
    // ���groupServer �������������ظ�groupServer������������ͬʱroleҲ���ߣ���ظ�role���������
    if (rmsg.m_nErrorType != ECeremonyErrType_OK)
    { // ���ܴ�����ֱ�ӻظ��ͻ���
        if (NULL != pRole)
        {
            GameMsg_S2C_RequestApplyCeremonyRes retClient; // 
            retClient.m_nErrorType = rmsg.m_nErrorType;
            pRole->SendPlayerMsg(&retClient);
        }
        return ;
    }
    else
    { // ���Դ���������Ҫ�ظ�group�� 
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

            CDanceGroupBaseInfo& baseInfo = pDanceGroup->GetDanceGroupInfo()->m_DGBaseInfo; // ������

            // �����Ѿ��ٰ�Ĵ���
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

            // �����ɹ�, 
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
            GameMsg_S2G_CeremonyCreateRoomNotify notifyMsg; // �ɹ�����������Ϣ��֪ͨgroup���䴴���ɹ���
            notifyMsg.m_nDanceGroupID = rmsg.m_nDanceGroupID;
            notifyMsg.m_nOldCeremonyTimes = runTimes;
            notifyMsg.m_nLine = g_nLine;
            notifyMsg.m_nCreateTime = (unsigned int)time(NULL);
            notifyMsg.m_strLineName = g_pCirculator->GetServerName();
            SendMsg2GroupServer(&notifyMsg);
        }
        else
        { // ����û�гɹ�
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

// �������
unsigned int CeremonyRoomManager::CeremonyTimes(unsigned int CeremonyTimes, unsigned int stamp, time_t now)
{
    return CeremonyTimes; // game server ����ʱ����ļ�⣬ֻ����groupserver �л�
}

// ���ݾٰ�Ĵ���������Ҫ��Ǯ��
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

	SendMsg2GroupServer(&sendMsg); // �ȴ�group ���ؽ��

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
	// ֪ͨgroup�����˷���ر�
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

	// �رո÷���
    m_mapCeremonyRoom.erase(danceGroupID); // �ر����������

    WriteLog(LOGLEVEL_DEBUG, "group[%u] ceremony room closed.", danceGroupID);

    return ;
}

int CeremonyRoomManager::CeremonyRoomNum() const
{
    return m_mapCeremonyRoom.size();
}


