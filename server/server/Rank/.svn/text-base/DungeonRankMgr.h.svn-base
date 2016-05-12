#ifndef __DUNGEONRANKMGR_H__
#define __DUNGEONRANKMGR_H__

#include <map>
#include "../group/Global.h"
#include "RankMgr.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../../socket/GameMsg_Processor.h"

#define PARCLOSEID_MAGICSTAR_RATE 1000000

class CDungeonRankInfo : public CRankInfo
{
public:
	CDungeonRankInfo( DungeonRankInfo & rankinfo );
	virtual ~CDungeonRankInfo();

	DungeonRankInfo  m_DungeonRankInfo;
};


class CDungeonRankMgr : public CRankMgr
{
public:
	CDungeonRankMgr();
	virtual ~CDungeonRankMgr();

public:
	void LoadRank( unsigned int nRoleID );
	void SetLoadComplete( bool bIsTrue );
	bool GetIsLoadComplete(){ return m_bLoadComplete; }
	void UpdateDungeonInfo( unsigned int nRoleID, const std::string & strName, unsigned int nParcloseID, unsigned int nStarCount );
	void RemoveDungeonInfo( unsigned int nRoleID );
	void AddFriend( unsigned int nRoleID, DungeonRankInfo & rankInfo );
	void GetRankList( std::list<CDungeonRankInfo*> & rankList);
	unsigned int  GetSelfRank( unsigned int nRoleID );

public:
	bool	m_bLoadComplete;
	//std::map<unsigned int,DungeonRankInfo> m_RankPlayers;
	std::vector<DungeonRankInfo> m_RankPlayers;

};

class CDungeonRankProcessor : public GameMsg_Processor
{
public:
	CDungeonRankProcessor();
	virtual ~CDungeonRankProcessor();

	static CDungeonRankProcessor & Instance();

public:
	// 注册消息
	void _RegisterMessage();

	// 注册消息处理
	void _RegisterMessageProcessor();

	// 初始化
	void Init();

	// 角色登录
	void PlayerLogin( CGroupRoleEntity & role );

	// 角色登出
	void PlayerLogout( CGroupRoleEntity & role );

	// 更新排行信息
	void OnUpdateParcloseInfo( GameMsg_Base &msg, CSlotPeer & slotPeer );

	// 删除好友
	void DelFriend( unsigned int nRoleID, unsigned int nFriendID );

	// 添加好友
	void AddFriend( unsigned int nRoleID, unsigned int nFriendID, DungeonRankInfo & rankInfo );

	// 清理排行
	void ClearRank();

	// 获取排行列表
	void OnGetRankList(GameMsg_Base &msg, CSlotPeer & slotPeer);

	// 获取排名
	int GetRank( unsigned int nRoleID );
	
	// 加载排行完成
	void OnLoadFriendDungeonInfo(QueryBase & rQuery);

	//向玩家发送消息
	void _SendPlayerMsg(unsigned int nRoleID, GameMsg_Base & rMsg);

    // 重置邀请信息
    void OnResetDungeonInviteFrienInfo( GameMsg_Base &msg, CSlotPeer & slotPeer );

    // 添加邀请信息
    void OnAddDungeonInviteFriendInfo(GameMsg_Base &msg, CSlotPeer & slotPeer);

    // 移除邀请玩家信息
	void OnRemoveDungeonInviteInfo(GameMsg_Base &msg, CSlotPeer & slotPeer);

    // 给好友赠送体力
    void OnGiveVitToFriend(GameMsg_Base &msg, CSlotPeer & slotPeer);

private:
	std::map<unsigned int,CDungeonRankMgr*>	m_DungeonRankMgr;
    std::map<unsigned int,CDungeonInviteInfo>   m_mapDungeonInviteInfo;

};


#endif

