#include "GameMsg_S2G_DanceGroup.h"
#include "DanceGroupMsgDef.h"
#include "../../socket/ParamPool.h"

// ----------------------------------------------- GameMsg_S2G_CreateDanceGroup

GameMsg_S2G_CreateDanceGroup::GameMsg_S2G_CreateDanceGroup()
    : GameMsg_Base(MSG_S2G_CreateDanceGroup)
    , m_strGroupName("")
    , m_nBadge(0)
    , m_strProfile("")
    , m_nLastOffLineTime(0)
{
}

bool GameMsg_S2G_CreateDanceGroup::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_strGroupName);
    m_nBadge = IOBuff.GetUShort();
    IOBuff.GetStringW(m_strProfile);
    m_nLastOffLineTime = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_CreateDanceGroup::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strGroupName.c_str());
    IOBuff.AddUShort(m_nBadge);
    IOBuff.AddStringW(m_strProfile.c_str());
    IOBuff.AddUInt(m_nLastOffLineTime);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_RequestEnterDanceGroup

GameMsg_S2G_RequestEnterDanceGroup::GameMsg_S2G_RequestEnterDanceGroup()
    : GameMsg_Base(MSG_S2G_RequestEnterDanceGroup)
    , m_nMyGroupID(0)
    , m_nRequestGroupID(0)
{
}

bool GameMsg_S2G_RequestEnterDanceGroup::doDecode(CParamPool &IOBuff)
{
    m_nMyGroupID = IOBuff.GetUInt();
    m_nRequestGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_RequestEnterDanceGroup::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nMyGroupID);
    IOBuff.AddUInt(m_nRequestGroupID);

    return true;
}


GameMsg_S2G_RequestEnterDanceGroupByName::GameMsg_S2G_RequestEnterDanceGroupByName()
    : GameMsg_Base(MSG_S2G_RequestEnterDanceGroupByName)
{

}

bool GameMsg_S2G_RequestEnterDanceGroupByName::doDecode(CParamPool &IOBuff)
{
    m_nMyGroupID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRequestDanceGroupName);

    return true;
}

bool GameMsg_S2G_RequestEnterDanceGroupByName::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nMyGroupID);
    IOBuff.AddStringW(m_strRequestDanceGroupName);

    return true;
}


// ----------------------------------------------- GameMsg_S2G_ReplyRequestEnterDanceGroup

GameMsg_S2G_ReplyRequestEnterDanceGroup::GameMsg_S2G_ReplyRequestEnterDanceGroup()
    : GameMsg_Base(MSG_S2G_ReplyRequestEnterDanceGroup)
    , m_nGroupID(0)
    , m_nRequestRoleID(0)
    , m_bAllow(false)
{
}

bool GameMsg_S2G_ReplyRequestEnterDanceGroup::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nRequestRoleID = IOBuff.GetUInt();
    m_bAllow = IOBuff.GetBool();

    return true;
}

bool GameMsg_S2G_ReplyRequestEnterDanceGroup::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUInt(m_nRequestRoleID);
    IOBuff.AddBool(m_bAllow);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_ReplayRequestListEnterDanceGroup

GameMsg_S2G_ReplyRequestListEnterDanceGroup::GameMsg_S2G_ReplyRequestListEnterDanceGroup()
    : GameMsg_Base(MSG_S2G_ReplyRequestListEnterDanceGroup)
    , m_bAllow(false)
    , m_nGroupID(0)
{
}

bool GameMsg_S2G_ReplyRequestListEnterDanceGroup::doDecode(CParamPool &IOBuff)
{
    m_bAllow = IOBuff.GetBool();
    m_nGroupID = IOBuff.GetUInt();

    unsigned short nLen = IOBuff.GetUShort();
    unsigned int nRequestID = 0;

    for(unsigned short i = 0; i < nLen; ++i)
    {
        nRequestID = IOBuff.GetUInt();

        m_listRoleID.push_back(nRequestID);
    }

    return true;
}

bool GameMsg_S2G_ReplyRequestListEnterDanceGroup::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bAllow);
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUShort((unsigned short)m_listRoleID.size());

    for (std::list<unsigned int>::const_iterator bIt = m_listRoleID.begin(), eIt = m_listRoleID.end(); 
         bIt != eIt; ++bIt)
    {
        IOBuff.AddUInt(*bIt);
    }

    return true;
}

// ----------------------------------------------- GameMsg_S2G_ExitDanceGroup

