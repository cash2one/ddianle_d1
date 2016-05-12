/********************************************************************
	created:	 2011/01/10
	author:		 meijiuhua
    FileName   : global.cpp
    Description: global data manager (Group Server) Source File
********************************************************************/

#include <stdarg.h>
#include <algorithm>
#include "Global.h"
#include "../chat/GameMsg_G2S_Chat.h"
#include "../Rank/DungeonRankMgr.h"
#include "../libServerFrame/QueryManager.h"


extern CQueryManager*	g_pQueryMgr;

using namespace std;

//存放所有线在线玩家基本信息的数组
//role id 是关键字
map<unsigned int , CGroupRoleEntity> g_mapRoleEntity;

//key --> account id,  value --> role id
map<unsigned int , unsigned int> g_mapAccountIndex;

CGroupPlayerManager::CGroupPlayerManager()
{
}

CGroupPlayerManager::~CGroupPlayerManager()
{
}

//角色上线（把对象插入map）
void CGroupPlayerManager::OnLogin(GameMsg_S2G_Login *pMsg, int nLine, const string &strLineName)
{
    if (pMsg != NULL)
    {
        if (SearchRoleByID(pMsg->m_nRoleID) )//重复登录玩家
        {
            g_mapRoleEntity.erase(pMsg->m_nRoleID);
        }

        CGroupRoleEntity role;
        role.m_nSession = pMsg->m_nSession;
        role.m_nRoleID = pMsg->m_nRoleID;
        strcpy(role.m_szRoleName, pMsg->m_strRoleName.c_str() );
        role.m_nAccount = pMsg->m_nAccount;
        role.m_nSex = pMsg->m_nSex;
        role.m_nIndex = pMsg->m_nIndex;
        role.SetLevel(pMsg->m_nLevel) ;
        role.SetMoney(pMsg->m_nMoney);
        role.m_nStarCount = pMsg->m_nStar;
        role.m_bIsVIP = pMsg->m_bIsVIP;
        role.m_nVIPLevel = pMsg->m_nVIPLevel;
        role.m_nLine = nLine;
        role.m_strLineName = strLineName;
        role.m_bTodayFirstLogin = pMsg->m_bTodayFirstLogin;
        role.m_nRoomID = pMsg->m_nRoomID;
        role.SetRoleAge(pMsg->m_nAge);
        role.SetRoleConstellation(pMsg->m_nConstellation);
        role.m_nLoginTimeOnGroup = (unsigned int)time(NULL);
        role.m_nTopParcloseID = pMsg->m_nTopParcloseID;
        role.m_nDivorceAgreementOtherRoleID = pMsg->m_nDivorceAgreementOtherRoleID;
        role.SetDanceGroupID(pMsg->m_nDanceGroupID);
        role.SetSkinColor(pMsg->m_nSkinColor);

        // 好友系统
        role.m_listFriend.insert(role.m_listFriend.begin(), pMsg->m_listFriend.begin(), pMsg->m_listFriend.end() );//好友
        role.m_listBlackList.insert(role.m_listBlackList.begin(), pMsg->m_listBlackList.begin(), pMsg->m_listBlackList.end() );//黑名单
        role.m_coupleInfo = pMsg->m_coupleInfo;

        

        g_mapRoleEntity[pMsg->m_nRoleID] = role ;

        g_mapAccountIndex[pMsg->m_nAccount] = pMsg->m_nRoleID;

        // 其他模块数据的初始化，譬如婚姻等需要玩家登陆初始化的数据
    }
}

void CGroupRoleEntity::SetRoleName(const char * szName)
{
	if (szName) {
		size_t nSize = strlen(szName);
		if (nSize > MAX_NAME_LEN - 1) {
			nSize = MAX_NAME_LEN - 1;
		}
		memset(m_szRoleName, 0, sizeof(m_szRoleName));
		memcpy(m_szRoleName, szName, nSize);
	}
}

int CGroupRoleEntity::GetRoomState()
{
	return m_nRoleRoomState;
}

CGroupOffLineRoleEntity::CGroupOffLineRoleEntity()
    :m_nOfflineTime(0)
{

}

CGroupOffLineRoleEntity::~CGroupOffLineRoleEntity()
{

}

bool CGroupOffLineRoleEntity::IsLoadImageData()
{
    return m_OfflineRoleImageData.m_nRoleID > 0;
}

