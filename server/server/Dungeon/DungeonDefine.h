#ifndef __DUNGEONDEFNE_H__
#define __DUNGEONDEFNE_H__

#include "../../datastructure/DataStruct_Base.h"
#include  <map>
#include <vector>

#define  PARCLOSE_FRIENDRANK_PAGENUM 5
#define  ITEM_PARCLOSEEFFECT_ADDMARK 31112
#define  ITEM_PARCLOSEEFFECT_MISSTOCOOL 31113
#define  ITEM_PARCLOSEEFFECT_COOLTOPERFECT 31114
#define  ITEM_PARCLOSEEFFECT_RATE 50

enum eDungeonError
{
    EDungeonError_Sucess,              // 成功
    EDungeonError_StarConfigErr,       // 许愿星奖励配置错误
    EDungeonError_RewardHaved,         // 奖励已经领取
    EDungeonError_StarNotEnough,       // 许愿星不够
    EDungeonError_DustConfigErr,       // 魔法尘埃奖励配置错误
    EDungeonError_DustNotEnough,       // 魔法尘埃不够
    EDungeonError_NotFindDungeonID,    // 副本没找到
    EDungeonError_NotFindParcloseID,   // 关卡没找到
    EDungeonError_PlayerNotOnLine,     // 玩家没在线
    EDungeonError_NotFriend,           // 不是玩家朋友
    EDungeonError_BuyCrystalConfigErr, // 购买水晶配置错误
    EDungeonError_CrystalConfigErr,	   // 水晶配置错误
    EDungeonError_CrystalIsFull,	   // 水晶满了
    EDungeonError_MoneyNotEnough,	   // 钱不够
    EDungeonError_ParcloseNotActived,  // 关卡未激活
    EDungeonError_ParcloseHasActived,  // 关卡已经激活
    EDungeonError_ParcloseNotCanSkip,  // 关卡不能跳过
    EDungeonError_ItemNotEnough,       // 道具不够
    EDungeonError_DungeonPNULL,        // 关卡指针空
    EDungeonError_DungeonCreateNew,    // 创建关卡New失败
    EDungeonError_InRoom,              // 已经在房间里面
    EDungeonError_InCoupleRoom,        // 已经在婚房
    EDungeonError_InPhoteRoom,         // 已经在照相房间
    EDungeonError_InAmuseRoom,         // 已经在开放场景
    EDungeonError_FriendRefuse,        // 好友拒绝
    EDungeonError_FriendNotOnLine,     // 好友不在线
    EDungeonError_NotFindStoryConfig,  // 剧情配置未找到
    EDungeonError_CrystalNotEnough,    // 水晶不够
    EDungeonError_InviteFriendTimeOut, // 邀请好友时间到
    EDungeonError_StageNotFind,        // 资源未找到
    EDungeonError_BuyCrystalMuch,      // 购买水晶超过上限
    EDungeonError_DebrisConfigErr,     // 碎片配置错误
    EDungeonError_DebrisCountNot,      // 碎片数目不够
    EDungeonError_InviteFriendCountNot,// 邀请好友的次数不够了
    EDungeonError_FriendHasInvited,    // 好友已经被邀请过了
    EDungeonError_FriendHasGiveVit,    // 好友已经被赠送过体力了
    EDungeonError_NotVitGet,           // 没有体力能够被领取的体力
    EDungeonError_NotHave3Star,        // 关卡没有3星，不能扫荡
    EDungeonError_NotQuickFinishNum,   // 关卡扫荡次数已经用完
    EDungeonError_NotVip,              // 不是VIP
    EDungeonError_InCeremonyRoom,      // 在舞团仪式房间
    EDungeonError_TodayGetVitMax,      // 今天领取值超过上限
    EDungeonError_ParcloseNotPassed,   // 关卡未通过
};

enum eStartParcloseErr
{
    ESPE_NotFindParcloseID   = 1,
    ESPE_ParcloseNotActived,
    ESPE_ItemNotEnough,
    ESPE_NotFriend,
    ESPE_InRoom,
    ESPE_InCoupleRoom,
    ESPE_InPhoteRoom,
    ESPE_InAmuseRoom,
    ESPN_NewRoomErr,
    ESPN_FrindNotOnline,
    ESPN_FriendRefuse,
    ESPN_InviteFriendTimeOut,
    ESPN_STAGNOTFIND,
};

