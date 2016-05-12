#ifndef __AMUSEROOM_COMPONENT_H__
#define __AMUSEROOM_COMPONENT_H__

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "GameMsg_ConstellationMatch.h"

class CAmuseRoom;
class CAmuseAction_Base;
class CAmuseEvent_Base;
class CAmuseRoomPlayerInfo;

class CEntityNetComponent;
class CEntityAttributeComponent;
class CPlayerInfoComponent;
class CEntityFriendComponent;
class CEntityItemComponent;
class CEntityGeneComponent;
class CEntityVIPComponent;
class CEntityDanceGroupComponent;
class CRoomComponent;
class CEntityCoupleComponent;
class CEntityPhotoRoomComponent;
class PetComponent;
class CEntityMedalComponent;
class CEntityHorseComponent;
class CEntityMallComponent;

struct TimeEggEntry;
struct CreateTimeEggEntry;

struct SInvitedAmuseRoomInfo
{
	unsigned int m_nRoomID;
	unsigned long m_lRoomCreateTime;
	unsigned long m_lInviteLeftTime;

	SInvitedAmuseRoomInfo(unsigned int nRoomID, unsigned long lRoomCreateTime, unsigned long lInviteLeftTime)
	{
		m_nRoomID = nRoomID;
		m_lRoomCreateTime = lRoomCreateTime;
		m_lInviteLeftTime = lInviteLeftTime;
	}
};


typedef std::map<unsigned int, SInvitedAmuseRoomInfo> AmuseInviteInfoMap;
typedef std::map<unsigned int, CAmuseAction_Base*> AmuseActionByIDMap;		//key - action id
typedef std::map<int, AmuseActionByIDMap> AmuseActionByTypeMap;		//key - action type
typedef std::multimap<unsigned long, CAmuseEvent_Base*> AmusePlayerEventMap;	//key - start time

enum EggInviteType
{
	EggInviteType_None,
	EggInviteType_Create,
	EggInviteType_Open
};

class CAmuseRoomComponent : public CEntityComponent
{
public:
	CAmuseRoomComponent(void);
	~CAmuseRoomComponent(void);

public:
	virtual void Start();
	virtual void OnEvent(CComponentEvent& refEvent) {}

	virtual void SerializeComponent(CParamPool &IOBuff) {}
	virtual const char* SerializeName() { return "AmuseRoom"; }

	virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
	virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const { return true; }
	virtual void OnUpdate(const unsigned long& lTimeElapsed);
	virtual void OnLogin();
	virtual void OnLogout();
	virtual void SendExtraStaticData() {}
	virtual void PacketBuff(std::list<BuffData>& listBuff) {}

public:
	void RegComponentNetMsgMap();
	void SendPlayerMsg(GameMsg_Base* pMsg);

	unsigned int RoleID() const;

	CAmuseRoom* AmuseRoom() const;
	unsigned int AmuseRoomID() const;

	void SetHost(bool bHost);
	bool IsHost() const;

	void InAmuseRoom(CAmuseRoom* pAmuseRoom, bool bHost);
	void OutAmuseRoom();

	void SetRoomColor(int nColor);
	int GetRoomColor();

	void SetRoomTop(bool bTop);
	bool GetRoomTop();

	unsigned long RefreshInviteeTime() const;
	void SetRefreshInviteeTime(unsigned long lTime);

	unsigned long LastInviteTime() const;
	void SetLastInviteTime(unsigned long lTime);

	bool IsAmuseInvited(unsigned int nRoomID, unsigned long lRoomCreateTime) const;
	bool RecvAmuseInvited(unsigned int nRoomID, unsigned long lRoomCreateTime);

	void AmuseInvitedOver();
	void AmuseInvitedOver(unsigned int nAmuseRoomID);

	void DoAmuseAction(CAmuseAction_Base* pAmuseAction);

	AmuseActionByTypeMap& AllAmuseActions();
	CAmuseAction_Base* UniqueAmuseAction(int nActionType);

	void AddAmuseAction(CAmuseAction_Base* pAmuseAction);
	void AmuseActionOver(CAmuseAction_Base* pAmuseAction);
	void AmuseActionOver();

	void EncodePlayerInfo(CAmuseRoomPlayerInfo& rPlayerInfo) const;

	TimeEggEntry* GetTimeEggInCurScene();
	void AddTimeEgg(TimeEggEntry* pEgg, bool bSyn);
	void SendSynTimeEggs();
	void OnCreateTimeEggSuc(CreateTimeEggEntry* pInfo);
	void OnOpenTimeEggSuc(unsigned int nEggID);
	EggInviteType GetInviteTypeInCurScene();
	bool CheckInviteTypeInCurScene(EggInviteType type, unsigned int nInviteeID, string& strError);

    bool CheckIsInDevice();

