/** Counter.h
* Description: new member ceremony room class.
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: liguanghao	
* Create time: 2015-08-10 11:48:00
*/

#ifndef __CEREMONYROOMHEAD_H__
#define __CEREMONYROOMHEAD_H__

#include <map>
#include <vector>
#include <string>


#define GROUPDANCE_CEREMONY_MAX_INTRODUCE				1024

class CParamPool;

enum ECeremonyErrType
{
    ECeremonyErrType_OK = 0,				// 无错误
    ECeremonyErrType_RightNotEnough,		// 无权执行操作
    ECeremonyErrType_ResNotEnough,			// 资源不足
    ECeremonyErrType_NotMember,				// 不是舞团成员
    ECeremonyErrType_OnApply,				// 正在申请仪式举办
    ECeremonyErrType_Running,				// 仪式正在进行
    ECeremonyErrType_Internal,				// 服务器错误
    ECeremonyErrType_NotOpen,				// 房间未开启
    ECeremonyErrType_StateErr,				// 房间状态错误
    ECeremonyErrType_NotMengxin,			// 不是小萌新
    ECeremonyErrType_SequenceRedumplicate,	// 新人顺序重复
    ECeremonyErrType_SequenceError,         // 新人顺序不存在
    ECeremonyErrType_NotNewMan,             // 未指定为新人
    ECeremonyErrType_NoNewMan,              // 没有新人
    ECeremonyErrType_NotInRoom,             // 不在房间内
    ECeremonyErrType_SequenceErr,           // 新人编号错误
    ECeremonyErrType_InOtherRoom,           // 在其他房间中
    ECeremonyErrType_CreatorNotInRoom,      // 房主不在房间中
    ECeremonyErrType_CreatorIsKicked,       // 申请人被提出舞团
    ECeremonyErrType_GroupDismissed,        // 舞团已经申请解散
    ECeremonyErrType_PraiseCD,              // 入团仪式点赞冷却时间
};


enum ECeremonyPlayerType
{
    ECeremonyPlayerType_None,			//普通观众
    ECeremonyPlayerType_Applicant,		//仪式申请人
    ECeremonyPlayerType_Rookie,			//仪式新人
};


enum ECeremonyRoomQuitType
{
    ECeremonyRoomQuitType_Quit = 0,
    ECeremonyRoomQuitType_Kicked,
    ECeremonyRoomQuitType_RoomClose,
    ECeremonyRoomQuitType_ExitDanceGroup,
    ECeremonyRoomQuitType_KickedFromDanceGroup,
};

enum ECeremonyRoomQuitTo
{
    ECeremonyRoomQuitTo_World = 0,
    ECeremonyRoomQuitTo_Dungeon,
};

enum ECeremonyState
{
	ECeremonyState_None,		//未申请仪式
	ECeremonyState_Prepare,	//仪式已申请-准备中时间 等待新人写自我介绍
	ECeremonyState_Start,		//仪式已开始-播放动画 点赞时间
};


enum ECeremonyRoomCloseReason
{
	ECeremonyRoomCloseReason_ByCreator = 0,		// 创建者关闭
	ECeremonyRoomCloseReason_TimeEnd,			// 时间结束
	ECeremonyRoomCloseReason_Empty,				// 房间没人了
};

enum ECeremonyNewMemberSeq
{
    ECeremonyNewMemberSeq_Min                   = 1,    // 仪式新人的排序最小值 
    ECeremonyNewMemberSeq_Max                   = 8     // 仪式新人排序的最大值
};


// 入团仪式日志房间开启信息
class CeremonyLogRoomInfo
{
public:
    CeremonyLogRoomInfo();
    ~CeremonyLogRoomInfo();

    void DoDecode(CParamPool &IOBuff);
    void DoEncode(CParamPool &IOBuff);

    unsigned int m_nLogID;                 // 跟某一次的房间对应
    unsigned int m_nCreatorID;
    std::string m_strCreatorName;
    unsigned char m_nCreatorVip;
    unsigned int m_logTime;                 
};

// 入团仪式日志房间信息数据库对应关系
class CeremonyLogInfoDb
{
public:
    unsigned int m_nGroupID;
    CeremonyLogRoomInfo m_info;
};

// 入团仪式日志新人信息
class CeremonyLogNewerInfo
{
public:
    CeremonyLogNewerInfo();
    ~CeremonyLogNewerInfo();

