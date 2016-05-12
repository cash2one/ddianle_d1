#include "GMCirculator.h"
#include "../libServerFrame/Main.h"
#include "GMQuery.h"
#include "../lua/InitLua.h"

#include "TipMsgDef.h"
#include "GMEntityMgr.h"
#include "GMEntity.h"
#include "priv/PrivCmdMacroDef.h"

#include "../../socket/GameMsg_Map.h"
#include "../../socket/GameMsg_System_Connect.h"
#include "../../socket/GameMsg_System_Disconnect.h"
#include "../../socket/SharedPtr.h"
#include "../../socket/MsgBaseStruct.h"
#include "../../socket/DBQuery_Map.h"
#include "../../datastructure/DataStruct_DB.h"
#include "GameMsg_GM.h"
#include "GMMsgDef.h"
#include "AnnounceMgr.h"

#include "../chat/GameMsg_G2S_AdminChat.h"
#include "GameMsg_GM_share.h"
#include "TimerMgrGm.h"
#include "priv/PrivMgr.h"
#include "InfoCenterMgr.h"
#include "../Medal/GameMsg_Gm_medal.h"
#include "../util/CommonFunc.h"

#include "JsonLog.h"

extern CIni g_Config;
extern CIni g_GameConfig;
extern int  g_nGroup;
extern int  g_nLine;
extern unsigned int s_nBlockTime[8];

CGMQueryMgr*	g_pGMQueryMgr = NULL;
CGMCirculator* g_pGMCirculator = NULL;

CGMCirculator::CGMCirculator()
{
    g_pGMCirculator = this;
    m_pGMQueryMgr = NULL;
    m_nGMRecGroupSvrInterval = 0;
    m_pSockWorldMgr = NULL;
    m_pSockClientMgr = NULL;

    m_pWorldSlotPeer = NULL;

    m_bGMConnectedToWorld = false;
}

CGMCirculator::~CGMCirculator()
{
    delete m_pSockClientMgr, m_pSockClientMgr = NULL;
    delete m_pSockWorldMgr, m_pSockWorldMgr = NULL;
    delete m_pWorldSlotPeer, m_pWorldSlotPeer = NULL;
}

bool CGMCirculator::Initialize()
{
    bool bRes = CCirculator::Initialize();
    RegisterMessage();
    RegisterMessageProcessor();

    m_nGMRecGroupSvrInterval = g_Config.GetInt("interval", "GMServerreconnectGroupServer", 6000) / m_nElapseHeartBeat;
    if (bRes)
    {
        char szAddr[MAX_NAMEPASS];
        string strSection = "GroupServer";
        g_Config.GetString(strSection.c_str(), "addr", szAddr, MAX_NAMEPASS, "");

        short nPort = (short)g_Config.GetInt(s_szSvrType[SVRTYPE_GROUP], "serverport", 7758);
        if (InitClientSock(m_pSockWorldMgr) && m_pSockWorldMgr->Connect(NULL, szAddr, nPort))
        {
            WriteLog(LOGLEVEL_DEBUG, "GMServer connect GroupServer %s,%d ... OK\n", szAddr, nPort);
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "GMServer connect GroupServer %s,%d ... Failed\n", szAddr, nPort);
        }
    }
    WriteLog(LOGLEVEL_DEBUG, "Begin Init GMQueryMgr....");
    m_pGMQueryMgr = new CGMQueryMgr;
    if (m_pGMQueryMgr != NULL &&
#ifdef WIN32
        m_pGMQueryMgr->Initialize(m_hEventNotify)
#else
        m_pGMQueryMgr->Initialize(&m_condNotify, &m_mutexNotify)
#endif
        )
    {
        WriteLog(LOGLEVEL_DEBUG, "Begin Init GMQueryMgr....Success!");
        g_pGMQueryMgr = m_pGMQueryMgr;
    }
    else
    {
        WriteLog(LOGLEVEL_ERROR, "Begin Init GMQueryMgr....Failure!");
        bRes = false;
    }
    if (g_pGMQueryMgr != NULL) {
        g_pGMQueryMgr->OpenDatabase();
    }
    if (bRes) {
        CInitLua::GetInstance().InitState();
        string sLuaFile = g_sPath + "GM/data/luascript.lua";

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

    m_pWorldSlotPeer = new CSlotPeer;
    m_pWorldSlotPeer->SetSocketMgr(m_pSockWorldMgr);
    m_SlotPeer->SetSocketMgr(m_pSocketMgr);

    InitManager();

    return bRes;
}

void CGMCirculator::Release()
{
}

void CGMCirculator::PumpProc()
{
    GameMsg_Base* pMsg = NULL;
    while (m_pSockWorldMgr && (pMsg = m_pSockWorldMgr->GetGameMsg()) != NULL)
    {
        ProcessWorldMessage(pMsg);
        delete pMsg;
        pMsg = NULL;
    }

    if (g_pGMQueryMgr)
    {
        QueryBase* pQueryFinished = NULL;
        while ((pQueryFinished = g_pGMQueryMgr->GetQueryFinished()) != NULL)
        {
            ProcessQueryFinished(pQueryFinished);

            delete pQueryFinished;
        }
    }

    //if(m_bExiting)//如果正在退出流程
    //{
    //	if((g_pGMQueryMgr && g_pGMQueryMgr->IsQueueEmpty()) || g_pGMQueryMgr == NULL)//查询都处理完成
    //	{
    //		//再等待3秒
    //		millisecondSleep(3000);
    //		m_bRunning = false;
    //	}
    //}

    CCirculator::PumpProc();
}

void CGMCirculator::ProcessQueryFinished(QueryBase* pQuery)
{
    if (pQuery != NULL) {
        CDBQuery_Map::Instantce().ProcessMsg(*pQuery);
    }
}

void CGMCirculator::ProcessHeartBeat(int& timeElapse)
{
    CCirculator::ProcessHeartBeat(timeElapse);

    if (m_nHeartBeatCount % m_nGMRecGroupSvrInterval == 0 && m_pSockWorldMgr != NULL){
        m_pSockWorldMgr->Reconnect();
    }

    if (m_nHeartBeatCount % 6000 == 0 && m_pGMQueryMgr && !m_pGMQueryMgr->GetThreadState()){
        m_pGMQueryMgr->RecreateThread();
    }

    CTimerMgr::Instance().OnUpdate(timeElapse);
}

void CGMCirculator::RegisterMessage()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GMLogin);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GMSendMail);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2M_GMSendMailRes);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_QueryRangeGift);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2M_QueryRangeGiftRes);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_QueryMultiGift);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2M_QueryMultiGiftRes);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GrantGMCmd);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RemoveGMCmd);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2M_SendGroupServerInfoRes);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GMLockOrUnLockAccount);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GMAddMCoin);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SendCompensation);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2M_UpdateCompensationRes);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetUsers);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SetPriv);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_AddUser);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DelUser);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GMGetServerLine);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GMGetServerRoomList);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2M_GetServerLineRes);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2M_GetRoomListRes);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GMSetMallUpdateTime);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GMGroupBuyControl);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeSelfPassword);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangePassword);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GMSendPrivMCoin);
    GAMEMSG_REGISTERCREATOR(GameMsg_A2M_SendPrivMCoinRet);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetServerPlayerInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SetServerPlayerInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2M_GetServerPlayerCountInfoRet);
    GAMEMSG_REGISTERCREATOR(GameMsg_A2G_GetServerPlayerLimitRet);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeAccountPassword);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2G_DeleteVersionMedal);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GMDeleteVersionMedal);
   // GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SendBlackDeviceList);
}

