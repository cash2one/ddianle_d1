#include "../group/Global.h"
#include "../../socket/SocketDef.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/Log.h"
#include "../libServerFrame/QueryManager.h"
#include "../share/ServerMsgDef.h"
#include "../util/CommonFunc.h"
#include "ActivityManager.h"
#include <time.h>
#include "GameMsg_S2S_Activities.h"
#include "RedEnvelope/GameMsg_GS_RedEnvelope.h"
#include "RedEnvelope/RedEnvelope.h"
#include "RedEnvelope/RedEnvelopeConfigManager.h"
#include "../dancegroup/DanceGroupProcessor.h"
#include "../../datastructure/Localization.h"
#include "../../socket/Formatter.h"
#include "../chatGrp/WChat.h"

extern CQueryManager    * g_pQueryMgr;

#define MAX_LONGACTING_BOARDCAST_NUM 10

ActivityManager::ActivityManager()
    : GameMsg_Processor(true, true)
    , m_bRedEnvelopeComplete(false)
{
}

ActivityManager::~ActivityManager()
{
}

ActivityManager & ActivityManager::Instance()
{
    static ActivityManager s_instance;
    return s_instance;
}

void ActivityManager::Initialize()
{
    RegMessageMap();

    InitializeRedEnvelope();
}

void ActivityManager::RegMessageMap()
{
    // 长效累冲
    GAMEMSG_REGISTERCREATOR( GameMsg_S2G_GetLongactingRechargeBoardCast );
    GAMEMSG_REGISTERCREATOR( GameMsg_S2G_AddLongactingRechargeBoardCast );
    RegMsgProcessor( MSG_S2G_AddLongactingRechargeBroadCase, &ActivityManager::OnAddLongactingRechargeBoardCast );
    RegMsgProcessor( MSG_S2G_GetLongactingRechargeBroadCase, &ActivityManager::OnGetLongactingRechargeBoardCast );

    //  红包
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_CreateRedEnvelope);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetRedEnvelopeList);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetRedEnvelopeDetails);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_OpenRedEnvelope);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetCanOpenRedEnvelopeCount);
    RegMsgProcessor(MSG_S2G_GetRedEnvelopeList, &ActivityManager::OnGetRedEnvelopeList);
    RegMsgProcessor(MSG_S2G_GetRedEnvelopeDetails, &ActivityManager::OnGetRedEnvelopeDetails);
    RegMsgProcessor(MSG_S2G_CreateRedEnvelope, &ActivityManager::OnRequestCreateRedEnvelope);
    RegMsgProcessor(MSG_S2G_OpenRedEnvelope, &ActivityManager::OnOpenRedEnvelope);
    RegMsgProcessor(MSG_S2G_GetCanOpenRedEnvelopeCount, &ActivityManager::OnGetCanOpenRedEnvelopeCount);
    RegQueryProcessor(QUERY_RedEnvelope_LoadInfo, &ActivityManager::OnLoadDanceGroupRedEnvelope);
}

void ActivityManager::OnAddLongactingRechargeBoardCast(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_S2G_AddLongactingRechargeBoardCast & addmsg = (GameMsg_S2G_AddLongactingRechargeBoardCast&)rMsg;
    
    m_deqBoardCase.push_front(addmsg.m_BoardCast);
}

void ActivityManager::OnGetLongactingRechargeBoardCast(GameMsg_Base &rMsg, CSlotPeer &rSlot)
{
    GameMsg_S2G_GetLongactingRechargeBoardCast & s2gmsg = (GameMsg_S2G_GetLongactingRechargeBoardCast&)rMsg;
    
    CGroupRoleEntity * pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(s2gmsg.m_nRoleID);
    if (pRoleEntity != NULL)
    {
        GameMsg_G2S_GetLongactingRechargeBoardCast g2smsg;

        unsigned short nCount = ((unsigned short)m_deqBoardCase.size()) >= MAX_LONGACTING_BOARDCAST_NUM ? MAX_LONGACTING_BOARDCAST_NUM :
            ((unsigned short)m_deqBoardCase.size());
        for (unsigned short i = 0; i < nCount; ++i)
        {
            g2smsg.m_listLongactingBoardCast.push_back(m_deqBoardCase[i]);
        }

        pRoleEntity->SendPlayerMsg(&g2smsg);
    }
}

void ActivityManager::OnLoadDanceGroupRedEnvelope(QueryBase &rQuery)
{
    RedEnvelopeManager::Instance().Initialize((unsigned int)rQuery.m_nAttachment);
    m_bRedEnvelopeComplete = true;

    WriteLog(LOGLEVEL_DEBUG, "Load RedEnvelope Scuess......");
}

