#include "GameMsg_C2S_DanceGroup.h"
#include "DanceGroupMsgDef.h"
#include "../../datastructure/DanceGroupInfoDef.h"
#include "../../socket/ParamPool.h"
#include "../../datastructure/datainfor/ServerConfig.h"


GameMsg_C2S_RequestMyDanceGroupInfo::GameMsg_C2S_RequestMyDanceGroupInfo()
    : GameMsg_Base(MSG_C2S_RequestMyDanceGroupInfo)
    , m_nClickedPlace(DANCE_GROUP_CLICKED_PLACE_NONE)
{
}

bool GameMsg_C2S_RequestMyDanceGroupInfo::doDecode(CParamPool &IOBuff)
{
    m_nClickedPlace = IOBuff.GetUShort();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_CreateDanceGroup

GameMsg_C2S_CreateDanceGroup::GameMsg_C2S_CreateDanceGroup()
    : GameMsg_Base(MSG_C2S_CreateDanceGroup)
    , m_strGroupName("")
    , m_nBadge(0)
    , m_strProfile("")
{
}

bool GameMsg_C2S_CreateDanceGroup::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_strGroupName);
    m_nBadge = IOBuff.GetUShort();
    IOBuff.GetStringW(m_strProfile);

    if (m_strGroupName.size() > (size_t)g_ServerConfig.m_DanceGroupNameLength)
        m_strGroupName.resize(g_ServerConfig.m_DanceGroupNameLength);

    if (m_strProfile.size() > (size_t)g_ServerConfig.m_DanceGroupSummary)
        m_strProfile.resize(g_ServerConfig.m_DanceGroupSummary);

    return true;
}

// ----------------------------------------------- GameMsg_C2S_RequestEnterDanceGroup

GameMsg_C2S_RequestEnterDanceGroup::GameMsg_C2S_RequestEnterDanceGroup()
    : GameMsg_Base(MSG_C2S_RequestEnterDanceGroup)
    , m_nGroupID(0)
{
}

bool GameMsg_C2S_RequestEnterDanceGroup::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_C2S_RequestEnterDanceGroup::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);

    return true;
}

// ----------------------------------------------- GameMsg_C2S_CancelRequestEnterDanceGroup

GameMsg_C2S_CancelRequestEnterDanceGroup::GameMsg_C2S_CancelRequestEnterDanceGroup()
    : GameMsg_Base(MSG_C2S_CancelRequestEnterDanceGroup)
    , m_nGroupID(0)
{
}

bool GameMsg_C2S_CancelRequestEnterDanceGroup::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_C2S_CancelRequestEnterDanceGroup::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);

    return true;
}

// ----------------------------------------------- GameMsg_C2S_ReplyRequestEnterDanceGroup

GameMsg_C2S_ReplyRequestEnterDanceGroup::GameMsg_C2S_ReplyRequestEnterDanceGroup()
    : GameMsg_Base(MSG_C2S_ReplyRequestEnterDanceGroup)
    , m_nRequestRoleID(0)
    , m_bAllow(false)
{
}

bool GameMsg_C2S_ReplyRequestEnterDanceGroup::doDecode(CParamPool &IOBuff)
{
    m_nRequestRoleID = IOBuff.GetUInt();
    m_bAllow = IOBuff.GetBool();

    return true;
}

bool GameMsg_C2S_ReplyRequestEnterDanceGroup::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRequestRoleID);
    IOBuff.AddBool(m_bAllow);

    return true;
}

// ----------------------------------------------- GameMsg_C2S_ReplyRequestListEnterDanceGroup

GameMsg_C2S_ReplyRequestListEnterDanceGroup::GameMsg_C2S_ReplyRequestListEnterDanceGroup()
    : GameMsg_Base(MSG_C2S_ReplyRequestListEnterDanceGroup)
    , m_bAllow(false)
{
}

