#include "../share/StdAfx.h"
#include "../libServerFrame/Main.h"
#include "LogicCirculator.h"
#include "../util/IDCreater.h"
#include "CommandProcessor.h"
#include "../../datastructure/LogStructDef.h"
#include <time.h>
#include <string>
#include <deque>
#include "../share/ServerMsgDef.h"
#include "MainManager.h"
#include "../libServerFrame/QueryManager.h"
#include "../libServerFrame/GameMsgCacheEnt.h"
#include "../util/CommonFunc.h"
#include "../lua/InitLua.h"
#include "../../socket/Md5.h"
#include "../../socket/Formatter.h"
#include "../../server/logic/PlayerManager.h"
#include "../../socket/GameMsg_Map.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../Couple/WeddingRoomManager.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../util/TimerMgr.h"
#include "../AmuseRoom/AmuseRoomDataManager.h"
#include "../player/GameMsg_Buff.h"


/* network */
#include "../share/GameMsg_Server_Connected.h"
#include "../login/GameMsg_Server_AccountLogin.h"
#include "../share/GameMsg_LinkCount.h"
#include "../login/GameMsg_Server_Login.h"
#include "../login/GameMsg_Server_LoginResult.h"
#include "../login/GameMsg_Server_DropClient.h"
#include "../login/GameMsg_Server_AccountLogout.h"
#include "../login/GameMsg_Game2GatePlayerLogout.h"
#include "../login/GameMsg_S2G_Logout.h"
#include "../chat/GameMsg_C2S_Chat_Priv.h" 
#include "../chat/GameMsg_C2S_Chat.h"
#include "../chat/GameMsg_S2C_Chat.h"
#include "../../socket/GameMsg_System_Connect.h"
#include "../agent/AgentMsg_Announce.h"
#include "../agent/AgentMsg_ShutdownInTime.h"
#include "../agent/AgentMsg_Close.h"
#include "HostMsgDef.h"
#include "../Couple/CoupleMsgDef.h"
#include "../login/GameMsg_C2S_TouristSignInfo.h"
/* About Role Action */
#include "../login/GameMsg_Player_CreateRole.h"
#include "../login/GameMsg_Player_CreateRoleRes.h"
#include "../login/GameMsg_Player_CheckRoleName.h"
#include "../login/GameMsg_Player_CheckRoleNameResult.h"
#include "../login/GameMsg_S2C_RequireCreateRole.h"
#include "../login/GameMsg_Player_Logout.h"
#include "../login/GameMsg_Player_SwitchLine.h"
#include "../login/GameMsg_S2G_SwitchLineComplete.h"
#include "../login/GameMsg_Server_VerifyAccount.h"
#include "../GM/GameMsg_GM_share.h"
#include "../share/GameMsg_ChargeOnline.h"
#include "../share/GameMsg_SyncSetting.h"
#include "../Couple/GameMsg_G2S_Couple.h"
#include "../Couple/GameMsg_S2C_Couple.h"
#include "../roomLib/GameMsg_C2S_RoomMsg.h"
#include "../roomLib/GameMsg_S2C_RoomMsg.h"
#include "../player/GameMsg_C2S_BugReport.h"
#include "../group/GameMsg_LBS.h"
#include "../mail/MailMgr.h"
#include "../../datastructure/Localization.h"
#include "../../datastructure/datainfor/ServerConfig.h"
#include "../../datastructure/RoleEntity.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../activity/EntityActivityComponent.h"
#include "../Medal/MedalCfgMgr.h"
#include "../Medal/GameMsg_Gm_medal.h"
#include "../GM/GMMsgDef.h"
/* GroupServer */
#include "../GM/GameMsg_G2S_Announce.h"
#include "../share/GameMsg_G2S_LockOrUnLockAccountByGm.h"
#include "../GM/GameMsg_S2A_LockOrUnLockAccountByGm.h"
#include "../share/GameMsg_S2A_AddMCoinByCmd.h"
#include "../share/GameMsg_G2S_AddMCoinByGm.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../activity/CumulativeRechargeMgr.h"
#include "../GM/GameMsg_G2S_NewCompensation.h"
#include "../compensation/CompensationManager.h"
#include "../compensation/GameMsg_S2C_NewCompensation.h"
#include "../payment/PaymentInfoMgr.h"
#include "../mail/GameMsg_Mail.h"
#include "../mail/EntityMailComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../login/GameMsg_Payment.h"
#include "../item/ItemProcess.h"
#include <sstream>
#include "../player/PlayerInfoComponent.h"
#include "../../datastructure/datainfor/GMDataInfo.h"
#include "../GM/GameMsg_G2S_GetRoomListByGM.h"
#include "../room/RoomManager.h"
#include "../room/LiveRoomManager.h"
#include "../AmuseRoom/AmuseRoomManager.h"
#include "../PhotoRoom/PhotoRoomManager.h"
#include "../share/GameMsg_S2G_GameServerClosed.h"
#include "../share/GameMsg_GameServerLinkInfo.h"
#include "../util/NameDeny.h"
#include "../GM/GameMsg_GM.h"
#include "../CeremonyRoom/ceremonyRoomManager.h"

/* For debug */
extern CIni g_Config;
extern CIni g_GameConfig;
extern int  g_nGroup;
extern int  g_nLine;

list< char* >g_listMD5;

bool g_bCheckVersion = true;

CIDCreater* g_pIDCreater = NULL;
CLogicCirculator* g_pCirculator = NULL;
CClientSocketMgr* g_pSockHost = NULL;
CClientSocketMgr* g_pSockLogin = NULL;
CQueryManager*	g_pLoginQueryMgr = NULL;

#ifndef MAX_NAMEPASS
#define MAX_NAMEPASS	80
#endif

CLogicCirculator::CLogicCirculator()
{
    m_pSockHost = NULL;
    m_pSockLogin = NULL;
    m_pIDCreater = NULL;

    m_pMainManager = NULL;
    m_pCommandProcessor = NULL;

    m_stat.nType = SVRTYPE_GAME;

    m_nReconnectLoginSvrInterval = 100;
    m_nReconnectGroupSvrInterval = 100;
    g_pCirculator = this;

    m_pHostSlotPeer = NULL;
    m_pLogirSlotPeer = NULL;

    srand((unsigned int)time(NULL));

    m_nInstallerID = INSTALLER_INVALID;
    m_nFeedVersionPtReturn = 0;

    m_bConnectedGroupServer = false;
    m_bConnectedAccountServer = false;

    m_pCommandProcessor = new CommandProcessor;
}

CLogicCirculator::~CLogicCirculator()
{
    delete m_pSockHost;
    delete m_pSockLogin;
    delete m_pIDCreater;
    delete m_pMainManager;
    delete m_pCommandProcessor;
    delete g_pLoginQueryMgr;

    if (m_pHostSlotPeer)
    {
        delete m_pHostSlotPeer;
        m_pHostSlotPeer = NULL;
    }

    if (m_pLogirSlotPeer)
    {
        delete m_pLogirSlotPeer;
        m_pLogirSlotPeer = NULL;
    }


    while (g_listMD5.size() > 0)
    {
        delete g_listMD5.front();
        g_listMD5.pop_front();
    }
}

