#ifndef	__GAMEMSG_S2C_DANCE_GROUP_H__
#define	__GAMEMSG_S2C_DANCE_GROUP_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "../../datastructure/DanceGroupDataMgr.h"
#include <list>

class CParamPool;
class CDanceGroupInfo;

class GameMsg_S2C_CreateDanceGroupResult : public GameMsg_Base
{
public:
    GameMsg_S2C_CreateDanceGroupResult();
    ~GameMsg_S2C_CreateDanceGroupResult() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
    unsigned int                m_nCreateCD;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_RequestEnterDanceGroupResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RequestEnterDanceGroupResult);
    GameMsg_S2C_RequestEnterDanceGroupResult();
    ~GameMsg_S2C_RequestEnterDanceGroupResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
    float                       m_fApplyCDTime;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_CancelRequestEnterDanceGroupResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_CancelRequestEnterDanceGroupResult);
    GameMsg_S2C_CancelRequestEnterDanceGroupResult();
    ~GameMsg_S2C_CancelRequestEnterDanceGroupResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_EnterDanceGroupSuccess : public GameMsg_Base
{
public:
    GameMsg_S2C_EnterDanceGroupSuccess();
    ~GameMsg_S2C_EnterDanceGroupSuccess() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int                m_nGroupID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_GetDanceGroupInfoResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetDanceGroupInfoResult);
    GameMsg_S2C_GetDanceGroupInfoResult();
    ~GameMsg_S2C_GetDanceGroupInfoResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_SendDanceGroupInfo : public GameMsg_Base
{
public:
    GameMsg_S2C_SendDanceGroupInfo();
    ~GameMsg_S2C_SendDanceGroupInfo();

public:
    bool doEncode(CParamPool &IOBuff);

public:
    CDanceGroupInfo             *m_pDGInfo;
};

//////////////////////////////////////////////////////////////////////////

class CDanceGroupItem
{
public:
    CDanceGroupItem();
    ~CDanceGroupItem() {}

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);

