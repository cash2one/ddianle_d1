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
	// ע����Ϣ
	void _RegisterMessage();

	// ע����Ϣ����
	void _RegisterMessageProcessor();

	// ��ʼ��
	void Init();

	// ��ɫ��¼
	void PlayerLogin( CGroupRoleEntity & role );

	// ��ɫ�ǳ�
	void PlayerLogout( CGroupRoleEntity & role );

	// ����������Ϣ
	void OnUpdateParcloseInfo( GameMsg_Base &msg, CSlotPeer & slotPeer );

	// ɾ������
	void DelFriend( unsigned int nRoleID, unsigned int nFriendID );

	// ��Ӻ���
	void AddFriend( unsigned int nRoleID, unsigned int nFriendID, DungeonRankInfo & rankInfo );

	// ��������
	void ClearRank();

	// ��ȡ�����б�
	void OnGetRankList(GameMsg_Base &msg, CSlotPeer & slotPeer);

	// ��ȡ����
	int GetRank( unsigned int nRoleID );
	
	// �����������
	void OnLoadFriendDungeonInfo(QueryBase & rQuery);

	//����ҷ�����Ϣ
	void _SendPlayerMsg(unsigned int nRoleID, GameMsg_Base & rMsg);

    // ����������Ϣ
    void OnResetDungeonInviteFrienInfo( GameMsg_Base &msg, CSlotPeer & slotPeer );

    // ���������Ϣ
    void OnAddDungeonInviteFriendInfo(GameMsg_Base &msg, CSlotPeer & slotPeer);

    // �Ƴ����������Ϣ
	void OnRemoveDungeonInviteInfo(GameMsg_Base &msg, CSlotPeer & slotPeer);

    // ��������������
    void OnGiveVitToFriend(GameMsg_Base &msg, CSlotPeer & slotPeer);

private:
	std::map<unsigned int,CDungeonRankMgr*>	m_DungeonRankMgr;
    std::map<unsigned int,CDungeonInviteInfo>   m_mapDungeonInviteInfo;

};


#endif