bool CLogicCirculator::Initialize()
{
    if (g_nGroup < 0)
        g_nGroup = g_Config.GetInt(g_sSection, "group", 0);
    if (g_nLine < 0)
        g_nLine = g_Config.GetInt(g_sSection, "line", 0);

    g_bCheckVersion = 0 != g_Config.GetInt(g_sSection, "CheckVersion", 1);

    Formatter(m_ServerName) << "GameServerLine" << g_nLine;

    bool bRes = CCirculator::Initialize();
    RegisterMessage();
    RegisterMessageProcessor();

    m_nReconnectLoginSvrInterval = g_Config.GetInt("interval", "reconnectAccountServer", 6000) / m_nElapseHeartBeat;
    m_nReconnectGroupSvrInterval = g_Config.GetInt("interval", "reconnectGroupServer", 6000) / m_nElapseHeartBeat;
    m_nFeedVersionPtReturn = g_Config.GetInt("interval", "FeedVersionPtRetrun", 0);	// Antroid版用不到
    m_bEnableClientCommand = 1 == g_Config.GetInt(g_sSection, "enableclientcommand", 0);
    m_bRecordLoad = 1 == g_Config.GetInt(g_sSection, "recordload", 0);

    if (bRes)
    {
        m_pIDCreater = new CIDCreater;
        g_pIDCreater = m_pIDCreater;
    }
    if (bRes)
    {
        CInitLua::GetInstance().InitState();
        string sLuaFile = g_sPath + "data/luascript.lua";

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
        g_pLoginQueryMgr = new CQueryManager;
        if (g_pLoginQueryMgr != NULL &&
#ifdef WIN32
            g_pLoginQueryMgr->Initialize(m_hEventNotify)
#else
            g_pLoginQueryMgr->Initialize(&m_condNotify, &m_mutexNotify)
#endif
            && g_pLoginQueryMgr->OpenDatabase(true, true)
            )
        {
            WriteLog(LOGLEVEL_DEBUG, "GameServer LoginQueryMgr initialize successed!\n");
        }
        else
        {
            bRes = false;
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
#ifndef WIN32
    if (bRes)
    {
        bool bCheckData = 0 != g_Config.GetInt(g_sSection, "CheckData", 1);
        bool bCheckDate = 0 != g_Config.GetInt(g_sSection, "CheckDate", 1);

        //检查相关数据的合法性
        std::string sFile(g_sPath);
        sFile.append("etc/servermap.conf");
        unsigned char szMap[16];
        MDFile(sFile.c_str(), szMap);
        sFile.assign(g_sPath), sFile.append("etc/version.key");
        FILE* fp = fopen(sFile.c_str(), "r");
        char szMapBak[16] = { 0 };
        char szDate[16] = {0};
        char szKey[16] = {0};
        if (fp != NULL)
        {
            fread(szMapBak, 16, 1, fp);
            fread(szDate, 16, 1, fp);
            fread(szKey, 16, 1, fp);
            fclose(fp);
        }
        CCryptDes des;
        des.Des_Go(szDate, szDate, 16, szMapBak, 16, _OP_DECRYPT);
        des.Des_Go(szKey, szKey, 16, "ixuanqu.hotdance.com", 16, _OP_DECRYPT);
        des.Des_Go(szMapBak, szMapBak, 16, (char*)szKey, 16, _OP_DECRYPT);

        if (0 != memcmp(szMap, szMapBak, 16) && bCheckData)
            bRes = false;

        time_t now = time(NULL);
        time_t date = 0;
        memcpy(&date, szDate, sizeof(int));

        if (now > date + 60 * 24 * 60 * 60 && bCheckDate)
            bRes = false;

        if (!bRes)
        {
            WriteLog(LOGLEVEL_ERROR, "Important data file modified!\n");
        }

        //载入合法的客户端版本的信息
        //if(bRes)
        //{
        //	sFile.assign(g_sPath), sFile.append("etc/validclient.md5");
        //	fp = fopen(sFile.c_str(), "r");
        //	if(fp != NULL)
        //	{
        //		while(!feof(fp))
        //		{
        //			char *pMD5 = new char[16];
        //			fread(pMD5, 16, 1, fp);
        //			g_listMD5.push_back(pMD5);
        //		}
        //		fclose(fp);
        //	}
        //	else
        //	{
        //		bRes = false;
        //	}
        //}
    }
#endif
    if (bRes)
    {
        m_pMainManager = new MainManager;
        ASSERT(m_pMainManager);
        if (m_pMainManager)
            bRes = m_pMainManager->Init();
        else
            bRes = false;

        ASSERT(bRes);
        if (!bRes)
            WriteLog(LOGLEVEL_ERROR, "Init() return false!\n");
    }
    //initialize link to groupserver
    if (bRes)
    {
        char szAddr[MAX_NAMEPASS];
        char szSection[MAX_NAMEPASS];
        Formatter(szSection, sizeof(szSection)) << "group" << g_nGroup;
        g_Config.GetString(szSection, s_szSvrType[SVRTYPE_GROUP], szAddr, MAX_NAMEPASS, "");
#ifdef WIN32
#ifndef _DEBUG
        strcpy_s(szAddr, "127.0.0.1");
#endif
#endif
        short nPort = (short)g_Config.GetInt(s_szSvrType[SVRTYPE_GROUP], "serverport", 7758);
        if (InitClientSock(m_pSockHost) && m_pSockHost->Connect(NULL, szAddr, nPort))
        {
            WriteLog(LOGLEVEL_SYSTEM, "GameServer connecting to GroupServer %s, %d ... OK\n", szAddr, nPort);
        }
        else
        {
            WriteLog(LOGLEVEL_SYSTEM, "GameServer connecting to GroupServer %s, %d ... Failed\n", szAddr, nPort);
        }
        g_pSockHost = m_pSockHost;
    }
    //initialize link to accountserver
    if (bRes)
    {
        char szAddr[MAX_NAMEPASS];
        g_Config.GetString(s_szSvrType[SVRTYPE_ACCOUNT], "internaladdr", szAddr, MAX_NAMEPASS, "");
#ifdef WIN32
#ifndef _DEBUG
        strcpy_s(szAddr, "127.0.0.1");
#endif
#endif
        short nPort = (short)g_Config.GetInt(s_szSvrType[SVRTYPE_ACCOUNT], "internalport", 7752);
        if (InitClientSock(m_pSockLogin) && m_pSockLogin->Connect(NULL, szAddr, nPort))
        {
            WriteLog(LOGLEVEL_SYSTEM, "GameServer connecting to AccountServer %s, %d ... OK\n", szAddr, nPort);
        }
        else
        {
            WriteLog(LOGLEVEL_SYSTEM, "GameServer connecting to AccountServer %s, %d ... Failed\n", szAddr, nPort);
        }
        g_pSockLogin = m_pSockLogin;
    }
    m_pHostSlotPeer = new CSlotPeer();
    m_pHostSlotPeer->SetSocketMgr(m_pSockHost);
    m_pLogirSlotPeer = new CSlotPeer();
    m_pLogirSlotPeer->SetSocketMgr(m_pSockLogin);

    return bRes;
}

void CLogicCirculator::Release()
{
    delete m_pSocketMgr;
    m_pSocketMgr = NULL;

    delete m_pSockLogin;
    m_pSockLogin = NULL;

    CCirculator::Release();
}

void CLogicCirculator::ProcessHeartBeat(int& timeElapse)
{
#ifdef WIN32
#else
    ASSERT(timeElapse < 100);
    ASSERT(timeElapse < 500);
    ASSERT(timeElapse < 1000);
#endif
    CCirculator::ProcessHeartBeat(timeElapse);

    if (m_nHeartBeatCount % m_nReconnectLoginSvrInterval == 0 && m_pSockLogin != NULL)
    {
        m_pSockLogin->Reconnect();
        if (!m_bConnectedAccountServer)
        {
            WriteLog(LOGLEVEL_SYSTEM, "Reconnect to AccountServer.\n");
            GameMsg_S2G_GameServerLinkInfo linkInfoMsg;
            BuildLinkInfo(linkInfoMsg);
            m_pSockLogin->SendGameMsg(&linkInfoMsg);
        }
    }

    if (m_nHeartBeatCount % m_nReconnectGroupSvrInterval == 0 && m_pSockHost != NULL)
    {
        m_pSockHost->Reconnect();
        if (!m_bConnectedGroupServer)
        {
            WriteLog(LOGLEVEL_SYSTEM, "Reconnect to GroupServer.\n");
            OnConnectedWithGroup();
        }
    }

    if (m_nHeartBeatCount % 6000 == 0 && g_pQueryMgr && !g_pQueryMgr->GetThreadState())
    {
        g_pQueryMgr->RecreateThread();
    }

    // 如果登录查询队列空闲
    while ( g_pLoginQueryMgr->GetQueueSize() < 3 )
    {
        SharedPtr<LogicSlot> sharedSlot = m_loginManager.GetNextQueryPlayer();
        if ( !sharedSlot.IsValid() || !sharedSlot->m_sharedRole.IsValid() )
        {
            break;
        }

        g_pLoginQueryMgr->AddQuery( QUERY_GetRoleInfo, sharedSlot->m_sharedRole );
    }

    m_loginManager.OnUpdate( timeElapse );
    m_pMainManager->OnUpdate(timeElapse);

    if (m_bShutdownInTime)
    {
        static time_t lastShow = time(NULL) - 60;
        time_t now = time(NULL);
        if (m_timeShutdown - now < 0)
        {
            EnterExitProcess();
        }
        else if ((m_timeShutdown - now > 60 && now - lastShow >= 60)
            || (m_timeShutdown - now <= 60 && now - lastShow >= 10))
        {
            int remaining = (int)(m_timeShutdown - now);
            std::string strTimeSuffix = CLocalization::Instance().GetString("Time_Second");

            if (remaining > 60)
            {
                remaining /= 60;
                strTimeSuffix = CLocalization::Instance().GetString("Time_Minute");
            }

            std::string strShutdownAnnounce;
            SafeFormat(strShutdownAnnounce, CLocalization::Instance().GetString("Server_Will_ShutDown"), remaining, strTimeSuffix.c_str());

            OnWorldAnnounce(PID_NONE, strShutdownAnnounce.c_str());

            lastShow = now;
        }
    }
}

void CLogicCirculator::ReportStat()
{
    m_stat.nCltCnt = (int)m_loginManager.GetOnlinePlayerCount();
    CCirculator::ReportStat();
}

void CLogicCirculator::BroadCastMsg2AllClient(GameMsg_Base * pChatMsg2Client)
{
    if ( pChatMsg2Client != NULL && m_pSocketMgr != NULL )
    {
        const std::map<unsigned int, SharedPtr<LogicSlot> > &rAllPlayerInfos = m_loginManager.GetAllPlayerInfos();

        for ( std::map<unsigned int, SharedPtr<LogicSlot> >::const_iterator itr = rAllPlayerInfos.begin(); itr != rAllPlayerInfos.end(); itr++ )
        {
            if ( itr->second.IsValid() && itr->second->IsInGame() )
            {
                pChatMsg2Client->nAccount = itr->first;
                pChatMsg2Client->SessionID = itr->second->m_tag.m_nSessionID;

                m_pSocketMgr->SendGameMsg( pChatMsg2Client, itr->second->m_tag.m_nSlot );
            }
        }
    }
}

void CLogicCirculator::RegisterMessage()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_Server_Login);
    GAMEMSG_REGISTERCREATOR(AgentMsg_Announce);
    GAMEMSG_REGISTERCREATOR(HostMsg_Announce);
    GAMEMSG_REGISTERCREATOR(GameMsg_Player_CreateRole);
    GAMEMSG_REGISTERCREATOR(GameMsg_Player_CheckRoleName);
    GAMEMSG_REGISTERCREATOR(GameMsg_Player_Logout);
    GAMEMSG_REGISTERCREATOR(GameMsg_Server_DropClient);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SwitchLine);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_BugReport);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_BlessingNotify);

    GAMEMSG_REGISTERCREATOR(GameMsg_M2S_GMSendMail);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2S_QueryRangeGift);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2S_QueryMultiGift);
    GAMEMSG_REGISTERCREATOR(GameMsg_A2S_ChargeNotify);
    GAMEMSG_REGISTERCREATOR(GameMsg_A2S_SyncChargeSetting);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_Announce);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_LockOrUnLockAccountByGm);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_AddMCoinByGm);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_SyncCompensation);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_NewCompensation);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetRoomListByGM);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2G_SendPrivMCoin);

    GAMEMSG_REGISTERCREATOR(AgentMsg_ShutdownInTime);
    GAMEMSG_REGISTERCREATOR(AgentMsg_Close);
    GAMEMSG_REGISTERCREATOR(GameMsg_A2S_SendMCoinOnline);
    GAMEMSG_REGISTERCREATOR(GameMsg_A2M_SendPrivMCoinRet);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2A_GetServerPlayerLimit);
    GAMEMSG_REGISTERCREATOR(GameMsg_A2G_GetServerPlayerLimitRet);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2A_SetPlayerLimit);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2A_SendBlackDeviceList);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2A_ChangeAccountPassword);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GMDeleteVersionMedal);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2G_GMAddMedal);
}

void CLogicCirculator::RegisterMessageProcessor()
{
    RegMsgProcessor(MSG_SYSTEM_Connect, &CLogicCirculator::OnClientConnect);
    RegMsgProcessor(MSG_SYSTEM_Disconnect, &CLogicCirculator::OnClientDisconnect);
    RegMsgProcessor(MSG_SYSTEM_Invalid, &CLogicCirculator::OnClientInvalidPacket);
    RegMsgProcessor(MSG_C2S_SwitchLine, &CLogicCirculator::OnPlayerSwitchLine);

    RegMsgProcessor(MSG_GM_M2S_SendMail, &CLogicCirculator::OnSendMailFromGMServer);
    RegMsgProcessor(MSG_GM_M2S_QueryRangeGift, &CLogicCirculator::OnQueryMultiGift);
    RegMsgProcessor(MSG_GM_M2S_QueryMultiGift, &CLogicCirculator::OnGetGiftCodesState);
    RegMsgProcessor(MSG_A2S_ChargeNotify, &CLogicCirculator::OnChargeNotify);
    RegMsgProcessor(MSG_A2S_SyncChargeSetting, &CLogicCirculator::OnSyncChargeSetting);
    RegMsgProcessor(MSG_G2S_Announce, &CLogicCirculator::OnAnnounce);
    RegMsgCreatorAndProcessor(GameMsg_C2S_UpdateLBSPositon, &CLogicCirculator::OnUpdateLBSPositon);
    RegMsgCreatorAndProcessor(GameMsg_C2S_GetNearbyPlayers, &CLogicCirculator::OnGetNearbyPlayers);
    RegMsgCreatorAndProcessor(GameMsg_G2S_NearbyPlayers, &CLogicCirculator::OnSyncNearbyPlayers);
    RegMsgCreatorAndProcessor(GameMsg_G2S_GameServerLinkInfoResult, &CLogicCirculator::OnReportServerInfoRespond)

        //account
        RegMsgProcessor(MSG_SERVER_Login, &CLogicCirculator::OnClientLogin);
    RegMsgProcessor(MSG_PLAYER_LOGOUT, &CLogicCirculator::OnPlayerLogout);
    RegMsgProcessor(MSG_Role_CheckRoleName, &CLogicCirculator::OnCheckRoleName);
    RegMsgCreatorAndProcessor(GameMsg_A2S_ChargeReward, &CLogicCirculator::OnChargeReward);
    RegMsgCreatorAndProcessor(GameMsg_A2S_FixedChargeReward, &CLogicCirculator::OnFixedChargeReward);
    RegMsgCreatorAndProcessor(GameMsg_A2M_SendBlackDeviceListRet, &CLogicCirculator::OnSendBlackDeviceListRet);
    RegMsgCreatorAndProcessor(GameMsg_A2M_ChangeAccountPasswordRet, &CLogicCirculator::OnChangeAccountPasswordRet);
    RegMsgCreatorAndProcessor( GameMsg_A2S_RequestPlayerCacheRes, &CLogicCirculator::OnRequestPlayerCacheRes );
    RegMsgCreatorAndProcessor( GameMsg_PlayerCacheTransferStart, &CLogicCirculator::OnPlayerCacheTransferStart );
    RegMsgCreatorAndProcessor( GameMsg_PlayerCacheTransfer, &CLogicCirculator::OnPlayerCacheTransfer );
    RegMsgCreatorAndProcessor( GameMsg_PlayerCacheTransferEnd, &CLogicCirculator::OnPlayerCacheTransferEnd );
    RegMsgCreatorAndProcessor( GameMsg_A2S_VerifyAccountRes, &CLogicCirculator::OnVerifyAccountRes );

    //client
    RegMsgProcessor(MSG_C2S_CreateRole, &CLogicCirculator::OnCreateRole);
    RegMsgProcessor(MSG_C2S_BugReport, &CLogicCirculator::OnBugReport);
    RegMsgCreatorAndProcessor(GameMsg_C2S_GetAllBuff, &CLogicCirculator::OnGetAllBuff);
    RegMsgCreatorAndProcessor(GameMsg_C2S_UnApplyBuff, &CLogicCirculator::OnUnApplyBuff);
    RegMsgCreatorAndProcessor(GameMsg_C2S_TouristSignInfo, &CLogicCirculator::OnTouristSignInfo);

    //AccountServer
    RegMsgProcessor(MSG_SERVER_DropClient, &CLogicCirculator::OnDropClient);
    RegMsgCreatorAndProcessor(GameMsg_A2S_SendMail, &CLogicCirculator::OnSendMailFromAccount);
    RegMsgProcessor(MSG_A2S_SendPrivMCoin, &CLogicCirculator::OnAccountSendOnlineMCoin);
    RegMsgProcessor(MSG_GM_A2M_SendPrivMCoinRet, &CLogicCirculator::OnAccountGMSendPrivMCoinRet);
    //Agent
    RegMsgProcessor(MSG_AGENT_Announce, &CLogicCirculator::OnAgentAnnounce);
    RegMsgProcessor(MSG_AGENT_ShutdownInTime, &CLogicCirculator::OnAgentShutdownInTime);
    RegMsgProcessor(MSG_AGENT_Close, &CLogicCirculator::OnAgentClose);
    //Host
    RegMsgProcessor(MSG_Host_Announce, &CLogicCirculator::OnHostAnnounce);
    //GM
    RegMsgProcessor(MSG_G2S_LockOrUnLockAccountByGm, &CLogicCirculator::OnLockOrUnLockAccountByGm);
    RegMsgProcessor(MSG_G2S_AddMCoinByGm, &CLogicCirculator::OnAddMCoinByGm);
    RegMsgProcessor(MSG_G2S_SyncCompensation, &CLogicCirculator::OnSyncCompensation);
    RegMsgProcessor(MSG_G2S_NewCompensation, &CLogicCirculator::OnNewCompensation);
    RegMsgProcessor(MSG_G2S_GetRoomListByGM, &CLogicCirculator::OnGetRoomListByGM);
    RegMsgProcessor(MSG_GM_M2G_SendPrivMCoin, &CLogicCirculator::OnSendPrivMCoinByGM);
    RegMsgProcessor(MSG_GM_G2A_GetServerPlayerLimit, &CLogicCirculator::OnGMGetServerLineLimit);
    RegMsgProcessor(MSG_GM_A2M_GetServerPlayerLimitRet, &CLogicCirculator::OnAccountSendServerLineLimit);
    RegMsgProcessor(MSG_GM_M2A_SetServerPlayerLimit, &CLogicCirculator::OnGMSetServerPlayerLimit);
    RegMsgProcessor(MSG_GM_M2A_SendBlackDeviceList, &CLogicCirculator::OnGMSendBlackDeviceList);
    RegMsgProcessor(MSG_GM_M2A_ChangeAccountPassword, &CLogicCirculator::OnGMChangeAccountPassword);
    RegMsgProcessor(MSG_GM_G2S_DeleteVersionMedal, &CLogicCirculator::OnGmDeleteVersionMedal);
    RegMsgProcessor(MSG_GM_M2G_AddMedalByGm, &CLogicCirculator::OnGmAddMedal);
    // 
    //     RegQueryProcessor(QUERY_Medal_DeleteVersionMedal,&CEntityMedalComponent::OnGmdeleteVersionMedalReturn);
    //	数据库
    RegQueryProcessor(QUERY_GetRoleInfo, &CLogicCirculator::OnGetRoleInfoQueryFinish);
    RegQueryProcessor(QUERY_CreateRole, &CLogicCirculator::OnDBCreateRole);
    RegQueryProcessor(QUERY_CheckRoleName, &CLogicCirculator::OnDBCheckRoleName);
    RegQueryProcessor(QUERY_UpdateRole, &CLogicCirculator::OnDBUpdateRole);
    RegQueryProcessor(GetRcvGMSysMailRoleInfo, &CLogicCirculator::OnDBGetRcvGMSysMailRoleInfo);
    RegQueryProcessor(QUERY_QueryGiftByTimeRange, &CLogicCirculator::OnDBQueryGiftByTimeRange);
    RegQueryProcessor(QUERY_QueryMultiGift, &CLogicCirculator::OnDBQueryMultiGift);
    RegQueryProcessor(QUERY_Medal_DeleteVersionMedal, &CLogicCirculator::onDBQueryDelVersionMedal);
    RegQueryProcessor(QUERY_Medal_AddMedalByGm, &CLogicCirculator::OnDBQueryAddMedalByGm);

}

