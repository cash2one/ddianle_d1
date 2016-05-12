#include "VIPStruct.h"
#include "../../socket/ParamPool.h"


CVIPExpInfo::CVIPExpInfo()
:m_nCurLevel(0)
,m_nExpLevepUpNeed(0)
{

}

CVIPExpInfo::~CVIPExpInfo()
{
}

void CVIPExpInfo::Serialize(CParamPool & IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_nCurLevel);
	IOBuff.AddInt(m_nExpLevepUpNeed);
}

CVIPLevelUpReward::CVIPLevelUpReward()
:m_nLevel(0)
,m_nMoney(0)
{

}

CVIPLevelUpReward::~CVIPLevelUpReward()
{

}

void CVIPLevelUpReward::Serialize( CParamPool & IOBuff, unsigned char nSex )
{
	std::list<CItem> listItem;
	if (nSex == ESexType_Male) {
		listItem = m_listMaleItem;
	} else {
		listItem = m_listFemaleItem;
	}
	IOBuff.AddUShort((unsigned short)listItem.size());
	for (std::list<CItem>::iterator itr = listItem.begin(); itr != listItem.end(); ++itr) {
		itr->doEncode(IOBuff);
	}
	IOBuff.AddInt(m_nMoney);	
}

/////////////////////////////////////////////
CVIPLevelPrivInfo::CVIPLevelPrivInfo()
    : m_nLevel(0)
    , m_nExp(0)
    , m_nDailyExpBonus(0)
    , m_bSpeakerFontEffe(false)
    , m_bCanKickRoomPlayer(false)
    , m_bExclusiveSelfInfoBoard(false)
    , m_bExclusiveRoomAction(false)
    , m_bEnterRoomSoundEffe(false)
    , m_nExpRateBonus(0)
    , m_nRoleNameColor(0)
    , m_nLevelUpAnnounce(0)
    , m_bVIPLevelUpReward(false)
    , m_strEnterRoomViewEffe("")
    , m_bOnlineAnnounce(false)
    , m_nOnlineAnnounceCoolDown(0)
    , m_nMaxFriendCount(0)
    , m_nMaxBlackListCount(0)
    , m_nConstellationMatchCount(0)
    , m_nClothGroupCount(0)
    , m_nDailyBindMBill(0)
    , m_nVipBuyVitCount(0)
    , m_nDanceGroupChallengeExtraChance( 0 )
    , m_nStorageCapacity(0)
    , m_nDanceGroupFairyLandExChance(0)
{
}

CVIPLevelPrivInfo::~CVIPLevelPrivInfo()
{
}

void CVIPLevelPrivInfo::Serialize(CParamPool & IOBuff, unsigned char nSex)
{
	IOBuff.AddUShort((unsigned short)m_nLevel);
	IOBuff.AddInt(m_nExp);
	IOBuff.AddBool(m_bSpeakerFontEffe);
	IOBuff.AddBool(m_bCanKickRoomPlayer);
	IOBuff.AddBool(m_bExclusiveSelfInfoBoard);
	IOBuff.AddBool(m_bExclusiveRoomAction);
	IOBuff.AddBool(m_bEnterRoomSoundEffe);
	IOBuff.AddUChar((unsigned char)m_nExpRateBonus);
	IOBuff.AddUChar((unsigned char)m_nRoleNameColor);
	IOBuff.AddStringW(m_strEnterRoomViewEffe.c_str());
	m_VIPLevelUpReward.Serialize(IOBuff, nSex);
	IOBuff.AddBool(m_bOnlineAnnounce);
	IOBuff.AddUShort((unsigned short)m_nMaxFriendCount);
	IOBuff.AddUShort((unsigned short)m_nMaxBlackListCount);
    IOBuff.AddUShort((unsigned short)m_nVipBuyVitCount);
    IOBuff.AddUShort((unsigned short)m_nDailyBindMBill);
    IOBuff.AddUShort((unsigned short)m_listDailyItemReward.size());
    std::list<CItem>::iterator it = m_listDailyItemReward.begin();
    for ( ; it != m_listDailyItemReward.end(); ++it )
    {
        it->doEncode(IOBuff);
    }

    IOBuff << m_nDanceGroupChallengeExtraChance;
    IOBuff << m_nDanceGroupFairyLandExChance;
    IOBuff << m_nDanceGroupFairyLandVipBuf;
}

