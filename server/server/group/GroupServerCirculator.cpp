#include "../share/StdAfx.h"
#include "Global.h"
#include "GroupServerCirculator.h"
#include "../util/IDCreater.h"
#include "../libServerFrame/Main.h"
#include "../libServerFrame/QueryManager.h"
#include "../share/GameMsg_Server_Connected.h"
#include "../util/CommonFunc.h"
#include "../../socket/GameMsg_Map.h"
#include "../friend/WFriendManager.h"
#include "../Rank/DungeonRankMgr.h"

#include "../login/GameMsg_S2G_Login.h"
#include "../login/GameMsg_S2G_Logout.h"
#include "../chat/GameMsg_G2S_ChatWindow.h"
#include "../GM/GameMsg_G2S_Announce.h"
#include "../vip/GameMsg_S2G_VIP.h"
#include "../activity/GameMsg_S2S_Activities.h"
#include "../share/GameMsg_G2S_AddMCoinByGm.h"
#include "../share/GameMsg_G2S_LockOrUnLockAccountByGm.h"
#include "GameMsg_LBS.h"
#include "../mail/GameMsg_External.h"
#include "../Couple/GameMsg_S2G_Couple.h"
#include "../Couple/GameMsg_G2S_Couple.h"
#include "../share/GameMsg_Forward.h"
#include "../share/GameMsg_LinkCount.h"
#include "../mall/GameMsg_Mall.h"
#include "../item/ItemMsgDefine.h"
#include "../Couple/CoupleMsgDef.h"
#include "../GM/GameMsg_GM_share.h"
#include "../dancegroup/DanceGroupProcessor.h"
#include "../mail/WordMailManager.h"
#include "../lua/InitLua.h"
#include "../mail/MailGiftSystem.h"
#include "../group/LBS.h"
#include <sstream>
#include "../Couple/CoupleProcessor.h"
#include "../GM/GameMsg_G2S_GetRoomListByGM.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../share/GameMsg_S2G_GameServerClosed.h"
#include "../CeremonyRoom/CeremonyBriefManager.h"
#include "../util/NameDeny.h"
#include "../Medal/GameMsg_Gm_medal.h"
#include "../share/GameMsg_GameServerLinkInfo.h"
#include "../dancegroup/BigMamaGroup.h"

#include "../room/GroupRoomPrecessor.h"

#include "../DanceGroupFairyLand/DanceGroupFairyLandProcessor.h"

extern CIni g_Config;
extern int g_nGroup;

extern CQueryManager *g_pQueryMgr;

CIDCreater* g_pIDCreater = NULL;
CGroupServerCirculator* g_pCirculator = NULL;

#define MALL_RELOAD_INTERVAL    5000

CGroupServerCirculator::CGroupServerCirculator()
{
    g_pCirculator = this;
    m_stat.nType = SVRTYPE_GROUP;
    m_pWorldHost = NULL;
    m_pIDCreater = NULL;

    m_nUpdateDelay = 0;
    m_nLastCheckHour = 0;
    m_nGMSlot = -1;

    m_uMallReloadInterval = 0;
    m_uMallReloadTime = 0;
    m_bMallNeedReload = 0;

    srand((unsigned int)time(NULL));
}

CGroupServerCirculator::~CGroupServerCirculator()
{
    delete m_pWorldHost;
    delete m_pIDCreater;
}

bool CGroupServerCirculator::Initialize()
{
    if (g_nGroup < 0)
        g_nGroup = g_Config.GetInt(g_sSection, "group", 0);

    bool bRes = CCirculator::Initialize();
    RegisterMessage();
    RegisterMessageProcessor();

    if (bRes)
    {
        m_pIDCreater = new CIDCreater;
        g_pIDCreater = m_pIDCreater;
    }

    if (bRes)
    {
        CInitLua::GetInstance().InitState();
        string sLuaFile = g_sPath + "data/groupluascript.lua";

        if (!CInitLua::GetInstance().LuaLoadFile(sLuaFile.c_str()))
        {
            bRes = false;
            WriteLog(LOGLEVEL_ERROR, "加载lua脚本文件失败\n");
        }
        else
        {
            WriteLog(LOGLEVEL_DEBUG, "加载lua脚本文件成功\n");
        }
    }

    if (bRes)
    {
        string sConfigIniFile = g_sPath + "data/config.ini";
        if (!g_ServerConfig.Load(sConfigIniFile))
        {
            bRes = false;
            WriteLog(LOGLEVEL_ERROR, "加载config.ini文件失败\n");
        }
        else
        {
            WriteLog(LOGLEVEL_DEBUG, "加载config.ini文件成功\n");
        }
    }

    if (bRes)
    {
        //读入被禁止使用的名字
        char szFile[MAX_PATH];
        g_Config.GetString(g_sSection, "namedeny", szFile, MAX_PATH, "");
        if (szFile[0] != 0 && !NameDeny::Instance().ReadInfoFromFile(g_sPath + szFile))
        {
            WriteLog(LOGLEVEL_ERROR, "Can not open namedeny file for read!\n");
        }
    }

    if (bRes)
    {
        if (!ConfigManager::Instance().InitAllData(g_sPath.c_str(), true))
        {
            LOG_ERROR << "CDataManager.InitAllData() return false!" << std::endl;

            return false;
        }

        m_linkingInfos.Initialize((unsigned short)m_pSocketMgr->GetMaxClientCount());

        m_pWorldHost = new CWorldHost;
        bRes = m_pWorldHost->Initialize();
        if (!bRes)
            WriteLog(LOGLEVEL_ERROR, "Initialize worldhost failed!\n");
    }

    return bRes;
}

