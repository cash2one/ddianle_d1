#ifndef __ROOMCOMPONENT_COMPONENT_H__
#define __ROOMCOMPONENT_COMPONENT_H__

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "../item/EntityItemComponent.h"
#include "../roomLib/RoomDataStruct.h"
#include "../roomLib/RoomMarkInfo.h"

class RoomPlayerInfo;
class InivteePlayerInfo;
class LiveRoomAudienceInfo;

class CRoom;
class CEntityNetComponent;
class CEntityAttributeComponent;
class CEntityVIPComponent;
class CEntityQuestComponent;
class CEntityAchievementComponent;
class CEntityFriendComponent;
class CEntityDanceGroupComponent;
class CPlayerInfoComponent;
class CEntityChatComponent;
class CEntityCoupleComponent;
class CEntityPhotoRoomComponent;
class CAmuseRoomComponent;
class CEntityAntiAddictonComponent;
class CEntityQuestNewComponent;
class PetComponent;
class CEntityMedalComponent;
class CEntityHorseComponent;

class PlayerRoomStatus;

struct SInvitedRoomInfo
{
    unsigned int m_nRoomID;
    int m_nRoomTime;
    int m_nLeftTime;

    SInvitedRoomInfo(unsigned int nRoomID, int nRoomTime, int nLeftTime)
    {
        m_nRoomID = nRoomID;
        m_nRoomTime = nRoomTime;
        m_nLeftTime = nLeftTime;
    }
};

// groupRoom信息
class GroupRoomInfo
{
public:
    GroupRoomInfo();
    ~GroupRoomInfo();
    // player is in room.
    bool IsInRoom() const;
    // 
    void ClearRoom();
    void LockRoom(ERoomType roomType);

    unsigned int m_nRoomID;
    ERoomType m_roomType;
};

class CRoomComponent : public CEntityComponent
{
public:
    CRoomComponent(void);
    ~CRoomComponent(void);

public:
    virtual void Start();
    virtual void OnEvent(CComponentEvent& refEvent) {}

    void RegComponentNetMsgMap();
    void SendPlayerMsg(GameMsg_Base* pMsg);

    CRoom* Room() const;
    unsigned int RoomID() const;
    // 
    bool IsInRoom() const;
    bool IsInGroupRoom() const;
    GroupRoomInfo &GetGroupRoomInfo();

    void InRoom(CRoom* pRoom, EDanceColorTeam eColorTeam, int nRoleType, int nRoleState, int nRolePos, bool bHost);
    void OutRoom();

    void SetHost(bool bHost);
    bool IsHost() const;
    void AsHost(bool bIsHost);

    int RoleRoomType() const;
    int RoleRoomPos() const;
    int RoleRoomState() const;
    void ChangeRoleState(int nRoleRoomState);
    void ChangeRoleType(int nRoleRoomType, int m_nRoleRoomPos, int m_nRoleRoomState);

    int RoleDefaultState() const;

    CRoomMarkInfo& RoleMarkInfo();
    bool RecvRoundMark(int nMode, int nRound, unsigned int nMark, char chKeyRank, const std::vector<char>& vecRoundRank);

    bool IsMatching() const;
    int MatchMode() const;
    void AutoMatch(bool bMatch, int nMode);

    unsigned long GetInviteeTime() const;
    void SetGetInviteeTime(unsigned long lTime);

    unsigned long LastInviteTime() const;
    void SetLastInviteTime(unsigned long lTime);

    bool IsBeingInvited() const;
    bool IsBeingInvited(unsigned int nRoomID, int nRoomTime) const;

    void InvitedOver();
    void InvitedOver(unsigned int nRoomID);

    bool TryBeInvited(unsigned int nRoomID, int nRoomTime);

    void EncodePlayerInfo(RoomPlayerInfo& rPlayerInfo);
    void EncodeInviteeInfo(InivteePlayerInfo& rPlayerInfo) const;
    void EncodeAudienceInfo(LiveRoomAudienceInfo& rPlayerInfo) const;

    void SetRoomColor(int nColor);
    int GetRoomColor();

    void SetRoomTop(bool bTop);
    bool GetRoomTop();

    void SetPhoneOS(int nPhoneOS);
    int GetPhoneOS();

    void SetHotValue(int nHotValue);
    int GetHotValue() const;

    void SetColorTeam(EDanceColorTeam nTeamColor);
    EDanceColorTeam ColorTeam() const;

    bool AcceptRoomInvitation();
    //发送关底奖励道具
    void SendRoomItemReward(int nMusicMode);
    void SendRoomCurrencyReward();
    unsigned int SimulateDanceItemDrop(int nMusicMode, unsigned int nCount);
    //情侣奖励
    void CoupleReward(CRoomComponent* pDanceRoom, CRoomComponent* pCoupleRoom, int nRank);
    int SendExpReward();

    void JoionTeam(EDanceColorTeam eColorTeam);
    void ExitTeam();

    void OnStartDance();
    int CalcuGrade();
    int CalcTotalMark();
    int CalcRankMark(int nMusicMode, int nMusicLevel, int nRank, int &nSkillMark);
    int CalcExp(int nDancerCount, int nMusicMode, int nMusicLevel);