GameMsg_S2G_ExitDanceGroup::GameMsg_S2G_ExitDanceGroup()
    : GameMsg_Base(MSG_S2G_ExitDanceGroup)
    , m_nGroupID(0)
{
}

bool GameMsg_S2G_ExitDanceGroup::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_ExitDanceGroup::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_ChangeDanceGroupLeader

GameMsg_S2G_ChangeDanceGroupLeader::GameMsg_S2G_ChangeDanceGroupLeader()
    : GameMsg_Base(MSG_S2G_ChangeDanceGroupLeader)
    , m_nGroupID(0)
    , m_nNewLeaderID(0)
{
}

bool GameMsg_S2G_ChangeDanceGroupLeader::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nNewLeaderID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_ChangeDanceGroupLeader::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUInt(m_nNewLeaderID);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_DismissDanceGroup

GameMsg_S2G_DismissDanceGroup::GameMsg_S2G_DismissDanceGroup()
    : GameMsg_Base(MSG_S2G_DismissDanceGroup)
    , m_nGroupID(0)
{
}

bool GameMsg_S2G_DismissDanceGroup::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_DismissDanceGroup::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_CancelDismissDanceGroup

GameMsg_S2G_CancelDismissDanceGroup::GameMsg_S2G_CancelDismissDanceGroup()
    : GameMsg_Base(MSG_S2G_CancelDismissDanceGroup)
    , m_nGroupID(0)
{
}

bool GameMsg_S2G_CancelDismissDanceGroup::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_CancelDismissDanceGroup::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_ChangeDanceGroupTitle

GameMsg_S2G_ChangeDanceGroupTitle::GameMsg_S2G_ChangeDanceGroupTitle()
    : GameMsg_Base(MSG_S2G_ChangeDanceGroupTitle)
    , m_nGroupID(0)
    , m_nTargetRoleID(0)
    , m_nNewTitle(0)
{
}

bool GameMsg_S2G_ChangeDanceGroupTitle::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nTargetRoleID = IOBuff.GetUInt();
    m_nNewTitle = IOBuff.GetUChar();

    return true;
}

bool GameMsg_S2G_ChangeDanceGroupTitle::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUInt(m_nTargetRoleID);
    IOBuff.AddUChar(m_nNewTitle);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_KickOutDanceGroupMember

GameMsg_S2G_KickOutDanceGroupMember::GameMsg_S2G_KickOutDanceGroupMember()
    : GameMsg_Base(MSG_S2G_KickOutDanceGroupMember)
    , m_nGroupID(0)
    , m_nTargetRoleID(0)
{
}

bool GameMsg_S2G_KickOutDanceGroupMember::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nTargetRoleID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_KickOutDanceGroupMember::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUInt(m_nTargetRoleID);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_ChangeDanceGroupColor

GameMsg_S2G_ChangeDanceGroupColor::GameMsg_S2G_ChangeDanceGroupColor()
    : GameMsg_Base(MSG_S2G_ChangeDanceGroupColor)
    , m_nGroupID(0)
    , m_nColor(0)
{
}

bool GameMsg_S2G_ChangeDanceGroupColor::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nColor = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2G_ChangeDanceGroupColor::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUShort(m_nColor);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_ChangeDanceGroupBadge

GameMsg_S2G_ChangeDanceGroupBadge::GameMsg_S2G_ChangeDanceGroupBadge()
    : GameMsg_Base(MSG_S2G_ChangeDanceGroupBadge)
    , m_nGroupID(0)
    , m_nBadge(0)
{
}

bool GameMsg_S2G_ChangeDanceGroupBadge::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nBadge = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2G_ChangeDanceGroupBadge::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUShort(m_nBadge);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_ChangeDanceGroupTitleName

GameMsg_S2G_ChangeDanceGroupTitleName::GameMsg_S2G_ChangeDanceGroupTitleName()
    : GameMsg_Base(MSG_S2G_ChangeDanceGroupTitleName)
    , m_nGroupID(0)
    , m_nTitle(0)
    , m_strTitleName("")
{
}

bool GameMsg_S2G_ChangeDanceGroupTitleName::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nTitle = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strTitleName);

    return true;
}

bool GameMsg_S2G_ChangeDanceGroupTitleName::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUChar(m_nTitle);
    IOBuff.AddStringW(m_strTitleName.c_str());

    return true;
}

// ----------------------------------------------- GameMsg_S2G_ChangeDanceGroupProfile

GameMsg_S2G_ChangeDanceGroupProfile::GameMsg_S2G_ChangeDanceGroupProfile()
    : GameMsg_Base(MSG_S2G_ChangeDanceGroupProfile)
    , m_nGroupID(0)
    , m_strProfile("")
{
}