void CLogicCirculator::EnterExitProcess()
{
    if (!m_bExiting)
    {
        GameMsg_S2G_GameServerClosed serverCloseMsg;

        if (NULL != m_pSockHost)
            m_pSockHost->SendGameMsg(&serverCloseMsg);
    }

    CCirculator::EnterExitProcess();
}

void CLogicCirculator::OnClientConnect(GameMsg_Base & pMsg, CSlotPeer & SlotPeer)
{
    //GateServer连接成功
    GameMsg_Server_Connected msgConnect;//new GameMsg_Server_Connected;

    msgConnect.nSvrType = SERVER_GAME;
    msgConnect.nLine = g_nLine;
    msgConnect.nInstallerId = m_nInstallerID;
    msgConnect.strName = m_ServerName;
    SlotPeer.SendGameMsg(&msgConnect);
    WriteLog(LOGLEVEL_DEBUG, "GateServer Connected on nSlot %d\n", SlotPeer.GetSlot());
}

void CLogicCirculator::OnClientDisconnect(GameMsg_Base & pMsg, CSlotPeer & SlotPeer)
{
    //踢掉从该GateServer登录的所有玩家
    std::list<unsigned int> allPlayerIDs;
    m_loginManager.GetAllPlayerAccountIDBySlot( SlotPeer.GetSlot(), allPlayerIDs );
    for ( std::list<unsigned int>::const_iterator itr = allPlayerIDs.begin(); itr != allPlayerIDs.end(); ++itr )
    {
        DropPlayer( *itr, true, false, false, DropPlayerCause_GateServerDown );
        WriteLog( LOGLEVEL_DEBUG, "------Drop out player:OnClientDisconnect\n" );
    }

    //通知最新玩家数量
    ReportPlayerCount();

    WriteLog( LOGLEVEL_DEBUG, "GateServer Disconnected on nSlot %d\n", SlotPeer.GetSlot() );
}

void CLogicCirculator::OnClientInvalidPacket(GameMsg_Base & pMsg, CSlotPeer & SlotPeer)
{
    WriteLog(LOGLEVEL_DEBUG, "Invalid packet from slot %d\n", SlotPeer.GetSlot());
    DropPlayer( pMsg.nAccount, true, true, false, DropPlayerCause_WeAreUnderAttack );
}

void CLogicCirculator::OnClientLogin(GameMsg_Base & pMsg, CSlotPeer & SlotPeer)
{
    GameMsg_Server_Login *pMsgLogin = dynamic_cast<GameMsg_Server_Login *>(&pMsg);
    if ( pMsgLogin == NULL )
        return;

    if ( !pMsgLogin->m_bSwitchLine )
    {
        _OnClientLogin( pMsgLogin->SessionID, pMsgLogin->nAccount, SlotPeer.GetSlot(),
            pMsgLogin->m_nInstallerID, pMsgLogin->m_nVersionType, false );
        return;
    }

    if ( !m_bConnectedAccountServer )
    {
        WriteLog( LOGLEVEL_DEBUG, "Can not connected to Account Server, drop the player who switching from the other line." );

        GameMsg_Game2GatePlayerLogout msgPlayerLogout;
        msgPlayerLogout.nAccount = pMsgLogin->nAccount;
        msgPlayerLogout.SessionID = pMsgLogin->SessionID;

        if ( m_pSocketMgr != NULL )
            m_pSocketMgr->SendGameMsg( &msgPlayerLogout, SlotPeer.GetSlot() );

        return;
    }

    GameMsg_S2A_VerifyAccount verifyMsg;
    verifyMsg.nAccount = pMsgLogin->nAccount;
    verifyMsg.SessionID = pMsgLogin->SessionID;
    verifyMsg.m_nSlot = SlotPeer.GetSlot();
    verifyMsg.m_nInstallerID = pMsgLogin->m_nInstallerID;
    verifyMsg.m_nVersionType = pMsgLogin->m_nVersionType;
    SendMsg2AccountServer( &verifyMsg );
}

void CLogicCirculator::_OnClientLogin(__int64 nSessionID, unsigned int nAccount, unsigned short nSlot,
    unsigned char nInstallerId, int nVersionType, bool bSwitchLine /* = false */)
{
    GameMsg_Server_LoginResult msgResult;
    msgResult.nAccount = nAccount;
    msgResult.SessionID = nSessionID;
    msgResult.nErrCode = 2;

    // 如果这个角色已经在线了，就把他踢掉
    DropPlayer( nAccount, false, true, false, DropPlayerCause_ReLogin );

    SharedPtr<LogicSlot> sharedSlot = LogicSlot::Create();
    if ( sharedSlot.IsValid() )
    {
        bool bChecked = false;
        // 校验服务器版本是否成功
        if ( !g_bCheckVersion )
        {
            bChecked = true;
        }

#ifdef SERVER_IOS
        bool isValidInstaller = true;
#else
        bool isValidInstaller = false;
        if ( nInstallerId == INSTALLER_FRESHER && GetServerInstallerID() == INSTALLER_FRESHER )
        {
            isValidInstaller = true;
        }
        else if ( nInstallerId == INSTALLER_FULL && GetServerInstallerID() == INSTALLER_FULL )
        {
            isValidInstaller = true;
        }
        else
        {
            isValidInstaller = false;
        }
#endif //SERVER_IOS

        // 检查安装包版本
        if ( isValidInstaller && bChecked )
        {
            // 登入成功
            msgResult.nErrCode = 0;

            // 初始化PlayerTag，这里的初始化非常重要
            sharedSlot->m_tag.m_nAccount = nAccount;
            sharedSlot->m_tag.m_nVersionType = nVersionType;
            sharedSlot->m_tag.m_nSlot = nSlot;
            sharedSlot->m_tag.m_nSessionID = nSessionID;
            sharedSlot->m_bSwitchLine = bSwitchLine;
        }
    }

    if ( msgResult.nErrCode == 0 )
    {
        WriteLog( LOGLEVEL_DEBUG, "Client login OK! nSlot = %d, nAccount = %u \n", nSlot, nAccount );

        if ( m_bConnectedAccountServer )
        {
            GameMsg_S2A_RequestPlayerCache requestCacheMsg;
            requestCacheMsg.nAccount = nAccount;
            requestCacheMsg.SessionID = nSessionID;
            SendMsg2AccountServer( &requestCacheMsg );
        }

        m_loginManager.EnteringLoginQueue( nAccount, sharedSlot, m_bConnectedAccountServer );
    }
    else
    {
        // 登入不成功
        WriteLog( LOGLEVEL_ERROR, "Login failed! nSlot= %d, nAccount= %u \n", nSlot, nAccount );

        // 登录失败不让逻辑层继续处理, 通知GateServer
        if ( m_pSocketMgr != NULL )
        {
            m_pSocketMgr->SendGameMsg( &msgResult, nSlot );
        }
    }
}

void CLogicCirculator::OnTouristSignInfo(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    //记录玩家为游客
    const int kTourist = 50;

    GameMsg_C2S_TouristSignInfo *pTouristMsg = (GameMsg_C2S_TouristSignInfo *)&msg;
    CRoleEntity *pRole = (CRoleEntity *)CPlayerManager::Instance().GetEntity(pTouristMsg->nIndex);
    if (pRole != NULL && pTouristMsg->m_nUserType == kTourist)
    {
        CPlayerType *pPlayerType = new CPlayerType();
        pPlayerType->m_nRoleID = pRole->GetRoleID();
        pPlayerType->m_nLastLoginTime = pRole->GetRoleAttr()->GetLastLoginTime();
        pPlayerType->m_nUserType = pTouristMsg->m_nUserType;

        m_pQueryMgr->AddQuery(QUERY_LogPlayerType, 0, pPlayerType, 0, NULL);
    }
}

void CLogicCirculator::OnPlayerLogout(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    DropPlayer( msg.nAccount, true, true, false, DropPlayerCause_Normal );
    WriteLog( LOGLEVEL_DEBUG, "drop out player:OnPlayerLogout" );
}

void CLogicCirculator::OnReportServerInfoRespond(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GameServerLinkInfoResult &rMsg = (GameMsg_G2S_GameServerLinkInfoResult &)msg;

    if (rMsg.m_nServerType == SERVER_GROUP)
    {
        if (!m_bConnectedGroupServer)
        {
            m_bConnectedGroupServer = true;
            WriteLog(LOGLEVEL_SYSTEM, "GroupServer has restore GameServer info.\n");
        }
    }
    else if (rMsg.m_nServerType == SERVER_LOGIN)
    {
        if (!m_bConnectedAccountServer)
        {
            m_bConnectedAccountServer = true;
            WriteLog(LOGLEVEL_SYSTEM, "AccountServer has restore GameServer info.\n");
        }
    }
}

