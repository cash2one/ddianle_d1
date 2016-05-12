#include "LogicCirculator.h"
#include "../libServerFrame/QueryManager.h"
#include "PlayerManager.h"
#include "../util/IDCreater.h"
#include "../libServerFrame/Main.h"
#include "../room/RoomManager.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../quest/EntityQuestComponent.h"
#include "../achievement/EntityAchievementComponent.h"
#include "../activity/EntityActivityComponent.h"
#include "../../datastructure/Entity.h"
#include "../../datastructure/LogicInitData.h"
#include "EntityComponentEventID.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../room/RoomComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../item/EntityItemComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../login/GameMsg_S2G_Login.h"
#include "../mall/EntityMallComponent.h"
#include "../mail/EntityMailComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../CheckIn/EntityCheckInComponent.h"
#include "EntityTimerComponent.h"
#include "../player/PlayerInfoComponent.h"
#include "../activity/SystemActivityManager.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../bet/BetComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../Notification/NotificationComponent.h"
#include "../security/SecurityComponent.h"
#include "../anti-addiction/EntityAntiAddictionComponent.h"
#include "../compensation/CompensationComponent.h"
#include "../PlatformPrivilege/EntityPlatformPrivilegeComponent.h"
#include "../quest/EntityQuestNewComponent.h"
#include "../Dungeon/EntityDungeonComponent.h"
#include "../MonthCard/EntityMonthCardComponent.h"
#include "../Rank/RankComponent.h"
#include "../activeness/ActivenessComponent.h"
#include "../Pet/PetComponent.h"
#include "../Medal/EntityMedalComponent.h"
#include "../Handbook/EntityHandbookComponent.h"
#include "../Horse/EntityHorseComponent.h"

extern CLogicCirculator* g_pCirculator;
extern int g_nSection;
extern CIni g_GameConfig;
extern int g_nLine;
extern CQueryManager*	g_pLoginQueryMgr;



bool CPlayerManager::Create()
{
    return CSrvManagerBase::Create(MAXPLAYERS);
}

void CPlayerManager::OnConnectedToWorld()
{
    if (GetCurCount() > 0)
    {
        for (int i = 0; i < m_nMaxCount; ++i)
        {
            if (m_anEntityState[i] != 3)
            {
                continue;
            }
            CRoleEntity *pPlayer = (CRoleEntity *)m_ppEntity[i];
            if (!pPlayer->IsValid())
            {
                continue;
            }
            NoticeGroupServerPlayerEnter(pPlayer);
        }
    }
}

CPlayerManager::CPlayerManager()
{
}

CPlayerManager::~CPlayerManager()
{
}

CPlayerManager& CPlayerManager::Instance()
{
    static CPlayerManager stcPlayerMgr;
    return stcPlayerMgr;
}

CRoleEntity* CPlayerManager::GetPlayer(const PLAYER_TAG& tagRole) const
{
    if (tagRole.nIndex <= 0 || tagRole.nIndex > m_nMaxIndex)
    {
        ASSERT(0);
        WriteLog(LOGLEVEL_ERROR, "CPlayerManager::GetPlayer1(%d,%d) return NULL,m_nMaxIndex=%d", tagRole.nIndex, tagRole.nRoleID, m_nMaxIndex);
        return NULL;
    }
    return GetPlayer(tagRole.nIndex, tagRole.nRoleID);
}

CRoleEntity* CPlayerManager::GetPlayer(int iEntity, unsigned int nRoleID) const
{
    CRoleEntity*pPlayer = GetPlayer(iEntity);
    if (pPlayer)
    {
        if (pPlayer->GetRoleID() != nRoleID || m_anEntityState[iEntity] <= 1)
            pPlayer = NULL;
    }
    return pPlayer;
}

CRoleEntity* CPlayerManager::GetPlayer(int iEntity)const
{
    CRoleEntity*pPlayer = NULL;
    if (iEntity <= 0 || iEntity > m_nMaxIndex)
    {
        WriteLog(LOGLEVEL_ERROR, "CPlayerManager::GetPlayer3(%d) return NULL,m_nMaxIndex=%d", iEntity, m_nMaxIndex);
        return pPlayer;
    }

    pPlayer = (CRoleEntity*)m_ppEntity[iEntity];
    if (pPlayer)
    {
        if (m_anEntityState[iEntity] <= 1)
        {
            pPlayer = NULL;
        }
    }

    return pPlayer;
}