    void DoEncode(CParamPool &IOBuff);
    void DoDecode(CParamPool &IOBuff);

// private:
    unsigned int m_nRoleID;
    std::string m_strRoleName;
    unsigned char m_nVipLv;
};

// 入团仪式日志投票人单次投票信息
class CeremonyLogVoterInfo
{
public:
    CeremonyLogVoterInfo();
    ~CeremonyLogVoterInfo();

    void DoEncode(CParamPool &IOBuff);
    void DoDecode(CParamPool &IOBuff);


    void Set(unsigned int nPraiserID, std::string strName, unsigned char vipLev, unsigned short count, unsigned int now);
    unsigned int m_nPraiserID;                  // 点赞人的ID
    std::string m_strPraiserName;               // 点赞人的名字
    unsigned char m_nPraiserVipLv;              // 点赞人的vip等级(当时的)
    unsigned short m_nPraiserCount;             // 点赞的个数
    unsigned int m_logTime;                     // 
};

// 入团仪式日志单次房间开启中，某个新人被投票信息汇总
class CeremonyPraiseLogInfo
{
public:
    CeremonyPraiseLogInfo();
    ~CeremonyPraiseLogInfo();

    // 存在则更新，否则添加
    void AddVoterInfo(CeremonyLogVoterInfo &voterInfo);

    void DoEncode(CParamPool &IOBuff);
    void DoDecode(CParamPool &IOBuff);

    CeremonyLogNewerInfo m_newerInfo;     // 
    std::vector<CeremonyLogVoterInfo> m_vecVoterInfo; // 
};

// 入团仪式日志 单次投票信息数据库记录
class CeremonyPraiseLogInfoDb
{
public:
    unsigned int m_nLogID;                      // 跟房间对应
    CeremonyLogNewerInfo m_newerInfo;     // 
    CeremonyLogVoterInfo m_voterInfo;     //     
};

// 按照舞团组织的Log，代表某次仪式，所有新人的被投票记录
class DanceGroupCeremonyLog
{
public:
    DanceGroupCeremonyLog();
    ~DanceGroupCeremonyLog();

    void DoEncode(CParamPool &IOBuff);
    void DoDecode(CParamPool &IOBuff);

    CeremonyLogRoomInfo m_roomBaseLog;             // 
    std::vector<CeremonyPraiseLogInfo> m_voteInfo; // 
};

// 某个舞团，所有的记录
class DanceGroupCeremonyLogs
{
public:
    DanceGroupCeremonyLogs();
    ~DanceGroupCeremonyLogs();

    unsigned int m_nGroupID;
    typedef std::vector<DanceGroupCeremonyLog> DanceGroupCeremonyLogVec;
    // 最新的在最前边
    DanceGroupCeremonyLogVec m_vecLog; 
};
// <dancegroupID, logs>
typedef std::map<unsigned int, DanceGroupCeremonyLogs> DanceGroupCeremonyLogMap;

class DanceGroupCeremonyLogsProcessor
{
public:
    DanceGroupCeremonyLogsProcessor();
    ~DanceGroupCeremonyLogsProcessor();

    // 把一条日志添加到整个map中
    static void InsertCeremonyLog(unsigned int groupID, DanceGroupCeremonyLog &log, 
        std::map<unsigned, DanceGroupCeremonyLogs> &logMap); 
    // 向整个仪式日志中塞入一条投票信息
    static void InsertCeremonyPraiseItem(DanceGroupCeremonyLog &roomLog, CeremonyPraiseLogInfoDb &info); 

};

// 新人
class CeremonyNewMember
{
public:
    CeremonyNewMember();
    ~CeremonyNewMember();

    unsigned int m_nRoleID;			// player id.
    unsigned char m_nSeq;			// 新人排序
    unsigned short m_nPraisedNum;	// 被点赞次数

    CeremonyPraiseLogInfo m_logs;   // 某新人被点赞的记录
};

// 包含所有人
class CeremonyNormalMember
{
public:
    CeremonyNormalMember();
    ~CeremonyNormalMember();

    unsigned int m_nRoleID;
    unsigned short m_nPraiseNum;		// 已经使用的点赞次数（本次）
};




#endif // __CEREMONYROOMHEAD_H__