void CGroupServerCirculator::RegisterMessage()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_Login);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_Logout);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2S_MallAskFor);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_MallDesire);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_WishComeTrue);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_BlessFriend);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_UpdateVIPInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_BlessingNotify);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GameServerClosed);

    GAMEMSG_REGISTERCREATOR(GameMsg_M2G_SendGroupServerInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2S_GMSendMail);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2M_GMSendMailRes);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2S_QueryRangeGift);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2M_QueryRangeGiftRes);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2S_QueryMultiGift);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2M_QueryMultiGiftRes);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2S_Announce);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2G_LockOrUnLockByGm);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2G_AddMCoinByGm);
    GAMEMSG_REGISTERCREATOR(GameMsg_EX2G_SendMail);
    GAMEMSG_REGISTERCREATOR(GameMsg_EX2G_SendMailGift);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2G_GetServerLine);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2G_GetRoomList);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetRoomListByGMRes);
    GAMEMSG_REGISTERCREATOR(GameMsg_Forward);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2G_SetMallUpdateTime);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2G_SendPrivMCoin);
    GAMEMSG_REGISTERCREATOR(GameMsg_A2M_SendPrivMCoinRet);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2G_GetServerPlayerCountInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_A2G_GetServerPlayerLimitRet);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2A_SetPlayerLimit);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2A_SendBlackDeviceList);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2A_ChangeAccountPassword);
}

void CGroupServerCirculator::RegisterMessageProcessor()
{
    RegMsgProcessor(MSG_SYSTEM_Connect, &CGroupServerCirculator::OnClientConnect);
    RegMsgProcessor(MSG_SYSTEM_Disconnect, &CGroupServerCirculator::OnClientDisconnect);
    RegMsgProcessor(MSG_S2G_Login, &CGroupServerCirculator::OnPlayerLogin);
    RegMsgProcessor(MSG_S2G_Logout, &CGroupServerCirculator::OnPlayerLogout);
    RegMsgCreatorAndProcessor(GameMsg_LinkCount, &CGroupServerCirculator::OnLinkCount);
    RegMsgProcessor(MSG_S2S_MALL_ASKFOR, &CGroupServerCirculator::OnMallAskFor);
    RegMsgProcessor(MSG_S2G_MALLDESIRE, &CGroupServerCirculator::OnMallDesire);
    RegMsgProcessor(MSG_S2G_WishComeTrue, &CGroupServerCirculator::OnWishComeTrue);
    RegMsgProcessor(MSG_S2G_BlessFriend, &CGroupServerCirculator::OnBlessFriend);
    RegMsgProcessor(MSG_S2G_UpdateVIPInfo, &CGroupServerCirculator::OnUpdateVIPInfo);
    RegMsgProcessor(MSG_S2G_BlessingNotify, &CGroupServerCirculator::OnDirectAllClient);
    RegMsgProcessor(MSG_S2G_GameServerClosed, &CGroupServerCirculator::OnGameServerClosed);
    RegMsgCreatorAndProcessor(GameMsg_S2G_GameServerLinkInfo, &CGroupServerCirculator::OnGameServerLinkInfo);

    RegMsgProcessor(MSG_GM_M2G_SendGroupServerInfo, &CGroupServerCirculator::OnGMServerReportInfo);
    RegMsgProcessor(MSG_GM_M2S_SendMail, &CGroupServerCirculator::OnGMSendMail);
    RegMsgProcessor(MSG_GM_S2M_SendMailRes, &CGroupServerCirculator::OnGMSendMailRes);
    RegMsgProcessor(MSG_GM_M2S_QueryRangeGift, &CGroupServerCirculator::OnGMGetActivatedGiftNum);
    RegMsgProcessor(MSG_GM_S2M_QueryRangeGiftRes, &CGroupServerCirculator::OnGMGetActivatedGiftNumRes);
    RegMsgProcessor(MSG_GM_M2S_QueryMultiGift, &CGroupServerCirculator::OnGMGetGiftCodesState);
    RegMsgProcessor(MSG_GM_S2M_QueryMultiGiftRes, &CGroupServerCirculator::OnGMGetGiftCodesStateRes);
    RegMsgProcessor(MSG_GM_M2S_Announce, &CGroupServerCirculator::OnGMSendAnnounce);
    RegMsgProcessor(MSG_GM_M2G_LockOrUnLockAccountByGm, &CGroupServerCirculator::OnGMLockOrUnLockAccount);
    RegMsgProcessor(MSG_GM_M2G_AddMCoinByGm, &CGroupServerCirculator::OnGMAddMCoin);
    RegMsgProcessor(MSG_GM_M2G_GetServerLine, &CGroupServerCirculator::OnGMGetServerLine);
    RegMsgProcessor(MSG_GM_M2G_GetRoomList, &CGroupServerCirculator::OnGMGetRoomList);
    RegMsgProcessor(MSG_S2G_GetRoomListByGMRes, &CGroupServerCirculator::OnServerGetRoomList);
    RegMsgProcessor(MSG_GM_M2G_SetMallUpdateTime, &CGroupServerCirculator::OnGMSetMallUpdateTime);
    RegMsgProcessor(MSG_GM_M2G_SendPrivMCoin, &CGroupServerCirculator::OnGMSendPrivMCoin);
    RegMsgProcessor(MSG_GM_A2M_SendPrivMCoinRet, &CGroupServerCirculator::OnGMSendPrivMCoinRet);
    RegMsgProcessor(MSG_GM_M2G_GetServerPlayerCountInfo, &CGroupServerCirculator::OnGMGetServerPlayerCountInfo);
    RegMsgProcessor(MSG_GM_A2M_GetServerPlayerLimitRet, &CGroupServerCirculator::OnGetServerPlayerLimitRet);
    RegMsgProcessor(MSG_GM_M2A_SetServerPlayerLimit, &CGroupServerCirculator::OnGMSetServerPlayerCountInfo);
    RegMsgProcessor(MSG_GM_M2A_SendBlackDeviceList, &CGroupServerCirculator::OnGMSendBlackDeviceList);
    RegMsgProcessor(MSG_GM_M2A_ChangeAccountPassword, &CGroupServerCirculator::OnGMChangeAccountPassword);


    RegMsgProcessor(MSG_EX2G_SendMail, &CGroupServerCirculator::OnExternalSendMail);
    RegMsgProcessor(MSG_EX2G_SendMailGift, &CGroupServerCirculator::OnExternalSendMailGift);
    RegMsgProcessor(MSG_S2S_Forward, &CGroupServerCirculator::OnForward);

    RegMsgCreatorAndProcessor(GameMsg_S2G_UpdateLBSPositon, &CGroupServerCirculator::OnUpdateLBSPositon);
    RegMsgCreatorAndProcessor(GameMsg_S2G_GetNearbyPlayers, &CGroupServerCirculator::OnGetNearbyPlayers);
    RegMsgCreatorAndProcessor(GameMsg_S2G_CloseLBS, &CGroupServerCirculator::OnCloseLBS);
    RegMsgCreatorAndProcessor(GameMsg_A2M_SendBlackDeviceListRet, &CGroupServerCirculator::OnSendBlackDeviceListRet);
    RegMsgCreatorAndProcessor(GameMsg_A2M_ChangeAccountPasswordRet, &CGroupServerCirculator::OnChangeAccountPasswordRet);
    RegMsgCreatorAndProcessor(GameMsg_M2G_GMDeleteVersionMedal, &CGroupServerCirculator::OnGMDeleteVersionMedal);
    RegMsgCreatorAndProcessor(GameMsg_M2G_GMAddMedal, &CGroupServerCirculator::OnGMAddMedal);
    RegMsgCreatorAndProcessor(GameMsg_S2G_GMDeleteVersionMedal, &CGroupServerCirculator::OnGMDeleteVersionMedalRet);
    RegMsgCreatorAndProcessor(GameMsg_G2M_GMAddMedal, &CGroupServerCirculator::OnGMAddMedalRet);

    //数据库
    RegQueryProcessor(QUERY_LoadCommands, &CGroupServerCirculator::OnLoadCommands);
    RegQueryProcessor(QUERY_External_SendMail, &CGroupServerCirculator::OnDBExternalSendMail);
    RegQueryProcessor(QUERY_External_SendMailGift, &CGroupServerCirculator::OnDBExternalSendMailGift);
}

