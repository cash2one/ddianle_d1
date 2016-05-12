#ifndef __GAMESERVER_ENTITYBIGMAMA_H__
#define __GAMESERVER_ENTITYBIGMAMA_H__

#include "DataStruct_BigMama.h"


class GameMsg_Base;
class CEntityDanceGroupComponent;
class CEntityAttributeComponent;
class CReward;
class BigMamaReward;
struct ROLEINFO_DB;

struct BigMamaConfigBrief
{
    BigMamaConfigBrief()
        : bOpened(false)
        , nStartTime(0)
        , nEndTime(0)
    {}

    bool bOpened;                       // 
    unsigned int nStartTime;
    unsigned int nEndTime;              // 开始时间，结束时间
};

class EntityBigMama
{
public:
    EntityBigMama(CEntityDanceGroupComponent* ptrDanceGroupComponent);
    ~EntityBigMama();

    void Init(CEntityAttributeComponent* pAttr, CRoleEntity* pRole);
    // 处理登陆时的下发消息
    void OnLogin();
    // 退出的时候，需要关闭跳舞的房间
    void OnLogout(); 

    void CreateFromDB(ROLEINFO_DB *pRoleInforDB);
    bool PacketToCache(ROLEINFO_DB *pRoleInfoCache) const;


    //////////////////////////////////////////////////////////////////////////
    // from group server.
    void OnG2SBigMamaInfoNotifyOnLogin(GameMsg_Base* pMsgBase); // 上线通知bigMama信息
    void OnG2SBigMamaInfoNotifyOnJoinDanceGroup(GameMsg_Base* pMsgBase); // 上线通知bigMama信息
    void OnG2SNoticeBigMamaState(GameMsg_Base* pMsgBase); // 活动状态变更/（开启/关闭）
    void OnG2SBroadBigMamaInfo(GameMsg_Base* pMsgBase);
    void OnG2SBigMamaInfoNotify(GameMsg_Base* pMsgBase);

    void OnG2SBigMamaFightResult(GameMsg_Base* pMsgBase);
    void OnG2SBigMamaGetRewardResult(GameMsg_Base* pMsgBase);

    //////////////////////////////////////////////////////////////////////////
    // from client.
    void OnRequestBigMamaStageInfo(GameMsg_Base *pMsgBase); // 请求关卡信息
    void OnRequestFightBigMama(GameMsg_Base *pMsgBase); // 请求挑战
    
    void OnRequestGetBigMamaReward(GameMsg_Base *pMsgBase); // 请求领取奖励
    void OnCompleteBigMamaGuide(GameMsg_Base *pMsgBase); // 完成大妈引导
    void OnRequestBigMamaGuideReward(GameMsg_Base *pMsgBase); // 请求领取大妈引导奖励

public:
    unsigned int BigMamaRoomID() const;
    // 玩家准备完毕
    void OnReadyRoom();
    bool EndRoom(); // 结束跳舞

    void LogRoleBigMamaRecord(unsigned int nRoleID, unsigned int nDanceGroupID, int nDanceRound);

private:
    void SendMsgToPlayer(GameMsg_Base &msg);
    void SendMsgToGroup(GameMsg_Base &msg);

    void FillGuidePrize(bool isMale, CReward &reward);

    void AddReward(CReward &reward, unsigned int nDancegroupContribution);
    void MailReward(std::list<CItem> &item);
    void BigMamaQuery();
    void CheckGuidePrizeOnLogin();
    void UpdateBigMamaPersonalData(bool bOpend, bool bRunning, time_t nStartTime, time_t nEndTime);
    void BigMamaRewardToReward(bool isMale, BigMamaReward* pReward, CReward &reward);


    BigMamaConfigBrief& ConfigBrief();
private:
    CEntityDanceGroupComponent *m_ptrDanceGroupComponent; // 
    CEntityAttributeComponent *m_ptrAttributeComponent;
    CRoleEntity *m_pRoleEntity;


    unsigned int m_stampLastPrize;                  // 上一次领取个人奖励的时间戳
    EBigMamaPrizeState m_prizeState;                // 个人的奖励状态（注意：奖励只有在活动进行的过程中才能领取）
    unsigned int m_stampLastJoin;                   // 上次参与时间戳
    bool m_bParticipant;                            // 是否是参与者

    unsigned int m_nRoomID;                         // 跳舞房间的ID
    unsigned char m_nBeatRound;                     // 正在跳的轮
    unsigned int m_nTargetScore;                    // 跳舞的目标分数

    bool m_bReceiveGroupData;
};


class EntityBigMamaManager : public GameMsg_Processor
{
protected:
    EntityBigMamaManager();

public:
    ~EntityBigMamaManager();
    static EntityBigMamaManager& Instance();
    void Initialize();
    void RegisterMessage();
    void OnG2SNoticeBigMamaState(GameMsg_Base &msgBase, CSlotPeer & SlotPeer); // 活动状态变更/（开启/关闭）

    BigMamaConfigBrief& GetCfg();

protected:
    BigMamaConfigBrief m_cfgBrief; // config
};


#endif // __GAMESERVER_ENTITYBIGMAMA_H__