enum eDungeonState
{
    eDungeonState_Playing = 0,
    eDungeonState_Ending,
    eDungeonState_End,
};

enum eStoryType
{
    eStoryType_Dust = 1,             // 擦神灯剧情
    eStoryType_Dungeon,              // 首次进入副本建筑物剧情
    eStoryType_ParcloseBegin ,       // 首次进入关卡剧情
    eStory_ParcloseEnd,				 // 首次挑战关卡成功剧情
};



// 排行榜每项信息
class CDungeonRank
{
public:
    CDungeonRank();
    virtual ~CDungeonRank(){};

public:
    unsigned int m_nFriendID;
    std::string  m_strFriendName;
    unsigned int m_nRank;
    unsigned int m_nStarCount;
    std::string  m_strDungeonName;
    std::string  m_strParcloseName;
};


class CStarRewardConfig
{
public:
    CStarRewardConfig();
    virtual ~CStarRewardConfig(){};

public:
    unsigned int      m_nID;                 // 奖励ID
    unsigned int      m_nCostStar;           // 消耗许愿星
    std::list<CItem>  m_listMaleReward;      // 男性奖励列表
    std::list<CItem>  m_listFemaleReward;    // 女性奖励列表
    unsigned int      m_nMoney;              // 金钱奖励
};
typedef std::map<unsigned int,CStarRewardConfig> StarRewardInfoTable;

class CDustRewardConfig
{
public:
    CDustRewardConfig();
    virtual ~CDustRewardConfig(){};

public:
    unsigned int     m_nCostDust;
    std::string      m_strActivityName;
    std::string      m_strIconName;
    std::list<CItem> m_listMaleReward;
    std::list<CItem> m_listFemaleReward;
    unsigned int     m_nMoney;
    unsigned int     m_nStoryID;
};

struct DebrisReward
{
    unsigned int nDebrisID;
    unsigned int nDebrisCount;

    DebrisReward()
    {
        nDebrisID = 0;
        nDebrisCount = 0;
    }
};

class CMagicLampRewardMail
{
public:
    CMagicLampRewardMail();
    virtual ~CMagicLampRewardMail(){};

public:
    std::string m_strStarRewardTitle;
    std::string m_strStarRewardContent;
    std::string m_strDustRewardTitle;
    std::string m_strDustRewardContent;
    std::string m_strParcloseDropTitle;
    std::string m_strParcloseDropContent;
};

class CMagicLampDesc
{
public:
    CMagicLampDesc();
    virtual ~CMagicLampDesc(){};

public:
    std::string m_strTitle;
    std::string m_strContent;
    unsigned int m_nMaxInviteFriendNum;
    std::vector<std::string> m_vecCoverPhoto;
};

class CCrystalConfig
{
public:
    CCrystalConfig();
    virtual ~CCrystalConfig(){};

public:
    unsigned int    m_nInitialValue;
    unsigned int    m_nMaxValue;
    unsigned int    m_nRestoreTime;
    unsigned int    m_nMinDustCount;
    unsigned int    m_nMaxDustCount;
    unsigned int    m_nFavorableBuyVitCount;
};

class CBuyCrystalConfig
{
public:
    CBuyCrystalConfig();
    virtual ~CBuyCrystalConfig(){};

public:
    void doEncode(CParamPool & IOBuff);

public:
    unsigned int  m_nIndex;
    std::string   m_strIconName;
    unsigned int  m_nCrystalCount;
    unsigned int  m_nPrice;
    unsigned int  m_nFavorablePrice;
};
typedef std::map<int,CBuyCrystalConfig> BuyCrystalConfigTable;


// 宣传图
class CPublicityPictureConfig
{
public:
    CPublicityPictureConfig(){};
    virtual ~CPublicityPictureConfig(){};

public:
    std::string m_strIcon;
    std::string m_strAtlas;
};

class CRoomEndPlayerScoreInfo
{
public:
    CRoomEndPlayerScoreInfo();
    virtual ~CRoomEndPlayerScoreInfo(){};

public:
    int	m_nPos;
    unsigned int m_nScore;
    std::string m_strName;
};

class ParcloseReward
{
public:
    ParcloseReward();
    virtual ~ParcloseReward();

public:
    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);

public:
    std::list<CItem>                m_listItem;
    unsigned int                    m_nExp;
    std::list<DebrisReward>         m_listDebris;
};


#endif

