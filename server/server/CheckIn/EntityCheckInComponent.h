#ifndef __ENTITYCHECKINCOMPONENT_H__
#define __ENTITYCHECKINCOMPONENT_H__

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "CheckInfoMgr.h"

class CEntityNetComponent;
class CEntityAttributeComponent;
class CEntityChatComponent;
class CEntityItemComponent;
class CEntityQuestComponent;
class CEntityAchievementComponent;
class CEntityMailComponent;
class CEntityVIPComponent;

class CEntityCheckInComponent : public CEntityComponent
{
public:
    CEntityCheckInComponent();
    ~CEntityCheckInComponent();

public:
    virtual void Start();
    void RegComponentNetMsgMap();

public:
    virtual bool CreateFromDB(ROLEINFO_DB* pRoleInforDB);
    virtual bool PacketToDB(ROLEINFO_DB*pRoleInforDB) const;
    virtual bool PacketToCache( ROLEINFO_DB* pRoleInforDB ) const;
    virtual void SerializeComponent(CParamPool &IOBuff);
    virtual void OnLogin();
    virtual void OnLogout();
    virtual void OnUpdate(const unsigned long & nTimeElapsed);
    virtual void OnEvent(CComponentEvent& refEvent);
    virtual const char* SerializeName(){ return "Check"; };
    virtual void SendExtraStaticData(){};
    virtual void PacketBuff(std::list<BuffData>& listBuff) {}

public:
    // 获取签到信息
    void OnGetCheckInInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // 签到
    void OnCheck(GameMsg_Base &msg, CSlotPeer &slotPeer);
    int CheckIn(unsigned int nDayIndex);

    // 充值签到数据
    bool  ResetByMonthChange();

    // vip变换发送奖励
    void ReissueRewardByVipLevelChange(int nOldVipLevel, int nNewVipLevel);
    void ReissueRewardByVipOpen(int nVipLevel);
    void SendReissRewardByMail(CCheckInConfig * pCheckInConfig);
    void VipExpired();
    bool IsTodayCheckIn(); // 今天是否已经签到

    // 签到次数
    unsigned int GetCurrentCheckCount();

    // 保存签到信息
    void SaveCheckInInfoToDB();

    // 发送消息
    void SendPlayerMsg(GameMsg_Base *pMsg);

    // log日志
    void _LogRoleCheckIn(unsigned int nRoleID, CRoleCheckInLog::ECheckInAction eCheckInAction, unsigned int nDayIndex, bool bIsVip, unsigned int nVipLevel);

    // GM 命令
    void GMClearCheckInData();

private:
    CEntityAttributeComponent   * m_pRoleAttr;
    CEntityNetComponent         * m_pRoleNet;
    CEntityChatComponent        * m_pRoleChat;
    CEntityItemComponent        * m_pRoleItem;
    CEntityAchievementComponent * m_pRoleAch;
    CEntityQuestComponent       * m_pRoleQuest;
    CEntityVIPComponent         * m_pRoleVip;

public:
    CCheckInInfo m_CheckInInfo;

};

typedef CRoleComponentImpl<CEntityCheckInComponent, CGetRoleProcessorObj, CGetRoleProcessorObj> CRoleCheckIn;

#endif //__ENTITYCHECKINCOMPONENT_H__

