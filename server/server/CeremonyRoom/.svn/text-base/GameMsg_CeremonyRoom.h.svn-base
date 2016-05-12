#ifndef	__GAMEMSG_CEREMONYROOM_H__
#define	__GAMEMSG_CEREMONYROOM_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "ceremonyRoomHead.h"


/// 点赞记录
class CeremonyPraiseSendInfo
{
public:
    CeremonyPraiseSendInfo();
    void DoEncode(CParamPool &IOBuff);
    void DoDecode(CParamPool &IOBuff);

    /// 点赞的玩家ID
    unsigned int m_nRoleID;
    /// 点赞的玩家名字
    std::string m_strRoleName;
    /// 玩家是否是VIP
    bool m_bIsVIP;
    /// 玩家VIP等级
    unsigned short m_nVIPLevel;
    /// 点赞数
    int m_nPraiseCount;

};

/// 新人获得的点赞记录
class CeremonyPraiseGainedInfo
{
public:
    CeremonyPraiseGainedInfo();
    void DoEncode(CParamPool &IOBuff);
    void DoDecode(CParamPool &IOBuff);

    /// 获得点赞的玩家ID
    unsigned int m_nRoleID;
    /// 获得点赞的玩家名字
    std::string m_strRoleName;
    /// 玩家是否是VIP
    bool m_bIsVIP;
    /// 玩家VIP等级
    unsigned short m_nVIPLevel;
    /// 获得点赞数
    int m_nPraiseCount;
};

/// 仪式日志信息
class CeremonyRecordInfo
{
public:
    CeremonyRecordInfo();
    bool DoEncode(CParamPool &IOBuff);
    bool DoDecode(CParamPool &IOBuff);

    // recordID
    unsigned int m_nLogID;
    /// 入团仪式申请人ID
    unsigned int m_nApplicantID;
    /// 入团仪式申请人名字
    std::string m_strApplicantName;
    /// 是否是VIP
    bool m_bIsVIP;
    /// VIP等级
    unsigned short m_nVIPLevel;
    /// 入团仪式日期
    unsigned int m_nCeremonyDate;
    /// 仪式新人点赞记录
    std::vector<CeremonyPraiseGainedInfo> m_vecPraiseInfo;
};



// 申请创建房间
class GameMsg_S2G_CeremonyApplyCreate : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyApplyCreate);
    GameMsg_S2G_CeremonyApplyCreate();
    ~GameMsg_S2G_CeremonyApplyCreate() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nDanceGroupID;
    unsigned int m_nRoleID;
    int m_nLine;
};

// 申请创建房间结果
class GameMsg_G2S_CeremonyApplyCreateRet : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_CeremonyApplyCreateRet);
    GameMsg_G2S_CeremonyApplyCreateRet();
    ~GameMsg_G2S_CeremonyApplyCreateRet() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nDanceGroupID;
    unsigned int m_nRoleID;
    unsigned char m_nErrorType; // 
};

// 房间成功创建通知
class GameMsg_S2G_CeremonyCreateRoomNotify : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyCreateRoomNotify);
    GameMsg_S2G_CeremonyCreateRoomNotify();
    ~GameMsg_S2G_CeremonyCreateRoomNotify() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nDanceGroupID;
    unsigned int m_nCreateTime;
    unsigned int m_nOldCeremonyTimes;
    unsigned int m_nLine;
    std::string m_strLineName;
};

// 取消房间创建申请 
class GameMsg_S2G_CeremonyApplyCreateCancel : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyApplyCreateCancel);
    GameMsg_S2G_CeremonyApplyCreateCancel();
    ~GameMsg_S2G_CeremonyApplyCreateCancel() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nDanceGroupID;
    unsigned int m_nCreateID;
};

// 房间关闭申请
class GameMsg_S2G_CeremonyCloseRoomNotify : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyCloseRoomNotify);
    GameMsg_S2G_CeremonyCloseRoomNotify();
    ~GameMsg_S2G_CeremonyCloseRoomNotify() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nDanceGroupID;
    unsigned char m_nCloseReason; // 关闭原因
};

class MsgStateChange
{
public:
    MsgStateChange();
    ~MsgStateChange();

    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);

    unsigned int m_nDanceGroupID;
    unsigned char m_nNewState;
};

// 房间状态变更通知
class GameMsg_S2G_CeremonyRoomStateChange : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyRoomStateChange);
    GameMsg_S2G_CeremonyRoomStateChange();
    ~GameMsg_S2G_CeremonyRoomStateChange() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    MsgStateChange m_msg;
};

class GameMsg_G2S_CeremonyRoomStateChange : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_CeremonyRoomStateChange);
    GameMsg_G2S_CeremonyRoomStateChange();
    ~GameMsg_G2S_CeremonyRoomStateChange() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    MsgStateChange m_msg;
};

class GameMsg_S2G_CeremonyRoomNewMember : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyRoomNewMember);
    GameMsg_S2G_CeremonyRoomNewMember();
    ~GameMsg_S2G_CeremonyRoomNewMember();

public:
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

    unsigned int m_nGroupID;
    std::vector<unsigned int> m_vecNewMember; // 这些人是新人，需要变成老团员
};

// s->g 写日志信息 分为三步(开始写，写入每个新人的被投票记录，写入结束)
class GameMsg_S2G_CeremonyRoomLogsStart : public GameMsg_Base
{
    //    MSG_S2G_CeremonyRoomLogsStart,				    // 日志记录
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyRoomLogsStart);
    GameMsg_S2G_CeremonyRoomLogsStart();
    ~GameMsg_S2G_CeremonyRoomLogsStart();

    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

    int m_nLine;                                        // 
    unsigned int m_nLogSeq;                                      // 序列
    unsigned int m_nGroupID;

    CeremonyLogRoomInfo m_roomInfo;                     // 
};