CRoleEntity* CPlayerManager::GetEntityByRoleID(unsigned int nRoleID)
{
    int nIndex = GetPlayerIndexByRoleID(nRoleID);
    if (nIndex == -1)
    {
        return NULL;
    }
    return GetPlayer(nIndex);
}

CRoleEntity* CPlayerManager::GetPlayerByAccount(unsigned int nAccount) const
{
    int i;
    CRoleEntity** ppEntity = (CRoleEntity**)m_ppEntity;
    for (i = 0; i <= m_nMaxIndex; i++)
    {
        ASSERT(!ppEntity[i] || ppEntity[i]->m_EntityType == EntityType_CRoleEntity);
        if (ppEntity[i] && ppEntity[i]->GetComponent<CRoleAttribute>()->GetAccount() == nAccount && m_anEntityState[i] > 1 && ppEntity[i]->IsValid())
            break;
    }

    return i <= m_nMaxIndex ? ppEntity[i] : NULL;
}

int CPlayerManager::AddEntity(CEntity* pNewEntity)
{
    int i;
    for (i = 1; i < m_nMaxCount; i++)
    {
        if (m_anEntityState[i] == 0)
        {
            ASSERT(m_ppEntity[i] == NULL);
            break;
        }
    }
    ASSERT(i < m_nMaxCount);
    if (i == m_nMaxCount)
        i = 0;
    if (i > 0)
    {
        m_ppEntity[i] = pNewEntity;
        m_anEntityState[i] = 3;
        m_nEntityCount++;
        if (m_nMaxIndex < i)
            m_nMaxIndex = i;
    }
    return i;
}

void CPlayerManager::AddEntityToMap(unsigned int nRoleID, int iEntity)
{
    if (nRoleID <= 0 || iEntity <= 0 || iEntity > m_nMaxIndex)
        return;
    m_mapEntityIndex[nRoleID] = iEntity;
}

void CPlayerManager::DeleteEntityFromMap(unsigned int nRoleID)
{
    tmapEntity::iterator it = m_mapEntityIndex.find(nRoleID);
    if (it != m_mapEntityIndex.end()){
        m_mapEntityIndex.erase(it);
    }
}

int CPlayerManager::GetPlayerIndexByRoleID(unsigned int nRoleID)
{
    tmapEntity::iterator it = m_mapEntityIndex.find(nRoleID);
    if (it != m_mapEntityIndex.end())
    {
        return it->second;
    }
    return -1;
}