void CGMCirculator::RegisterMessageProcessor()
{
    RegMsgProcessor(MSG_SYSTEM_Connect, &CGMCirculator::OnClientConnect);
    RegMsgProcessor(MSG_SYSTEM_Disconnect, &CGMCirculator::OnClientDisconnect);
    RegMsgProcessor(GM_C2S_Login, &CGMCirculator::OnGMLogin);
    RegMsgProcessor(GM_C2S_SendMail, &CGMCirculator::OnGMSendMail);
    RegMsgProcessor(MSG_GM_S2M_SendMailRes, &CGMCirculator::OnGMSendMailRes);
    RegMsgProcessor(GM_C2S_QueryRangeGift, &CGMCirculator::OnGMQueryGiftByTimeRange);
    RegMsgProcessor(MSG_GM_S2M_QueryRangeGiftRes, &CGMCirculator::OnGMQueryGiftByTimeRangeRes);
    RegMsgProcessor(GM_C2S_QueryMultiGift, &CGMCirculator::OnGMQueryMultiGift);
    RegMsgProcessor(MSG_GM_S2M_QueryMultiGiftRes, &CGMCirculator::OnGMQueryMultiGiftRes);
    RegMsgProcessor(GM_C2S_GrantGMCmds, &CGMCirculator::OnGrantGMCmd);
    RegMsgProcessor(GM_C2S_RemoveGMCmds, &CGMCirculator::OnRemoveGrantedCmd);
    RegMsgProcessor(MSG_GM_G2S_SendGroupServerInofRes, &CGMCirculator::OnGMServerReportWorldInfoRes);
    RegMsgProcessor(GM_C2S_GMLockOrUnLockAccount, &CGMCirculator::OnLockAccount);
    RegMsgProcessor(GM_C2S_GMAddMCoin, &CGMCirculator::OnAddMCoin);
    RegMsgProcessor(GM_C2S_SendCompensation, &CGMCirculator::OnSendCompensation);
    RegMsgProcessor(MSG_GM_G2M_UpdateCompensationRes, &CGMCirculator::OnUpdateCompensationRes);
    RegMsgProcessor(GM_C2S_GetUsers, &CGMCirculator::OnGetUsers);
    RegMsgProcessor(GM_C2S_SetPriv, &CGMCirculator::OnSetPriv);
    RegMsgProcessor(GM_C2S_AddUser, &CGMCirculator::OnAddUser);
    RegMsgProcessor(GM_C2S_DelUser, &CGMCirculator::OnDelUser);
    RegMsgProcessor(GM_C2S_GetServerLine, &CGMCirculator::OnGetServerLine);
    RegMsgProcessor(GM_C2S_GetServerRoomList, &CGMCirculator::OnGetServerRoomList);
    RegMsgProcessor(MSG_GM_G2M_GetServerLineRes, &CGMCirculator::OnGroupGetServerLineRes);
    RegMsgProcessor(MSG_GM_G2M_GetRoomListRes, &CGMCirculator::OnGroupGetRoomListRes);
    RegMsgProcessor(GM_C2S_SetMallUpdateTime, &CGMCirculator::OnSetMallUpdateTime);
    RegMsgProcessor(GM_C2S_GroupBuyControl, &CGMCirculator::OnGroupBuyControl);
    RegMsgProcessor(GM_C2S_ChangeSelfPassword, &CGMCirculator::OnChangeSelfPassword);
    RegMsgProcessor(GM_C2S_ChangePassword, &CGMCirculator::OnChangePassword);
    RegMsgProcessor(GM_C2S_SendPrivMCoin, &CGMCirculator::OnPrivAddMCoin);
    RegMsgProcessor(MSG_GM_A2M_SendPrivMCoinRet, &CGMCirculator::OnPrivAddMCoinRet);
    RegMsgProcessor(GM_C2S_GetServerPlayerInfo, &CGMCirculator::OnGetServerPlayerCountInfo);
    RegMsgProcessor(GM_C2S_SetServerPlayerInfo, &CGMCirculator::OnSetServerPlayerCountInfo);
    RegMsgProcessor(MSG_GM_G2M_GetServerPlayerCountInfoRet, &CGMCirculator::OnGetServerPlayerCountInfoRet);
    RegMsgProcessor(MSG_GM_A2M_GetServerPlayerLimitRet, &CGMCirculator::OnGetServerPlayerLimitRet);
    RegMsgProcessor(GM_C2S_ChangeAccountPassword, &CGMCirculator::OnChangeAccountPassword);
    RegMsgProcessor(GM_M2G_DeleteVersionMedal, &CGMCirculator::OnDeletVersionMedal);
    RegMsgProcessor(MSG_GM_S2G_DeleteVersionMedal, &CGMCirculator::OnDeletVersionMedalRet);
    //RegMsgProcessor(GM_C2S_SendBlackDeviceList, &CGMCirculator::OnSendBlackDeviceList);

    RegMsgCreatorAndProcessor(GameMsg_A2M_SendBlackDeviceListRet, &CGMCirculator::OnSendBlackDeviceListRet);
    RegMsgCreatorAndProcessor(GameMsg_A2M_ChangeAccountPasswordRet, &CGMCirculator::OnChangeAccountPasswordRet);
    RegMsgCreatorAndProcessor(GameMsg_C2M_AddMedal, &CGMCirculator::OnAddMedal);
    RegMsgCreatorAndProcessor(GameMsg_G2M_GMAddMedal, &CGMCirculator::OnAddMedalRet);
    RegMsgCreatorAndProcessor(GameMsg_C2M_RequestLockList, &CGMCirculator::OnRequestLockList);
    RegMsgCreatorAndProcessor(GameMsg_C2M_HarassList, &CGMCirculator::OnRequestHarassList);
    RegMsgCreatorAndProcessor(GameMsg_C2M_RoleHarassChatInfo, &CGMCirculator::OnRequestRoleHarassChatInfo);
    RegMsgCreatorAndProcessor(GameMsg_C2S_QueryLog, &CGMCirculator::OnRequestQueryLog);
    RegMsgCreatorAndProcessor(GameMsg_C2S_QueryLogItem, &CGMCirculator::OnRequestQueryLogItem);
    RegMsgCreatorAndProcessor(GameMsg_C2M_RequestRechargeRecordList, &CGMCirculator::OnRequestRechargeRecordList);
    RegQueryProcessor(Query_GMCreateAccount, &CGMCirculator::CreateGMAccountDBCB);
    RegQueryProcessor(Query_GetGMAccInfo, &CGMCirculator::GMLoginDBCB);
    RegQueryProcessor(Query_GetGMRoleInfo, &CGMCirculator::GMGetRoleInfoDBCB);
    RegQueryProcessor(Query_InsertGrantedCmd, &CGMCirculator::GrantGMCmdDBCB);
    RegQueryProcessor(Query_RemoveGrantedCmd, &CGMCirculator::RemoveGMCmdDBCB);
    RegQueryProcessor(Query_InsertSysMailLog, &CGMCirculator::InsertSysMailLogDBCB);
    RegQueryProcessor(Query_AddUser, &CGMCirculator::AddUserDBCB);
    RegQueryProcessor(Query_SetPriv, &CGMCirculator::SetPrivDBCB);
    RegQueryProcessor(Query_DelUser, &CGMCirculator::DelUserDBCB);
    RegQueryProcessor(Query_ChangePassword, &CGMCirculator::ChangePasswdDBCB);
    RegQueryProcessor(Query_PlayerLockQuery, &CGMCirculator::OnQueryPlayerLockCB);
    RegQueryProcessor(Query_HarassPlayers, &CGMCirculator::OnQueryHarassListCB);
    RegQueryProcessor(Query_HarassPlayerChat, &CGMCirculator::OnQueryRoleHarassChatInfoCB);
    RegQueryProcessor(Query_RechagreRecordList, &CGMCirculator::OnQueryRechargeRecordList);    
    RegQueryProcessor(Query_LogLoad, &CGMCirculator::OnQueryLogLoad);
    RegQueryProcessor(Query_LogItemLoad, &CGMCirculator::OnQueryLogItemLoad);
}

void CGMCirculator::ProcessWorldMessage(LPGameMsg_Base pGameMsg)
{
    switch (pGameMsg->nMsg)
    {
    case MSG_SYSTEM_Connect:
        if (((GameMsg_System_Connect*)pGameMsg)->nConnCode == 0)
        {
            WriteLog(LOGLEVEL_DEBUG, "GroupServer connected, report GM server.\n");
            GameMsg_M2G_SendGroupServerInfo reportMsg;
            SendMsgToGroup(&reportMsg);
        }
        break;
    case MSG_SYSTEM_Disconnect:
    {
                                  m_bGMConnectedToWorld = false;
                                  WriteLog(LOGLEVEL_DEBUG, "Disconnect from GroupServer\n");
    }
        break;
    default:
        GameMsg_Map::Instantce().ProcessMsg(*pGameMsg, *m_pWorldSlotPeer);
        break;
    }
}

void CGMCirculator::OnClientConnect(GameMsg_Base & pMsg, CSlotPeer & slotPeer)
{
    WriteLog(LOGLEVEL_DEBUG, "client connected, slot %d\n", slotPeer.GetSlot());
    GMEntity *pEntity = new GMEntity;
    pEntity->SetSession(GMEntityMgr::Instance().GetNewSession());
    pEntity->SetSlot(slotPeer.GetSlot());
    GMEntityMgr::Instance().AddSlotEntity(pEntity->GetSlot(), pEntity);
}

void CGMCirculator::OnClientDisconnect(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    WriteLog(LOGLEVEL_DEBUG, "client disconnected, slot %d\n", slotPeer.GetSlot());
    GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pEntity != NULL) {
        GMEntityMgr::Instance().PlayerLogout(pEntity);
        delete pEntity, pEntity = NULL;
    }
}

void CGMCirculator::OnGMServerReportWorldInfoRes(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    if (!m_bGMConnectedToWorld) {
        m_bGMConnectedToWorld = true;
        WriteLog(LOGLEVEL_DEBUG, "GMServer reporeted info to world server successfully\n");
    }
}

