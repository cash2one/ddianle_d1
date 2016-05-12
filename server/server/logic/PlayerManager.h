/********************************************************************
created:	2010/12/06
filename: 	server\logic\PlayerManager.h
author:		meijiuhua

purpose:	server端所有player的管理类

m_ppPlayer[0]始终是空，不能使用
*********************************************************************/

#ifndef __CPlayerManager_H__
#define __CPlayerManager_H__

#include "SrvManagerBase.h"

#include "../share/ServerDataStruct.h"
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/LogStructDef.h"
#include <list>

class CServerRoleEntity;
struct ROLEINFO_DB;

struct PlayerWaitLogin
{
    unsigned int nRoleID;
    PLAYERTAG tagPlayer;
};

#define MAXPLAYERS 2000		//单线最大玩家数量

class CPlayerManager : public CSrvManagerBase
{
protected:
    CPlayerManager();
    virtual ~CPlayerManager();

public:
    static CPlayerManager& Instance();

    void OnConnectedToWorld();// 连接上world server

    bool Create();
    void SendGlobalMessage(GameMsg_Base*pMsg);
    void SendGlobalPlatformMessage(unsigned short nPID, GameMsg_Base*pMsg);

    void LogoutPlayer(const PLAYERTAG* pTag, bool bIsSwitchServer = false);
    void LogoutPlayer(CRoleEntity* pPlayer, bool bIsSwitchServer = false);

    CRoleEntity* GetPlayer(const PLAYER_TAG& tagRole) const;
    CRoleEntity* GetPlayer(int iEntity, unsigned int nRoleID)const;
    CRoleEntity* GetPlayer(int iEntity)const;
    CRoleEntity* GetPlayerByAccount(unsigned int nAccount)const;

    CRoleEntity* GetEntityByRoleID(unsigned int nRoleID);

    bool NewPlayer(ROLEINFO_DB* pRoleInfoDB, PLAYERTAG* pTag, bool bSwitchLine);
    void NoticeGroupServerPlayerEnter(CRoleEntity* pNewPlayer);
    virtual void OnUpdate(const unsigned long &lTimeElapsed);

    int CountPlayer();

    void GetAllPlayers(list<CRoleEntity *> &listPlayer)const;
    void GetRoleIDList(std::list< unsigned int > & rRoleIDList) const;

    void OnActivityChange(unsigned char nActivityID, bool bOpen);

    void OnMallReload();

protected:
    void SavePlayer(CRoleEntity*pPlayer, bool bIsSwitchServer = false);
    void CachePlayer(CRoleEntity *pPlayer);
    int AddEntity(CEntity* pNewEntity);

    void AddEntityToMap(unsigned int nRoleID, int iEntity);
    void DeleteEntityFromMap(unsigned int nRoleID);
    int  GetPlayerIndexByRoleID(unsigned int nRoleID);

private:
    void _LogPlayerAction(CPlayerActionLog::EPlayerAction ePlayerAction, unsigned int nAccount, unsigned int nRoleID, const char * szRoleName, int nMoney, unsigned int nCash);
    void LogPlayerSwicthLine(CPlayerSwitchLineLog::EPlayerAction eAction, unsigned int nRoleID, int nLine);

protected:
    typedef map<unsigned int, int> tmapEntity;
    tmapEntity	m_mapEntityIndex;
    int m_nSetExpTimeDelay;
};

#endif	//__CPlayerManager_H__