RoleImageOfflineData * CGroupOffLineRoleEntity::GetOfflineRoleImageData()
{
    return &m_OfflineRoleImageData;
}

void CGroupOffLineRoleEntity::GetOfflineRoleImageData(RoleImageOfflineData & offlineData)
{
    offlineData = m_OfflineRoleImageData;
}


void CGroupOffLineRoleEntity::UpdateOfflineData(const RoleImageOfflineData & offlinedata)
{
    m_OfflineRoleImageData = offlinedata;
}

bool CGroupOffLineRoleEntity::IsLoadOfflineTimeData()
{
    return m_nOfflineTime > 0;
}

unsigned int CGroupOffLineRoleEntity::GetOfflineTimeData()
{
    return m_nOfflineTime;
}

unsigned int * CGroupOffLineRoleEntity::GetOfflineTimePoint()
{
    return &m_nOfflineTime;
}

void CGroupOffLineRoleEntity::UpdateOfflineTimeData(unsigned int nOfflineTime)
{
    m_nOfflineTime = nOfflineTime;
}

CGroupOffLinePlayerManager::CGroupOffLinePlayerManager()
{

}

CGroupOffLinePlayerManager & CGroupOffLinePlayerManager::Instance()
{
    static CGroupOffLinePlayerManager stcMgr;
    return stcMgr;
}

CGroupOffLinePlayerManager::~CGroupOffLinePlayerManager()
{
    std::map<unsigned int, CGroupOffLineRoleEntity*>::iterator it = m_OffLinePlayerMgr.begin();
    for (; it != m_OffLinePlayerMgr.end(); ++it)
    {
        delete it->second;
        it->second = NULL;
    }

    m_OffLinePlayerMgr.clear();
}

CGroupOffLineRoleEntity * CGroupOffLinePlayerManager::GetOfflineRoleImageData(unsigned int nRoleID)
{
    std::map<unsigned int,CGroupOffLineRoleEntity*>::iterator it = m_OffLinePlayerMgr.find( nRoleID );
    if (it != m_OffLinePlayerMgr.end())
    {
        return it->second;
    }

    return NULL;
}

void CGroupOffLinePlayerManager::LoadOfflineRoleImageData(const std::set<unsigned int> & setRoleID)
{
    std::set<unsigned int> * pSetLoadRoleID = new std::set<unsigned int>();
    std::map<unsigned int, RoleImageOfflineData*> * pMapOfflineData = new std::map<unsigned int, RoleImageOfflineData*>();
    std::set<unsigned int>::const_iterator it = setRoleID.begin();
    for (; it != setRoleID.end(); ++it)
    {
        std::map<unsigned int, CGroupOffLineRoleEntity*>::iterator itr = m_OffLinePlayerMgr.find(*it);
        if (itr == m_OffLinePlayerMgr.end() && (*it) != 0)
        {
            pSetLoadRoleID->insert(*it);
            CGroupOffLineRoleEntity * pRoleOfflineData = new CGroupOffLineRoleEntity();
            m_OffLinePlayerMgr[*it] = pRoleOfflineData;

            pMapOfflineData->insert(make_pair(*it, pRoleOfflineData->GetOfflineRoleImageData()));
        }
    }

    if (!pSetLoadRoleID->empty())
    {
        g_pQueryMgr->AddQuery(QUERY_LoadListOfflineRoleData, 0, pSetLoadRoleID, 0, pMapOfflineData);
    }
    else
    {
        delete pMapOfflineData; pMapOfflineData = NULL;
        delete pSetLoadRoleID; pSetLoadRoleID = NULL;
    }
}

void CGroupOffLinePlayerManager::LoadOfflineRoleImageData(unsigned int nRoleID)
{
    if (nRoleID == 0)
    {
        return;
    }

	std::map<unsigned int,CGroupOffLineRoleEntity*>::iterator it = m_OffLinePlayerMgr.find(nRoleID);
	if ( it == m_OffLinePlayerMgr.end() )
	{
		CGroupOffLineRoleEntity * pRoleEntity = new CGroupOffLineRoleEntity();
		m_OffLinePlayerMgr.insert( make_pair(nRoleID, pRoleEntity));

		g_pQueryMgr->AddQuery(QUERY_LoadOfflineRoleData, nRoleID, pRoleEntity->GetOfflineRoleImageData());
	}
    else if (!it->second->IsLoadImageData())
    {
        g_pQueryMgr->AddQuery(QUERY_LoadOfflineRoleData, nRoleID, it->second->GetOfflineRoleImageData());
    }
}