void CGMCirculator::OnGMSendMail(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_SendSystemMail))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }
        GameMsg_C2S_GMSendMail *pMsg = (GameMsg_C2S_GMSendMail *)&msg;
        std::string strJson = GMJsonLog::SendSystemMailToJson(pMsg->m_listRcvRoleInfo);
        LogGM(pRole->GetAccount(), EGMActionType_SendSystemMail, strJson);

        if (pMsg != NULL)
        {
            GameMsg_M2S_GMSendMail sendMsg;
            sendMsg.m_nSlot = slotPeer.GetSlot();
            sendMsg.m_listRcvRoleInfo = pMsg->m_listRcvRoleInfo;
            pRole->SendMsgToGroup(&sendMsg);
        }
    }
}

void CGMCirculator::OnGMSendMailRes(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_S2M_GMSendMailRes *pMsg = (GameMsg_S2M_GMSendMailRes *)&msg;
    if (pMsg != NULL) {
        GMEntity *pRole = GMEntityMgr::Instance().GetEntityBySlot(pMsg->m_nSlot);
        if (pRole != NULL) {
            GameMsg_S2C_GMSendMailRes resMsg;
            resMsg.m_listRcvRoleInfo = pMsg->m_listRcvRoleInfo;
            pRole->SendMsgToGM(&resMsg);

            for (list<CRcvSysMailRoleInfo>::iterator itr = pMsg->m_listRcvRoleInfo.begin(); itr != pMsg->m_listRcvRoleInfo.end(); ++itr) {
                if (itr->m_bSuc) {
                    CGMSysMailLog *pMailLog = new CGMSysMailLog;
                    pMailLog->m_nGMAccount = pRole->GetAccount();
                    pMailLog->m_strPUID = itr->m_strPUID;
                    pMailLog->m_strRoleName = itr->m_strRoleName;
                    pMailLog->m_strMailTitle = itr->m_MailInfo.m_strTitle;
                    pMailLog->m_strMailContent = itr->m_MailInfo.m_strContent;
                    pMailLog->m_nItemType = itr->m_MailInfo.m_nItemType;
                    pMailLog->m_nItemCount = (unsigned short)itr->m_MailInfo.m_nItemCount;
                    pMailLog->m_nValidTime = itr->m_MailInfo.m_nItemDuration;
                    pMailLog->m_nMoney = itr->m_MailInfo.m_nMoney;
                    pMailLog->m_nSendTime = time(NULL);

                    g_pGMQueryMgr->AddQuery(Query_InsertSysMailLog, 0, pMailLog);
                }
            }
        }
    }
}

void CGMCirculator::OnGMLogin(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GMLogin *pMsg = (GameMsg_C2S_GMLogin *)&msg;
    if (pMsg == NULL) {
        return;
    }
    GMEntity *pLoginingPlayer = GetLoginingPlayer(slotPeer.GetSlot());
    if (pLoginingPlayer != NULL) {
        return;
    }

    GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pEntity != NULL) {
        AddLoginingMap(slotPeer.GetSlot(), pEntity);

        GMAccountInfo *pInfo = new GMAccountInfo;
        memcpy(pInfo->m_szName, pMsg->m_szName, GM_MAX_NAME_LEN - 1);
        pInfo->m_nSessionID = pEntity->GetSession();
        pInfo->m_nSlot = pEntity->GetSlot();

        char *pPasswd = new char[GM_MAX_PASSWD_LEN];
        memset(pPasswd, 0, GM_MAX_PASSWD_LEN);
        memcpy(pPasswd, pMsg->m_szPasswd, GM_MAX_PASSWD_LEN - 1);

        g_pGMQueryMgr->AddQuery(Query_GetGMAccInfo, 0, pInfo, 0, pPasswd);
    }
}

void CGMCirculator::OnGMLogout(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pEntity != NULL) {
        GMEntityMgr::Instance().PlayerLogout(pEntity);
        m_pSockClientMgr->DisconnectClient(slotPeer.GetSlot(), EDisconnectReason_Normal);
        delete pEntity, pEntity = NULL;
    }
}

void CGMCirculator::OnGMQueryGiftByTimeRange(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_GenGiftCode))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }
        GameMsg_C2S_QueryRangeGift *pMsg = (GameMsg_C2S_QueryRangeGift *)&msg;
        std::string strJson = pMsg->ToJson();
//         LogGM(pRole->GetAccount(), EGMActionType_QueryGiftCode, strJson);
        if (pMsg != NULL) {
            GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
            if (pEntity != NULL) {
                GameMsg_M2S_QueryRangeGift sendMsg;
                sendMsg.m_nGiftID = pMsg->m_nGiftID;
                sendMsg.m_strStartTime = pMsg->m_strStartTime;
                sendMsg.m_strEndTime = pMsg->m_strEndTime;
                GetActivateGiftNum(sendMsg, *pEntity);
            }
        }
    }
}

void CGMCirculator::OnGMQueryGiftByTimeRangeRes(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GMEntity *pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pRole != NULL)
    {
        GameMsg_S2C_QueryRangeGiftRes resMsg;
        resMsg.m_nActivatedNum = ((GameMsg_S2M_QueryRangeGiftRes &)msg).m_nActivatedNum;
        pRole->SendMsgToGM(&resMsg);
    }
}

void CGMCirculator::OnGMQueryMultiGift(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_GenGiftCode))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }
        GameMsg_C2S_QueryMultiGift *pMsg = (GameMsg_C2S_QueryMultiGift *)&msg;
        if (pMsg != NULL)
        {
//             std::string strJson = pMsg->ToJson();
//             LogGM(pRole->GetAccount(), EGMActionType_QueryGiftCode, strJson);

            GameMsg_M2S_QueryMultiGift sendMsg;
            sendMsg.m_listGiftCodes = pMsg->m_listGiftCodes;
            sendMsg.m_nSlot = slotPeer.GetSlot();
            GetGiftCodesState(sendMsg, *pRole);
        }
    }
}

void CGMCirculator::OnGMQueryMultiGiftRes(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_S2M_QueryMultiGiftRes *pMsg = (GameMsg_S2M_QueryMultiGiftRes *)&msg;
    if (pMsg != NULL) {
        GMEntity *pRole = GMEntityMgr::Instance().GetEntityBySlot(pMsg->m_nSolt);
        if (pRole != NULL) {
            GameMsg_S2C_QueryMultiGiftRes resMsg;
            resMsg.m_listQueryCodes = pMsg->m_listQueryCodes;
            pRole->SendMsgToGM(&resMsg);
        }
    }
}

void CGMCirculator::OnGrantGMCmd(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_UserPriv))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }
        GameMsg_C2S_GrantGMCmd *pMsg = (GameMsg_C2S_GrantGMCmd *)&msg;
        if (pMsg != NULL) {
            GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
            if (pEntity != NULL) {
                string strCmd = string(GRANTGMCMD);
                if (pEntity->HasCmdPriv(strCmd)) {
                    char *pName = new char[GM_MAX_NAME_LEN];
                    memset(pName, 0, GM_MAX_NAME_LEN);
                    memcpy(pName, pMsg->m_szName, GM_MAX_NAME_LEN - 1);
                    list<string> *pListCmd = new list<string>;
                    *pListCmd = pMsg->m_listGrantCmds;

                    g_pGMQueryMgr->AddQuery(Query_InsertGrantedCmd, 1, pName, slotPeer.GetSlot(), pListCmd);
                }
                else {
                    GameMsg_S2C_GrantGMCmdFail failMsg;
                    memcpy(failMsg.m_szName, pEntity->GetName(), GM_MAX_NAME_LEN - 1);
                    failMsg.m_nErrno = GrantCmd_NotHavePriv;
                    pEntity->SendMsgToGM(&failMsg);
                }
            }
        }
    }
}

void CGMCirculator::OnRemoveGrantedCmd(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_Compsition))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }
        GameMsg_C2S_RemoveGMCmd *pMsg = (GameMsg_C2S_RemoveGMCmd *)&msg;
        if (pMsg != NULL) {
            GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
            if (pEntity != NULL) {
                string strCmd = string(REMOVEGMCMD);
                if (pEntity->HasCmdPriv(strCmd)) {
                    char *pName = new char[GM_MAX_NAME_LEN];
                    memset(pName, 0, GM_MAX_NAME_LEN);
                    memcpy(pName, pMsg->m_szName, GM_MAX_NAME_LEN - 1);
                    list<string> *pListCmd = new list<string>;
                    *pListCmd = pMsg->m_listRemoveCmds;

                    g_pGMQueryMgr->AddQuery(Query_RemoveGrantedCmd, 1, pName, slotPeer.GetSlot(), pListCmd);
                }

            }
        }
    }
}
void CGMCirculator::OnLockAccount(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_PlayerFrozen))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2S_GMLockOrUnLockAccount *pMsg = (GameMsg_C2S_GMLockOrUnLockAccount *)&msg;
        std::string strJson = GMJsonLog::LockPlayerToJson(pMsg->m_nPid, pMsg->m_strPUID, pMsg->m_bLock, pMsg->m_nLockTimeType, pMsg->m_nLockReason);
        LogGM(pRole->GetAccount(), EGMActionType_PlayerFrozen, strJson);
        int nMaxLockTimeLevel = sizeof(s_nBlockTime) / sizeof(s_nBlockTime[0]);

        if (pMsg->m_nLockTimeType < 0 || pMsg->m_nLockTimeType >= nMaxLockTimeLevel)
        {
            return;
        }

        PlayerLockRequestDb *request = new PlayerLockRequestDb;
        request->m_nPid = pMsg->m_nPid;
        request->m_strPuid = pMsg->m_strPUID;
        request->m_bIsLocked = pMsg->m_bLock;
        request->m_nLockType = (short)pMsg->m_nLockTimeType;
        request->m_nLockReason = pMsg->m_nLockReason; // 没有原因
        request->m_strOperatorName = pRole->GetName(); // 管理员的名字
        g_pGMQueryMgr->AddQuery(Query_WriteLockPlayerRecord, 0, request, 0, NULL);

        GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
        if (pEntity != NULL)
        {
            std::string strTransID = pEntity->GetName();

            GameMsg_M2G_LockOrUnLockByGm sendMsg;
            sendMsg.m_nPid = pMsg->m_nPid;
            sendMsg.m_strPUID = pMsg->m_strPUID;
            sendMsg.m_strTransID = strTransID;
            sendMsg.m_bLock = pMsg->m_bLock;
            sendMsg.m_nLockTimeType = pMsg->m_nLockTimeType;
            SendMsgToGroup(&sendMsg);
        }
    }
}

