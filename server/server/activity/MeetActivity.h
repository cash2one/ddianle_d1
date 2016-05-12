#ifndef __MEETACTIVITY_H__
#define __MEETACTIVITY_H__

#include "TimerActivity.h"
#include "ActivityDataStruct.h"
#include "GameMsg_Activity.h"

#define  MEET_WAIT_TIME 20

class CMeetActivity : public ITimerActivity
{
public:
    CMeetActivity(unsigned int nPhotoCount, unsigned int nWaitTime);
    virtual ~CMeetActivity(){}

public:
    // 心跳
    virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);

    // 是否有效
    virtual bool IsValid();

public:
    // 初始化
    void Init();

    // 设置拍照个数
    void SetPhotoIndexCount(int nCount);

    // 匹配
    int Match(unsigned int nRoleID, unsigned int nSex, unsigned int nCoupleID);

    // 取消匹配
    void CancelMatch(unsigned int nRoleID, unsigned int nSex);

    // 照相
    int PhotoEvent(unsigned int nRoleID, unsigned int nPhotoIndex);

    // 移除下线玩家
    void RemovePlayer(unsigned int nRoleID);

    bool IsCanDestoryAmuseScene(unsigned int nRoleID);

    // 邂逅结束
    void ActivityEnd(unsigned int nRoleID, MeetPhotoInfoList & photosinfo, unsigned int &nSameIndexCount);

    // 获取一组的对方
    unsigned int GetOppositeID(unsigned int nRoleID);
    bool IsOppositeCanGetReward(unsigned int nRoleID, unsigned int & nOppositeID);

    // 获取/设置开放场景房间ID
    void SetCoupleAmuseSceneID(unsigned int nRoleID, int nAmuseRoomID);
    int  GetCoupleAmuseSceneID(unsigned int nRoleID);

    // 是否在活动中
    bool IsInActivity(unsigned int nRoleID);

    // 是否双方都可以发放奖励
    bool IsBothCanReward(unsigned int nRoleID, unsigned int & nOppositeID, int & nIsMail);

    bool IsOppositeWaitReward(unsigned int nRoleID);

private:
    // 等待时间到
    void HandleTimeOut(unsigned int nRoleID);

    // 发送错误消息
    void SendMatchFailed(unsigned int nErr, unsigned int nRoleID);

    // 在等待列表玩家中找
    int MatchInMaleWaitList(unsigned int nRoleID);
    int MatchInFemaleWaitList(unsigned int nRoleID);

private:
    MeetPlayerObjectTable          m_MeetPlayer;
    MeetWaitList                   m_WaitMaleList;
    MeetWaitList		           m_WaitFemaleList;
    MeetCoupleTable				   m_MeetCoupleTable;
    unsigned int                   m_nCoupleCount;
    unsigned int                   m_nPhotoCount;
    unsigned int                   m_nWaitTime;           // 秒
};


#endif