void CLogicCirculator::OnPlayerSwitchLine(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    /*
    CSlotLogic* pSlotLogic = FindAndDeleteSlotLogicDataFromMap(msg.nAccount);
    GameMsg_C2S_SwitchLine *pMsg = (GameMsg_C2S_SwitchLine *)&msg;

    if( pSlotLogic && pSlotLogic->m_nState == CONN_STATE_INGAME)
    {
    if (g_nLine == pMsg->m_nLine) {	//同一条线则返回
    return;
    }

    CRoleEntity* pDstPlayer = CPlayerManager::Instance().GetPlayer(pSlotLogic->tag.m_nIndexLogic);

    GameMsg_S2S_SwitchLineToDstServer toDstMsg;
    // 打包玩家数据
    pDstPlayer->PacketToDB(&toDstMsg.m_roleInfoDB);
    toDstMsg.m_nSrcLine = g_nLine;
    toDstMsg.m_nDstLine = pMsg->m_nLine;
    toDstMsg.m_nLocation = pDstPlayer->IsInRoomHall() ? 0 : 1;
    pDstPlayer->SendPlayerMsg(&toDstMsg);

    OnDropPlayerByServerIndex(pSlotLogic->tag.m_nIndexLogic,false,false,true, DropPlayerCause_PlayerSwitchLine);
    delete pSlotLogic, pSlotLogic = NULL;
    }
    */
    DropPlayer( msg.nAccount, false, false, true, DropPlayerCause_Normal );
}

void CLogicCirculator::OnSendMailFromGMServer(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    GameMsg_M2S_GMSendMail *pMsg = (GameMsg_M2S_GMSendMail *)&msg;

    if (pMsg != NULL) {
        list<CRcvSysMailRoleInfo> *pListRoleInfo = new list<CRcvSysMailRoleInfo>;
        *pListRoleInfo = pMsg->m_listRcvRoleInfo;

        g_pQueryMgr->AddQuery(GetRcvGMSysMailRoleInfo, pMsg->m_nSlot, pListRoleInfo, pMsg->SessionID);
    }
}

void CLogicCirculator::OnQueryMultiGift(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    GameMsg_M2S_QueryRangeGift *pMsg = (GameMsg_M2S_QueryRangeGift *)&msg;
    if (pMsg != NULL) {
        CQueryGiftInfo *pQueryInfo = new CQueryGiftInfo;
        pQueryInfo->m_nGiftID = (unsigned short)pMsg->m_nGiftID;
        pQueryInfo->m_strStartTime = pMsg->m_strStartTime;
        pQueryInfo->m_strEndTime = pMsg->m_strEndTime;

        g_pQueryMgr->AddQuery(QUERY_QueryGiftByTimeRange, 0, pQueryInfo, pMsg->SessionID);
    }
}

void CLogicCirculator::OnGetGiftCodesState(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    GameMsg_M2S_QueryMultiGift *pMsg = (GameMsg_M2S_QueryMultiGift*)&msg;
    if (pMsg != NULL) {
        list<CQueryGiftState> *pListQueryCodes = new list<CQueryGiftState>;
        for (list<string>::iterator itr = pMsg->m_listGiftCodes.begin(); itr != pMsg->m_listGiftCodes.end(); ++itr) {
            CQueryGiftState giftStateInfo;
            giftStateInfo.m_strGiftCode = *itr;
            pListQueryCodes->push_back(giftStateInfo);
        }
        g_pQueryMgr->AddQuery(QUERY_QueryMultiGift, pMsg->m_nSlot, pListQueryCodes, pMsg->SessionID);
    }
}

void CLogicCirculator::OnChargeNotify(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_A2S_ChargeNotify &rMsg = (GameMsg_A2S_ChargeNotify &)msg;

    int nAccount = rMsg.nAccount;
    unsigned int nRoleID = rMsg.nRoleID;
    int nAmount = rMsg.nAmount;
    bool bIsRetryOrder = rMsg.bIsRetryOrder;
    unsigned int nChargeTime = rMsg.nChargeTime;

    // 检查充值条件 (帐号、充值额度、订单号）
    if (nAccount> 0 && rMsg.strOrderId.size() > 0 && nAmount >= 0)
    {
        CRoleEntity *pRole = CPlayerManager::Instance().GetPlayerByAccount(nAccount);
        if (pRole != NULL)
        {
            // 充值日志 & 同步客户端显示, 无DB操作，已在账号服务器操作db
            CRoleAttribute *pRoleAttr = pRole->GetComponent<CRoleAttribute>();
            if (pRoleAttr != NULL)
            {
                pRoleAttr->ChangeBill(nAmount, EChangeBillCause_Add_Charge, false);
                pRoleAttr->m_bCompleteFirstPaymentTag = true; // 完成首充标志
            }

            // 累计充值活动
            if (CCumulativeRechargeMgr::Instance().IsOpening()
                && CCumulativeRechargeMgr::Instance().IsValidRechargeTime(time(NULL)))
            {
                CRoleActivity *pRoleActivity = pRole->GetComponent<CRoleActivity>();
                if (pRoleActivity != NULL)
                {
                    pRoleActivity->OnPlayerRecharge(nAmount);
                }
            }
        }

        // 补发的订单需要邮件通知
        if (bIsRetryOrder)
        {
            SendChargeNoticeMail(nRoleID, nAmount, nChargeTime);
        }
    }
}

void CLogicCirculator::SendChargeNoticeMail(unsigned int nRoleID, int nPoints, unsigned int nChargeTime)
{
    std::string strSender = CLocalization::Instance().GetString(std::string("Charge_Retry_Mail_Sender"));
    std::string strTitle = CLocalization::Instance().GetString(std::string("Charge_Retry_Mail_Title"));
    std::string strContent;

    if (nChargeTime != 0)
    {
        std::string strDateFormat = CLocalization::Instance().GetString(std::string("Date_Format"));
        std::string strTimeFormat = CLocalization::Instance().GetString(std::string("Time_Format"));
        strDateFormat.append(strTimeFormat);

        std::string strTimeBuff;
        LocalizationTime((time_t)nChargeTime, strDateFormat.c_str(), strTimeBuff);

        SafeFormat(strContent, CLocalization::Instance().GetString("Charge_Retry_Mail_Content"),
            strTimeBuff.c_str(), nPoints);
    }
    else
    {
        SafeFormat(strContent, CLocalization::Instance().GetString("Charge_Retry_Mail_Content_GM"),
            nPoints);
    }

    CMailMgr::Instance().SendMailToPlayer( nRoleID, EMailType_ChargeNotice, strSender, strTitle, strContent );
}

void CLogicCirculator::OnSyncChargeSetting(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_A2S_SyncChargeSetting * pSyncMsg = (GameMsg_A2S_SyncChargeSetting *)&msg;

    if (pSyncMsg != NULL)
    {
        CSystemSettingMgr::Instance().SetRecharge(pSyncMsg->m_bEnableRecharge);
        CSystemSettingMgr::Instance().SetFirstPaymentReward(pSyncMsg->m_bEnableFirstRechargeReward);
    }
}

void CLogicCirculator::OnAnnounce(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_Announce & msgAnnounce = (GameMsg_G2S_Announce &)msg;
    if (msgAnnounce.m_announce.m_bShowInChatWindow) {
        OnWorldAnnounce(msgAnnounce.m_announce.m_nPID, msgAnnounce.m_announce.m_strContent.c_str());
    }

    if (msgAnnounce.m_announce.m_bIsAdminChat) {
        OnAdminAnnouce(msgAnnounce.m_announce.m_nPID, msgAnnounce.m_announce.m_strContent);
    }
}

void CLogicCirculator::OnCreateRole(GameMsg_Base &pMsg, CSlotPeer & SlotPeer)
{
    GameMsg_Player_CreateRole *pCreateRoleMsg = (GameMsg_Player_CreateRole *)&pMsg;
    WriteLog(LOGLEVEL_DEBUG, "player try to create role, name = %s ", pCreateRoleMsg->m_szRoleName);
    if (pCreateRoleMsg == NULL)
    {
        return;
    }

    int nSlot = SlotPeer.GetSlot();

#ifndef SERVER_IOS
    bool isValidInstaller = false;
    if (pCreateRoleMsg->nInstallerId == INSTALLER_FRESHER && GetServerInstallerID() == INSTALLER_FRESHER)
    {
        isValidInstaller = true;
    }
    else if (pCreateRoleMsg->nInstallerId == INSTALLER_FULL && GetServerInstallerID() == INSTALLER_FULL)
    {
        isValidInstaller = true;
    }
    else
    {
        isValidInstaller = false;
    }

    //检查安装包版本
    if (!isValidInstaller)
    {
        GameMsg_Player_CreateRoleFail CreateRoleFailMsg;
        CreateRoleFailMsg.m_strResultMsg = CreateRoleFail_InvalidInstaller;
        CreateRoleFailMsg.SessionID = pMsg.SessionID;
        CreateRoleFailMsg.nAccount = pMsg.nAccount;
        g_pSocketMgr->SendGameMsg(&CreateRoleFailMsg, (unsigned short)nSlot);
        return;
    }
#endif //SERVER_IOS

    if (NameDeny::Instance().IsValidText(pCreateRoleMsg->m_szRoleName))
    {
        int nYear = atoi(pCreateRoleMsg->m_szYear);
        int nMonth = atoi(pCreateRoleMsg->m_szMonth);
        int nDay = atoi(pCreateRoleMsg->m_szDay);
        if (IsValidBirthDay(nYear, nMonth, nDay))
        {
            std::string strSignature(pCreateRoleMsg->m_szSignature);
            NameDeny::Instance().FilterInvalidText(strSignature);

            CreateRoleInfo *pCreateRoleInfo = new CreateRoleInfo;
            strcpy(pCreateRoleInfo->szRoleName, pCreateRoleMsg->m_szRoleName);
            pCreateRoleInfo->nSex = pCreateRoleMsg->m_nSex;
            if (pCreateRoleInfo->nSex != ESexType_Male && pCreateRoleInfo->nSex != ESexType_Female)
            {
                pCreateRoleInfo->nSex = ESexType_Female; // 默认女性角色，防止出现脏数据
            }
            Formatter(pCreateRoleInfo->szSignature, MAX_SIGNATURE_LEN) << strSignature;
            pCreateRoleInfo->m_nVersionType = pCreateRoleMsg->m_nVersionType;
            int nBirthDay = nYear * 10000 + nMonth * 100 + nDay;
            pCreateRoleInfo->SetBirthday(nBirthDay);
            pCreateRoleInfo->nColor = pCreateRoleMsg->m_nColor;
            pCreateRoleInfo->nAccount = pCreateRoleMsg->nAccount;
            pCreateRoleInfo->nInstallerId = pCreateRoleMsg->nInstallerId;

            if (g_pQueryMgr != NULL)
            {
                int *pError = new int(ECreateRoleError_OK);
                g_pQueryMgr->AddQuery(QUERY_CreateRole, nSlot, pCreateRoleInfo, pMsg.SessionID, pError);
            }
        }
        else
        {
            GameMsg_Player_CreateRoleFail CreateRoleFailMsg;
            CreateRoleFailMsg.m_strResultMsg = CreateRoleFail_InvalidBirthday;
            CreateRoleFailMsg.SessionID = pMsg.SessionID;
            CreateRoleFailMsg.nAccount = pMsg.nAccount;
            g_pSocketMgr->SendGameMsg(&CreateRoleFailMsg, (unsigned short)nSlot);
        }
    }
    else
    {
        GameMsg_Player_CreateRoleFail CreateRoleFailMsg;
        CreateRoleFailMsg.SessionID = pMsg.SessionID;
        CreateRoleFailMsg.nAccount = pMsg.nAccount;
        CreateRoleFailMsg.m_strResultMsg = CreateRoleFail_InvalidRoleName;
        g_pSocketMgr->SendGameMsg(&CreateRoleFailMsg, (unsigned short)nSlot);
    }
}

