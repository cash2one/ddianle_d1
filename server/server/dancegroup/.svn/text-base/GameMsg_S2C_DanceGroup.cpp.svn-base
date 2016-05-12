#include "GameMsg_S2C_DanceGroup.h"
#include "DanceGroupMsgDef.h"
#include "../../socket/ParamPool.h"
#include "../../datastructure/DanceGroupInfoDef.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "../../datastructure/DanceGroupDataMgr.h"

// ----------------------------------------------- GameMsg_S2C_CreateDanceGroupResult

GameMsg_S2C_CreateDanceGroupResult::GameMsg_S2C_CreateDanceGroupResult()
    : GameMsg_Base(MSG_S2C_CreateDanceGroupResult)
    , m_nResult(ECreateDanceGroupResult_Success)
    , m_nCreateCD(0)
{
}

bool GameMsg_S2C_CreateDanceGroupResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);
    IOBuff.AddUInt(m_nCreateCD);
    return true;
}

// ----------------------------------------------- GameMsg_S2C_RequestEnterDanceGroupResult

GameMsg_S2C_RequestEnterDanceGroupResult::GameMsg_S2C_RequestEnterDanceGroupResult()
    : GameMsg_Base(MSG_S2C_RequestEnterDanceGroupResult)
    , m_nResult(ERequestEnterDanceGroupResult_Success)
	, m_fApplyCDTime(0.0f)
{
}

bool GameMsg_S2C_RequestEnterDanceGroupResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();
    m_fApplyCDTime = IOBuff.GetFloat();
    return true;
}

bool GameMsg_S2C_RequestEnterDanceGroupResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);
    IOBuff.AddFloat(m_fApplyCDTime);
    return true;
}

// ----------------------------------------------- GameMsg_S2C_CancelRequestEnterDanceGroupResult

GameMsg_S2C_CancelRequestEnterDanceGroupResult::GameMsg_S2C_CancelRequestEnterDanceGroupResult()
    : GameMsg_Base(MSG_S2C_CancelRequestEnterDanceGroupResult)
    , m_nResult(ECancelRequestEnterDanceGroupResult_Success)
{
}

bool GameMsg_S2C_CancelRequestEnterDanceGroupResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_CancelRequestEnterDanceGroupResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_EnterDanceGroupSuccess

GameMsg_S2C_EnterDanceGroupSuccess::GameMsg_S2C_EnterDanceGroupSuccess()
    : GameMsg_Base(MSG_S2C_EnterDanceGroupSuccess)
    , m_nGroupID(0)
{
}

bool GameMsg_S2C_EnterDanceGroupSuccess::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_GetDanceGroupInfoResult

GameMsg_S2C_GetDanceGroupInfoResult::GameMsg_S2C_GetDanceGroupInfoResult()
    : GameMsg_Base(MSG_S2C_GetDanceGroupInfoResult)
    , m_nResult(EGetDanceGroupInfoResult_Success)
{
}

bool GameMsg_S2C_GetDanceGroupInfoResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_GetDanceGroupInfoResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_SendDanceGroupInfo

GameMsg_S2C_SendDanceGroupInfo::GameMsg_S2C_SendDanceGroupInfo()
    : GameMsg_Base(MSG_S2C_SendDanceGroupInfo)
    , m_pDGInfo(NULL)
{
}

GameMsg_S2C_SendDanceGroupInfo::~GameMsg_S2C_SendDanceGroupInfo()
{
    delete m_pDGInfo;
    m_pDGInfo = NULL;
}

bool GameMsg_S2C_SendDanceGroupInfo::doEncode(CParamPool &IOBuff)
{
    if (NULL != m_pDGInfo)
        m_pDGInfo->doEncodeAllInfoToClient(IOBuff);

    return true;
}

// ----------------------------------------------- CDanceGroupItem

CDanceGroupItem::CDanceGroupItem()
{
    m_nGroupID = 0;
    m_strGroupName = "";
    m_nLeaderRoleID = 0;
    m_strLeaderRoleName = "";
    m_nLevel = 0;
    m_nBadge = 0;
    m_nHonor = 0;
    m_strProfile = "";
    m_nCreateTime = 0;
    m_bCanRequestEnter = true;
    m_bHaveRequest = false;
    m_nMemberCount = 0;
    m_nMaxMemberCount = 0;
    m_bLeaderIsVIP = false;
    m_nLeaderVIPLevel = 0;
    m_nEffectID = 0;
}

void CDanceGroupItem::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strGroupName);
    m_nLeaderRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strLeaderRoleName);
    m_nLevel = IOBuff.GetUShort();
    m_nBadge = IOBuff.GetUShort();
    m_nHonor = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strProfile);
    m_nCreateTime = IOBuff.GetUInt();
    m_bCanRequestEnter = IOBuff.GetBool();
    m_bHaveRequest = IOBuff.GetBool();
    m_nMemberCount = IOBuff.GetUShort();
    m_nMaxMemberCount = IOBuff.GetUShort();
    m_bLeaderIsVIP = IOBuff.GetBool();
    m_nLeaderVIPLevel = IOBuff.GetUShort();
    m_nEffectID = IOBuff.GetUShort();
}