bool CPlayerManager::NewPlayer(ROLEINFO_DB* pRoleInfoDB, PLAYERTAG* pTag, bool bSwitchLine)
{
    WriteLog(LOGLEVEL_DEBUG, "player enter in NewPlayer, account = %u, roleid = %u\n", pRoleInfoDB->nAccount, pRoleInfoDB->m_nRoleID);

    bool bReturnValue = false;
    if (m_nEntityCount >= m_nMaxCount || pRoleInfoDB == NULL || pTag == NULL)
    {
        return bReturnValue;
    }

    bool bFirstLogin = false;
    bFirstLogin = (pRoleInfoDB->m_nLastLoginTime == 0);

    CRoleEntity* pNewPlayer = new CRoleEntity;
    if (pNewPlayer == NULL){
        return bReturnValue;
    }

    // add new player to array
    int i = AddEntity(pNewPlayer);
    if (i < 1)
    {
        delete pNewPlayer;
        return bReturnValue;
    }
    WriteLog(LOGLEVEL_DEBUG, "new player's index = %d\n", i);


    CRoleAttribute * pRoleAttribute = pNewPlayer->AttachComponent<CRoleAttribute>();
    CRoleNet * pRoleNet = pNewPlayer->AttachComponent<CRoleNet>();
    pNewPlayer->AttachComponent<CRoleFriend>();
    pNewPlayer->AttachComponent<CRoleAchievement>();
    pNewPlayer->AttachComponent<CRoleRoom>();
    pNewPlayer->AttachComponent<CRoleChat>();
    pNewPlayer->AttachComponent<CRoleItem>();
    pNewPlayer->AttachComponent<CRoleGene>();
    pNewPlayer->AttachComponent<CRoleQuest>();
    pNewPlayer->AttachComponent<CRoleMall>();
    pNewPlayer->AttachComponent<CRoleMail>();
    pNewPlayer->AttachComponent<CRoleVIP>();
    pNewPlayer->AttachComponent<CRolePlatformPrivilege>();
    pNewPlayer->AttachComponent<CRoleCheckIn>();
    pNewPlayer->AttachComponent<CRoleTimer>();
    pNewPlayer->AttachComponent<CPlayerInfo>();
    pNewPlayer->AttachComponent<CRoleCouple>();
    pNewPlayer->AttachComponent<CRoleBet>();
    pNewPlayer->AttachComponent<CRolePhotoRoom>();
    CRoleActivity *pRoleActivity = pNewPlayer->AttachComponent<CRoleActivity>();
    pNewPlayer->AttachComponent<CRoleDanceGroup>();
    pNewPlayer->AttachComponent<CRoleAmuseRoom>();

    pNewPlayer->AttachComponent<CRoleNotification>();
    pNewPlayer->AttachComponent<CRoleSecurity>();

    pNewPlayer->AttachComponent<CRoleQuestNew>();
    pNewPlayer->AttachComponent<CRoleDungeon>();
    //满足条件才增加防沉迷组件
    if (CAntiAddictionDataMgr::Instance().IsOpenAntiaddiction() &&
        CAntiAddictionDataMgr::Instance().HasConfig(pRoleInfoDB->m_nPID))
    {
        pNewPlayer->AttachComponent<CRoleAntiAddiction>();
    }
    pNewPlayer->AttachComponent<CRoleCompensation>();
    pNewPlayer->AttachComponent<CRoleMonthCard>();
    pNewPlayer->AttachComponent<CRoleRank>();
    pNewPlayer->AttachComponent<CRoleActiveness>();
    pNewPlayer->AttachComponent<CRolePet>();
    pNewPlayer->AttachComponent<CRoleMedal>();
    pNewPlayer->AttachComponent<CRoleHandbook>();
    pNewPlayer->AttachComponent<CRoleHorse>();
    pNewPlayer->Start();//组件间相互依赖

    //读取人物属性数据
    if (!pNewPlayer->CreateFromDB(pRoleInfoDB))
    {
        ASSERT(0);
        delete pNewPlayer;
        return bReturnValue;
    }

    pTag->m_nIndexLogic = (unsigned short)i;
    pTag->m_nRoleID = pRoleAttribute->GetRoleID();
    WriteLog(LOGLEVEL_DEBUG, "new player's Tag is index = %u, roleid = %u, account = %u\n", pTag->m_nIndexLogic, pTag->m_nRoleID, pTag->m_nAccount);

    pNewPlayer->ReadToRun(pTag->m_nIndexLogic);

    pRoleNet->SetPlayerTag(pTag);
    pNewPlayer->SetValid(true);

    AddEntityToMap(pTag->m_nRoleID, pTag->m_nIndexLogic);

    pNewPlayer->SendExtraStaticData();

    if (bFirstLogin)
    {
        pNewPlayer->SendEvent(eComponentEvent_FirstLogin, pRoleInfoDB);
    }
    else if (pRoleAttribute->GetTodayFirstLoginTag())
    {
        pNewPlayer->SendEvent(eComponentEvent_TodayFirstLogin, pRoleInfoDB);
    }
    else
    {
        pNewPlayer->SendEvent(eComponentEvent_NormalLogin, pRoleInfoDB);
    }

    //Send Create Role Msg;
    if (!pNewPlayer->PacketAndSendCreateRoleMsg())
    {
        delete pNewPlayer;
        return bReturnValue;
    }

    NoticeGroupServerPlayerEnter(pNewPlayer);
    pRoleActivity->UpdateOnlineRewardInfoToClient();

    pNewPlayer->OnLogin();

    CSystemActivityManager::Instance().OnRoleLogin(*pNewPlayer);

    if (!bSwitchLine)
    {
        _LogPlayerAction(CPlayerActionLog::EPlayerAction_Login, pNewPlayer->GetAccount(), pNewPlayer->GetRoleID(),
            pNewPlayer->GetRoleName(), pNewPlayer->GetMoney(), pNewPlayer->GetPoint());
    }
    else
    {
        LogPlayerSwicthLine(CPlayerSwitchLineLog::EPlayerAction_Login, pNewPlayer->GetRoleID(), g_nLine);
    }

    return true;
}

