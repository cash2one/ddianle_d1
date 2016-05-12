#include <time.h>
#include "AnnounceMgr.h"
#include "ITimerGm.h"
#include "GMMsgDef.h"
#include "GameMsg_GM.h"
#include "GameMsg_GM_share.h"
#include "../../socket/GameMsg_Map.h"
#include "AnnounceTimer.h"
#include "TimerMgrGm.h"
#include "GMEntity.h"
#include "GMEntityMgr.h"
#include "priv/PrivCmdMacroDef.h"
#include "TipMsgDef.h"
#include "priv/PrivMgr.h"
#include "GMCirculator.h"
#include "JsonLog.h"
#include "GMQuery.h"

extern CGMQueryMgr*	g_pGMQueryMgr;

CAnnounceMgr::CAnnounceMgr() :
GameMsg_Processor(true, true)
{

}

CAnnounceMgr::~CAnnounceMgr()
{
    for (MapAnnouce_t::iterator itr = m_mapAnnounce.begin(); itr != m_mapAnnounce.end(); ++itr) {
        if (itr->second != NULL) {
            delete itr->second, itr->second = NULL;
        }
    }
    m_mapAnnounce.clear();
}

CAnnounceMgr & CAnnounceMgr::Instance()
{
    static CAnnounceMgr s_TimeMgr;

    return s_TimeMgr;
}

void CAnnounceMgr::Initialize()
{
    _RegisterMessage();
    _RegisterMessageProcessor();

    std::map< unsigned int, CAnnounceInfo * > *pMapAnnounce = new std::map< unsigned int, CAnnounceInfo * >;
    if (pMapAnnounce != NULL) {
        g_pGMQueryMgr->AddQuery(Query_LoadAnnounce, 0, pMapAnnounce);
    }
}

void CAnnounceMgr::Add(ITimerGm *pTimer)
{
    if (pTimer != NULL) {
        CTimerMgr::Instance().Add(pTimer);
    }
}

void CAnnounceMgr::ClearAllAnnounceTimer()
{
    for (MapAnnouce_t::iterator itr = m_mapAnnounce.begin(); itr != m_mapAnnounce.end(); ++itr) {
        if (itr->second != NULL) {
            ITimerGm *pTimer = CTimerMgr::Instance().Remove(ETimerType_Announce, itr->second->m_nID);
            if (pTimer != NULL) {
                delete pTimer, pTimer = NULL;
            }
        }
    }
}