void CDanceGroupItem::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddStringW(m_strGroupName.c_str());
    IOBuff.AddUInt(m_nLeaderRoleID);
    IOBuff.AddStringW(m_strLeaderRoleName.c_str());
    IOBuff.AddUShort(m_nLevel);
    IOBuff.AddUShort(m_nBadge);
    IOBuff.AddUInt(m_nHonor);
    IOBuff.AddStringW(m_strProfile.c_str());
    IOBuff.AddUInt(m_nCreateTime);
    IOBuff.AddBool(m_bCanRequestEnter);
    IOBuff.AddBool(m_bHaveRequest);
    IOBuff.AddUShort(m_nMemberCount);
    IOBuff.AddUShort(m_nMaxMemberCount);
    IOBuff.AddBool(m_bLeaderIsVIP);
    IOBuff.AddUShort(m_nLeaderVIPLevel);
    IOBuff.AddUShort(m_nEffectID);
}

// ----------------------------------------------- GameMsg_S2C_SendSimpleDanceGroupInfo

GameMsg_S2C_SendSimpleDanceGroupInfo::GameMsg_S2C_SendSimpleDanceGroupInfo()
    : GameMsg_Base(MSG_S2C_SendSimpleDanceGroupInfo)
{
}

bool GameMsg_S2C_SendSimpleDanceGroupInfo::doDecode(CParamPool &IOBuff)
{
    m_DGItem.doDecode(IOBuff);

    return true;
}

bool GameMsg_S2C_SendSimpleDanceGroupInfo::doEncode(CParamPool &IOBuff)
{
    m_DGItem.doEncode(IOBuff);
    return true;
}

// ----------------------------------------------- GameMsg_S2C_BeFefuseRefresh

GameMsg_S2C_BeFefuseRefresh::GameMsg_S2C_BeFefuseRefresh()
    : GameMsg_Base(MSG_S2C_DanceGroupBeRefuseRefresh)
{
}

bool GameMsg_S2C_BeFefuseRefresh::doDecode(CParamPool &IOBuff)
{
    m_DGItem.doDecode(IOBuff);

    return true;
}

bool GameMsg_S2C_BeFefuseRefresh::doEncode(CParamPool &IOBuff)
{
    m_DGItem.doEncode(IOBuff);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_SendDanceGroupInfoList

GameMsg_S2C_SendDanceGroupInfoList::GameMsg_S2C_SendDanceGroupInfoList()
    : GameMsg_Base(MSG_S2C_SendDanceGroupInfoList)
{
    m_nBeginIndex = 0;
    m_nCount = 0;
    m_nTotal = 0;
}

bool GameMsg_S2C_SendDanceGroupInfoList::doDecode(CParamPool &IOBuff)
{
    m_nBeginIndex = IOBuff.GetUInt();
    m_nCount = IOBuff.GetUShort();
    m_nTotal = IOBuff.GetUInt();

    unsigned short nCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nCount; ++i)
    {
        CDanceGroupItem item;
        item.doDecode(IOBuff);

        m_DGItemList.push_back(item);
    }

    return true;
}

bool GameMsg_S2C_SendDanceGroupInfoList::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nBeginIndex);
    IOBuff.AddUShort(m_nCount);
    IOBuff.AddUInt(m_nTotal);

    IOBuff.AddUShort((unsigned short)m_DGItemList.size());
    for (std::list<CDanceGroupItem>::iterator bIt = m_DGItemList.begin(), eIt = m_DGItemList.end(); 
         bIt != eIt; ++bIt)
    {
        bIt->doEncode(IOBuff);
    }

    return true;
}

// ----------------------------------------------- GameMsg_S2C_ExitDanceGroupResult

GameMsg_S2C_ExitDanceGroupResult::GameMsg_S2C_ExitDanceGroupResult()
    : GameMsg_Base(MSG_S2C_ExitDanceGroupResult)
    , m_nResult(EExitDanceGroupResult_Success)
{
}

bool GameMsg_S2C_ExitDanceGroupResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_ExitDanceGroupResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_ChangeDanceGroupLeaderResult

GameMsg_S2C_ChangeDanceGroupLeaderResult::GameMsg_S2C_ChangeDanceGroupLeaderResult()
    : GameMsg_Base(MSG_S2C_ChangeDanceGroupLeaderResult)
    , m_nResult(EChangeDanceGroupLeaderResult_Success)
{
}

bool GameMsg_S2C_ChangeDanceGroupLeaderResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_ChangeDanceGroupLeaderResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_DismissDanceGroupResult

GameMsg_S2C_DismissDanceGroupResult::GameMsg_S2C_DismissDanceGroupResult()
    : GameMsg_Base(MSG_S2C_DismissDanceGroupResult)
    , m_nResult(EDismissDanceGroupResult_Success)
{
}

bool GameMsg_S2C_DismissDanceGroupResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_DismissDanceGroupResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_CancelDismissDanceGroupResult

GameMsg_S2C_CancelDismissDanceGroupResult::GameMsg_S2C_CancelDismissDanceGroupResult()
    : GameMsg_Base(MSG_S2C_CancelDismissDanceGroupResult)
    , m_nResult(ECancelDismissDanceGroupResult_Success)
{
}

bool GameMsg_S2C_CancelDismissDanceGroupResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_CancelDismissDanceGroupResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_ChangeDanceGroupTitleResult

GameMsg_S2C_ChangeDanceGroupTitleResult::GameMsg_S2C_ChangeDanceGroupTitleResult()
    : GameMsg_Base(MSG_S2C_ChangeDanceGroupTitleResult)
    , m_nResult(EChangeDanceGroupTitleResult_Success)
{
}

