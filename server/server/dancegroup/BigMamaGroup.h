#ifndef __GROUPSERVER_BIGMAMAGROUP_H__
#define __GROUPSERVER_BIGMAMAGROUP_H__

#include <string>
#include <map>
#include <vector>
#include "../../socket/SlotPeer.h"

#include "DataStruct_BigMama.h"

class CGroupRoleEntity;
using namespace ZeroSocket;

// groupServer �м�¼�������ŵ�bigMaMa ��Ϣ
class BigMamaGroup : public GameMsg_Processor
{
public:
    BigMamaGroup();
    ~BigMamaGroup();

    static BigMamaGroup& Instance();
    bool Initialize();

    void RegisterMsgMap();
    void RegisterMsgMapDelay();
    void OnLoadBigMamaGroupInfo(QueryBase &rQuery);

    void Update(time_t now); // 
    void UpdateBigMamaState(time_t now); // ����������㲥֪ͨ��ң����������ݿ⣩
    void UpdateAnnouncement(time_t now);
    void OnRoleLogin(CGroupRoleEntity &role);
    void OnRoleJoinDanceGroup(CGroupRoleEntity &role);

    void OnBigMamaStageInfoRequest(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // GameServer ����(����)�ؿ���Ϣ
    void OnS2GBigMamaFightRequest(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnS2GBigMamaGetRewardRequest(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnBigMamaBeatSucess(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // GameServer����һ���ؿ���֪ͨ

private:
    bool ClearBigMamaGroupInfo(DanceGroupBigMamaInfo &bigMamaInfo);
    void GenerateNewRound(DanceGroupBigMamaInfo &bigMamaInfo); // �����µ�һ�ֹؿ�

    void LogDanceGroupBigMamaRecord(unsigned int nDanceGroupID, unsigned int nDanceRound, int nBeatType, unsigned int nBeatRole);
private:
    time_t CalcNextCheckTime(time_t now); // ������һ������ʱ��
    bool IsActive(time_t now); // ����Ƿ���
    bool IsValidBigMamaInfo(const DanceGroupBigMamaInfo& mamInfo, time_t now); // ����Ƿ��Ѿ�������֮�����Ϣ
    bool IsSamePeriod(time_t t1, time_t t2);

    std::pair<unsigned int, unsigned int> GetStartAndEnd(time_t now);

    DanceGroupBigMamaInfo* GetDanceGroupMamaInfo(unsigned int nDanceGroupID);
    static void TransfromBigMamaInfoToStageInfo(const DanceGroupBigMamaInfo &bigMamaInfo, BigMamaProperty &bigMamaStageInfo);

    void SendMsgToGameServerPlayer(unsigned int nRoleID, GameMsg_Base &msg);

    void Announcement(time_t now);

private:
    DanceGroupBigMamaMap m_mapBigMamaInfo; // 

    unsigned int m_nNextCheckTime; // ��һ�����ʱ��
    bool m_bPrevOpened; // ֮ǰ�Ƿ���

//     bool m_bOpened; // ���ǰ�Ƿ��ǿ���״̬��
    unsigned int m_nNextAnnounceTime;
};




#endif // __GROUPSERVER_BIGMAMAGROUP_H__

