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
	// ��ȡˮ������
	unsigned int getCostCrystal(){ return m_ParcloseConfig.m_nCostCrystal; }

	// �Ƿ�BOSS�ؿ�
	bool isBossParclose(){ return m_ParcloseConfig.IsBossParclose(); }

    // ��ȡcomponent
	CEntityDungeonComponent * GetComponent( unsigned int nRoleID );
	CEntityDungeonComponent * GetComponentByPos( int pos );
	int GetDancerScoreByPos( int pos );
    int GetAllDancerScore();
	void SetDancerScoreByPos( int pos,  int nScore );
	void SetDancerNameByPos( int pos, std::string & name );
	std::string GetDancerNameByPos( int pos );

	// ��ȡ/���÷���ID
	unsigned int GetHost(){ return m_DancerInfo.szDancerID[DUNGEON_HOST_POS]; }
	void SetHost( unsigned int nRoleID ){ m_DancerInfo.szDancerID[DUNGEON_HOST_POS] = nRoleID; }
    bool IsHost(unsigned int nRoleID);

	// ���÷���component
	void SetHostComponent( CEntityDungeonComponent* pRoleComponent );

	// ����/��ȡ����ID
	unsigned int GetFriendID(){ return m_DancerInfo.szDancerID[DUNGEON_FRIEND_POS]; }
	void SetFriendID( unsigned int nRoleID ){ m_DancerInfo.szDancerID[DUNGEON_FRIEND_POS] = nRoleID; }

	// ��ȡ��Ҹ���
	unsigned int GetDanceCount();

	// ���ú���component
	void SetFriendComponent( CEntityDungeonComponent* pRoleComponent );

    // �㲥��Ϣ
    void SendMsgToAll( GameMsg_Base* pMsg , unsigned int nRoleID = 0 );

	// ��ȡ�ؿ�ID
	unsigned int GetParcloseID(){ return m_ParcloseConfig.m_nParcloseID; }

	// �ؿ���Ա��ϸ��Ϣ
	void StartParclose();
    void DoEncodePlayerInfo(CEntityDungeonComponent * pRoleComonent, DungeonPlayerInfo & playerInfo);
    void DoEncodeBossInfo(unsigned int nBossID, DungeonPlayerInfo & bossInfo);
    void DoEncodeStage(std::string & strCheckKey, std::vector<char> & vecStage);

	// �ؿ�����
	void ParcloseEnd();

    // �ؿ�����
    void CalcResult();

	// �뿪����
	void QuitRoom( unsigned int nRoleID, bool bOut = false );

	// �Ƿ�ȫ���뿪
	bool IsAllDanceOut();

	// ��ȡ����λ��
	int GetPos( unsigned int nRoleID );

	// ��ȡBOSS����
	int GetBossCount();

	// ��ȡboss����
	int GetBossScore( unsigned int nBossID, int nRound );

	// ��ȡBOSS ID
	unsigned int  GetBossID( int nIndex );

	// ����boss����
	void LoadBossInfo( unsigned int nStageCount );

	// ����ͽ�ɫID��ͬ��BOSSID
	unsigned int DealWithBossID( unsigned int nBossID );

	// �Ƿ�ȫ�����������
	bool IsReadyEnd();

    // ��ȡ��������
    EDungeonType GetDungeonType(){ return m_DungeonType;}

    // ��ȡ����ID
    unsigned int GetDungeonID(){ return m_nDungeonID;}

    // 
    void SyncRoomEndScore( std::vector<CRoomEndPlayerScoreInfo> & playerScore );

private:
	unsigned int			   m_szRoomBossID[MAX_DUNGEON_BOSS_COUNT];          // bossID
	std::map<unsigned int,mapTable>   m_BossScore;                              // boss score       
	eDungeonState			   m_eDungeonState;									// ����״̬									
	CParcloseConfig            m_ParcloseConfig;							    // �ؿ�������Ϣ
	ParcloseRoomDancer         m_DancerInfo;
    EDungeonType               m_DungeonType;
    unsigned int               m_nDungeonID;     
};


#endif