public:
    unsigned int                m_nGroupID;             // 舞团ID
    std::string                 m_strGroupName;         // 舞团名称
    unsigned int                m_nLeaderRoleID;        // 舞团团长RoleID
    std::string                 m_strLeaderRoleName;    // 舞团团长名称
    unsigned short              m_nLevel;               // 舞团等级
    unsigned short              m_nBadge;               // 舞团徽章
    unsigned int                m_nHonor;               // 舞团荣誉
    std::string                 m_strProfile;           // 舞团简介
    unsigned int                m_nCreateTime;          // 创建日期
    bool                        m_bCanRequestEnter;     // 是否可以申请加入
    bool                        m_bHaveRequest;         // 是否已经申请
    unsigned short              m_nMemberCount;         // 成员数
    unsigned short              m_nMaxMemberCount;      // 最大成员数
    bool                        m_bLeaderIsVIP;         // 团长是否VIP 
    unsigned short              m_nLeaderVIPLevel;      // 团长VIP等级
    unsigned short              m_nEffectID;            // 舞团特效
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_SendSimpleDanceGroupInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_SendSimpleDanceGroupInfo);
    GameMsg_S2C_SendSimpleDanceGroupInfo();
    ~GameMsg_S2C_SendSimpleDanceGroupInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    CDanceGroupItem             m_DGItem;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_BeFefuseRefresh : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_BeFefuseRefresh);
    GameMsg_S2C_BeFefuseRefresh();
    ~GameMsg_S2C_BeFefuseRefresh() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    CDanceGroupItem             m_DGItem;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_SendDanceGroupInfoList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_SendDanceGroupInfoList);
    GameMsg_S2C_SendDanceGroupInfoList();
    ~GameMsg_S2C_SendDanceGroupInfoList() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int                m_nBeginIndex;          // 开始条目
    unsigned short              m_nCount;               // 结束条目
    unsigned int                m_nTotal;               // 总条目数
    std::list<CDanceGroupItem>  m_DGItemList;           // 舞团信息列表
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ExitDanceGroupResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ExitDanceGroupResult);
    GameMsg_S2C_ExitDanceGroupResult();
    ~GameMsg_S2C_ExitDanceGroupResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ChangeDanceGroupLeaderResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeDanceGroupLeaderResult);
    GameMsg_S2C_ChangeDanceGroupLeaderResult();
    ~GameMsg_S2C_ChangeDanceGroupLeaderResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_DismissDanceGroupResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DismissDanceGroupResult);
    GameMsg_S2C_DismissDanceGroupResult();
    ~GameMsg_S2C_DismissDanceGroupResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_CancelDismissDanceGroupResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_CancelDismissDanceGroupResult);
    GameMsg_S2C_CancelDismissDanceGroupResult();
    ~GameMsg_S2C_CancelDismissDanceGroupResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ChangeDanceGroupTitleResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeDanceGroupTitleResult);
    GameMsg_S2C_ChangeDanceGroupTitleResult();
    ~GameMsg_S2C_ChangeDanceGroupTitleResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_KickOutDanceGroupMemberResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_KickOutDanceGroupMemberResult);
    GameMsg_S2C_KickOutDanceGroupMemberResult();
    ~GameMsg_S2C_KickOutDanceGroupMemberResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_KickedOutDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_KickedOutDanceGroup);
    GameMsg_S2C_KickedOutDanceGroup();
    ~GameMsg_S2C_KickedOutDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ChangeDanceGroupColorResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeDanceGroupColorResult);
    GameMsg_S2C_ChangeDanceGroupColorResult();
    ~GameMsg_S2C_ChangeDanceGroupColorResult(){}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ChangeDanceGroupBadgeResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeDanceGroupBadgeResult);
    GameMsg_S2C_ChangeDanceGroupBadgeResult();
    ~GameMsg_S2C_ChangeDanceGroupBadgeResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ChangeDanceGroupTitleNameResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeDanceGroupTitleNameResult);
    GameMsg_S2C_ChangeDanceGroupTitleNameResult();
    ~GameMsg_S2C_ChangeDanceGroupTitleNameResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ChangeDanceGroupProfileResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeDanceGroupProfileResult);
    GameMsg_S2C_ChangeDanceGroupProfileResult();
    ~GameMsg_S2C_ChangeDanceGroupProfileResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ChangeDanceGroupAnnouncementResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeDanceGroupAnnouncementResult);
    GameMsg_S2C_ChangeDanceGroupAnnouncementResult();
    ~GameMsg_S2C_ChangeDanceGroupAnnouncementResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_DanceGroupRemoved : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DanceGroupRemoved);
    GameMsg_S2C_DanceGroupRemoved();
    ~GameMsg_S2C_DanceGroupRemoved() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_UpdateDanceGroupBaseInfo : public GameMsg_Base
{
public:
    GameMsg_S2C_UpdateDanceGroupBaseInfo();
    ~GameMsg_S2C_UpdateDanceGroupBaseInfo() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    CDanceGroupBaseInfo         m_DGBaseInfo;           // 基本信息
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_AddDanceGroupMember : public GameMsg_Base
{
public:
    GameMsg_S2C_AddDanceGroupMember();
    ~GameMsg_S2C_AddDanceGroupMember() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    CDanceGroupMember           m_DGMember;             // 舞团成员信息
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_UpdateDanceGroupMemberInfo : public GameMsg_Base
{
public:
    GameMsg_S2C_UpdateDanceGroupMemberInfo();
    ~GameMsg_S2C_UpdateDanceGroupMemberInfo() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    CDanceGroupMember           m_DGMember;             // 舞团成员信息
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_RemoveDanceGroupMember : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RemoveDanceGroupMember);
    GameMsg_S2C_RemoveDanceGroupMember();
    ~GameMsg_S2C_RemoveDanceGroupMember() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int                m_nRoleID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_NotifyRequestEnterDanceGroup : public GameMsg_Base
{
public:
    GameMsg_S2C_NotifyRequestEnterDanceGroup();
    ~GameMsg_S2C_NotifyRequestEnterDanceGroup() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    CRequestDanceGroupInfo      m_RequestInfo;          // 申请信息
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_NotifyRemoveRequestEnter : public GameMsg_Base
{
public:
    GameMsg_S2C_NotifyRemoveRequestEnter();
    ~GameMsg_S2C_NotifyRemoveRequestEnter() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int                m_nRoleID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ReplyRequestEnterDanceGroupResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ReplyRequestEnterDanceGroupResult);
    GameMsg_S2C_ReplyRequestEnterDanceGroupResult();
    ~GameMsg_S2C_ReplyRequestEnterDanceGroupResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int                m_nRequestRoleID;       // 申请角色ID
    unsigned short              m_nResult;              // 结果
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ReplyRequestListEnterDanceGroupResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ReplyRequestListEnterDanceGroupResult);
    GameMsg_S2C_ReplyRequestListEnterDanceGroupResult();
    ~GameMsg_S2C_ReplyRequestListEnterDanceGroupResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int                m_nRoleID;
    unsigned int                m_nAllowCount;
    unsigned short              m_nResult;
    bool                        m_bAllow;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_DonateDanceGroupResourceResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DonateDanceGroupResourceResult);
    GameMsg_S2C_DonateDanceGroupResourceResult();
    ~GameMsg_S2C_DonateDanceGroupResourceResult() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_UpgradeDanceGroupResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_UpgradeDanceGroupResult);
    GameMsg_S2C_UpgradeDanceGroupResult();
    ~GameMsg_S2C_UpgradeDanceGroupResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;              // 结果
    unsigned short              m_nCurLevel;            // 当前等级
    unsigned int                m_nNextLevelUpResA;     // 升级所需资源A
    unsigned int                m_nNextLevelUpResB;     // 升级所需资源B
    unsigned int                m_nNextLevelUpResC;     // 升级所需资源C
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ChangeDanceGroupNameResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeDanceGroupNameResult);
    GameMsg_S2C_ChangeDanceGroupNameResult();
    ~GameMsg_S2C_ChangeDanceGroupNameResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
    std::string                 m_strNewGroupName;      // 舞团新名称
    unsigned int                m_nTimeCoolDown;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_UpdateDanceGroupAnnounce: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_UpdateDanceGroupAnnounce);
    GameMsg_S2C_UpdateDanceGroupAnnounce();
    ~GameMsg_S2C_UpdateDanceGroupAnnounce() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    std::string                 m_strNewAnnounce;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_UpdateDanceGroupProfile: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_UpdateDanceGroupProfile);
    GameMsg_S2C_UpdateDanceGroupProfile();
    ~GameMsg_S2C_UpdateDanceGroupProfile() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    std::string                 m_strNewProfile;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_DanceGroupInfo: public GameMsg_Base
{
public:
    GameMsg_S2C_DanceGroupInfo();
    ~GameMsg_S2C_DanceGroupInfo();

public:
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int                m_nGroupID;
    bool                        m_bIsValid;
    unsigned short              m_nClickedPlace;
    CDanceGroupInfo             *m_pDGInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_GetDanceGroupBadgeInfosResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetDanceGroupBadgeInfosResult);
    GameMsg_S2C_GetDanceGroupBadgeInfosResult();
    ~GameMsg_S2C_GetDanceGroupBadgeInfosResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nErrorCode;
    std::vector<unsigned short> m_vectBadgeEffect;      // 特效
    std::vector<bool>           m_vectEffectState;      // 特效解锁状态
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_UnlockDanceGroupBadgeResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_UnlockDanceGroupBadgeResult);
    GameMsg_S2C_UnlockDanceGroupBadgeResult();
    ~GameMsg_S2C_UnlockDanceGroupBadgeResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nType;
    unsigned short              m_nUnlockID;
    unsigned short              m_nErrorCode;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_ChangeDanceGroupBadgeOrEffectResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_ChangeDanceGroupBadgeOrEffectResult);
    GameMsg_G2S_ChangeDanceGroupBadgeOrEffectResult();
    ~GameMsg_G2S_ChangeDanceGroupBadgeOrEffectResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nErrorCode;
    unsigned int                m_nRoleID;
    unsigned short              m_nBadge;
    unsigned short              m_nEffect;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_DanceGroupActivityGetStarInfoResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DanceGroupActivityGetStarInfoResult);
    GameMsg_S2C_DanceGroupActivityGetStarInfoResult();
    ~GameMsg_S2C_DanceGroupActivityGetStarInfoResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nErrorCode;
    unsigned short              m_nDayPacket;
    unsigned short              m_nCurrentStarLevel;    // 当前正在进行的星星
    unsigned int                m_nCurrentPowerValue;   // 剩余动力值
    std::map<unsigned short, CDGActivityStarBaseInfo> m_mStarProgress; // 星星基本信息
    std::map<unsigned short, std::list<CItem> >m_mPacketList;// 礼包ID列表
    std::list<CItem>            m_listReward;           // 首次登录舞团奖励（每天会更新）
    std::list<unsigned short>   m_listOpenPacket;       // 打开过的礼包列表
    unsigned short              m_nNormalCount;         // 点一下次数
    unsigned short              m_nMoneyCount;          // 推一把次数
    unsigned short              m_nPointCount;          // 推一把次数
    unsigned short              m_nMoneyNum;            // 推一把消耗的金券数
    unsigned short              m_nMCoinNum;            // 推一把消耗的Mcoin
    unsigned short              m_nTotalNormalCount;    // 总共可操作的次数
    unsigned short              m_nTotalMoneyCount;     // 总共可操作的次数
    unsigned short              m_nTotalMCoinCount;     // 总共可操作的次数
    std::string                 m_strRuleContent;       // 规则
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_DanceGroupActivityGetStarPowerInfoResult: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DanceGroupActivityGetStarPowerInfoResult);
    GameMsg_S2C_DanceGroupActivityGetStarPowerInfoResult();
    ~GameMsg_S2C_DanceGroupActivityGetStarPowerInfoResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nErrorCode;
    unsigned int                m_nStarLevel;
    unsigned int                m_nCrurrentPower;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_DanceGroupActivityAddStarPowerResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DanceGroupActivityAddStarPowerResult);
    GameMsg_S2C_DanceGroupActivityAddStarPowerResult();
    ~GameMsg_S2C_DanceGroupActivityAddStarPowerResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nErrorCode;
    unsigned short              m_nType;
    unsigned short              m_nStarLevel;           // 当前到第几颗星星
    unsigned short              m_nCurrentPowerValue;   // 当前动力值
    bool                        m_bReward;              // 是否获得首次登录奖励
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_DanceGroupActivityStarAddPowerMCoin2MoneyResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DanceGroupActivityStarAddPowerMCoin2MoneyResult);
    GameMsg_S2C_DanceGroupActivityStarAddPowerMCoin2MoneyResult();
    ~GameMsg_S2C_DanceGroupActivityStarAddPowerMCoin2MoneyResult() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nErrorCode;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_NotifyDanceGroupActivityStarState : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_NotifyDanceGroupActivityStarState);
    GameMsg_S2C_NotifyDanceGroupActivityStarState();
    ~GameMsg_S2C_NotifyDanceGroupActivityStarState() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nOldStarLevel;
    unsigned short              m_nStarLevel;
    unsigned int                m_nCurrentPower;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_DanceGroupActivityOpenStarPacketResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DanceGroupActivityOpenStarPacketResult);
    GameMsg_S2C_DanceGroupActivityOpenStarPacketResult();
    ~GameMsg_S2C_DanceGroupActivityOpenStarPacketResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nErrorCode;
    unsigned short              m_nStarLevel;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_DanceGroupActivityReset : public GameMsg_Base
{
public:
	GameMsg_S2C_DanceGroupActivityReset();
	~GameMsg_S2C_DanceGroupActivityReset() {}

public:
	bool doEncode(CParamPool &IOBuff);

};

