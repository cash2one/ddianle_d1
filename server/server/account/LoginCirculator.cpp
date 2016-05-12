#include "../../socket/inc/SocketLib.h"
#include "../../datastructure/DataStruct_DB.h"

#include "../libServerFrame/Main.h"
#include "../libServerFrame/GameMsgCacheEnt.h"
#include "LoginCirculator.h"
#include "AccountMsgDef.h"
#include "../util/CommonFunc.h"
#include "../util/IDCreater.h"
#include "../libServerFrame/QueryManager.h"
#include "GameMsg_Account_LoginResult.h"
#include "GameMsg_Account_Login.h"
#include "GameMsg_Account_CreateAccount.h"
#include "GameMsg_Account_CreateAccountResult.h"
#include "GameMsg_Account_QuickCreateAccount.h"
#include "GameMsg_Account_QuickCreateAccountResult.h"
#include "GameMsg_Account_ServerState.h"
#include "GameMsg_Server_State.h"
#include "../share/GameMsg_ChargeOnline.h"
#include "GameMsg_Account_NotActivated.h"
#include "GameMsg_Account_CheckActivate.h"
#include "GameMsg_Account_CheckActivateResult.h"
#include "GameMsg_Account_GetGameServersInfo.h"
#include "GameMsg_Account_RebindAccount.h"
#include "../login/GameMsg_Player_SwitchLine.h"
#include "../share/GameMsg_SyncSetting.h"
#include "../share/GameMsg_S2A_AddMCoinByCmd.h"
#include "../GM/GameMsg_S2A_LockOrUnLockAccountByGm.h"
#include "../GM/GameMsg_GM_share.h"

#include "../login/GameMsg_Server_AccountLogout.h"
#include "../share/GameMsg_LinkCount.h"
#include "../share/GameMsg_Server_Connected.h"
#include "../share/GameMsg_GameServerLinkInfo.h"
#include "../login/GameMsg_Server_AccountLogin.h"
#include "../login/GameMsg_Server_DropClient.h"
#include "../login/GameMsg_Server_VerifyAccount.h"
#include "../../datastructure/datainfor/ServerConfig.h"

#include "../mail/GameMsg_Mail.h"

#include "../../socket/GameMsg_Map.h"
#include "../../socket/Md5.h"
#include "../../socket/Formatter.h"

#include "../lua/InitLua.h"
#include "../payment/PaymentInfoMgr.h"
#include "CurrencyTable.h"
#include "GameMsg_Currency.h"
#include "../util/NameDeny.h"

#include <sstream>
#include <set>
#include <time.h>
#include <string>
#include <algorithm>

//账号在线信息
#define AOS_NOT_ONLIE   0//没有在线
#define AOS_LOGIN       1//在登录中
#define AOS_GAME        2//在游戏中


extern CIni g_Config;
extern unsigned int s_nBlockTime[8];

CLoginCirculator *g_pCirculator = NULL;

list<std::string> g_listSpecialUser;

extern int g_nSection;

PlayerCacheManager CLoginCirculator::m_playerCacheManager;

CLoginCirculator::CLoginCirculator()
{
    m_stat.nType = SVRTYPE_ACCOUNT;
    g_pCirculator = this;

    m_nSoftLimitClientCount = 0;
    m_nServerUpTime = 0;

    m_bEnableRecharge = false;
    m_bEnableFirstRechargeReward = false;

    m_nAccountSessionIdCounter = 0;

    m_pGameSrvSocketMgr = NULL;
    m_pLinkerSlotPeer = NULL;
}

CLoginCirculator::~CLoginCirculator()
{
    if (m_pGameSrvSocketMgr)
    {
        delete m_pGameSrvSocketMgr;
    }

    while (g_listSpecialUser.size() > 0)
    {
        g_listSpecialUser.pop_front();
    }

    while (m_listAutoAccount.size() > 0)
    {
        delete m_listAutoAccount.front();
        m_listAutoAccount.pop_front();
    }

    if (m_pLinkerSlotPeer != NULL)
    {
        delete m_pLinkerSlotPeer;
        m_pLinkerSlotPeer = NULL;
    }

    for ( LoginSessionMap::iterator slotItr = m_mapLoginSession.begin();
        slotItr != m_mapLoginSession.end(); ++slotItr )
    {
        std::map<__int64, AccountInfo *> &mapSession = slotItr->second;
        std::map<__int64, AccountInfo *>::iterator itr = mapSession.begin();

        while ( itr != mapSession.end() )
        {
            delete itr->second, itr->second = NULL;
            ++itr;
        }

        mapSession.clear();
    }
    m_mapLoginSession.clear();

    for ( OnlineAccountMap::iterator onlineItr = m_mapAccountOnline.begin();
        onlineItr != m_mapAccountOnline.end(); ++onlineItr )
    {
        delete onlineItr->second, onlineItr->second = NULL;
    }
    m_mapAccountOnline.clear();
}

void CLoginCirculator::InitPlayerCacheManager( unsigned int nMaxCount, unsigned int nValidTime )
{
    m_playerCacheManager.Init( nMaxCount, nValidTime );
}

//在查询管理器还没来得及返回结果，同一账号再次登录时会出现两指针不等的情况, 需要注册内存管理，防止内存泄露
bool CLoginCirculator::IsInLoginProgress(AccountInfo *pAccount)
{
    if (pAccount != NULL)
    {
        AccountInfo *pAccountInMap = GetLoginAccount(pAccount->nSlot, pAccount->SessionID);
        if (pAccountInMap != NULL && pAccount == pAccountInMap)
            return true;
    }

    return false;
}

AccountInfo* CLoginCirculator::GetLoginAccount(int nSlot, __int64 nSessionID)
{
    LoginSessionMap::iterator itSlot = m_mapLoginSession.find(nSlot);
    if (itSlot != m_mapLoginSession.end())
    {
        map<__int64, AccountInfo*>::iterator it = itSlot->second.find(nSessionID);
        if (it != itSlot->second.end())
        {
            return it->second;
        }
    }

    return NULL;
}

bool CLoginCirculator::AddLoginAccount(__int64 nSessionID, AccountInfo *pAccount)
{
    if (nSessionID > 0 && pAccount)
    {
        m_mapLoginSession[pAccount->nSlot][nSessionID] = pAccount;
        return true;
    }
    return false;
}

bool CLoginCirculator::RemoveLoginAccount(int nSlot, __int64 nSessionID)
{
    if (nSessionID > 0)
    {
        LoginSessionMap::iterator itSlot = m_mapLoginSession.find(nSlot);
        if (itSlot != m_mapLoginSession.end())
        {
            map<__int64, AccountInfo*>::iterator it = itSlot->second.find(nSessionID);
            if (it != itSlot->second.end())
            {
                itSlot->second.erase(it);
                return true;
            }
        }
    }
    return false;
}

AccountInfo *CLoginCirculator::GetOnlineAccount(unsigned int nAccount)
{
    AccountInfo * pAccountInfo = NULL;
    if (m_mapAccountOnline.find(nAccount) != m_mapAccountOnline.end())
    {
        pAccountInfo = m_mapAccountOnline[nAccount];
    }
    return pAccountInfo;
}

int CLoginCirculator::GetOnlineAccountCount()
{
    int nCount = 0;
    const GameServerLinkInfo *pInfo = NULL;
    const std::vector<GameServerLinkInfo *> &rLinkInfos = m_linkingInfos.GetAllLinkInfos();
    for (std::vector<GameServerLinkInfo *>::const_iterator itr = rLinkInfos.begin(); itr != rLinkInfos.end(); ++itr)
    {
        pInfo = *itr;
        if (pInfo != NULL)
        {
            nCount += pInfo->GetClient();
        }
    }

    return nCount;
}

bool CLoginCirculator::Initialize()
{
    bool bRes = CCirculator::Initialize();

    if (bRes && m_pQueryMgr != NULL)
    {
        bRes = m_pQueryMgr->OpenBackupAccountDB();
    }

    if (bRes)
    {
        CInitLua& lua = CInitLua::GetInstance();
        lua.InitState();
        string sLuaFile = g_sPath + "data/accountserverlua.lua";
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
        std::string strConfigFile = g_sPath + "data/config.ini";
        if (!g_ServerConfig.Load(strConfigFile))
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
        //单线限制数量
        m_nSoftLimitClientCount = g_Config.GetInt(g_sSection, "SoftLimitClientCount", 1000);
    }

    if (bRes)
    {
        short nMaxClient = (short)g_Config.GetInt(g_sSection, "maxgamesvr", 12);
        short nPort = (short)g_Config.GetInt(g_sSection, "internalport", 7752);
        bRes = InitSocketMgr(m_pGameSrvSocketMgr, nMaxClient, nPort);
        if (bRes)
        {
            m_linkingInfos.Initialize((unsigned short)m_pGameSrvSocketMgr->GetMaxClientCount());
        }
    }

    if (bRes)
    {
        m_bEnableRecharge = (g_Config.GetInt(g_sSection, "EnableRecharge", 1) == 0 ? false : true);
        m_bEnableFirstRechargeReward = (g_Config.GetInt(g_sSection, "EnableFirstRechargeReward", 1) == 0 ? false : true);
    }

    CurrencyTable::Instance().Reload();

#ifndef WIN32
    if (bRes)
    {
        std::string strFile(g_sPath);
        strFile.append("etc/specialuser.lst");
        FILE *fp = fopen(strFile.c_str(), "r");
        char username[MAX_NAME_LEN + 4];
        if (fp != NULL)
        {
            while (!feof(fp))
            {
                memset(username, 0, sizeof(username));
                fgets(username, MAX_NAME_LEN, fp);
                if (strlen(username) > 2)//前缀最少要有3个字符
                    g_listSpecialUser.push_back(std::string(username));
            }
            fclose(fp);
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "Can not open etc/specialuser.lst for read!\n");
        }
    }

#endif
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

    if (m_pQueryMgr)
    {
        m_pQueryMgr->AddQuery(QUERY_ResetAccountState);
        m_pQueryMgr->AddQuery(QUERY_ACCOUNT_LoadAutoAccount, 0, &m_listAutoAccount);
        //m_pQueryMgr->AddQuery(QUERY_ACCOUNT_LoadActivateCode, 0, &m_mapActivateKey);
    }

    m_pLinkerSlotPeer = new CSlotPeer();
    m_pLinkerSlotPeer->SetSocketMgr(m_pGameSrvSocketMgr);
    RegisterMessage();
    RegisterMessageProcessor();
    return bRes;
}

void CLoginCirculator::RegisterMessage()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_Account_Login);
    GAMEMSG_REGISTERCREATOR(GameMsg_Account_ServerState);
    GAMEMSG_REGISTERCREATOR(GameMsg_LinkCount);
    GAMEMSG_REGISTERCREATOR(GameMsg_Server_AccountLogout);
    GAMEMSG_REGISTERCREATOR(GameMsg_Server_AccountLogin);
    GAMEMSG_REGISTERCREATOR(GameMsg_Account_CreateAccount);
    //GAMEMSG_REGISTERCREATOR(GameMsg_Account_QuickCreateAccount);
    GAMEMSG_REGISTERCREATOR(GameMsg_Bill2Account_ChargeByAccountID);
    GAMEMSG_REGISTERCREATOR(GameMsg_Bill2Account_ChargeByPUID);
    GAMEMSG_REGISTERCREATOR(GameMsg_Account_CheckActivate);
    GAMEMSG_REGISTERCREATOR(GameMsg_Account_C2S_GetGameServersInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2A_PlayerSwithLine);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2A_AddMCoinByCmd);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2A_LockOrUnLockAccountByGm);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2A_AddMCoinByGm);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2G_SendPrivMCoin);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2A_GetServerPlayerLimit);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2A_SetPlayerLimit);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ClientDeviceInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2A_SendBlackDeviceList);
    GAMEMSG_REGISTERCREATOR(GameMsg_M2A_ChangeAccountPassword);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SaveChargeAction);
    GAMEMSG_REGISTERCREATOR(GameMsg_Account_RebindAccount);
}