void CAnnounceMgr::OnGMSendAnnounceOnce(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_C2S_GMSendAnnounceOnce & msgAnnouce = (GameMsg_C2S_GMSendAnnounceOnce &)msg;

    std::string strError = "";
    GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pEntity != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pEntity->GetName(), EUserPrivType_SendAnnounce))
        {
            GameMsg_S2C_GMSendAnnounceFail msgFail;
            msgFail.m_nPID = msgAnnouce.m_nPID;
            msgFail.m_bIsSysChat = msgAnnouce.m_bIsSysChat;
            msgFail.m_bIsAdminChat = msgAnnouce.m_bIsAdminChat;
            msgFail.m_strContent = msgAnnouce.m_strContent;
            pEntity->SendMsgToGM(&msgFail);
            return;
        }
        std::string strJson = GMJsonLog::SendAnnounceOnceToJson(msgAnnouce.m_nPID, msgAnnouce.m_strContent, msgAnnouce.m_bIsAdminChat, msgAnnouce.m_bIsSysChat, msgAnnouce.m_nDelayTime);
        CGMCirculator::LogGM(pEntity->GetAccount(), EGMActionType_SendAnnounce, strJson);

        CAnnounceInfo info;
        info.m_bIsSysChat = msgAnnouce.m_bIsSysChat;
        info.m_bIsAdminChat = msgAnnouce.m_bIsAdminChat;
        info.m_strContent = msgAnnouce.m_strContent;
        info.m_nStartTime = (unsigned int)time(NULL) + msgAnnouce.m_nDelayTime;
        info.m_nPID = msgAnnouce.m_nPID;
        _AddNewAnnounceDB(info);
        GameMsg_S2C_GMSendAnnounceSuc msgSuc;
        msgSuc.m_nPID = msgAnnouce.m_nPID;
        msgSuc.m_bIsSysChat = msgAnnouce.m_bIsSysChat;
        msgSuc.m_bIsAdminChat = msgAnnouce.m_bIsAdminChat;
        msgSuc.m_strContent = msgAnnouce.m_strContent;
        pEntity->SendMsgToGM(&msgSuc);

        //string strCmd = string(ADMINCHAT);
        //if (!pEntity->HasCmdPriv(strCmd)) {
        //	strError = STR_GM_NO_AUTHORITY;	
        //} else if (msgAnnouce.m_strContent == "") {
        //	strError = STR_SEND_ANNOUNCE_ERROR1 ;	
        //} else if (!msgAnnouce.m_bIsSysChat && !msgAnnouce.m_bIsAdminChat) {
        //	strError = STR_SEND_ANNOUNCE_ERROR2 ;
        //} else {
        //	CAnnounceInfo info;
        //	info.m_bIsSysChat = msgAnnouce.m_bIsSysChat;
        //	info.m_bIsAdminChat = msgAnnouce.m_bIsAdminChat;
        //	info.m_strContent = msgAnnouce.m_strContent;
        //	info.m_nStartTime = (unsigned int)time(NULL) + msgAnnouce.m_nDelayTime;
        //	info.m_nPID = msgAnnouce.m_nPID;
        //	_AddNewAnnounceDB(info);
        //	GameMsg_S2C_GMSendAnnounceSuc msgSuc;
        //	msgSuc.m_nPID = msgAnnouce.m_nPID;
        //	msgSuc.m_bIsSysChat = msgAnnouce.m_bIsSysChat;
        //	msgSuc.m_bIsAdminChat = msgAnnouce.m_bIsAdminChat;
        //	msgSuc.m_strContent = msgAnnouce.m_strContent;
        //	pEntity->SendMsgToGM(&msgSuc);
        //}

        //if (!strError.empty()) {
        //	GameMsg_S2C_GMSendAnnounceFail msgFail;
        //	msgFail.m_nPID = msgAnnouce.m_nPID;
        //	msgFail.m_bIsSysChat = msgAnnouce.m_bIsSysChat;
        //	msgFail.m_bIsAdminChat = msgAnnouce.m_bIsAdminChat;
        //	msgFail.m_strContent = msgAnnouce.m_strContent;
        //	pEntity->SendMsgToGM(&msgFail);
        //}
    }
}