void CGroupOffLinePlayerManager::LoadOfflineTimeData(unsigned int nRoleID)
{
    std::map<unsigned int,CGroupOffLineRoleEntity*>::iterator it = m_OffLinePlayerMgr.find( nRoleID );
    if ( it == m_OffLinePlayerMgr.end() )
    {
        CGroupOffLineRoleEntity * pRoleEntity = new CGroupOffLineRoleEntity();
        m_OffLinePlayerMgr.insert( make_pair(nRoleID, pRoleEntity));
        g_pQueryMgr->AddQuery(QUERY_LoadRoleOfflineTime, nRoleID, pRoleEntity->GetOfflineTimePoint());
    }
    else if (!it->second->IsLoadOfflineTimeData())
    {
        g_pQueryMgr->AddQuery(QUERY_LoadRoleOfflineTime, nRoleID, it->second->GetOfflineTimePoint());
    }
}

unsigned int * CGroupOffLinePlayerManager::GetOfflineTimePoint(unsigned int nRoleID)
{
    std::map<unsigned int,CGroupOffLineRoleEntity*>::iterator it = m_OffLinePlayerMgr.find( nRoleID );
    if ( it != m_OffLinePlayerMgr.end())
    {
        return it->second->GetOfflineTimePoint();
    }
    else
    {
        CGroupOffLineRoleEntity * pRoleEntity = new CGroupOffLineRoleEntity();
        m_OffLinePlayerMgr.insert( make_pair(nRoleID, pRoleEntity));

        return pRoleEntity->GetOfflineTimePoint();
    }
}

void CGroupOffLinePlayerManager::UpdateOfflineTimeData(unsigned int nRoleID, unsigned int nOfflineTime)
{
    std::map<unsigned int,CGroupOffLineRoleEntity*>::iterator it = m_OffLinePlayerMgr.find( nRoleID );
    if ( it != m_OffLinePlayerMgr.end())
    {
        it->second->UpdateOfflineTimeData(nOfflineTime);
    }
}

unsigned int CGroupOffLinePlayerManager::GetOfflineTime(unsigned int nRoleID)
{
    std::map<unsigned int,CGroupOffLineRoleEntity*>::iterator it = m_OffLinePlayerMgr.find( nRoleID );
    if ( it != m_OffLinePlayerMgr.end())
    {
        return it->second->GetOfflineTimeData();
    }

    return 0;
}

/*
//角色下线（把对象从map移除）
void CPlayMgr::OnLogout(GAMEMSG_S2G_Logout * pMsg)
{
	//家族部分处理
	CServerRoleEnty * pRole = SearchRoleByID(pMsg->rolePlayer.nRoleID);
	if (pRole)
	{
		g_FamilyMgr.RoleLogout(pRole->GetRoleID(), pRole);
		g_mapRoleEntity.erase(pMsg->rolePlayer.nRoleID);
	}

	if (SearchRoleByUUID(pMsg->logout_uuid))
	{
		g_mapUUIDRoleEntity.erase(pMsg->logout_uuid);
	}
}

void CPlayMgr::OnRoomStatChange(GAMEMSG_S2G_RoomStatChange * pMsg)
{
	CServerRoleEnty* pPlayer = SearchRoleByID(pMsg->tag.nRoleID);
	if(pPlayer)
	{
		pPlayer->m_nRoomId = pMsg->nRoomId;
	}
}

void CPlayMgr::OnRoleNameChange(GAMEMSG_S2G_RoleNameChange * pMsg)
{
	CServerRoleEnty* pPlayer = SearchRoleByID(pMsg->nRoleId);
	if(pPlayer)
	{
		memset(pPlayer->rolePlayer.szName,0,MAX_NAME_LEN-1);
		memcpy(pPlayer->rolePlayer.szName,pMsg->newUserName,MAX_NAME_LEN-1);

		if (pPlayer->HasFamily())
			g_FamilyMgr.SynchronizeOnlineMemberName(pPlayer->GetRoleID(),pPlayer->GetFamilyId(),pPlayer->rolePlayer.szName);
	}
}
*/
void CGroupPlayerManager::NoticeAllPlayer(char * strContent)
{
	map<unsigned int , CGroupRoleEntity>::iterator it;
	for(it=g_mapRoleEntity.begin();it!=g_mapRoleEntity.end();++it)
	{
		(it->second).SysChat(strContent);
	}
}

