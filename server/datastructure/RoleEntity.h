/********************************************************************
	created:	2010/12/02
	filename: 	DataStructure\biologicEntity.h
	author:		meijiuhua 
	
	purpose:	player,monster(����npc)������ʵ��,������������ֵ
*********************************************************************/

#ifndef __CRoleEntity_H__
#define __CRoleEntity_H__

#include "Entity.h"
#include "datainfor/ItemType.h"
#include "../nsytworld/QuestDoc.h"
#include "DataStruct_DB.h"

#include <list>
using namespace std;

class GameMsg_S2C_CreateRoleSuc;
class CWeddingRoom;
class CEntityCoupleComponent;
class CPhotoRoom;
class CAmuseRoom;
class CEntityItemComponent;
class CEntityGeneComponent;
class CEntityDungeonComponent;
class CRoomComponent;
class CEntityAttributeComponent;
class CeremonyRoom;
class CRoom;
class RoleRoomImageData;

enum ELwRoom
{
    ELwRoom_Out = 0,                    // �����κη���
    ELwRoom_DanceRoom = 1000,           // 
    ELwRoom_CoupleRoom,
    ELwRoom_PhotoRoom,
    ELwRoom_AmuseRoom,
    ELwRoom_CeremonyRoom,
    ELwRoom_GroupFailyRoom,
};

class CRoleEntity : 
	public CEntity
{
	friend class CLogPlayerAllNormal;
public:
	int  m_nSaveTime;
    unsigned int m_nRandomSaveCycle;
	bool m_bOffLine;
	time_t m_tLBSUpPos;
	time_t m_tLBSGetNearby;
	bool m_bUpdatedLBSPositon;
public:
	CRoleEntity(void);
	virtual ~CRoleEntity(void);
	
	//�洢����
	void Save(){m_nSaveTime = -1;};

	bool SetOffLine(bool bOffLine)//�����Ƿ�ɹ�
	{		
		ASSERT(bOffLine || IsValid());
		if(bOffLine || IsValid())
		{
			m_bOffLine = bOffLine;
			if(m_bOffLine)
			{
				m_nSaveTime = 0;
			}
			return true;
		}

		return false;
	}

public:
	virtual void EntityUpdate(const unsigned long & nTimeElapsed);

	unsigned short PID() const;
	virtual unsigned int GetAccount() const;
	virtual unsigned int GetRoleID() const;
	virtual const char * GetRoleName() const;
	virtual int GetMoney() const;
	//��ȡM��
	unsigned int GetPoint() const;
	unsigned int GetBindPoint() const;
	unsigned int GetAvailablePoint() const;
	virtual unsigned char GetSex() const;
	virtual unsigned short GetLevel() const;
	virtual unsigned char GetColor() const;
	virtual unsigned int GetHot() const;
	virtual unsigned int GetFriendCount() const;
	virtual bool IsVIP() const;
	virtual int GetVIPLevel()const;
	CEntityItemComponent * GetItemComponent();

	// ��ȡ������
	virtual unsigned int GetLuckStar() const;

	virtual const PLAYERTAG* GetPlayerTag()const;

	virtual unsigned int GetRoomID() const;
	//virtual unsigned int GetLastLoginTIme()const;
	virtual bool GetTodayFirstLoginTag() const;
	virtual void SendPlayerMsg(GameMsg_Base *pMsg) const;
	virtual void SendMsg2GroupServer(GameMsg_Base *pMsg);
	virtual void SysChat(const char * szFormat, ...);

	virtual __int64 GetSessionID() const;
	virtual unsigned short GetIndex() const;
	virtual unsigned int GetEntityID() const;

	void SetLocation(int nLocation);
	int GetLocation() const;
	void ChangeMoney(int nValue, EChangeMoneyCause eChangeMoneyCause, int nAffixData);

	bool IsInBlackList(unsigned int nRoleID);

	bool IsSingle();
	bool IsMarried();

	void SetPairID(unsigned int nPairID);
	unsigned int PairID();

	void SetCoupleID(unsigned int nCoupleID);
	unsigned int CoupleID();
	CEntityCoupleComponent * GetRoleCouple();

	void SetCoupleBlessingValue( unsigned int nBlessingValue );

	void SetCoupleName(const std::string & strName);
    void SetCoupleSmallRoleID( unsigned int nRoleID );

	void SetCoupleDate(unsigned int nCoupleDate);
	unsigned int CoupleDate();

	void SetMarriageDate(unsigned int nMarriageDate);
	unsigned int MarriageDate();

	void SetTransformIDAndNotify(unsigned int nId);
	unsigned int TransformID();

    void PackRoleRoomImageData(RoleRoomImageData &imageData);

	CWeddingRoom * WeddingRoom();
	CPhotoRoom * PhotoRoom();
	CAmuseRoom * AmuseRoom();
    CeremonyRoom * GetCeremonyRoom();
    CRoom * GetRoom();
    bool InBigMamaRoom();
    bool InChanllengeRoom();
    bool InGroupRoom();

    ELwRoom RoomType();
    bool InSomeRoom();

	void ResetCoupleData();

	void SetLastInvitedPhotoRoomPlayerTime(int nLastInvitedPlayerTime);
	int LastInvitedPhotoRoomPlayerTime();

	bool TryBeInvitedToPhotoRoom(unsigned int nRoomID, int nCreatedTime);

	// ��ȡ�����б�
	void GetFriendList( std::vector<unsigned int> & friendlist );

	CEntityItemComponent * GetRoleItem();
	CEntityGeneComponent * GetRoleGene();
	CEntityDungeonComponent * GetRoleDungeon();
	CRoomComponent * GetRoleRoom();
	CEntityAttributeComponent * GetRoleAttr();

    unsigned int GetDanceGroupID() const;
    void GetDanceGroupInfo( unsigned int nRoleID, std::string & strGroupName, char & nGroupTitle, unsigned short & nBadge, unsigned short & nEffect );

public:
	bool PacketAndSendCreateRoleMsg();
	bool IsInRoomHall();

	void ReadToRun(int nTargetIndex);

    // ����
    void CacheEncode(ROLEINFO_DB *roleCache);
    static void CacheDecode(ROLEINFO_DB *roleCache, unsigned short key, char* data, int dataSize);
};

#endif //__CRoleEntity_H__



//end file