void CAnnounceMgr::OnGMSendAnnounceInterval(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    GameMsg_C2S_GMSendAnnounceInterval & msgAnnouce = (GameMsg_C2S_GMSendAnnounceInterval &)msg;

    std::string strError = "";
    GMEntity *pEntity = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pEntity != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pEntity->GetName(), EUserPrivType_SendAnnounce))
        {
            GameMsg_S2C_GMSendAnnounceFail msgFail;
            msgFail.m_nPID = msgAnnouce.m_annouce.m_nPID;
            msgFail.m_bIsSysChat = msgAnnouce.m_annouce.m_bIsSysChat;
            msgFail.m_bIsAdminChat = msgAnnouce.m_annouce.m_bIsAdminChat;
            msgFail.m_strContent = msgAnnouce.m_annouce.m_strContent;
            pEntity->SendMsgToGM(&msgFail);
            return;
        }

        std::string strJson = GMJsonLog::SendAnnounceIntervalToJson(msgAnnouce.m_annouce);
        CGMCirculator::LogGM(pEntity->GetAccount(), EGMActionType_SendAnnounce, strJson);

        CAnnounceInfo info(msgAnnouce.m_annouce);
        info.m_nStartTime = msgAnnouce.m_annouce.m_nStartTime == 0 ? (unsigned int)time(NULL) : msgAnnouce.m_annouce.m_nStartTime;
        _AddNewAnnounceDB(info);
        GameMsg_S2C_GMSendAnnounceSuc msgSuc;
        msgSuc.m_nPID = msgAnnouce.m_annouce.m_nPID;
        msgSuc.m_bIsSysChat = msgAnnouce.m_annouce.m_bIsSysChat;
        msgSuc.m_bIsAdminChat = msgAnnouce.m_annouce.m_bIsAdminChat;
        msgSuc.m_strContent = msgAnnouce.m_annouce.m_strContent;
        pEntity->SendMsgToGM(&msgSuc);

        //string strCmd = string(ADMINCHAT);
        //if (!pEntity->HasCmdPriv(strCmd)) {
        //	strError = STR_GM_NO_AUTHORITY;	
        //} else if (msgAnnouce.m_annouce.m_strContent == "") {
        //	strError = STR_SEND_ANNOUNCE_ERROR1 ;	
        //} else if (!msgAnnouce.m_annouce.m_bIsSysChat && !msgAnnouce.m_annouce.m_bIsAdminChat) {
        //	strError = STR_SEND_ANNOUNCE_ERROR2 ;
        //} else if (msgAnnouce.m_annouce.m_nSendInterval == 0) {
        //	strError = STR_SEND_ANNOUNCE_ERROR3;
        //} else {
        //	CAnnounceInfo info(msgAnnouce.m_annouce);
        //	info.m_nStartTime = msgAnnouce.m_annouce.m_nStartTime == 0 ? (unsigned int)time(NULL) : msgAnnouce.m_annouce.m_nStartTime;
        //	_AddNewAnnounceDB(info);
        //	GameMsg_S2C_GMSendAnnounceSuc msgSuc;
        //	msgSuc.m_nPID = msgAnnouce.m_annouce.m_nPID;
        //	msgSuc.m_bIsSysChat = msgAnnouce.m_annouce.m_bIsSysChat;
        //	msgSuc.m_bIsAdminChat = msgAnnouce.m_annouce.m_bIsAdminChat;
        //	msgSuc.m_strContent = msgAnnouce.m_annouce.m_strContent;
        //	pEntity->SendMsgToGM(&msgSuc);
        //}

        //if (strError != "") {
        //	GameMsg_S2C_GMSendAnnounceFail msgFail;
        //	msgFail.m_nPID = msgAnnouce.m_annouce.m_nPID;
        //	msgFail.m_bIsSysChat = msgAnnouce.m_annouce.m_bIsSysChat;
        //	msgFail.m_bIsAdminChat = msgAnnouce.m_annouce.m_bIsAdminChat;
        //	msgFail.m_strContent = msgAnnouce.m_annouce.m_strContent;
        //	pEntity->SendMsgToGM(&msgFail);
        //}
    }
}

void CAnnounceMgr::LoadAnnounceDBCallBack(QueryBase & query)
{
    MapAnnouce_t *pMapAnnouce = (MapAnnouce_t *)query.m_pAttachment;
    if (pMapAnnouce != NULL) {
        if (query.m_nResult == 0) {
            m_mapAnnounce = *pMapAnnouce;
            for (MapAnnouce_t::iterator itr = m_mapAnnounce.begin(); itr != m_mapAnnounce.end();) {
                if (itr->second != NULL) {
                    time_t nTimeNow = time(NULL);
                    bool bAnnouceCompleted = false;
                    if (itr->second->m_nSendInterval > 0 && itr->second->m_nStartTime > 0 && itr->second->m_nStartTime <= (unsigned int)nTimeNow) {
                        itr->second->m_nState = CAnnounceInfo::ANNOUNCE_STATE_PROCESSING;

                        unsigned int nTime = itr->second->m_nLastSendTime > itr->second->m_nStartTime ? itr->second->m_nLastSendTime : itr->second->m_nStartTime;
                        unsigned int nTriggerTime = nTime + itr->second->m_nSendInterval;
                        if (itr->second->m_nEndTime == 0 || nTriggerTime <= itr->second->m_nEndTime) {
                            CAnnounceTimer *pTimer = new CAnnounceTimer;
                            if (pTimer != NULL) {
                                pTimer->m_nTriggerTime = nTriggerTime;
                                pTimer->m_annouceInfo = *(itr->second);
                                Add(pTimer);
                                _UpdateAnnounceRecordDB(*(itr->second));
                            }
                        }
                        else {
                            bAnnouceCompleted = true;
                        }
                    }
                    else {
                        bAnnouceCompleted = true;
                    }

                    if (bAnnouceCompleted) {
                        itr->second->m_nState = CAnnounceInfo::ANNOUNCE_STATE_FINISHED;
                        _UpdateAnnounceRecordDB(*(itr->second));
                        delete itr->second, itr->second = NULL;
                        m_mapAnnounce.erase(itr++);
                    }
                    else {
                        ++itr;
                    }
                }
            }
        }
    }
}