void CGroupServerCirculator::OnLoadCommands(QueryBase &pQuery)
{
    while (m_listCmd.size() > 0)
    {
        ParseCommand(m_listCmd.front());
        m_listCmd.pop_front();
    }
}

#define BroadcastCmd(pGameMsg, nServer)			for(int i = 0; i < LINES_IN_GROUP; i++){if( (nServer & ( 1 << i) ) ){	m_pSocketMgr->SendGameMsg(pGameMsg, m_anLine[i]);}}

void CGroupServerCirculator::ParseCommand(const CMDINFO& info)
{
    switch (info.nCmdID)
    {
    case CMD_Announce:
    case CMD_AdminMsg:
    case CMD_KickRole:
    case CMD_Shutdown:
    default:
        break;
    }
}


void CGroupServerCirculator::ProcessHeartBeat(int& timeElapse)
{
    CCirculator::ProcessHeartBeat(timeElapse);

    if (m_nHeartBeatCount % 2400 == 0)//每隔60秒读取一次控制指令
    {
        m_pQueryMgr->AddQuery(QUERY_LoadCommands, 0, &m_listCmd);
    }
    //Add code here

    //每天凌晨重新随机每日任务。。。。

    m_nUpdateDelay += timeElapse;
    //五秒钟读一起
    if (m_nUpdateDelay >= 5000)
    {
        m_nUpdateDelay -= 5000;
        time_t _time;
        time(&_time);
        struct tm tTime;
        LocalTime(&_time, &tTime);
        m_nLastCheckHour = tTime.tm_hour;
    }

    m_uMallReloadInterval += timeElapse;

    if (m_bMallNeedReload && m_uMallReloadInterval >= (unsigned int)MALL_RELOAD_INTERVAL)
    {
        m_uMallReloadInterval -= MALL_RELOAD_INTERVAL;

        time_t tNow = time(NULL);

        if (tNow >= (time_t)m_uMallReloadTime)
        {
            m_bMallNeedReload = false;

            SendMallReloadMsg();
        }
    }

    if (m_pWorldHost)
        m_pWorldHost->OnUpdate(timeElapse);
}

void CGroupServerCirculator::ReportStat()
{
    m_stat.nCltCnt = m_pSocketMgr->GetCurClientCount();
    CCirculator::ReportStat();
}

unsigned short CGroupServerCirculator::GetSlotByLine(int nLine) const
{
    return m_linkingInfos.GetSlotByLine(nLine);
}

void SendMsg2GameServer(LPGameMsg_Base pMsg, int nLine)
{
    if (g_pCirculator != NULL && g_pCirculator->m_pSocketMgr != NULL)
    {
        unsigned short nSlot = g_pCirculator->GetSlotByLine(nLine);
        if (nSlot < g_pCirculator->m_pSocketMgr->GetMaxClientCount())
            g_pCirculator->m_pSocketMgr->SendGameMsg(pMsg, nSlot);
    }
}

void SendMsg2GameServerBySlot(LPGameMsg_Base pMsg, unsigned short nSlot)
{
    if (g_pCirculator != NULL && g_pCirculator->m_pSocketMgr != NULL)
        g_pCirculator->m_pSocketMgr->SendGameMsg(pMsg, nSlot);
}

//向游戏服务器广播消息
void BroadcastMsg2GameServer(LPGameMsg_Base pMsg)
{
    BroadcastMsg2GameServer(pMsg, LINES_IN_GROUP);
}

void BroadcastMsg2GameServer(LPGameMsg_Base pMsg, int nLineExcluded)
{
    for (char i = 0; i < LINES_IN_GROUP; i++)
    {
        if (i != nLineExcluded)
        {
            SendMsg2GameServer(pMsg, i);
        }
    }
}

void SendMsgToGMServer(LPGameMsg_Base pMsg, unsigned short nSolt)
{
    if (g_pCirculator != NULL && g_pCirculator->m_pSocketMgr != NULL)
    {
        g_pCirculator->m_nGMSlot = nSolt;
        g_pCirculator->m_pSocketMgr->SendGameMsg(pMsg, nSolt);
    }
}