	CEntityNetComponent* NetCOM() const;
	CEntityAttributeComponent* AttrCOM() const;
	CPlayerInfoComponent* InfoCOM()const;
	CEntityFriendComponent* FriendCOM()const;
	CEntityVIPComponent* VipCOM() const;
	CRoomComponent* StageRoomCOM() const;
	CEntityCoupleComponent* CoupleCOM() const;
	CEntityPhotoRoomComponent* PhotoRoomCOM() const;
	CEntityItemComponent* ItemCOM() const;
    CEntityDanceGroupComponent *RoleDanceGroup() const;
    CEntityMallComponent * RoleMall() const;

public:
	void OnGetAmuseRoomList(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

	void OnCreateAmuseRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
	void OnQuitAmuseRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

	void OnTryEnterAmuseRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
	void OnEnterAmuseRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

	void OnAmuseKickPlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
	void OnAmusePromoteHost(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

	void OnAmuseInvitePlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
	void OnAmuseInviteeResponse(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

	void OnChangeAmuseRoomInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
	void OnDoAmuseAction(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

	void OnAmuseApplyMatch(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

	void OnQueryTimeEggReward(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

	void UpdateAmusePlayerEvent(unsigned long lDelayTime, CAmuseEvent_Base* pAmuseEvent);
	void OnDBCreateTimeEgg(QueryBase & refQuery);
	void OnDBGetTimeEggsByRoleID(QueryBase & refQuery);

	void OnAutoOpenEgg(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

	void OnEnterConstellationLobby(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
	void OnQuitConstellationLobby(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
	void OnConstellationMatch(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
	void OnEnterConstellationRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
	void OnSendQuestionAnswer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

	bool HasEnterConstellationLobby() const;
	void SetEnterConstellationLobby();

	int MaxConstellationMatchCount() const;
	int ConstellationMatchCount() const;

	void EncodeConstellationCardInfo(std::list<SConstellationCardInfo> &listCard);
	SConstellationCardInfo *GetConstellationCard(unsigned short nCardIndex);

	bool ConstellationMatch(unsigned short nCardIndex);
	void OnConstellationMatchSuc();
	void CancelConstellationMatch();
	int MatchedConstellation() const;

	void RandomQuestion(std::vector<ConstellationConfig> &vecQuestion);

	unsigned int ConstellationRoomID() const;
	unsigned int ConstellationRoomCreateTime() const;
	int ConstellationRoomLine() const;
	void SetConstellationRoom(unsigned int nRoomID, unsigned int nCreateTime, int nLine);
	void ShuffleCard();

protected:
	void _LoadAmusePlayerEvent();
	void _ReleaseAmusePlayerEvent();
	void _LogRoleAmuse(int nAction, int nSceneID, int nStayTime);
	void _LogRoleTimeEgg(unsigned int nRoleID, int nAction, int nScene);

	bool IsCurrentRoundMatchTime(unsigned int nMatchTime, unsigned int nNow);
	void AddRandomCard();

protected:
	bool m_bIsHost;
	CAmuseRoom* m_pAmuseRoom;

	int m_nRoomColor;
	bool m_bRoomTop;

	unsigned long m_lRefreshInviteeTime;
	unsigned long m_lLastInviteTime;

	unsigned long m_lInterElapse;
	unsigned long m_lInviteStep;
	unsigned long m_lEventStep;

	unsigned int m_nEnterTime;

	bool m_bHasEnterConstellationLobby;
	int m_nConstellationMatchCount;
	unsigned int m_nConstellationMatchTime;

	// key - card index, value - card info
	unsigned long m_lMatchRoundStep;
	std::map<unsigned short, SConstellationCardInfo> m_mapMatchCard;
	bool m_bConstellationMatch;
	int  m_nMatchConstellation;
	SConstellationCardInfo *m_pMatchCard;

	unsigned int m_nConstellationRoomID;
	unsigned int m_nConstellationRoomCreateTime;
	int m_nConstellationRoomLine;

	AmuseInviteInfoMap m_mapAmuseInvited;
	AmuseActionByTypeMap m_mapAmuseActions;

	AmusePlayerEventMap m_AllPlayerEvents;

	CEntityNetComponent* m_pRoleNetCOM;
	CEntityAttributeComponent* m_pRoleAttrCOM;
	CPlayerInfoComponent* m_pRoleInfoCOM;
	CEntityFriendComponent* m_pRoleFriendCOM;
	CEntityItemComponent* m_pRoleItemCOM;
	CEntityGeneComponent* m_pRoleGeneCOM;
	CEntityVIPComponent* m_pRoleVipCOM;
	CEntityDanceGroupComponent* m_pRoleDanceGroupCOM;
	CRoomComponent* m_pRoleStageRoomCOM;
	CEntityCoupleComponent* m_pRoleCoupleCOM;
	CEntityPhotoRoomComponent* m_pRolePhotoRoomCOM;
	PetComponent                * m_pRolePet;
    CEntityMedalComponent       *m_pRoleMedal;
    CEntityHorseComponent       *m_pRoleHorse;
    CEntityMallComponent *       m_pRoleMall;

	std::map<int, TimeEggEntry*> m_mapEgg;//scendid<=>egg

	std::vector<int> m_Question;		// 本次配对成功，发给客户端的问题, value->question index
	std::set<int> m_allSentQuestion;	// 在当前活动开启时间段，发给客户端的所有问题, value->question index
};

typedef CRoleComponentImpl<CAmuseRoomComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleAmuseRoom;

#endif
//file end