void CGMCirculator::OnAddMCoin(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_SendMCoin))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2S_GMAddMCoin *pMsg = (GameMsg_C2S_GMAddMCoin *)&msg;
        std::string strJson = GMJsonLog::SendMCoinToJson(pMsg->m_nPid, pMsg->m_strPUID, pMsg->m_nAmount, pMsg->m_nBindAmount);
        LogGM(pRole->GetAccount(), EGMActionType_SendMCoin, strJson);
        GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
        if (pEntity != NULL)
        {
            std::string strTransID = pEntity->GetName();

            GameMsg_M2G_AddMCoinByGm sendMsg;
            sendMsg.m_nPid = pMsg->m_nPid;
            sendMsg.m_strPUID = pMsg->m_strPUID;
            sendMsg.m_nAmount = pMsg->m_nAmount;
            sendMsg.m_nBindAmount = pMsg->m_nBindAmount;
            sendMsg.m_strTransID = strTransID;
            SendMsgToGroup(&sendMsg);
        }
    }
}

void CGMCirculator::SendMsgToGroup(GameMsg_Base *pMsg)
{
    m_pSockWorldMgr->SendGameMsg(pMsg);
}

void CGMCirculator::SendMsgToGM(GameMsg_Base *pMsg, unsigned short nSlot)
{
    m_pSocketMgr->SendGameMsg(pMsg, nSlot);
}

void CGMCirculator::GetActivateGiftNum(GameMsg_Base &msg, GMEntity &entity)
{
    string strCmd = string(QUERYGIFTCODE);
    if (entity.HasCmdPriv(strCmd)) {
        entity.SendMsgToGroup(&msg);
    }
}

void CGMCirculator::GetGiftCodesState(GameMsg_Base &msg, GMEntity &entity)
{
    entity.SendMsgToGroup(&msg);
}

bool CGMCirculator::HasPriv(int nEvent, int nPriv)
{
    return true;
}

void CGMCirculator::KickGM(GMEntity *pEntity)
{
    if (pEntity != NULL) {
        GMEntityMgr::Instance().PlayerLogout(pEntity);
        m_pSockClientMgr->DisconnectClient(pEntity->GetSlot(), EDisconnectReason_Normal);// 会走onClientDisconnect回调,但不走完
        delete pEntity, pEntity = NULL;
    }
}

void CGMCirculator::CreateGMAccountDBCB(QueryBase & query)
{
    WriteLog(LOGLEVEL_DEBUG, "Hi, This is Create GM Account DB call back!\n");

}

void CGMCirculator::GMLoginDBCB(QueryBase & query)
{
    WriteLog(LOGLEVEL_DEBUG, "This is GMLoginDBCB!\n");
    GMAccountInfo *pAccount = (GMAccountInfo *)query.m_pAttachment;
    char *szPasswd = (char *)query.m_pTag;

    GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(pAccount->m_nSlot);
    if (query.m_nResult == 0 && pEntity != NULL && pAccount != NULL && szPasswd != NULL) {
        if (strcmp(szPasswd, pAccount->m_szPasswd) == 0) {
            GMEntity *pOnlineEntity = GMEntityMgr::Instance().GetEntityByAccount(pAccount->GetAccount());
            if (pOnlineEntity != NULL) {
                GMEntityMgr::Instance().PlayerLogout(pOnlineEntity);

                GameMsg_S2C_KickPlayer kickMsg;
                kickMsg.m_nReason = PlayerReLogin;
                pOnlineEntity->SendMsgToGM(&kickMsg);
                delete pOnlineEntity, pOnlineEntity = NULL;
            }
            CGMRoleInfoDB *pRoleInfo = new CGMRoleInfoDB;
            pRoleInfo->m_nAccount = pAccount->GetAccount();
            memcpy(pRoleInfo->m_szName, pAccount->m_szName, GM_MAX_NAME_LEN - 1);
            pRoleInfo->m_nSessionID = pAccount->GetSession();
            pRoleInfo->m_nSlot = pAccount->GetSlot();

            g_pGMQueryMgr->AddQuery(Query_GetGMRoleInfo, pAccount->m_nSlot, pRoleInfo);
            WriteLog(LOGLEVEL_DEBUG, "GM verified successfully.slot = %u, session = %lld, account = %u\n", pEntity->GetSlot(), pEntity->GetSession(), pEntity->GetAccount());
        }
        else {
            RemoveLoginingMap(pAccount->GetSlot());
            GameMsg_S2C_GMLoginFail failMsg;
            failMsg.m_nError = GMLoginFail;
            pEntity->SendMsgToGM(&failMsg);
        }
    }
    else {
        WriteLog(LOGLEVEL_ERROR, "GM Login DB callback error.\n");
        RemoveLoginingMap(pAccount->GetSlot());
    }

    delete pAccount, pAccount = NULL;
    delete szPasswd, szPasswd = NULL;
}

void CGMCirculator::GMGetRoleInfoDBCB(QueryBase & query)
{
    CGMRoleInfoDB *pRoleInfo = (CGMRoleInfoDB *)query.m_pAttachment;
    unsigned short nSlot = (unsigned short)query.m_nAttachment;
    RemoveLoginingMap(nSlot);

    GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(pRoleInfo->m_nSlot);
    if (pRoleInfo != NULL && pEntity != NULL) {
        if (query.m_nResult == 0) {
            pEntity->CreateFromDB(*pRoleInfo);
            GMEntityMgr::Instance().AddGMEntity(pEntity);
            GameMsg_S2C_GMLoginSuc sucMsg;
            memcpy(sucMsg.m_szName, pRoleInfo->m_szName, GM_MAX_NAME_LEN - 1);
            sucMsg.m_Priv = pRoleInfo->m_nPriv;
            pEntity->SendMsgToGM(&sucMsg);
            WriteLog(LOGLEVEL_DEBUG, "GM Login successfully.slot = %u, session = %lld, account = %u\n", pEntity->GetSlot(), pEntity->GetSession(), pEntity->GetAccount());
        }
    }

    delete pRoleInfo, pRoleInfo = NULL;
}

void CGMCirculator::GrantGMCmdDBCB(QueryBase & query)
{
    int nErrno = (int)query.m_nTag;
    unsigned short nSlot = (unsigned short)query.m_nTag;
    char *pName = (char *)query.m_pAttachment;
    list<string> *pListCmd = (list<string>*)query.m_pTag;

    GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(nSlot);
    if (pName != NULL && pListCmd != NULL && pEntity != NULL) {
        if (query.m_nResult == 0) {
            GameMsg_S2C_GMLoginSuc sucMsg;
            memcpy(sucMsg.m_szName, pName, GM_MAX_NAME_LEN - 1);
            pEntity->SendMsgToGM(&sucMsg);
        }
        else if (nErrno != 0) {
            GameMsg_S2C_GrantGMCmdFail failMsg;
            memcpy(failMsg.m_szName, pName, GM_MAX_NAME_LEN - 1);
            failMsg.m_nErrno = GrantCmd_NoSuchGM;
            pEntity->SendMsgToGM(&failMsg);
        }
    }

    delete pName, pName = NULL;
    delete pListCmd, pListCmd = NULL;
}