void CPlayerManager::NoticeGroupServerPlayerEnter(CRoleEntity* pNewPlayer)
{
    if (pNewPlayer != NULL)
    {
        GameMsg_S2G_Login msgLogin2G;

        msgLogin2G.m_nSession = pNewPlayer->GetSessionID();
        msgLogin2G.m_nAccount = pNewPlayer->GetAccount();
        msgLogin2G.m_nRoleID = pNewPlayer->GetRoleID();
        msgLogin2G.m_strRoleName = pNewPlayer->GetRoleName();
        msgLogin2G.m_nSex = pNewPlayer->GetSex();
        msgLogin2G.m_nIndex = pNewPlayer->GetPlayerTag()->m_nIndexLogic;
        msgLogin2G.m_nLevel = pNewPlayer->GetLevel();
        msgLogin2G.m_nMoney = pNewPlayer->GetMoney();
        msgLogin2G.m_nStar = pNewPlayer->GetLuckStar();
        msgLogin2G.m_bTodayFirstLogin = pNewPlayer->GetTodayFirstLoginTag() ? 1 : 0;
        msgLogin2G.m_nRoomID = (unsigned short)pNewPlayer->GetRoomID();
        msgLogin2G.m_bIsVIP = pNewPlayer->IsVIP();
        msgLogin2G.m_nVIPLevel = (unsigned short)pNewPlayer->GetVIPLevel();
        msgLogin2G.m_nAge = pNewPlayer->GetComponent<CRoleAttribute>()->GetAge();
        msgLogin2G.m_nConstellation = (short)pNewPlayer->GetComponent<CRoleAttribute>()->m_nConstellation;
        msgLogin2G.m_nTopParcloseID = (unsigned short)pNewPlayer->GetComponent<CRoleDungeon>()->GetTopActivedParcloseID();
        msgLogin2G.m_nDanceGroupID = pNewPlayer->GetDanceGroupID();
        msgLogin2G.m_nSkinColor = pNewPlayer->GetComponent<CRoleAttribute>()->GetColor();
        msgLogin2G.m_nTransFormID = pNewPlayer->GetComponent<CRoleAttribute>()->GetTransformId();

        map< unsigned int, FriendInfo* > mapFriend = pNewPlayer->GetComponent<CRoleFriend>()->m_mapFriend;
        for (map< unsigned int, FriendInfo* >::iterator it = mapFriend.begin(); it != mapFriend.end(); ++it)
        {
            msgLogin2G.m_listFriend.push_back(it->second->m_nRoleID);
        }

        map< unsigned int, FriendInfo* > mapBlackFriend = pNewPlayer->GetComponent<CRoleFriend>()->m_mapBlackList;
        for (map< unsigned int, FriendInfo* >::iterator it = mapBlackFriend.begin(); it != mapBlackFriend.end(); ++it)
        {
            msgLogin2G.m_listBlackList.push_back(it->second->m_nRoleID);
        }

        if (pNewPlayer->GetComponent<CRoleCouple>() != NULL)
        {
            msgLogin2G.m_coupleInfo = pNewPlayer->GetComponent<CRoleCouple>()->CoupleInfo();
            msgLogin2G.m_nDivorceAgreementOtherRoleID = pNewPlayer->GetComponent<CRoleCouple>()->GetDivorecAgreementOtherRoleID();
        }

        pNewPlayer->PackRoleRoomImageData(msgLogin2G.m_imageData);
    
        pNewPlayer->SendMsg2GroupServer(&msgLogin2G);
    }
}

void CPlayerManager::OnUpdate(const unsigned long &lTimeElapsed)
{
    CRoleEntity*pPlayer = NULL;
    for (int i = 1; i <= m_nMaxIndex; i++)
    {
        switch (m_anEntityState[i])
        {
        case 3://合法对象执行功能
            pPlayer = (CRoleEntity*)m_ppEntity[i];
            if (pPlayer->IsValid())
            {
                pPlayer->EntityUpdate(lTimeElapsed);

                if (pPlayer->IsValid())
                {
                    if (pPlayer->m_nSaveTime < 0 || pPlayer->m_nSaveTime + lTimeElapsed > pPlayer->m_nRandomSaveCycle)
                    {
                        SavePlayer(pPlayer);
                    }
                    else
                    {
                        pPlayer->m_nSaveTime += lTimeElapsed;
                    }
                }
            }
            else
            {
                pPlayer->SetOffLine(true);
                m_anEntityState[i] = 2;
            }
            break;
        case 2://呆删除对象
        {
                   pPlayer = (CRoleEntity*)m_ppEntity[i];
                   pPlayer->m_nSaveTime += lTimeElapsed;
                   if (pPlayer->m_nSaveTime > 60000)
                   {
                       m_anEntityState[i] = 1;
                   }
        }
            break;
        case 1://立即删除对象
            ////////////added by Rosey
            pPlayer = (CRoleEntity*)m_ppEntity[i];
            delete m_ppEntity[i];
            m_ppEntity[i] = NULL;
            m_anEntityState[i] = 0;
            m_nEntityCount--;

            if (m_nMaxIndex == i){
                int k;
                for (k = m_nMaxIndex - 1; k > 0; k--)
                {
                    if (m_anEntityState[k] != 0)
                        break;
                }
                m_nMaxIndex = k;
                ASSERT(m_nMaxIndex >= m_nEntityCount - 1);
            }
            break;
        }
    }

    m_nSetExpTimeDelay += lTimeElapsed;
    if (m_nSetExpTimeDelay > 60 * 1000)
    {
        m_nSetExpTimeDelay = 0;

        for (int i = 1; i <= m_nMaxIndex; i++)
        {
            if (m_anEntityState[i] != 3)
            {
                continue;
            }
            pPlayer = (CRoleEntity*)m_ppEntity[i];
        }
    }
}