bool GameMsg_S2C_ChangeDanceGroupTitleResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_ChangeDanceGroupTitleResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_KickOutDanceGroupMemberResult

GameMsg_S2C_KickOutDanceGroupMemberResult::GameMsg_S2C_KickOutDanceGroupMemberResult()
    : GameMsg_Base(MSG_S2C_KickOutDanceGroupMemberResult)
    , m_nResult(EKickOutDanceGroupMemberResult_Success)
{
}

bool GameMsg_S2C_KickOutDanceGroupMemberResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_KickOutDanceGroupMemberResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_KickedOutDanceGroup

GameMsg_S2C_KickedOutDanceGroup::GameMsg_S2C_KickedOutDanceGroup()
    : GameMsg_Base(MSG_S2C_KickedOutDanceGroup)
{
}

bool GameMsg_S2C_KickedOutDanceGroup::doDecode(CParamPool &IOBuff)
{
    return true;
}

bool GameMsg_S2C_KickedOutDanceGroup::doEncode(CParamPool &IOBuff)
{
    return true;
}

// ----------------------------------------------- GameMsg_S2C_ChangeDanceGroupColorResult

GameMsg_S2C_ChangeDanceGroupColorResult::GameMsg_S2C_ChangeDanceGroupColorResult()
    : GameMsg_Base(MSG_S2C_ChangeDanceGroupColorResult)
    , m_nResult(EChangeDanceGroupColorResult_Success)
{
}

bool GameMsg_S2C_ChangeDanceGroupColorResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_ChangeDanceGroupColorResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_ChangeDanceGroupBadgeResult

GameMsg_S2C_ChangeDanceGroupBadgeResult::GameMsg_S2C_ChangeDanceGroupBadgeResult()
    : GameMsg_Base(MSG_S2C_ChangeDanceGroupBadgeResult)
    , m_nResult(EChangeDanceGroupBadgeResult_Success)
{
}

bool GameMsg_S2C_ChangeDanceGroupBadgeResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_ChangeDanceGroupBadgeResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_ChangeDanceGroupTitleNameResult

GameMsg_S2C_ChangeDanceGroupTitleNameResult::GameMsg_S2C_ChangeDanceGroupTitleNameResult()
    : GameMsg_Base(MSG_S2C_ChangeDanceGroupTitleNameResult)
    , m_nResult(EChangeDanceGroupTitleNameResult_Success)
{
}

bool GameMsg_S2C_ChangeDanceGroupTitleNameResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_ChangeDanceGroupTitleNameResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_ChangeDanceGroupProfileResult

GameMsg_S2C_ChangeDanceGroupProfileResult::GameMsg_S2C_ChangeDanceGroupProfileResult()
    : GameMsg_Base(MSG_S2C_ChangeDanceGroupProfileResult)
    , m_nResult(EChangeDanceGroupProfileResult_Success)
{
}

bool GameMsg_S2C_ChangeDanceGroupProfileResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_ChangeDanceGroupProfileResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_ChangeDanceGroupAnnouncementResult

GameMsg_S2C_ChangeDanceGroupAnnouncementResult::GameMsg_S2C_ChangeDanceGroupAnnouncementResult()
    : GameMsg_Base(MSG_S2C_ChangeDanceGroupAnnouncementResult)
    , m_nResult(EChangeDanceGroupAnnouncementResult_Success)
{
}

bool GameMsg_S2C_ChangeDanceGroupAnnouncementResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_ChangeDanceGroupAnnouncementResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_DanceGroupRemoved

GameMsg_S2C_DanceGroupRemoved::GameMsg_S2C_DanceGroupRemoved()
    : GameMsg_Base(MSG_S2C_DanceGroupRemoved)
{
}

bool GameMsg_S2C_DanceGroupRemoved::doDecode(CParamPool &IOBuff)
{
    return true;
}

bool GameMsg_S2C_DanceGroupRemoved::doEncode(CParamPool &IOBuff)
{
    return true;
}

// ----------------------------------------------- GameMsg_S2C_UpdateDanceGroupBaseInfo

GameMsg_S2C_UpdateDanceGroupBaseInfo::GameMsg_S2C_UpdateDanceGroupBaseInfo()
    : GameMsg_Base(MSG_S2C_UpdateDanceGroupBaseInfo)
{
}

