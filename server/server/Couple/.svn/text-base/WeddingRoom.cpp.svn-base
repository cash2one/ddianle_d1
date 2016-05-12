#include "WeddingRoom.h"
#include "EntityCoupleComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "CoupleDataStructrue.h"
#include "WeddingRoomManager.h"
#include "GameMsg_S2C_Couple.h"
#include "GameMsg_S2G_Couple.h"
#include "../chat/ChatManager.h"
#include "../../datastructure/Localization.h"
#include "../../socket/Formatter.h"
#include "../mail/MailMgr.h"
#include "../logic/LogicCirculator.h"

extern int g_nLine;
extern CLogicCirculator * g_pCirculator;

void SendMsg2GroupServer(LPGameMsg_Base pMsg);

CWeddingRoom::CWeddingRoom()
: m_nRoomID(0)
, m_nPairID(0)
, m_nHostID(0)
, m_nBlessingValue(0)
, m_nGroomID(0)
, m_nBrideID(0)
, m_nCreateTime(0)
, m_bValid(false)
{

}

CWeddingRoom::~CWeddingRoom()
{

}

EWeddingRoomType CWeddingRoom::RoomType()
{
    return m_eWeddingType;
}

unsigned int CWeddingRoom::RoomID()
{
    return m_nRoomID;
}

unsigned int CWeddingRoom::PairID()
{
    return m_nPairID;
}

unsigned int CWeddingRoom::HostID()
{
    return m_nHostID;
}

time_t CWeddingRoom::ExpiredTime()
{
    return	m_nCreateTime + C_MAX_WEDDING_ROOM_EXISTS_TIME;
}

time_t CWeddingRoom::StartingNotificatioiTime()
{
    int nNoficationTime = C_MAX_WEDDING_ROOM_EXISTS_TIME - C_MAX_WEDDING_ROOM_NOTIFY_BEFORE_EXPIRED;

    if (nNoficationTime > 0) {
        return m_nCreateTime + nNoficationTime;
    }

    return m_nCreateTime;
}

const std::string & CWeddingRoom::RoomName()
{
    return m_strRoomName;
}


bool CWeddingRoom::HasPassword() const
{
    return !m_strPassword.empty();
}

void CWeddingRoom::SetPassword(const std::string & strPassword)
{
    m_strPassword = strPassword;
}

const std::string & CWeddingRoom::Password()
{
    return m_strPassword;
}

unsigned int CWeddingRoom::GroomID()
{
    return m_nGroomID;
}

const std::string & CWeddingRoom::GroomName()
{
    return m_strRoomName;
}

unsigned int CWeddingRoom::BrideID()
{
    return m_nBrideID;
}

const std::string & CWeddingRoom::BrideName()
{
    return m_strBrideName;
}

time_t CWeddingRoom::CreateTime()
{
    return m_nCreateTime;
}

const WeddingRoomPlayerMap_t & CWeddingRoom::WeddingRoomPlayerMap()
{
    return m_mapPlayers;
}

int CWeddingRoom::AddWeddingPlayer(CEntityCoupleComponent * pRoleCouple)
{
    int nRes = -1;

    if (pRoleCouple != NULL) {
        int nCount = 0;
        for (WeddingRoomPlayerMap_t::iterator itr = m_mapPlayers.begin(); itr != m_mapPlayers.end(); ++itr)
        {
            if (itr->second->RoomRoleType() != EWeddingRoomRoleType_Groom && itr->second->RoomRoleType() != EWeddingRoomRoleType_Bride) {
                ++nCount;
            }
        }

        if (nCount < C_MAX_WEDDING_ROOM_PLAYERS - 2) {	// not include groom and bride
            m_mapPlayers.insert(make_pair(pRoleCouple->RoleAttr()->GetRoleID(), pRoleCouple));
            nRes = 0;
        }
        else if ((int)m_mapPlayers.size() < C_MAX_WEDDING_ROOM_PLAYERS && (pRoleCouple->RoleID() == m_nGroomID || pRoleCouple->RoleID() == m_nBrideID))
        {
            m_mapPlayers.insert(make_pair(pRoleCouple->RoleAttr()->GetRoleID(), pRoleCouple));

            nRes = 0;
        }
    }


    return nRes;
}

