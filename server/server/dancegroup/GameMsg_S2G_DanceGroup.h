#ifndef	__GAMEMSG_S2G_DANCE_GROUP_H__
#define	__GAMEMSG_S2G_DANCE_GROUP_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DanceGroup.h"

class CParamPool;

class GameMsg_S2G_CreateDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CreateDanceGroup);
    GameMsg_S2G_CreateDanceGroup();
    ~GameMsg_S2G_CreateDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    std::string         m_strGroupName;         // 舞团名称
    unsigned short      m_nBadge;               // 舞团徽章
    std::string         m_strProfile;           // 舞团简介
    unsigned int        m_nLastOffLineTime;     // 最近的登出时间
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_RequestEnterDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_RequestEnterDanceGroup);
    GameMsg_S2G_RequestEnterDanceGroup();
    ~GameMsg_S2G_RequestEnterDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nMyGroupID;
    unsigned int        m_nRequestGroupID;
};

class GameMsg_S2G_RequestEnterDanceGroupByName : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_RequestEnterDanceGroupByName);
    GameMsg_S2G_RequestEnterDanceGroupByName();
    ~GameMsg_S2G_RequestEnterDanceGroupByName() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nMyGroupID;
    std::string         m_strRequestDanceGroupName;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ReplyRequestEnterDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ReplyRequestEnterDanceGroup);
    GameMsg_S2G_ReplyRequestEnterDanceGroup();
    ~GameMsg_S2G_ReplyRequestEnterDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;             // 舞团编号
    unsigned int        m_nRequestRoleID;       // 申请者RoleID
    bool                m_bAllow;               // 是否批准加入
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ReplyRequestListEnterDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ReplyRequestListEnterDanceGroup);
    GameMsg_S2G_ReplyRequestListEnterDanceGroup();
    ~GameMsg_S2G_ReplyRequestListEnterDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    bool                m_bAllow;               // 是否批准加入
    unsigned int        m_nGroupID;             // 舞团编号
    std::list<unsigned int> m_listRoleID;       // 申请者列表
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ExitDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ExitDanceGroup);
    GameMsg_S2G_ExitDanceGroup();
    ~GameMsg_S2G_ExitDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ChangeDanceGroupLeader : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ChangeDanceGroupLeader);
    GameMsg_S2G_ChangeDanceGroupLeader();
    ~GameMsg_S2G_ChangeDanceGroupLeader() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;             // 舞团ID
    unsigned int        m_nNewLeaderID;         // 新团长ID
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_DismissDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_DismissDanceGroup);
    GameMsg_S2G_DismissDanceGroup();
    ~GameMsg_S2G_DismissDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;             // 舞团ID
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_CancelDismissDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_CancelDismissDanceGroup);
    GameMsg_S2G_CancelDismissDanceGroup();
    ~GameMsg_S2G_CancelDismissDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;             // 舞团ID
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ChangeDanceGroupTitle : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ChangeDanceGroupTitle);
    GameMsg_S2G_ChangeDanceGroupTitle();
    ~GameMsg_S2G_ChangeDanceGroupTitle() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;             // 舞团ID
    unsigned int        m_nTargetRoleID;        // 目标角色ID
    unsigned char       m_nNewTitle;            // 新的职位
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_KickOutDanceGroupMember : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_KickOutDanceGroupMember);
    GameMsg_S2G_KickOutDanceGroupMember();
    ~GameMsg_S2G_KickOutDanceGroupMember() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;             // 舞团ID
    unsigned int        m_nTargetRoleID;        // 目标角色ID
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ChangeDanceGroupColor : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ChangeDanceGroupColor);
    GameMsg_S2G_ChangeDanceGroupColor();
    ~GameMsg_S2G_ChangeDanceGroupColor() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;             // 舞团ID
    unsigned short      m_nColor;               // 舞团颜色
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ChangeDanceGroupBadge : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ChangeDanceGroupBadge);
    GameMsg_S2G_ChangeDanceGroupBadge();
    ~GameMsg_S2G_ChangeDanceGroupBadge() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;             // 舞团ID
    unsigned short      m_nBadge;               // 舞团徽章
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ChangeDanceGroupTitleName : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ChangeDanceGroupTitleName);
    GameMsg_S2G_ChangeDanceGroupTitleName();
    ~GameMsg_S2G_ChangeDanceGroupTitleName() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;             // 舞团ID
    unsigned char       m_nTitle;               // 职位
    std::string         m_strTitleName;         // 职位名称
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ChangeDanceGroupProfile : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ChangeDanceGroupProfile);
    GameMsg_S2G_ChangeDanceGroupProfile();
    ~GameMsg_S2G_ChangeDanceGroupProfile() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;             // 舞团ID
    std::string         m_strProfile;           // 舞团简介
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ChangeDanceGroupAnnouncement : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ChangeDanceGroupAnnouncement);
    GameMsg_S2G_ChangeDanceGroupAnnouncement();
    ~GameMsg_S2G_ChangeDanceGroupAnnouncement() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;             // 舞团ID
    std::string         m_strAnnouncement;      // 舞团公告
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_UpdateDanceGroupMemberInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateDanceGroupMemberInfo);
    GameMsg_S2G_UpdateDanceGroupMemberInfo();
    ~GameMsg_S2G_UpdateDanceGroupMemberInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;             // 舞团ID
    unsigned char       m_nState;               // 当前状态
    unsigned int        m_nLastOffLineTime;     // 最近的登出时间
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_DonateDanceGroupResource : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_DonateDanceGroupResource);
    GameMsg_S2G_DonateDanceGroupResource();
    ~GameMsg_S2G_DonateDanceGroupResource() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
    unsigned int        m_nResA;
    unsigned int        m_nResB;
    unsigned int        m_nResC;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_UpgradeDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpgradeDanceGroup);
    GameMsg_S2G_UpgradeDanceGroup();
    ~GameMsg_S2G_UpgradeDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ChangeDanceGroupName : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ChangeDanceGroupName);
    GameMsg_S2G_ChangeDanceGroupName();
    ~GameMsg_S2G_ChangeDanceGroupName() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
    std::string         m_strNewGroupName;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ChangeDanceGroupHonor : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ChangeDanceGroupHonor);
    GameMsg_S2G_ChangeDanceGroupHonor();
    ~GameMsg_S2G_ChangeDanceGroupHonor() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
    int                 m_nCause;
    int                 m_nPara;
    int                 m_nChangeHonor;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_UnlockDanceGroupBadge : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_UnlockDanceGroupBadge);
    GameMsg_S2G_UnlockDanceGroupBadge();
    ~GameMsg_S2G_UnlockDanceGroupBadge() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGoupID;
    unsigned short      m_nType;
    unsigned short      m_nUnlockID;
    unsigned int        m_nResA;
    unsigned int        m_nResB;
    unsigned int        m_nResC;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ChangeDanceGroupBadgeOrEffect : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ChangeDanceGroupBadgeOrEffect);
    GameMsg_S2G_ChangeDanceGroupBadgeOrEffect();
    ~GameMsg_S2G_ChangeDanceGroupBadgeOrEffect() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
    unsigned short      m_nBadgeID;
    unsigned short      m_nEffectID;
    unsigned int        m_nResA;
    unsigned int        m_nResB;
    unsigned int        m_nResC;
};