void SendMsgToGMServer(LPGameMsg_Base pMsg)
{
    if (g_pCirculator != NULL && g_pCirculator->m_pSocketMgr != NULL)
    {
        g_pCirculator->m_pSocketMgr->SendGameMsg(pMsg, (unsigned short)g_pCirculator->m_nGMSlot);
    }
}

void CGroupServerCirculator::OnClientConnect(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_Server_Connected msgConnect;
    msgConnect.nSvrType = SERVER_GROUP;
    msgConnect.nCurPlayerCount = 0;
    msgConnect.strName = "GroupServer";
    slotPeer.SendGameMsg(&msgConnect);

    WriteLog(LOGLEVEL_SYSTEM, "Server connected. Slot: %u.\n", slotPeer.GetSlot());
}

void CGroupServerCirculator::OnClientDisconnect(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    unsigned short nSlot = slotPeer.GetSlot();
    WriteLog(LOGLEVEL_SYSTEM, "Server disconnected. Slot: %u.\n", nSlot);

    int nLine = m_linkingInfos.RemoveServerInfoBySlot(nSlot);
    if (nLine != INVALID_LINE)
    {
        WriteLog(LOGLEVEL_SYSTEM, "GameServer disconnected. Line: %d, slot: %u.\n", nLine, nSlot);
    }
}

void CGroupServerCirculator::OnPlayerLogin(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_Login &rMsg = (GameMsg_S2G_Login &)msg;
    const GameServerLinkInfo *pInfo = m_linkingInfos.GetLinkInfoBySlot(slotPeer.GetSlot());

    int nLine = INVALID_LINE;
    string strLineName;
    if (pInfo != NULL)
    {
        nLine = pInfo->GetLine();
        strLineName = pInfo->GetName();
    }

    CGroupPlayerManager::Instance().OnLogin(&rMsg, nLine, strLineName);

    CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(rMsg.m_nRoleID);
    if (pRole != NULL)
    {
        CWFriendManager::Instance().OnPlayerLogin(*pRole);
        CDungeonRankProcessor::Instance().PlayerLogin(*pRole);
        CCoupleProcessor::Instance().OnPlayerLogin(*pRole);
        CeremonyBriefManager::Instance().OnPlayerLogin(*pRole);
        BigMamaGroup::Instance().OnRoleLogin(*pRole);
        CDanceGroupFairyLandProcessor::Instance().OnRoleLogin(*pRole);
        CWMailManager::Instance().OnPlayerLogin(*pRole);
    }

}

void CGroupServerCirculator::OnPlayerLogout(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_Logout &rMsg = (GameMsg_S2G_Logout &)msg;
    CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(rMsg.m_nRoleID);

    if (pRole != NULL && pRole->GetLine() == rMsg.m_nLine)
    {
        //各模块的退出函数
        CDungeonRankProcessor::Instance().PlayerLogout(*pRole);
        CWFriendManager::Instance().OnPlayerLogout(*pRole);
        CLBS::Instance().RemovePos(rMsg.m_nRoleID, (ESexType)pRole->GetSex());
        GroupRoomProcessor::Instance().OnRoleLogOut(pRole);
        g_mapRoleEntity.erase(rMsg.m_nRoleID);
    }
}

void CGroupServerCirculator::OnGameServerLinkInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    unsigned short nSlot = slotPeer.GetSlot();
    GameMsg_S2G_GameServerLinkInfo &rMsg = (GameMsg_S2G_GameServerLinkInfo &)msg;
    rMsg.m_linkInfo.SetSlot(nSlot);

    in_addr addr;
    addr.s_addr = rMsg.m_linkInfo.GetAddr();
    WriteLog(LOGLEVEL_SYSTEM, "Get GameServer connecting msg. Line: %d, slot: %u, addr: %s, port: %d\n",
        rMsg.m_linkInfo.GetLine(), nSlot, inet_ntoa(addr), rMsg.m_linkInfo.GetPort());

    if (m_linkingInfos.AddServerInfo(rMsg.m_linkInfo))
    {
        if (m_pWorldHost != NULL)
        {
            m_pWorldHost->OnGameServerConnected(nSlot);
        }

        if (m_pSocketMgr != NULL)
        {
            GameMsg_G2S_GameServerLinkInfoResult resMsg;
            resMsg.m_nServerType = SERVER_GROUP;
            m_pSocketMgr->SendGameMsg(&resMsg, nSlot);
        }

        WriteLog(LOGLEVEL_SYSTEM, "GameServer connected success. Line: %d, slot: %u, addr: %s, port: %d\n",
            rMsg.m_linkInfo.GetLine(), nSlot, inet_ntoa(addr), rMsg.m_linkInfo.GetPort());
    }
}

void CGroupServerCirculator::OnLinkCount(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    unsigned short nClient = ((GameMsg_LinkCount &)msg).nCount;
    m_linkingInfos.SetClientBySlot(SlotPeer.GetSlot(), nClient);
}

void CGroupServerCirculator::OnMallAskFor(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2S_MallAskFor &rMsg = (GameMsg_S2S_MallAskFor &)msg;
    CGroupRoleEntity *pSender = CGroupPlayerManager::Instance().SearchRoleByID(rMsg.m_nAskForRoleID);
    CGroupRoleEntity *pDestRole = CGroupPlayerManager::Instance().SearchRoleByID(rMsg.m_nDestRoleID);

    if (pDestRole != NULL)
    {
        if (pSender != NULL)
        {
            rMsg.m_nResult = 2;
            pSender->SendPlayerMsg(&rMsg);

            memcpy(rMsg.szAskForRoleName, pSender->GetName(), MAX_NAME_LEN - 1);
        }

        if (!pDestRole->HasBlackList(rMsg.m_nAskForRoleID))
        {
            rMsg.m_nResult = 0;
            pDestRole->SendPlayerMsg(&rMsg);
        }
    }
    else if (pSender != NULL)
    {
        rMsg.m_nResult = 1;
        pSender->SendPlayerMsg(&rMsg);
    }
}