void CGMCirculator::RemoveGMCmdDBCB(QueryBase & query)
{
    unsigned short nSlot = (unsigned short)query.m_nTag;
    char *pName = (char *)query.m_pAttachment;
    list<string> *pListCmd = (list<string>*)query.m_pTag;
    if (pName == NULL || pListCmd == NULL) {
        return;
    }
    GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(nSlot);
    if (pName != NULL && pListCmd != NULL && pEntity != NULL) {
        if (query.m_nResult == 0) {
            GameMsg_S2C_RemoveGMCmdSuc sucMsg;
            memcpy(sucMsg.m_szName, pName, GM_MAX_NAME_LEN - 1);
            pEntity->SendMsgToGM(&sucMsg);
        }
        else {
            GameMsg_S2C_RemoveGMCmdFail failMsg;
            failMsg.m_nErrno = RemoveCmd_NoSuchGM;
            pEntity->SendMsgToGM(&failMsg);
        }
    }

    delete pName, pName = NULL;
    delete pListCmd, pListCmd = NULL;
}

void CGMCirculator::InsertSysMailLogDBCB(QueryBase & query)
{
    CGMSysMailLog *pMailLog = (CGMSysMailLog *)query.m_pAttachment;
    if (pMailLog != NULL) {
        delete pMailLog, pMailLog = NULL;
    }
}

void CGMCirculator::AddLoginingMap(unsigned short nSlot, GMEntity *pEntity)
{
    if (pEntity != NULL) {
        map<unsigned short, GMEntity *>::iterator itr = m_mapLoginingPlayer.find(nSlot);
        if (itr == m_mapLoginingPlayer.end()) {
            m_mapLoginingPlayer.insert(make_pair(nSlot, pEntity));
        }
    }
}

GMEntity * CGMCirculator::RemoveLoginingMap(unsigned short nSlot)
{
    GMEntity *pEntity = NULL;
    map<unsigned short, GMEntity *>::iterator itr = m_mapLoginingPlayer.find(nSlot);
    if (itr != m_mapLoginingPlayer.end()) {
        pEntity = itr->second;
        m_mapLoginingPlayer.erase(itr);
        return pEntity;
    }

    return NULL;
}

GMEntity *CGMCirculator::GetLoginingPlayer(unsigned short nSlot)
{
    map<unsigned short, GMEntity *>::iterator itr = m_mapLoginingPlayer.find(nSlot);
    if (itr != m_mapLoginingPlayer.end()) {
        return itr->second;
    }

    return NULL;
}

void CGMCirculator::InitManager(void)
{
    CAnnounceMgr::Instance().Initialize();
    CPrivMgr::Instance().Init();
    InfoCenterMgr::Instance().Initialize();
}

void CGMCirculator::OnSendCompensation(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_Compsition))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }
        GameMsg_C2S_SendCompensation& rMsg = (GameMsg_C2S_SendCompensation&)msg;
        std::string strJson = GMJsonLog::CompsitionToJson(rMsg.m_strEndTime, rMsg.m_nMoney, rMsg.m_strItemMale, rMsg.m_strItemFemale, rMsg.m_strTitle, rMsg.m_strContent);
        LogGM(pRole->GetAccount(), EGMActionType_Compsition, strJson);
        //转发消息到GroupServer
        GameMsg_M2G_UpdateCompensation sendMsg;
        sendMsg.m_nEndTime = SetTime(rMsg.m_strEndTime.c_str());
        sendMsg.m_nMoney = rMsg.m_nMoney;
        sendMsg.m_strTitle = rMsg.m_strTitle;
        sendMsg.m_strContent = rMsg.m_strContent;
        sendMsg.m_strItemMale = rMsg.m_strItemMale;
        sendMsg.m_strItemFemale = rMsg.m_strItemFemale;

        SendMsgToGroup(&sendMsg);
    }
}

void CGMCirculator::OnUpdateCompensationRes(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_G2M_UpdateCompensationRes& rMsg = (GameMsg_G2M_UpdateCompensationRes&)msg;

    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pRole != NULL)
    {
        GameMsg_S2C_SendCompensationRes resMsg;
        resMsg.m_strError = rMsg.m_strError;

        pRole->SendMsgToGM(&resMsg);
    }
}

void CGMCirculator::OnGetUsers(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_UserPriv))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_S2C_GetUserRes res;
        CPrivMgr::Instance().GetUsers(res.m_UserPrivList);
        pRole->SendMsgToGM(&res);

    }
}

void CGMCirculator::OnAddUser(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_UserPriv))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }
        GameMsg_C2S_AddUser& rMsg = (GameMsg_C2S_AddUser&)msg;
        std::string strJson = GMJsonLog::AddUserToJson(rMsg.m_UserName, rMsg.m_Password);
        LogGM(pRole->GetAccount(), EGMActionType_SetUserPriv, strJson);
        if (CPrivMgr::Instance().GetUserPriv(rMsg.m_UserName))
        {
            GameMsg_S2C_AddUserRes retMsg;
            retMsg.m_nRes = EGMErrorType_UserExist;
            retMsg.m_UserName = rMsg.m_UserName;
            retMsg.m_Priv = 0;
            pRole->SendMsgToGM(&retMsg);
        }
        else
        {
            CGMRoleInfoDB* pGMRoleInfo = new CGMRoleInfoDB();
            strncpy(pGMRoleInfo->m_szName, rMsg.m_UserName.c_str(), GM_MAX_NAME_LEN);
            strncpy(pGMRoleInfo->m_szPassword, rMsg.m_Password.c_str(), GM_MAX_PASSWD_LEN);
            CDelHelper delHelper(pGMRoleInfo);
            g_pGMQueryMgr->AddQuery(Query_AddUser, slotPeer.GetSlot(), (void*)pGMRoleInfo, 0, NULL, &delHelper);
        }
    }
}

void CGMCirculator::AddUserDBCB(QueryBase& query)
{
    if (query.m_nResult == 0)
    {
        CPrivMgr::Instance().LoadUsers();
        GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot((unsigned short)query.m_nAttachment);
        if (pRole != NULL)
        {
            CGMRoleInfoDB* pGMRoleInfo = (CGMRoleInfoDB*)query.m_pAttachment;
            GameMsg_S2C_AddUserRes retMsg;
            retMsg.m_nRes = EGMErrorType_Success;
            retMsg.m_UserName = pGMRoleInfo->m_szName;
            retMsg.m_Priv = 0;
            pRole->SendMsgToGM(&retMsg);
        }
    }
    delete (CGMRoleInfoDB*)query.m_pAttachment;
    query.m_pAttachment = NULL;
}

void CGMCirculator::OnDelUser(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_UserPriv))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }
        GameMsg_C2S_DelUser& rMsg = (GameMsg_C2S_DelUser&)msg;
        std::string strJson = GMJsonLog::DelUserToJson(rMsg.m_UserName);
        LogGM(pRole->GetAccount(), EGMActionType_SetUserPriv, strJson);
        if (CPrivMgr::Instance().GetUserPriv(rMsg.m_UserName))
        {
            CGMRoleInfoDB* pGMRoleInfo = new CGMRoleInfoDB();
            strncpy(pGMRoleInfo->m_szName, rMsg.m_UserName.c_str(), GM_MAX_NAME_LEN);
            CDelHelper delHelper(pGMRoleInfo);
            g_pGMQueryMgr->AddQuery(Query_DelUser, slotPeer.GetSlot(), pGMRoleInfo, 0, NULL, &delHelper);
        }
        else
        {
            GameMsg_S2C_DelUserRes retMsg;
            retMsg.m_nRes = EGMErrorType_UserNotExist;
            pRole->SendMsgToGM(&retMsg);
        }
    }
}

void CGMCirculator::DelUserDBCB(QueryBase& query)
{
    GMEntity* pGMEntity = GMEntityMgr::Instance().GetEntityBySlot((unsigned short)query.m_nAttachment);
    if (query.m_nResult == 0)
    {
        CGMRoleInfoDB* pRole = (CGMRoleInfoDB*)query.m_pAttachment;
        CPrivMgr::Instance().DeleteUser(pRole->m_szName);

        if (pGMEntity != NULL)
        {
            GameMsg_S2C_DelUserRes msg;
            msg.m_nRes = 0;
            pGMEntity->SendMsgToGM(&msg);
        }
    }
    else
    {
        if (pGMEntity != NULL)
        {
            if (pGMEntity != NULL)
            {
                GameMsg_S2C_DelUserRes msg;
                msg.m_nRes = EGMErrorType_Databases;
                pGMEntity->SendMsgToGM(&msg);
            }
        }
    }

    delete (CGMRoleInfoDB*)query.m_pAttachment;
    query.m_pAttachment = NULL;
}

void CGMCirculator::OnSetPriv(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_UserPriv))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2S_SetPriv& rMsg = (GameMsg_C2S_SetPriv&)msg;
        std::string strJson = GMJsonLog::SetUserPrivToJson(rMsg.m_UserName, rMsg.m_Priv);
        LogGM(pRole->GetAccount(), EGMActionType_SetUserPriv, strJson);
        CGMRoleInfoDB* pGMRoleInfo = new CGMRoleInfoDB();
        strncpy(pGMRoleInfo->m_szName, rMsg.m_UserName.c_str(), GM_MAX_NAME_LEN);
        pGMRoleInfo->m_nPriv = rMsg.m_Priv;
        CDelHelper delHelper(pGMRoleInfo);
        g_pGMQueryMgr->AddQuery(Query_SetPriv, slotPeer.GetSlot(), pGMRoleInfo, 0, NULL, &delHelper);

    }
}