    CEntityNetComponent* NetCOM() const;
    void SetAttrCOM(CEntityAttributeComponent *pAttrCOM);
    CEntityAttributeComponent* AttrCOM() const;
    CEntityItemComponent* ItemCOM() const;
    CEntityVIPComponent* VipCOM() const;
    CEntityQuestComponent* QuestCOM() const;
    CEntityAchievementComponent* AchieveCOM() const;
    CEntityFriendComponent *FriendCOM()const;
    CEntityDanceGroupComponent *DanceGroupCOM()const;
    CPlayerInfoComponent *PlayerInfoCOM()const;
    CEntityChatComponent *ChatCOM()const;
    CEntityCoupleComponent *CoupleCOM()const;
    CEntityGeneComponent *GeneCOM()const;
    CEntityCoupleComponent *RoleCouple()const;
    CEntityPhotoRoomComponent * RolePhotoRoom()const;
    CAmuseRoomComponent *RoleAmuseRoom() const;
    CEntityQuestNewComponent* RoleQuestNew() const;
public:
    virtual void SerializeComponent(CParamPool &IOBuff) {}
    virtual const char* SerializeName() {
        return "Room";
    }

    virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB) {
        return true;
    }
    virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const {
        return true;
    };
    virtual void OnUpdate(const unsigned long& lTimeElapsed);
    virtual void OnLogin();
    virtual void OnLogout();
    virtual void SendExtraStaticData(){};
    virtual void PacketBuff(std::list<BuffData>& listBuff) {}

protected:
    void OnGetRolePointAndHot(QueryBase &rQuery);
    void LogRoleCheckingMarkFailed();

public:
    void OnEnterLobby(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnExitLobby(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnGetRoomList(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnGetInviteeList(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnCreateRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnQuitRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnStartRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnReadyRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnEndRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnTryEnterPlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnEnterPlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnKickPlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnApplyMatch(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnCancelMatch(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnInvitePlayer(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnInviteeResponse(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnInEquip(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnOutEquip(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnChangeRoomInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnChangeMusicInfo(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnChangeRoleRoomType(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnChangeRoleRoomState(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnChangeRoomPosState(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnReportRoundMark(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnReportEffectChange(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnQuitMarkAward(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);

    void OnPromoteRoomHost(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);

    void OnLoadingStartGameProgress(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);

    void OnSwitchDanceRoomMode(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);

    void OnJoinDanceRoomTeam(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);

    void OnEnterLiveLobby(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnExitLiveLobby(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnGetLiveRoomList(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnCreateLiveRoom(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void OnEnterLiveRoom(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    void OnQuitLiveRoom(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    void OnCloseLiveRoom(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    void OnSetLiveRoomDefaultCloth(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    void OnGetLiveRoomScore(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    void OnUpdateLiveRoomDancer(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    void OnSetLiveRoomDancerToAudience(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);

    //////////////////////////////////////////////////////////////////////////
    void OnOpenVote(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    void OnGetLiveRoomHotList(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    void OnGetLiveRoomAuditionScoreList(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    void OnGetMyAuditionScore(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    void OnVote(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    void OnLiveRoomJoinAudition(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    void OnLiveRoomCancelAudition(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    //////////////////////////////////////////////////////////////////////////

    void SetAuditionScore(int nScore);
    int AuditionScore() const;
    void UpdateAuditionScore(int nScore);

    void OnRoomTest(int nPara1, int nPara2, int nPara3);

    void OnSelectPartner(GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);

    void AddLiveRoomAuditionScoreRankData(int nAddCount);

protected:
    bool m_bIsHost;
    int m_nRoleRoomType;
    int m_nRoleRoomPos;

    int m_nRoleRoomState;
    int m_nRoleDefaultState;

    bool m_bIsMatching;
    int m_nMatchMode;

    EDanceColorTeam m_eColorTeam;

    unsigned long m_lGetInviteeTime;
    unsigned long m_lLastInviteTime;

    std::map<unsigned int, SInvitedRoomInfo> m_mapInvitedRoom;

    CRoom* m_pRoom;
    CRoomMarkInfo m_MarkInfo;

    int m_nRoomColor;
    bool m_bRoomTop;

    int m_nPhoneOS;
    int m_nHotValue;
    int m_nAuditionScore;

    PlayerRoomStatus m_status;
    GroupRoomInfo m_groupRoomInfo;

    CEntityNetComponent* m_pRoleNetCOM;
    CEntityAttributeComponent* m_pRoleAttrCOM;
    CEntityVIPComponent* m_pRoleVipCOM;
    CEntityQuestComponent* m_pRoleQuestCOM;
    CEntityAchievementComponent* m_pRoleAchieveCOM;
    CEntityItemComponent* m_pItemCOM;
    CEntityGeneComponent* m_pGeneCOM;
    CEntityFriendComponent* m_pFriendCOM;
    CEntityDanceGroupComponent* m_pDanceGroupCOM;
    CPlayerInfoComponent * m_pRolePlayerInfo;
    CEntityChatComponent * m_pChatCOM;
    CEntityCoupleComponent *m_pCoupleCOM;
    CEntityPhotoRoomComponent *m_pRolePhotoRoom;
    CAmuseRoomComponent *m_pRoleAmuseRoom;
    CEntityAntiAddictonComponent *m_pRoleAntiAddiction;
    CEntityQuestNewComponent* m_pRoleQuestNew;
    PetComponent * m_pRolePet;
    CEntityMedalComponent *m_pRoleMedal;
    CEntityHorseComponent *m_pRoleHorse;
};

typedef CRoleComponentImpl<CRoomComponent, CGetRoleProcessorObj, CGetRoleProcessorObj> CRoleRoom;

#endif

