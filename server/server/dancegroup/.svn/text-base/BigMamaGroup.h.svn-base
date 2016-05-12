#ifndef __GROUPSERVER_BIGMAMAGROUP_H__
#define __GROUPSERVER_BIGMAMAGROUP_H__

#include <string>
#include <map>
#include <vector>
#include "../../socket/SlotPeer.h"

#include "DataStruct_BigMama.h"

class CGroupRoleEntity;
using namespace ZeroSocket;

// groupServer 中记录所有舞团的bigMaMa 信息
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
    void UpdateBigMamaState(time_t now); // 检测活动开启，广播通知玩家（不更新数据库）
    void UpdateAnnouncement(time_t now);
    void OnRoleLogin(CGroupRoleEntity &role);
    void OnRoleJoinDanceGroup(CGroupRoleEntity &role);

    void OnBigMamaStageInfoRequest(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // GameServer 请求(生成)关卡信息
    void OnS2GBigMamaFightRequest(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnS2GBigMamaGetRewardRequest(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnBigMamaBeatSucess(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // GameServer击败一个关卡的通知

private:
    bool ClearBigMamaGroupInfo(DanceGroupBigMamaInfo &bigMamaInfo);
    void GenerateNewRound(DanceGroupBigMamaInfo &bigMamaInfo); // 生成新的一轮关卡

    void LogDanceGroupBigMamaRecord(unsigned int nDanceGroupID, unsigned int nDanceRound, int nBeatType, unsigned int nBeatRole);
private:
    time_t CalcNextCheckTime(time_t now); // 计算下一个检测的时间
    bool IsActive(time_t now); // 检测活动是否开启
    bool IsValidBigMamaInfo(const DanceGroupBigMamaInfo& mamInfo, time_t now); // 检测是否已经是生成之后的信息
    bool IsSamePeriod(time_t t1, time_t t2);

    std::pair<unsigned int, unsigned int> GetStartAndEnd(time_t now);

    DanceGroupBigMamaInfo* GetDanceGroupMamaInfo(unsigned int nDanceGroupID);
    static void TransfromBigMamaInfoToStageInfo(const DanceGroupBigMamaInfo &bigMamaInfo, BigMamaProperty &bigMamaStageInfo);

    void SendMsgToGameServerPlayer(unsigned int nRoleID, GameMsg_Base &msg);

    void Announcement(time_t now);

private:
    DanceGroupBigMamaMap m_mapBigMamaInfo; // 

    unsigned int m_nNextCheckTime; // 下一个检测时间
    bool m_bPrevOpened; // 之前是否开启

//     bool m_bOpened; // 活动当前是否是开启状态：
    unsigned int m_nNextAnnounceTime;
};




#endif // __GROUPSERVER_BIGMAMAGROUP_H__