bool GameMsg_S2C_UpdateDanceGroupBaseInfo::doEncode(CParamPool &IOBuff)
{
    m_DGBaseInfo.doEncodeToClientData(IOBuff);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_AddDanceGroupMember

GameMsg_S2C_AddDanceGroupMember::GameMsg_S2C_AddDanceGroupMember()
    : GameMsg_Base(MSG_S2C_AddDanceGroupMember)
{
}

bool GameMsg_S2C_AddDanceGroupMember::doEncode(CParamPool &IOBuff)
{
    m_DGMember.doEncodeToClient(IOBuff);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_UpdateDanceGroupMemberInfo

GameMsg_S2C_UpdateDanceGroupMemberInfo::GameMsg_S2C_UpdateDanceGroupMemberInfo()
    : GameMsg_Base(MSG_S2C_UpdateDanceGroupMemberInfo)
{
}

bool GameMsg_S2C_UpdateDanceGroupMemberInfo::doEncode(CParamPool &IOBuff)
{
    m_DGMember.doEncodeToClient(IOBuff);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_RemoveDanceGroupMember

GameMsg_S2C_RemoveDanceGroupMember::GameMsg_S2C_RemoveDanceGroupMember()
    : GameMsg_Base(MSG_S2C_RemoveDanceGroupMember)
{
}

bool GameMsg_S2C_RemoveDanceGroupMember::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2C_RemoveDanceGroupMember::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_NotifyRequestEnterDanceGroup

GameMsg_S2C_NotifyRequestEnterDanceGroup::GameMsg_S2C_NotifyRequestEnterDanceGroup()
    : GameMsg_Base(MSG_S2C_NotifyRequestEnterDanceGroup)
{
}

bool GameMsg_S2C_NotifyRequestEnterDanceGroup::doEncode(CParamPool &IOBuff)
{
    m_RequestInfo.doEncodeToClient(IOBuff);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_NotifyRemoveRequestEnter

GameMsg_S2C_NotifyRemoveRequestEnter::GameMsg_S2C_NotifyRemoveRequestEnter()
    : GameMsg_Base(MSG_S2C_NotifyRemoveRequestEnter)
    , m_nRoleID(0)
{
}

bool GameMsg_S2C_NotifyRemoveRequestEnter::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);

    return true;
}


// ----------------------------------------------- GameMsg_S2C_ReplyRequestEnterDanceGroupResult

GameMsg_S2C_ReplyRequestEnterDanceGroupResult::GameMsg_S2C_ReplyRequestEnterDanceGroupResult()
    : GameMsg_Base(MSG_S2C_ReplyRequestEnterDanceGroupResult)
    , m_nRequestRoleID(0)
    , m_nResult(EReplyRequestEnterDanceGroupResult_Success)
{
}

bool GameMsg_S2C_ReplyRequestEnterDanceGroupResult::doDecode(CParamPool &IOBuff)
{
    m_nRequestRoleID = IOBuff.GetUInt();
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_ReplyRequestEnterDanceGroupResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRequestRoleID);
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_ReplyRequestListEnterDanceGroupResult

GameMsg_S2C_ReplyRequestListEnterDanceGroupResult::GameMsg_S2C_ReplyRequestListEnterDanceGroupResult()
    : GameMsg_Base(MSG_S2C_ReplyRequestListEnterDanceGroupResult)
    , m_nRoleID(0)
    , m_nAllowCount(0)
    , m_nResult(0)
{
}

bool GameMsg_S2C_ReplyRequestListEnterDanceGroupResult::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nAllowCount = IOBuff.GetUInt();
    m_nResult = IOBuff.GetUShort();
    m_bAllow = IOBuff.GetBool();

    return true;
}

bool GameMsg_S2C_ReplyRequestListEnterDanceGroupResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUInt(m_nAllowCount);
    IOBuff.AddUShort(m_nResult);
    IOBuff.AddBool(m_bAllow);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_DonateDanceGroupResourceResult

GameMsg_S2C_DonateDanceGroupResourceResult::GameMsg_S2C_DonateDanceGroupResourceResult()
    : GameMsg_Base(MSG_S2C_DonateDanceGroupResourceResult)
    , m_nResult(EDonateDanceGroupResourceResult_Success)
{
}

bool GameMsg_S2C_DonateDanceGroupResourceResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_DonateDanceGroupResourceResult

GameMsg_S2C_UpgradeDanceGroupResult::GameMsg_S2C_UpgradeDanceGroupResult()
    : GameMsg_Base(MSG_S2C_UpgradeDanceGroupResult)
    , m_nResult(EUpgradeGroupDanceResult_Success)
    , m_nCurLevel(0)
    , m_nNextLevelUpResA(0)
    , m_nNextLevelUpResB(0)
    , m_nNextLevelUpResC(0)
{
}

bool GameMsg_S2C_UpgradeDanceGroupResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();
    m_nCurLevel = IOBuff.GetUShort();
    m_nNextLevelUpResA = IOBuff.GetUInt();
    m_nNextLevelUpResB = IOBuff.GetUInt();
    m_nNextLevelUpResC = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2C_UpgradeDanceGroupResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);
    IOBuff.AddUShort(m_nCurLevel);
    IOBuff.AddUInt(m_nNextLevelUpResA);
    IOBuff.AddUInt(m_nNextLevelUpResB);
    IOBuff.AddUInt(m_nNextLevelUpResC);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_ChangeDanceGroupNameResult

GameMsg_S2C_ChangeDanceGroupNameResult::GameMsg_S2C_ChangeDanceGroupNameResult()
    : GameMsg_Base(MSG_S2C_ChangeDanceGroupNameResult)
    , m_nResult(EChangeDanceGroupNameResult_Success)
    , m_strNewGroupName("")
    , m_nTimeCoolDown(0)
{
}

bool GameMsg_S2C_ChangeDanceGroupNameResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();
    IOBuff.GetStringW(m_strNewGroupName);
    m_nTimeCoolDown = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2C_ChangeDanceGroupNameResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);
    IOBuff.AddStringW(m_strNewGroupName.c_str());
    IOBuff.AddUInt(m_nTimeCoolDown);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_UpdateDanceGroupAnnounce

GameMsg_S2C_UpdateDanceGroupAnnounce::GameMsg_S2C_UpdateDanceGroupAnnounce()
    : GameMsg_Base(MSG_S2C_UpdateDanceGroupAnnounce)
    , m_strNewAnnounce("")
{
}

bool GameMsg_S2C_UpdateDanceGroupAnnounce::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_strNewAnnounce);

    return true;
}