void CGMCirculator::SetPrivDBCB(QueryBase& query)
{
    GMEntity* pGMEntity = GMEntityMgr::Instance().GetEntityBySlot((unsigned short)query.m_nAttachment);
    if (query.m_nResult == 0)
    {
        CGMRoleInfoDB* pRole = (CGMRoleInfoDB*)query.m_pAttachment;
        CPrivMgr::Instance().SetUserPriv(pRole->m_szName, pRole->m_nPriv);

        if (pGMEntity != NULL)
        {
            GameMsg_S2C_SetPrivRes msg;
            msg.m_nRes = 0;
            pGMEntity->SendMsgToGM(&msg);
        }
    }
    else
    {
        if (pGMEntity != NULL)
        {
            if (pGMEntity != NULL)
            {
                GameMsg_S2C_SetPrivRes msg;
                msg.m_nRes = EGMErrorType_Databases;
                pGMEntity->SendMsgToGM(&msg);
            }
        }
    }
    delete (CGMRoleInfoDB*)query.m_pAttachment;
    query.m_pAttachment = NULL;
}

void CGMCirculator::LogGM(unsigned int account, EGMActionType action, std::string& param)
{
    char* szBuff = new char[param.size() + 1];
    memset(szBuff, 0, sizeof(char)* (param.size() + 1));
    strncpy(szBuff, param.c_str(), param.size());
    g_pGMQueryMgr->AddQuery(Query_Log, action, szBuff, account);
}

void CGMCirculator::OnGetServerLine(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        GameMsg_M2G_GetServerLine gMsg;
        gMsg.m_nSlot = slotPeer.GetSlot();
        SendMsgToGroup(&gMsg);
    }
}

void CGMCirculator::OnGroupGetServerLineRes(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_G2M_GetServerLineRes& rMsg = (GameMsg_G2M_GetServerLineRes&)msg;
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(rMsg.m_nSlot);
    if (pRole != NULL)
    {
        GameMsg_S2C_GMGetServerLineRes retMsg;
        retMsg.m_Lines.insert(rMsg.m_Lines.begin(), rMsg.m_Lines.end());
        pRole->SendMsgToGM(&retMsg);
    }
}

void CGMCirculator::OnGetServerRoomList(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_RoomManager))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2S_GMGetServerRoomList& rMsg = (GameMsg_C2S_GMGetServerRoomList&)msg;

        std::string strJson = rMsg.ToJson();
//         LogGM(pRole->GetAccount(), EGMActionType_GetRoomList, strJson);

        GameMsg_M2G_GetRoomList mgMsg;
        mgMsg.m_nSlot = slotPeer.GetSlot();
        mgMsg.m_nLine = rMsg.m_nLine;
        mgMsg.m_nGMAskRoomType = rMsg.m_GMAskRoomType;
        SendMsgToGroup(&mgMsg);
    }
}

void CGMCirculator::OnGroupGetRoomListRes(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_G2M_GetRoomListRes& rMsg = (GameMsg_G2M_GetRoomListRes&)msg;

    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(rMsg.m_nSlot);

    if (pRole != NULL)
    {
        GameMsg_S2C_GMGetServerRoomListRes retMsg;
        std::vector<GMRoomInfo>::iterator it = rMsg.m_RoomList.begin();
        for (; it != rMsg.m_RoomList.end(); it++)
        {
            retMsg.m_RoomList.push_back(*it);
        }
        pRole->SendMsgToGM(&retMsg);
    }
}

void CGMCirculator::OnSetMallUpdateTime(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_SetMallUpdateTime))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2S_GMSetMallUpdateTime& rMsg = (GameMsg_C2S_GMSetMallUpdateTime&)msg;

        std::string strJson = GMJsonLog::SetMallUpdateTimeToJson(rMsg.m_UpdateTime);
        LogGM(pRole->GetAccount(), EGMActionType_SetMallUpdateTime, strJson);

        GameMsg_M2G_SetMallUpdateTime m2gMsg;
        m2gMsg.m_UpdateTime = rMsg.m_UpdateTime;

        SendMsgToGroup(&m2gMsg);

        GameMsg_S2C_SetMallUpdateTimeRet retMsg;
        pRole->SendMsgToGM(&retMsg);
    }
}

void CGMCirculator::OnGroupBuyControl(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_GroupByControl))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2S_GMGroupBuyControl& rMsg = (GameMsg_C2S_GMGroupBuyControl&)msg;

        std::string strJson = GMJsonLog::SetGroupBuyControlToJson(rMsg.m_nLine, rMsg.m_nStartTime, rMsg.m_nEndTime, rMsg.m_nInterval, rMsg.m_nMinPeople, rMsg.m_nMaxPeople);
        LogGM(pRole->GetAccount(), EGMActionType_GroupBuyControl, strJson);

        GameMsg_M2G_GroupBuyControl mgMsg;
        mgMsg.m_nLine = rMsg.m_nLine;
        mgMsg.m_nInterval = rMsg.m_nInterval;
        mgMsg.m_nEndTime = rMsg.m_nEndTime;
        mgMsg.m_nMaxPeople = rMsg.m_nMaxPeople;
        mgMsg.m_nMinPeople = rMsg.m_nMinPeople;
        mgMsg.m_nStartTime = rMsg.m_nStartTime;

        SendMsgToGroup(&mgMsg);

        GameMsg_S2C_GroupBuyControlRet retMsg;
        pRole->SendMsgToGM(&retMsg);
    }
}

void CGMCirculator::OnChangeSelfPassword(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        GameMsg_C2S_ChangeSelfPassword& rMsg = (GameMsg_C2S_ChangeSelfPassword&)msg;
        std::string password = CPrivMgr::Instance().GetUserPassword(pRole->GetName());
        if (password == "" || password != rMsg.m_OldPassword || rMsg.m_NewPassword.size() > 12)
        {
            GameMsg_S2C_ChangeSelfPasswordRet retMsg;
            retMsg.m_bResult = false;
            pRole->SendMsgToGM(&retMsg);
            return;
        }
        char* newPassword = new char[rMsg.m_NewPassword.size() + 1];
        memset(newPassword, 0, rMsg.m_NewPassword.size() + 1);
        strncpy(newPassword, rMsg.m_NewPassword.c_str(), rMsg.m_NewPassword.size());
        g_pGMQueryMgr->AddQuery(Query_ChangePassword, pRole->GetAccount(), newPassword, slotPeer.GetSlot());
    }
}

void CGMCirculator::ChangePasswdDBCB(QueryBase& query)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot((int)query.m_nTag);

    if (pRole != NULL)
    {
        if (query.m_nResult == 0)
        {
            unsigned int nAccount = (unsigned int)query.m_nAttachment;
            char* newPassword = (char*)query.m_pAttachment;
            CPrivMgr::Instance().SetUserPassword(nAccount, newPassword);

            GameMsg_S2C_ChangeSelfPasswordRet retMsg;
            retMsg.m_bResult = true;
            pRole->SendMsgToGM(&retMsg);
        }
        else
        {
            GameMsg_S2C_ChangeSelfPasswordRet retMsg;
            retMsg.m_bResult = false;
            pRole->SendMsgToGM(&retMsg);
        }
    }
}

void CGMCirculator::OnChangePassword(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_UserPriv))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2S_ChangePassword& rMsg = (GameMsg_C2S_ChangePassword&)msg;

        CGMRoleInfoDB* pRoleInfo = NULL;
        bool bFindUser = CPrivMgr::Instance().GetUser(rMsg.m_UserName, pRoleInfo);
        if (!bFindUser || pRoleInfo == NULL)
        {
            GameMsg_S2C_ChangeSelfPasswordRet retMsg;
            retMsg.m_bResult = false;
            pRole->SendMsgToGM(&retMsg);
        }
        else
        {
            std::string jparam = GMJsonLog::ChangeGMPasswordToJson(rMsg.m_UserName, rMsg.m_Password);
            LogGM(pRole->GetAccount(), EGMActionType_SetUserPriv, jparam); // 记录事件

            char* newPassword = new char[rMsg.m_Password.size() + 1];
            memset(newPassword, 0, rMsg.m_Password.size() + 1);
            strncpy(newPassword, rMsg.m_Password.c_str(), rMsg.m_Password.size());
            g_pGMQueryMgr->AddQuery(Query_ChangePassword, pRoleInfo->m_nAccount, newPassword, slotPeer.GetSlot());
        }
    }
}

void CGMCirculator::OnPrivAddMCoin(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_PrivMCoin))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2S_GMSendPrivMCoin& rMsg = (GameMsg_C2S_GMSendPrivMCoin&)msg;

        string strJson = GMJsonLog::AddPrivMCoinToJson(rMsg.m_PlayerMoneyInfo);
        LogGM(pRole->GetAccount(), EGMActionType_AddPrivMCoin, strJson);

        GameMsg_M2G_SendPrivMCoin m2gMsg;
        m2gMsg.m_nSlot = slotPeer.GetSlot();
        for (size_t i = 0; i < rMsg.m_PlayerMoneyInfo.size(); i++)
        {
            m2gMsg.m_PlayerMoneyInfo.push_back(rMsg.m_PlayerMoneyInfo[i]);
        }
        SendMsgToGroup(&m2gMsg);
    }
}