void CLoginCirculator::RegisterMessageProcessor()
{
    //网络消息
    RegMsgProcessor(MSG_ACCOUNT_CreateAccount, &CLoginCirculator::OnCreateAccount);
    RegMsgProcessor(MSG_ACCOUNT_Login, &CLoginCirculator::OnAccountLogin);

    RegMsgProcessor(MSG_SYSTEM_Connect, &CLoginCirculator::OnClientConnect);
    RegMsgProcessor(MSG_SYSTEM_Disconnect, &CLoginCirculator::OnClientDisconnect);
    RegMsgProcessor(MSG_SYSTEM_Invalid, &CLoginCirculator::OnClientInvalidPacket);
    RegMsgProcessor(MSG_ACCOUNT_ServerState, &CLoginCirculator::OnServerState);
    RegMsgProcessor(MSG_SERVER_LinkCount, &CLoginCirculator::OnLinkCount);
    RegMsgProcessor(MSG_SERVER_AccountLogin, &CLoginCirculator::OnServerAccountLogin);
    RegMsgProcessor(MSG_SERVER_AccountLogout, &CLoginCirculator::OnServerAccountLogout);
    RegMsgProcessor(MSG_SERVER_ChargeByAccountID, &CLoginCirculator::OnPlayerCharge);
    RegMsgProcessor(MSG_SERVER_ChargeByPUID, &CLoginCirculator::OnPlayerChargeByPUID);
    RegMsgProcessor(MSG_ACCOUNT_CheckActivate, &CLoginCirculator::OnCheckActivateCode);
    RegMsgProcessor(MSG_Account_C2S_GetGameServersInfo, &CLoginCirculator::OnGetServersInfo);
    RegMsgProcessor(MSG_S2A_PlayerSwitchLine, &CLoginCirculator::OnPlayerSwitchLine);
    RegMsgProcessor(MSG_S2A_AddMCoinByCmd, &CLoginCirculator::OnCmdAddMCoin);
    RegMsgProcessor(MSG_S2A_LockOrUnLockAccountByGm, &CLoginCirculator::OnGMAccountLock);
    RegMsgProcessor(MSG_S2A_AddMCoinByGm, &CLoginCirculator::OnGMAddMCoin);
    RegMsgProcessor(MSG_GM_M2G_SendPrivMCoin, &CLoginCirculator::OnGMAddPrivMCoin);
    RegMsgProcessor(MSG_GM_G2A_GetServerPlayerLimit, &CLoginCirculator::OnGMGetServerLineLimit);
    RegMsgProcessor(MSG_GM_M2A_SetServerPlayerLimit, &CLoginCirculator::OnGMSetServerLineLimit);
    RegMsgProcessor(MSG_ACCOUNT_C2S_ClientDeviceInfo, &CLoginCirculator::OnClientDeviceInfo);
    RegMsgProcessor(MSG_GM_M2A_SendBlackDeviceList, &CLoginCirculator::OnGMSendBlackDeviceList);
    RegMsgProcessor(MSG_ACCOUNT_C2S_SaveChargeAction, &CLoginCirculator::OnSaveChargeAction);
    RegMsgProcessor(MSG_ACCOUNT_RebindAccount, &CLoginCirculator::OnTransformPlatform);
    RegMsgCreatorAndProcessor(GameMsg_S2G_GameServerLinkInfo, &CLoginCirculator::OnGameServerLinkInfo);
    RegMsgCreatorAndProcessor( GameMsg_PlayerCacheTransferStart, &CLoginCirculator::OnPlayerCacheTransferStart );
    RegMsgCreatorAndProcessor( GameMsg_PlayerCacheTransfer, &CLoginCirculator::OnPlayerCacheTransfer );
    RegMsgCreatorAndProcessor( GameMsg_PlayerCacheTransferEnd, &CLoginCirculator::OnPlayerCacheTransferEnd );
    RegMsgCreatorAndProcessor( GameMsg_S2A_RequestPlayerCache, &CLoginCirculator::OnRequestPlayerCache );
    RegMsgCreatorAndProcessor( GameMsg_S2A_VerifyAccount, &CLoginCirculator::OnVerifyAccount );

    //数据库
    RegQueryProcessor(QUERY_GetRegistedRoleCount, &CLoginCirculator::OnDBGetGetRegistedRoleCount);
    RegQueryProcessor(QUERY_CreateAccount, &CLoginCirculator::OnDBCreateAccount);
    RegQueryProcessor(QUERY_AccountLogin, &CLoginCirculator::OnDBAccountLogin);
    RegQueryProcessor(QUERY_QuickCreateAccount, &CLoginCirculator::OnDBQuickCreateAccount);
    RegQueryProcessor(QUERY_ACCOUNT_CreateNewAutoAccount, &CLoginCirculator::OnDBCreateNewAutoAccount);
    RegQueryProcessor(QUERY_Bill_ChongZhi, &CLoginCirculator::OnChargeCallBack);
    RegQueryProcessor(QUERY_ACCOUNT_ActivateCode, &CLoginCirculator::OnDBActivateCode);
    RegQueryProcessor(QUERY_UpdateAccountLockState, &CLoginCirculator::OnDBLockAccountCallBack);
    RegQueryProcessor(QUERY_GetFirstPaymentRewardByAccountId, &CLoginCirculator::OnGetFirstPaymentRewardByAccountIdCallBack);
    RegQueryProcessor(QUERY_GMPrivRecharge, &CLoginCirculator::OnGMPrivRechargeCallBack);
    RegQueryProcessor(QUERY_SendDeviceList, &CLoginCirculator::OnSendBlackDeviceCallBack);
    RegQueryProcessor(QUERY_DeleteDeviceList, &CLoginCirculator::OnDeleteBlackDeviceCallBack);
    RegQueryProcessor(QUERY_AccountTran_CheckTranCode, &CLoginCirculator::OnTransformPlatform_DBCheckActcode);
    RegQueryProcessor(QUERY_AccountTran_UseTranCode, &CLoginCirculator::OnTransformPlatform_DBUpdateAccount);
}

void CLoginCirculator::Release()
{
    CCirculator::Release();
}

void CLoginCirculator::PumpProc()
{
    GameMsg_Base *pMsg = NULL;
    unsigned short nSlot;

    while (m_pGameSrvSocketMgr->GetGameMsg(pMsg, nSlot))
    {
        if (pMsg != NULL)
        {
            _OnServerLink(pMsg, nSlot);
        }
        delete pMsg;
        pMsg = NULL;
    }

    CCirculator::PumpProc();
}

void CLoginCirculator::ProcessHeartBeat(int& timeElapse)
{
    CCirculator::ProcessHeartBeat(timeElapse);

    m_nServerUpTime += timeElapse;
    if (m_nServerUpTime >= 300000)//5分钟统计一次
    {
        m_nServerUpTime = 0;

        if (m_pQueryMgr)
            m_pQueryMgr->AddQuery(QUERY_AddOnlineState, GetOnlineAccountCount());

        CurrencyTable::Instance().Reload();
    }

    m_playerCacheManager.OnUpdate( timeElapse );
}

void CLoginCirculator::ReportStat()
{
    m_stat.nCltCnt = GetOnlineAccountCount();
    CCirculator::ReportStat();
}