void CGroupServerCirculator::OnMallDesire(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_MallDesire &rMsg = (GameMsg_S2G_MallDesire &)msg;

    GameMsg_G2S_MallDesire desireMsg;
    desireMsg.m_nTypeID = rMsg.m_nTypeID;
    desireMsg.nAddNum = rMsg.nAddNum;

    unsigned short nSlot = slotPeer.GetSlot();
    BroadcastMsg2GameServer(&desireMsg, m_linkingInfos.GetLineBySlot(nSlot));
}

void CGroupServerCirculator::OnWishComeTrue(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_WishComeTrue & c2smsg = (GameMsg_S2G_WishComeTrue &)msg;

    CGroupRoleEntity * pRole = CGroupPlayerManager::Instance().SearchRoleByID(c2smsg.m_nDstRoleID);
    if (pRole != NULL)
    {
        GameMsg_G2S_WishComeTrue g2sMsg;
        g2sMsg.m_nSrcRoleID = c2smsg.m_nSrcRoleID;
        g2sMsg.m_nDstRoleID = c2smsg.m_nDstRoleID;
        g2sMsg.m_nItemID = c2smsg.m_nItemID;

        pRole->SendPlayerMsg(&g2sMsg);
    }
}

void CGroupServerCirculator::OnGMServerReportInfo(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_M2G_SendGroupServerInfo *pMsg = (GameMsg_M2G_SendGroupServerInfo*)&msg;
    if (pMsg != NULL) {
        m_nGMSlot = slotPeer.GetSlot();
        GameMsg_G2M_SendGroupServerInfoRes msgRes;
        SendMsgToGMServer(&msgRes, slotPeer.GetSlot());
        WriteLog(LOGLEVEL_DEBUG, "GMServer slot [%d]", m_nGMSlot);
    }
}

void CGroupServerCirculator::OnGMSendMail(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_M2S_GMSendMail *pMsg = (GameMsg_M2S_GMSendMail *)&msg;
    if (pMsg != NULL) {
        int nLine = GetAvailableGameServerLine();
        if (nLine != -1) {
            SendMsg2GameServer(pMsg, nLine);
        }
    }
}

void CGroupServerCirculator::OnGMSendMailRes(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2M_GMSendMailRes *pMsg = (GameMsg_S2M_GMSendMailRes *)&msg;
    if (pMsg != NULL) {
        SendMsgToGMServer(pMsg);
    }
}

void CGroupServerCirculator::OnGMGetActivatedGiftNum(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_M2S_QueryRangeGift *pMsg = (GameMsg_M2S_QueryRangeGift *)&msg;
    if (pMsg != NULL) {
        int nLine = GetAvailableGameServerLine();
        if (nLine != -1) {
            SendMsg2GameServer(pMsg, nLine);
        }
    }
}

void CGroupServerCirculator::OnGMGetActivatedGiftNumRes(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2M_QueryRangeGiftRes *pMsg = (GameMsg_S2M_QueryRangeGiftRes *)&msg;
    if (pMsg != NULL) {
        SendMsgToGMServer(pMsg);
    }
}

void CGroupServerCirculator::OnGMGetGiftCodesState(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_M2S_QueryMultiGift *pMsg = (GameMsg_M2S_QueryMultiGift *)&msg;
    if (pMsg != NULL)
    {
        int nLine = GetAvailableGameServerLine();
        if (nLine != -1)
        {
            SendMsg2GameServer(pMsg, nLine);
        }
    }
}

void CGroupServerCirculator::OnGMGetGiftCodesStateRes(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2M_QueryMultiGiftRes *pMsg = (GameMsg_S2M_QueryMultiGiftRes *)&msg;
    if (pMsg != NULL)
    {
        SendMsgToGMServer(pMsg);
    }
}

void CGroupServerCirculator::OnGMSendAnnounce(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_M2S_Announce &rMsgAnnounce = (GameMsg_M2S_Announce &)msg;

    GameMsg_G2S_Announce msgAnnounce;
    msgAnnounce.m_announce = rMsgAnnounce.m_announce;
    BroadcastMsg2GameServer(&msgAnnounce);
}

void CGroupServerCirculator::OnBlessFriend(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_BlessFriend & msgFromGame = (GameMsg_S2G_BlessFriend &)msg;

    CGroupRoleEntity * pRole = CGroupPlayerManager::Instance().SearchRoleByID(msgFromGame.m_nRcverID);
    if (pRole != NULL) {
        GameMsg_G2S_BlessFriend msgToGame;
        msgToGame.m_nSenderID = msgFromGame.m_nSenderID;
        msgToGame.m_nRcverID = msgFromGame.m_nRcverID;
        msgToGame.m_strSenderName = msgFromGame.m_strSenderName;
        msgToGame.m_nWordsIndex = msgFromGame.m_nWordsIndex;
        msgToGame.m_nBlessTime = msgFromGame.m_nBlessTime;

        pRole->SendPlayerMsg(&msgToGame);
    }
}

void CGroupServerCirculator::OnUpdateVIPInfo(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_UpdateVIPInfo & rMsgUpdate = (GameMsg_S2G_UpdateVIPInfo &)msg;

    CGroupRoleEntity * pRole = CGroupPlayerManager::Instance().SearchRoleByID(rMsgUpdate.m_nRoleID);
    if (pRole != NULL) {
        pRole->SetIsVIP(rMsgUpdate.m_bIsVIP);
        pRole->SetVIPLevel(rMsgUpdate.m_nVIPLevel);

        GroupRoomProcessor::Instance().OnRoleVipChanged(pRole, rMsgUpdate.m_bIsVIP, rMsgUpdate.m_nVIPLevel);
    }
}

void CGroupServerCirculator::OnDirectAllClient(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_BlessingNotify & snotify = (GameMsg_S2G_BlessingNotify &)msg;

    GameMsg_G2S_BlessingNotify  gnotify;
    gnotify.m_strManName = snotify.m_strManName;
    gnotify.m_strWomanName = snotify.m_strWomanName;
    gnotify.m_nPointIndex = snotify.m_nPointIndex;

    std::map<unsigned int, CGroupRoleEntity>::iterator it = g_mapRoleEntity.begin();
    for (; it != g_mapRoleEntity.end(); ++it)
    {
        if (it->second.GetRoleID() != snotify.m_nMaleRoleID && it->second.GetRoleID() != snotify.m_nFemaleRoleID)
        {
            it->second.SendPlayerMsg(&gnotify);
        }
    }
}