bool GameMsg_S2G_ChangeDanceGroupProfile::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strProfile);

    return true;
}

bool GameMsg_S2G_ChangeDanceGroupProfile::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddStringW(m_strProfile.c_str());

    return true;
}

// ----------------------------------------------- GameMsg_S2G_ChangeDanceGroupAnnouncement

GameMsg_S2G_ChangeDanceGroupAnnouncement::GameMsg_S2G_ChangeDanceGroupAnnouncement()
    : GameMsg_Base(MSG_S2G_ChangeDanceGroupAnnouncement)
    , m_nGroupID(0)
    , m_strAnnouncement("")
{
}

bool GameMsg_S2G_ChangeDanceGroupAnnouncement::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strAnnouncement);

    return true;
}

bool GameMsg_S2G_ChangeDanceGroupAnnouncement::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddStringW(m_strAnnouncement.c_str());

    return true;
}

// ----------------------------------------------- GameMsg_S2G_UpdateDanceGroupMemberInfo

GameMsg_S2G_UpdateDanceGroupMemberInfo::GameMsg_S2G_UpdateDanceGroupMemberInfo()
    : GameMsg_Base(MSG_S2G_UpdateDanceGroupMemberInfo)
    , m_nGroupID(0)
    , m_nState(0)
    , m_nLastOffLineTime(0)
{
}

bool GameMsg_S2G_UpdateDanceGroupMemberInfo::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nState = IOBuff.GetUChar();
    m_nLastOffLineTime = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_UpdateDanceGroupMemberInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUChar(m_nState);
    IOBuff.AddUInt(m_nLastOffLineTime);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_DonateDanceGroupResource

GameMsg_S2G_DonateDanceGroupResource::GameMsg_S2G_DonateDanceGroupResource()
    : GameMsg_Base(MSG_S2G_DonateDanceGroupResource)
    , m_nGroupID(0)
    , m_nResA(0)
    , m_nResB(0)
    , m_nResC(0)
{
}

bool GameMsg_S2G_DonateDanceGroupResource::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nResA = IOBuff.GetUInt();
    m_nResB = IOBuff.GetUInt();
    m_nResC = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_DonateDanceGroupResource::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUInt(m_nResA);
    IOBuff.AddUInt(m_nResB);
    IOBuff.AddUInt(m_nResC);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_UpgradeDanceGroup

GameMsg_S2G_UpgradeDanceGroup::GameMsg_S2G_UpgradeDanceGroup()
    : GameMsg_Base(MSG_S2G_UpgradeDanceGroup)
    , m_nGroupID(0)
{
}

bool GameMsg_S2G_UpgradeDanceGroup::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_UpgradeDanceGroup::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_UpgradeDanceGroup

GameMsg_S2G_ChangeDanceGroupName::GameMsg_S2G_ChangeDanceGroupName()
    : GameMsg_Base(MSG_S2G_ChangeDanceGroupName)
    , m_nGroupID(0)
    , m_strNewGroupName("")
{
}

bool GameMsg_S2G_ChangeDanceGroupName::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strNewGroupName);

    return true;
}

bool GameMsg_S2G_ChangeDanceGroupName::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddStringW(m_strNewGroupName.c_str());

    return true;
}

// ----------------------------------------------- GameMsg_S2G_ChangeDanceGroupHonor

GameMsg_S2G_ChangeDanceGroupHonor::GameMsg_S2G_ChangeDanceGroupHonor()
    : GameMsg_Base(MSG_S2G_ChangeDanceGroupHonor)
    , m_nGroupID(0)
    , m_nCause(0)
    , m_nPara(0)
    , m_nChangeHonor(0)
{
}

bool GameMsg_S2G_ChangeDanceGroupHonor::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nCause = IOBuff.GetInt();
    m_nPara = IOBuff.GetInt();
    m_nChangeHonor = IOBuff.GetInt();

    return true;
}

bool GameMsg_S2G_ChangeDanceGroupHonor::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddInt(m_nCause);
    IOBuff.AddInt(m_nPara);
    IOBuff.AddInt(m_nChangeHonor);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_UnlockDanceGroupBadge

GameMsg_S2G_UnlockDanceGroupBadge::GameMsg_S2G_UnlockDanceGroupBadge()
    : GameMsg_Base(MSG_S2G_UnlockDanceGroupBadge)
    , m_nGoupID(0)
    , m_nType(0)
    , m_nUnlockID(0)
    , m_nResA(0)
    , m_nResB(0)
    , m_nResC(0)
{
}