bool GameMsg_C2S_ReplyRequestListEnterDanceGroup::doDecode(CParamPool &IOBuff)
{
    unsigned short nLen = IOBuff.GetUShort();
    unsigned int nRequestRoleID = 0;

    for (unsigned short i = 0; i < nLen; ++i)
    {
        nRequestRoleID = IOBuff.GetUInt();

        m_listRoleID.push_back(nRequestRoleID);
    }

    m_bAllow = IOBuff.GetBool();

    return true;
}

bool GameMsg_C2S_ReplyRequestListEnterDanceGroup::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listRoleID.size());

    for (std::list<unsigned int>::const_iterator bIt = m_listRoleID.begin(), eIt = m_listRoleID.end(); 
         bIt != eIt; ++bIt)
    {
        IOBuff.AddUInt(*bIt);
    }

    IOBuff.AddBool(m_bAllow);

    return true;
}

// ----------------------------------------------- GameMsg_C2S_GetDanceGroupInfo

GameMsg_C2S_GetDanceGroupInfo::GameMsg_C2S_GetDanceGroupInfo()
    : GameMsg_Base(MSG_C2S_GetDanceGroupInfo)
    , m_nGroupID(0)
{
}

bool GameMsg_C2S_GetDanceGroupInfo::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_GetDanceGroupInfoList

GameMsg_C2S_GetDanceGroupInfoList::GameMsg_C2S_GetDanceGroupInfoList()
    : GameMsg_Base(MSG_C2S_GetDanceGroupInfoList)
    , m_nBeginIndex(0)
    , m_nCount(0)
{
}

bool GameMsg_C2S_GetDanceGroupInfoList::doDecode(CParamPool &IOBuff)
{
    m_nBeginIndex = IOBuff.GetUInt();
    m_nCount = IOBuff.GetUShort();

    return true;
}

bool GameMsg_C2S_GetDanceGroupInfoList::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nBeginIndex);
    IOBuff.AddUShort(m_nCount);

    return true;
}

// ----------------------------------------------- GameMsg_C2S_ExitDanceGroup

GameMsg_C2S_ExitDanceGroup::GameMsg_C2S_ExitDanceGroup()
    : GameMsg_Base(MSG_C2S_ExitDanceGroup)
{
}

bool GameMsg_C2S_ExitDanceGroup::doDecode(CParamPool &IOBuff)
{
    return true;
}

// ----------------------------------------------- GameMsg_C2S_ChangeDanceGroupLeader

GameMsg_C2S_ChangeDanceGroupLeader::GameMsg_C2S_ChangeDanceGroupLeader()
    : GameMsg_Base(MSG_C2S_ChangeDanceGroupLeader)
    , m_nNewLeaderID(0)
{
}