void CLogicCirculator::OnCheckRoleName(GameMsg_Base &pMsg, CSlotPeer &Slotpeer)
{
    GameMsg_Player_CheckRoleName *pCheckRoleNameMsg = (GameMsg_Player_CheckRoleName *)&pMsg;
    CQueryTag *pQueryTag = new CQueryTag;
    pQueryTag->m_nSession = pMsg.SessionID;
    pQueryTag->m_nAccount = pMsg.nAccount;
    pQueryTag->m_nSlotPeer = Slotpeer.GetSlot();

    if (pCheckRoleNameMsg != NULL)
    {
        if (!NameDeny::Instance().IsValidText(pCheckRoleNameMsg->m_szRoleName))
        {
            GameMsg_Player_CheckRoleNameResult resultMsg;
            resultMsg.SessionID = pCheckRoleNameMsg->SessionID;
            resultMsg.nAccount = pCheckRoleNameMsg->nAccount;

            resultMsg.m_nResult = CheckRoleName_Fail;
            resultMsg.m_strResult = StrCheckRoleName_NotValid;
            g_pSocketMgr->SendGameMsg(&resultMsg, Slotpeer.GetSlot());
        }
        else
        {
            string *pStrRoleName = new string(pCheckRoleNameMsg->m_szRoleName);
            g_pQueryMgr->AddQuery(QUERY_CheckRoleName, 0, pStrRoleName, 0, pQueryTag);
        }
    }
}

void CLogicCirculator::OnDropClient(GameMsg_Base & pMsg, CSlotPeer & SlotPeer)
{
    GameMsg_Server_DropClient *pDrop = (GameMsg_Server_DropClient*)&pMsg;

    DropPlayer( pDrop->nAccount, false, true, false, pDrop->m_nDropReason );
    WriteLog( LOGLEVEL_DEBUG, "Drop out player:OnDropClient, %u", pDrop->m_nDropReason );
}

void CLogicCirculator::PumpProc()
{
    GameMsg_Base* pMsg = NULL;
    while (m_pSockLogin && (pMsg = m_pSockLogin->GetGameMsg()) != NULL)
    {
        ProcessLoginSvrMsg(pMsg);
        delete pMsg;
        pMsg = NULL;
    }

    while (m_pSockHost && (pMsg = m_pSockHost->GetGameMsg()) != NULL)
    {
        ProcessHostMsg(pMsg);
        delete pMsg;
        pMsg = NULL;
    }

    if (g_pLoginQueryMgr)
    {
        QueryBase* pQueryFinished = NULL;
        while ((pQueryFinished = g_pLoginQueryMgr->GetQueryFinished()) != NULL)
        {
            ProcessQueryFinished(pQueryFinished);

            delete pQueryFinished;
        }
    }

    CCirculator::PumpProc();
}

void CLogicCirculator::ProcessHostMsg(LPGameMsg_Base pHostMsg)
{
    switch (pHostMsg->nMsg)
    {
    case MSG_SYSTEM_Connect:
        break;
    case MSG_SYSTEM_Disconnect:
    {
                                  m_bConnectedGroupServer = false;
                                  WriteLog(LOGLEVEL_SYSTEM, "GroupServer is disconnected.\n");
    }
        break;
    default:
        GameMsg_Map::Instantce().ProcessMsg(*pHostMsg, *m_pHostSlotPeer);
        break;
    }
}

void CLogicCirculator::ProcessLoginSvrMsg(LPGameMsg_Base pGameMsg)
{
    switch (pGameMsg->nMsg)
    {
    case MSG_SYSTEM_Connect:
        break;
    case MSG_SYSTEM_Disconnect:
    {
                                  m_bConnectedAccountServer = false;
                                  WriteLog(LOGLEVEL_SYSTEM, "AccountServer is disconnected.\n");
    }
        break;
    default:
        GameMsg_Map::Instantce().ProcessMsg(*pGameMsg, *m_pLogirSlotPeer);
        break;
    }
}

void CLogicCirculator::OnAgentAnnounce(GameMsg_Base & refMsg, CSlotPeer & SlotPeer)
{
    AgentMsg_Announce *pAnnounce = (AgentMsg_Announce*)&refMsg;
    OnWorldAnnounce(PID_NONE, pAnnounce->szContent.c_str());
}

void CLogicCirculator::OnAgentShutdownInTime(GameMsg_Base & refMsg, CSlotPeer & SlotPeer)
{
    unsigned int nInTime = ((AgentMsg_ShutdownInTime *)&refMsg)->nSeconds;
    if (nInTime < 60 || nInTime > 3600)
        nInTime = 900;
    m_bShutdownInTime = true;
    m_timeShutdown = time(NULL) + nInTime;
}

void CLogicCirculator::OnAgentClose(GameMsg_Base & refMsg, CSlotPeer & SlotPeer)
{
    EnterExitProcess();
}

void CLogicCirculator::OnHostAnnounce(GameMsg_Base & refMsg, CSlotPeer & SlotPeer)
{
    //只要GameServer转发就可以了
    HostMsg_Announce *pAnnounce = (HostMsg_Announce*)&refMsg;
    OnWorldAnnounce(PID_NONE, pAnnounce->szContent.c_str());
}

void CLogicCirculator::OnUpdateLBSPositon(GameMsg_Base & refMsg, CSlotPeer & SlotPeer)
{
    GameMsg_C2S_UpdateLBSPositon *pMsg = (GameMsg_C2S_UpdateLBSPositon*)&refMsg;
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_LBS))
    {
        return;
    }
    CRoleEntity *pRole = CPlayerManager::Instance().GetPlayerByAccount(pMsg->nAccount);
    if (pRole)
    {
        CPlayerInfoComponent* pRoleInfoComponent = pRole->GetComponent<CPlayerInfo>();
        if (!pRoleInfoComponent->IsOpenLBS())
        {
            return;
        }
        time_t tNow = time(NULL);
        if (tNow - pRole->m_tLBSUpPos > CSystemSettingMgr::Instance().m_nUpdateLBSPosInterval - 2)
        {
            GameMsg_S2G_UpdateLBSPositon msg;
            msg.m_nRoleID = pRole->GetRoleID();
            msg.m_fX = pMsg->m_fX;
            msg.m_fY = pMsg->m_fY;
            SendMsg2GroupServer(&msg);
            pRole->m_bUpdatedLBSPositon = true;
            pRole->m_tLBSUpPos = tNow;
        }
    }
}

void CLogicCirculator::OnGetNearbyPlayers(GameMsg_Base & refMsg, CSlotPeer & SlotPeer)
{
    GameMsg_C2S_GetNearbyPlayers *pMsg = (GameMsg_C2S_GetNearbyPlayers*)&refMsg;
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_LBS))
    {
        return;
    }
    CRoleEntity *pRole = CPlayerManager::Instance().GetPlayerByAccount(pMsg->nAccount);
    if (pRole && pRole->m_bUpdatedLBSPositon)
    {
        CPlayerInfoComponent* pRoleInfoComponent = pRole->GetComponent<CPlayerInfo>();
        if (!pRoleInfoComponent->IsOpenLBS())
        {
            return;
        }
        time_t tNow = time(NULL);
        if (tNow - pRole->m_tLBSGetNearby > 3)
        {
            GameMsg_S2G_GetNearbyPlayers msg;
            msg.m_nRoleID = pRole->GetRoleID();
            msg.m_cFlag = pMsg->m_cFlag;
            SendMsg2GroupServer(&msg);
            pRole->m_tLBSGetNearby = tNow;
        }
    }
}

void CLogicCirculator::OnSyncNearbyPlayers(GameMsg_Base & refMsg, CSlotPeer & SlotPeer)
{
    GameMsg_G2S_NearbyPlayers *pMsg = (GameMsg_G2S_NearbyPlayers*)&refMsg;
    CRoleEntity* pRole = CPlayerManager::Instance().GetEntityByRoleID(pMsg->m_nRoleID);
    if (pRole)
    {
        CEntityFriendComponent* pFriendCOM = pRole->GetComponent<CRoleFriend>();
        if (pFriendCOM)
        {
            for (vector<NearbyPlayer>::iterator it = pMsg->m_listNearbyPlayer.begin();
                it != pMsg->m_listNearbyPlayer.end(); ++it)
            {
                FriendInfo* info = pFriendCOM->GetFriend(it->m_nRoleID);
                if (info)
                {
                    it->m_nCallTo = info->m_nCallTo;
                }
            }
        }
        GameMsg_S2C_NearbyPlayers msg;
        msg.m_cFlag = pMsg->m_cFlag;
        msg.m_listNearbyPlayer = pMsg->m_listNearbyPlayer;
        pRole->SendPlayerMsg(&msg);
    }
}

void CLogicCirculator::OnWorldAnnounce(unsigned short nPID, const char * pContent)
{
    //聊天改造
    GameMsg_S2C_Chat *pChatMsg2Client = NULL;
    pChatMsg2Client = GameMsg_S2C_Chat::Create(CLocalization::Instance().GetString("System").c_str(),
        0, eChatSystem, pContent, CHATCOLOR_SYSTEMCHANNEL,
        0, EChatExtraType_Default,
        false, 0, false, 0);
    if (pChatMsg2Client)
    {
        BroadcastPlatformMsg(nPID, pChatMsg2Client);
        delete pChatMsg2Client;
    }
}

void CLogicCirculator::OnAdminAnnouce(unsigned short nPID, std::string & strContent)
{
    GameMsg_S2C_Chat chatAdmine(CLocalization::Instance().GetString("AdminAnnounce").c_str(), 0, eChatAdmin, CHATCOLOR_ADMINCHANNEL,
        strContent.c_str(), 0, EChatExtraType_Default, false, 0, false, 0);
    BroadcastPlatformMsg(nPID, &chatAdmine);
}

void CLogicCirculator::OnConnectedWithGroup()
{
    if (m_pSockHost == NULL)
        return;

    // 通知GroupServer自己是谁
    GameMsg_S2G_GameServerLinkInfo linkInfoMsg;
    BuildLinkInfo(linkInfoMsg);
    m_pSockHost->SendGameMsg(&linkInfoMsg);

    // PlayerManager向GroupServer汇报当前在线的所有玩家信息
    CPlayerManager::Instance().OnConnectedToWorld();
    CAmuseRoomDataManager::Instance().OnGroupServerConnected();
    CeremonyRoomManager::Instance().OnGroupServerConnected();
}

void CLogicCirculator::OnLockOrUnLockAccountByGm(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_LockOrUnLockAccountByGm *pMsg = (GameMsg_G2S_LockOrUnLockAccountByGm *)&msg;
    GameMsg_S2A_LockOrUnLockAccountByGm s2aMsg;
    s2aMsg.nPid = pMsg->m_nPid;
    s2aMsg.strPUID = pMsg->m_strPUID;
    s2aMsg.strTransID = pMsg->m_strTransID;
    s2aMsg.m_bLock = pMsg->m_bLock;
    s2aMsg.m_nLockTimeType = pMsg->m_nLockTimeType;
    SendMsg2AccountServer(&s2aMsg);
}

//<<<<<<
void CLogicCirculator::OnBlessingNotify(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_BlessingNotify & pGNotify = (GameMsg_G2S_BlessingNotify&)msg;

    GameMsg_S2C_BlessingNotify snotify;
    snotify.m_strManName = pGNotify.m_strManName;
    snotify.m_strWomanName = pGNotify.m_strWomanName;
    snotify.m_nPointIndex = pGNotify.m_nPointIndex;

    BroadcastMsg(&snotify);
}

void CLogicCirculator::OnAddMCoinByGm(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_AddMCoinByGm *pMsg = (GameMsg_G2S_AddMCoinByGm *)&msg;
    GameMsg_S2A_AddMCoinByGm s2aMsg;
    s2aMsg.nPid = pMsg->m_nPid;
    s2aMsg.strPUID = pMsg->m_strPUID;
    s2aMsg.nAmount = pMsg->m_nAmount;
    s2aMsg.nBindAmount = pMsg->m_nBindAmount;
    s2aMsg.strTransID = pMsg->m_strTransID;
    s2aMsg.m_nCurrency = pMsg->m_nCurrency;
    SendMsg2AccountServer(&s2aMsg);
}

void CLogicCirculator::OnSendMailFromAccount(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_A2S_SendMail* pMsg = (GameMsg_A2S_SendMail*)&msg;
    MailItem item;
    if (!pMsg->m_mailInfo.m_mapItems.empty())
    {
        mapMailItems::iterator it = pMsg->m_mailInfo.m_mapItems.begin();
        item = it->second;
    }
    CRoleEntity* pRole = CPlayerManager::Instance().GetEntityByRoleID(pMsg->m_nRoleID);
    if (pRole != NULL)//在线
    {
        CEntityMailComponent* pMailCOM = pRole->GetComponent<CRoleMail>();
        if (pMailCOM != NULL)
        {
            pMailCOM->SendMailToPlayer(pMsg->m_nRoleID, (EMailType)pMsg->m_mailInfo.m_nType, pMsg->m_mailInfo.GetTitle().c_str(), pMsg->m_mailInfo.GetContent().c_str(), pMsg->m_mailInfo.m_nMoney, item.m_nType, item.m_nCount, item.m_nValidTime);
        }
    }
    else//不在线
    {
        CMailMgr::Instance().SendSystemMailToPlayer( pMsg->m_nRoleID, (EMailType)pMsg->m_mailInfo.m_nType, pMsg->m_mailInfo.GetTitle(),
            pMsg->m_mailInfo.GetContent(), pMsg->m_mailInfo.m_nMoney, 0, item.m_nType, item.m_nCount, item.m_nValidTime, item.m_nDynamicBoxRelatedValue );
    }
}