void CGroupPlayerManager::SendMsgToAll(GameMsg_Base &msg)
{
    map<unsigned int, CGroupRoleEntity>::iterator it;
    for (it = g_mapRoleEntity.begin(); it != g_mapRoleEntity.end(); ++it)
    {
        it->second.SendPlayerMsg(&msg);
    }
}


/*
void CPlayMgr::OnRoleSexChange(GAMEMSG_S2G_RoleSexChange * pMsg)
{
	CServerRoleEnty* pPlayer = SearchRoleByID(pMsg->nRoleId);
	if(pPlayer)
	{
		pPlayer->m_nSex = pMsg->nSex;
	}
}

void CPlayMgr::OnRoleLevelChange(GAMEMSG_S2G_RoleLevelChange * pMsg)
{
	CServerRoleEnty* pPlayer = SearchRoleByID(pMsg->nRoleId);
	if(pPlayer)
	{
		pPlayer->m_nLevel = pMsg->nLevel;

		if (pPlayer->HasFamily())
		{
			g_FamilyMgr.SynchronizeOnlineMemberLevel(pPlayer->GetRoleID(),pPlayer->GetFamilyId(),pPlayer->m_nLevel);
		}
	}
}

void CPlayMgr::OnRolePreChange(GAMEMSG_S2G_RolePreChange * pMsg)
{
	CServerRoleEnty* pPlayer = SearchRoleByID(pMsg->nRoleId);
	if(pPlayer)
	{
		short nPreIncrease = pMsg->nPre - pPlayer->m_nPre;
		pPlayer->m_nPre = pMsg->nPre;
		//声望减少，只有族员主动退出时才会发生，这时候族员退出流程已经对该家族的该成员声望进行了删除处理，不要要同步啦
		if (nPreIncrease > 0 && pPlayer->HasFamily())
		{
			g_FamilyMgr.SynchronizeOnlineMemberPre(pPlayer->GetRoleID(),pPlayer->GetFamilyId(),nPreIncrease);
		}
	}
}

void CPlayMgr::OnRoleContributeChange(GAMEMSG_S2G_RoleContributeChange * pMsg)
{
	CServerRoleEnty* pPlayer = SearchRoleByID(pMsg->nRoleId);
	if(pPlayer && pPlayer->HasFamily())
		g_FamilyMgr.SynchronizeOnlineMemberContribute(pPlayer->GetRoleID(),pPlayer->GetFamilyId(),pMsg->nContribute);
}

void CPlayMgr::OnRoleMoneyChange(GAMEMSG_S2G_RoleMoneyChange * pMsg)
{
	CServerRoleEnty* pPlayer = SearchRoleByID(pMsg->nRoleId);
	if(pPlayer)
	{
		pPlayer->m_nMoney = pMsg->nMoney;
		if (pPlayer->HasFamily())
		{
			g_FamilyMgr.SynchronizeOnlineMemberMoney(pPlayer->GetRoleID(),pPlayer->GetFamilyId(),pPlayer->m_nMoney);
		}
	}
}
*/
void CGroupPlayerManager::OnGameServerDisconnect(int nLine)
{
	for( map<unsigned int , CGroupRoleEntity>::iterator it = g_mapRoleEntity.begin(); it != g_mapRoleEntity.end(); )
	{
		if( it->second.m_nLine == nLine )
		{
			//g_mapRoleByName.erase( it->second.rolePlayer.szName);
			g_mapAccountIndex.erase(it->second.m_nAccount);
			g_mapRoleEntity.erase(it++);
		}
		else
			++it;
	}
}