void CGroupServerCirculator::OnGMLockOrUnLockAccount(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_M2G_LockOrUnLockByGm *pMsg = (GameMsg_M2G_LockOrUnLockByGm *)&msg;
    GameMsg_G2S_LockOrUnLockAccountByGm sendMsg;
    sendMsg.m_nPid = pMsg->m_nPid;
    sendMsg.m_strPUID = pMsg->m_strPUID;
    sendMsg.m_strTransID = pMsg->m_strTransID;
    sendMsg.m_bLock = pMsg->m_bLock;
    sendMsg.m_nLockTimeType = pMsg->m_nLockTimeType;

    int nLine = GetAvailableGameServerLine();//随意选择一条正常的线，将请求发过去
    if (nLine != -1)
    {
        SendMsg2GameServer(&sendMsg, nLine);
    }
}

void CGroupServerCirculator::OnGMAddMCoin(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_M2G_AddMCoinByGm *pMsg = (GameMsg_M2G_AddMCoinByGm *)&msg;
    GameMsg_G2S_AddMCoinByGm sendMsg;
    sendMsg.m_nPid = pMsg->m_nPid;
    sendMsg.m_strPUID = pMsg->m_strPUID;
    sendMsg.m_nAmount = pMsg->m_nAmount;
    sendMsg.m_nBindAmount = pMsg->m_nBindAmount;
    sendMsg.m_strTransID = pMsg->m_strTransID;
    sendMsg.m_nCurrency = pMsg->m_nCurrency;

    int nLine = GetAvailableGameServerLine();//随意选择一条正常的线，将请求发过去
    if (nLine != -1)
    {
        SendMsg2GameServer(&sendMsg, nLine);
    }
}

void CGroupServerCirculator::OnUpdateLBSPositon(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_UpdateLBSPositon* pMsg = (GameMsg_S2G_UpdateLBSPositon*)&msg;
    CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
    if (pRole != NULL)
    {
        pRole->SetX(pMsg->m_fX);
        pRole->SetY(pMsg->m_fY);
        LBSPOS pos(pRole->m_fX, pRole->m_fY);
        CLBS::Instance().RemovePos(pMsg->m_nRoleID, (ESexType)pRole->GetSex());
        CLBS::Instance().AddPos(pos, pMsg->m_nRoleID, (ESexType)pRole->GetSex());
    }
}

void CGroupServerCirculator::OnGetNearbyPlayers(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_GetNearbyPlayers* pMsg = (GameMsg_S2G_GetNearbyPlayers*)&msg;
    CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
    if (pRole != NULL)
    {
        set<unsigned int> setFilter;
        setFilter.insert(pRole->GetBlackList().begin(), pRole->GetBlackList().end());
        Pos2RoleMap mapPos;
        size_t nSum = 0;
        nSum += CLBS::Instance().GetPosRoundRole(pMsg->m_nRoleID, LBS_SEARCH_RADIUS, LBS_SEARCH_PLAYER_COUNT, mapPos, true, ESexType_No, setFilter);
        GameMsg_G2S_NearbyPlayers retMsg;
        retMsg.m_nRoleID = pMsg->m_nRoleID;
        retMsg.m_cFlag = pMsg->m_cFlag;
        retMsg.m_listNearbyPlayer.reserve(nSum);
        for (Pos2RoleMap::iterator it = mapPos.begin(); it != mapPos.end(); ++it)
        {
            for (set<unsigned int>::iterator setit = it->second.begin(); setit != it->second.end(); ++setit)
            {
                pRole = CGroupPlayerManager::Instance().SearchRoleByID(*setit);
                if (pRole != NULL)
                {
                    NearbyPlayer ply;
                    ply.m_nRoleID = pRole->GetRoleID();
                    ply.m_strRoleName.assign(pRole->GetName());
                    ply.m_nSex = pRole->GetSex();
                    ply.m_nLine = pRole->GetLine();
                    ply.m_nLevel = pRole->GetLevel();
                    ply.m_nVIPLevel = pRole->VIPLevel();
                    ply.m_fDist = it->first;
                    ply.m_fX = pRole->GetX();
                    ply.m_fY = pRole->GetY();
                    retMsg.m_listNearbyPlayer.push_back(ply);
                }
            }
        }
        SendMsg2GameServerBySlot(&retMsg, slotPeer.GetSlot());
    }
}

void CGroupServerCirculator::OnCloseLBS(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_S2G_CloseLBS* pMsg = (GameMsg_S2G_CloseLBS*)&msg;
    CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(pMsg->m_nRoleID);
    if (pRole != NULL)
    {
        pRole->SetX(0);
        pRole->SetY(0);
        CLBS::Instance().RemovePos(pMsg->m_nRoleID, (ESexType)pRole->GetSex());
    }
}

void CGroupServerCirculator::OnForward(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_Forward &forwardMsg = (GameMsg_Forward &)msg;
    if (forwardMsg.m_forwardMsg != NULL && !forwardMsg.m_RoleIDList.empty())
    {
        forwardMsg.m_forwardMsg->nMsg = forwardMsg.m_nForwardMsgID;
        Forward(*forwardMsg.m_forwardMsg, forwardMsg.m_RoleIDList);
    }
}

void CGroupServerCirculator::Forward(GameMsg_Base &msg, const std::list<unsigned int> &roleIDList)
{
    for (std::list<unsigned int>::const_iterator it = roleIDList.begin(); it != roleIDList.end(); ++it)
    {
        unsigned int nRoleID = *it;
        Forward(msg, nRoleID);
    }
}

void CGroupServerCirculator::Forward(GameMsg_Base &msg, unsigned int nRoleID)
{
    CGroupRoleEntity *pRole = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);
    if (pRole != NULL)
    {
        GameMsg_Forward toForwardMsg;
        toForwardMsg.m_nForwardMsgID = msg.nMsg;
        toForwardMsg.m_forwardMsg = &msg;
        pRole->SendPlayerMsg(&toForwardMsg);
    }
}