void CLogicCirculator::OnGetRoleInfoQueryFinish(QueryBase& refQuery)
{
    Query<ROLEINFO_DB> *pTmp = (Query<ROLEINFO_DB> *)&refQuery;

    if ( pTmp == NULL || !pTmp->IsValid() )
        return;

    SharedPtr<ROLEINFO_DB> sharedRole = pTmp->GetSharedPtr();
    SharedPtr<LogicSlot> sharedSlot = m_loginManager.GetPlayerInfo( sharedRole->nAccount );

    // 防止重复登陆
    if ( !sharedSlot.IsValid() || sharedSlot->m_sharedRole != sharedRole )
        return;

    // 无角色或者角色加载失败，告知客户端跳转至创建角色界面
    if ( pTmp->m_nResult != Query_Res_OK || sharedRole.Get()[0].m_nRoleID == 0 )
    {
        if ( m_pSocketMgr != NULL )
        {
            GameMsg_S2C_RequireCreateRole requireCreateRoleMsg;
            requireCreateRoleMsg.nAccount = sharedSlot->m_tag.m_nAccount;
            requireCreateRoleMsg.SessionID = sharedSlot->m_tag.m_nSessionID;
            m_pSocketMgr->SendGameMsg( &requireCreateRoleMsg, sharedSlot->m_tag.m_nSlot );
        }

        m_loginManager.RemovePlayer( sharedRole->nAccount );
        return;
    }

    // 查到有角色, 并且角色信息加载成功, 创建角色实体
    bool bSuccess = CPlayerManager::Instance().NewPlayer( sharedRole.Get(), &sharedSlot->m_tag, sharedSlot->m_bSwitchLine );
    if ( !bSuccess )
    {
        WriteLog( LOGLEVEL_ERROR, "PlayerManager::NewPlayer failed" );

        m_loginManager.RemovePlayer( sharedRole->nAccount );
        return;
    }

    sharedSlot->SetInGame(); // 转换角色在线状态
    sharedSlot->m_sharedRole = SharedPtr<ROLEINFO_DB>();

    // 发送至Account server通知更改状态为在线AOS_GAME
    GameMsg_Server_AccountLogin msgAccountLogin;
    msgAccountLogin.nAccount = sharedSlot->m_tag.m_nAccount;
    msgAccountLogin.SessionID = sharedSlot->m_tag.m_nSessionID;
    SendMsg2AccountServer( &msgAccountLogin );

    // 发送统计信息
    ReportPlayerCount();

    // 登陆日志
    const char *strRolePriv = "Normal";
    if ( sharedRole->m_nPriv == 0xffffffff )
        strRolePriv = "Admin";
    else if ( sharedRole->m_nPriv != 0 )
        strRolePriv = "GM";
    WriteLog( LOGLEVEL_DEBUG, "[LOGIN] %s rolename: %s roleid: %u accountid: %u login",
        strRolePriv, sharedRole->GetName().c_str(), sharedRole->m_nRoleID, sharedRole->nAccount );
}

void CLogicCirculator::OnDBCreateRole(QueryBase & refQuery)
{
    CreateRoleInfo *pCreateRoleInfo = (CreateRoleInfo *)refQuery.m_pAttachment;
    unsigned short nSlot = (unsigned short)refQuery.m_nAttachment;
    __int64 SessionID = refQuery.m_nTag;
    int *pError = (int *)refQuery.m_pTag;

    if (pError != NULL) {
        if (*pError == ECreateRoleError_OK){
            if (pCreateRoleInfo != NULL) {
                WriteLog(LOGLEVEL_DEBUG, "player create role successfully,role name[%s]", pCreateRoleInfo->szRoleName);
                _OnClientLogin(SessionID, pCreateRoleInfo->nAccount, nSlot, pCreateRoleInfo->nInstallerId, pCreateRoleInfo->m_nVersionType);
            }
        }
        else {
            GameMsg_Player_CreateRoleFail CreateRoleMsgResultMsg;
            CreateRoleMsgResultMsg.SessionID = SessionID;
            CreateRoleMsgResultMsg.nAccount = pCreateRoleInfo->nAccount;

            if (*pError == ECreateRoleError_Duplicated_Role) {
                CreateRoleMsgResultMsg.m_strResultMsg = CreateRoleFail_DuplicatedRole;
            }
            else if (*pError == ECreateRoleError_Duplidated_Name) {
                CreateRoleMsgResultMsg.m_strResultMsg = CreateRoleFail_DuplicatedName;
            }

            g_pSocketMgr->SendGameMsg(&CreateRoleMsgResultMsg, nSlot);
        }
    }

    delete pError, pError = NULL;
    delete pCreateRoleInfo, pCreateRoleInfo = NULL;
}

void CLogicCirculator::OnDBCheckRoleName(QueryBase & refQuery)
{
    QueryBase * pQuery = &refQuery;
    string *pStrRoleName = (string *)pQuery->m_pAttachment;
    CQueryTag *pTag = (CQueryTag *)pQuery->m_pTag;

    GameMsg_Player_CheckRoleNameResult CheckRoleNameMsg;
    CheckRoleNameMsg.nAccount = pTag->m_nAccount;
    CheckRoleNameMsg.SessionID = pTag->m_nSession;

    if (pQuery->m_nResult == Query_Res_Fail)
    {
        CheckRoleNameMsg.m_strResult = StrCheckRoleName_MySQLError;
        CheckRoleNameMsg.m_nResult = CheckRoleName_Fail;
    }
    else if (pQuery->m_nResult == Query_Res_AlreadyExist)
    {
        CheckRoleNameMsg.m_strResult = StrCheckRoleName_Duplicated;
        CheckRoleNameMsg.m_nResult = CheckRoleName_Fail;
    }
    else
    {
        CheckRoleNameMsg.m_nResult = CheckRoleName_Successfully;
        CheckRoleNameMsg.m_strResult = StrCheckRoleName_OK;
    }

    g_pSocketMgr->SendGameMsg(&CheckRoleNameMsg, pTag->m_nSlotPeer);
    delete pStrRoleName, pStrRoleName = NULL;
    delete pTag, pTag = NULL;
}

void CLogicCirculator::OnDBUpdateRole(QueryBase & refQuery)
{
    PLAYERTAG *pTag = (PLAYERTAG *)refQuery.m_pTag;
    if (pTag != NULL)
    {
        if (pTag->m_nAccount > 0) {
            GameMsg_S2G_SwitchLineComplete Msg;
            Msg.nAccount = pTag->m_nAccount;
            Msg.SessionID = pTag->m_nSessionID;
            m_pSocketMgr->SendGameMsg(&Msg, pTag->m_nSlot);
        }

        delete pTag, pTag = NULL;
    }
}

void CLogicCirculator::OnDBGetRcvGMSysMailRoleInfo(QueryBase & refQuery)
{
    __int64 nSessionID = refQuery.m_nTag;

    list<CRcvSysMailRoleInfo> *pListRoleInfo = (list<CRcvSysMailRoleInfo> *)(refQuery.m_pAttachment);

    if (pListRoleInfo != NULL && nSessionID > 0)
    {
        GameMsg_S2M_GMSendMailRes resMsg;
        resMsg.SessionID = nSessionID;
        resMsg.m_nSlot = (unsigned short)refQuery.m_nAttachment;

        if (refQuery.m_nResult == 0)
        {
            for (list<CRcvSysMailRoleInfo>::iterator itr = pListRoleInfo->begin(); itr != pListRoleInfo->end(); ++itr)
            {
                if (itr->m_nRoleID != 0)
                {
                    CMailMgr::Instance().SendSystemMailToPlayer( itr->m_nRoleID, EMailType_GM_Send_Mail, itr->m_MailInfo.m_strTitle, itr->m_MailInfo.m_strContent,
                        itr->m_MailInfo.m_nMoney, 0, itr->m_MailInfo.m_nItemType, (itemcount_t)itr->m_MailInfo.m_nItemCount, itr->m_MailInfo.m_nItemDuration );
                    itr->m_bSuc = true;
                }
                resMsg.m_listRcvRoleInfo.push_back(*itr);
            }
        }
        SendMsg2GroupServer(&resMsg);
    }

    delete pListRoleInfo, pListRoleInfo = NULL;
}

void CLogicCirculator::OnDBQueryGiftByTimeRange(QueryBase & refQuery)
{
    CQueryGiftInfo *pInfo = (CQueryGiftInfo *)refQuery.m_pAttachment;
    __int64 nSessionID = refQuery.m_nTag;

    if (pInfo != NULL) {
        GameMsg_S2M_QueryRangeGiftRes resMsg;
        resMsg.SessionID = nSessionID;
        resMsg.m_nActivatedNum = pInfo->m_nActivatedNum;
        SendMsg2GroupServer(&resMsg);
    }

    delete pInfo, pInfo = NULL;
}

void CLogicCirculator::OnDBQueryMultiGift(QueryBase & refQuery)
{
    list<CQueryGiftState> *pListCodesState = (list<CQueryGiftState> *)(refQuery.m_pAttachment);
    if (pListCodesState != NULL) {
        GameMsg_S2M_QueryMultiGiftRes resMsg;
        resMsg.m_nSolt = (unsigned short)refQuery.m_nAttachment;
        resMsg.SessionID = refQuery.m_nSessionID;
        resMsg.m_listQueryCodes = *pListCodesState;

        SendMsg2GroupServer(&resMsg);
    }
}

void CLogicCirculator::onDBQueryDelVersionMedal(QueryBase & refQuery)
{
    GameMsg_S2G_GMDeleteVersionMedal resMsg;
    if (refQuery.m_nResult == 0)
    {
        resMsg.m_nRetCode = MedalSuc;
    }
    else
    {
        resMsg.m_nRetCode = MedalFail;
    }
    SendMsg2GroupServer(&resMsg);
}

void CLogicCirculator::OnDBQueryAddMedalByGm(QueryBase & refQuery)
{
    GameMsg_G2M_GMAddMedal resMsg;
    if (refQuery.m_nResult == 0)
    {
        resMsg.m_nRetCode = MedalSuc;
    }
    else
    {
        resMsg.m_nRetCode = MedalFail;
    }
    SendMsg2GroupServer(&resMsg);
}

void CLogicCirculator::DropPlayer( unsigned int nAccountID, bool bIsNoticeAccount, bool bNoticeGateServer, bool bIsSwitchServer, int nCause )
{
    if ( nAccountID == 0 )
        return;

    SharedPtr<LogicSlot> sharedSlot = m_loginManager.GetPlayerInfoAndRemove( nAccountID );
    if ( !sharedSlot.IsValid() )
        return;

    CRoleEntity *pRoleEntity = NULL;
    const PLAYERTAG &tag = sharedSlot->m_tag;
    if ( tag.m_nIndexLogic > 0 )
        pRoleEntity = CPlayerManager::Instance().GetPlayer( tag.m_nIndexLogic );

    if ( pRoleEntity != NULL && pRoleEntity->IsValid() )
    {
        if ( nCause == DropPlayerCause_ReLogin )
        {
            pRoleEntity->SysChat( CLocalization::Instance().GetString( "Chat_System_Remind15" ).c_str() );
        }

        CPlayerManager::Instance().LogoutPlayer( pRoleEntity, bIsSwitchServer );

        // 报告人数
        ReportPlayerCount();

        // 通知GroupServer
        GameMsg_S2G_Logout msgLogout( tag.m_nRoleID, g_nLine );
        SendMsg2GroupServer( &msgLogout );

        WriteLog( LOGLEVEL_DEBUG, "Drop Player nAccount = %u , nIndex = %u", nAccountID, tag.m_nIndexLogic );
    }

    if ( bIsNoticeAccount )
    {
        GameMsg_Server_AccountLogout accountMsg( nAccountID );
        SendMsg2AccountServer( &accountMsg );
    }

    // 通知gate玩家登出游戏
    if ( bNoticeGateServer )
    {
        GameMsg_Game2GatePlayerLogout msgPlayerLogout;
        msgPlayerLogout.nAccount = nAccountID;
        msgPlayerLogout.SessionID = tag.m_nSessionID;

        if ( m_pSocketMgr != NULL )
        {
            m_pSocketMgr->SendGameMsg( &msgPlayerLogout, tag.m_nSlot );
            WriteLog( LOGLEVEL_DEBUG, "------send logout message to gate server" );
        }
    }
}