bool GameMsg_C2S_ChangeDanceGroupLeader::doDecode(CParamPool &IOBuff)
{
    m_nNewLeaderID = IOBuff.GetUInt();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_DismissDanceGroup

GameMsg_C2S_DismissDanceGroup::GameMsg_C2S_DismissDanceGroup()
    : GameMsg_Base(MSG_C2S_DismissDanceGroup)
{
}

bool GameMsg_C2S_DismissDanceGroup::doDecode(CParamPool &IOBuff)
{
    return true;
}

bool GameMsg_C2S_DismissDanceGroup::doEncode(CParamPool &IOBuff)
{
    return true;
}

// ----------------------------------------------- GameMsg_C2S_CancelDismissDanceGroup

GameMsg_C2S_CancelDismissDanceGroup::GameMsg_C2S_CancelDismissDanceGroup()
    : GameMsg_Base(MSG_C2S_CancelDismissDanceGroup)
{
}

bool GameMsg_C2S_CancelDismissDanceGroup::doDecode(CParamPool &IOBuff)
{
    return true;
}

// ----------------------------------------------- GameMsg_C2S_ChangeDanceGroupTitle

GameMsg_C2S_ChangeDanceGroupTitle::GameMsg_C2S_ChangeDanceGroupTitle()
    : GameMsg_Base(MSG_C2S_ChangeDanceGroupTitle)
    , m_nTargetRoleID(0)
    , m_nNewTitle(0)
{
}

bool GameMsg_C2S_ChangeDanceGroupTitle::doDecode(CParamPool &IOBuff)
{
    m_nTargetRoleID = IOBuff.GetUInt();
    m_nNewTitle = IOBuff.GetUInt();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_KickOutDanceGroupMember

GameMsg_C2S_KickOutDanceGroupMember::GameMsg_C2S_KickOutDanceGroupMember()
    : GameMsg_Base(MSG_C2S_KickOutDanceGroupMember)
    , m_nTargetRoleID(0)
{
}

bool GameMsg_C2S_KickOutDanceGroupMember::doDecode(CParamPool &IOBuff)
{
    m_nTargetRoleID = IOBuff.GetUInt();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_ChangeDanceGroupColor

GameMsg_C2S_ChangeDanceGroupColor::GameMsg_C2S_ChangeDanceGroupColor()
    : GameMsg_Base(MSG_C2S_ChangeDanceGroupColor)
    , m_nColor(0)
{
}

bool GameMsg_C2S_ChangeDanceGroupColor::doDecode(CParamPool &IOBuff)
{
    m_nColor = IOBuff.GetUShort();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_ChangeDanceGroupBadge

GameMsg_C2S_ChangeDanceGroupBadge::GameMsg_C2S_ChangeDanceGroupBadge()
    : GameMsg_Base(MSG_C2S_ChangeDanceGroupBadge)
    , m_nBadge(0)
{
}

bool GameMsg_C2S_ChangeDanceGroupBadge::doDecode(CParamPool &IOBuff)
{
    m_nBadge = IOBuff.GetUShort();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_ChangeDanceGroupTitleName

GameMsg_C2S_ChangeDanceGroupTitleName::GameMsg_C2S_ChangeDanceGroupTitleName()
    : GameMsg_Base(MSG_C2S_ChangeDanceGroupTitleName)
    , m_nTitle(0)
    , m_strTitleName("")
{
}

bool GameMsg_C2S_ChangeDanceGroupTitleName::doDecode(CParamPool &IOBuff)
{
    m_nTitle = IOBuff.GetUShort();
    IOBuff.GetStringW(m_strTitleName);

    if (m_strTitleName.size() > (size_t)g_ServerConfig.m_DanceGroupTitleName)
        m_strTitleName.resize(g_ServerConfig.m_DanceGroupTitleName);

    return true;
}

// ----------------------------------------------- GameMsg_C2S_ChangeDanceGroupProfile

GameMsg_C2S_ChangeDanceGroupProfile::GameMsg_C2S_ChangeDanceGroupProfile()
    : GameMsg_Base(MSG_C2S_ChangeDanceGroupProfile)
    , m_strProfile("")
{
}

bool GameMsg_C2S_ChangeDanceGroupProfile::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_strProfile);

    if (m_strProfile.size() > cMAX_DANCE_GROUP_PROFILE_LEN)
        m_strProfile.resize(cMAX_DANCE_GROUP_PROFILE_LEN);

    return true;
}

// ----------------------------------------------- GameMsg_C2S_ChangeDanceGroupAnnouncement

GameMsg_C2S_ChangeDanceGroupAnnouncement::GameMsg_C2S_ChangeDanceGroupAnnouncement()
    : GameMsg_Base(MSG_C2S_ChangeDanceGroupAnnouncement)
    , m_strAnnouncement("")
{
}

bool GameMsg_C2S_ChangeDanceGroupAnnouncement::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_strAnnouncement);

    if (m_strAnnouncement.size() > (size_t)g_ServerConfig.m_DanceGroupAnnouncementLength)
        m_strAnnouncement.resize(g_ServerConfig.m_DanceGroupAnnouncementLength);

    return true;
}

// ----------------------------------------------- GameMsg_C2S_DonateDanceGroupResource

GameMsg_C2S_DonateDanceGroupResource::GameMsg_C2S_DonateDanceGroupResource()
    : GameMsg_Base(MSG_C2S_DonateDanceGroupResource)
    , m_nResA(0)
    , m_nResB(0)
    , m_nResC(0)
{
}

bool GameMsg_C2S_DonateDanceGroupResource::doDecode(CParamPool &IOBuff)
{
    m_nResA = IOBuff.GetUInt();
    m_nResB = IOBuff.GetUInt();
    m_nResC = IOBuff.GetUInt();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_UpgradeDanceGroup

GameMsg_C2S_UpgradeDanceGroup::GameMsg_C2S_UpgradeDanceGroup()
    : GameMsg_Base(MSG_C2S_UpgradeDanceGroup)
{
}

bool GameMsg_C2S_UpgradeDanceGroup::doDecode(CParamPool &IOBuff)
{
    return true;
}

// ----------------------------------------------- GameMsg_C2S_ChangeDanceGroupName

GameMsg_C2S_ChangeDanceGroupName::GameMsg_C2S_ChangeDanceGroupName()
    : GameMsg_Base(MSG_C2S_ChangeDanceGroupName)
    , m_strNewGroupName("")
{
}

bool GameMsg_C2S_ChangeDanceGroupName::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_strNewGroupName);

    if (m_strNewGroupName.size() > cMAX_DANCE_GROUP_NAME_LEN)
        m_strNewGroupName.resize(cMAX_DANCE_GROUP_NAME_LEN);

    return true;
}