//////////////////////////////////////////////////////////////////////////

class DanceGroupShopExchangeInfo
{
public:
    DanceGroupShopExchangeInfo();
    ~DanceGroupShopExchangeInfo();
    void Encode(CParamPool &IOBuff);


public:
    // 兑换编号
    unsigned int m_nIndex;
    // 兑换物品信息
    CItem m_ExchangeItem;
    // 兑换所需物品列表
    std::vector<CItem> m_ListRequestItem;
    // 兑换所需金钱类型
    unsigned char m_nRequestMoneyType;
    // 兑换所需金钱数量
    unsigned int m_nRequestMoneyCount;
    // 兑换所需贡献数量
    unsigned int m_nRequestContribution;
//     // 是否已拥有 客户端用
//     public bool m_nAlreadyHave = false;
};

class GameMsg_S2C_DanceGroupShopGetInfoResult: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DanceGroupShopGetInfoResult);
    GameMsg_S2C_DanceGroupShopGetInfoResult();
    ~GameMsg_S2C_DanceGroupShopGetInfoResult() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned char              m_nErrorCode;
    std::vector<DanceGroupShopExchangeInfo> m_vecExchange;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_DanceGroupShopExchangeResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_DanceGroupShopExchangeResult);
    GameMsg_S2C_DanceGroupShopExchangeResult();
    ~GameMsg_S2C_DanceGroupShopExchangeResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned char               m_nErrorCode;
    unsigned int                m_nIndex;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_GetDanceGroupRecrodsResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetDanceGroupRecrodsResult);
    GameMsg_S2C_GetDanceGroupRecrodsResult();
    ~GameMsg_S2C_GetDanceGroupRecrodsResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nErrorCode;
    std::map<unsigned int, CDanceGroupRecordsInfo> m_recordList;
};