CEntityCoupleComponent * CWeddingRoom::RemoveWeddingPlayer(unsigned int nRoleID)
{
    CEntityCoupleComponent *pPlayer = NULL;
    WeddingRoomPlayerMap_t::iterator itr = m_mapPlayers.find(nRoleID);
    if (itr != m_mapPlayers.end()) {
        pPlayer = itr->second;
        m_mapPlayers.erase(itr);
    }

    return pPlayer;
}

CEntityCoupleComponent * CWeddingRoom::RemoveWeddingPlayer(CEntityCoupleComponent * pRoleCouple)
{
    CEntityCoupleComponent *pPlayer = NULL;
    if (pRoleCouple != NULL && pRoleCouple->RoleAttr() != NULL) {
        WeddingRoomPlayerMap_t::iterator itr = m_mapPlayers.find(pRoleCouple->RoleAttr()->GetRoleID());
        if (itr != m_mapPlayers.end()) {
            pPlayer = itr->second;
            m_mapPlayers.erase(itr);
        }
    }

    return pPlayer;
}

CEntityCoupleComponent * CWeddingRoom::GetWeddingPlayer(unsigned int nRoleID)
{
    WeddingRoomPlayerMap_t::iterator itr = m_mapPlayers.find(nRoleID);
    if (itr != m_mapPlayers.end() && itr->second != NULL) {
        return itr->second;
    }

    return NULL;
}

CEntityCoupleComponent * CWeddingRoom::GetWeddingPlayer(EWeddingRoomRoleType eRoomRoleType)
{
    if (eRoomRoleType > EWeddingRoomRoleType_None && eRoomRoleType < EWeddingRoomRoleType_Max) {
        for (WeddingRoomPlayerMap_t::iterator itr = m_mapPlayers.begin(); itr != m_mapPlayers.end(); ++itr) {
            if (itr->second != NULL && itr->second->m_eRoomRoleType == eRoomRoleType) {
                return itr->second;
            }
        }
    }

    return NULL;
}

void CWeddingRoom::SetValid(bool bValid)
{
    m_bValid = bValid;
}

bool CWeddingRoom::Valid()
{
    return m_bValid;
}

void CWeddingRoom::OnRoomStartingNofiication()
{
    WeddingRoomPlayerMap_t::iterator itr = m_mapPlayers.find(m_nGroomID);

    GameMsg_S2G_WeddingRoomStartingNofication msgNoficationToGroup;
    msgNoficationToGroup.m_nGroomID = m_nGroomID;
    msgNoficationToGroup.m_nBrideID = m_nBrideID;
    msgNoficationToGroup.m_nLine = g_nLine;
    msgNoficationToGroup.m_strLineName = g_pCirculator->GetServerName();
    msgNoficationToGroup.m_nRoomID = m_nRoomID;
    msgNoficationToGroup.m_nStartTime = (unsigned int)ExpiredTime();
    ::SendMsg2GroupServer(&msgNoficationToGroup);
}

void CWeddingRoom::OnRoomEndNofiication()
{
    GameMsg_S2G_WeddingRoomStartingNofication msgNoficationToGroup;
    msgNoficationToGroup.m_nGroomID = m_nGroomID;
    msgNoficationToGroup.m_nBrideID = m_nBrideID;
    msgNoficationToGroup.m_nLine = g_nLine;
    msgNoficationToGroup.m_strLineName = g_pCirculator->GetServerName();
    msgNoficationToGroup.m_nRoomID = m_nRoomID;
    msgNoficationToGroup.m_nStartTime = 0;
    ::SendMsg2GroupServer(&msgNoficationToGroup);
}

