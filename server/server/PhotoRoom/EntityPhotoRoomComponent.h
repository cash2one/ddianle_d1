#ifndef		__ENTITYPHOTOROOMCOMPONENT_H__
#define		__ENTITYPHOTOROOMCOMPONENT_H__

#include <map>

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "PhotoRoomDataStruct.h"
#include "PhotoRoomMacroDefine.h"

class CEntityAttributeComponent;
class CRoomComponent;
class CEntityItemComponent;
class CEntityCoupleComponent;
class CEntityNetComponent;
class CPhotoRoom;
class CEntityGeneComponent;
class CEntityDanceGroupComponent;
class CPhotoRoomPlayerInfo;
class CEntityFriendComponent;
class CEntityChatComponent;
class CEntityVIPComponent;
class CAmuseRoomComponent;
class PetComponent;
class CEntityMedalComponent;

class CEntityPhotoRoomComponent : 
	public CEntityComponent
{
public:
	CEntityPhotoRoomComponent();
	~CEntityPhotoRoomComponent();
public:
	virtual void Start();
	virtual void OnEvent( CComponentEvent& refEvent );
	virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
	virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const;
	virtual void OnUpdate(const unsigned long &nTimeElapsed);
	virtual const char* SerializeName();
	virtual void SerializeComponent(CParamPool &IOBuff);
	virtual void OnLogin();
	virtual void OnLogout();
	virtual void SendExtraStaticData(){};
	virtual void PacketBuff(std::list<BuffData>& listBuff) {}
	void RegComponentNetMsgMap();
public:
	CEntityAttributeComponent	* RoleAttr() const;
	CEntityItemComponent		* RoleItem() const;
	CRoomComponent				* RoleRoom() const;
	CEntityCoupleComponent		* RoleCouple() const;
	CEntityFriendComponent		* RoleFriend() const;
	CEntityChatComponent		* RoleChat() const;
	CAmuseRoomComponent			* RoleAmuseRoom() const;
    CEntityDanceGroupComponent  * RoleDanceGroup() const;
public:
	void OnGetMyPhotoRoomInfo(GameMsg_Base & rMsg, CSlotPeer & slotPeer);
	void OnCreatePhotoRoom(GameMsg_Base & rMsg, CSlotPeer & slotPeer);
	void OnTryEnterMyPhotoRoom(GameMsg_Base & rMsg, CSlotPeer & slotPeer);
	void OnQuitPhotoRoom(GameMsg_Base & rMsg, CSlotPeer & slotPeer);
	void OnCloseRoom(GameMsg_Base & rMsg, CSlotPeer & slotPeer);
	void OnInvitedPhotoRoomPlayer(GameMsg_Base & rMsg, CSlotPeer & slotPeer);
	void OnInviteeResponse(GameMsg_Base & rMsg, CSlotPeer & slotPeer);
	void OnKickPhotoRoomPlayer(GameMsg_Base &rMsg, CSlotPeer & slotPeer);
	void OnChangePhotoRoomPlayerState(GameMsg_Base &rMsg, CSlotPeer & slotPeer);
	void OnChangePhotoRoomState(GameMsg_Base &rMsg, CSlotPeer & slotPeer);
	void OnChangePhotoRoomBG(GameMsg_Base &rMsg, CSlotPeer & slotPeer);
	void OnChangePhotoRoomAction(GameMsg_Base &rMsg, CSlotPeer & slotPeer);
	void OnRenewPhotoRoom(GameMsg_Base &rMsg, CSlotPeer & slotPeer);
    void OnPressPhotoButton(GameMsg_Base &rMsg, CSlotPeer & slotPeer);

public:
	void SetPhotoRoom(CPhotoRoom *pRoom);
	CPhotoRoom *PhotoRoom();

	void SetHost(bool bHost);
	bool IsHost();

	void SetPlayerState(EPhotoRoomPlayerState eState);
	EPhotoRoomPlayerState PlayerState();

	void SetLastInviteTime(int nInviteTime);
	int LastInvitedTime();

	void InPhotoRoom(CPhotoRoom *pRoom);
	void OutPhotoRoom();

	void EncodePhotoRoomPlayerInfo(CPhotoRoomPlayerInfo & rPhotoRoomPlayerInfo);

	bool TryBeInvited(unsigned int nRoomID, int nRoomTime);
	void InvitedFinished();
	void InvitedOver(unsigned int nRoomID);

	bool IsInvited(unsigned int nRoomID, int nCreatedTime);

	void SendPlayerMsg(GameMsg_Base *pMsg);

	void OnPhotoRoomTest(int nPara1, int nPara2, int nPara3);

    // »’÷æ
    void InsertActionLog(CRolePhotoRoomLog::EPhotoRoomAction eAction);

private:
	CPhotoRoom * m_pPhotoRoom;
	bool m_bHost;
	EPhotoRoomPlayerState m_ePlayerState;
	int m_nLastInviteTime;
	std::map<unsigned int, CInvitPhotoRoomInfo > m_mapInvitedRoom;

	CEntityAttributeComponent * m_pRoleAttr;
	CEntityNetComponent	* m_pRoleNet;
	CEntityItemComponent * m_pRoleItem;
	CRoomComponent * m_pRoleRoom;
	CEntityCoupleComponent * m_pRoleCouple;
	CEntityGeneComponent * m_pRoleGene;
	CEntityDanceGroupComponent * m_pRoleDanceGroup;
	CEntityFriendComponent * m_pRoleFriend;
	CEntityChatComponent * m_pRoleChat;
	CEntityVIPComponent *m_pRoleVIP;
	CAmuseRoomComponent *m_pRoleAmuseRoom;
	PetComponent *m_pRolePet;
    CEntityMedalComponent *m_pRoleMedal;
};


typedef CRoleComponentImpl<CEntityPhotoRoomComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRolePhotoRoom;

#endif