bool GameMsg_S2C_UpdateDanceGroupAnnounce::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strNewAnnounce.c_str());

    return true;
}

// ----------------------------------------------- GameMsg_S2C_UpdateDanceGroupProfile

GameMsg_S2C_UpdateDanceGroupProfile::GameMsg_S2C_UpdateDanceGroupProfile()
    : GameMsg_Base(MSG_S2C_UpdateDanceGroupProfile)
    , m_strNewProfile("")
{
}

bool GameMsg_S2C_UpdateDanceGroupProfile::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_strNewProfile);

    return true;
}

bool GameMsg_S2C_UpdateDanceGroupProfile::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strNewProfile.c_str());

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_DanceGroupInfo::GameMsg_S2C_DanceGroupInfo()
    : GameMsg_Base(MSG_S2C_DanceGroupInfo)
    , m_nGroupID(0)
    , m_bIsValid(false)
    , m_nClickedPlace(0)
    , m_pDGInfo(NULL)
{
}

GameMsg_S2C_DanceGroupInfo::~GameMsg_S2C_DanceGroupInfo()
{
    if (m_pDGInfo != NULL)
    {
        delete m_pDGInfo;
        m_pDGInfo = NULL;
    }
}

bool GameMsg_S2C_DanceGroupInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddBool(m_bIsValid);
    IOBuff.AddUShort(m_nClickedPlace);
    IOBuff.AddBool(m_pDGInfo != NULL);

    if (m_pDGInfo != NULL)
        m_pDGInfo->doEncodeAllInfoToClient(IOBuff);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_GetDanceGroupBadgeInfosResult

GameMsg_S2C_GetDanceGroupBadgeInfosResult::GameMsg_S2C_GetDanceGroupBadgeInfosResult()
    : GameMsg_Base(MSG_S2C_GetDanceGroupBadgeInfosResult)
    , m_nErrorCode(0)
{
}

bool GameMsg_S2C_GetDanceGroupBadgeInfosResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorCode = IOBuff.GetUShort();

    unsigned short count = IOBuff.GetUShort();

    for (unsigned short i = 0; i < count; ++i)
    {
        m_vectBadgeEffect.push_back(IOBuff.GetUShort());
        m_vectEffectState.push_back(IOBuff.GetBool());
    }

    return true;
}

bool GameMsg_S2C_GetDanceGroupBadgeInfosResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nErrorCode);

    unsigned int count = m_vectBadgeEffect.size();

    if (count > m_vectEffectState.size())
        count = m_vectEffectState.size();

    IOBuff.AddUShort((unsigned short)count);
    for (unsigned int i = 0; i < count; ++i)
    {
        IOBuff.AddUShort(m_vectBadgeEffect[i]);
        IOBuff.AddBool(m_vectEffectState[i]);
    }

    return true;
}

// ----------------------------------------------- GameMsg_S2C_UnlockDanceGroupBadgeResult

GameMsg_S2C_UnlockDanceGroupBadgeResult::GameMsg_S2C_UnlockDanceGroupBadgeResult()
    : GameMsg_Base(MSG_S2C_UnlockDanceGroupBadgeResult)
    , m_nType(0)
    , m_nUnlockID(0)
    , m_nErrorCode(0)
{
}

bool GameMsg_S2C_UnlockDanceGroupBadgeResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorCode = IOBuff.GetUShort();
    m_nType = IOBuff.GetUShort();
    m_nUnlockID = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_UnlockDanceGroupBadgeResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nErrorCode);
    IOBuff.AddUShort(m_nType);
    IOBuff.AddUShort(m_nUnlockID);

    return true;
}

// ----------------------------------------------- GameMsg_G2S_ChangeDanceGroupBadgeOrEffectResult

GameMsg_G2S_ChangeDanceGroupBadgeOrEffectResult::GameMsg_G2S_ChangeDanceGroupBadgeOrEffectResult()
    : GameMsg_Base(MSG_G2S_ChangeDanceGroupBadgeOrEffectResult)
    , m_nErrorCode(0)
    , m_nRoleID(0)
    , m_nBadge(0)
    , m_nEffect(0)
{
}

bool GameMsg_G2S_ChangeDanceGroupBadgeOrEffectResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorCode = IOBuff.GetUShort();
    m_nRoleID = IOBuff.GetUInt();
    m_nBadge = IOBuff.GetUShort();
    m_nEffect = IOBuff.GetUShort();

    return true;
}

bool GameMsg_G2S_ChangeDanceGroupBadgeOrEffectResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nErrorCode);
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUShort(m_nBadge);
    IOBuff.AddUShort(m_nEffect);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_DanceGroupActivityStarGetInfo

GameMsg_S2C_DanceGroupActivityGetStarInfoResult::GameMsg_S2C_DanceGroupActivityGetStarInfoResult()
    : GameMsg_Base(MSG_S2C_DanceGroupActivityGetStarInfoResult)
    , m_nErrorCode(EDanceGroupActivityStar_Success)
    , m_nDayPacket(0)
    , m_nCurrentStarLevel(0)
    , m_nCurrentPowerValue(0)
    , m_nNormalCount(0)
    , m_nMoneyCount(0)
    , m_nPointCount(0)
    , m_nMoneyNum(0)
    , m_nMCoinNum(0)
    , m_nTotalNormalCount(0)
    , m_nTotalMoneyCount(0)
    , m_nTotalMCoinCount(0)
{
}