//////////////////////////////////////////////////////////////////////////

//舞团团徽每天消耗操作
class GameMsg_S2G_DanceGroupTuanhuiPerDayConsume : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_DanceGroupTuanhuiPerDayConsume);
	GameMsg_S2G_DanceGroupTuanhuiPerDayConsume();
	~GameMsg_S2G_DanceGroupTuanhuiPerDayConsume() {}

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned int m_nGroupID;
	unsigned int m_nResA;
	unsigned int m_nResB;
	unsigned int m_nResC;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_DanceGroupActivityAddStarPower : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_DanceGroupActivityAddStarPower);
    GameMsg_S2G_DanceGroupActivityAddStarPower();
    ~GameMsg_S2G_DanceGroupActivityAddStarPower() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
    unsigned short      m_nType;                // 操作类型
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_DanceGroupActivityOpenStarPacket : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_DanceGroupActivityOpenStarPacket);
    GameMsg_S2G_DanceGroupActivityOpenStarPacket();
    ~GameMsg_S2G_DanceGroupActivityOpenStarPacket() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
    unsigned short      m_nStarLevel;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_UpdateDanceGroupActivityReward : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_UpdateDanceGroupActivityReward);
	GameMsg_S2G_UpdateDanceGroupActivityReward();
	~GameMsg_S2G_UpdateDanceGroupActivityReward() {}

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned int m_nGroupID;
	unsigned int m_nRoleID;
	bool m_bReward;

};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_DanceGroupShopExchange : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_DanceGroupShopExchange);
    GameMsg_S2G_DanceGroupShopExchange();
    ~GameMsg_S2G_DanceGroupShopExchange() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
    unsigned short      m_nIndex;
    unsigned short      m_nNeedContribution;
    unsigned short      m_nTotalCount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_DanceGroupShopResetExchangeCount : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_DanceGroupShopResetExchangeCount);
	GameMsg_S2G_DanceGroupShopResetExchangeCount();
	~GameMsg_S2G_DanceGroupShopResetExchangeCount() {}

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned int m_nGroupID;

};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_GetDanceGroupRecrods : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetDanceGroupRecrods);
    GameMsg_S2G_GetDanceGroupRecrods();
    ~GameMsg_S2G_GetDanceGroupRecrods() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_DanceGroupAddRecord : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_DanceGroupAddRecord);
	GameMsg_S2G_DanceGroupAddRecord();
	~GameMsg_S2G_DanceGroupAddRecord() {}

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	CDanceGroupRecordsInfo m_recordInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_AddDanceGroupContribution : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_AddDanceGroupContribution);
    GameMsg_S2G_AddDanceGroupContribution();
    ~GameMsg_S2G_AddDanceGroupContribution() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
    unsigned int        m_nChangeValue;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_S2G_ReduceDanceGroupContribution : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ReduceDanceGroupContribution);
    GameMsg_S2G_ReduceDanceGroupContribution();
    ~GameMsg_S2G_ReduceDanceGroupContribution() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
    unsigned int        m_nChangeValue;
};