void CGroupServerCirculator::OnGameServerClosed(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (NULL != m_pWorldHost)
        m_pWorldHost->SetClosed();
}

//DB返回外部系统发送邮件请求
void CGroupServerCirculator::OnDBExternalSendMail(QueryBase &pQuery)
{
    ExternalSendMailInfo * pESMInfo = (ExternalSendMailInfo *)pQuery.m_pAttachment;
    if (pESMInfo == NULL)
    {
        WriteLog(LOGLEVEL_ERROR, "Error: CGroupServerCirculator::OnDBExternalSendMail. pESMInfo == NULL.");
        return;
    }
    if (pESMInfo->m_nReceiveID > 0)
    {
        CWMailManager::Instance().SendSystemMailToPlayer(pESMInfo->m_nReceiveID, EMailType_ExternalSendMail, pESMInfo->m_strTitle, pESMInfo->m_strContent,
            pESMInfo->m_nMoney, 0, pESMInfo->m_nItemType, pESMInfo->m_nCount, pESMInfo->m_nValidTime);
    }
    delete pESMInfo; pESMInfo = NULL;
}
//DB返回外部系统发送Gift邮件请求
void CGroupServerCirculator::OnDBExternalSendMailGift(QueryBase &pQuery)
{
    ExternalSendMailGiftInfo * pESMGInfo = (ExternalSendMailGiftInfo *)pQuery.m_pAttachment;
    if (pESMGInfo == NULL)
    {
        WriteLog(LOGLEVEL_ERROR, "Error: CGroupServerCirculator::OnDBExternalSendMailGift. pESMGInfo == NULL.");
        return;
    }
    if (pESMGInfo->m_nReceiveID > 0)
    {
        CMailGiftInfo * pMailGiftInfo = CMailGiftSystem::Instance().GetMailGiftInfo(pESMGInfo->m_nMailGiftID);
        if (pMailGiftInfo)
        {
            pMailGiftInfo->SendMailGiftToRole(pESMGInfo->m_nReceiveID, pESMGInfo->m_nbMale);
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "Error: CGroupServerCirculator::OnDBExternalSendMailGift. MailGiftNotExist. PID=%u, PUID=%s, MailGiftID=%d, OrderID=%s",
                pESMGInfo->m_nPID, pESMGInfo->m_strPUID.c_str(), pESMGInfo->m_nMailGiftID, pESMGInfo->m_strOrderID.c_str());
        }
    }
    else
    {
        WriteLog(LOGLEVEL_ERROR, "Error: CGroupServerCirculator::OnDBExternalSendMailGift. RoleNotExist. PID=%u, PUID=%s, MailGiftID=%d, OrderID=%s",
            pESMGInfo->m_nPID, pESMGInfo->m_strPUID.c_str(), pESMGInfo->m_nMailGiftID, pESMGInfo->m_strOrderID.c_str());
    }
    delete pESMGInfo; pESMGInfo = NULL;
}
//外部系统请求发送邮件
void CGroupServerCirculator::OnExternalSendMail(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_EX2G_SendMail & rMsg = (GameMsg_EX2G_SendMail &)msg;
    ExternalSendMailInfo * pESMInfo = new ExternalSendMailInfo();
    pESMInfo->m_nPID = rMsg.m_nPID;
    pESMInfo->m_strPUID = rMsg.m_strPUID;
    pESMInfo->m_nReceiveID = 0;
    pESMInfo->m_strTitle = rMsg.m_strTitle;
    pESMInfo->m_strContent = rMsg.m_strContent;
    pESMInfo->m_nMoney = rMsg.m_nMoney;
    pESMInfo->m_nItemType = rMsg.m_nItemType;
    pESMInfo->m_nCount = rMsg.m_nCount;
    pESMInfo->m_nValidTime = rMsg.m_nValidTime;
    pESMInfo->m_strOrderID = rMsg.m_strOrderID;

    m_pQueryMgr->AddQuery(QUERY_External_SendMail, 0, pESMInfo);
}

//外部系统请求发送Gift邮件
void CGroupServerCirculator::OnExternalSendMailGift(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_EX2G_SendMailGift & rMsg = (GameMsg_EX2G_SendMailGift &)msg;
    ExternalSendMailGiftInfo * pESMGInfo = new ExternalSendMailGiftInfo();
    pESMGInfo->m_nPID = rMsg.m_nPID;
    pESMGInfo->m_strPUID = rMsg.m_strPUID;
    pESMGInfo->m_nReceiveID = 0;
    pESMGInfo->m_nbMale = true;
    pESMGInfo->m_nMailGiftID = rMsg.m_nMailGiftID;
    pESMGInfo->m_strOrderID = rMsg.m_strOrderID;

    m_pQueryMgr->AddQuery(QUERY_External_SendMailGift, 0, pESMGInfo);
}

int CGroupServerCirculator::GetAvailableGameServerLine()
{
    return m_linkingInfos.GetAvailableLine();
}

void CGroupServerCirculator::SendMallReloadMsg()
{
    GameMsg_G2S_MallReload reloadMsg;
    BroadcastMsg2GameServer(&reloadMsg);
}

void CGroupServerCirculator::OnGMGetServerLine(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_M2G_GetServerLine &rMsg = (GameMsg_M2G_GetServerLine &)msg;

    GameMsg_G2M_GetServerLineRes retMsg;
    retMsg.m_nSlot = rMsg.m_nSlot;

    const GameServerLinkInfo *pInfo = NULL;
    const std::vector<GameServerLinkInfo *> &rLinkInfos = m_linkingInfos.GetAllLinkInfos();
    for (std::vector<GameServerLinkInfo *>::const_iterator itr = rLinkInfos.begin();
        itr != rLinkInfos.end(); ++itr)
    {
        pInfo = *itr;
        if (pInfo == NULL)
            continue;

        retMsg.m_Lines.insert(std::make_pair(pInfo->GetLine(), pInfo->GetName()));
    }

    SendMsgToGMServer(&retMsg, slotPeer.GetSlot());
}