void CWeddingRoom::OnRoomAutoStart()
{
    WeddingRoomPlayerMap_t::iterator itr = m_mapPlayers.find(m_nHostID);

    int nRes = EAutoStartWeddingRoom_Suc;
    std::string strError;

    if (itr == m_mapPlayers.end() || itr->second == NULL) {
        nRes = EAutoStartWeddingRoom_HostNotInRoom;
        strError = "EAutoStartWeddingRoom_HostNotInRoom";
    }
    else {
        itr->second->AutoStartWedding(nRes, strError);
    }

    if (nRes != EAutoStartWeddingRoom_Suc) {//跨线通知不能完成婚礼
        GameMsg_S2G_AutoStartWeddingFail msgFail;
        msgFail.m_nGroomID = GetWeddingPlayer(m_nGroomID) == NULL ? m_nGroomID : 0;
        msgFail.m_nBrideID = GetWeddingPlayer(m_nBrideID) == NULL ? m_nBrideID : 0;
        msgFail.m_nError = (unsigned char)nRes;
        msgFail.m_strError = strError;
        SendMsg2GroupServer(&msgFail);

        GameMsg_S2C_AutoStartWeddingRoomFail msgNofication;
        msgNofication.m_nError = (unsigned char)nRes;
        msgNofication.m_strError = strError;

        SendMsgToAll(&msgNofication);
    }
}

void CWeddingRoom::OnStateChange(bool bValid)
{
    if (!bValid) {
        for (WeddingRoomPlayerMap_t::iterator itr = m_mapPlayers.begin(); itr != m_mapPlayers.end(); ++itr) {
            if (itr->second != NULL && itr->second->RoomRoleType() != EWeddingRoomRoleType_Guest) {
                itr->second->SetRoomRoleType(EWeddingRoomRoleType_Guest);
            }
        }

        GameMsg_S2C_WeddingRoomStateChanged msgNotify;
        msgNotify.m_bValid = false;
        SendMsgToAll(&msgNotify);
    }
}

void CWeddingRoom::SendGiftToBlessinger()
{
    std::map<unsigned int, unsigned int>::iterator it = m_RedEnvelopePlayers.begin();
    for (; it != m_RedEnvelopePlayers.end(); ++it)
    {
        std::string strMailTitle(CLocalization::Instance().GetString("Couple_BlessingSender_Gift_Title"));
        std::string strMailContent;
        SafeFormat(strMailContent, CLocalization::Instance().GetString("Couple_BlessingSender_Gift_Content"),
            m_strGroomName.c_str(), m_strBrideName.c_str());

        CItem item;
        item.m_nItemType = CCoupleConfigDataMgr::Instance().GetBlessingFeedBackItemID();
        item.m_nItemCount = 1;
        item.m_nDynamicBoxRelatedValue = it->second*CCoupleConfigDataMgr::Instance().GetRedEnvelopeBlessing();

        CMailMgr::Instance().SendSystemMailToPlayer( it->first, EMailType_BlessingGift, strMailTitle, strMailContent, item );
    }
}

void CWeddingRoom::SendGiftToCouple()
{
    itemtype_t nItemID = CCoupleConfigDataMgr::Instance().GetWeddingGiftBoxID(GetRedEnvelopeAllCount());
    if (nItemID <= 0)
    {
        return;
    }

    std::string strMailTitle(CLocalization::Instance().GetString("Couple_BlessingCouple_Gift_Title"));
    std::string strMailContent;
    SafeFormat(strMailContent, CLocalization::Instance().GetString("Couple_BlessingCouple_Gift_Content"),
        m_strGroomName.c_str(), m_strBrideName.c_str());

    CMailMgr::Instance().SendSystemMailToPlayer( m_nBrideID, EMailType_BlessingGift, strMailTitle, strMailContent, nItemID, 1, 0 );
    CMailMgr::Instance().SendSystemMailToPlayer( m_nGroomID, EMailType_BlessingGift, strMailTitle, strMailContent, nItemID, 1, 0 );

    //log
    CRoleCoupleLog * pLog = new CRoleCoupleLog();
    pLog->m_nMaleRoleID = m_nGroomID;
    pLog->m_nFemaleRoleID = m_nBrideID;
    pLog->m_nBlessingValue = GetBlessingValue();
    pLog->m_eCoupleCause = CRoleCoupleLog::ERoleCoupleCause_CoupleReward;
    g_pQueryMgr->AddQuery(QUERY_LogRoleCouple, 0, pLog);
}

