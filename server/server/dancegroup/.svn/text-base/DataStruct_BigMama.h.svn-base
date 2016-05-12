#ifndef __LIBSERVERFRAME_DATASTRUCT_BIGMAMA_H__
#define __LIBSERVERFRAME_DATASTRUCT_BIGMAMA_H__

#include <string>
#include <map>

class CParamPool;

enum EBigMamaState
{
    EBigMamaState_NotOpen = 0,		//活动未开始
    EBigMamaState_InProgress,		//进行中
    EBigMamaState_Success,		//挑战成功
};


enum EFightBigMamaResult
{
    EFightBigMamaResult_Sucess = 0,
    EFightBigMamaResult_ActiveNotOpen,
    EFightBigMamaResult_FightOver,          // 挑战已经结束，处于领奖状态
    EFightBigMamaResult_NoDanceGroup,
    EFightBigMamaResult_LoadStageFailed,
    EFightBigMamaResult_InOtherRoom,        // 在其他房间中
    EFightBigMamaResult_NotMengXin,         // 不能是小萌新
    EFightBigMamaResult_InternalError,      // 服务器错误
};



enum EBigMamaPrizeState
{
    EBigMamaPrizeState_NoPrize = 0,             // 无奖励            
    EBigMamaPrizeState_HasPrize,                // 有奖励，未领取
    EBigMamaPrizeState_ReceivePrize,            // 有奖励，已经领取
};

// 领取奖励错误返回
enum EBigMamaRewardResult
{
    EBigMamaRewardResult_Sucess = 0,
    EBigMamaRewardResult_ActiviteClosed,        // 活动已经关闭
    EBigMamaRewardResult_NoPrize,               // 没有奖励
    EBigMamaRewardResult_HasRecived,            // 已经领取过大妈奖励
    EBigMamaRewardResult_HasRecivedGuidePrize,  // 已经领取过引导奖励
    EBigMamaRewardResult_NotMengXin,            // 不能是小萌新
    EBigMamaRewardResult_InternalError,         // 服务器错误
};


// 活动的状态类型
enum EBigMamaActiveState
{
    EBigMamaActiveState_Running = 1,      // 进行中
    EBigMamaActiveState_AllBeat,          // 全部击败
};

// 大妈最后被击败的类型
enum EBigMamaLastBeatType
{
    EBigMamaLastBeatType_XiaoLian = 0,
    EBigMamaLastBeatType_XiaoRuan,
    EBigMamaLastBeatType_GroupMember,
};


// 一个舞团的大妈信息
class DanceGroupBigMamaInfo
{
public:
    DanceGroupBigMamaInfo();
    ~DanceGroupBigMamaInfo();

    unsigned int m_nDanceGroupID;           // 舞团ID

    unsigned int m_nStamp;                  // 时间戳
    unsigned char m_nCurrentRound;          // 第几波
    unsigned int m_nCurrentStageID;         // 当前大妈对应的关卡
    EBigMamaActiveState m_nState;           // 活动状态

    EBigMamaLastBeatType m_nBeaterType;     // 击杀类型
    unsigned int m_nBeaterRoleID;           // 击杀者
    std::string m_strBeaterName;            // 击杀者的名字
};
typedef std::map<unsigned int, DanceGroupBigMamaInfo> DanceGroupBigMamaMap;


class BigMamaProperty
{
public:
    BigMamaProperty();
    ~BigMamaProperty();
    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);

    // 所有大妈是否都挑战完成
    bool m_bBigMamaAllLose;
    // 当前第几轮挑战
    unsigned short m_nCurRound;
    // 当前挑战模式
    unsigned char m_nCurMode;
    // 目标分数
    unsigned int m_nTargetScore;
    // 最后完成挑战者类型: 0-小恋 1-小软 2-舞团成员
    EBigMamaLastBeatType m_nBeaterType;
    // 最后一轮挑战成功的玩家名
    std::string m_strRoleName;
};



#endif // __LIBSERVERFRAME_DATASTRUCT_BIGMAMA_H__