void CGroupServerCirculator::OnGMGetRoomList(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_M2G_GetRoomList& rMsg = (GameMsg_M2G_GetRoomList&)msg;
    GameMsg_G2S_GetRoomListByGM gsMsg;
    gsMsg.m_nSlot = rMsg.m_nSlot;
    gsMsg.m_nGMAskRoomType = rMsg.m_nGMAskRoomType;
    SendMsg2GameServer(&gsMsg, rMsg.m_nLine);
}

void CGroupServerCirculator::OnServerGetRoomList(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_S2G_GetRoomListByGMRes& rMsg = (GameMsg_S2G_GetRoomListByGMRes&)msg;

    GameMsg_G2M_GetRoomListRes retMsg;
    retMsg.m_nSlot = rMsg.m_nSlot;

    std::vector<GMRoomInfo>::iterator it = rMsg.m_RoomList.begin();
    for (; it != rMsg.m_RoomList.end(); ++it)
    {
        retMsg.m_RoomList.push_back(*it);
    }
    SendMsgToGMServer(&retMsg);
}

void CGroupServerCirculator::OnGMSetMallUpdateTime(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    time_t tNow = time(NULL);
    GameMsg_M2G_SetMallUpdateTime& rMsg = (GameMsg_M2G_SetMallUpdateTime&)msg;

    if (rMsg.m_UpdateTime > (unsigned int)tNow)
    {
        m_bMallNeedReload = true;
        m_uMallReloadTime = rMsg.m_UpdateTime;
    }
    else if (rMsg.m_UpdateTime == (unsigned int)tNow)
    {
        SendMallReloadMsg();
    }
}

void CGroupServerCirculator::OnGMSendPrivMCoin(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    int nLine = GetAvailableGameServerLine();//随意选择一条正常的线，将请求发过去
    if (nLine != -1)
    {
        SendMsg2GameServer(&msg, nLine);
    }
}

void CGroupServerCirculator::OnGMSendPrivMCoinRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    SendMsgToGMServer(&msg);
}

void CGroupServerCirculator::OnGMGetServerPlayerCountInfo(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_M2G_GetServerPlayerCountInfo &rMsg = (GameMsg_M2G_GetServerPlayerCountInfo &)msg;

    GameMsg_G2M_GetServerPlayerCountInfoRet retMsg;
    retMsg.m_nSolt = rMsg.m_nSolt;

    //根据线路来创建返回数据对象
    const GameServerLinkInfo *pInfo = NULL;
    const std::vector<GameServerLinkInfo *> &rLinkInfos = m_linkingInfos.GetAllLinkInfos();
    for (std::vector<GameServerLinkInfo *>::const_iterator itr = rLinkInfos.begin();
        itr != rLinkInfos.end(); ++itr)
    {
        pInfo = *itr;
        if (pInfo == NULL)
            continue;

        CServerPlayerCountInfo serverInfo;
        serverInfo.m_nLine = pInfo->GetLine();
        serverInfo.m_strName = pInfo->GetName();
        serverInfo.m_nCurrentCount = pInfo->GetClient();
        serverInfo.m_nLimitCount = 0;

        retMsg.m_ServerPlayerCountInfoMap.insert(std::make_pair(serverInfo.m_nLine, serverInfo));
    }

    //这些结果先返回给GM客户端
    SendMsgToGMServer(&retMsg, slotPeer.GetSlot());

    //取得每条线路人数上限
    GameMsg_G2A_GetServerPlayerLimit gsMsg;
    gsMsg.m_nSolt = rMsg.m_nSolt;

    //随意选择一条正常的线，将请求发过去
    int nLine = GetAvailableGameServerLine();
    if (nLine != INVALID_LINE)
    {
        SendMsg2GameServer(&gsMsg, nLine);
    }
}

void CGroupServerCirculator::OnGMSendBlackDeviceList(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    int nLine = GetAvailableGameServerLine();
    if (nLine != -1)
    {
        SendMsg2GameServer(&msg, nLine);
    }
}

void CGroupServerCirculator::OnGMChangeAccountPassword(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    int nLine = GetAvailableGameServerLine();
    if (nLine != -1)
    {
        SendMsg2GameServer(&msg, nLine);
    }
}

void CGroupServerCirculator::OnSendBlackDeviceListRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    SendMsgToGMServer(&msg);
}

void CGroupServerCirculator::OnChangeAccountPasswordRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    SendMsgToGMServer(&msg);
}

void CGroupServerCirculator::OnGetServerPlayerLimitRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    SendMsgToGMServer(&msg);
}

void CGroupServerCirculator::OnGMSetServerPlayerCountInfo(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    int nLine = GetAvailableGameServerLine();//随意选择一条正常的线，将请求发过去
    if (nLine != -1) {
        SendMsg2GameServer(&msg, nLine);
    }
}

void CGroupServerCirculator::OnGMDeleteVersionMedal(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_G2S_GMDeleteVersionMedal resultMsg;

    int nLine = GetAvailableGameServerLine();
    if (nLine != -1) {
        SendMsg2GameServer(&resultMsg, nLine);
    }
}

void CGroupServerCirculator::OnGMDeleteVersionMedalRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    SendMsgToGMServer(&msg);
}

void CGroupServerCirculator::OnGMAddMedal(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    int nLine = GetAvailableGameServerLine();
    if (nLine != -1)
    {
        SendMsg2GameServer(&msg, nLine);
    }
}

void CGroupServerCirculator::OnGMAddMedalRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    SendMsgToGMServer(&msg);
}

void Forward(GameMsg_Base &msg, unsigned int nRoleID)
{
    if (g_pCirculator != NULL && nRoleID > 0)
    {
        g_pCirculator->Forward(msg, nRoleID);
    }
}

void Forward(GameMsg_Base &msg, const std::list<unsigned int> &roleIDList)
{
    if (g_pCirculator != NULL && !roleIDList.empty())
    {
        g_pCirculator->Forward(msg, roleIDList);
    }
}