//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_GetDanceGroupInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetDanceGroupInfo);
    GameMsg_S2G_GetDanceGroupInfo();
    ~GameMsg_S2G_GetDanceGroupInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_GetMyDanceGroupInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetMyDanceGroupInfo);
    GameMsg_S2G_GetMyDanceGroupInfo();
    ~GameMsg_S2G_GetMyDanceGroupInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
    unsigned short      m_nClickedPlace;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_DanceGroupActivityGetStarInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_DanceGroupActivityGetStarInfo);
    GameMsg_S2G_DanceGroupActivityGetStarInfo();
    ~GameMsg_S2G_DanceGroupActivityGetStarInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_DanceGroupActivityGetStarPowerInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_DanceGroupActivityGetStarPowerInfo);
    GameMsg_S2G_DanceGroupActivityGetStarPowerInfo();
    ~GameMsg_S2G_DanceGroupActivityGetStarPowerInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_DanceGroupShopGetInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_DanceGroupShopGetInfo);
    GameMsg_S2G_DanceGroupShopGetInfo();
    ~GameMsg_S2G_DanceGroupShopGetInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nGroupID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_AcceptDanceGroupMemberInvitation : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_AcceptDanceGroupMemberInvitation);
    GameMsg_S2G_AcceptDanceGroupMemberInvitation();
    ~GameMsg_S2G_AcceptDanceGroupMemberInvitation() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nDanceGroupID;
    std::string         m_strInviterRoleName;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_AddDanceGroupChallengeActive : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_AddDanceGroupChallengeActive)

public:
    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned int m_nGroupID;
    int m_nAddValue;
};


//////////////////////////////////////////////////////////////////////////
// 检测是否可以邀请某人加入
class GameMsg_S2G_CanBeInvitedCheck : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_CanBeInvitedCheck);

    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned int m_nToGroupID;
    unsigned int m_nInvitedRole;
};

class GameMsg_G2S_CanBeInvitedCheck : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_G2S_CanBeInvitedCheck);

    bool doEncode( CParamPool& IOBuff );
    bool doDecode( CParamPool& IOBuff );

public:
    unsigned char m_nRet;
    unsigned int m_nInvitedRole;
};


#endif
// end of file


