#ifndef __DUNGEON_H__
#define __DUNGEON_H__

#include "DungeonDefine.h"
#include "../room/Room.h"
#include "EntityDungeonComponent.h"
#include "../../datastructure/datainfor/MagicLampConfig.h"

typedef std::map<int,int> mapTable;
class DungeonPlayerInfo;

class ParcloseRoomDancer
{
public:
	unsigned int szDancerID[MAX_DUNGEON_PLAYER_COUNT];
	int	szDancerScore[MAX_DUNGEON_PLAYER_COUNT];
	std::string szDancerName[MAX_DUNGEON_PLAYER_COUNT];
	CEntityDungeonComponent * szDancerComponent[MAX_DUNGEON_PLAYER_COUNT];

	ParcloseRoomDancer()
	{
		for ( int i = 0; i < MAX_DUNGEON_PLAYER_COUNT; ++i )
		{
			szDancerID[i] = 0;
			szDancerScore[i] = -1;
			szDancerComponent[i] = NULL;
		}
	}
};


class CDungeon : public CRoom
{
public:
	CDungeon( unsigned int nRoomID, EDungeonType eType, unsigned int nDungeonID, const CParcloseConfig & parclose );
	virtual ~CDungeon(){};

public:
	void Initialize();

    //
    void RoomPrepare();

public:
	// 获取水晶消耗
	unsigned int getCostCrystal(){ return m_ParcloseConfig.m_nCostCrystal; }

	// 是否BOSS关卡
	bool isBossParclose(){ return m_ParcloseConfig.IsBossParclose(); }

    // 获取component
	CEntityDungeonComponent * GetComponent( unsigned int nRoleID );
	CEntityDungeonComponent * GetComponentByPos( int pos );
	int GetDancerScoreByPos( int pos );
    int GetAllDancerScore();
	void SetDancerScoreByPos( int pos,  int nScore );
	void SetDancerNameByPos( int pos, std::string & name );
	std::string GetDancerNameByPos( int pos );

	// 获取/设置房主ID
	unsigned int GetHost(){ return m_DancerInfo.szDancerID[DUNGEON_HOST_POS]; }
	void SetHost( unsigned int nRoleID ){ m_DancerInfo.szDancerID[DUNGEON_HOST_POS] = nRoleID; }
    bool IsHost(unsigned int nRoleID);

	// 设置房主component
	void SetHostComponent( CEntityDungeonComponent* pRoleComponent );

	// 设置/获取好友ID
	unsigned int GetFriendID(){ return m_DancerInfo.szDancerID[DUNGEON_FRIEND_POS]; }
	void SetFriendID( unsigned int nRoleID ){ m_DancerInfo.szDancerID[DUNGEON_FRIEND_POS] = nRoleID; }

	// 获取玩家个数
	unsigned int GetDanceCount();

	// 设置好友component
	void SetFriendComponent( CEntityDungeonComponent* pRoleComponent );

    // 广播消息
    void SendMsgToAll( GameMsg_Base* pMsg , unsigned int nRoleID = 0 );

	// 获取关卡ID
	unsigned int GetParcloseID(){ return m_ParcloseConfig.m_nParcloseID; }

	// 关卡队员详细信息
	void StartParclose();
    void DoEncodePlayerInfo(CEntityDungeonComponent * pRoleComonent, DungeonPlayerInfo & playerInfo);
    void DoEncodeBossInfo(unsigned int nBossID, DungeonPlayerInfo & bossInfo);
    void DoEncodeStage(std::string & strCheckKey, std::vector<char> & vecStage);

	// 关卡结束
	void ParcloseEnd();

    // 关卡结算
    void CalcResult();

	// 离开房间
	void QuitRoom( unsigned int nRoleID, bool bOut = false );

	// 是否全部离开
	bool IsAllDanceOut();

	// 获取房间位置
	int GetPos( unsigned int nRoleID );

	// 获取BOSS个数
	int GetBossCount();

	// 获取boss分数
	int GetBossScore( unsigned int nBossID, int nRound );

	// 获取BOSS ID
	unsigned int  GetBossID( int nIndex );

	// 加载boss配置
	void LoadBossInfo( unsigned int nStageCount );

	// 处理和角色ID相同的BOSSID
	unsigned int DealWithBossID( unsigned int nBossID );

	// 是否全部都跳舞完毕
	bool IsReadyEnd();

    // 获取副本类型
    EDungeonType GetDungeonType(){ return m_DungeonType;}

    // 获取副本ID
    unsigned int GetDungeonID(){ return m_nDungeonID;}

    // 
    void SyncRoomEndScore( std::vector<CRoomEndPlayerScoreInfo> & playerScore );

private:
	unsigned int			   m_szRoomBossID[MAX_DUNGEON_BOSS_COUNT];          // bossID
	std::map<unsigned int,mapTable>   m_BossScore;                              // boss score       
	eDungeonState			   m_eDungeonState;									// 副本状态									
	CParcloseConfig            m_ParcloseConfig;							    // 关卡配置信息
	ParcloseRoomDancer         m_DancerInfo;
    EDungeonType               m_DungeonType;
    unsigned int               m_nDungeonID;     
};


#endif