void CAnnounceMgr::AddAnnounceDBCallBack(QueryBase & query)
{
    CAnnounceInfo *pRecord = (CAnnounceInfo *)query.m_pAttachment;
    if (pRecord != NULL) {
        if (query.m_nResult == 0) {
            m_mapAnnounce.insert(make_pair(pRecord->m_nID, pRecord));
            CAnnounceTimer *pTimer = new CAnnounceTimer;
            if (pTimer != NULL) {
                pTimer->m_nTriggerTime = pRecord->m_nStartTime;
                pTimer->m_annouceInfo = *pRecord;
                Add(pTimer);
            }
        }
    }
}

void CAnnounceMgr::_AddNewAnnounceDB(CAnnounceInfo & rAnnounceInfo)
{
    if (rAnnounceInfo.m_nStartTime > 0) {
        CAnnounceInfo *pRecord = new CAnnounceInfo(rAnnounceInfo);
        if (pRecord != NULL) {
            g_pGMQueryMgr->AddQuery(Query_AddAnnounce, 0, pRecord);
        }
    }
}

void CAnnounceMgr::HandleNextAnnounce(CAnnounceInfo & rAnnounceInfo)
{
    bool bAnnouceCompleted = false;
    MapAnnouce_t::iterator itr = m_mapAnnounce.find(rAnnounceInfo.m_nID);
    if (itr != m_mapAnnounce.end() && itr->second != NULL) {
        time_t nTimeNow = time(NULL);
        itr->second->m_nLastSendTime = (unsigned int)nTimeNow;

        if (itr->second->m_nSendInterval > 0) {
            itr->second->m_nState = CAnnounceInfo::ANNOUNCE_STATE_PROCESSING;
            unsigned int nNextTriggerTime = itr->second->m_nLastSendTime + itr->second->m_nSendInterval;
            if (itr->second->m_nEndTime == 0 || nNextTriggerTime <= itr->second->m_nEndTime) {
                CAnnounceTimer *pTimer = new CAnnounceTimer;
                if (pTimer != NULL) {
                    pTimer->m_nTriggerTime = nNextTriggerTime;
                    pTimer->m_annouceInfo = *(itr->second);
                    Add(pTimer);
                    _UpdateAnnounceRecordDB(*(itr->second));
                }
            }
            else {
                bAnnouceCompleted = true;
            }
        }
        else {
            bAnnouceCompleted = true;
        }

        if (bAnnouceCompleted) {
            itr->second->m_nState = CAnnounceInfo::ANNOUNCE_STATE_FINISHED;
            _UpdateAnnounceRecordDB(*(itr->second));
            delete itr->second, itr->second = NULL;
            m_mapAnnounce.erase(itr);
        }
    }
}

void CAnnounceMgr::_RegisterMessage()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GMSendAnnounceOnce);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GMSendAnnounceInterval);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetAnnounce);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DeleteAnnounce);
}

void CAnnounceMgr::_RegisterMessageProcessor()
{
    RegMsgProcessor(GM_C2S_SendAnnounceOnce, &CAnnounceMgr::OnGMSendAnnounceOnce);
    RegMsgProcessor(GM_C2S_SendAnnounceInterval, &CAnnounceMgr::OnGMSendAnnounceInterval);
    RegMsgProcessor(GM_C2S_GetAnnounce, &CAnnounceMgr::OnGMGetAnnounce);
    RegMsgProcessor(GM_C2S_DeleteAnnounce, &CAnnounceMgr::OnGMDeleteAnnounce);

    RegQueryProcessor(Query_LoadAnnounce, &CAnnounceMgr::LoadAnnounceDBCallBack);
    RegQueryProcessor(Query_AddAnnounce, &CAnnounceMgr::AddAnnounceDBCallBack);

}