bool GameMsg_S2G_UnlockDanceGroupBadge::doDecode(CParamPool &IOBuff)
{
    m_nGoupID = IOBuff.GetUInt();
    m_nType = IOBuff.GetUShort();
    m_nUnlockID = IOBuff.GetUShort();
    m_nResA = IOBuff.GetUInt();
    m_nResB = IOBuff.GetUInt();
    m_nResC = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_UnlockDanceGroupBadge::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGoupID);
    IOBuff.AddUShort(m_nType);
    IOBuff.AddUShort(m_nUnlockID);
    IOBuff.AddUInt(m_nResA);
    IOBuff.AddUInt(m_nResB);
    IOBuff.AddUInt(m_nResC);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_ChangeDanceGroupBadgeOrEffect

GameMsg_S2G_ChangeDanceGroupBadgeOrEffect::GameMsg_S2G_ChangeDanceGroupBadgeOrEffect()
    : GameMsg_Base(MSG_S2G_ChangeDanceGroupBadgeOrEffect)
    , m_nGroupID(0)
    , m_nBadgeID(0)
    , m_nEffectID(0)
    , m_nResA(0)
    , m_nResB(0)
    , m_nResC(0)
{
}

bool GameMsg_S2G_ChangeDanceGroupBadgeOrEffect::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nBadgeID = IOBuff.GetUShort();
    m_nEffectID = IOBuff.GetUShort();
    m_nResA = IOBuff.GetUInt();
    m_nResB = IOBuff.GetUInt();
    m_nResC = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_ChangeDanceGroupBadgeOrEffect::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUShort(m_nBadgeID);
    IOBuff.AddUShort(m_nEffectID);
    IOBuff.AddUInt(m_nResA);
    IOBuff.AddUInt(m_nResB);
    IOBuff.AddUInt(m_nResC);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_DanceGroupTuanhuiPerDayConsume
GameMsg_S2G_DanceGroupTuanhuiPerDayConsume::GameMsg_S2G_DanceGroupTuanhuiPerDayConsume()
:GameMsg_Base(MSG_S2G_DanceGroupTuanhuiPerDayConsume)
,m_nGroupID(0)
,m_nResA(0)
,m_nResB(0)
,m_nResC(0)
{

}

bool GameMsg_S2G_DanceGroupTuanhuiPerDayConsume::doDecode(CParamPool &IOBuff)
{
	m_nGroupID = IOBuff.GetUInt();
	m_nResA = IOBuff.GetUInt();
	m_nResB = IOBuff.GetUInt();
	m_nResC = IOBuff.GetUInt();
	return true;
}//end

bool GameMsg_S2G_DanceGroupTuanhuiPerDayConsume::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nGroupID);
	IOBuff.AddUInt(m_nResA);
	IOBuff.AddUInt(m_nResB);
	IOBuff.AddUInt(m_nResC);
	return true;
}//end

// ----------------------------------------------- GameMsg_S2G_DanceGroupActivityAddStarPower

GameMsg_S2G_DanceGroupActivityAddStarPower::GameMsg_S2G_DanceGroupActivityAddStarPower()
    : GameMsg_Base(MSG_S2G_DanceGroupActivityAddStarPower) 
    , m_nGroupID(0)
    , m_nType(0)
{
}

bool GameMsg_S2G_DanceGroupActivityAddStarPower::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nType = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2G_DanceGroupActivityAddStarPower::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUShort(m_nType);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_DanceGroupActivityOpenStarPacket

GameMsg_S2G_DanceGroupActivityOpenStarPacket::GameMsg_S2G_DanceGroupActivityOpenStarPacket()
    : GameMsg_Base(MSG_S2G_DanceGroupActivityOpenStarPacket)
    , m_nGroupID(0)
    , m_nStarLevel(0)
{
}

bool GameMsg_S2G_DanceGroupActivityOpenStarPacket::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nStarLevel = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2G_DanceGroupActivityOpenStarPacket::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUShort(m_nStarLevel);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_UpdateDanceGroupActivityReward
GameMsg_S2G_UpdateDanceGroupActivityReward::GameMsg_S2G_UpdateDanceGroupActivityReward()
:GameMsg_Base(MSG_S2G_UpdateDanceGroupActivityReward)
,m_nGroupID(0)
,m_nRoleID(0)
,m_bReward(false)
{

}//end