void CGMCirculator::OnPrivAddMCoinRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_A2M_SendPrivMCoinRet& rMsg = (GameMsg_A2M_SendPrivMCoinRet&)msg;
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(rMsg.m_nSlot);
    if (pRole != NULL)
    {
        GameMsg_S2C_SendPrivMCoinRet retMsg;
        for (size_t i = 0; i < rMsg.m_GMRechargeResult.size(); i++)
        {
            retMsg.m_PlayerResult.push_back(rMsg.m_GMRechargeResult[i]);
        }
        pRole->SendMsgToGM(&retMsg);
    }
}

void CGMCirculator::OnGetServerPlayerCountInfo(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_ServerPlayerCount))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_M2G_GetServerPlayerCountInfo m2gMsg;
        m2gMsg.m_nSolt = slotPeer.GetSlot();
        SendMsgToGroup(&m2gMsg);
    }
}

void CGMCirculator::OnSetServerPlayerCountInfo(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_ServerPlayerCount))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2S_SetServerPlayerInfo& rMsg = (GameMsg_C2S_SetServerPlayerInfo&)msg;

        GameMsg_M2A_SetPlayerLimit aMsg;
        aMsg.m_PlayerLimit = rMsg.m_nLimit;
        SendMsgToGroup(&aMsg);

        GameMsg_S2C_SetServerPlayerInfoRet retMsg;
        pRole->SendMsgToGM(&retMsg);
    }
}

void CGMCirculator::OnGetServerPlayerCountInfoRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_G2M_GetServerPlayerCountInfoRet& rMsg = (GameMsg_G2M_GetServerPlayerCountInfoRet&)msg;
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(rMsg.m_nSolt);
    if (pRole != NULL)
    {
        GameMsg_S2C_GetServerPlayerInfoRet retMsg;
        std::map<int, CServerPlayerCountInfo>::iterator it = rMsg.m_ServerPlayerCountInfoMap.begin();
        for (; it != rMsg.m_ServerPlayerCountInfoMap.end(); it++)
        {
            retMsg.m_ServerPlayerCountInfoList.push_back(it->second);
        }
        pRole->SendMsgToGM(&retMsg);
    }
}

void CGMCirculator::OnGetServerPlayerLimitRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_A2G_GetServerPlayerLimitRet& rMsg = (GameMsg_A2G_GetServerPlayerLimitRet&)msg;
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(rMsg.m_nSolt);
    if (pRole != NULL)
    {
        GameMsg_S2C_GetServerPlayerLimit sMsg;
        sMsg.m_nLimit = rMsg.m_PlayerLimit;
        pRole->SendMsgToGM(&sMsg);
    }
}

void CGMCirculator::OnChangeAccountPassword(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_C2S_ChangePassword& rMsg = (GameMsg_C2S_ChangePassword&)msg;
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_ChangeAccountPassword))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }


        std::string jparam = GMJsonLog::ChangeAccountPasswordToJson(rMsg.m_UserName, rMsg.m_Password);
        LogGM(pRole->GetAccount(), EGMActionType_ChangeAccountPassword, jparam); // 记录事件

        //发送消息到GroupServer
        GameMsg_M2A_ChangeAccountPassword sMsg;
        sMsg.m_Account = rMsg.m_UserName;
        sMsg.m_Password = rMsg.m_Password;
        sMsg.m_nSlot = slotPeer.GetSlot();

        SendMsgToGroup(&sMsg);
    }
}

void CGMCirculator::OnSendBlackDeviceList(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_C2S_SendBlackDeviceList& rMsg = (GameMsg_C2S_SendBlackDeviceList&)msg;
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_SendBlackDevice))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        std::string jparam = GMJsonLog::SendBlackDeviceListToJson(rMsg.m_bFrozenDevice, rMsg.m_DeviceNameList);
        LogGM(pRole->GetAccount(), EGMActionType_SendBlackDeviceList, jparam); 

        //发送消息到GroupServer
        GameMsg_M2A_SendBlackDeviceList sMsg;
        sMsg.m_nSlot = slotPeer.GetSlot();
        sMsg.m_NameList.insert(sMsg.m_NameList.begin(), rMsg.m_DeviceNameList.begin(), rMsg.m_DeviceNameList.end());
        sMsg.m_GM = pRole->GetName();
        sMsg.m_bFrozenDevice = rMsg.m_bFrozenDevice;
        SendMsgToGroup(&sMsg);
    }
}

void CGMCirculator::OnSendBlackDeviceListRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_A2M_SendBlackDeviceListRet& rMsg = (GameMsg_A2M_SendBlackDeviceListRet&)msg;
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot((unsigned short)rMsg.m_nSlot);
    if (pRole != NULL)
    {
        GameMsg_S2C_SendBlackDeviceListRes retMsg;
        retMsg.m_Result = rMsg.m_Result;

        SendMsgToGM(&retMsg, (unsigned short)rMsg.m_nSlot);
    }
}

void CGMCirculator::OnChangeAccountPasswordRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_A2M_ChangeAccountPasswordRet& rMsg = (GameMsg_A2M_ChangeAccountPasswordRet&)msg;
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot((unsigned short)rMsg.m_nSlot);
    if (pRole != NULL)
    {
        GameMsg_S2C_ChangeAccountPasswordRes retMsg;
        retMsg.m_Result = rMsg.m_Result;
        SendMsgToGM(&retMsg, (unsigned short)rMsg.m_nSlot);
    }
}

void CGMCirculator::OnDeletVersionMedal(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (NULL != pRole)
    {
        std::string Jparam = GMJsonLog::DeleteVersionMedalToJson();
        LogGM(pRole->GetAccount(), EGMActionType_Medal, Jparam);

        GameMsg_M2G_GMDeleteVersionMedal resultMsg;
        SendMsgToGroup(&resultMsg);
    }
}

void CGMCirculator::OnDeletVersionMedalRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_G2M_GMDeleteVersionMedal &resultMsg = (GameMsg_G2M_GMDeleteVersionMedal&)msg;

    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pRole != NULL)
    {
        GameMsg_G2M_DeleteVersionMedal GmMsg;
        GmMsg.m_nRetCode = resultMsg.m_nRetCode;
        pRole->SendMsgToGM(&GmMsg);
    }
}

void CGMCirculator::OnAddMedal(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (NULL != pRole)
    {

        GameMsg_C2M_AddMedal& resultMsg = (GameMsg_C2M_AddMedal&)msg;
        std::string jparam = GMJsonLog::AddMedalToJson(resultMsg.m_nPlayerID, resultMsg.m_nMedalID);
        LogGM(pRole->GetAccount(), EGMActionType_Medal, jparam);
            
        GameMsg_M2G_GMAddMedal retMsg;
        retMsg.m_nMedalID = resultMsg.m_nMedalID;
        retMsg.m_nPlayerID = resultMsg.m_nPlayerID;
        SendMsgToGroup(&retMsg);
    }
}

void CGMCirculator::OnAddMedalRet(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_G2M_GMAddMedal &resultMsg = (GameMsg_G2M_GMAddMedal&)msg;

    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pRole != NULL)
    {
        GameMsg_M2C_AddMedal GmMsg;
        GmMsg.m_nRetCode = resultMsg.m_nRetCode;
        pRole->SendMsgToGM(&GmMsg);
    }
}

void CGMCirculator::OnRequestRechargeRecordList(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity * pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pRole == NULL)
    {
        return;
    }

    GameMsg_C2M_RequestRechargeRecordList *pMsg = (GameMsg_C2M_RequestRechargeRecordList *)&msg;

    // 查询数据库，得到请求信息之后直接回复给客户端
    IosForbidRechargeDB *query = new IosForbidRechargeDB;
    query->m_nSlot = (int)slotPeer.GetSlot();
    query->m_nStartTime = pMsg->m_nStartTime;
    query->m_nEndTime = pMsg->m_nEndTime;
    query->m_nQueryBeginIndex = pMsg->m_nQueryBeginIndex;
    query->m_nQueryCount = pMsg->m_nQueryCount;
    g_pGMQueryMgr->AddQuery(Query_RechagreRecordList, 0, query, 0, NULL); // 发送这个查询请求
}

void CGMCirculator::OnQueryRechargeRecordList(QueryBase& query)
{
    SharedPtr<IosForbidRechargeDB> tempPtr((IosForbidRechargeDB*)query.m_pAttachment);
    query.m_pAttachment = NULL;
    ENSURE_WITH_LOG_CMD(NULL != tempPtr.Get(), return, "attachment should not null");

    GameMsg_M2C_ResponseRechargeRecordList retMsg;
    retMsg.m_listRecharge = tempPtr->m_listRecharge;

    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot((unsigned short)tempPtr->m_nSlot);
    if (pRole == NULL)
        return; //

    pRole->SendMsgToGM(&retMsg);
}

