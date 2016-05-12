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
    unsigned int nEndTime;              // ��ʼʱ�䣬����ʱ��
};

class EntityBigMama
{
public:
    EntityBigMama(CEntityDanceGroupComponent* ptrDanceGroupComponent);
    ~EntityBigMama();

    void Init(CEntityAttributeComponent* pAttr, CRoleEntity* pRole);
    // �����½ʱ���·���Ϣ
    void OnLogin();
    // �˳���ʱ����Ҫ�ر�����ķ���
    void OnLogout(); 

    void CreateFromDB(ROLEINFO_DB *pRoleInforDB);
    bool PacketToCache(ROLEINFO_DB *pRoleInfoCache) const;


    //////////////////////////////////////////////////////////////////////////
    // from group server.
    void OnG2SBigMamaInfoNotifyOnLogin(GameMsg_Base* pMsgBase); // ����֪ͨbigMama��Ϣ
    void OnG2SBigMamaInfoNotifyOnJoinDanceGroup(GameMsg_Base* pMsgBase); // ����֪ͨbigMama��Ϣ
    void OnG2SNoticeBigMamaState(GameMsg_Base* pMsgBase); // �״̬���/������/�رգ�
    void OnG2SBroadBigMamaInfo(GameMsg_Base* pMsgBase);
    void OnG2SBigMamaInfoNotify(GameMsg_Base* pMsgBase);

    void OnG2SBigMamaFightResult(GameMsg_Base* pMsgBase);
    void OnG2SBigMamaGetRewardResult(GameMsg_Base* pMsgBase);

    //////////////////////////////////////////////////////////////////////////
    // from client.
    void OnRequestBigMamaStageInfo(GameMsg_Base *pMsgBase); // ����ؿ���Ϣ
    void OnRequestFightBigMama(GameMsg_Base *pMsgBase); // ������ս
    
    void OnRequestGetBigMamaReward(GameMsg_Base *pMsgBase); // ������ȡ����
    void OnCompleteBigMamaGuide(GameMsg_Base *pMsgBase); // ��ɴ�������
    void OnRequestBigMamaGuideReward(GameMsg_Base *pMsgBase); // ������ȡ������������

public:
    unsigned int BigMamaRoomID() const;
    // ���׼�����
    void OnReadyRoom();
    bool EndRoom(); // ��������

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


    unsigned int m_stampLastPrize;                  // ��һ����ȡ���˽�����ʱ���
    EBigMamaPrizeState m_prizeState;                // ���˵Ľ���״̬��ע�⣺����ֻ���ڻ���еĹ����в�����ȡ��
    unsigned int m_stampLastJoin;                   // �ϴβ���ʱ���
    bool m_bParticipant;                            // �Ƿ��ǲ�����

    unsigned int m_nRoomID;                         // ���跿���ID
    unsigned char m_nBeatRound;                     // ����������
    unsigned int m_nTargetScore;                    // �����Ŀ�����

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
    void OnG2SNoticeBigMamaState(GameMsg_Base &msgBase, CSlotPeer & SlotPeer); // �״̬���/������/�رգ�

    BigMamaConfigBrief& GetCfg();

protected:
    BigMamaConfigBrief m_cfgBrief; // config
};


#endif // __GAMESERVER_ENTITYBIGMAMA_H__