void CLogicCirculator::ReportPlayerCount() const
{
    GameMsg_LinkCount msgLinkCount;
    msgLinkCount.nCount = (unsigned short)m_loginManager.GetOnlinePlayerCount();

    SendMsg2AccountServer( &msgLinkCount );
    SendMsg2GroupServer( &msgLinkCount );
    if ( m_pSocketMgr != NULL )
        m_pSocketMgr->BroadcastMsg( &msgLinkCount );
}

void CLogicCirculator::OnSyncCompensation(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    CompensationMgr::Instance().ClearCompensation();

    GameMsg_G2S_SyncCompensation& sycMsg = (GameMsg_G2S_SyncCompensation&)msg;
    for (std::map<int, CompensationInfo>::iterator it = sycMsg.m_CompensationMap.begin(); it != sycMsg.m_CompensationMap.end(); it++)
    {
        CompensationMgr::Instance().InsertCompensation(it->second);
    }
}

void CLogicCirculator::OnNewCompensation(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_G2S_NewCompensation& ncMsg = (GameMsg_G2S_NewCompensation&)msg;
    CompensationMgr::Instance().InsertCompensation(ncMsg.m_CompensationInfo);

    GameMsg_S2C_NewCompensation cMsg;
    BroadcastMsg(&cMsg);
}

void CLogicCirculator::OnBugReport(GameMsg_Base & msg, CSlotPeer & SlotPeer)
{
    GameMsg_C2S_BugReport &rMsg = (GameMsg_C2S_BugReport &)msg;

    WriteLog(LOGLEVEL_DEBUG, "========== BUG REPORT BEGIN ==========");
    WriteLog(LOGLEVEL_DEBUG, "===>>> %s", rMsg.m_strBugDesc.c_str());
    WriteLog(LOGLEVEL_DEBUG, "========== BUG REPORT END ==========");
}

void CLogicCirculator::SetServerNameInfo(const char * pstrName)
{
    m_ServerName = pstrName;
}
std::string& CLogicCirculator::GetServerName()
{
    return m_ServerName;
}

void CLogicCirculator::SetServerInstallerID(unsigned char nInstallerID)
{
    m_nInstallerID = nInstallerID;
}

unsigned char CLogicCirculator::GetServerInstallerID()
{
    return m_nInstallerID;
}

void CLogicCirculator::SetEnableAudition(bool bEnable)
{
    m_bEnableAudition = bEnable;
}

bool CLogicCirculator::IsEnableAudition() const
{
    return m_bEnableAudition;
}

void CLogicCirculator::BuildLinkInfo(GameMsg_S2G_GameServerLinkInfo &infoMsg)
{
    //这里要注意一下，是否获得的地址是正确的
    unsigned int nAddr;
    s_Host.GetHostAddr(&nAddr, (s_Host.GetHostAddr(NULL, 1).size() > 3) ? 1 : 0);

    infoMsg.m_linkInfo.SetAddr(nAddr);
    infoMsg.m_linkInfo.SetPort(m_pSocketMgr != NULL ? m_pSocketMgr->GetListenPort() : 0);
    infoMsg.m_linkInfo.SetGroup(g_nGroup);
    infoMsg.m_linkInfo.SetLine(g_nLine);
    infoMsg.m_linkInfo.SetName(m_ServerName);
    infoMsg.m_linkInfo.SetInstallerType(m_nInstallerID);
    infoMsg.m_linkInfo.SetEnableAudition(m_bEnableAudition);
}

void CLogicCirculator::OnGetAllBuff(GameMsg_Base & pMsg, CSlotPeer & SlotPeer)
{
    CRoleEntity* pRole = CPlayerManager::Instance().GetPlayerByAccount(pMsg.nAccount);

    if (pRole != NULL)
    {
        if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_BuffBar))
        {
            WriteLog(LOGLEVEL_ERROR, "buffbar function is not open, role %u cannot get buff", pRole->GetRoleID());
            return;
        }
        GameMsg_S2C_GetAllBuff retMsg;
        pRole->PacketBuff(retMsg.m_listBuffData);
        pRole->SendPlayerMsg(&retMsg);
    }
}

void CLogicCirculator::OnUnApplyBuff(GameMsg_Base & msg, CSlotPeer & SlotPeer)
{
    GameMsg_C2S_UnApplyBuff *pMsg = (GameMsg_C2S_UnApplyBuff *)&msg;

    CRoleEntity* pRole = CPlayerManager::Instance().GetPlayerByAccount(pMsg->nAccount);

    if (pRole != NULL)
    {
        if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_BuffBar))
        {
            WriteLog(LOGLEVEL_ERROR, "buffbar function is not open, role %u cannot unapply buff(gene id=%d)", pRole->GetRoleID(), (int)pMsg->m_nGeneId);
            return;
        }
        CEntityGeneComponent* pRoleGene = pRole->GetComponent<CRoleGene>();
        if (pRoleGene != NULL)
        {
            GameMsg_S2C_UnApplyBuff retMsg;
            retMsg.m_nGeneId = pMsg->m_nGeneId;
            GeneData* pGeneData = pRoleGene->FindGene(pMsg->m_nGeneId);
            if (pGeneData != NULL)
            {
                if (CSystemSettingMgr::Instance().CanRemoveBuff(pGeneData->nGeneId))
                {
                    pRoleGene->DelGene(pMsg->m_nGeneId);
                }
                else
                {
                    retMsg.m_strError.assign("UnApplyBuff_CannotRemove");//此buff不能解除
                }
            }
            else
            {
                retMsg.m_strError.assign("UnApplyBuff_NoBuff");//无此buff
            }
            pRole->SendPlayerMsg(&retMsg);
        }
    }
}

void CLogicCirculator::FinishChargeReward(int nChargeRewardType, CRoleEntity *pRole, unsigned int nRoleID,
    const string& strRoleName, Version_Type nVersionType, MailInfo& mailInfo, int nBindMCoinReward, int nMoneyReward)
{
    std::list<CItem> listItemReward;
    unsigned int nNowTime = (unsigned int)time(NULL);
    CEntityAttributeComponent* pRoleAttr = NULL;
    if (pRole != NULL)
    {
        pRoleAttr = pRole->GetComponent<CRoleAttribute>();
        if (pRoleAttr == NULL)
        {
            return;
        }
        if (nChargeRewardType == EChargeRewardType_FirstCharge)
        {
            if (!pRoleAttr->m_bCompleteFirstPaymentTag || !pRoleAttr->m_bCanSendFirstPaymentReward)
            {
                return;
            }
        }
    }

    // 金券奖励，不在线已改成放入邮件
    if (nMoneyReward > 0 && pRoleAttr != NULL)
    {
        pRoleAttr->ChangeMoney(nMoneyReward, nChargeRewardType == EChargeRewardType_FirstCharge ? EChangeMoneyCause_Add_FirstRecharge : EChangeMoneyCause_Add_RechargeReward, 0, false, false);
    }
    // 绑定M币奖励
    if (nBindMCoinReward > 0 && pRoleAttr != NULL)
    {
        pRoleAttr->ChangeBindBill(nBindMCoinReward, nChargeRewardType == EChargeRewardType_FirstCharge ? EChangeBindBillCause_Add_FirstChargeReward : EChangeBindBillCause_Add_ChargeReward, false);
    }
    for (mapMailItems::iterator it = mailInfo.m_mapItems.begin(); it != mailInfo.m_mapItems.end(); ++it)
    {
        CItem item(it->second.m_nType, it->second.m_nCount, it->second.m_nValidTime, false);
        listItemReward.push_back(item);
    }

    // 发送道具奖励，添加至背包或者发邮件
    std::string strTitle = mailInfo.GetTitle();
    std::string strContent = mailInfo.GetContent();
    EMailType eMailType = EMailType_Default;
    if (nChargeRewardType == EChargeRewardType_FirstCharge)
    {
        eMailType = EMailType_First_Recharge_Reward;
    }
    else
    {
        eMailType = EMailType_Recharge_Reward;
    }

    if (mailInfo.m_nMoney > 0)
    {
        CMailMgr::Instance().SendSystemMailToPlayer( nRoleID, eMailType, strTitle, strContent, (pRoleAttr != NULL ? 0 : mailInfo.m_nMoney) );
    }

    strContent.clear();
    std::stringstream ss;
    if (!listItemReward.empty())
    {
        for (std::list<CItem>::iterator it = listItemReward.begin(); it != listItemReward.end(); ++it)
        {
            ss << it->m_nItemType << " ";
        }
        if (pRole != NULL)//在线
        {
            CItemProcess::AddOrMailItems(*pRole, listItemReward, nChargeRewardType == EChargeRewardType_FirstCharge ? EItemAction_Add_FirstRecharge : EItemAction_Add_Charge, 0, true, eMailType, strTitle, strContent);
        }
        else//不在线
        {
            CMailMgr::Instance().SendSystemMailToPlayer( nRoleID, eMailType, strTitle, strContent, listItemReward );
        }
        WriteLog(LOGLEVEL_DEBUG, "role id:%u got charge reward items: %s", nRoleID, ss.str().c_str());
    }

    if (nChargeRewardType == EChargeRewardType_FirstCharge)
    {
        //首充奖励完成，更新数据库事件
        EventInfoDB* pEventInfoDB = new EventInfoDB(nRoleID, (unsigned char)GetChargeSendRewardEventID(nVersionType), 0, nNowTime);
        m_pQueryMgr->AddQuery(QUERY_AddOrUpdateEvent, 0, pEventInfoDB);

        // 聊天框提醒，形成道具奖励名单, 超过两件用"等"代替
        int nCount = listItemReward.size() > 2 ? 2 : listItemReward.size();
        vector<string> vecItemName;
        for (list<CItem>::iterator itr = listItemReward.begin(); itr != listItemReward.end() && nCount > 0; ++itr, --nCount)
        {
            ItemConfig* pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(itr->m_nItemType);
            if (pItemInfo != NULL)
            {
                vecItemName.push_back(pItemInfo->m_strName);
            }
        }

        std::string strBuff;
        if (vecItemName.size() > 0 && vecItemName.size() < 2)
        {
            std::string strNotify = CLocalization::Instance().GetString("Chat_FirstChargeReward1");
            SafeFormat(strBuff, strNotify, strRoleName.c_str(), vecItemName[0].c_str());
        }
        else if (vecItemName.size() >= 2)
        {
            std::string strNotify = CLocalization::Instance().GetString("Chat_FirstChargeReward2");
            SafeFormat(strBuff, strNotify, strRoleName.c_str(), vecItemName[0].c_str(), vecItemName[1].c_str());
        }

        // 聊天框提醒
        GameMsg_S2C_Chat chatMsg(CLocalization::Instance().GetString("System").c_str(), 0, eChatSystem, CHATCOLOR_GLOBALCHANNEL,
            strBuff.c_str(), 0, EChatExtraType_Default, false, 0, false, 0);
        // 广播到所有客户端
        g_pCirculator->BroadCastMsg2AllClient(&chatMsg);

        // 赠送首充奖励 至此完成
        if (pRoleAttr != NULL)
        {
            pRoleAttr->m_bCanSendFirstPaymentReward = false;

            GameMsg_S2C_FinishedFirstPayment msgFirstChargeRewardsFinish;
            msgFirstChargeRewardsFinish.m_listItem = listItemReward;
            msgFirstChargeRewardsFinish.m_nMcoinReward = nBindMCoinReward;
            msgFirstChargeRewardsFinish.m_nMoneyReward = nMoneyReward;
            pRoleAttr->SendPlayerMsg(&msgFirstChargeRewardsFinish);
        }

        if (pRoleAttr != NULL)
        {
            pRoleAttr->SetFirstPaymentInfoLastNotify(PaymentInfoMgr::Instance().GetFirstPaymentInfo(nVersionType, nNowTime));
        }
    }
}

void CLogicCirculator::OnAccountGMSendPrivMCoinRet(GameMsg_Base& pMsg, CSlotPeer& SlotPeer)
{
    SendMsg2GroupServer(&pMsg);
}