void CAnnounceMgr::_RemoveAnnounceRecordDB(CAnnounceInfo & announce)
{
    CAnnounceInfo *pRecord = new CAnnounceInfo(announce);
    if (pRecord != NULL) {
        g_pGMQueryMgr->AddQuery(Query_RemoveAnnouce, 0, pRecord);
    }
}

void CAnnounceMgr::_UpdateAnnounceRecordDB(CAnnounceInfo & announce)
{
    CAnnounceInfo *pRecord = new CAnnounceInfo(announce);
    g_pGMQueryMgr->AddQuery(Query_UpdateAnnouce, 0, pRecord);
}

void CAnnounceMgr::_InitLoadedAnnouce()
{
    ClearAllAnnounceTimer();

    for (MapAnnouce_t::iterator itr = m_mapAnnounce.begin(); itr != m_mapAnnounce.end();) {
        if (itr->second != NULL) {
            time_t nTimeNow = time(NULL);
            bool bAnnouceCompleted = false;
            if (itr->second->m_nSendInterval > 0 && itr->second->m_nStartTime > 0 && itr->second->m_nStartTime <= (unsigned int)nTimeNow) {
                itr->second->m_nState = CAnnounceInfo::ANNOUNCE_STATE_PROCESSING;

                unsigned int nTime = itr->second->m_nLastSendTime > itr->second->m_nStartTime ? itr->second->m_nLastSendTime : itr->second->m_nStartTime;
                unsigned int nTriggerTime = nTime + itr->second->m_nSendInterval;
                if (itr->second->m_nEndTime == 0 || nTriggerTime <= itr->second->m_nEndTime) {
                    CAnnounceTimer *pTimer = new CAnnounceTimer;
                    if (pTimer != NULL) {
                        pTimer->m_nTriggerTime = nTriggerTime;
                        pTimer->m_annouceInfo = *(itr->second);
                        Add(pTimer);
                        _UpdateAnnounceRecordDB(*(itr->second));
                    }
                }
                else {
                    bAnnouceCompleted = true;
                }
            }
            else {
                bAnnouceCompleted = true;
            }

            if (bAnnouceCompleted) {
                itr->second->m_nState = CAnnounceInfo::ANNOUNCE_STATE_FINISHED;
                _UpdateAnnounceRecordDB(*(itr->second));
                delete itr->second, itr->second = NULL;
                m_mapAnnounce.erase(itr++);
            }
            else {
                ++itr;
            }
        }
    }
}

void CAnnounceMgr::OnGMGetAnnounce(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_SendAnnounce))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        GameMsg_S2C_GetAnnounceRes sMsg;
        MapAnnouce_t::iterator it = m_mapAnnounce.begin();
        for (; it != m_mapAnnounce.end(); it++)
        {
            sMsg.m_AnnounceInfoList.push_back(*it->second);
        }
        pRole->SendMsgToGM(&sMsg);
    }
}

void CAnnounceMgr::OnGMDeleteAnnounce(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
    GameMsg_C2S_DeleteAnnounce& rMsg = (GameMsg_C2S_DeleteAnnounce&)msg;
    GMEntity* pRole = GMEntityMgr::Instance().GetEntityBySlot(slotPeer.GetSlot());
    if (pRole != NULL)
    {
        if (!CPrivMgr::Instance().HasPriv(pRole->GetName(), EUserPrivType_SendAnnounce))
        {
            GameMsg_S2C_PrivError retMsg;
            pRole->SendMsgToGM(&retMsg);
            return;
        }

        bool bRet = false;

        MapAnnouce_t::iterator it = m_mapAnnounce.find(rMsg.m_nAnnounceID);
        if (it != m_mapAnnounce.end())
        {
            ITimerGm *pTimer = CTimerMgr::Instance().Remove(ETimerType_Announce, it->second->m_nID);
            if (pTimer != NULL)
            {
                delete pTimer, pTimer = NULL;
            }

            _RemoveAnnounceRecordDB(*it->second);

            m_mapAnnounce.erase(it);

            bRet = true;
        }

        GameMsg_S2C_DeleteAnnounceRes sMsg;
        sMsg.m_bResult = bRet;
        pRole->SendMsgToGM(&sMsg);
    }
}

