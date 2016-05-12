#ifndef	__GAMEMSG_C2S_DANCE_GROUP_H__
#define	__GAMEMSG_C2S_DANCE_GROUP_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

#define DANCE_GROUP_CLICKED_PLACE_NONE  0

class GameMsg_C2S_RequestMyDanceGroupInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestMyDanceGroupInfo);
    GameMsg_C2S_RequestMyDanceGroupInfo();
    ~GameMsg_C2S_RequestMyDanceGroupInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned short          m_nClickedPlace;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_CreateDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_CreateDanceGroup);
    GameMsg_C2S_CreateDanceGroup();
    ~GameMsg_C2S_CreateDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    std::string             m_strGroupName;     // 舞团名称
    unsigned short          m_nBadge;           // 舞团徽章
    std::string             m_strProfile;       // 舞团简介
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_RequestEnterDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestEnterDanceGroup);
    GameMsg_C2S_RequestEnterDanceGroup();
    ~GameMsg_C2S_RequestEnterDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int            m_nGroupID;         // 舞团ID
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_CancelRequestEnterDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_CancelRequestEnterDanceGroup);
    GameMsg_C2S_CancelRequestEnterDanceGroup();
    ~GameMsg_C2S_CancelRequestEnterDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned                int m_nGroupID;     // 舞团ID
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_ReplyRequestEnterDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ReplyRequestEnterDanceGroup);
    GameMsg_C2S_ReplyRequestEnterDanceGroup();
    ~GameMsg_C2S_ReplyRequestEnterDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nRequestRoleID;       // 申请者ID
    bool                m_bAllow;               // 是否同意加入
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_ReplyRequestListEnterDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ReplyRequestListEnterDanceGroup);
    GameMsg_C2S_ReplyRequestListEnterDanceGroup();
    ~GameMsg_C2S_ReplyRequestListEnterDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    std::list<unsigned int> m_listRoleID;       // 批量处理的申请者ID
    bool                m_bAllow;               // 是否同意加入
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_GetDanceGroupInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetDanceGroupInfo);
    GameMsg_C2S_GetDanceGroupInfo();
    ~GameMsg_C2S_GetDanceGroupInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int            m_nGroupID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_GetDanceGroupInfoList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetDanceGroupInfoList);
    GameMsg_C2S_GetDanceGroupInfoList();
    ~GameMsg_C2S_GetDanceGroupInfoList() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int            m_nBeginIndex;      // 开始条目
    unsigned short          m_nCount;           // 结束条目
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_ExitDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ExitDanceGroup);
    GameMsg_C2S_ExitDanceGroup();
    ~GameMsg_C2S_ExitDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_ChangeDanceGroupLeader : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeDanceGroupLeader);
    GameMsg_C2S_ChangeDanceGroupLeader();
    ~GameMsg_C2S_ChangeDanceGroupLeader() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int            m_nNewLeaderID;     // 新团长ID
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_DismissDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_DismissDanceGroup);
    GameMsg_C2S_DismissDanceGroup();
    ~GameMsg_C2S_DismissDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_CancelDismissDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_CancelDismissDanceGroup);
    GameMsg_C2S_CancelDismissDanceGroup();
    ~GameMsg_C2S_CancelDismissDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_ChangeDanceGroupTitle : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeDanceGroupTitle);
    GameMsg_C2S_ChangeDanceGroupTitle();
    ~GameMsg_C2S_ChangeDanceGroupTitle() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int            m_nTargetRoleID;    // 目标角色ID
    unsigned int            m_nNewTitle;        // 新职位
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_KickOutDanceGroupMember : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_KickOutDanceGroupMember);
    GameMsg_C2S_KickOutDanceGroupMember();
    ~GameMsg_C2S_KickOutDanceGroupMember() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int            m_nTargetRoleID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_ChangeDanceGroupColor : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeDanceGroupColor);
    GameMsg_C2S_ChangeDanceGroupColor();
    ~GameMsg_C2S_ChangeDanceGroupColor() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned short          m_nColor;           // 舞团颜色
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_ChangeDanceGroupBadge : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeDanceGroupBadge);
    GameMsg_C2S_ChangeDanceGroupBadge();
    ~GameMsg_C2S_ChangeDanceGroupBadge() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned short          m_nBadge;           // 舞团徽章
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_ChangeDanceGroupTitleName : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeDanceGroupTitleName);
    GameMsg_C2S_ChangeDanceGroupTitleName();
    ~GameMsg_C2S_ChangeDanceGroupTitleName() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned short          m_nTitle;           // 职位
    std::string             m_strTitleName;     // 职位名称
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_ChangeDanceGroupProfile : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeDanceGroupProfile);
    GameMsg_C2S_ChangeDanceGroupProfile();
    ~GameMsg_C2S_ChangeDanceGroupProfile() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    std::string             m_strProfile;       // 舞团简介
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_ChangeDanceGroupAnnouncement : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeDanceGroupAnnouncement);
    GameMsg_C2S_ChangeDanceGroupAnnouncement();
    ~GameMsg_C2S_ChangeDanceGroupAnnouncement() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    std::string             m_strAnnouncement;  // 舞团公告
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_DonateDanceGroupResource : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_DonateDanceGroupResource);
    GameMsg_C2S_DonateDanceGroupResource();
    ~GameMsg_C2S_DonateDanceGroupResource() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int            m_nResA;
    unsigned int            m_nResB;
    unsigned int            m_nResC;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_UpgradeDanceGroup : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_UpgradeDanceGroup);
    GameMsg_C2S_UpgradeDanceGroup();
    ~GameMsg_C2S_UpgradeDanceGroup() {}