// ----------------------------------------------- GameMsg_C2S_GetDanceGroupBadgeInfos

GameMsg_C2S_GetDanceGroupBadgeInfos::GameMsg_C2S_GetDanceGroupBadgeInfos()
    : GameMsg_Base(MSG_C2S_GetDanceGroupBadgeInfos)
    , m_unType(0)
{

}

bool GameMsg_C2S_GetDanceGroupBadgeInfos::doDecode(CParamPool &IOBuff)
{
    m_unType = IOBuff.GetUShort();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_UnlockDanceGroupBadge

GameMsg_C2S_UnlockDanceGroupBadge::GameMsg_C2S_UnlockDanceGroupBadge()
    : GameMsg_Base(MSG_C2S_UnlockDanceGroupBadge)
    , m_nType(0)
    , m_nUnlockID(0)
{
}

bool GameMsg_C2S_UnlockDanceGroupBadge::doDecode(CParamPool &IOBuff)
{
    m_nType = IOBuff.GetUShort();
    m_nUnlockID = IOBuff.GetUShort();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_ChangeDanceGroupBadgeOrEffect

GameMsg_C2S_ChangeDanceGroupBadgeOrEffect::GameMsg_C2S_ChangeDanceGroupBadgeOrEffect()
    : GameMsg_Base(MSG_C2S_ChangeDanceGroupBadgeOrEffect)
    , m_nBadgeID(0)
    , m_nEffectID(0)
{
}

bool GameMsg_C2S_ChangeDanceGroupBadgeOrEffect::doDecode(CParamPool &IOBuff)
{
    m_nBadgeID = IOBuff.GetUShort();
    m_nEffectID = IOBuff.GetUShort();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_DanceGroupActivityGetStarInfo

GameMsg_C2S_DanceGroupActivityGetStarInfo::GameMsg_C2S_DanceGroupActivityGetStarInfo()
    : GameMsg_Base(MSG_C2S_DanceGroupActivityGetStarInfo)
{
}

bool GameMsg_C2S_DanceGroupActivityGetStarInfo::doDecode(CParamPool &IOBuff)
{
    return true;
}

// ----------------------------------------------- GameMsg_C2S_DanceGroupActivityGetStarPowerInfo

GameMsg_C2S_DanceGroupActivityGetStarPowerInfo::GameMsg_C2S_DanceGroupActivityGetStarPowerInfo()
    : GameMsg_Base(MSG_C2S_DanceGroupActivityGetStarPowerInfo)
{
}

bool GameMsg_C2S_DanceGroupActivityGetStarPowerInfo::doDecode(CParamPool &IOBuff)
{
    return true;
}

// ----------------------------------------------- GameMsg_C2S_DanceGroupActivityAddStarPower

GameMsg_C2S_DanceGroupActivityAddStarPower::GameMsg_C2S_DanceGroupActivityAddStarPower()
    : GameMsg_Base(MSG_C2S_DanceGroupActivityAddStarPower)
    , m_nType(0)
{
}

bool GameMsg_C2S_DanceGroupActivityAddStarPower::doDecode(CParamPool &IOBuff)
{
    m_nType = IOBuff.GetUShort();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_DanceGroupActivityAddStarPowerPoint2Money

GameMsg_C2S_DanceGroupActivityAddStarPowerPoint2Money::GameMsg_C2S_DanceGroupActivityAddStarPowerPoint2Money()
    : GameMsg_Base(MSG_C2S_DanceGroupActivityAddStarPowerPoint2Money)
{
}

bool GameMsg_C2S_DanceGroupActivityAddStarPowerPoint2Money::doDecode(CParamPool &IOBuff)
{
    return true;
}

// ----------------------------------------------- GameMsg_C2S_DanceGroupActivityOpenStarPacket

GameMsg_C2S_DanceGroupActivityOpenStarPacket::GameMsg_C2S_DanceGroupActivityOpenStarPacket()
    : GameMsg_Base(MSG_C2S_DanceGroupActivityOpenStarPacket)
    , m_nStarLevel(0)
{
}

bool GameMsg_C2S_DanceGroupActivityOpenStarPacket::doDecode(CParamPool &IOBuff)
{
    m_nStarLevel = IOBuff.GetUShort();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_DanceGroupShopGetInfo

GameMsg_C2S_DanceGroupShopGetInfo::GameMsg_C2S_DanceGroupShopGetInfo()
    : GameMsg_Base(MSG_C2S_DanceGroupShopGetInfo)
{
}

bool GameMsg_C2S_DanceGroupShopGetInfo::doDecode(CParamPool &IOBuff)
{
    return true;
}

// ----------------------------------------------- GameMsg_C2S_DanceGroupShopExchange

GameMsg_C2S_DanceGroupShopExchange::GameMsg_C2S_DanceGroupShopExchange()
    : GameMsg_Base(MSG_C2S_DanceGroupShopExchange)
    , m_nIndex(0)
{
}

bool GameMsg_C2S_DanceGroupShopExchange::doDecode(CParamPool &IOBuff)
{
    m_nIndex = IOBuff.GetUInt();

    return true;
}

// ----------------------------------------------- GameMsg_C2S_GetDanceGroupRecords

GameMsg_C2S_GetDanceGroupRecords::GameMsg_C2S_GetDanceGroupRecords()
    : GameMsg_Base(MSG_C2S_GetDanceGroupRecords)
{
}

bool GameMsg_C2S_GetDanceGroupRecords::doDecode(CParamPool &IOBuff)
{
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_InviteDanceGroupMember::GameMsg_C2S_InviteDanceGroupMember()
    : GameMsg_Base(MSG_C2S_InviteDanceGroupMember)
    , m_nInviteeRoleID(0)
{
}

bool GameMsg_C2S_InviteDanceGroupMember::doDecode(CParamPool &IOBuff)
{
    m_nInviteeRoleID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_C2S_InviteDanceGroupMember::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nInviteeRoleID);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_AcceptDanceGroupMemberInvitation::GameMsg_C2S_AcceptDanceGroupMemberInvitation()
    : GameMsg_Base(MSG_C2S_AcceptDanceGroupMemberInvitation)
    , m_nDanceGroupID(0)
{
}

bool GameMsg_C2S_AcceptDanceGroupMemberInvitation::doDecode(CParamPool &IOBuff)
{
    m_nDanceGroupID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strInviterRoleName);

    return true;
}

bool GameMsg_C2S_AcceptDanceGroupMemberInvitation::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nDanceGroupID);
    IOBuff.AddStringW(m_strInviterRoleName);

    return true;
}

