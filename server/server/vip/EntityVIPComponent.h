#ifndef	__ENTITYVIPCOMPONENT_H__
#define __ENTITYVIPCOMPONENT_H__
#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "VIPDefine.h"


class GameMsg_S2C_CreateRoleSuc;

class CEntityNetComponent;
class CEntityAttributeComponent;
class CEntityChatComponent;
class CEntityItemComponent;
class CEntityQuestComponent;
class CEntityAchievementComponent;
class CEntityMailComponent;
class CEntityFriendComponent;
class CRoomComponent;
class CEntityDanceGroupComponent;
class CEntityCoupleComponent;
class CEntityPhotoRoomComponent;
class CAmuseRoomComponent;
class CEntityCheckInComponent;

class ISQLLog;

class CEntityVIPComponent : public CEntityComponent
{
public:
    CEntityVIPComponent();
    ~CEntityVIPComponent();
public:
    virtual void Start();
    void RegComponentNetMsgMap();
public:
    CEntityAttributeComponent * GetRoleAttr(){
        return m_pRoleAttr;
    }
    CEntityNetComponent		* GetRoleNet(){
        return m_pRoleNet;
    }
    CEntityChatComponent	* GetRoleChat(){
        return m_pRoleChat;
    }
    CEntityMailComponent	* GetRoleMail(){
        return m_pRoleMail;
    }
public:
    bool CreateFromDB(ROLEINFO_DB* pRoleInfoDB);
    bool PacketToDB(ROLEINFO_DB*pRoleInfoDB) const;
    virtual void SerializeComponent(CParamPool &IOBuff);
    virtual void OnLogin();
    virtual void OnLogout();
    virtual void OnUpdate(const unsigned long & nTimeElapsed);
    virtual void OnEvent(CComponentEvent& refEvent);
    virtual const char* SerializeName(){
        return "VIP";
    };
    virtual void SendExtraStaticData(){};
    virtual void PacketBuff(std::list<BuffData>& listBuff);
public:
    void OnGetVIPDailyReward(GameMsg_Base & rMsg, CSlotPeer & rSlotPeer);
    void OnRequestBuyVIP(GameMsg_Base & rMsg, CSlotPeer & rSlotPeer);
    void OnSendOpenVipFrom(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer);

    void OnUpdateVIPInfo(unsigned int nAddExp, unsigned int nAddTime);
    void ChangeVIPInfo(unsigned int nAddTime, unsigned int nAddExp, ERoleVipExpCause eCause);

    bool IsVIP();
    int VIPLevel();
    int VIPExp();

    void SetVIPConsumedPt(int nConsumedPt);
    void ChangeConsumedPt(int nConsumePt);
    int ConsumedPt();

    void SetExchangedpt(int nExchangedPt);
    void ChangeExchangedPt(int nValue);
    int ExchangedPt();

    int GetVIPMatchExpRateBouns();
    void OnVIPTest(int nPara1, int nPara2, int nPara3);
    void CmdSetVIPLevel(int nVIPLevel);

    void VIPBillExpBounus(int nPt);

    unsigned short GetDanceGroupChallengeExtraChance();
private:
    void SendPlayerMsg(GameMsg_Base *pMsg);
    void NotifyClientUpdateVIPInfo();

    void NotifyGroupUpdateVIPInfo();

    void UpdateVIPInfoToGroup();
    void SendVIPLevelUpReward();
    void LevelUpRewardNotification(int nLevel);
    void VIPLevelUpAnnounce(int nVIPLevel);

    void VIPOnlineAnnounce();

    void UpdateVIPInfoToDB();
    void OnVIPInfoChanged(int nOldVipLevel, int nNewVipLevel);
    void OnVIPExpired();

    void SendOpenVipFrom();

    void LogVipOpen();
    void LogVipLevelUp(int nNewLevel);
    void LogVipExp(ERoleVipExpCause eCause, int nAddValue);
    void LogVipDuration(int nAddValue);
    void LogRoleVip(ISQLLog *pLog);

public:
    CRoleVIPInfo m_VIPInfo;
    unsigned short m_nPrivLevel;			// 使用道具或穿装备产生的VIP等级
    map<unsigned char, unsigned short> m_mapVIPCards;
private:
    CEntityAttributeComponent		* m_pRoleAttr;
    CEntityNetComponent				* m_pRoleNet;
    CEntityChatComponent			* m_pRoleChat;
    CEntityItemComponent			* m_pRoleItem;
    CEntityAchievementComponent		* m_pRoleAch;
    CEntityQuestComponent			* m_pRoleQuest;
    CEntityMailComponent			* m_pRoleMail;
    CEntityFriendComponent			* m_pRoleFriend;
    CRoomComponent					* m_pRoleRoom;
    CEntityDanceGroupComponent		* m_pRoleDanceGroup;
    CEntityCoupleComponent			* m_pRoleCouple;
    CEntityPhotoRoomComponent		* m_pRolePhotoRoom;
    CAmuseRoomComponent				* m_pRoleAmuseRoom;
    CEntityCheckInComponent			* m_pRoleCheckIn;
    time_t							  m_tLastSecond;

private:
    std::list<unsigned char>        m_listOpenVipFromType; // 客户端需要，只做记录
};

typedef CRoleComponentImpl<CEntityVIPComponent, CGetRoleProcessorObj, CGetRoleProcessorObj> CRoleVIP;

#endif