void ActivityManager::OnGetRedEnvelopeList(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetRedEnvelopeList & s2gmsg = (GameMsg_S2G_GetRedEnvelopeList&)msg;

    std::list<unsigned int> listRedEnvelope;
    RedEnvelopeManager::Instance().GetDanceGroupRedEnvelopeList(s2gmsg.m_nDanceGroupID, listRedEnvelope);

    GameMsg_G2S_GetRedEnvelopeList g2smsg;
    std::list<unsigned int>::iterator it = listRedEnvelope.begin();
    for (; it != listRedEnvelope.end(); ++it)
    {
        RedEnvelope * pRedEnvelope = RedEnvelopeManager::Instance().GetRedEnvelope(*it);
        if (pRedEnvelope != NULL)
        {
            BriefRedEnvelope brief;
            brief.m_nRedEnvelopeID = pRedEnvelope->m_nRedEnvelopeID;
            brief.m_strSendRoleName = pRedEnvelope->m_strSendRoleName;
            brief.m_bGetFinish = pRedEnvelope->IsAllOpen();
            brief.m_bHasOpen = pRedEnvelope->IsOpenByRoleID(s2gmsg.m_nRoleID);
            unsigned int nGetMoney = 0;
            pRedEnvelope->GetOpenValue(s2gmsg.m_nRoleID, nGetMoney, brief.m_nOpenTime);
            g2smsg.m_listRedEnvelope.push_back(brief);

//             if (pRedEnvelope->IsOpenByRoleID(s2gmsg.m_nRoleID))
//             {
//                 g2smsg.m_listGetRedEnvelopeID.push_back(pRedEnvelope->m_nRedEnvelopeID);
//             }
        }
    }
    SendPlayerMsgByRoleID(s2gmsg.m_nRoleID, g2smsg);
}

void ActivityManager::OnGetRedEnvelopeDetails(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetRedEnvelopeDetails & g2smsg = (GameMsg_S2G_GetRedEnvelopeDetails&)msg;

    RedEnvelope * pRedEnvelope = RedEnvelopeManager::Instance().GetRedEnvelope(g2smsg.m_nRedEnvelopeID);
    if (pRedEnvelope != NULL)
    {
        GameMsg_G2S_GetRedEnvelopeDetails detailsmsg;
        detailsmsg.m_RedEnvelope = *pRedEnvelope;
        SendPlayerMsgByRoleID(g2smsg.m_nRoleID, detailsmsg);
    }
}

void ActivityManager::OnOpenRedEnvelope(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_OpenRedEnvelope & s2gmsg = (GameMsg_S2G_OpenRedEnvelope&)msg;

    RedEnvelope * pRedEnvelope = RedEnvelopeManager::Instance().GetRedEnvelope(s2gmsg.m_nRedEnvelopeID);
    if (pRedEnvelope != NULL)
    {
        OpenRoleInfo openinfo;
        openinfo.m_nRoleID = s2gmsg.m_nRoleID;
        openinfo.m_strRoleName = s2gmsg.m_strRoleName;
        openinfo.m_nCurrencyValue = pRedEnvelope->m_nCurrencyValue;

        int nError = RedEnvelopeManager::Instance().OpenRedEnvelope(s2gmsg.m_nRedEnvelopeID, s2gmsg.m_nDanceGroupID, openinfo);
        GameMsg_G2S_OpenRedEnvelope openmsg;
        openmsg.m_nResultFlag = (unsigned int)nError;
        openmsg.m_nCurrencyType = pRedEnvelope->m_nCurrencyType;
        openmsg.m_nCurrencyValue = openinfo.m_nCurrencyValue;
        openmsg.m_nRedEnvelopeID = s2gmsg.m_nRedEnvelopeID;
        openmsg.m_nOpenTime = openinfo.m_nOpenTime;
        SendPlayerMsgByRoleID(s2gmsg.m_nRoleID, openmsg);

        if (nError == ERedEnvelopeError_Scuess)
        {
            InsertIntoLog(s2gmsg.m_nRoleID, CRedEnvelopeLog::ERedEnvelopeAction_Open, s2gmsg.m_nRedEnvelopeID, openinfo.m_nOpenIndex, openinfo.m_nCurrencyValue);
        }
    }
    else
    {
        GameMsg_G2S_OpenRedEnvelope openmsg;
        openmsg.m_nResultFlag = (unsigned int)ERedEnvelopeError_NotExist;
        SendPlayerMsgByRoleID(s2gmsg.m_nRoleID, openmsg);
    }
}

