#ifndef __REDENVELOPE_ACTIVITY_MANAGER_H__
#define __REDENVELOPE_ACTIVITY_MANAGER_H__

#include <list>
#include <map>
#include "../../../socket/GameMsg_Processor.h"
#include "../../../datastructure/LogStructDef.h"
#include "../TimerActivity.h"

class GameMsg_Base;
class CEntityCoupleComponent;

class RedEnvelopeActivity : 
    public CTimeActivity
{
public:
    RedEnvelopeActivity();
    virtual ~RedEnvelopeActivity();

public:
    void Init(time_t tBeginTime, time_t tEndTime);
    virtual void OnActivityBegin();
    virtual void OnActivityEnd();
};

class RedEnvelopeActivityManager : public GameMsg_Processor
{
public:
    RedEnvelopeActivityManager();
    virtual ~RedEnvelopeActivityManager();

    static RedEnvelopeActivityManager & Instance();

public:
    // 初始化
    void Initialize();
    void InitializeActivity();

    // 充值
    void OnPlayerCharge(GameMsg_Base & msg, CSlotPeer & slotPeer);
    void OnLoadRoleInfo(QueryBase & query);

    // 同步充值信息到group
    void SyncChargeInfoToGroup(unsigned int nRoleID, const std::string & strRoleName, int nMCoin,
        unsigned int nDanceGroupID, bool bAnonymity);

    // 获取/设置是否匿名
    void SetAnonymity(bool bAnonymity);
    bool IsAnonymity();

    // 活动是否开放
    bool IsActivityOpen();
    void SetActivityOpen(bool bIsOpen);

private:
    void RegisterMessage();

public:
    bool            m_bAnonymity;
    bool            m_bActivityOpen;
};




#endif