unsigned int CWeddingRoom::GetRedEnvelopeAllCount()
{
    unsigned int nAllCount = 0;
    std::map<unsigned int, unsigned int>::iterator it = m_RedEnvelopePlayers.begin();
    for (; it != m_RedEnvelopePlayers.end(); ++it)
    {
        if (it->second > 0)
        {
            nAllCount += it->second;
        }
    }

    return nAllCount;
}

void CWeddingRoom::BlessingCouple(unsigned int nRoleID, unsigned int nSex)
{
    if (IsHaveBlessing(nRoleID))
    {
        return;
    }

    m_BlessingRoleIDList.push_back(nRoleID);
    m_nBlessingValue += 1;

    // 更新祝福值到group的婚礼记录
    GameMsg_S2G_UpdateWeddingRecord  s2gMsg;
    s2gMsg.m_nBlessingType = GameMsg_S2G_UpdateWeddingRecord::EBlessingType_BlessingCouple;
    s2gMsg.m_nPairID = m_nPairID;
    s2gMsg.m_nBlessingValue = m_nBlessingValue;
    s2gMsg.m_nBlessingRoleID = nRoleID;
    ::SendMsg2GroupServer(&s2gMsg);
}

bool CWeddingRoom::IsHaveBlessing(unsigned int nRoleID)
{
    std::vector<unsigned int>::iterator it = m_BlessingRoleIDList.begin();
    for (; it != m_BlessingRoleIDList.end(); ++it)
    {
        if (*it == nRoleID)
        {
            return true;
        }
    }

    return false;
}



void CWeddingRoom::RedEnvelope(unsigned int nRoleID, std::string & strRoleName, unsigned int nRedEnvelopeCount)
{
    // 添加祝福值
    unsigned int nAddBlessingValue = nRedEnvelopeCount*CCoupleConfigDataMgr::Instance().GetRedEnvelopeBlessing();
    m_nBlessingValue += nAddBlessingValue;
    m_RedEnvelopePlayers[nRoleID] += nRedEnvelopeCount;

    // 红包日志
    CRoleCoupleLog * pLog = new CRoleCoupleLog();
    pLog->m_nMaleRoleID = m_nGroomID;
    pLog->m_nFemaleRoleID = m_nBrideID;
    pLog->m_nRedEvnelope = nRedEnvelopeCount;
    pLog->m_eCoupleCause = CRoleCoupleLog::ERoleCoupleCause_RedEvnelopeCount;
    g_pQueryMgr->AddQuery(QUERY_LogRoleCouple, 0, pLog);

    // 祝福值成功角色扣钱
    int nCostMoney = ((int)nRedEnvelopeCount)*CCoupleConfigDataMgr::Instance().GetRedEnvelopeValue();
    CRoleEntity * pRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(nRoleID);
    if (pRoleEntity != NULL)
    {
        pRoleEntity->GetRoleAttr()->ChangeBill((-1)*nCostMoney, EChangeBillCause_Del_RedEnvelope);
    }

    // 更新祝福值到group的婚礼记录
    GameMsg_S2G_UpdateWeddingRecord  s2gMsg;
    s2gMsg.m_nBlessingType = GameMsg_S2G_UpdateWeddingRecord::EBlessingType_RedEnvelope;
    s2gMsg.m_nBlessingRoleID = nRoleID;
    s2gMsg.m_nPairID = m_nPairID;
    s2gMsg.m_nBlessingValue = m_nBlessingValue;
    ::SendMsg2GroupServer(&s2gMsg);

    // 红字公告
    if (m_RedEnvelopePlayers[nRoleID] >= CCoupleConfigDataMgr::Instance().GetRedEnvelopeNoticeCount())
    {
        CRoleEntity * pBrideEntity = CPlayerManager::Instance().GetEntityByRoleID(m_nBrideID);
        if (pBrideEntity != NULL)
        {
            pBrideEntity->SysChat(CLocalization::Instance().GetString("Couple_RedEnvelope_Content").c_str(), strRoleName.c_str());
        }

        CRoleEntity * pGroomEntity = CPlayerManager::Instance().GetEntityByRoleID(m_nGroomID);
        if (pGroomEntity != NULL)
        {
            pGroomEntity->SysChat(CLocalization::Instance().GetString("Couple_RedEnvelope_Content").c_str(), strRoleName.c_str());
        }
    }
}