void ActivityManager::OnGetCanOpenRedEnvelopeCount(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetCanOpenRedEnvelopeCount & g2smsg = (GameMsg_S2G_GetCanOpenRedEnvelopeCount&)msg;

    unsigned int nCanOpenCount = RedEnvelopeManager::Instance().GetCanOpenRedEnvelopeCount(g2smsg.m_nRoleID, g2smsg.m_nDanceGroupID);
    GameMsg_G2S_GetCanOpenRedEnvelopeCount resultmsg;
    resultmsg.m_nCanOpenCount = nCanOpenCount;
    SendPlayerMsgByRoleID(g2smsg.m_nRoleID, resultmsg);
}

void ActivityManager::InsertIntoLog(unsigned int nRoleID, CRedEnvelopeLog::ERedEnvelopeAction eAction,
    unsigned int nRedEnvelopeID, int nOpenIndex, int nMoney)
{
    CRedEnvelopeLog * pLog = new CRedEnvelopeLog();
    pLog->m_eAction = eAction;
    pLog->m_nRoleID = nRoleID;
    pLog->m_nRedEnvelopeID = nRedEnvelopeID;
    pLog->m_nOpenIndex = nOpenIndex;
    pLog->m_nRedEnvelopeMoney = nMoney;
    g_pQueryMgr->AddQuery(QUERY_LogRoleRedEnvelopeLog, 0, pLog);
}

void ActivityManager::InitializeRedEnvelope()
{
    g_pQueryMgr->AddQuery(QUERY_RedEnvelope_LoadInfo, 0, RedEnvelopeManager::Instance().GetAllRedEnvelope());
}

void ActivityManager::OnRequestCreateRedEnvelope(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (!m_bRedEnvelopeComplete)
    {
        WriteLog(LOGLEVEL_ERROR, "Load RedEnvelope not Scuess......");
        return;
    }

    // 舞团是否存在
    GameMsg_S2G_CreateRedEnvelope & s2gmsg = (GameMsg_S2G_CreateRedEnvelope&)msg;
    const CDanceGroupInfo* pDanceGroupInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo(s2gmsg.m_nDanceGroupID);
    if (pDanceGroupInfo == NULL)
    {
        return;
    }

    // 充值开始日志
    InsertIntoLog(s2gmsg.m_nRoleID, CRedEnvelopeLog::ERedEnvelopeAction_Recharge, 0, 0, s2gmsg.m_nMCoin);

    // 生成红包
    unsigned int nRedEnvelopeID = RedEnvelopeManager::Instance().CreateRedEnvelope(s2gmsg.m_nRoleID, s2gmsg.m_strRoleName, s2gmsg.m_nMCoin, 
        s2gmsg.m_nDanceGroupID, s2gmsg.m_bIsAnonymity);

    // 成功
    if (nRedEnvelopeID > 0)
    {
        RedEnvelope * pRedEnvelope = RedEnvelopeManager::Instance().GetRedEnvelope(nRedEnvelopeID);
        if(pRedEnvelope == NULL)
        {
            return;
        }

        // 广播红包消息
        GameMsg_G2S_NewRedEnvelope newredenvelopemsg;
        newredenvelopemsg.m_nSendRoleID = s2gmsg.m_nRoleID;
        newredenvelopemsg.m_strRoleName = s2gmsg.m_bIsAnonymity ? "" : s2gmsg.m_strRoleName;
        newredenvelopemsg.m_nMCoin = s2gmsg.m_nMCoin;
        CDanceGroupProcessor::Instance().SendMsg2DanceGroupMember(s2gmsg.m_nDanceGroupID, 0 , newredenvelopemsg);

		// 广播聊天消息
		std::string strContent;
		SafeFormat(strContent, CLocalization::Instance().GetString("DanceGroup_NewRedEnvelope"),
			pRedEnvelope->m_strSendRoleName.c_str());
		CWChatMgr::Instance().SendDanceGroupAnnouncement(s2gmsg.m_nDanceGroupID, strContent);

        // 创建成功日志
        InsertIntoLog(s2gmsg.m_nRoleID, CRedEnvelopeLog::ERedEnvelopeAction_CreateRedEnvelope, nRedEnvelopeID, 0, 0);
    }
}

void ActivityManager::SendPlayerMsgByRoleID(unsigned int nRoleID, GameMsg_Base &msg)
{
    if (0 != nRoleID)
    {
        CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);

        if (NULL != pRoleEntity)
            pRoleEntity->SendPlayerMsg(&msg);
    }
}