void CLogicCirculator::OnAccountSendOnlineMCoin(GameMsg_Base& pMsg, CSlotPeer& SlotPeer)
{
    GameMsg_A2S_SendMCoinOnline& rMsg = (GameMsg_A2S_SendMCoinOnline&)pMsg;
    CRoleEntity* pRole = CPlayerManager::Instance().GetPlayerByAccount(rMsg.m_nAccount);
    if (pRole != NULL)
    {
        CRoleAttribute* pRoleAttr = pRole->GetComponent<CRoleAttribute>();
        if (rMsg.m_nMCoin > 0 && pRoleAttr != NULL)
        {
            pRoleAttr->ChangeBill(rMsg.m_nMCoin, EChangeBillCause_Add_GMClient, false);
        }
        if (rMsg.m_nBindMCoin > 0 && pRoleAttr != NULL)
        {
            pRoleAttr->ChangeBindBill(rMsg.m_nBindMCoin, EChangeBindBillCause_Add_GMClient, false);
        }
    }
}

void CLogicCirculator::OnGMSendBlackDeviceList(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    SendMsg2AccountServer(&msg);
}

void CLogicCirculator::OnGMChangeAccountPassword(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    SendMsg2AccountServer(&msg);
}

void CLogicCirculator::OnSendBlackDeviceListRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    SendMsg2GroupServer(&msg);
}

void CLogicCirculator::OnChangeAccountPasswordRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    SendMsg2GroupServer(&msg);
}

void CLogicCirculator::OnChargeReward(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_A2S_ChargeReward* pMsg = (GameMsg_A2S_ChargeReward*)&msg;
    pMsg->m_nVersionType = Version_Free;
    CRoleEntity* pRole = CPlayerManager::Instance().GetEntityByRoleID(pMsg->m_nRoleID);
    FinishChargeReward(pMsg->m_nChargeRewardType, pRole, pMsg->m_nRoleID, pMsg->m_strRoleName,
        (Version_Type)pMsg->m_nVersionType, pMsg->m_mailInfo, pMsg->m_nRewardBindMCoin, pMsg->m_nRewardMoney);
}

void CLogicCirculator::OnFixedChargeReward(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_A2S_FixedChargeReward* pMsg = (GameMsg_A2S_FixedChargeReward*)&msg;
    ENSURE_WITH_LOG_CMD(pMsg->m_nBindMCoinUpdate != 0, return, "role[%u] reward bindMCoin should not 0 on fixedCharge", pMsg->m_nRoleID);
    ENSURE_WITH_LOG_CMD(pMsg->m_nAmount != 0, return, "role[%u] amount should not 0 on fixedCharge", pMsg->m_nRoleID);

    CRoleEntity* pRole = CPlayerManager::Instance().GetEntityByRoleID(pMsg->m_nRoleID);
    if (NULL == pRole)
    {
        WriteLog(LOGLEVEL_DEBUG, "role[%u] not on line on notify game server...", pMsg->m_nRoleID);
        return; // 不在线不管
    }

    CEntityAttributeComponent* pRoleAttr = pRole->GetComponent<CRoleAttribute>();
    ENSURE_WITH_LOG_CMD(NULL != pRoleAttr, return, "role[%u] missed roleAttr ent...", pMsg->m_nRoleID);

    // 更新奖励领取状态
    pRoleAttr->SetFixedChargeRewardDone(pMsg->m_nAmount);
    // 绑定M币奖励，更新内存，不写数据库
    pRoleAttr->ChangeBindBill(pMsg->m_nBindMCoinUpdate, EChangeBindBillCause_FixedCharge, false);

    return;
}


void CLogicCirculator::OnSendPrivMCoinByGM(GameMsg_Base& pMsg, CSlotPeer& SlotPeer)
{
    SendMsg2AccountServer(&pMsg);
}

void CLogicCirculator::OnGMGetServerLineLimit(GameMsg_Base& pMsg, CSlotPeer& SlotPeer)
{
    SendMsg2AccountServer(&pMsg);
}

void CLogicCirculator::OnAccountSendServerLineLimit(GameMsg_Base& pMsg, CSlotPeer& SlotPeer)
{
    SendMsg2GroupServer(&pMsg);
}

void CLogicCirculator::OnGMSetServerPlayerLimit(GameMsg_Base& pMsg, CSlotPeer& SlotPeer)
{
    SendMsg2AccountServer(&pMsg);
}

void CLogicCirculator::OnGetRoomListByGM(GameMsg_Base& pMsg, CSlotPeer& SlotPeer)
{
    GameMsg_G2S_GetRoomListByGM& rMsg = (GameMsg_G2S_GetRoomListByGM&)pMsg;

    GameMsg_S2G_GetRoomListByGMRes retMsg;
    retMsg.m_nSlot = rMsg.m_nSlot;

    switch (rMsg.m_nGMAskRoomType)
    {
    case 0:
        CRoomManager::Instance().GetRoomListByGM(retMsg.m_RoomList);
        break;
    case 1:
        CAmuseRoomManager::Instance().GetRoomListByGM(retMsg.m_RoomList);
        break;
    case 2:
        CPhotoRoomManager::Instance().GetRoomListByGM(retMsg.m_RoomList);
        break;
    case 3:
        CLiveRoomManager::Instance().GetRoomListByGM(retMsg.m_RoomList);
        break;
    default:
        break;
    }


    SendMsg2GroupServer(&retMsg);
}

void CLogicCirculator::OnGmDeleteVersionMedal(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer)
{
    std::list<int> *pMedallist = new std::list<int>();
    std::list<int> allMedal;
    CMedalCfgMgr::Instance().GetAllMedalIDList(allMedal);
    for (std::list<int>::const_iterator it = allMedal.begin();
        it != allMedal.end(); ++it)
    {
        if (CMedalCfgMgr::Instance().GetMedalType(*it) ==
            CMedalCfgMgr::Instance().GetVersionMedalType())
        {
            pMedallist->push_back(*it);
        }
    }

    g_pLoginQueryMgr->AddQuery(QUERY_Medal_DeleteVersionMedal, 0, pMedallist);
}

void CLogicCirculator::OnGmAddMedal(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_M2G_GMAddMedal& resultMsg = (GameMsg_M2G_GMAddMedal&)msg;
    std::list<int> *plistMedalID = new std::list<int>;
    std::list<int> allMedalList;
    CMedalCfgMgr::Instance().GetAllMedalIDList(allMedalList);

    int nMedalType = CMedalCfgMgr::Instance().GetMedalType(resultMsg.m_nMedalID);
    if (nMedalType == 0)
    {
        GameMsg_G2M_GMAddMedal resMsg;
        resMsg.m_nRetCode = MedalFail;
        SendMsg2GroupServer(&resMsg);
        return;
    }

    for (std::list<int>::iterator it = allMedalList.begin();
        it != allMedalList.end(); ++it)
    {
        if (CMedalCfgMgr::Instance().GetMedalType(*it) == nMedalType)
        {
            plistMedalID->push_back(*it);
        }
    }

    g_pQueryMgr->AddQuery(QUERY_Medal_AddMedalByGm, resultMsg.m_nPlayerID, plistMedalID, resultMsg.m_nMedalID);
}

void CLogicCirculator::OnRequestPlayerCacheRes( GameMsg_Base& msg, CSlotPeer& slotPeer )
{
    GameMsg_A2S_RequestPlayerCacheRes &rMsg = (GameMsg_A2S_RequestPlayerCacheRes &)msg;

    int nRes = m_loginManager.RequestPlayerCacheRes( rMsg.nAccount, rMsg.SessionID, rMsg.m_bCacheSuccess, rMsg.m_bHasCache );
    if ( nRes != ERequestCacheRes_Success )
    {
        GameMsg_Server_LoginResult msgResult;
        msgResult.nAccount = rMsg.nAccount;
        msgResult.SessionID = rMsg.SessionID;
        msgResult.nErrCode = 2;

        if ( m_pSocketMgr != NULL )
            m_pSocketMgr->BroadcastMsg( &msgResult );

        WriteLog( LOGLEVEL_ERROR, "[Cache]Request player cache error(%d).", nRes );
    }
}

void CLogicCirculator::OnPlayerCacheTransferStart( GameMsg_Base& msg, CSlotPeer& slotPeer )
{
    GameMsg_PlayerCacheTransferStart &rMsg = (GameMsg_PlayerCacheTransferStart &)msg;

    m_loginManager.ReceiveCacheDataBegin( rMsg.m_nAccountID );
}

void CLogicCirculator::OnPlayerCacheTransfer( GameMsg_Base& msg, CSlotPeer& slotPeer )
{
    GameMsg_PlayerCacheTransfer &rMsg = (GameMsg_PlayerCacheTransfer &)msg;

    m_loginManager.ReceiveCacheData( rMsg.m_nAccountID, rMsg.m_nKey, rMsg.m_nAllDataSize,
        rMsg.m_nThisSize, rMsg.m_pData );
}

void CLogicCirculator::OnPlayerCacheTransferEnd( GameMsg_Base& msg, CSlotPeer& slotPeer )
{
    GameMsg_PlayerCacheTransferEnd &rMsg = (GameMsg_PlayerCacheTransferEnd &)msg;

    m_loginManager.ReceiveCacheDataEnd( rMsg.m_nAccountID );
}

void CLogicCirculator::OnVerifyAccountRes( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_A2S_VerifyAccountRes *pVerifyResMsg = dynamic_cast<GameMsg_A2S_VerifyAccountRes *>(&msg);
    if ( pVerifyResMsg == NULL )
        return;

    if ( !pVerifyResMsg->m_bVerifyingPassed )
    {
        WriteLog( LOGLEVEL_DEBUG, "Verifying account has not passed, drop the player who switching from the other line." );

        GameMsg_Game2GatePlayerLogout msgPlayerLogout;
        msgPlayerLogout.nAccount = pVerifyResMsg->nAccount;
        msgPlayerLogout.SessionID = pVerifyResMsg->SessionID;

        if ( m_pSocketMgr != NULL )
            m_pSocketMgr->SendGameMsg( &msgPlayerLogout, pVerifyResMsg->m_nSlot );

        return;
    }

    _OnClientLogin( pVerifyResMsg->SessionID, pVerifyResMsg->nAccount, pVerifyResMsg->m_nSlot,
        pVerifyResMsg->m_nInstallerID, pVerifyResMsg->m_nVersionType, true );
}

int SendGameMsg(LPGameMsg_Base pGameMsg, const PLAYERTAG* pTag)
{
    if (pTag && pTag->m_nAccount>0 && pTag->m_nIndexLogic>0 && g_pSocketMgr && pTag->m_nSessionID)
    {
        if (NULL == CPlayerManager::Instance().GetEntity(pTag->m_nIndexLogic))
        {
            WriteLog(LOGLEVEL_ERROR, "PlayerNotExists: Index=%d,Slot=%d,Accout=%u\n", 
                pTag->m_nIndexLogic, pTag->m_nSlot, pTag->m_nAccount);
            return 3;
        }

        pGameMsg->nAccount = pTag->m_nAccount;
        pGameMsg->SessionID = pTag->m_nSessionID;
        pGameMsg->nIndex = pTag->m_nIndexLogic;
        bool bResSend = g_pSocketMgr->SendGameMsg(pGameMsg, pTag->m_nSlot);
        if (!bResSend)
        {
            TRACE("SendFail: Index=%d,Slot=%d,Account=%lld\n", pTag->m_nIndexLogic, pTag->m_nSlot, pTag->m_nAccount);
            return 2;
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

//这里表示向连着当前GameServer的所有GateServer发送消息
void BroadcastMsg(LPGameMsg_Base pMsg)
{
    CPlayerManager::Instance().SendGlobalMessage(pMsg);
}

void BroadcastPlatformMsg(unsigned short nPID, LPGameMsg_Base pMsg)
{
    CPlayerManager::Instance().SendGlobalPlatformMessage(nPID, pMsg);
}

void SendMsg2GroupServer(LPGameMsg_Base pMsg)
{
    ASSERT(g_pSockHost != NULL);
    if (g_pSockHost)
        g_pSockHost->SendGameMsg(pMsg);
}

void SendMsg2AccountServer(LPGameMsg_Base pMsg)
{
    ASSERT(g_pSockLogin != NULL);
    if (g_pSockLogin)
        g_pSockLogin->SendGameMsg(pMsg);
}

void SendMsg2GateServer(LPGameMsg_Base pMsg, int nSlot)
{
    ASSERT(g_pSocketMgr != NULL);
    if (g_pSocketMgr)
        g_pSocketMgr->SendGameMsg(pMsg, (unsigned short)nSlot);
}