//////////////////////////////////////////////////////////////////////////

//添加记录通知
class GameMsg_S2C_DanceGroupAddRecrodsNotify : public GameMsg_Base
{
public:
	GameMsg_S2C_DanceGroupAddRecrodsNotify();
	~GameMsg_S2C_DanceGroupAddRecrodsNotify() {}

public:
	bool doEncode(CParamPool &IOBuff);

public:
	CDanceGroupRecordsInfo m_recordInfo;
};

class GameMsg_S2C_DanceGroupChangeDayNotify : public GameMsg_Base
{
public:
	GameMsg_S2C_DanceGroupChangeDayNotify();
	~GameMsg_S2C_DanceGroupChangeDayNotify() {}

public:
	bool doEncode(CParamPool &IOBuff);

public:
	unsigned short m_nErrorCode;
	std::map<unsigned int, CDanceGroupRecordsInfo> m_recordList;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_InviteDanceGroupMemberResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_InviteDanceGroupMemberResult);
    GameMsg_S2C_InviteDanceGroupMemberResult();
    ~GameMsg_S2C_InviteDanceGroupMemberResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_BeInvitedAsDanceGroupMember : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_BeInvitedAsDanceGroupMember);
    GameMsg_S2C_BeInvitedAsDanceGroupMember();
    ~GameMsg_S2C_BeInvitedAsDanceGroupMember() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    std::string                 m_strInviterRoleName;
    unsigned int                m_nDanceGroupID;
    std::string                 m_strDanceGroupName;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_AcceptDanceGroupMemberInvitationResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_AcceptDanceGroupMemberInvitationResult);
    GameMsg_S2C_AcceptDanceGroupMemberInvitationResult();
    ~GameMsg_S2C_AcceptDanceGroupMemberInvitationResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short              m_nResult;
};

#endif
// end of file