public:
    bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_ChangeDanceGroupName : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeDanceGroupName);
    GameMsg_C2S_ChangeDanceGroupName();
    ~GameMsg_C2S_ChangeDanceGroupName() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    std::string             m_strNewGroupName;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_GetDanceGroupBadgeInfos : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetDanceGroupBadgeInfos);
    GameMsg_C2S_GetDanceGroupBadgeInfos();
    ~GameMsg_C2S_GetDanceGroupBadgeInfos() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned short          m_unType;           // 选择的类型(badge, effect)
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_UnlockDanceGroupBadge : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_UnlockDanceGroupBadge);
    GameMsg_C2S_UnlockDanceGroupBadge();
    ~GameMsg_C2S_UnlockDanceGroupBadge() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:

    unsigned short          m_nType;            // 选择的类型(badge, effect)
    unsigned short          m_nUnlockID;        // 解锁ID(badge, effect)
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_ChangeDanceGroupBadgeOrEffect : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeDanceGroupBadgeOrEffect);
    GameMsg_C2S_ChangeDanceGroupBadgeOrEffect();
    ~GameMsg_C2S_ChangeDanceGroupBadgeOrEffect() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned short          m_nBadgeID;         // 选择的徽章
    unsigned short          m_nEffectID;        // 选择的特效 
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_DanceGroupActivityGetStarInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_DanceGroupActivityGetStarInfo);
    GameMsg_C2S_DanceGroupActivityGetStarInfo();
    ~GameMsg_C2S_DanceGroupActivityGetStarInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

//主动获取动力值信息（刷新动力值）
class GameMsg_C2S_DanceGroupActivityGetStarPowerInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_DanceGroupActivityGetStarPowerInfo);
    GameMsg_C2S_DanceGroupActivityGetStarPowerInfo();
    ~GameMsg_C2S_DanceGroupActivityGetStarPowerInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_DanceGroupActivityAddStarPower : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_DanceGroupActivityAddStarPower);
    GameMsg_C2S_DanceGroupActivityAddStarPower();
    ~GameMsg_C2S_DanceGroupActivityAddStarPower() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned short          m_nType;            // free, money, mcoin
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_DanceGroupActivityAddStarPowerPoint2Money : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_DanceGroupActivityAddStarPowerPoint2Money);
    GameMsg_C2S_DanceGroupActivityAddStarPowerPoint2Money();
    ~GameMsg_C2S_DanceGroupActivityAddStarPowerPoint2Money() {}

public:
    bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_DanceGroupActivityOpenStarPacket : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_DanceGroupActivityOpenStarPacket);
    GameMsg_C2S_DanceGroupActivityOpenStarPacket();
    ~GameMsg_C2S_DanceGroupActivityOpenStarPacket() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned short          m_nStarLevel;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_DanceGroupShopGetInfo : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_DanceGroupShopGetInfo);
    GameMsg_C2S_DanceGroupShopGetInfo();
    ~GameMsg_C2S_DanceGroupShopGetInfo() {}

public:
    bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_DanceGroupShopExchange : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_DanceGroupShopExchange);
    GameMsg_C2S_DanceGroupShopExchange();
    ~GameMsg_C2S_DanceGroupShopExchange() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int          m_nIndex;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_GetDanceGroupRecords: public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetDanceGroupRecords);
    GameMsg_C2S_GetDanceGroupRecords();
    ~GameMsg_C2S_GetDanceGroupRecords() {}

public:
    bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_InviteDanceGroupMember : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_InviteDanceGroupMember);
    GameMsg_C2S_InviteDanceGroupMember();
    ~GameMsg_C2S_InviteDanceGroupMember() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nInviteeRoleID;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_AcceptDanceGroupMemberInvitation : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_AcceptDanceGroupMemberInvitation);
    GameMsg_C2S_AcceptDanceGroupMemberInvitation();
    ~GameMsg_C2S_AcceptDanceGroupMemberInvitation() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned int            m_nDanceGroupID;
    std::string             m_strInviterRoleName;
};

#endif
// end of file