void CPlayerManager::SavePlayer(CRoleEntity*pPlayer, bool bIsSwitchServer)
{
    ROLEINFO_DB *pInfo = new ROLEINFO_DB;
    pPlayer->PacketToDB(pInfo);
    PLAYERTAG *pTag = new PLAYERTAG;

    if (bIsSwitchServer)
    {
        pTag->m_nSlot = pPlayer->GetComponent<CRoleNet>()->GetPlayerSlot();
        pTag->m_nAccount = pPlayer->GetComponent<CRoleAttribute>()->GetAccount();
        pTag->m_nSessionID = pPlayer->GetPlayerTag()->m_nSessionID;
    }
    g_pLoginQueryMgr->AddQuery(QUERY_UpdateRole, 0, pInfo, 0, pTag);

    pPlayer->m_nSaveTime = 0;
}

void CPlayerManager::CachePlayer(CRoleEntity *pPlayer)
{
    ROLEINFO_DB cache;
    pPlayer->PacketToCache(&cache);
    
    // 这里把数据发送出去了。
    pPlayer->CacheEncode(&cache); // to account.
}


void CPlayerManager::LogoutPlayer(CRoleEntity* pPlayer, bool bSwitchLine)
{
    if (pPlayer == NULL || pPlayer->IsValid() == false)
    {
        return;
    }

    if (!bSwitchLine)
    {
        _LogPlayerAction(CPlayerActionLog::EPlayerAction_Logout, pPlayer->GetAccount(), pPlayer->GetRoleID(),
            pPlayer->GetRoleName(), pPlayer->GetMoney(), pPlayer->GetPoint());
    }
    else
    {
        LogPlayerSwicthLine(CPlayerSwitchLineLog::EPlayerAction_Logout, pPlayer->GetRoleID(), g_nLine);
    }

    pPlayer->OnLogout();
    SavePlayer(pPlayer, bSwitchLine);
    // 把player数据缓存起来
    CachePlayer(pPlayer);

    DeleteEntityFromMap(pPlayer->GetRoleID());    
	pPlayer->EntityDestroy();
    pPlayer->GetComponent<CRoleNet>()->SetPlayerTag(NULL);
}

void CPlayerManager::LogoutPlayer(const PLAYERTAG* pTag, bool bIsSwitchServer)
{
    if (pTag != NULL)
    {
        CRoleEntity* pPlayer = GetPlayer(pTag->m_nIndexLogic);
        if (pPlayer != NULL)
        {
            LogoutPlayer(pPlayer, bIsSwitchServer);
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, " * LogoutPlayer Error [%d,%u,%u]\n", 
                pTag->m_nIndexLogic, pTag->m_nRoleID, pTag->m_nAccount);
        }
    }
    else
    {
        WriteLog(LOGLEVEL_ERROR, "* LogoutPlayer Error, tag is null--------");
    }
}

int CPlayerManager::CountPlayer()
{
    int nCount = 0;

    for (int i = 1; i <= m_nMaxIndex; i++)
    {
        if (m_anEntityState[i] == 3) //合法对象
        {
            CRoleEntity* pPlayer = (CRoleEntity*)m_ppEntity[i];
            if (pPlayer->IsValid() == false)continue;
            nCount++;
        }
    }

    return nCount;
}