void CGMCirculator::OnRequestLockList(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_PlayerFrozen))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2M_RequestLockList *pMsg = (GameMsg_C2M_RequestLockList *)&msg;

        // 查询数据库，得到请求信息之后直接回复给客户端
        PlayerLockQueryDb *query = new PlayerLockQueryDb;
        query->m_nStartTime = pMsg->m_nStartTime;
        query->m_nEndTime = pMsg->m_nEndTime;
        g_pGMQueryMgr->AddQuery(Query_PlayerLockQuery, 0, query, 0, NULL); // 发送这个查询请求
    }

    return;
}

void CGMCirculator::OnQueryPlayerLockCB(QueryBase& query)
{
    SharedPtr<PlayerLockQueryDb> tempPtr((PlayerLockQueryDb*)query.m_pAttachment);
    query.m_pAttachment = NULL;
    ENSURE_WITH_LOG_CMD(NULL != tempPtr.Get(), return, "attachment should not null");

    GameMsg_M2C_ResponseLockList retMsg;
    retMsg.m_lstLockItem.assign(tempPtr->m_lstLockItem.begin(), tempPtr->m_lstLockItem.end());

    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot((unsigned short)tempPtr->m_nSlot);
    if (pRole == NULL)
        return; //

    pRole->SendMsgToGM(&retMsg);

    return;
}

// 请求：
void CGMCirculator::OnRequestHarassList(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_Harass))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2M_HarassList &rMsg = (GameMsg_C2M_HarassList &)msg;

        // 构造一个查询请求然后发送查询
        HarassReqDb *harassDb = new HarassReqDb;
        harassDb->m_nSlot = slotPeer.GetSlot();
        harassDb->m_nStartTime = rMsg.m_nStartTime;
        harassDb->m_nEndTime = rMsg.m_nEndTime;
        harassDb->m_strHarassInfo = rMsg.m_strHarassInfo;

        g_pGMQueryMgr->AddQuery(Query_HarassPlayers, 0, harassDb); // 转发请求
    }

    return;
}

void CGMCirculator::OnQueryHarassListCB(QueryBase& query)
{
    SharedPtr<HarassReqDb> tempPtr((HarassReqDb*)query.m_pAttachment);
    query.m_pAttachment = NULL;
    ENSURE_WITH_LOG_CMD(NULL != tempPtr.Get(), return, "attachment should not null");

    GameMsg_M2C_HarassList retMsg;
    retMsg.m_lstRoleHarass.assign(tempPtr->m_lstRoleHarass.begin(), tempPtr->m_lstRoleHarass.end());

    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot((unsigned short)tempPtr->m_nSlot);
    if (pRole == NULL)
        return; //

    pRole->SendMsgToGM(&retMsg);

    return;
}

void CGMCirculator::OnRequestRoleHarassChatInfo(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_Harass))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2M_RoleHarassChatInfo &rMsg = (GameMsg_C2M_RoleHarassChatInfo &)msg;

        // 构造一个查询请求然后发送查询
        HarassChatReqDb *harassDb = new HarassChatReqDb;
        harassDb->m_nSlot = slotPeer.GetSlot();
        harassDb->m_nStartTime = rMsg.m_nStartTime;
        harassDb->m_nEndTime = rMsg.m_nEndTime;
        harassDb->m_strHarassChat = rMsg.m_strHarassInfo;
        harassDb->m_nRoleID = rMsg.m_nRoleID;

        g_pGMQueryMgr->AddQuery(Query_HarassPlayerChat, 0, harassDb); // 转发请求
    }

    return;
}

void CGMCirculator::OnQueryRoleHarassChatInfoCB(QueryBase& query)
{
    SharedPtr<HarassChatReqDb> tempPtr((HarassChatReqDb*)query.m_pAttachment);
    query.m_pAttachment = NULL;
    ENSURE_WITH_LOG_CMD(NULL != tempPtr.Get(), return, "attachment should not null");

    GameMsg_M2C_RoleHarassChatInfo retMsg;
    retMsg.m_nPlatformID = tempPtr->m_nPlatformID;
    retMsg.m_nRoleID = tempPtr->m_nRoleID;
    retMsg.m_strPuid = tempPtr->m_strPuid;
    retMsg.m_lstRoleChatInfo.assign(tempPtr->m_lstRoleChatInfo.begin(), tempPtr->m_lstRoleChatInfo.end());

    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot((unsigned short)tempPtr->m_nSlot);
    if (pRole == NULL)
        return; //

    pRole->SendMsgToGM(&retMsg);

    return;
}

void CGMCirculator::OnRequestQueryLog(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_QueryGMLog))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2S_QueryLog &rMsg = (GameMsg_C2S_QueryLog &)msg;

        // 构造一个查询请求然后发送查询
        QueryLogDb *pQueryDb = new QueryLogDb;
        pQueryDb->m_nSlot = slotPeer.GetSlot();
        pQueryDb->logType = rMsg.m_eActionType;
        pQueryDb->m_nStartTime = rMsg.m_nStartTime;
        pQueryDb->m_nEndTime = rMsg.m_nEndTime;

        g_pGMQueryMgr->AddQuery(Query_LogLoad, 0, pQueryDb); // 
    }

    return;

}

void CGMCirculator::OnQueryLogLoad(QueryBase& query)
{
    QueryLogDb* pQueryDb = (QueryLogDb*)query.m_pAttachment;
    query.m_pAttachment = NULL;
    ENSURE_WITH_LOG_CMD(NULL != pQueryDb, return, "attachment should not null in QueryLogLoad cb");
    SharedPtr<QueryLogDb> tempQuery(pQueryDb);

    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(pQueryDb->m_nSlot);

    if (pRole == NULL)
        return; //

    GameMsg_S2C_QueryLogRet retMsg;
    // 最多200条
    const size_t MAX_SEND_NUM = 200;
    retMsg.m_nLogNum = pQueryDb->m_vecLogs.size();
    retMsg.m_bEndMsg = false;

    int nStart = 0;

    for (size_t i = 0; nStart + i < pQueryDb->m_vecLogs.size(); ++i)
    {
        if (i >= MAX_SEND_NUM)
        {
            pRole->SendMsgToGM(&retMsg);
            retMsg.m_logInfo.clear();
            nStart = nStart + i;
            i = 0;
        }

        QueryLogResult &lres = pQueryDb->m_vecLogs[nStart + i];
        QueryLogInfo qi;
        qi.m_nLogID = lres.m_nLogID;
        qi.m_strAccouont = lres.m_strGmaAccountName;
        qi.m_nOperTime = lres.m_nOperTime;
        qi.m_strOperDesc = GMJsonLog::JsonToDesc(lres.m_strOperParamJson);

        retMsg.m_logInfo.push_back(qi); // 
    }

    retMsg.m_bEndMsg = true;
    pRole->SendMsgToGM(&retMsg); // 发送出去了
}


void CGMCirculator::OnRequestQueryLogItem(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());

    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_QueryGMLog))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_C2S_QueryLogItem &rMsg = (GameMsg_C2S_QueryLogItem &)msg;

        // 构造一个查询请求然后发送查询
        QueryLogItemDb *pQueryDb = new QueryLogItemDb;
        pQueryDb->m_nLogID = rMsg.m_nLogID;
        pQueryDb->m_nSlot = slotPeer.GetSlot();
        g_pGMQueryMgr->AddQuery(Query_LogItemLoad, 0, pQueryDb); // 
    }

    return;
}

void CGMCirculator::OnQueryLogItemLoad(QueryBase& query)
{
    if (NULL == query.m_pAttachment)
        return;
    SharedPtr<QueryLogItemDb> queryItem((QueryLogItemDb*)query.m_pAttachment);

    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(queryItem->m_nSlot);
    if (pRole == NULL)
        return; //


    std::list<std::string> listDesc;
    GMJsonLog::JsonToDescList(queryItem->jsonParam, listDesc);

    GameMsg_S2C_QueryLogItemRet retMsg;
    // 最多200条
    const size_t MAX_SEND_NUM = 200;

    retMsg.m_nLogNum = listDesc.size();
    retMsg.m_bEndMsg = false;

    for (std::list<std::string>::iterator iter = listDesc.begin(); iter != listDesc.end(); ++iter)
    {
        retMsg.m_logDesc.push_back(*iter); // 
        if (retMsg.m_logDesc.size() >= MAX_SEND_NUM)
        {
            pRole->SendMsgToGM(&retMsg);
            retMsg.m_logDesc.clear();
        }
    }
    
// 
//     GameMsg_S2C_QueryLogRet rrr;
//     rrr.m_bEndMsg = true;
//     pRole->SendMsgToGM(&rrr);

    retMsg.m_bEndMsg = true;
    pRole->SendMsgToGM(&retMsg);

    return;
}