CGroupRoleEntity * CGroupPlayerManager::SearchRoleByAccountID(unsigned int nAccountID)
{
	if (g_mapAccountIndex.find(nAccountID) != g_mapAccountIndex.end())
	{
		return this->SearchRoleByID(g_mapAccountIndex.find(nAccountID)->second);
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CGroupRoleEntity::CGroupRoleEntity()
{
    m_nSession = 0;
    m_nAccount = 0;
    m_nRoleID = 0;
    memset(m_szRoleName, 0, MAX_NAME_LEN);
    m_nSex = ESexType_Male;
    m_nIndex = 0;
    m_nLevel = 1;
    m_nMoney = 0;
    m_nStarCount = 0;
    m_bIsVIP = false;
    m_nVIPLevel = 0;
    m_nLine = -1;
    m_nRoomID = 0;
    m_nRoleRoomType = RoleRoomType_None;
    m_nRoleRoomState = RoleRoomState_None;
    m_nRoleLocation = Zone_None;
    m_bTodayFirstLogin = 0;
    m_nAge = 0;
    m_nConstellation = 0;
    m_nLoginTimeOnGroup = 0;
    m_nTopParcloseID = 0;
    m_nDivorceAgreementOtherRoleID = 0;
    m_nDanceGroupID = 0;
    m_fX = 0.0f;
    m_fY = 0.0f;
    m_nTransFormID = 0;

    m_nGroupRoomID = 0;
    m_eGroupRoomType = eRoomType_None;
}

CGroupRoleEntity::~CGroupRoleEntity()
{
}

bool  CGroupRoleEntity::ModifyMoney(int nValue,int nCauseType,int nAffixData)	
{
	if(nValue==0)
		return true;

	SetMoney(m_nMoney+nValue);
	return true;
}

void CGroupRoleEntity::SendPlayerMsg(GameMsg_Base *pMsg)
{
	if (pMsg != NULL)
	{
		pMsg->nIndex = m_nIndex;
		pMsg->SessionID = m_nSession;
		pMsg->nAccount = m_nAccount;
		::SendMsg2GameServer(pMsg, m_nLine);
	}
}

void CGroupRoleEntity::SendPlayerMsgByAccountID(GameMsg_Base &msg, unsigned int nAccount)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByAccountID(nAccount);
    if (NULL != pGroupRole)
        pGroupRole->SendPlayerMsg(&msg);
}

void CGroupRoleEntity::SendPlayerMsgByRoleID(GameMsg_Base &msg, unsigned int nRoleID)
{
    CGroupRoleEntity *pGroupRole = CGroupPlayerManager::Instance().SearchRoleByID(nRoleID);
    if (NULL != pGroupRole)
        pGroupRole->SendPlayerMsg(&msg);
}


//发送系统聊天框消息
void CGroupRoleEntity::SysChat(const char* szFormat, ...)
{ 
	char buffer[MAX_CHAT_BUFF_SIZE];
	va_list args;

	va_start(args, szFormat);
	_vsnprintf(buffer, sizeof(buffer), szFormat, args);//int size = 
	va_end(args);

	buffer[MAX_CHAT_BUFF_SIZE - 1] = 0;
	
	//发送系统消息
	GameMsg_G2S_Chat sendMsg(0, 0, eChatSystem, CHAT_DEFAULT_TEXT_COLOR, 0, m_nRoleID, 
							 NULL, NULL, buffer, 0, EChatExtraType_Default,
							 false, 0, false, 0);

	SendPlayerMsg(&sendMsg);
}

/*
bool CGroupRoleEntity::AddUpdateMsg(GameMsg_Base*pMsg,bool bNotifySelf,bool bNotifyOther)
{
	int  nSize = pMsg->nSize + sizeof(GAMEMSG_Family_Group2Server) - 1;
	if( nSize >= MAX_PACKET_SIZE )
		return false;
	GAMEMSG_Family_Group2Server  * pG2SMsg = new (nSize) GAMEMSG_Family_Group2Server;
	if (!pG2SMsg)
		return false;

	pG2SMsg->pData = NULL;
	pG2SMsg->pGSMsg = (LPGameMsg_Base)(pG2SMsg->pBuff);
	pG2SMsg->nRoleID = GetRoleID();
	pG2SMsg->nSize = (unsigned short)nSize;
	pG2SMsg->nIndex = m_nIndex;

	memcpy( (void*)(pG2SMsg->pBuff), (void*)pMsg, pMsg->nSize);
	//发送消息给Game Server
	SendMsgToGameServer( pG2SMsg,  m_nLine);
	//delete  pMsg;//把原来的消息释放
	delete pG2SMsg, pG2SMsg=NULL;
	return  true;
}
*/
bool CGroupRoleEntity::HasFriend(unsigned int nRoleID)
{
	return (!m_listFriend.empty() && (m_listFriend.end()!=find(m_listFriend.begin(), m_listFriend.end(), nRoleID)));
}

bool CGroupRoleEntity::HasBlackList(unsigned int nRoleID)
{
	return (!m_listBlackList.empty() && (m_listBlackList.end()!=find(m_listBlackList.begin(), m_listBlackList.end(), nRoleID)));
}

PLAYER_TAG CGroupRoleEntity::GetPlayer_Tag()
{
	return PLAYER_TAG(GetRoleID(), m_nIndex);
}

void CGroupRoleEntity::AddFriend(unsigned int nRoleID)
{
	list<unsigned int>::iterator it;
	it = find(m_listFriend.begin(), m_listFriend.end(), nRoleID);
	if (it == m_listFriend.end())
	{
		m_listFriend.push_back(nRoleID);
	}

	// 更新副本好友排行榜
	CGroupRoleEntity * pFriendEntity = CGroupPlayerManager::Instance().SearchRoleByID( nRoleID );
	if ( pFriendEntity != NULL )
	{
		DungeonRankInfo rankInfo;
		rankInfo.nRoleID = nRoleID;
		rankInfo.nStarCount = pFriendEntity->GetStarCount();
		rankInfo.strRoleName = pFriendEntity->GetName();
		rankInfo.nParcloseID = pFriendEntity->GetTopParcloseID();
		CDungeonRankProcessor::Instance().AddFriend( m_nRoleID, nRoleID, rankInfo );
	}
}

void CGroupRoleEntity::RemoveFriend(unsigned int nRoleID)
{
	list<unsigned int>::iterator it;
	it = find(m_listFriend.begin(), m_listFriend.end(), nRoleID);
	if (it != m_listFriend.end())
	{
		m_listFriend.erase(it);
	}

	CDungeonRankProcessor::Instance().DelFriend( m_nRoleID, nRoleID );
}

void CGroupRoleEntity::AddBlackList(unsigned int nRoleID)
{
	list<unsigned int>::iterator it;
	it = find(m_listBlackList.begin(), m_listBlackList.end(), nRoleID);
	if (it == m_listBlackList.end())
	{
		m_listBlackList.push_back(nRoleID);
	}
}

void CGroupRoleEntity::RemoveBlackList(unsigned int nRoleID)
{
	list<unsigned int>::iterator it;
	it = find(m_listBlackList.begin(), m_listBlackList.end(), nRoleID);
	if (it != m_listBlackList.end())
	{
		m_listBlackList.erase(it);
	}
}

bool CGroupRoleEntity::IsInBlackList(unsigned int nFriendID)
{
	return  m_listBlackList.end() != find(m_listBlackList.begin(), m_listBlackList.end(), nFriendID);
}

void CGroupRoleEntity::SetIsVIP(bool bIsVIP)
{
	m_bIsVIP = bIsVIP;
}

bool CGroupRoleEntity::IsVIP() const
{
	return m_bIsVIP;
}

void CGroupRoleEntity::SetVIPLevel(unsigned short nVIPLevel)
{
	m_nVIPLevel = nVIPLevel;
}

unsigned short CGroupRoleEntity::VIPLevel() const
{
	return m_nVIPLevel;
}

void CGroupRoleEntity::SetPairID(unsigned int nPairID)
{
	m_coupleInfo.m_nPairID = nPairID;
}

unsigned int CGroupRoleEntity::PairID()
{
	return m_coupleInfo.m_nPairID;
}

void CGroupRoleEntity::SetCoupleID(unsigned int nCoupleID)
{
	m_coupleInfo.m_nCoupleID = nCoupleID;
}

unsigned int CGroupRoleEntity::CoupleID()
{
	return m_coupleInfo.m_nCoupleID;
}

void CGroupRoleEntity::SetCoupleDate(unsigned int nCoupleDay)
{
	m_coupleInfo.m_nCoupleDate = nCoupleDay;
}

unsigned int CGroupRoleEntity::CoupleDate()
{
	return m_coupleInfo.m_nCoupleDate;
}

void CGroupRoleEntity::SetMarriageDate(unsigned int nMarriageDate)
{
	m_coupleInfo.m_nMarriageDate = nMarriageDate;
}

unsigned int CGroupRoleEntity::MarriageDate()
{
	return m_coupleInfo.m_nMarriageDate;
}

void CGroupRoleEntity::SetCoupleIntimacy(int nIntimacyInc)
{
	if (m_coupleInfo.m_nPairID > 0 && nIntimacyInc != 0) {
		__int64 nResult = m_coupleInfo.GetIntimacy() + nIntimacyInc;
		if (nResult > 0) {
			nResult = nResult > MAX_COUPLE_INTIMACY ? MAX_COUPLE_INTIMACY : nResult;
		} else {
			nResult = 0;
		}

		m_coupleInfo.SetIntimacy( (int)nResult );
	}
}



void CGroupRoleEntity::SetCoupleName(std::string & strName)
{
	m_coupleInfo.m_strCoupleName = strName;
}

void CGroupRoleEntity::ResetCoupleInfo()
{
	m_coupleInfo.m_nPairID = 0;
	m_coupleInfo.m_nCoupleID = 0;		
	m_coupleInfo.m_strCoupleName = "";
	m_coupleInfo.SetIntimacy(0);
	m_coupleInfo.m_nCoupleDate = 0;
	m_coupleInfo.m_nMarriageDate = 0;
}


void CGroupRoleEntity::UpdateRoleInfo(CWorldRoleUpdateAttrInfo &updateInfo)
{
	memcpy(m_szRoleName, updateInfo.m_szRoleName, MAX_NAME_LEN - 1);
	m_nLevel = updateInfo.m_nLevel;
	m_nMoney = updateInfo.m_nMoney;
}

void CGroupRoleEntity::UpdateRoomInfo(CWorldRoleUpdateRoomInfo &updateInfo)
{
	m_nRoomID = updateInfo.m_nRoomID;
	m_nRoleRoomType = updateInfo.m_nRoleRoomType;
	m_nRoleRoomState = updateInfo.m_nRoleRoomState;
}

ERoomType CGroupRoleEntity::GetGroupRoomType()
{
    return m_eGroupRoomType;
}
unsigned int CGroupRoleEntity::GetGroupRoomID()
{
    return m_nGroupRoomID;
}
void CGroupRoleEntity::ClearGroupRoom()
{
    m_nGroupRoomID = 0;
    m_eGroupRoomType = eRoomType_None;
}

void CGroupRoleEntity::SetNewGroupRoom(ERoomType roomType, unsigned int nRoomID)
{
    m_nGroupRoomID = nRoomID;
    m_eGroupRoomType = roomType;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
bool  SendCommDialogMessage(CGroupRoleEntity*  pRole, const char* szText)
{
	/*
	GAMEMSG_G2S_Comm_Dialog *pMsg= new  GAMEMSG_G2S_Comm_Dialog;
	if(!pMsg)
		return false;

	memcpy(pMsg->text,  szText, strlen(szText));
	pRole->AddUpdateMsg(pMsg,true,false);
	delete pMsg;
	*/

	return  true;
}
/*
bool  SendChatWindowMessage(CGroupRoleEntity*  pRole, const char* szText)
{
	GAMEMSG_Chat_ChatWindow * pMsg= new GAMEMSG_G2S_ChatWindow;
	if(!pMsg)
		return false;

	memcpy(pMsg->text,  szText, strlen(szText) >= sizeof(pMsg->text) ? sizeof(pMsg->text) -1 : strlen(szText) );
	pRole->AddUpdateMsg(pMsg, true, false);
	delete pMsg;
	return  true;
}
*/
void DirectMsgToClient(GameMsg_Base* pMsg, const PLAYER_TAG& tagRole, int nLine)
{
	/*
	GAMEMSG_G2S_ToClient* pDirectMsg;
	unsigned short nSize = sizeof(GAMEMSG_G2S_ToClient) + pMsg->nSize - 1;
	pDirectMsg = new (nSize) GAMEMSG_G2S_ToClient;
	pDirectMsg->nSize = nSize;

	memcpy(pDirectMsg->pBuff, pMsg, pMsg->nSize);
	pDirectMsg->tagRole = tagRole;
	SendMsgToGameServer(pDirectMsg, nLine);
	delete pDirectMsg;
	*/
}

void DirectMsgToClient(GameMsg_Base* pMsg, CGroupRoleEntity* pRole)
{
	DirectMsgToClient(pMsg, pRole->GetPlayer_Tag(), pRole->m_nLine);
}







//file end