bool GameMsg_S2G_UpdateDanceGroupActivityReward::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nGroupID);
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddBool(m_bReward);

	return true;
}//end

bool GameMsg_S2G_UpdateDanceGroupActivityReward::doDecode(CParamPool &IOBuff)
{
	m_nGroupID = IOBuff.GetUInt();
	m_nRoleID = IOBuff.GetUInt();
	m_bReward = IOBuff.GetBool();

	return true;
}//end


// ----------------------------------------------- GameMsg_S2G_DanceGroupShopExchange

GameMsg_S2G_DanceGroupShopExchange::GameMsg_S2G_DanceGroupShopExchange()
    : GameMsg_Base(MSG_S2G_DanceGroupShopExchange)
    , m_nGroupID(0)
    , m_nIndex(0)
{
}

bool GameMsg_S2G_DanceGroupShopExchange::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nIndex = IOBuff.GetUShort();
    m_nNeedContribution = IOBuff.GetUShort();
    m_nTotalCount = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2G_DanceGroupShopExchange::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUShort(m_nIndex);
    IOBuff.AddUShort(m_nNeedContribution);
    IOBuff.AddUShort(m_nTotalCount);

    return true;
}

// ----------------------------------------------- GameMsg_S2G_DanceGroupShopResetExchangeCount
GameMsg_S2G_DanceGroupShopResetExchangeCount::GameMsg_S2G_DanceGroupShopResetExchangeCount()
:GameMsg_Base(MSG_S2G_DanceGroupShopResetExchangeCount)
,m_nGroupID(0)
{

}
bool GameMsg_S2G_DanceGroupShopResetExchangeCount::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nGroupID);

	return true;
}//end

bool GameMsg_S2G_DanceGroupShopResetExchangeCount::doDecode(CParamPool &IOBuff)
{
	m_nGroupID = IOBuff.GetUInt();

	return true;
}//end

// ----------------------------------------------- GameMsg_S2G_GetDanceGroupRecrods

GameMsg_S2G_GetDanceGroupRecrods::GameMsg_S2G_GetDanceGroupRecrods()
    : GameMsg_Base(MSG_S2G_GetDanceGroupRecords)
    , m_nGroupID(0)
{
}

bool GameMsg_S2G_GetDanceGroupRecrods::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_GetDanceGroupRecrods::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nGroupID);

	return true;
}

// ----------------------------------------------- GameMsg_S2G_DanceGroupAddRecord
GameMsg_S2G_DanceGroupAddRecord::GameMsg_S2G_DanceGroupAddRecord()
:GameMsg_Base(MSG_S2G_DanceGroupAddRecord)
{

}//end
bool GameMsg_S2G_DanceGroupAddRecord::doEncode(CParamPool &IOBuff)
{
	m_recordInfo.doEncode(IOBuff);

	return true;
}//end

bool GameMsg_S2G_DanceGroupAddRecord::doDecode(CParamPool &IOBuff)
{
	m_recordInfo.doDecode(IOBuff);

	return true;
}//end

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_AddDanceGroupContribution::GameMsg_S2G_AddDanceGroupContribution()
    : GameMsg_Base(MSG_S2G_AddDanceGroupContribution)
    , m_nGroupID(0)
    , m_nChangeValue(0)
{
}

bool GameMsg_S2G_AddDanceGroupContribution::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nChangeValue = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_AddDanceGroupContribution::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUInt(m_nChangeValue);

    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2G_ReduceDanceGroupContribution::GameMsg_S2G_ReduceDanceGroupContribution()
    : GameMsg_Base(MSG_S2G_ReduceDanceGroupContribution)
    , m_nGroupID(0)
    , m_nChangeValue(0)
{
}

bool GameMsg_S2G_ReduceDanceGroupContribution::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nChangeValue = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_ReduceDanceGroupContribution::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUInt(m_nChangeValue);

    return true;
}


//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_GetDanceGroupInfo::GameMsg_S2G_GetDanceGroupInfo()
    : GameMsg_Base(MSG_S2G_GetDanceGroupInfo)
    , m_nGroupID(0)
{
}

bool GameMsg_S2G_GetDanceGroupInfo::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_GetDanceGroupInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_GetMyDanceGroupInfo::GameMsg_S2G_GetMyDanceGroupInfo()
    : GameMsg_Base(MSG_S2G_GetMyDanceGroupInfo)
    , m_nGroupID(0)
    , m_nClickedPlace(0)
{
}