class GameMsg_S2G_CeremonyRoomLogsItem : public GameMsg_Base
{
    //    MSG_S2G_CeremonyRoomLogsItem,                   // 每个人的被投票人的信息
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyRoomLogsItem);
    GameMsg_S2G_CeremonyRoomLogsItem();
    ~GameMsg_S2G_CeremonyRoomLogsItem();

    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

    int m_nLine;
    unsigned int m_nLogSeq;

    CeremonyPraiseLogInfo m_logItem;
};

class GameMsg_S2G_CeremonyRoomLogsEnd : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyRoomLogsEnd);
    GameMsg_S2G_CeremonyRoomLogsEnd();
    ~GameMsg_S2G_CeremonyRoomLogsEnd();

    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

    int m_nLine;
    unsigned int m_nLogSeq;
};


/// C2S 请求仪式日志
class GameMsg_C2S_RequestCeremonyRecord : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestCeremonyRecord);
    GameMsg_C2S_RequestCeremonyRecord();
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

    /// 第几条日志
    int m_nRecordIndex;
};

/// S2C 请求仪式日志结果
class GameMsg_S2C_RequestCeremonyRecordRes : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RequestCeremonyRecordRes);
    GameMsg_S2C_RequestCeremonyRecordRes();
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

    /// 当前第几条日志
    int m_nCurIndex;
    /// 日志总数
    int m_nTotleCount;
    /// 日志信息
    /// ligh 添加了ID
    CeremonyRecordInfo m_recordInfo;
};


/// C2S 请求点赞信息
class GameMsg_C2S_RequestCeremonyPraise : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestCeremonyPraise);
    GameMsg_C2S_RequestCeremonyPraise();
    virtual bool doDecode(CParamPool &IOBuff);
    virtual bool doEncode(CParamPool &IOBuff);

    unsigned int m_nLogID;
    /// 玩家ID
    unsigned int m_nRoleID;
    /// 请求页数
    int m_nPage;
    /// 请求个数
    int m_nCount;
};

/// S2C 请求点赞信息结果
class GameMsg_S2C_RequestCeremonyPraiseRes : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RequestCeremonyPraiseRes);
    GameMsg_S2C_RequestCeremonyPraiseRes();
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

    unsigned int m_logID;
    /// 玩家ID
    unsigned int m_nRoleID;
    /// 当前页码
    int m_nCurPage;
    /// 点赞信息总数
    int m_nTotleCount;
    /// 点赞信息列表
    std::vector<CeremonyPraiseSendInfo> m_vecPraiseInfo;
};


/// S2C 同步入团仪式状态
class GameMsg_S2C_SyncCeremonyState : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_SyncCeremonyState);

    GameMsg_S2C_SyncCeremonyState();
    ~GameMsg_S2C_SyncCeremonyState(){}
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

    /// 仪式状态CeremonyState
    unsigned char m_nCeremonyState;
    int m_nLineID;
    /// 举办仪式线名
    std::string m_strLineName;
};

// 新加 上线时候发送
// 入团仪式配置信息
class GameMsg_S2C_SyncCeremonyConfig : public GameMsg_Base
{
public:
    GameMsg_S2C_SyncCeremonyConfig();
    ~GameMsg_S2C_SyncCeremonyConfig();

    virtual bool doEncode(CParamPool &IOBuff);

    unsigned char m_nFreeApplyTimes;
    unsigned char m_nApplyMoneyType;
    unsigned int m_nApplyMoneyCount;

    unsigned char m_nFreePraiseTimes;
    unsigned char m_nPraiseMoneyType;
    unsigned int m_nPraiseMoneyCount;

    float m_fStartDuration;          // 启动后点赞的时间
};


// 新加
// 入团仪式已经使用次数通知
class GameMsg_S2C_SyncCermonyApplyTimes : public GameMsg_Base
{
public:
    GameMsg_S2C_SyncCermonyApplyTimes();
    ~GameMsg_S2C_SyncCermonyApplyTimes();
    virtual bool doEncode(CParamPool &IOBuff);

    unsigned char m_nUsedTimes;
};

//      MSG_S2G_CeremonyRoomStatics,                    // 请求glbal数据统计
//     MSG_G2S_CeremonyRoomStatics,                    // 回复数据统计
class GameMsg_S2G_CeremonyRoomStatics : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonyRoomStatics);
    GameMsg_S2G_CeremonyRoomStatics();
    ~GameMsg_S2G_CeremonyRoomStatics();

    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

};

class GameMsg_G2S_CeremonyRoomStatics : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_CeremonyRoomStatics);
    GameMsg_G2S_CeremonyRoomStatics();
    ~GameMsg_G2S_CeremonyRoomStatics();

    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

    std::string m_strStatics; // 统计结果
};

class CeremonyRoomSyncInfo
{
public:
    CeremonyRoomSyncInfo();
    ~CeremonyRoomSyncInfo() {}

    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

    unsigned int m_nRoomID; // 对于房间来说就是dancegroupID.
    unsigned char m_state; //
    unsigned int m_createTime;			// 仪式举办时间
};

// 同步房间信息
class GameMsg_S2G_CeremonySyncRoomInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CeremonySyncRoomInfo);
    GameMsg_S2G_CeremonySyncRoomInfo();
    ~GameMsg_S2G_CeremonySyncRoomInfo();

    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int m_nLine;
    std::string m_strLineName;

    std::vector<CeremonyRoomSyncInfo> m_vecRooms;
};


#endif // __GAMEMSG_CEREMONYROOM_H__