bool GameMsg_S2C_DanceGroupActivityGetStarInfoResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorCode = IOBuff.GetUShort();
    m_nDayPacket = IOBuff.GetUShort();
    m_nCurrentStarLevel = IOBuff.GetUShort();
    m_nCurrentPowerValue = IOBuff.GetUInt();

    m_nNormalCount = IOBuff.GetUShort();
    m_nTotalNormalCount = IOBuff.GetUShort();

    m_nMoneyCount = IOBuff.GetUShort();
    m_nTotalMoneyCount = IOBuff.GetUShort();
    m_nMoneyNum = IOBuff.GetUShort();

    m_nPointCount = IOBuff.GetUShort();
    m_nTotalMCoinCount = IOBuff.GetUShort();
    m_nMCoinNum = IOBuff.GetUShort();

    IOBuff.GetStringW(m_strRuleContent);

    unsigned short i = 0;
    unsigned short count = IOBuff.GetUShort();
    for (i = 0; i < count; ++i)
    {
        CDGActivityStarBaseInfo baseInfo;

        baseInfo.m_nStarLevel = IOBuff.GetUShort();
        baseInfo.m_nProgress = IOBuff.GetUShort();
        baseInfo.m_nContributionLow = (unsigned int)IOBuff.GetUShort();
        baseInfo.m_nContributionMid = (unsigned int)IOBuff.GetUShort();
        baseInfo.m_nContributionHigh = (unsigned int)IOBuff.GetUShort();
        IOBuff.GetStringW(baseInfo.m_strName);

        m_mStarProgress.insert(std::make_pair(baseInfo.m_nStarLevel, baseInfo));
    }

    CItem item;
    std::list<CItem> listItem;
    unsigned short j = 0;
    unsigned short key = 0;
    unsigned short innerCount = 0;

    count = IOBuff.GetUShort();
    for (i = 0; i < count; ++i)
    {
        listItem.clear();

        key = IOBuff.GetUShort();
        innerCount = IOBuff.GetUShort();

        for (j = 0; j < innerCount; ++j)
        {
            item.doDecode(IOBuff);

            listItem.push_back(item);
        }

        m_mPacketList.insert(std::make_pair(key, listItem));
    }

    count = IOBuff.GetUShort();
    for (i = 0; i < count; ++i)
    {
        m_listOpenPacket.push_back(IOBuff.GetUShort());
    }

    count = IOBuff.GetUShort();
    for (i = 0; i < count; ++i)
    {
        item.doDecode(IOBuff);

        m_listReward.push_back(item);
    }

    return true;
}

bool GameMsg_S2C_DanceGroupActivityGetStarInfoResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nErrorCode);
    IOBuff.AddUShort(m_nDayPacket);
    IOBuff.AddUShort(m_nCurrentStarLevel);
    IOBuff.AddUInt(m_nCurrentPowerValue);

    IOBuff.AddUShort(m_nNormalCount);
    IOBuff.AddUShort(m_nTotalNormalCount);

    IOBuff.AddUShort(m_nMoneyCount);
    IOBuff.AddUShort(m_nTotalMoneyCount);
    IOBuff.AddUShort(m_nMoneyNum);

    IOBuff.AddUShort(m_nPointCount);
    IOBuff.AddUShort(m_nTotalMCoinCount);
    IOBuff.AddUShort(m_nMCoinNum);

    IOBuff.AddStringW(m_strRuleContent);

    IOBuff.AddUShort((unsigned short)m_mStarProgress.size());
    for (std::map<unsigned short, CDGActivityStarBaseInfo>::const_iterator bIt = m_mStarProgress.begin(), 
        eIt = m_mStarProgress.end(); bIt != eIt; ++bIt)
    {
        IOBuff.AddUShort(bIt->second.m_nStarLevel);
        IOBuff.AddUShort(bIt->second.m_nProgress);
        IOBuff.AddUShort((unsigned short)bIt->second.m_nContributionLow);
        IOBuff.AddUShort((unsigned short)bIt->second.m_nContributionMid);
        IOBuff.AddUShort((unsigned short)bIt->second.m_nContributionHigh);
        IOBuff.AddStringW(bIt->second.m_strName.c_str());
    }

    IOBuff.AddUShort((unsigned short)m_mPacketList.size());
    for (std::map<unsigned short, std::list<CItem> >::iterator bOuterIt = m_mPacketList.begin(), 
         eOuterIt = m_mPacketList.end(); bOuterIt != eOuterIt; ++bOuterIt)
    {
        IOBuff.AddUShort(bOuterIt->first);
        IOBuff.AddUShort((unsigned short)bOuterIt->second.size());

        for (std::list<CItem>::iterator bInnerIt = bOuterIt->second.begin(), 
             eInnerIt = bOuterIt->second.end(); bInnerIt != eInnerIt; ++bInnerIt)
        {
            bInnerIt->doEncode(IOBuff);
        }
    }

    IOBuff.AddUShort((unsigned short)m_listOpenPacket.size());
    for (std::list<unsigned short>::const_iterator bIt = m_listOpenPacket.begin(), 
         eIt = m_listOpenPacket.end(); bIt != eIt; ++bIt)
    {
        IOBuff.AddUShort(*bIt);
    }

    IOBuff.AddUShort((unsigned short)m_listReward.size());
    for (std::list<CItem>::iterator bIt = m_listReward.begin(), 
         eIt = m_listReward.end(); bIt != eIt; ++bIt)
    {
        bIt->doEncode(IOBuff);
    }

    return true;
}