bool GameMsg_S2G_GetMyDanceGroupInfo::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    m_nClickedPlace = IOBuff.GetUShort();

    return true;
}

bool GameMsg_S2G_GetMyDanceGroupInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUShort(m_nClickedPlace);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_DanceGroupActivityGetStarInfo::GameMsg_S2G_DanceGroupActivityGetStarInfo()
    : GameMsg_Base(MSG_S2G_DanceGroupActivityGetStarInfo)
    , m_nGroupID(0)
{
}

bool GameMsg_S2G_DanceGroupActivityGetStarInfo::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_DanceGroupActivityGetStarInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_DanceGroupActivityGetStarPowerInfo::GameMsg_S2G_DanceGroupActivityGetStarPowerInfo()
    : GameMsg_Base(MSG_S2G_DanceGroupActivityGetStarPowerInfo)
    , m_nGroupID(0)
{
}

bool GameMsg_S2G_DanceGroupActivityGetStarPowerInfo::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_DanceGroupActivityGetStarPowerInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_DanceGroupShopGetInfo::GameMsg_S2G_DanceGroupShopGetInfo()
    : GameMsg_Base(MSG_S2G_DanceGroupShopGetInfo)
    , m_nGroupID(0)
{
}

bool GameMsg_S2G_DanceGroupShopGetInfo::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_DanceGroupShopGetInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_AcceptDanceGroupMemberInvitation::GameMsg_S2G_AcceptDanceGroupMemberInvitation()
    : GameMsg_Base(MSG_S2G_AcceptDanceGroupMemberInvitation)
    , m_nDanceGroupID(0)
{
}

bool GameMsg_S2G_AcceptDanceGroupMemberInvitation::doDecode(CParamPool &IOBuff)
{
    m_nDanceGroupID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strInviterRoleName);

    return true;
}

bool GameMsg_S2G_AcceptDanceGroupMemberInvitation::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nDanceGroupID);
    IOBuff.AddStringW(m_strInviterRoleName);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_AddDanceGroupChallengeActive::GameMsg_S2G_AddDanceGroupChallengeActive()
    : GameMsg_Base( MSG_S2G_AddDanceGroupChallengeActive )
    , m_nGroupID( 0 )
    , m_nAddValue( 0 )
{
}

GameMsg_S2G_AddDanceGroupChallengeActive::~GameMsg_S2G_AddDanceGroupChallengeActive()
{
}

bool GameMsg_S2G_AddDanceGroupChallengeActive::doEncode( CParamPool& IOBuff )
{
    IOBuff << m_nGroupID << m_nAddValue;

    return true;
}

bool GameMsg_S2G_AddDanceGroupChallengeActive::doDecode( CParamPool& IOBuff )
{
    IOBuff >> m_nGroupID >> m_nAddValue;

    return true;
}


GameMsg_S2G_CanBeInvitedCheck::GameMsg_S2G_CanBeInvitedCheck()
    : GameMsg_Base(MSG_S2G_CanBeInviteCheck)
    , m_nToGroupID(0)
    , m_nInvitedRole(0)
{

}
GameMsg_S2G_CanBeInvitedCheck::~GameMsg_S2G_CanBeInvitedCheck()
{

}

bool GameMsg_S2G_CanBeInvitedCheck::doEncode( CParamPool& IOBuff )
{
    IOBuff.AddUInt(m_nToGroupID);
    IOBuff.AddUInt(m_nInvitedRole);

    return true;
}

bool GameMsg_S2G_CanBeInvitedCheck::doDecode( CParamPool& IOBuff )
{
    m_nToGroupID = IOBuff.GetUInt();
    m_nInvitedRole = IOBuff.GetUInt();

    return true;
}


GameMsg_G2S_CanBeInvitedCheck::GameMsg_G2S_CanBeInvitedCheck()
    : GameMsg_Base(MSG_G2S_CanBeInviteCheck)
    , m_nRet(0)
    , m_nInvitedRole(0)
{

}
GameMsg_G2S_CanBeInvitedCheck::~GameMsg_G2S_CanBeInvitedCheck()
{

}

bool GameMsg_G2S_CanBeInvitedCheck::doEncode( CParamPool& IOBuff )
{
    IOBuff.AddUChar(m_nRet);
    IOBuff.AddUInt(m_nInvitedRole);

    return true;
}

bool GameMsg_G2S_CanBeInvitedCheck::doDecode( CParamPool& IOBuff )
{
    m_nRet = IOBuff.GetUChar();
    m_nInvitedRole = IOBuff.GetUInt();

    return true;
}