void CWeddingRoom::SetRoomBriefInfo(CWeddingRoomBriefInfo & rRoomBriefIno) const
{
    rRoomBriefIno.m_nRoomID = m_nRoomID;
    rRoomBriefIno.m_strRoomName = m_strRoomName;
    rRoomBriefIno.m_bHasPassword = HasPassword();
    rRoomBriefIno.m_nWeddingType = (unsigned char)m_eWeddingType;
    int i = 0;
    for (WeddingRoomPlayerMap_t::const_iterator itr = m_mapPlayers.begin(); itr != m_mapPlayers.end() && i < C_WEDDINGROOM_GEUST_COUNT; ++itr) {
        if (itr->second != NULL && itr->second->RoleAttr() != NULL && itr->second->RoomRoleType() != EWeddingRoomRoleType_Groom && itr->second->RoomRoleType() != EWeddingRoomRoleType_Bride) {
            rRoomBriefIno.m_arrPlayerSex[i++] = itr->second->RoleAttr()->GetSex();
        }
    }
    rRoomBriefIno.m_nGroomID = m_nGroomID;
    rRoomBriefIno.m_strGroomName = m_strGroomName;
    rRoomBriefIno.m_nBrideID = m_nBrideID;
    rRoomBriefIno.m_strBrideName = m_strBrideName;
}

void CWeddingRoom::SetWholeInfo(CWeddingRoomWholeInfo& rWholeInfo, bool bHost) const
{
    rWholeInfo.m_nRoomID = m_nRoomID;
    rWholeInfo.m_nPairID = m_nPairID;
    rWholeInfo.m_strRoomName = m_strRoomName;
    rWholeInfo.m_nGroomID = m_nGroomID;
    rWholeInfo.m_nBrideID = m_nBrideID;
    rWholeInfo.m_bHasPwd = !m_strPassword.empty();
    rWholeInfo.m_strRoomPwd = bHost ? m_strPassword : "";
    rWholeInfo.m_bIsHost = bHost;
    rWholeInfo.m_nWeddingType = (unsigned char)m_eWeddingType;
    __int64 nRemainTime = m_nCreateTime + C_MAX_WEDDING_ROOM_EXISTS_TIME - time(NULL);
    rWholeInfo.m_nWeddingTimeRemaind = nRemainTime > 0 ? (unsigned int)nRemainTime : 0;

    for (WeddingRoomPlayerMap_t::const_iterator itr = m_mapPlayers.begin(); itr != m_mapPlayers.end(); ++itr) {
        if (itr->second != NULL) {
            CWeddingRoomPlayerInfo weddingRoomPlayer;
            (itr->second)->SetRoomPlayerInfo(weddingRoomPlayer);
            rWholeInfo.m_listPlayers.push_back(weddingRoomPlayer);
        }
    }
}

void CWeddingRoom::SendMsgToAllBut(GameMsg_Base *pMsg, CEntityCoupleComponent *pExceptRole)
{
    if (pMsg != NULL) {
        for (WeddingRoomPlayerMap_t::iterator itr = m_mapPlayers.begin(); itr != m_mapPlayers.end(); ++itr) {
            if (itr->second != NULL && itr->second != pExceptRole) {
                itr->second->SendPlayerMsg(pMsg);
            }
        }
    }
}

void CWeddingRoom::SendMsgToAll(GameMsg_Base *pMsg)
{
    if (pMsg != NULL) {
        for (WeddingRoomPlayerMap_t::iterator itr = m_mapPlayers.begin(); itr != m_mapPlayers.end(); ++itr) {
            if (itr->second != NULL) {
                itr->second->SendPlayerMsg(pMsg);
            }
        }
    }
}