// ----------------------------------------------- GameMsg_S2C_DanceGroupActivityGetStarPowerInfoResult

GameMsg_S2C_DanceGroupActivityGetStarPowerInfoResult::GameMsg_S2C_DanceGroupActivityGetStarPowerInfoResult()
    : GameMsg_Base(MSG_S2C_DanceGroupActivityGetStarPowerInfoResult)
    , m_nErrorCode(0)
    , m_nStarLevel(0)
    , m_nCrurrentPower(0)
{
}

bool GameMsg_S2C_DanceGroupActivityGetStarPowerInfoResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorCode = IOBuff.GetUShort();
    m_nStarLevel = IOBuff.GetUInt();
    m_nCrurrentPower = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2C_DanceGroupActivityGetStarPowerInfoResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nErrorCode);
    IOBuff.AddUInt(m_nStarLevel);
    IOBuff.AddUInt(m_nCrurrentPower);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_DanceGroupActivityStarAddPowerResult

GameMsg_S2C_DanceGroupActivityAddStarPowerResult::GameMsg_S2C_DanceGroupActivityAddStarPowerResult()
    : GameMsg_Base(MSG_S2C_DanceGroupActivityAddStarPowerResult)
    , m_nErrorCode(0)
    , m_nType(0)
    , m_nStarLevel(1)
    , m_nCurrentPowerValue(0)
    , m_bReward(false)
{
}

bool GameMsg_S2C_DanceGroupActivityAddStarPowerResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorCode = IOBuff.GetUShort();
    m_nType = IOBuff.GetUShort();
    m_nStarLevel = IOBuff.GetUShort();
    m_nCurrentPowerValue = IOBuff.GetUShort();
    m_bReward = IOBuff.GetBool();

    return true;
}

bool GameMsg_S2C_DanceGroupActivityAddStarPowerResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nErrorCode);
    IOBuff.AddUShort(m_nType);
    IOBuff.AddUShort(m_nStarLevel);
    IOBuff.AddUShort(m_nCurrentPowerValue);
    IOBuff.AddBool(m_bReward);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_DanceGroupActivityStarAddPowerMCoin2MoneyResult

GameMsg_S2C_DanceGroupActivityStarAddPowerMCoin2MoneyResult::GameMsg_S2C_DanceGroupActivityStarAddPowerMCoin2MoneyResult()
    : GameMsg_Base(MSG_S2C_DanceGroupActivityStarAddPowerResultMCoin2MoneyResult)
{
}

bool GameMsg_S2C_DanceGroupActivityStarAddPowerMCoin2MoneyResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nErrorCode);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_NotifyDanceGroupActivityStarState

GameMsg_S2C_NotifyDanceGroupActivityStarState::GameMsg_S2C_NotifyDanceGroupActivityStarState()
    : GameMsg_Base(MSG_S2C_NotifyDanceGroupActivityStarState)
    , m_nOldStarLevel(1)
    , m_nStarLevel(1)
    , m_nCurrentPower(0)
{
}

bool GameMsg_S2C_NotifyDanceGroupActivityStarState::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nOldStarLevel);
    IOBuff.AddUShort(m_nStarLevel);
    IOBuff.AddUInt(m_nCurrentPower);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_DanceGroupActivityStarOpenPacket

GameMsg_S2C_DanceGroupActivityOpenStarPacketResult::GameMsg_S2C_DanceGroupActivityOpenStarPacketResult()
    : GameMsg_Base(MSG_S2C_DanceGroupActivityOpenStarPacketResult)
    , m_nErrorCode(0)
    , m_nStarLevel(0)
{
}

bool GameMsg_S2C_DanceGroupActivityOpenStarPacketResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorCode = IOBuff.GetUShort();
    m_nStarLevel = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_DanceGroupActivityOpenStarPacketResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nErrorCode);
    IOBuff.AddUShort(m_nStarLevel);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_DanceGroupActivityReset
GameMsg_S2C_DanceGroupActivityReset::GameMsg_S2C_DanceGroupActivityReset()
:GameMsg_Base(MSG_S2C_DanceGroupActivityReset)
{

}//end

bool GameMsg_S2C_DanceGroupActivityReset::doEncode(CParamPool &IOBuff)
{
	return true;
}//end

// ----------------------------------------------- GameMsg_S2C_DanceGroupShopGetInfoResult
DanceGroupShopExchangeInfo::DanceGroupShopExchangeInfo()
    : m_nIndex(0)
    , m_nRequestMoneyType(0)
    , m_nRequestMoneyCount(0)
    , m_nRequestContribution(0)
{}
DanceGroupShopExchangeInfo::~DanceGroupShopExchangeInfo()
{}
void DanceGroupShopExchangeInfo::Encode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nIndex);
    m_ExchangeItem.doEncode(IOBuff);

    IOBuff.AddUShort((unsigned short)m_ListRequestItem.size());
    for (unsigned short i = 0; i < m_ListRequestItem.size(); ++i)
    {
        m_ListRequestItem[i].doEncode(IOBuff);
    }

    IOBuff.AddUChar(m_nRequestMoneyType);
    IOBuff.AddUInt(m_nRequestMoneyCount);
    IOBuff.AddUInt(m_nRequestContribution);

    return;
}