void CLoginCirculator::_OnChargeOK(ChongZhiInfo * pInfo)
{
    int nNowTime = (int)time(NULL);

    bool bReward = false;
    if (pInfo->pChargeReward != NULL)
    {
        if (pInfo->pChargeReward->m_nRewardBindMCoin > 0
            || pInfo->pChargeReward->m_nRewardMoney > 0
            || !pInfo->pChargeReward->m_mailInfo.m_mapItems.empty())
        {
            bReward = true;
        }
    }

    if (pInfo->eChargeRewardType == EChargeRewardType_FirstCharge)
    {
        if (bReward)
        {
            EventInfoDB* pEventInfoDB = new EventInfoDB(pInfo->nRoleID, (unsigned char)GetChargeEventID(pInfo->nVersionType), pInfo->nAllAmount, nNowTime);
            m_pQueryMgr->AddQuery(QUERY_AddOrUpdateEvent, 0, pEventInfoDB);
        }
        else //没有奖励的首充需要设置奖励领取事件的时间
        {
            EventInfoDB* pEventInfoDB = new EventInfoDB(pInfo->nRoleID, (unsigned char)GetChargeSendRewardEventID(pInfo->nVersionType), 0, nNowTime);
            m_pQueryMgr->AddQuery(QUERY_AddOrUpdateEvent, 0, pEventInfoDB);
        }
    }

    WriteLog(LOGLEVEL_DEBUG, "role id:%u charge ok, version_type=%d, amount=%u", pInfo->nRoleID, pInfo->nVersionType, pInfo->nAllAmount);

    GameMsg_A2S_ChargeNotify notifyMsg;
    notifyMsg.nAccount = pInfo->nAccount;
    notifyMsg.nRoleID = pInfo->nRoleID;
    notifyMsg.strOrderId = pInfo->strOrderID;
    notifyMsg.nAmount = pInfo->nAllAmount;
    notifyMsg.nChargeTime = pInfo->nChargeTime;
    notifyMsg.bIsRetryOrder = pInfo->bIsRetryOrder;

    // 查看玩家是否在线
    AccountInfo * pAccount = GetOnlineAccount(pInfo->nAccount);
    if (pInfo->nAllAmount > 0)
    {
        if (pAccount != NULL)
        {
            notifyMsg.nAccount = pAccount->nAccount;
            notifyMsg.SessionID = pAccount->SessionID;
            SendMsgToPlayerLine(&notifyMsg, pAccount->nAccount);
        }
        else if (pInfo->bIsRetryOrder)
        {
            SendMsgToAnyLine(&notifyMsg);
        }
    }

    // 舞团红包
    GameMsg_A2S_RedEnvelope redenvelopemsg;
    redenvelopemsg.m_nRoleID = pInfo->nRoleID;
    redenvelopemsg.m_nMCoin = pInfo->nAllAmount;
    if (pAccount != NULL)
    {
        redenvelopemsg.nAccount = pAccount->nAccount;
        redenvelopemsg.SessionID = pAccount->SessionID;
        SendMsgToPlayerLine(&redenvelopemsg, pAccount->nAccount);
    }
    else
    {
        SendMsgToAnyLine(&redenvelopemsg);
    }


    if (pInfo->m_nFixedChargeBindPoint != 0)
    { // 存在固定金额首冲
        // 更新 固定首冲 状态。
        FixedChargeRecordSingle *pRec = new FixedChargeRecordSingle;
        pRec->m_nAccount = pInfo->nAccount;
        pRec->m_nMCoin = pInfo->nAmount;
        pRec->m_nChargeTime = (unsigned int)time(NULL);
        CDelHelper helper(pRec);

        m_pQueryMgr->AddQuery(QUERY_UpdateFixedChargeReward, 0, pRec, 0, 0, &helper);

        // bindMB 更新
        GameMsg_A2S_FixedChargeReward frMsg;
        frMsg.Set(pInfo->nRoleID, pInfo->nAmount, pInfo->m_nFixedChargeBindPoint);

        if (pAccount != NULL) //在线发送
            SendMsgToPlayerLine(&frMsg, pInfo->nAccount);
    }

    //如有奖励
    if (pInfo->pChargeReward != NULL)
    {
        if (bReward)
        {
            WriteLog(LOGLEVEL_DEBUG, "[INFO] CHARGE REWARD role id:%u charge reward ok, version_type=%d, eChargeRewardType=%d, amount=%u, "
                "bindmcoin_reward=%d, bindmcoin=%d, money_reward=%d, feed_money_reward=%d",
                pInfo->nRoleID, pInfo->nVersionType, pInfo->eChargeRewardType, pInfo->nAllAmount,
                pInfo->nRechargeBindMCoinReward, pInfo->pChargeReward->m_nRewardBindMCoin,
                pInfo->nRechargeMoneyReward, pInfo->nFeedVersionMoneyReward);

            GameMsg_A2S_ChargeReward chargeRewardMsg;
            chargeRewardMsg.m_nRoleID = pInfo->pChargeReward->m_nRoleID;
            chargeRewardMsg.m_strRoleName = pInfo->pChargeReward->m_strRoleName;
            chargeRewardMsg.m_nChargeRewardType = pInfo->pChargeReward->m_nChargeRewardType;
            chargeRewardMsg.m_nRewardBindMCoin = pInfo->pChargeReward->m_nRewardBindMCoin;
            chargeRewardMsg.m_nRewardMoney = pInfo->pChargeReward->m_nRewardMoney;
            chargeRewardMsg.m_mailInfo = pInfo->pChargeReward->m_mailInfo;
            chargeRewardMsg.m_nVersionType = pInfo->pChargeReward->m_nVersionType;

            if (pAccount != NULL)//在线发送
            {
                SendMsgToPlayerLine(&chargeRewardMsg, pInfo->nAccount);
            }
            else//不在线,随便找一个游戏服务器
            {
                SendMsgToAnyLine(&chargeRewardMsg);
            }
        }
        delete pInfo->pChargeReward;
        pInfo->pChargeReward = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////
//这里现在是处理GateServer发送的消息，GateServer相当于代理了所有的玩家连接//
////////////////////////////////////////////////////////////////////////////

#define RES_FAIL            -1  //失败
#define RES_OK              0   //成功
#define RES_INVALIDNAME     1   //不正确的名称或密码
#define RES_ACCOUNT_NOTEXIS 2   //账号还没有创建
#define RES_SERVER_NOT_RUN  3   //连接服务器没有开
#define RES_ALREADYLOGIN    4   //账号已经有人登录
#define RES_BLOCKED         5   //账号被锁定
#define RES_NAME_ALREADY_EXIST  6   //要注册的名字已经存在
#define RES_NOPOINTS            8   //没有剩余点数
#define RES_ACCOUNT_DISABLED    9   //账号暂时禁止登录游戏
#define RES_SERVER_FULL         10  //服务器满员
#define RES_INVALIDVERSION      11  //客户端版本错误
#define RES_ROLE_ONLINE         12
#define RES_ROLE_LOAD_ERROR     13  //LOAD角色过程出现错误

bool CLoginCirculator::_ProcessLogin(AccountInfo *pAccountInfo, int nLine /* = -1 */)
{
    if (m_pSocketMgr == NULL || pAccountInfo == NULL)
    {
        return false;
    }

    GameMsg_Account_LoginResult msgResult;
    msgResult.m_nVersionType = (unsigned char)pAccountInfo->nVersionType;
    msgResult.nAccount = pAccountInfo->nAccount;
    msgResult.m_nGateSessionID = pAccountInfo->SessionID;

    int nResult = 0;
    int nSlot = pAccountInfo->nSlot;

    //检查是否被禁用、被封停、被锁定
    if (pAccountInfo->nBlockState & STATE_BIT_DISABLE)
    {
        nResult = RES_ACCOUNT_DISABLED;
        msgResult.m_strErrorMsg = "Account_Disabled";
        WriteLog(LOGLEVEL_ERROR, "玩家%s账号未激活,不能登入！", pAccountInfo->szName);
    }
    else if (pAccountInfo->nBlockState & STATE_BIT_BLOCK)
    {
        int nBlockType = (pAccountInfo->nBlockState >> 2) & 0x00000007;
        unsigned int now = (unsigned int)time(NULL);

        if (now > pAccountInfo->nBlockTime && now - pAccountInfo->nBlockTime >= s_nBlockTime[nBlockType])
        { //block完成，解封
            pAccountInfo->nBlockTime = 0;
            pAccountInfo->nBlockState &= 0xFFFFFFE1;//解除block
        }
        else
        {
            nResult = RES_BLOCKED;
            msgResult.m_strErrorMsg = "Account_Blocked";
            WriteLog(LOGLEVEL_ERROR, "玩家%s账号被封,不能登入！", pAccountInfo->szName);
        }
    }

    if ( nResult == 0 )
    {
        bool bOnlined = false;

        LPACCOUNTINFO pOldInfo = GetOnlineAccount( pAccountInfo->nAccount );
        if ( pOldInfo != NULL )
        {
            m_mapAccountOnline.erase( pOldInfo->nAccount );
            delete pOldInfo, pOldInfo = NULL;

            nResult = RES_FAIL;
            msgResult.m_strErrorMsg = "Account_Quick_Create_Fail";
            bOnlined = true;

            WriteLog( LOGLEVEL_ERROR, "Account(%u) is already onlined.", pAccountInfo->nAccount );
        }
        else if ( m_playerCacheManager.IsInReceiving( pAccountInfo->nAccount ) )
        {
            nResult = RES_FAIL;
            msgResult.m_strErrorMsg = "Account_Quick_Create_Fail";
            bOnlined = true;

            WriteLog( LOGLEVEL_ERROR, "Account(%u) is cached.", pAccountInfo->nAccount );
        }

        //踢掉其他线登录的此角色
        if ( m_pGameSrvSocketMgr != NULL && bOnlined )
        {
            GameMsg_Server_DropClient msgDrop;
            msgDrop.nAccount = pAccountInfo->nAccount;
            BroadcastMsg2GameServer( &msgDrop );
        }
    }

    //账号无异常
    if (nResult == 0)
    {
        //登录成功需要将账号状态改为已登录！！
        //但是鉴于目前没有完整的系统保证玩家断开连接后及时将登录状态改回到未登录状态，允许重复登录
        pAccountInfo->nBlockState |= 0x00000001;//标记为已登录
        pAccountInfo->nLastLoginTime = time(NULL);

        LPACCOUNTINFO pStateQueryInfo = new AccountInfo;
        *pStateQueryInfo = *pAccountInfo;
        m_pQueryMgr->AddQuery(QUERY_UpdateAccountState, 0, pStateQueryInfo, 0, NULL);//更新账号状态
        WriteLog(LOGLEVEL_DEBUG, "[LOGIN] account[%s][%u] login from ip[%s]", pAccountInfo->szName, pAccountInfo->nAccount, pAccountInfo->szLoginIp);

        m_mapAccountOnline.insert(std::make_pair(pAccountInfo->nAccount, pAccountInfo));
        pAccountInfo->nLoginState = AOS_LOGIN;
        pAccountInfo->nAccountSessionID = GenerateSession();

        msgResult.m_nLoginResult = nResult;
        msgResult.SessionID = pAccountInfo->nAccountSessionID;
        GetRecTag(msgResult.m_szRecTag);
        SendMessageToGate(&msgResult, (unsigned short)nSlot);//票据发往GateServer

        // 将服务器允许的货币列表发给客户端
        GameMsg_S2C_AllowedCurrencyList currencyMsg;
        CurrencyTable::Instance().GetCurrencyList(currencyMsg.m_listCurrency);
        SendMessageToPlayer(&currencyMsg, pAccountInfo);
    }
    else
    {
        msgResult.m_nLoginResult = nResult;
        SendMessageToGate(&msgResult, (unsigned short)nSlot);

        delete pAccountInfo, pAccountInfo = NULL;
    }

    RemoveLoginAccount(nSlot, msgResult.m_nGateSessionID);
    return nResult == 0;
}

__int64 CLoginCirculator::GenerateSession()
{
    ++m_nAccountSessionIdCounter;

    return m_nAccountSessionIdCounter;
}

__int64 CLoginCirculator::GetSesssionID()
{
    return m_nAccountSessionIdCounter;
}

void CLoginCirculator::GetRecTag(char *szRecTag)
{
    if (szRecTag == NULL) {
        return;
    }
    int nLen = Random(MIN_REC_RANDOM_LEN, MAX_REC_RANDOM_LEN);
    for (int i = 0; i < nLen; ++i)
    {
        char c = (char)Random('a', 'z');
        szRecTag[i] = c;
    }

    std::string strID;
    Formatter(strID) << GetSesssionID();
    memcpy(&szRecTag[nLen], strID.c_str(), strID.length());
    szRecTag[nLen + strID.length()] = 0;
}

void CLoginCirculator::OnClientConnect(GameMsg_Base & pMsg, CSlotPeer & SlotPeer)
{
    GameMsg_Server_Connected msgConnect;
    msgConnect.nSvrType = SERVER_LOGIN;
    msgConnect.nCurPlayerCount = (unsigned short)m_nSoftLimitClientCount;
    SlotPeer.SendGameMsg(&msgConnect);
    TRACE("GateServer Connected on slot %d\n", SlotPeer.GetSlot());
}

void CLoginCirculator::OnClientDisconnect(GameMsg_Base & pMsg, CSlotPeer & SlotPeer)
{
    TRACE("GateServer Disconnected on slot %d\n", SlotPeer.GetSlot());
}

void CLoginCirculator::OnClientInvalidPacket(GameMsg_Base & pMsg, CSlotPeer & SlotPeer)
{
    //非法的数据包，记录错误信息，禁止Ip一段时间
    ZeroSocket::SLOT* pSlot = m_pSocketMgr->GetSlot(SlotPeer.GetSlot());
    if (pSlot != NULL)
    {
        WriteLog(LOGLEVEL_ERROR, "Invalid packet from %d, %s\n", SlotPeer.GetSlot(), inet_ntoa(pSlot->addr));
    }
    else
    {
        LOG_ERROR << "Invalid packet slot = NULL , slot = " << SlotPeer.GetSlot() << std::endl;
    }
}

void CLoginCirculator::OnServerState(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    GameMsg_Account_ServerState &rMsg = (GameMsg_Account_ServerState &)msg;
    const AccountInfo *pAccountInfo = GetOnlineAccount(rMsg.nAccount);

    if (pAccountInfo != NULL)
    {
        GameMsg_Server_State stateMsg;
        stateMsg.nAccount = pAccountInfo->nAccount;
        stateMsg.SessionID = pAccountInfo->SessionID;

        std::fill(stateMsg.aState, stateMsg.aState + LINES_IN_GROUP, (unsigned short)LineState_Disable);

        const GameServerLinkInfo *pInfo = NULL;
        const std::vector<GameServerLinkInfo *> &rLinkInfos = m_linkingInfos.GetAllLinkInfos();
        for (std::vector<GameServerLinkInfo *>::const_iterator itr = rLinkInfos.begin();
            itr != rLinkInfos.end(); ++itr)
        {
            pInfo = *itr;
            if (pInfo == NULL)
                continue;

            int nLine = pInfo->GetLine();
            if (nLine < 0 || nLine >= LINES_IN_GROUP)
                continue;

            unsigned short nClient = pInfo->GetClient();
            if (nClient < m_nSoftLimitClientCount * 3 / 8)
            {
                stateMsg.aState[nLine] = LineState_Good;
            }
            else if (nClient < m_nSoftLimitClientCount * 5 / 8)
            {
                stateMsg.aState[nLine] = LineState_Normal;
            }
            else if (nClient == m_nSoftLimitClientCount)
            {
                stateMsg.aState[nLine] = LineState_Full;
            }
            else
            {
                stateMsg.aState[nLine] = LineState_Bad;
            }
        }

        SendMessageToGate(&stateMsg, pAccountInfo->nSlot);
    }
}

void CLoginCirculator::_OnServerLink(LPGameMsg_Base pGameMsg, unsigned short &nSlot)
{
    switch (pGameMsg->nMsg)
    {
    case MSG_SYSTEM_Connect:
        break;
    case MSG_SYSTEM_Disconnect:
        {
            int nLine = m_linkingInfos.RemoveServerInfoBySlot( nSlot );
            if ( nLine != INVALID_LINE )
            {
                WriteLog( LOGLEVEL_SYSTEM, "GameServer disconnected. Line: %d, slot: %u.\n", nLine, nSlot );

                //游戏服务器断线，则上面的所有账号均断线
                for ( OnlineAccountMap::iterator it = m_mapAccountOnline.begin(); it != m_mapAccountOnline.end(); )
                {
                    if ( it->second->nSlot == nSlot )
                    {
                        m_pQueryMgr->AddQuery( QUERY_ResetAccountState, it->first );

                        SAFE_DELETE( it->second );
                        m_mapAccountOnline.erase( it++ );
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
            break;
        }
    default:
        m_pLinkerSlotPeer->SetSlot(nSlot);
        GameMsg_Map::Instantce().ProcessMsg(*pGameMsg, *m_pLinkerSlotPeer);
        break;
    }
}

void CLoginCirculator::OnLinkCount(GameMsg_Base &msg, CSlotPeer &SlotPeer)
{
    unsigned short nClient = ((GameMsg_LinkCount &)msg).nCount;
    m_linkingInfos.SetClientBySlot(SlotPeer.GetSlot(), nClient);
}

void CLoginCirculator::OnGameServerLinkInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
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
        if (m_pGameSrvSocketMgr != NULL)
        {
            GameMsg_G2S_GameServerLinkInfoResult resMsg;
            resMsg.m_nServerType = SERVER_LOGIN;
            m_pGameSrvSocketMgr->SendGameMsg(&resMsg, nSlot);

            GameMsg_A2S_SyncChargeSetting syncMsg;
            syncMsg.m_bEnableRecharge = m_bEnableRecharge;
            syncMsg.m_bEnableFirstRechargeReward = m_bEnableFirstRechargeReward;
            m_pGameSrvSocketMgr->SendGameMsg(&syncMsg, nSlot);
        }

        WriteLog(LOGLEVEL_SYSTEM, "GameServer connected success. Line: %d, slot: %u, addr: %s, port: %d\n",
            rMsg.m_linkInfo.GetLine(), nSlot, inet_ntoa(addr), rMsg.m_linkInfo.GetPort());
    }
}

void CLoginCirculator::OnServerAccountLogin(GameMsg_Base & pMsg, CSlotPeer & SlotPeer)
{
    GameMsg_Server_AccountLogin* pLogin = (GameMsg_Server_AccountLogin*)&pMsg;
    AccountInfo  * pAccounfInfo = GetOnlineAccount(pLogin->nAccount);
    if (pAccounfInfo != NULL)
    {
        pAccounfInfo->nLoginState = AOS_GAME;
        pAccounfInfo->nSlot = SlotPeer.GetSlot();
    }
}

void CLoginCirculator::OnServerAccountLogout(GameMsg_Base & rMsg, CSlotPeer & SlotPeer)
{
    GameMsg_Server_AccountLogout* pMsg = (GameMsg_Server_AccountLogout*)&rMsg;
    if (pMsg != NULL && pMsg->nAccount > 0)
    {
        AccountInfo *pAccounfInfo = GetOnlineAccount(pMsg->nAccount);
        if (pAccounfInfo != NULL && pAccounfInfo->nSlot == SlotPeer.GetSlot())
        {
            m_pQueryMgr->AddQuery(QUERY_AccountLogout, pMsg->nAccount, NULL, pAccounfInfo->nLastLoginTime);

            m_mapAccountOnline.erase(pMsg->nAccount);
            delete pAccounfInfo, pAccounfInfo = NULL;
        }
    }
}

void CLoginCirculator::OnAccountLogin(GameMsg_Base & msg, CSlotPeer & SlotPeer)
{
    GameMsg_Account_Login* pLoginMsg = (GameMsg_Account_Login *)&msg;

    GameMsg_Account_LoginResult resultMsg;
    resultMsg.m_nVersionType = pLoginMsg->m_nVersionType;
    resultMsg.m_nGateSessionID = pLoginMsg->m_nGateSessionID;

    // 防止重复登录
    if (GetLoginAccount(SlotPeer.GetSlot(), pLoginMsg->m_nGateSessionID) != NULL)
    {
        WriteLog(LOGLEVEL_ERROR, "[LOGIN] duplicated login: %d %s",
            (int)pLoginMsg->m_nPID, pLoginMsg->m_strPUID.c_str());
        return;
    }

    // GameServer没有连接上Account
    if (m_pGameSrvSocketMgr == NULL || m_pGameSrvSocketMgr->GetCurClientCount() == 0)
    {
        resultMsg.m_nLoginResult = RES_SERVER_NOT_RUN;
        resultMsg.m_strErrorMsg = "Account_Server_Not_Run";
        m_pSocketMgr->SendGameMsg(&resultMsg, SlotPeer.GetSlot());//票据发往GateServer

        WriteLog(LOGLEVEL_ERROR, "[LOGIN] GameServer down, player: %d %s",
            (int)pLoginMsg->m_nPID, pLoginMsg->m_strPUID.c_str());
        return;
    }

    //ddianle平台检查用户名/密码
    if (pLoginMsg->m_nPID == PID_INTERNAL)
    {
        if (pLoginMsg->m_strName.length() >= MAX_NAME_LEN)
        {
            resultMsg.m_nLoginResult = INVALID_NAME_LENGTH;
            resultMsg.m_strErrorMsg = "Account_Invalid_Name_Len";
            m_pSocketMgr->SendGameMsg(&resultMsg, SlotPeer.GetSlot());
            return;
        }
    }

    // 检查指定线是否连接正常
    const GameServerLinkInfo *pLinkInfo = m_linkingInfos.GetLinkInfoByLine(pLoginMsg->m_nLine);

    if (pLinkInfo == NULL)
    {
        resultMsg.m_nLoginResult = RES_SERVER_NOT_RUN;
        resultMsg.m_strErrorMsg = "Account_Server_Not_Run";
        m_pSocketMgr->SendGameMsg(&resultMsg, SlotPeer.GetSlot());

        WriteLog(LOGLEVEL_ERROR, "[LOGIN] GameServer down, line:%d player: %d %s",
            pLoginMsg->m_nLine, (int)pLoginMsg->m_nPID, pLoginMsg->m_strPUID.c_str());
        return;
    }

    if (pLinkInfo->GetClient() >= m_nSoftLimitClientCount)
    {
        resultMsg.m_nLoginResult = RES_SERVER_FULL;
        m_pSocketMgr->SendGameMsg(&resultMsg, SlotPeer.GetSlot());

        WriteLog(LOGLEVEL_ERROR, "[LOGIN] GameServer down, line:%d is full, player: %d %s",
            pLoginMsg->m_nLine, (int)pLoginMsg->m_nPID, pLoginMsg->m_strPUID.c_str());
        return;
    }

    // 准备账号查询信息
    AccountInfo *pInfo = new AccountInfo;
    safememcpy(pInfo->szName, MAX_NAME_LEN - 1, pLoginMsg->m_strName.c_str(), pLoginMsg->m_strName.length());
    safememcpy(pInfo->szPhone, MAX_PHONE_LEN - 1, pLoginMsg->m_strPhone.c_str(), pLoginMsg->m_strPhone.length());
    pInfo->nVersionType = pLoginMsg->m_nVersionType;
    pInfo->SessionID = pLoginMsg->m_nGateSessionID;
    pInfo->nSlot = SlotPeer.GetSlot();
    pInfo->nPID = pLoginMsg->m_nPID;
    safememcpy(pInfo->szPUID, MAX_PUID_LEN, pLoginMsg->m_strPUID.c_str(), pLoginMsg->m_strPUID.length());
    safememcpy(pInfo->szLoginIp, MAX_IP_LEN, pLoginMsg->m_strLoginIP.c_str(), pLoginMsg->m_strLoginIP.length());

    string strPassword = BinaryData2String((const unsigned char*)pLoginMsg->m_szPass, MAX_MD5_LEN - 1);
    char * pstrPasswd = new char[MAX_PASS_LEN];
    memset(pstrPasswd, 0, MAX_PASS_LEN);
    memcpy(pstrPasswd, strPassword.c_str(), MAX_PASS_LEN - 1);
    memcpy(pInfo->szPass, pstrPasswd, MAX_PASS_LEN - 1);

    // 执行DB查询
    m_pQueryMgr->AddQuery(QUERY_AccountLogin, 0, pInfo, pLoginMsg->m_nLine, pstrPasswd);

    // 插入在线玩家列表
    AddLoginAccount(pLoginMsg->m_nGateSessionID, pInfo);
}


void CLoginCirculator::OnCreateAccount(GameMsg_Base &rMsg, CSlotPeer & SlotPeer)
{
    GameMsg_Account_CreateAccount *pMsg = (GameMsg_Account_CreateAccount *)&rMsg;

    // 防止重复登录
    if (GetLoginAccount(SlotPeer.GetSlot(), pMsg->nGateSessionID) != NULL)
    {
        WriteLog(LOGLEVEL_ERROR, "[ACCOUNT] double account login");
        return;
    }

    // 服务器是否正常开启
    if (m_pQueryMgr == NULL)
    {
        WriteLog(LOGLEVEL_ERROR, "[ACCOUNT] server is not ready");
        return;
    }

    GameMsg_Account_CreateAccountResult msgResult;
    msgResult.m_nGateSessionID = pMsg->nGateSessionID;

    // 账号名检查
    int nAccountLen = strlen(pMsg->szName);
    if (nAccountLen <= 0 || nAccountLen >= MAX_NAME_LEN)
    {
        msgResult.m_nResult = INVALID_NAME_LENGTH;
        msgResult.m_strResultMsg = (nAccountLen > 0 ? "Account_Name_Too_Long" : "Account_Name_Is_Empty");
        m_pSocketMgr->SendGameMsg(&msgResult, SlotPeer.GetSlot());//票据发往GateServer

        WriteLog(LOGLEVEL_ERROR, "[ACCOUNT]invalid account name=%s, length=%d", pMsg->szName, nAccountLen);
    }
    else if (!NameDeny::Instance().IsValidText(pMsg->szName))
    {
        msgResult.m_nResult = CONTAIN_INVALID_WORDS;
        msgResult.m_strResultMsg = "Account_Contain_Invalid_Word";
        m_pSocketMgr->SendGameMsg(&msgResult, SlotPeer.GetSlot());//票据发往GateServer

        WriteLog(LOGLEVEL_ERROR, "[ACCOUNT]invalid account name=%s", pMsg->szName);
    }
    else
    {
        string strPassword = BinaryData2String((const unsigned char*)pMsg->szPass, MAX_MD5_LEN - 1);
        AccountInfo *pAccountInfo = new AccountInfo;
        pAccountInfo->nVersionType = pMsg->nVersionType;
        pAccountInfo->SessionID = pMsg->nGateSessionID;
        pAccountInfo->nSlot = SlotPeer.GetSlot();
        pAccountInfo->nPID = PID_INTERNAL;
        safememcpy(pAccountInfo->szName, MAX_NAME_LEN - 1, pMsg->szName, MAX_NAME_LEN - 1);
        safememcpy(pAccountInfo->szPass, MAX_PASS_LEN - 1, strPassword.c_str(), strPassword.length());
        safememcpy(pAccountInfo->szLoginIp, MAX_IP_LEN - 1, pMsg->szLoginIp, strlen(pMsg->szLoginIp));

        AddLoginAccount(pAccountInfo->SessionID, pAccountInfo);

        m_pQueryMgr->AddQuery(QUERY_CreateAccount, 0, pAccountInfo);

        WriteLog(LOGLEVEL_DEBUG, "[ACCOUNT]try to create account=%s", pAccountInfo->szName);
    }
}

void CLoginCirculator::OnQuickCreateAccount(GameMsg_Base &pMsg, CSlotPeer & SlotPeer)
{
    GameMsg_Account_QuickCreateAccount *pQuickCreateAccountMsg = (GameMsg_Account_QuickCreateAccount *)&pMsg;

    if (GetLoginAccount(SlotPeer.GetSlot(), pQuickCreateAccountMsg->SessionID) != NULL) // 防止重复登录  
        return;

    if (pQuickCreateAccountMsg != NULL)
    {
        AccountInfo *pCreateAccount = new AccountInfo;
        pCreateAccount->nSlot = SlotPeer.GetSlot();
        pCreateAccount->SessionID = pQuickCreateAccountMsg->m_nGateSessionID;

        if (m_listAutoAccount.size() > 0)
        {
            CAutoAccount *pInfo = m_listAutoAccount.front();
            if (pInfo != NULL)
            {
                unsigned char szPassWordMD5[MAX_PASS_LEN];
                memset(szPassWordMD5, 0, MAX_PASS_LEN);
                MDString(pInfo->m_szPasswd, szPassWordMD5);

                pCreateAccount->nAccount = pInfo->m_nAccountID;
                memcpy(pCreateAccount->szName, pInfo->m_szName, MAX_NAME_LEN - 1);
                memcpy(pCreateAccount->szPass, szPassWordMD5, MAX_PASS_LEN - 1);
                m_pQueryMgr->AddQuery(QUERY_QuickCreateAccount, 0, pInfo, 0, pCreateAccount);
            }
        }
        else
        {
            m_pQueryMgr->AddQuery(QUERY_ACCOUNT_CreateNewAutoAccount, AUTO_CREATE_ACCOUNT_INCREAMENT, &m_listAutoAccount, 0, pCreateAccount); // 该变量信息为填充，不影响
        }
        AddLoginAccount(pQuickCreateAccountMsg->m_nGateSessionID, pCreateAccount);
    }
}

void CLoginCirculator::OnPlayerCharge(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_Bill2Account_ChargeByAccountID * pChargeMsg = (GameMsg_Bill2Account_ChargeByAccountID *)&msg;
    if (pChargeMsg != NULL)
    {
        WriteLog(LOGLEVEL_DEBUG, "[CHARGE]BY ACCOUNTID accountid=%u deviceid=%s currencyType=%d "
            "orderid=%s allMCoin=%d mcoin=%d currencyAmount=%d isValidPayment=%s ChargeTime=%u "
            "isRetryOrder=%s retryTimes=%d partnerOrderID=%s",
            msg.nAccount, pChargeMsg->strDeviceID.c_str(), (int)pChargeMsg->nCurrencyType,
            pChargeMsg->strOrderId.c_str(), pChargeMsg->nAllAmount, pChargeMsg->nAmount,
            pChargeMsg->nCurrency, pChargeMsg->bIsValidPayment != 0 ? "true" : "false",
            pChargeMsg->nChargeTime, pChargeMsg->bIsRetryOrder ? "true" : "false",
            (int)pChargeMsg->nRetryTimes, pChargeMsg->strPartnerOrderID.c_str());

        // 检查是否是黑名单设备，如果消息中的设备号不是黑名单设备，就再多检查一个账号对应的设备是否是黑名单设备，增强健壮性
        bool bIsInBlackList = CurrencyTable::Instance().IsBlackListDevice(pChargeMsg->strDeviceID);
        AccountInfo *pAccount = GetOnlineAccount(msg.nAccount);
        if (!bIsInBlackList && pAccount != NULL)
        {
            bIsInBlackList |= CurrencyTable::Instance().IsBlackListDevice(pAccount->strDeviceID);
        }

        if (bIsInBlackList && pAccount != NULL)
        {
            GameMsg_S2C_ValidChargeDevice validMsg;
            validMsg.m_bIsInBlackList = bIsInBlackList;

            SendMessageToPlayer(&validMsg, pAccount);
        }

        // 检查是否是合法的支付货币，设备是否在黑名单中
        if (pChargeMsg->bIsValidPayment == 0
            || !CurrencyTable::Instance().IsValidCurrency(pChargeMsg->nCurrencyType)
            || bIsInBlackList)
        {
            WriteLog(LOGLEVEL_ERROR, "[CHARGE]BY ACCOUNTID invalid currency accountid=%u "
                "deviceid=%s isBlackDevice=%s currencyType=%d",
                msg.nAccount, pChargeMsg->strDeviceID.c_str(), bIsInBlackList ? "true" : "false",
                (int)pChargeMsg->nCurrencyType);
            return;
        }

        if (pChargeMsg->nAccount == 0 || pChargeMsg->strOrderId.length() == 0 || pChargeMsg->nAllAmount == 0)
        {
            WriteLog(LOGLEVEL_ERROR, "[CHARGE]BY ACCOUNTID invalid param accountid=%u orderid=%s amount=%u",
                msg.nAccount, pChargeMsg->strOrderId.c_str(), pChargeMsg->nAllAmount);
            return;
        }

        // 准备充值,先获得玩家首充信息,再计算玩家充值奖励,因为玩家充值奖励与首充互斥
        ChongZhiInfo *pInfo = new ChongZhiInfo();
        pInfo->nPID = pChargeMsg->nPid;
        pInfo->strOrderID = pChargeMsg->strOrderId;
        pInfo->strPartnerOrderID = pChargeMsg->strPartnerOrderID;
        pInfo->nAccount = pChargeMsg->nAccount;
        pInfo->nVersionType = Version_Free;
        pInfo->nAmount = pChargeMsg->nAmount;
        pInfo->nAllAmount = pChargeMsg->nAllAmount;
        pInfo->nCurrency = pChargeMsg->nCurrency;
        pInfo->strDeviceID = pChargeMsg->strDeviceID;
        pInfo->nFeedVersionMoneyRewardRatio = 0;
        pInfo->nChargeTime = pChargeMsg->nChargeTime;
        pInfo->bIsRetryOrder = pChargeMsg->bIsRetryOrder;

        RechargeRewardInfo* pRechargeRewardInfo = new RechargeRewardInfo();
        pRechargeRewardInfo->pChongZhiInfo = pInfo;
        pRechargeRewardInfo->nAccountID = pChargeMsg->nAccount;

        FillFixedChargeData(pRechargeRewardInfo, pChargeMsg->nAmount);

        m_pQueryMgr->AddQuery(QUERY_GetFirstPaymentRewardByAccountId, 0, pRechargeRewardInfo);
    }
}

void CLoginCirculator::FillFixedChargeData(RechargeRewardInfo* pRechargeRewardInfo, unsigned amount)
{
    if (0 != PaymentInfoMgr::Instance().FixedChargeGiftCoin(amount)
        && PaymentInfoMgr::Instance().FixedChargeIsRun(time(NULL), NULL, NULL))
    { // 需要查询充值信息
        pRechargeRewardInfo->m_fixedChargeRecord.m_nMCoin = amount;
    }
}


void CLoginCirculator::OnPlayerChargeByPUID(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_Bill2Account_ChargeByPUID * pChargeMsg = (GameMsg_Bill2Account_ChargeByPUID *)&msg;
    if (pChargeMsg != NULL && !pChargeMsg->strPUID.empty() && strlen(pChargeMsg->strOrderId.c_str()) > 0 && pChargeMsg->nAmount > 0)
    {
        WriteLog(LOGLEVEL_DEBUG, "[CHARGE]BY PUID accountid=%u pid=%d puid=%s "
            "orderid=%s allMCoin=%d mcoin=%d currencyAmount=%d",
            msg.nAccount, pChargeMsg->nPid, pChargeMsg->strPUID.c_str(),
            pChargeMsg->strOrderId.c_str(), pChargeMsg->nAllAmount,
            pChargeMsg->nAmount, pChargeMsg->nCurrency);

        // 准备充值,先获得玩家首充信息,再计算玩家充值奖励,因为玩家充值奖励与首充互斥
        ChongZhiByPUIDInfo *pInfo = new ChongZhiByPUIDInfo();
        pInfo->czInfo.nPID = pChargeMsg->nPid;
        pInfo->czInfo.strOrderID = pChargeMsg->strOrderId;
        pInfo->czInfo.nVersionType = Version_Free;
        pInfo->czInfo.nAmount = pChargeMsg->nAmount;
        pInfo->czInfo.nAllAmount = pChargeMsg->nAllAmount;
        pInfo->czInfo.nCurrency = pChargeMsg->nCurrency;
        pInfo->strPUID = pChargeMsg->strPUID;
        pInfo->czInfo.nFeedVersionMoneyRewardRatio = 0;

        RechargeRewardInfo* pRechargeRewardInfo = new RechargeRewardInfo();
        pRechargeRewardInfo->pChongZhiByPUIDInfo = pInfo;
        pRechargeRewardInfo->nAccountID = pChargeMsg->nAccount;

        FillFixedChargeData(pRechargeRewardInfo, pChargeMsg->nAmount);

        m_pQueryMgr->AddQuery(QUERY_GetFirstPaymentRewardByAccountId, 0, pRechargeRewardInfo);
    }
}

void CLoginCirculator::OnClientDeviceInfo(GameMsg_Base& msg, CSlotPeer& SlotPeer)
{
    std::string strCurrencyType;
    GameMsg_C2S_ClientDeviceInfo &rMsg = (GameMsg_C2S_ClientDeviceInfo &)msg;

    bool bIsInBlackList = CurrencyTable::Instance().IsBlackListDevice(rMsg.m_strDeviceId);

    AccountInfo *pAccount = GetOnlineAccount(rMsg.nAccount);
    if (pAccount != NULL)
    {
        pAccount->strDeviceID = rMsg.m_strDeviceId;

        GameMsg_S2C_ValidChargeDevice validMsg;
        //validMsg.m_strDeviceID = rMsg.m_strDeviceId;
        validMsg.m_bIsInBlackList = bIsInBlackList;

        SendMessageToPlayer(&validMsg, pAccount);
    }

    rMsg.ToCurrencyString(strCurrencyType);
    WriteLog(LOGLEVEL_DEBUG, "[DEVICE] accountid=%u deviceid=%s isBlackDevice=%s currencyTypeList=%s",
        rMsg.nAccount, rMsg.m_strDeviceId.c_str(), bIsInBlackList ? "true" : "false", strCurrencyType.c_str());
}

void CLoginCirculator::OnSaveChargeAction(GameMsg_Base& pMsg, CSlotPeer& SlotPeer)
{
    GameMsg_C2S_SaveChargeAction & chargeMsg = (GameMsg_C2S_SaveChargeAction&)pMsg;

    ChargeAction * pChargeAction = new ChargeAction(chargeMsg.m_Action);
    m_pQueryMgr->AddQuery(QUERY_Bill_SaveChargeAction, 0, pChargeAction);
}

void CLoginCirculator::OnPlayerCacheTransferStart( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_PlayerCacheTransferStart &rMsg = (GameMsg_PlayerCacheTransferStart &)msg;

    m_playerCacheManager.ReceiveCacheDataBegin( rMsg.m_nAccountID );
}

void CLoginCirculator::OnPlayerCacheTransfer( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_PlayerCacheTransfer &rMsg = (GameMsg_PlayerCacheTransfer &)msg;

    m_playerCacheManager.ReceiveCacheData( rMsg.m_nAccountID, rMsg.m_nKey, rMsg.m_nAllDataSize,
        rMsg.m_nThisSize, rMsg.m_pData );
}

void CLoginCirculator::OnPlayerCacheTransferEnd( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_PlayerCacheTransferEnd &rMsg = (GameMsg_PlayerCacheTransferEnd &)msg;

    m_playerCacheManager.ReceiveCacheDataEnd( rMsg.m_nAccountID );
}

void CLoginCirculator::OnRequestPlayerCache( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_S2A_RequestPlayerCache &rMsg = (GameMsg_S2A_RequestPlayerCache &)msg;
    unsigned short slot = slotPeer.GetSlot();

    unsigned int nAccountID = rMsg.nAccount;
    LPACCOUNTINFO pOldInfo = GetOnlineAccount( nAccountID );

    GameMsg_A2S_RequestPlayerCacheRes resMsg;
    resMsg.nAccount = nAccountID;
    resMsg.SessionID = rMsg.SessionID;
    if ( pOldInfo != NULL && pOldInfo->nAccountSessionID == rMsg.SessionID && m_playerCacheManager.IsCacheSuccess( nAccountID ) )
    {
        resMsg.m_bCacheSuccess = true;
        resMsg.m_bHasCache = m_playerCacheManager.HasCached( rMsg.nAccount );
    }
    else
    {
        resMsg.m_bCacheSuccess = false;
        resMsg.m_bHasCache = false;
    }
    m_pGameSrvSocketMgr->SendGameMsg( &resMsg, slot );

    if ( resMsg.m_bHasCache )
    {
        GameMsg_PlayerCacheTransferStart startMsg;
        startMsg.m_nAccountID = rMsg.nAccount;
        m_pGameSrvSocketMgr->SendGameMsg( &startMsg, slot );

        GameMsg_PlayerCacheTransfer transferMsg;
        transferMsg.m_nAccountID = rMsg.nAccount;
        std::string strData;

        while ( m_playerCacheManager.GetNextPlayerCacheData( rMsg.nAccount, transferMsg.m_nKey, transferMsg.m_nAllDataSize, strData ) )
        {
            transferMsg.m_nThisSize = strData.size();
            transferMsg.m_pData = const_cast<char *>( strData.c_str() );

            if ( transferMsg.m_nThisSize > 0 )
            {
                m_pGameSrvSocketMgr->SendGameMsg( &transferMsg, slot );
            }
        }

        GameMsg_PlayerCacheTransferEnd endMsg;
        endMsg.m_nAccountID = rMsg.nAccount;
        m_pGameSrvSocketMgr->SendGameMsg( &endMsg, slot );
    }
}

void CLoginCirculator::OnVerifyAccount( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    GameMsg_S2A_VerifyAccount *pVerifyMsg = dynamic_cast<GameMsg_S2A_VerifyAccount *>(&msg);
    if ( pVerifyMsg == NULL )
        return;

    GameMsg_A2S_VerifyAccountRes resMsg;
    resMsg.nAccount = pVerifyMsg->nAccount;
    resMsg.SessionID = pVerifyMsg->SessionID;
    resMsg.m_bVerifyingPassed = false;
    resMsg.m_nSlot = pVerifyMsg->m_nSlot;
    resMsg.m_nInstallerID = pVerifyMsg->m_nInstallerID;
    resMsg.m_nVersionType = pVerifyMsg->m_nVersionType;

    AccountInfo *pAccount = GetOnlineAccount( pVerifyMsg->nAccount );
    if ( pAccount != NULL && pAccount->nAccountSessionID == pVerifyMsg->SessionID )
        resMsg.m_bVerifyingPassed = true;

    if ( m_pGameSrvSocketMgr != NULL )
        m_pGameSrvSocketMgr->SendGameMsg( &resMsg, slotPeer.GetSlot() );
}

// 账号迁移处理函数
void CLoginCirculator::OnTransformPlatform(GameMsg_Base& pMsg, CSlotPeer& SlotPeer)
{
    GameMsg_Account_RebindAccount &msg = (GameMsg_Account_RebindAccount &)pMsg;

    // 进入数据库验证
    SharedPtr<RebindAccountInfo> pInfo(new RebindAccountInfo);

    pInfo->m_nGateSessionID = msg.SessionID;
    pInfo->m_nSlot = SlotPeer.GetSlot();

    pInfo->m_strName = msg.m_szName;
    pInfo->m_strPass = msg.m_szPass;
    pInfo->m_nPid = msg.m_nPid;
    pInfo->m_strPuid = msg.m_szPuid;
    pInfo->m_strActcode = msg.m_szActcode;
    pInfo->m_nServerid = msg.m_nServerid;



    //     GameMsg_Account_RebindAccountResult retMsg;
    //     retMsg.m_nServerid = pInfo->m_nServerid;
    //     retMsg.m_nPid = pInfo->m_nPid;
    //     retMsg.m_nResult = 0;
    //     retMsg.m_strName = pInfo->m_strName;
    //     retMsg.m_strPwd = pInfo->m_strPass;
    //     retMsg.m_strPuid = pInfo->m_strPuid;
    //     retMsg.m_strActCode = pInfo->m_strActcode;
    // 
    //     retMsg.SessionID = pInfo->m_nGateSessionID;
    //     m_pSocketMgr->SendGameMsg(&retMsg, pInfo->m_nSlot);

    m_pQueryMgr->AddQuery(QUERY_AccountTran_CheckTranCode, pInfo);

    return;
}

// 账号迁移，激活码验证之后的处理流程
void CLoginCirculator::OnTransformPlatform_DBCheckActcode(QueryBase& pQuery)
{
    // 如果激活码有效，则执行更新
    RebindAccountInfo *pRebindInfo = ((Query<RebindAccountInfo>&)pQuery).Get();

    if (NULL == pRebindInfo)
    {
        WriteLog(LOGLEVEL_ERROR, "error to find rebind info ptr");
        return;
    }

    if (0 != pRebindInfo->m_nResult)
    {
        //GameMsg_Account_RebindAccountResult retMsg;
        //retMsg.m_nResult = pRebindInfo->m_nResult;
        //retMsg.SessionID = pRebindInfo->m_nGateSessionID;
        //m_pSocketMgr->SendGameMsg(&retMsg, pRebindInfo->m_nSlot);

        return;
    }

    // 刷新玩家账号数据，删除激活码信息，并且记录入表
    m_pQueryMgr->AddQuery(QUERY_AccountTran_UseTranCode, ((Query<RebindAccountInfo>&)pQuery).GetSharedPtr());

    return;
}

// 账号迁移，迁移完毕之后的处理流程
void CLoginCirculator::OnTransformPlatform_DBUpdateAccount(QueryBase& pQuery)
{
    RebindAccountInfo *pRebindInfo = ((Query<RebindAccountInfo>&)pQuery).Get();
    if (NULL == pRebindInfo)
    {
        WriteLog(LOGLEVEL_ERROR, "error to find rebindInfo...");
        return;
    }

    AccountInfo* pAccount = GetOnlineAccount(pRebindInfo->m_nAccount);
    if (NULL != pAccount)
        KickPlayOff(pAccount, DropPlayerCause_ReLogin);

    // 更新成功之后，回复成功消息。
    // 回复，流程成功
    GameMsg_Account_RebindAccountResult retMsg;
    retMsg.m_nServerid = pRebindInfo->m_nServerid;
    retMsg.m_nPid = pRebindInfo->m_nPidOld;
    retMsg.m_nResult = 1;
    retMsg.m_strName = pRebindInfo->m_strName;
    retMsg.m_strPwd = pRebindInfo->m_strPass;
    retMsg.m_strPuid = pRebindInfo->m_strPuid;
    retMsg.m_strActCode = pRebindInfo->m_strActcode;

    retMsg.SessionID = pRebindInfo->m_nGateSessionID;
    m_pSocketMgr->SendGameMsg(&retMsg, pRebindInfo->m_nSlot);

    return;
}


void CLoginCirculator::OnCheckActivateCode(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_Account_CheckActivate *pCheckMsg = (GameMsg_Account_CheckActivate *)&msg;

    list<__int64>::iterator itr = find(m_listActivateQueue.begin(), m_listActivateQueue.end(), pCheckMsg->SessionID);
    if (itr != m_listActivateQueue.end()) //激活处理中
    {
        return;
    }

    map<unsigned int, unsigned char>::iterator mapItr = m_mapActivateKey.find(atoi(pCheckMsg->m_szActivateCode));
    if (mapItr != m_mapActivateKey.end() && mapItr->second != 0)// 存在该激活码且没有被激活
    {
        m_listActivateQueue.push_back(pCheckMsg->SessionID);

        mapItr->second = 0;
        AccountInfo *pInfo = new AccountInfo;
        pInfo->SessionID = pCheckMsg->SessionID;
        pInfo->nAccount = pCheckMsg->nAccount;
        safememcpy(pInfo->szPUID, MAX_PUID_LEN, pCheckMsg->m_strPUID.c_str(), pCheckMsg->m_strPUID.length());
        pInfo->nSlot = slotPeer.GetSlot();

        m_pQueryMgr->AddQuery(QUERY_ACCOUNT_ActivateCode, mapItr->first, pInfo, pCheckMsg->m_nLine);
    }
    else// 不存在该激活码或该激活码已被使用
    {
        GameMsg_Account_CheckActivateResult resMsg;
        resMsg.m_nResult = 1;
        resMsg.SessionID = msg.SessionID;
        m_pSocketMgr->SendGameMsg(&resMsg, slotPeer.GetSlot());
    }
}

void CLoginCirculator::OnGetServersInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_Account_C2S_GetGameServersInfo &rMsg = (GameMsg_Account_C2S_GetGameServersInfo &)msg;

    GameMsg_Account_S2C_GetGameServersInfoRes serverInfoMsg;
    serverInfoMsg.SessionID = rMsg.SessionID;
    serverInfoMsg.nAccount = rMsg.nAccount;

    const GameServerLinkInfo *pInfo = NULL;
    const std::vector<GameServerLinkInfo *> &rLinkInfos = m_linkingInfos.GetAllLinkInfos();
    for (std::vector<GameServerLinkInfo *>::const_iterator itr = rLinkInfos.begin();
        itr != rLinkInfos.end(); ++itr)
    {
        pInfo = *itr;
        if (pInfo == NULL)
            continue;

        CGameServerInfo info;
        info.m_nLine = pInfo->GetLine();
        info.m_strServerName = pInfo->GetName();
        info.m_nOnlinePlayersNum = pInfo->GetClient();
        info.m_nLimitNum = m_nSoftLimitClientCount;
        info.m_nInstallerID = pInfo->GetInstallerType();
        info.m_bEnableAudition = pInfo->HasEnableAudition();

        serverInfoMsg.m_listGameServersInfo.push_back(info);
    }

    AccountInfo *pPlayerInfo = GetOnlineAccount(rMsg.nAccount);
    if (pPlayerInfo != NULL)
        serverInfoMsg.m_nCurLine = m_linkingInfos.GetLineBySlot(pPlayerInfo->nSlot);
    else
        serverInfoMsg.m_nCurLine = -1;

    SendMessageToGate(&serverInfoMsg, slotPeer.GetSlot());
}

void CLoginCirculator::OnPlayerSwitchLine(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2A_PlayerSwithLine &rMsg = (GameMsg_S2A_PlayerSwithLine &)msg;
    AccountInfo *pAccountInfo = GetOnlineAccount(rMsg.nAccount);
    if (pAccountInfo != NULL)
        pAccountInfo->nSlot = slotPeer.GetSlot();
}

void CLoginCirculator::OnCmdAddMCoin(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2A_AddMCoinByCmd *pMsg = (GameMsg_S2A_AddMCoinByCmd  *)&msg;

    AccountInfo *pAccountInfo = GetOnlineAccount(pMsg->nAccount);
    if (pAccountInfo == NULL)
    {
        return;
    }
    if (pMsg != NULL && pMsg->nAccount > 0 && pMsg->nAmount > 0)
    {
        ChongZhiInfo *pInfo = new ChongZhiInfo();
        pInfo->nPID = pAccountInfo->nPID;
        Formatter(pInfo->strOrderID) << "addbycmd" << (unsigned int)time(NULL);
        pInfo->nAccount = pAccountInfo->nAccount;
        pInfo->nVersionType = Version_Free;
        pInfo->nAmount = pMsg->nAmount;
        pInfo->nAllAmount = pMsg->nAmount;
        pInfo->nFeedVersionMoneyRewardRatio = 0;

        RechargeRewardInfo* pRechargeRewardInfo = new RechargeRewardInfo();
        pRechargeRewardInfo->pChongZhiInfo = pInfo;
        pRechargeRewardInfo->nAccountID = pInfo->nAccount;

        FillFixedChargeData(pRechargeRewardInfo, pMsg->nAmount);

        m_pQueryMgr->AddQuery(QUERY_GetFirstPaymentRewardByAccountId, 0, pRechargeRewardInfo);

        WriteLog(LOGLEVEL_DEBUG, "OnAddMCoinByCmd nAccountID=%u nAmount=u", pAccountInfo->nAccount, pMsg->nAmount);
    }
}

void CLoginCirculator::OnGMAccountLock(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2A_LockOrUnLockAccountByGm *pMsg = (GameMsg_S2A_LockOrUnLockAccountByGm*)&msg;

    int nMaxLockTimeLevel = sizeof(s_nBlockTime) / sizeof(s_nBlockTime[0]);
    if (pMsg->m_nLockTimeType < 0 || pMsg->m_nLockTimeType >= nMaxLockTimeLevel)
    {
        return;
    }

    LockAccountInfo* pInfo = new LockAccountInfo();
    pInfo->nBlockType = (pMsg->m_bLock ? pMsg->m_nLockTimeType : -1);
    pInfo->nPID = pMsg->nPid;
    pInfo->strPUID = pMsg->strPUID;
    pInfo->strTransID = pMsg->strTransID;
    m_pQueryMgr->AddQuery(QUERY_UpdateAccountLockState, 0, pInfo);//更新账号状态
}

void CLoginCirculator::OnGMAddMCoin(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2A_AddMCoinByGm *pMsg = (GameMsg_S2A_AddMCoinByGm  *)&msg;

    if ((pMsg->nAmount > 0 || pMsg->nBindAmount > 0) && !pMsg->strPUID.empty())
    {
        ChongZhiByPUIDInfo *pInfo = new ChongZhiByPUIDInfo();
        pInfo->czInfo.nPID = pMsg->nPid;
        pInfo->strPUID = pMsg->strPUID;
        pInfo->czInfo.nAmount = pMsg->nAmount;
        pInfo->czInfo.nAllAmount = pMsg->nAmount;
        pInfo->czInfo.nRechargeBindMCoinReward = pMsg->nBindAmount;// 将绑定M币作为奖励发放
        pInfo->czInfo.nCurrency = pMsg->m_nCurrency;
        pInfo->czInfo.bIsRetryOrder = true;    //该标志用于通知GameServer发送通知邮件
        Formatter(pInfo->czInfo.strOrderID) << "addbygm" << (unsigned int)time(NULL)
            << "_" + pInfo->czInfo.nPID << "_" << pInfo->strPUID;

        pInfo->czInfo.nVersionType = Version_Free;//默认免费版
        //在线列表中查找玩家，获得客户端版本
        for (OnlineAccountMap::iterator it = m_mapAccountOnline.begin(); it != m_mapAccountOnline.end(); ++it)
        {
            if (strcmp(it->second->szPUID, pMsg->strPUID.c_str()) == 0)
            {
                pInfo->czInfo.nAccount = it->second->nAccount;
                break;
            }
        }

        RechargeRewardInfo* pRechargeRewardInfo = new RechargeRewardInfo();
        pRechargeRewardInfo->pChongZhiByPUIDInfo = pInfo;

        FillFixedChargeData(pRechargeRewardInfo, pMsg->nAmount);

        m_pQueryMgr->AddQuery(QUERY_GetFirstPaymentRewardByAccountId, 0, pRechargeRewardInfo);
    }
}

void CLoginCirculator::OnDBQuickCreateAccount(QueryBase & pQuery)
{
    CAutoAccount *pAutoAccount = (CAutoAccount *)(pQuery.m_pAttachment);
    AccountInfo *pInfo = (AccountInfo *)(pQuery.m_pTag);

    if (pInfo == NULL || pAutoAccount == NULL)
        return;

    GameMsg_Account_QuickCreateAccountResult MsgReturn;
    MsgReturn.nAccount = pInfo->nAccount;
    MsgReturn.SessionID = pInfo->SessionID;

    unsigned short nSlot = pInfo->nSlot;

    m_listAutoAccount.pop_front();
    //同一个ip在有效连接时间内发送过来的多次同样的快速注册申请的sessionID肯定相同,指针也一样，必定在此队列中。这里的判断可以修改或者删除
    if (IsInLoginProgress(pInfo) && pQuery.m_nResult == 0)
    {
        if (pAutoAccount != NULL && pInfo != NULL)
        {
            if (pInfo->nAccount > 0 && pQuery.m_nResult == Query_Res_OK)
            {
                MsgReturn.nResult = QuickCreateResult_SUCCESS;
                MsgReturn.m_strResultMsg = "Account_Quick_Create_Suc";

                memcpy(MsgReturn.szName, pAutoAccount->m_szName, MAX_NAME_LEN - 1);
                memcpy(MsgReturn.szPass, pAutoAccount->m_szPasswd, MAX_PASS_LEN - 1);
                m_pSocketMgr->SendGameMsg(&MsgReturn, nSlot);//主要发往客户端

                _ProcessLogin(pInfo);
                delete pAutoAccount, pAutoAccount = NULL;
                //这里一定要return，后面的逻辑不需要处理了
                return;
            }
        }
    }
    else
    {
        MsgReturn.nResult = QuickCreateResult_FAIL;
        MsgReturn.m_strResultMsg = "Account_Quick_Create_Fail";
        m_pSocketMgr->SendGameMsg(&MsgReturn, nSlot);//主要发往客户端
        RemoveLoginAccount(nSlot, pInfo->SessionID);
    }
    delete pInfo, pInfo = NULL;
}

void CLoginCirculator::OnDBCreateNewAutoAccount(QueryBase &query)
{
    AccountInfo *pInfo = (AccountInfo *)(query.m_pTag);

    if (pInfo == NULL)
        return;

    unsigned short nSlot = pInfo->nSlot;

    GameMsg_Account_QuickCreateAccountResult msgReturn;
    msgReturn.nAccount = pInfo->nAccount;
    msgReturn.SessionID = pInfo->SessionID;

    if (query.m_nResult == 0 && m_listAutoAccount.size() > 0 && IsInLoginProgress(pInfo))
    {
        CAutoAccount *pAutoAccount = m_listAutoAccount.front();
        m_listAutoAccount.pop_front();
        msgReturn.nAccount = pAutoAccount->m_nAccountID;

        msgReturn.nResult = QuickCreateResult_SUCCESS;
        msgReturn.m_strResultMsg = "Account_Quick_Create_Suc";
        memcpy(msgReturn.szName, pAutoAccount->m_szName, MAX_NAME_LEN - 1);
        memcpy(msgReturn.szPass, pAutoAccount->m_szPasswd, MAX_PASS_LEN - 1);
        m_pSocketMgr->SendGameMsg(&msgReturn, nSlot);//主要发往客户端

        unsigned char szPassWordMD5[MAX_PASS_LEN] = { 0 };
        MDString(pAutoAccount->m_szPasswd, szPassWordMD5);
        string strPassword = BinaryData2String(szPassWordMD5, MAX_MD5_LEN - 1);
        strcpy((char *)szPassWordMD5, strPassword.c_str());

        memcpy(pInfo->szName, pAutoAccount->m_szName, MAX_NAME_LEN - 1);
        memcpy(pInfo->szPass, szPassWordMD5, MAX_PASS_LEN - 1);
        pInfo->nAccount = pAutoAccount->m_nAccountID;
        _ProcessLogin(pInfo);
        m_pQueryMgr->AddQuery(QUERY_ACCOUNT_UpdateAutoAccountState, pAutoAccount->m_nAccountID);//更新DB该账号已经被使用，不返回
        delete pAutoAccount, pAutoAccount = NULL;
        //这里一定要return，后面的逻辑不需要处理了
        return;
    }
    else
    {
        msgReturn.nResult = QuickCreateResult_FAIL;
        msgReturn.m_strResultMsg = "Account_Quick_Create_Fail";
        m_pSocketMgr->SendGameMsg(&msgReturn, nSlot);//主要发往客户端
        RemoveLoginAccount(nSlot, pInfo->SessionID);
    }
    delete pInfo, pInfo = NULL;
}


void CLoginCirculator::OnDBAccountLogin(QueryBase & pQuery)
{
    LPACCOUNTINFO pInfo = (LPACCOUNTINFO)pQuery.m_pAttachment;
    char *pstrPasswd = (char *)pQuery.m_pTag;
    int nLine = (int)pQuery.m_nTag;

    //同一个ip在有效连接时间内发送过来的多次同样的快速注册申请的sessionID肯定相同,指针也一样，必定在此队列中。这里的判断可以修改或者删除。
    if (pInfo == NULL || !IsInLoginProgress(pInfo))
    {
        delete pInfo, pInfo = NULL;
        delete []pstrPasswd, pstrPasswd = NULL;

        return;
    }
    else if (pstrPasswd == NULL || pQuery.m_nResult != Query_Res_OK)
    {
        RemoveLoginAccount(pInfo->nSlot, pInfo->SessionID);

        delete pInfo, pInfo = NULL;
        delete []pstrPasswd, pstrPasswd = NULL;

        return;
    }

    bool bSuccess = (pInfo->nAccount > 0);
    if (bSuccess && pInfo->nPID == PID_INTERNAL)
    {
        bSuccess = (memcmp(pInfo->szPass, pstrPasswd, MAX_PASS_LEN - 1) == 0);
    }

    delete []pstrPasswd, pstrPasswd = NULL;

    if (bSuccess)
    {
        _ProcessLogin(pInfo, nLine);
    }
    else
    {
        GameMsg_Account_LoginResult msgResult;
        msgResult.m_nGateSessionID = pInfo->SessionID;
        if (pQuery.m_nAttachment != 0)
        { // 平台禁止登陆
            msgResult.m_nLoginResult = PLATFORM_FORBIDDEN_REG;
            msgResult.m_strErrorMsg = "Account_Forbidden_Register";
        }
        else if (pInfo->nAccount == 0)
        {
            msgResult.m_nLoginResult = NAME_NOT_EXITST;
            msgResult.m_strErrorMsg = "Account_Name_Not_Exist";
            WriteLog(LOGLEVEL_ERROR, "玩家%s输入的账号不存在！", pInfo->szName);
        }
        else //验证失败
        {
            msgResult.m_nLoginResult = INVALID_PASSWORD;
            msgResult.m_strErrorMsg = "Account_Invalid_Password";
            WriteLog(LOGLEVEL_ERROR, "玩家%s输入密码有误！", pInfo->szName);
        }

        m_pSocketMgr->SendGameMsg(&msgResult, pInfo->nSlot);//票据发往GateServer

        WriteLog(LOGLEVEL_ERROR, "玩家%s登陆验证失败！", pInfo->szName);

        RemoveLoginAccount(pInfo->nSlot, pInfo->SessionID);

        delete pInfo, pInfo = NULL;
    }
}

void CLoginCirculator::OnDBCreateAccount(QueryBase & pQuery)
{
    LPACCOUNTINFO pInfo = (LPACCOUNTINFO)pQuery.m_pAttachment;

    if (pInfo == NULL || !IsInLoginProgress(pInfo))
    {
        delete pInfo, pInfo = NULL;

        return;
    }

    GameMsg_Account_CreateAccountResult msgResult;
    msgResult.m_nGateSessionID = pInfo->SessionID;
    msgResult.nAccount = pInfo->nAccount;

    if (pInfo->nAccount > 0 && pQuery.m_nResult == Query_Res_OK)
    {
        msgResult.m_nResult = CREATE_SUCCESS;
        msgResult.m_strResultMsg = "Account_Create_Suc";
        m_pSocketMgr->SendGameMsg(&msgResult, pInfo->nSlot);

        _ProcessLogin(pInfo);
    }
    else
    {//不成功
        msgResult.m_nResult = DUPLICATED_ACCOUNT_NAME;
        msgResult.m_strResultMsg = "Account_Duplicated_Name";
        m_pSocketMgr->SendGameMsg(&msgResult, pInfo->nSlot);

        RemoveLoginAccount(pInfo->nSlot, pInfo->SessionID);

        delete pInfo, pInfo = NULL;
    }
}

void CLoginCirculator::OnChargeCallBack(QueryBase & query)
{
    ChongZhiInfo * pInfo = (ChongZhiInfo *)query.m_pAttachment;
    if (pInfo != NULL)
    {
        if (query.m_nResult == 0)
        {
            _OnChargeOK(pInfo);
            WriteLog(LOGLEVEL_DEBUG, "[CHARGE] charge ok: sql execute successfully");
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "[CHARGE] charge failed: sql execute failed: %d", query.m_nResult);
        }
        delete pInfo, pInfo = NULL;
    }
}

void CLoginCirculator::OnDBActivateCode(QueryBase & query)
{
    AccountInfo *pInfo = (AccountInfo *)query.m_pAttachment;
    int nLine = (int)query.m_nTag;

    list<__int64>::iterator itr = find(m_listActivateQueue.begin(), m_listActivateQueue.end(), pInfo->SessionID);
    if (itr != m_listActivateQueue.end())
    {
        m_listActivateQueue.erase(itr);
    }

    if (query.m_nResult == Query_Res_OK && pInfo != NULL)
    {
        _ProcessLogin(pInfo, nLine);
    }
    else
    {
        delete pInfo;
        pInfo = NULL;
    }
}

void CLoginCirculator::OnDBLockAccountCallBack(QueryBase & query)
{
    LockAccountInfo* pInfo = (LockAccountInfo*)query.m_pAttachment;
    if (pInfo != NULL)
    {
        if (query.m_nResult == 0)
        {
            AccountInfo *pAccountInfo = GetOnlineAccount(pInfo->nAccount);
            //如果锁定账号且玩家在线，先踢出玩家
            if (pInfo->nBlockType >= 0 && pAccountInfo != NULL)
            {
                WriteLog(LOGLEVEL_DEBUG, "server will drop client of player accountid=%u, because of locking account", pInfo->nAccount);

                pAccountInfo->nBlockState |= ((pInfo->nBlockType << 2) + 0x2);//设置锁定状态
                pAccountInfo->nBlockState &= 0xFFFFFFFE;//置成下线
                pAccountInfo->nBlockTime = pInfo->nBlockTime;//设置锁定时间

                GameMsg_Server_DropClient dropMsg;
                dropMsg.nAccount = pAccountInfo->nAccount;
                BroadcastMsg2GameServer(&dropMsg);

                m_mapAccountOnline.erase(pInfo->nAccount);
                delete pAccountInfo;
                pAccountInfo = NULL;
                m_pQueryMgr->AddQuery(QUERY_ResetAccountState, pInfo->nAccount);
            }
            if (pInfo->nBlockType >= 0)
            {
                WriteLog(LOGLEVEL_DEBUG, "player accountid=%u is locked (level %u, %u seconds) by gm %s",
                    pInfo->nAccount, pInfo->nBlockType, s_nBlockTime[pInfo->nBlockType], pInfo->strTransID.c_str());
            }
            else
            {
                WriteLog(LOGLEVEL_DEBUG, "player accountid=%u is unlocked by gm %s", pInfo->nAccount, pInfo->strTransID.c_str());
            }
        }
        delete pInfo;
        pInfo = NULL;
    }
}

void CLoginCirculator::KickPlayOff(AccountInfo *pAccountInfo, unsigned short reason)
{
    ENSURE_WITH_LOG_CMD(NULL != pAccountInfo, return, "Account info should not null...");

    pAccountInfo->nBlockState &= 0xFFFFFFFE;//置成下线

    GameMsg_Server_DropClient msgDrop;
    msgDrop.m_nDropReason = reason;
    msgDrop.nAccount = pAccountInfo->nAccount;
    BroadcastMsg2GameServer(&msgDrop);

    m_mapAccountOnline.erase(pAccountInfo->nAccount);
    m_pQueryMgr->AddQuery(QUERY_ResetAccountState, pAccountInfo->nAccount);

    SAFE_DELETE(pAccountInfo);
}

void CLoginCirculator::OnDBGetGetRegistedRoleCount(QueryBase& pQuery)
{
    WriteLog(LOGLEVEL_DEBUG, "Registed role count = %d\n", pQuery.m_nAttachment);
}

void CLoginCirculator::SendMsgToPlayerLine(LPGameMsg_Base pMsg, unsigned int nAccount)
{
    //通知玩家所在游戏线
    OnlineAccountMap::const_iterator it = m_mapAccountOnline.find(nAccount);
    if (it != m_mapAccountOnline.end() && m_pGameSrvSocketMgr != NULL)
    {
        unsigned short nPlayerSlot = it->second->nSlot;

        if (nPlayerSlot < m_pGameSrvSocketMgr->GetMaxClientCount())
        {
            m_pGameSrvSocketMgr->SendGameMsg(pMsg, nPlayerSlot);
        }
    }
}

bool CLoginCirculator::SendMsgToAnyLine(LPGameMsg_Base pMsg)
{
    do
    {
        BREAK_IF(m_pGameSrvSocketMgr == NULL);

        int nLine = m_linkingInfos.GetAvailableLine();
        BREAK_IF(nLine == INVALID_LINE);

        unsigned short nSlot = m_linkingInfos.GetSlotByLine(nLine);
        return m_pGameSrvSocketMgr->SendGameMsg(pMsg, nSlot);
    } while (0);

    return false;
}

bool CLoginCirculator::IsSpecialUser(const char* szUsername)
{
    for (list<std::string>::iterator it = g_listSpecialUser.begin(); it != g_listSpecialUser.end(); it++)
    {
        if (memcmp(szUsername, (*it).c_str(), (*it).size()) == 0)
        {
            return true;
        }
    }

    return false;
}

void CLoginCirculator::SendMessageToGate(GameMsg_Base *pMsg, unsigned short nSlot)
{
    if (pMsg != NULL && m_pSocketMgr != NULL)
    {
        m_pSocketMgr->SendGameMsg(pMsg, nSlot);
    }
}

void CLoginCirculator::SendMessageToPlayer(GameMsg_Base *pMsg, const AccountInfo *pAccountInfo)
{
    if (pMsg != NULL && m_pSocketMgr != NULL && pAccountInfo != NULL)
    {
        pMsg->nAccount = pAccountInfo->nAccount;
        pMsg->SessionID = pAccountInfo->nAccountSessionID;
        m_pSocketMgr->SendGameMsg(pMsg, pAccountInfo->nSlot);
    }
}

void CLoginCirculator::BroadcastMsg2GameServer(GameMsg_Base *pMsg, int nExcludedLine)
{
    if (m_pGameSrvSocketMgr == NULL || pMsg == NULL)
        return;

    const GameServerLinkInfo *pInfo = NULL;
    const std::vector<GameServerLinkInfo *> &rLinkInfos = m_linkingInfos.GetAllLinkInfos();
    for (std::vector<GameServerLinkInfo *>::const_iterator itr = rLinkInfos.begin();
        itr != rLinkInfos.end(); ++itr)
    {
        pInfo = *itr;
        if (pInfo == NULL || pInfo->GetLine() == nExcludedLine)
            continue;

        m_pGameSrvSocketMgr->SendGameMsg(pMsg, pInfo->GetSlot());
    }
}

const GameServerLinkInfo* CLoginCirculator::GetGameServerLinkInfo(unsigned short nSlot) const
{
    return m_linkingInfos.GetLinkInfoBySlot(nSlot);
}

/*
计算充值奖励，涉及首充奖励、普通充值奖励、收费版充值奖励，其中当首充奖励与普通充值奖励并存时取首充奖励
*/
void CLoginCirculator::OnGetFirstPaymentRewardByAccountIdCallBack(QueryBase & refQuery)
{
    RechargeRewardInfo* pRechargeRewardInfo = (RechargeRewardInfo*)refQuery.m_pAttachment;

    int nNowTime = (int)time(NULL);

    ChongZhiInfo * pChongZhiInfo = NULL;
    if (pRechargeRewardInfo->pChongZhiInfo != NULL)
    {
        pChongZhiInfo = pRechargeRewardInfo->pChongZhiInfo;
    }
    else if (pRechargeRewardInfo->pChongZhiByPUIDInfo != NULL)
    {
        pChongZhiInfo = &pRechargeRewardInfo->pChongZhiByPUIDInfo->czInfo;
    }

    if (pChongZhiInfo != NULL)
    {
        pChongZhiInfo->nAccount = pRechargeRewardInfo->nAccountID;
        pChongZhiInfo->nRoleID = pRechargeRewardInfo->nRoleID;
        pChongZhiInfo->bOnline = (GetOnlineAccount(pChongZhiInfo->nAccount) != NULL);

        //收费版奖励计算
        if (pChongZhiInfo->nVersionType == Version_Charge && pChongZhiInfo->nFeedVersionMoneyRewardRatio > 0)
        {
            int nValue = (pChongZhiInfo->nAllAmount*pChongZhiInfo->nFeedVersionMoneyRewardRatio) / 100;
            if (nValue > 0)
            {
                pChongZhiInfo->nFeedVersionMoneyReward = nValue;
            }
        }

        pChongZhiInfo->eChangeBillCause = EChangeBillCause_Add_Charge;
        if (pChongZhiInfo->nAllAmount > 0 && PaymentInfoMgr::Instance().FirstChargeRewardIsOpen(pRechargeRewardInfo->m_eVersionType, nNowTime))
        {
            FirstPaymentInfo* pFirstPaymentInfoLast = PaymentInfoMgr::Instance().GetFirstPaymentInfo(pRechargeRewardInfo->m_eVersionType, (int)pRechargeRewardInfo->m_firstChargeEvent.m_nTime);
            FirstPaymentInfo* pFirstPaymentInfoNow = PaymentInfoMgr::Instance().GetFirstPaymentInfo(pRechargeRewardInfo->m_eVersionType, nNowTime);
            if (pFirstPaymentInfoLast != pFirstPaymentInfoNow)
            {
                pChongZhiInfo->eChargeRewardType = (pFirstPaymentInfoNow != NULL ? EChargeRewardType_FirstCharge : EChargeRewardType_NormalCharge);
                if (pChongZhiInfo->eChargeRewardType == EChargeRewardType_FirstCharge)
                {
                    pChongZhiInfo->pChargeReward = new ChargeRewardInfo;
                    pChongZhiInfo->pChargeReward->m_nChargeRewardType = pChongZhiInfo->eChargeRewardType;
                    pChongZhiInfo->pChargeReward->m_nVersionType = pRechargeRewardInfo->m_eVersionType;
                    pChongZhiInfo->pChargeReward->m_nRoleID = pRechargeRewardInfo->nRoleID;
                    pChongZhiInfo->pChargeReward->m_strRoleName = pRechargeRewardInfo->strRoleName;
                }
            }
        }

        int nRewardBindMCoin = 0;
        int nRewardMoney = 0;

        // 首充奖励
        if (pChongZhiInfo->pChargeReward != NULL)
        {
            std::string strTitle = PaymentInfoMgr::Instance().MailTitle(pRechargeRewardInfo->m_eVersionType, nNowTime);
            std::string strContent = PaymentInfoMgr::Instance().MailContent(pRechargeRewardInfo->m_eVersionType, nNowTime);

            std::list<CItem> listItemReward;
            if (pRechargeRewardInfo->m_nSex == ESexType_Male)
            {
                listItemReward = PaymentInfoMgr::Instance().MaleRewards(pRechargeRewardInfo->m_eVersionType, nNowTime);
            }
            else if (pRechargeRewardInfo->m_nSex == ESexType_Female)
            {
                listItemReward = PaymentInfoMgr::Instance().FemaleRewards(pRechargeRewardInfo->m_eVersionType, nNowTime);
            }

            PaymentInfoMgr::Instance().GetFirstChargeReward(nRewardMoney, nRewardBindMCoin,
                pChongZhiInfo->nAllAmount, pRechargeRewardInfo->m_eVersionType, nNowTime);

            pChongZhiInfo->nRechargeMoneyReward += nRewardMoney;
            pChongZhiInfo->nRechargeBindMCoinReward += nRewardBindMCoin;

            pChongZhiInfo->pChargeReward->m_nRewardMoney = pChongZhiInfo->nRechargeMoneyReward;
            pChongZhiInfo->pChargeReward->m_nRewardMoney += pChongZhiInfo->nFeedVersionMoneyReward;
            pChongZhiInfo->pChargeReward->m_nRewardBindMCoin = pChongZhiInfo->nRechargeBindMCoinReward;
            pChongZhiInfo->pChargeReward->m_mailInfo.m_nType = EMailType_First_Recharge_Reward;
            pChongZhiInfo->pChargeReward->m_mailInfo.m_nSendTime = nNowTime;
            pChongZhiInfo->pChargeReward->m_mailInfo.m_nHasItem = (unsigned char)listItemReward.size();
            if (!pChongZhiInfo->bOnline)
            {
                pChongZhiInfo->pChargeReward->m_mailInfo.m_nMoney = pChongZhiInfo->pChargeReward->m_nRewardMoney;//金券放入邮件
            }
            for (std::list<CItem>::iterator it = listItemReward.begin(); it != listItemReward.end(); ++it)
            {
                MailItem mailItem;
                mailItem.m_nType = it->m_nItemType;
                mailItem.m_nCount = it->m_nItemCount;
                mailItem.m_nValidTime = it->m_nValidTime;
                pChongZhiInfo->pChargeReward->m_mailInfo.m_mapItems.insert(std::make_pair(mailItem.m_nType, mailItem));
            }

            std::string strFullContent;
            SafeFormat(strFullContent, strContent, (int)pChongZhiInfo->nAllAmount,
                pChongZhiInfo->pChargeReward->m_nRewardBindMCoin, pChongZhiInfo->pChargeReward->m_nRewardMoney);
            pChongZhiInfo->pChargeReward->m_mailInfo.SetContent(strFullContent);
            pChongZhiInfo->pChargeReward->m_mailInfo.SetTitle(strTitle);
            pChongZhiInfo->eChangeBindBillRewardCause = EChangeBindBillCause_Add_FirstChargeReward;
            pChongZhiInfo->eChangeMoneyRewardCause = EChangeMoneyCause_Add_FirstRecharge;
        }
        else //if( pChongZhiInfo->pChargeReward == NULL ) // 日常充值奖励
        {
            // 确定 日常充值奖励活动开放
            bool bRewardIsOpen = PaymentInfoMgr::Instance().RechargeRewardIsOpen((Version_Type)pRechargeRewardInfo->m_eVersionType, nNowTime);
            if (bRewardIsOpen || pChongZhiInfo->nRechargeBindMCoinReward > 0)
            {
                pChongZhiInfo->eChargeRewardType = EChargeRewardType_NormalCharge;
                pChongZhiInfo->pChargeReward = new ChargeRewardInfo;
                pChongZhiInfo->pChargeReward->m_nChargeRewardType = pChongZhiInfo->eChargeRewardType;
                pChongZhiInfo->pChargeReward->m_nVersionType = pRechargeRewardInfo->m_eVersionType;
                pChongZhiInfo->pChargeReward->m_nRoleID = pRechargeRewardInfo->nRoleID;
                pChongZhiInfo->pChargeReward->m_strRoleName = pRechargeRewardInfo->strRoleName;
                pChongZhiInfo->pChargeReward->m_mailInfo.m_nType = EMailType_Recharge_Reward;
                pChongZhiInfo->pChargeReward->m_mailInfo.m_nSendTime = nNowTime;
                pChongZhiInfo->pChargeReward->m_mailInfo.m_nHasItem = 0;

                std::string strMailTitleFmt;
                std::string strMailContentFmt;
                if (bRewardIsOpen)
                {
                    PaymentInfoMgr::Instance().GetRechargeReward(nRewardMoney, nRewardBindMCoin,
                        strMailTitleFmt, strMailContentFmt, pChongZhiInfo->nAllAmount, pRechargeRewardInfo->m_eVersionType, nNowTime);

                    pChongZhiInfo->nRechargeMoneyReward += nRewardMoney;
                    pChongZhiInfo->nRechargeBindMCoinReward += nRewardBindMCoin;
                }

                pChongZhiInfo->pChargeReward->m_nRewardMoney = pChongZhiInfo->nRechargeMoneyReward;
                pChongZhiInfo->pChargeReward->m_nRewardMoney += pChongZhiInfo->nFeedVersionMoneyReward;
                pChongZhiInfo->pChargeReward->m_nRewardBindMCoin = pChongZhiInfo->nRechargeBindMCoinReward;
                if (pChongZhiInfo->nAllAmount > 0)
                {
                    pChongZhiInfo->eChangeBindBillRewardCause = EChangeBindBillCause_Add_ChargeReward;
                }
                else //通过充值获得的绑定M币，如果充值金额是0，只有GMClient一个来源
                {
                    pChongZhiInfo->eChangeBindBillRewardCause = EChangeBindBillCause_Add_GMCommand;
                }
                pChongZhiInfo->eChangeMoneyRewardCause = EChangeMoneyCause_Add_RechargeReward;
                if (!pChongZhiInfo->bOnline) // 玩家不在线时，金券放入邮件
                {
                    pChongZhiInfo->pChargeReward->m_mailInfo.m_nMoney = pChongZhiInfo->pChargeReward->m_nRewardMoney;
                }

                if (bRewardIsOpen)
                {
                    std::string strFullContent;
                    SafeFormat(strFullContent, strMailContentFmt, (int)pChongZhiInfo->nAllAmount,
                        pChongZhiInfo->pChargeReward->m_nRewardBindMCoin, pChongZhiInfo->pChargeReward->m_nRewardMoney);
                    pChongZhiInfo->pChargeReward->m_mailInfo.SetContent(strFullContent);
                    pChongZhiInfo->pChargeReward->m_mailInfo.SetTitle(strMailTitleFmt);
                    pChongZhiInfo->pChargeReward->m_mailInfo.m_nType = EMailType_Recharge_Reward;
                    pChongZhiInfo->pChargeReward->m_mailInfo.m_nSendTime = nNowTime;
                    pChongZhiInfo->pChargeReward->m_mailInfo.m_nHasItem = 0;
                }
            }


            do
            {
                if (0 == pRechargeRewardInfo->m_fixedChargeRecord.m_nMCoin)
                    break;
                unsigned int tmStart = 0;
                unsigned int tmEnd = 0;
                if (!PaymentInfoMgr::Instance().FixedChargeIsRun(time(NULL), &tmStart, &tmEnd))
                    break;

                if (pRechargeRewardInfo->m_fixedChargeRecord.m_nChargeTime > tmStart
                    && pRechargeRewardInfo->m_fixedChargeRecord.m_nChargeTime < tmEnd)
                    break; // 已经完成了这个时间段的充值
                // 需要赠送的绑定M币金额
                pChongZhiInfo->m_nFixedChargeBindPoint = PaymentInfoMgr::Instance().FixedChargeGiftCoin(pChongZhiInfo->nAmount);
            } while (0);
        }

        //充值(原始值),回调中会消息通知 Game Server
        m_pQueryMgr->AddQuery(QUERY_Bill_ChongZhi, 0, pChongZhiInfo);
    }

    delete pRechargeRewardInfo;
    pRechargeRewardInfo = NULL;
}

void CLoginCirculator::OnGMAddPrivMCoin(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_M2G_SendPrivMCoin& rMsg = (GameMsg_M2G_SendPrivMCoin&)msg;

    vector<PlayerMoneyInfo>* pPlayerMoneyList = new vector<PlayerMoneyInfo>();
    vector<GMRechargeResult>* pPlayerResultList = new vector<GMRechargeResult>();
    for (size_t i = 0; i < rMsg.m_PlayerMoneyInfo.size(); i++)
    {
        pPlayerMoneyList->push_back(rMsg.m_PlayerMoneyInfo[i]);
        GMRechargeResult gr;
        gr.m_PlayerPUID = rMsg.m_PlayerMoneyInfo[i].m_PlayerPUID;
        gr.m_Result = false;
        pPlayerResultList->push_back(gr);
    }
    m_pQueryMgr->AddQuery(QUERY_GMPrivRecharge, rMsg.m_nSlot, pPlayerMoneyList, 0, pPlayerResultList);
}

void CLoginCirculator::OnGMPrivRechargeCallBack(QueryBase& query)
{
    vector<PlayerMoneyInfo>& pPlayerMoneyList = *(vector<PlayerMoneyInfo>*)query.m_pAttachment;
    vector<GMRechargeResult>& pPlayerResultList = *(vector<GMRechargeResult>*)query.m_pTag;

    ASSERT(pPlayerResultList.size() == pPlayerMoneyList.size());

    unsigned short nSlot = (unsigned short)query.m_nAttachment;

    GameMsg_A2M_SendPrivMCoinRet msg;
    msg.m_nSlot = nSlot;

    for (size_t i = 0; i < pPlayerResultList.size(); i++)
    {
        unsigned int nAccount = (unsigned int)atoi(pPlayerResultList[i].m_PlayerPUID.c_str());
        OnlineAccountMap::iterator itFind = m_mapAccountOnline.find(nAccount);
        if (pPlayerResultList[i].m_Result && itFind != m_mapAccountOnline.end())
        {
            AccountInfo* pAccountInfo = GetOnlineAccount(nAccount);
            if (pAccountInfo != NULL)
            {
                GameMsg_A2S_SendMCoinOnline msgOnline;
                msgOnline.m_nAccount = nAccount;
                msgOnline.m_nMCoin = pPlayerMoneyList[i].m_nMoney;
                msgOnline.m_nBindMCoin = pPlayerMoneyList[i].m_nBindMoney;
                m_pGameSrvSocketMgr->SendGameMsg(&msgOnline, pAccountInfo->nSlot);
            }
        }
        msg.m_GMRechargeResult.push_back(pPlayerResultList[i]);
    }

    SendMsgToAnyLine(&msg);

    delete (vector<PlayerMoneyInfo>*)query.m_pAttachment;
    query.m_pAttachment = NULL;
    delete (vector<GMRechargeResult>*)query.m_pTag;
    query.m_pTag = NULL;
}

void CLoginCirculator::OnGMGetServerLineLimit(GameMsg_Base& pMsg, CSlotPeer& SlotPeer)
{
    GameMsg_G2A_GetServerPlayerLimit& msg = (GameMsg_G2A_GetServerPlayerLimit&)pMsg;
    GameMsg_A2G_GetServerPlayerLimitRet retMsg;
    retMsg.m_nSolt = msg.m_nSolt;
    retMsg.m_PlayerLimit = m_nSoftLimitClientCount;

    SendMsgToAnyLine(&retMsg);
}

void CLoginCirculator::OnGMSetServerLineLimit(GameMsg_Base& pMsg, CSlotPeer& SlotPeer)
{
    GameMsg_M2A_SetPlayerLimit& msg = (GameMsg_M2A_SetPlayerLimit&)pMsg;
    m_nSoftLimitClientCount = msg.m_PlayerLimit;
}

void CLoginCirculator::OnGMSendBlackDeviceList(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_M2A_SendBlackDeviceList& rMsg = (GameMsg_M2A_SendBlackDeviceList&)msg;
    vector<string>* pDeviceList = new vector<string>();
    pDeviceList->insert(pDeviceList->begin(), rMsg.m_NameList.begin(), rMsg.m_NameList.end());
    if (rMsg.m_bFrozenDevice)
    {
        std::string* pStrGM = new std::string();
        *pStrGM = rMsg.m_GM;
        m_pQueryMgr->AddQuery(QUERY_SendDeviceList, rMsg.m_nSlot, pDeviceList, 0, pStrGM);
    }
    else
    {
        m_pQueryMgr->AddQuery(QUERY_DeleteDeviceList, rMsg.m_nSlot, pDeviceList, 0, NULL);
    }

}

void CLoginCirculator::OnSendBlackDeviceCallBack(QueryBase& query)
{
    if (query.m_nResult == Query_Res_OK)
    {
        CurrencyTable::Instance().Reload();
    }
    GameMsg_A2M_SendBlackDeviceListRet retMsg;
    retMsg.m_nSlot = query.m_nAttachment;
    retMsg.m_Result = (unsigned short)query.m_nResult;
    SendMsgToAnyLine(&retMsg);
}

void CLoginCirculator::OnDeleteBlackDeviceCallBack(QueryBase& query)
{
    if (query.m_nResult == Query_Res_OK)
    {
        CurrencyTable::Instance().Reload();
    }
    GameMsg_A2M_SendBlackDeviceListRet retMsg;
    retMsg.m_nSlot = query.m_nAttachment;
    retMsg.m_Result = (unsigned short)query.m_nResult;
    SendMsgToAnyLine(&retMsg);
}