void CPlayerManager::SendGlobalMessage(GameMsg_Base*pMsg)
{
    CRoleEntity*pPlayer = NULL;

    CEntity** ppEntity = m_ppEntity;
    ppEntity++;
    for (int i = 1; i <= m_nMaxIndex; i++, ppEntity++)
    {
        if (*ppEntity != NULL && (*ppEntity)->IsValid())
        {
            pPlayer = (CRoleEntity*)(*ppEntity);
            pPlayer->SendPlayerMsg(pMsg);
        }
    }
}

void CPlayerManager::SendGlobalPlatformMessage(unsigned short nPID, GameMsg_Base*pMsg)
{
    CRoleEntity*pPlayer = NULL;

    CEntity** ppEntity = m_ppEntity;
    ppEntity++;
    for (int i = 1; i <= m_nMaxIndex; i++, ppEntity++)
    {
        if (*ppEntity != NULL && (*ppEntity)->IsValid())
        {
            pPlayer = (CRoleEntity*)(*ppEntity);
            if (nPID == PID_NONE || pPlayer->PID() == nPID) {
                pPlayer->SendPlayerMsg(pMsg);
            }
        }
    }
}

void CPlayerManager::GetAllPlayers(list<CRoleEntity *> &listPlayer) const
{
    CRoleEntity*pPlayer = NULL;

    CEntity** ppEntity = m_ppEntity;
    ppEntity++;
    for (int i = 1; i <= m_nMaxIndex; i++, ppEntity++)
    {
        if (*ppEntity == NULL || (*ppEntity)->IsValid() == false)
            continue;

        pPlayer = (CRoleEntity*)(*ppEntity);

        listPlayer.push_back(pPlayer);
    }
}
void CPlayerManager::GetRoleIDList(std::list< unsigned int > & rRoleIDList) const
{
    CRoleEntity*pPlayer = NULL;

    CEntity** ppEntity = m_ppEntity;
    ppEntity++;
    for (int i = 1; i <= m_nMaxIndex; i++, ppEntity++)
    {
        if (*ppEntity == NULL || (*ppEntity)->IsValid() == false)
            continue;

        pPlayer = (CRoleEntity*)(*ppEntity);

        rRoleIDList.push_back(pPlayer->GetRoleID());
    }
}

void CPlayerManager::_LogPlayerAction(CPlayerActionLog::EPlayerAction ePlayerAction, unsigned int nAccount, unsigned int nRoleID, const char * szRoleName, int nMoney, unsigned int nCash)
{
    CPlayerActionLog * pPAL = new CPlayerActionLog();
    pPAL->m_nRoleID = nRoleID;
    pPAL->m_ePlayerAction = ePlayerAction;
    pPAL->m_nMoney = nMoney;
    pPAL->m_nCash = nCash;
    g_pQueryMgr->AddQuery(QUERY_LogPlayerAction, 0, pPAL);
}

void CPlayerManager::LogPlayerSwicthLine(CPlayerSwitchLineLog::EPlayerAction eAction, unsigned int nRoleID, int nLine)
{
    CPlayerSwitchLineLog * pLog = new CPlayerSwitchLineLog();
    pLog->m_nRoleID = nRoleID;
    pLog->m_nLine = nLine;
    pLog->m_eAction = eAction;
    g_pQueryMgr->AddQuery(QUERY_LogRoleSwicthLine, 0, pLog);
}

void CPlayerManager::OnActivityChange(unsigned char nActivityID, bool bOpen)
{
    CRoleEntity*pPlayer = NULL;

    CEntity** ppEntity = m_ppEntity;
    ppEntity++;
    for (int i = 1; i <= m_nMaxIndex; i++, ppEntity++)
    {
        if (*ppEntity != NULL && (*ppEntity)->IsValid())
        {
            pPlayer = (CRoleEntity*)(*ppEntity);
            CEntityActivityComponent* pActivityComponent = pPlayer->GetComponent<CRoleActivity>();
            if (pActivityComponent != NULL)
            {
                pActivityComponent->OnActivityChange(nActivityID, bOpen);
            }
        }
    }
}

void CPlayerManager::OnMallReload()
{
    CRoleEntity *pPlayer = NULL;
    CEntityMallComponent *pMallComponent = NULL;

    CEntity **ppEntity = m_ppEntity;
    ++ppEntity;

    for (int i = 1; i <= m_nMaxIndex; ++i, ++ppEntity)
    {
        if (NULL != *ppEntity && (*ppEntity)->IsValid())
        {
            pPlayer = (CRoleEntity *)(*ppEntity);
            pMallComponent = pPlayer->GetComponent<CRoleMall>();

            if (NULL != pMallComponent)
                pMallComponent->SendExtraStaticData();
        }
    }
}