GameMsg_S2C_DanceGroupShopGetInfoResult::GameMsg_S2C_DanceGroupShopGetInfoResult()
    : GameMsg_Base(MSG_S2C_DanceGroupShopGetInfoResult)
    , m_nErrorCode(0)
{
}

bool GameMsg_S2C_DanceGroupShopGetInfoResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);

    IOBuff.AddUShort((unsigned short)m_vecExchange.size());
    for (size_t i = 0; i < m_vecExchange.size(); ++i)
    {
        m_vecExchange[i].Encode(IOBuff);
    }

    return true;
}

// ----------------------------------------------- GameMsg_S2C_DanceGroupShopExchangeResult

GameMsg_S2C_DanceGroupShopExchangeResult::GameMsg_S2C_DanceGroupShopExchangeResult()
    : GameMsg_Base(MSG_S2C_DanceGroupShopExchangeResult)
    , m_nErrorCode(0)
    , m_nIndex(0)
{
}

bool GameMsg_S2C_DanceGroupShopExchangeResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorCode = IOBuff.GetUChar();
    m_nIndex = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2C_DanceGroupShopExchangeResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);
    IOBuff.AddUInt(m_nIndex);

    return true;
}

// ----------------------------------------------- GameMsg_S2C_GetDanceGroupRecrodsResult

GameMsg_S2C_GetDanceGroupRecrodsResult::GameMsg_S2C_GetDanceGroupRecrodsResult()
    : GameMsg_Base(MSG_S2C_GetDanceGroupRecrodsResult)
    , m_nErrorCode(0)
{
}

bool GameMsg_S2C_GetDanceGroupRecrodsResult::doDecode(CParamPool &IOBuff)
{
    m_nErrorCode = IOBuff.GetUShort();

    unsigned short count = IOBuff.GetUShort();

    for (unsigned short i = 0; i < count; ++i)
    {
        CDanceGroupRecordsInfo recordsInfo;

        recordsInfo.doDecode(IOBuff);

        m_recordList.insert(std::make_pair(recordsInfo.m_nRecordTime, recordsInfo));
    }

    return true;
}

bool GameMsg_S2C_GetDanceGroupRecrodsResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nErrorCode);
    IOBuff.AddUShort((unsigned short)m_recordList.size());

    for (std::map<unsigned int, CDanceGroupRecordsInfo>::iterator bIt = m_recordList.begin(), 
         eIt = m_recordList.end(); bIt != eIt; ++bIt)
    {
        bIt->second.doEncode(IOBuff);
    }

    return true;
}


// ----------------------------------------------- GameMsg_S2C_DanceGroupRecrodsNotify
GameMsg_S2C_DanceGroupAddRecrodsNotify::GameMsg_S2C_DanceGroupAddRecrodsNotify()
:GameMsg_Base(MSG_S2C_DanceGroupAddRecordsNotify)
{

}


bool GameMsg_S2C_DanceGroupAddRecrodsNotify::doEncode(CParamPool &IOBuff)
{
	m_recordInfo.doEncode(IOBuff);

	return true;
}//end


// ----------------------------------------------- GameMsg_S2C_DanceGroupChangeDayNotify
GameMsg_S2C_DanceGroupChangeDayNotify::GameMsg_S2C_DanceGroupChangeDayNotify()
:GameMsg_Base(MSG_S2C_DanceGroupChangeDayNotify)
,m_nErrorCode(0)
{

}//end


bool GameMsg_S2C_DanceGroupChangeDayNotify::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nErrorCode);

	unsigned short nLen = (unsigned short)m_recordList.size();
	IOBuff.AddUShort(nLen);

	std::map<unsigned int, CDanceGroupRecordsInfo>::iterator iter = m_recordList.begin(), iterEnd = m_recordList.end();
	for(; iter != iterEnd; ++iter)
	{
		iter->second.doEncode(IOBuff);
	}//end for

	return true;
}//end

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_InviteDanceGroupMemberResult::GameMsg_S2C_InviteDanceGroupMemberResult()
    : GameMsg_Base(MSG_S2C_InviteDanceGroupMemberResult)
    , m_nResult(EInviteDanceGroupMemberResult_Success)
{
}

bool GameMsg_S2C_InviteDanceGroupMemberResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_InviteDanceGroupMemberResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_BeInvitedAsDanceGroupMember::GameMsg_S2C_BeInvitedAsDanceGroupMember()
    : GameMsg_Base(MSG_S2C_BeInvitedAsDanceGroupMember)
    , m_nDanceGroupID(0)
{
}

bool GameMsg_S2C_BeInvitedAsDanceGroupMember::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_strInviterRoleName);
    m_nDanceGroupID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strDanceGroupName);

    return true;
}

bool GameMsg_S2C_BeInvitedAsDanceGroupMember::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strInviterRoleName);
    IOBuff.AddUInt(m_nDanceGroupID);
    IOBuff.AddStringW(m_strDanceGroupName);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_AcceptDanceGroupMemberInvitationResult::GameMsg_S2C_AcceptDanceGroupMemberInvitationResult()
    : GameMsg_Base(MSG_S2C_AcceptDanceGroupMemberInvitationResult)
    , m_nResult(EInviteDanceGroupMemberResult_Success)
{
}

bool GameMsg_S2C_AcceptDanceGroupMemberInvitationResult::doDecode(CParamPool &IOBuff)
{
    m_nResult = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2C_AcceptDanceGroupMemberInvitationResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nResult);

    return true;
}

