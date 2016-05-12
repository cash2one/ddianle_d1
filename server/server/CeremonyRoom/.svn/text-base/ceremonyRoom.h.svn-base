/** Counter.h
* Description: new member ceremony room class.
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: liguanghao	
* Create time: 2015-08-10 11:48:00
*/

#ifndef __CEREMONYROOM_H__
#define __CEREMONYROOM_H__

#include <map>
#include <string>
#include "ceremonyRoomHead.h"
#include "GameMsg_CeremonyRoom.h"
#include "GameMsg_CeremonyRoomSC.h"


class CRoleEntity;

// 仪式中成员信息
// 日志统计的时候，成员可能已经不在线，因此需要在仪式开启的一刻，把所有成员的信息保存下来，
// 在需要的时候，直接从这里取
class CeremonyMemberLogInfo
{
public:
    unsigned int m_nRoleID;
    std::string m_strName;
    unsigned char m_nVipLev;
};

class CeremonyRoom
{
public:
	CeremonyRoom(unsigned int danceGroupID, CRoleEntity *pRole);
	~CeremonyRoom();

    bool IsCreator(unsigned nRoleID);
    bool IsNewMember(unsigned nRoleID);
    bool IsNormalMember(unsigned nRoleID);
    bool IsNewMemberAtStart(unsigned nRoleID);
    bool IsNormalMemberAtStart(unsigned nRoleID);
    // 房间是否已经开启了。
    bool IsRoomStart();

	// 创建者操作
	int SetNewMember(unsigned int nRoleID, int seq, bool bNeedMengxin);
	int CancelNewMember(unsigned int nRoleID); // 取消指定为新人

	int KickPerson(unsigned int nRoleID); // 从房间踢人

	int Play(std::vector<CeremonyRookieInfo> &vecRookieInfo); // 客户端开始播放，进入点赞流程

	// 
	int EnterRoom(unsigned int nRoleID, CeremonyRoomInfo& roomInfo, CeremonyPlayerShowInfo &selfInfo); // 进入房间
	int LeaveRoom(unsigned int nRoleID); // 离开房间

	int SaveIntroduce(unsigned int nRoleID, std::string strIntroduce); // 写自我介绍信

	unsigned char APraiseB(unsigned int nRoleA, unsigned int nRoleNewer, unsigned char nNewerIndex, int count, int &nNewCount); // A向B点赞

	void GetRoomInfo(CeremonyRoomInfo& roomInfo); // 取得房间信息
	bool GetRoomPlayerShowInfo(unsigned int nRoleID, CeremonyPlayerShowInfo &info);

	// 广播所有人，除了一个人
	void RoomBroadcastExcept(unsigned int nRoleID, GameMsg_Base &msg);
    // 广播所有人
    void RoomBroadcast(GameMsg_Base &msg);

    unsigned int GetCreator(); // 取得仪式创建者
    
    bool HasMember(unsigned int nRoleID);

    unsigned int GetApplyTime();
    unsigned int GetStartTime();
    ECeremonyState GetState();

    // 提交房间日志
    void ComitRoomLog();
    // 发送邮件奖励
    void MailRoomPrize();

    void CloseRoom();

protected:
    void FromNormalMemberToNewMember(unsigned int nRoleID, unsigned seq);
    void FromNewMemberToNormalMember(unsigned int nRoleID);

    void AllMember(std::vector<unsigned int> &allMember);

    
    CeremonyNormalMember* GetNormalMember(unsigned int nRoleID);
    CeremonyNewMember* GetNewerMember(unsigned int nRoleID);
    CeremonyNormalMember* GetNormalMemberAtStart(unsigned int nRoleID);
    CeremonyNewMember* GetNewerMemberAtStart(unsigned int nRoleID);
    CeremonyMemberLogInfo *GetMemberLogInfo(unsigned int nRoleID);

    std::string GetIntroduce(unsigned int roleID);
    void SetIntroduct(unsigned int roleID, std::string strIntroduce);

protected:
    typedef std::map<unsigned int, CeremonyNormalMember> CeremonyNormalMemberMap;
    typedef std::map<unsigned int, CeremonyNewMember> CeremonyNewMemberMap; // 
    typedef std::map<unsigned int, CeremonyMemberLogInfo> CeremonyMemberLogInfoMap;
    typedef std::map<unsigned int, std::string> StringMap;
	unsigned int m_nGroupID;						// 所属舞团ID
	unsigned int m_nCreater;						// 房间的创建者
    CeremonyLogRoomInfo m_logRoomInfo;              // 房间的创建信息
    
    CeremonyNormalMemberMap m_mapNormalMember;	    // 所有人（创建者，观众，新人）
	CeremonyNewMemberMap m_mapNewMember;            // 所有新人
    CeremonyMemberLogInfoMap m_mapMemberLogInfo;    // 日志相关的个人信息
    StringMap m_mapIntroduce;                       // 新人自我介绍

    // 仪式开启之后，参与者即使退出房间也是可以获得奖励，因此需要保存这些人，用来发奖励
    CeremonyNormalMemberMap m_mapNormalMemberAtStart; // 仪式开启时的所有人
    CeremonyNewMemberMap m_mapNewMemberAtStart;   // 仪式开启时候的新人

	ECeremonyState m_crsState;		    			// 房间状态

	// 这两个时间需要做倒计时
	unsigned int m_nApplyTime;						// 房间开启时间 （倒计时一个小时）（倒计时结束之后，强制开启）
	unsigned int m_nStartTime;						// 房间点击开启之后的时间 （倒计时结束之后，房间强制关闭）

};


struct PraiseSort
{
    unsigned int nRoleID;
    int nSeq;
    int nPraise;
    static bool Great(const PraiseSort& s1, const PraiseSort &s2);
};

#endif // __CEREMONYROOM_H__





