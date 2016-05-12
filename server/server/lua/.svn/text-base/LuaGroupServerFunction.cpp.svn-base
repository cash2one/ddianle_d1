#include "LuaCallback.h"
#include "../../datastructure/DataStruct_Game.h"
#include "../mail/MailGiftSystem.h"
#include "InitLua.h"
#include "../../datastructure/DanceGroupDataMgr.h"
#include "../Rank/LiveRoomRankManager.h"
#include "../../datastructure/FriendDataManager.h"
#include "../util/CommonFunc.h"
#include "../util/Split.h"
#include "../specialMallGrp/SpecialMallProcessor.h"
#include "../Rank/GlobalRankListManager.h"
#include "../group/GroupSystemSetting.h"
#include "../SystemSetting/SystemSettingMgr.h" // 
#include "../chatGrp/WChat.h"
#include "../dancegroup/DanceGroupChallengeProcessor.h"
#include "../dancegroup/BigMamaConfigManager.h"
#include "../activity/RedEnvelope/RedEnvelopeConfigManager.h"
#include "../dancegroup/BigMamaConfigManager.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../grade/GradeInfoMgr.h"
#include "../roomLib/RoomCheckDataMgr.h"
#include "../roomLib/RoomSettingMgr.h"
#include "../music/MusicBoardMgr.h"
#include "../music/BGMMgr.h"
#include "../vip/VIPStruct.h"
#include "../vip/VIPDataMgr.h"
#include "../DanceGroupFairyLand/DanceGroupFairyLandCfgMgr.h"

extern string g_sPath;

void CInitLua::RegLuaRoleFuction()
{
}

int lua_GetServerPath( lua_State * L )
{
	lua_pushstring( L , g_sPath.c_str() );
	return 1;
}
int lua_AddMailGiftInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 6 && lua_isnumber(L, 1) && lua_isstring(L, 2) && lua_isstring(L, 3) && lua_isnumber(L, 4) && lua_isstring(L, 5) && lua_isstring(L, 6));
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}
	int nGiftID = (int)lua_tonumber(L, 1);
	const char * szMaleReward = lua_tostring(L, 2);
	const char * szFemaleReward = lua_tostring(L, 3);
	unsigned int nRewardMoney = (unsigned int)lua_tonumber(L, 4);
	const char * szTitle = lua_tostring(L, 5);
	const char * szContent = lua_tostring(L, 6);

	CMailGiftInfo * pMailGiftInfo = new CMailGiftInfo();
	pMailGiftInfo->Init(nGiftID, szMaleReward, szFemaleReward, nRewardMoney, szTitle, szContent);

	CMailGiftSystem::Instance().AddMailGift(pMailGiftInfo);
	return 1;
}

int lua_AddGroupDanceLeveUpRequireResLua( lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 4 && 
					lua_isnumber(L, 1) && 
					lua_isnumber(L, 2) &&
					lua_isnumber(L, 3) && 
					lua_isnumber(L, 4) );
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}

	unsigned short nGroupLevel = (unsigned short)lua_tonumber(L, 1);
	unsigned int nResA = (unsigned int)lua_tonumber(L, 2);
	unsigned int nResB = (unsigned int)lua_tonumber(L, 3);
	unsigned int nResC = (unsigned int)lua_tonumber(L, 4);

	CDGLevelUpRequireRes *pInfo = new CDGLevelUpRequireRes;
	pInfo->m_nGroupLevel = nGroupLevel;
	pInfo->m_nResA = nResA;
	pInfo->m_nResB = nResB;
	pInfo->m_nResC = nResC;


	CDanceGroupDataMgr::Instance().AddDGLevelUpRequireResInfo(pInfo);

	return 1;
}

int lua_AddDanceGrupInfoLua(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 11 && 
		lua_isnumber(L, 1) &&
		lua_isnumber(L, 2) && 
		lua_isnumber(L, 3) && 
		lua_isnumber(L, 4) &&
		lua_isnumber(L, 5) &&
		lua_isnumber(L, 6) &&
		lua_isnumber(L, 7) &&
		lua_isnumber(L, 8) &&
		lua_isnumber(L, 9) &&
		lua_isnumber(L, 10) &&
		lua_isnumber(L, 11));
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}

	unsigned short nMaxDanceGroupLevel = (unsigned short)lua_tonumber(L, 1);
	int nMemberLoginGroupHonorBonusLevelNeed = (int)lua_tonumber(L, 2);
	int nMemberLoginGropHonorBonus = (int)lua_tonumber(L, 3);
	int nMinRoomDancersGroupHonorBonusNeed = (int)lua_tonumber(L, 4);
	int nMemberMatchBonusLeve1 = (int)lua_tonumber(L, 5);

	unsigned int nIncDGHonorNumPerDonatedResA = (unsigned int)lua_tonumber(L, 6);
	unsigned int nIncDGHonorNumPerDonatedResB = (unsigned int)lua_tonumber(L, 7);
	unsigned int nIncDGHonorNumPerDonatedResC = (unsigned int)lua_tonumber(L, 8);

	unsigned int nIncMemberContributionNumPerDonatedResA = (unsigned int)lua_tonumber(L, 9);
	unsigned int nIncMemberContributionNumPerDonatedResB = (unsigned int)lua_tonumber(L, 10);
	unsigned int nIncMemberContributionNumPerDonatedResC = (unsigned int)lua_tonumber(L, 11);

	CDanceGroupDataMgr::Instance().AddDanceGroupInfo(nMaxDanceGroupLevel, nMemberLoginGroupHonorBonusLevelNeed, nMemberLoginGropHonorBonus,
													 nMinRoomDancersGroupHonorBonusNeed, nMemberMatchBonusLeve1,
													 nIncDGHonorNumPerDonatedResA, nIncDGHonorNumPerDonatedResB, nIncDGHonorNumPerDonatedResC,
													 nIncMemberContributionNumPerDonatedResA, nIncMemberContributionNumPerDonatedResB, nIncMemberContributionNumPerDonatedResC);

	return 1;
}


int lua_SetCeremonyApplyInfo(lua_State *L)
{
    unsigned int freeApplyTimes = 0;
    unsigned int applyCostType = 0;
    unsigned int applyCostNum = 0;

    int nError = ParseLuaParam(L, freeApplyTimes, applyCostType, applyCostNum);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    CDanceGroupDataMgr::Instance().SetCeremonyApplyInfo((unsigned char)freeApplyTimes, (unsigned char)applyCostType, applyCostNum);
    return 1;
}

int lua_SetCeremonyPraiseInfo(lua_State *L)
{
    unsigned int freePraiseTimes = 0;
    unsigned int praiseCostType = 0;
    unsigned int praiseCostNum = 0;
    unsigned int praiseCD = 0;

    int nError = ParseLuaParam(L, freePraiseTimes, praiseCostType, praiseCostNum, praiseCD);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    CDanceGroupDataMgr::Instance().SetCeremonyPraiseInfo((unsigned char)freePraiseTimes, (unsigned char)praiseCostType, praiseCostNum, praiseCD);
    return 1;
}

int lua_SetCeremonyBaseInfo(lua_State *L)
{
    unsigned int applyDuration = 0;
    unsigned int startDuration = 0;
    unsigned int nEnterNewGroupCD = 0;
    unsigned int nEnterOldGroupCD = 0;

    int nError = ParseLuaParam(L, applyDuration, startDuration, nEnterNewGroupCD, nEnterOldGroupCD);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    CDanceGroupDataMgr::Instance().SetCeremonyBaseInfo(applyDuration, startDuration, nEnterNewGroupCD, nEnterOldGroupCD);

    return 1;
}

int lua_AddCeremonyNewManPrize(lua_State *L)
{
    CeremonyNewerPrize newPrize;
    int nError = ParseLuaParam(L, newPrize.m_strSender, newPrize.m_strMailTitle, newPrize.m_strContext, newPrize.m_strItem);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    CDanceGroupDataMgr::Instance().AddNewManPrize(newPrize);

    return 1;
}

int lua_AddCeremonyNormalPrize(lua_State *L)
{
    CeremonyNormalPrize prize;

    int nError = ParseLuaParam(L, prize.m_strSender, prize.m_strMailTitle, prize.m_strContext, prize.m_nMaxPraiser, prize.m_strItem);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    CDanceGroupDataMgr::Instance().AddNormalPrize(prize);

    return 1;
}

int lua_SetBigMamaConfig(lua_State *L)
{
    std::string strStartTime;
    std::string strEndTime;
    unsigned char nMaxRound = 0;
    unsigned int nXiaoLian = 0;
    unsigned int nXiaoRuan = 0;
    std::string strAnnouncement;
    unsigned int nAnnouncement = 5 * 60;
    std::string strIntroduce;

    int nError = ParseLuaParam(L, strStartTime, strEndTime, nMaxRound, nXiaoLian, nXiaoRuan, strAnnouncement, nAnnouncement, strIntroduce);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    unsigned int nBeginTime = 0;
    unsigned int nEndTime = 0;
    std::vector<int> t;
    SplitString(strStartTime, ":", t);
    if (t.size() == 3)
        nBeginTime = t[0] * 60 * 60 + t[1] * 60 + t[2];

    t.clear();
    SplitString(strEndTime, ":", t);
    if (t.size() == 3)
        nEndTime = t[0] * 60 * 60 + t[1] * 60 + t[2];

    if (nEndTime < nBeginTime)
        return 0; // lua 配置文件错误
    
    g_BigMamaConfigManager.SetBigMamaConfig(nBeginTime, nEndTime, nMaxRound, nXiaoLian, nXiaoRuan, strAnnouncement, nAnnouncement, strIntroduce);
    return 1;
}

int lua_AddBigMamaReward(lua_State *L)
{
    BigMamaReward reward;
    unsigned char rewardType;
    std::string maleReward;
    std::string femaleReward;

    int nError = ParseLuaParam(L, rewardType, maleReward, femaleReward, reward.m_nMoney, reward.m_nBindCoin, reward.m_nDanceGroupContribution);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    TransferItemList(maleReward.c_str(), reward.m_vecMaleItem);
    TransferItemList(femaleReward.c_str(), reward.m_vecFemaleItem);
    reward.m_rewardType = (EBigMamaRewardsType)rewardType;

    g_BigMamaConfigManager.AddBigMamaReward(reward);
    return 1;
}

// void AddBigMamaLuckData(const BigMamaLuckData &luckData);
int lua_AddBigMamaLuckData(lua_State *L)
{
    BigMamaLuckData luckData;

    int nError = ParseLuaParam(L, luckData.m_nNpcID, luckData.m_nBaseProb, luckData.m_nProbGrowRatio);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    g_BigMamaConfigManager.AddBigMamaLuckData(luckData);
    return 1;
}


int lua_SetBigMamaGuideReward(lua_State *L)
{
    std::string maleRewards;
    std::string femaleRewards;

    int nError = ParseLuaParam(L, maleRewards, femaleRewards);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    std::list<CItem> maleItemList;
    std::list<CItem> femaleItemList;
    TransferItemList(maleRewards.c_str(), maleItemList);
    TransferItemList(femaleRewards.c_str(), femaleItemList);
    g_BigMamaConfigManager.SetBigMamaGuideReward(maleItemList, femaleItemList);
    return 1;
}


int lua_SetLiveRoomGroupLua(lua_State *L)
{
	bool bValid = (
		lua_gettop( L ) == 6 
		&& lua_isstring(L, 1)
		&& lua_isstring(L, 2)
		&& lua_isnumber(L, 3)
		&& lua_isstring(L, 4)
		&& lua_isnumber(L, 5)
        && lua_isnumber(L, 6));
	if (!bValid)
	{
		PARAM_ERROR;
		return 0;
	}

	const char *pstrStartTime = lua_tostring(L, 1);
	const char *pstrEndTime = lua_tostring(L, 2);
	int nLeaderBoardSize = (int)lua_tonumber(L, 3);
	const char *pstrFirstUpdateLeaderBoardTime = lua_tostring(L, 4);
	int nUpdateLearderBoardInterval = (int)lua_tonumber(L, 5);
    int nAuditionBaseScore = (int)lua_tonumber(L, 6);

	CLiveRoomRankManager::Instance().SetLiveRoomConfig(
		pstrStartTime, pstrEndTime, nLeaderBoardSize, 
		pstrFirstUpdateLeaderBoardTime, nUpdateLearderBoardInterval,nAuditionBaseScore);

	return 1;
}

int lua_AddStarProgressLua( lua_State *L )
{
	bool bValid = ( lua_gettop(L ) == 6 
		&& lua_isnumber( L, 1 ) 
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L ,3 )
		&& lua_isnumber( L, 4 )
		&& lua_isnumber( L, 5 )
		&& lua_isstring( L , 6 )
		);
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int nStarLevel = (int)lua_tonumber( L, 1 );
	int nProgress = (int)lua_tonumber( L, 2 );
	unsigned int conLow = (unsigned int)lua_tonumber( L , 3 );
	unsigned int conMid = (unsigned int)lua_tonumber( L, 4 );
	unsigned int conHigh = (unsigned int)lua_tonumber( L, 5 );
	const char * name = lua_tostring( L , 6 );

	CDanceGroupDataMgr::Instance().SetStarProgress( nStarLevel, nProgress, conLow, conMid, conHigh, name );

	return 1;
}//end

int lua_SetFindFriendInfo(lua_State *L)
{
	bool bValid = ( lua_gettop( L ) == 2 
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 ));

	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int nCoolDown = (int)lua_tonumber( L, 1 );
	int nPlayerCount = (int)lua_tonumber( L, 2 );

	CFriendDataManager::Instance().SetFindFriend(nCoolDown, nPlayerCount);

	return 1;
}

int lua_AddFindFriendTypeInfo(lua_State *L)
{
	bool bValid = ( lua_gettop( L ) == 3 
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isnumber( L, 3 ));

	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int nIndex = (int)lua_tonumber( L, 1 );
	const char *strDesc = lua_tostring( L, 2 );
	int nType = (int)lua_tonumber( L, 3 );

	CFriendDataManager::Instance().AddFindFriendType(nIndex, strDesc, nType);

	return 1;
}


int lua_AddFindFriendLevelInfo(lua_State *L)
{
	bool bValid = ( lua_gettop( L ) == 4 
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isnumber( L, 4 ));

	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int nIndex = (int)lua_tonumber( L, 1 );
	const char *strDesc = lua_tostring( L, 2 );
	int nValueMin = (int)lua_tonumber( L, 3 );
	int nValueMax = (int)lua_tonumber( L, 4 );

	CFriendDataManager::Instance().AddFindFriendLevel(nIndex, strDesc, nValueMin, nValueMax);

	return 1;
}

int lua_AddFindFriendSexInfo(lua_State *L)
{
	bool bValid = ( lua_gettop( L ) == 3 
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isnumber( L, 3 ));

	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int nIndex = (int)lua_tonumber( L, 1 );
	const char *strDesc = lua_tostring( L, 2 );
	int nValueMin = (int)lua_tonumber( L, 3 );

	CFriendDataManager::Instance().AddFindFriendSex(nIndex, strDesc, nValueMin);

	return 1;
}

int lua_AddFindFriendAgeInfo(lua_State *L)
{
	bool bValid = ( lua_gettop( L ) == 4 
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isnumber( L, 4 ));

	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int nIndex = (int)lua_tonumber( L, 1 );
	const char *strDesc = lua_tostring( L, 2 );
	int nValueMin = (int)lua_tonumber( L, 3 );
	int nValueMax = (int)lua_tonumber( L, 4 );

	CFriendDataManager::Instance().AddFindFriendAge(nIndex, strDesc, nValueMin, nValueMax);

	return 1;
}

int lua_AddFindFriendConstellationInfo(lua_State *L)
{
	bool bValid = ( lua_gettop( L ) == 3 
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isnumber( L, 3 ));

	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int nIndex = (int)lua_tonumber( L, 1 );
	const char *strDesc = lua_tostring( L, 2 );
	int nValueMin = (int)lua_tonumber( L, 3 );

	CFriendDataManager::Instance().AddFindFriendConstellation(nIndex, strDesc, nValueMin);

	return 1;
}

int lua_AddStarPacketItem( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 4 
		&& lua_isnumber( L, 1 )&& lua_isnumber( L, 2 )
		&& lua_isstring( L ,3 ) && lua_isstring( L, 4 )
		);
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	unsigned short nDay = (unsigned short)lua_tonumber( L, 1 );
	unsigned short nIndex = (unsigned short)lua_tonumber( L, 2 );
	const char* szMale = lua_tostring( L, 3 );
	const char* szFemale = lua_tostring( L, 4 );

	CDanceGroupDataMgr::Instance().AddStarPacket( nDay, nIndex, szMale, szFemale );

	return 1;
}//end

int lua_AddStarBaseInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 14 
        && lua_isstring( L, i++ )   // male day reward
        && lua_isstring( L, i++ )   // female day reward
        && lua_isnumber( L, i++ )   // normal count
        && lua_isnumber( L, i++ )   // normal power
        && lua_isnumber( L, i++ )   // normal contribution
        && lua_isnumber( L, i++ )   // money count
        && lua_isnumber( L, i++ )   // money num
        && lua_isnumber( L, i++ )   // money power
        && lua_isnumber( L, i++ )   // money contribution
        && lua_isnumber( L, i++ )   // point count
        && lua_isnumber( L, i++ )   // point num
        && lua_isnumber( L, i++ )   // point power
        && lua_isnumber( L, i++ )   // point contribution
        && lua_isstring( L, i++ )   // rule content
        );

    if ( !bValid )
    {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    string strMaleDayReward = lua_tostring( L, i++ );
    string strFemaleDayReward = lua_tostring( L, i++ );
    unsigned short nNormalCount = (unsigned short)lua_tonumber( L, i++ );
    int nNormalPower = (int)lua_tonumber( L, i++ );
    int nNormalContribution = (int)lua_tonumber( L, i++ );
    unsigned short nMoneyCount = (unsigned short)lua_tonumber( L, i++ );
    int nMoneyNum = (int)lua_tonumber( L, i++ );
    int nMoneyPower = (int)lua_tonumber( L, i++ );
    int nMoneyContribution = (int)lua_tonumber( L, i++ );
    unsigned short nPointCount = (unsigned short)lua_tonumber( L, i++ );
    int nPointNum = (int)lua_tonumber( L, i++ );
    int nPointPower = (int)lua_tonumber( L, i++ );
    int nPointContribution = (int)lua_tonumber( L, i++ );
    string strRuleContent = lua_tostring( L, i++ );

    CDanceGroupDataMgr::Instance().SetBaseInfo(strMaleDayReward.c_str(), strFemaleDayReward.c_str(), 
        nNormalCount, nNormalPower, nNormalContribution, 
        nMoneyCount, nMoneyNum, nMoneyPower, nMoneyContribution, 
        nPointCount, nPointNum, nPointPower, nPointContribution, 
        strRuleContent);

    return 1;
}

int lua_AddGroupBuyingInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 4 
        && lua_isstring( L, i++ )   // mail title
        && lua_isstring( L, i++ )   // mail content
        && lua_isnumber( L, i++ )   // add lower
        && lua_isnumber( L, i++ )   // add upper
        );

    if ( !bValid )
    {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    string mailTitle = lua_tostring( L, i++ );
    string mailContent = lua_tostring( L, i++ );
    unsigned int nAdditionLower = (unsigned int)lua_tonumber( L, i++ );
    unsigned int nAdditionUpper = (unsigned int)lua_tonumber( L, i++ );

    CLimitedSalesSettings settings;
    settings.m_strMailTitle = mailTitle;
    settings.m_strMailContent = mailContent;
    settings.m_uAdditionLower = nAdditionLower;
    settings.m_uAdditionUpper = nAdditionUpper;

    CSpecialMallProcessor::Instance().InitLuaInfo(ESpecialMallType_GroupBuying, &settings);

    return 1;
}

int lua_AddGroupBuyingTimeRangeInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 3 
        && lua_isstring( L, i++ )   // begin time
        && lua_isstring( L, i++ )   // end time
        && lua_isstring( L, i++ )   // title
        );

    if ( !bValid ) {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    string strBegin = lua_tostring( L, i++ );
    string strEnd = lua_tostring( L, i++ );
    string strTitle = lua_tostring( L, i++ );

    time_t begin = ::SetTime(strBegin.c_str());
    time_t end = ::SetTime(strEnd.c_str());

    CSpecialMallProcessor::Instance().AddTimeRangeInfo(ESpecialMallType_GroupBuying, begin, end, strTitle);

    return 1;
}

int lua_AddLimitedEditionInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 4 
        && lua_isstring( L, i++ )   // mail title
        && lua_isstring( L, i++ )   // mail content
        && lua_isnumber( L, i++ )   // add lower
        && lua_isnumber( L, i++ )   // add upper
        );

    if ( !bValid )
    {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    string mailTitle = lua_tostring( L, i++ );
    string mailContent = lua_tostring( L, i++ );
    unsigned int nAdditionLower = (unsigned int)lua_tonumber( L, i++ );
    unsigned int nAdditionUpper = (unsigned int)lua_tonumber( L, i++ );

    CLimitedSalesSettings settings;
    settings.m_strMailTitle = mailTitle;
    settings.m_strMailContent = mailContent;
    settings.m_uAdditionLower = nAdditionLower;
    settings.m_uAdditionUpper = nAdditionUpper;

    CSpecialMallProcessor::Instance().InitLuaInfo(ESpecialMallType_LimitedEdition, &settings);

    return 1;
}

int lua_AddLimitedEditionTimeRangeInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 3 
        && lua_isstring( L, i++ )   // begin time
        && lua_isstring( L, i++ )   // end time
        && lua_isstring( L, i++ )   // title
        );

    if ( !bValid ) {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    string strBegin = lua_tostring( L, i++ );
    string strEnd = lua_tostring( L, i++ );
    string strTitle = lua_tostring( L, i++ );

    time_t begin = ::SetTime(strBegin.c_str());
    time_t end = ::SetTime(strEnd.c_str());

    CSpecialMallProcessor::Instance().AddTimeRangeInfo(ESpecialMallType_LimitedEdition, begin, end, strTitle);

    return 1;
}

int lua_AddAuctionInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 3 
        && lua_isstring( L, i++ )   // mail title
        && lua_isstring( L, i++ )   // mail content
        && lua_isstring( L, i++ )   // rule
        );

    if ( !bValid )
    {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    string mailTitle = lua_tostring( L, i++ );
    string mailContent = lua_tostring( L, i++ );
    string rule = lua_tostring( L, i++ );

    CAuctionSettings settings;
    settings.m_strMailTitle = mailTitle;
    settings.m_strMailContent = mailContent;
    settings.m_strRule = rule;

    CSpecialMallProcessor::Instance().InitLuaInfo(ESpecialMallType_Auction, &settings);

    return 1;
}

int lua_AddAuctionTimeRangeInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 3 
        && lua_isstring( L, i++ )   // begin time
        && lua_isstring( L, i++ )   // end time
        && lua_isstring( L, i++ )   // title
        );

    if ( !bValid )
    {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    string strBegin = lua_tostring( L, i++ );
    string strEnd = lua_tostring( L, i++ );
    string strTitle = lua_tostring( L, i++ );

    time_t begin = ::SetTime(strBegin.c_str());
    time_t end = ::SetTime(strEnd.c_str());

    CSpecialMallProcessor::Instance().AddTimeRangeInfo(ESpecialMallType_Auction, begin, end, strTitle);

    return 1;
}

int lua_LoadRankStartTimeInfo( lua_State * L )
{
    bool bValid = ( lua_gettop( L ) == 2 
        && lua_isnumber( L, 1 ) 
        && lua_isnumber( L, 2 )
        );

    if ( !bValid )
    {
        PARAM_ERROR;

        return 0;
    }

    unsigned int nStartWeekDay = (unsigned int)lua_tonumber( L, 1 );
    unsigned int nStartHour = (unsigned int)lua_tonumber( L, 2 );

    CGlobalRankListManager::Instance().LoadStartTime(nStartWeekDay, nStartHour);

    return 1;
}

int lua_LoadRankMaxOfflineTime( lua_State * L )
{
    int nMaxOfflineTime = 0;

    int nError = ParseLuaParam( L, nMaxOfflineTime );
    if ( nError != EParseLuaParam_Success )
    {
        PARAM_ERROR;
        return 0;
    }

    CGlobalRankListManager::Instance().LoadMaxOfflineTime( nMaxOfflineTime * 24 * 3600 );

    return 1;
}

int lua_LoadRankMailInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 3 
        && lua_isstring( L, i++ )   // mail sender
        && lua_isstring( L, i++ )   // mail title
        && lua_isstring( L, i++ )   // mail content
        );

    if ( !bValid )
    {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    string strSender = lua_tostring( L, i++ );
    string strTitle = lua_tostring( L, i++ );
    string strContent = lua_tostring( L, i++ );

    CGlobalRankListManager::Instance().SetMailInfo( strSender, strTitle, strContent );

    return 1;
}

int lua_AddRankHeaderNameInfo( lua_State * L )
{
    int i = 1;
    int count = 5;
    bool bValid = ( lua_gettop( L ) == count 
        && lua_isnumber( L, i++ )   // index
        && lua_isstring( L, i++ )   // name1
        && lua_isstring( L, i++ )   // name2
        && lua_isstring( L, i++ )   // name3
        && lua_isstring( L, i++ )   // name4
        );

    if ( !bValid )
    {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    list<string> listHeaderName;
    unsigned short nHeadIndex = (unsigned short)lua_tonumber( L, i++ );

    for (; i <= count; ++i)
    {
        listHeaderName.push_back(lua_tostring(L, i));
    }

    CGlobalRankListManager::Instance().AddHeaderName( nHeadIndex, listHeaderName );

    return 1;
}

int lua_AddRankHallInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 5 
        && lua_isnumber( L, i++ )   // index
        && lua_isnumber( L, i++ )   // type
        && lua_isnumber( L, i++ )   // header
        && lua_isstring( L, i++ )   // title
        && lua_isnumber( L, i++ )   // max count
        );

    if ( !bValid )
    {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    unsigned short nIndex = (unsigned short)lua_tonumber( L, i++ );
    unsigned short nType = (unsigned short)lua_tonumber( L, i++ );
    unsigned short nHeaderIndex = (unsigned short)lua_tonumber( L, i++ );
    string strTitle = lua_tostring( L, i++ );
    unsigned int nMaxCount = (unsigned int)lua_tonumber( L, i++ );

    CGlobalRankListManager::Instance().AddRankHall( nIndex, nType, nHeaderIndex, strTitle, nMaxCount );

    return 1;
}

int lua_AddRankRewardInfo( lua_State * L )
{
    unsigned short nType = 0, nSecond = 0;
    string strRewards;
    int money = 0, bindmcoin = 0;

    int nError = ParseLuaParam( L, nType, nSecond, strRewards, money, bindmcoin );
    if ( nError != EParseLuaParam_Success )
    {
        PARAM_ERROR;
        return 0;
    }

    std::list<CItem> listItems;
    TransferItemList( strRewards.c_str(), listItems );

    CGlobalRankListManager::Instance().AddRankReward( nType, nSecond, listItems, money, bindmcoin );

    return 1;
}

int lua_AddFunctionSwitch( lua_State * L )
{
    bool bValid = ( lua_gettop( L ) == 3 
        && lua_isnumber( L, 1 ) 
        && lua_isstring( L, 2 )
        && lua_isboolean( L, 3 )
        );

    if ( !bValid )
    {
        PARAM_ERROR;

        return 0;
    }

    unsigned short index = (unsigned short)lua_tonumber( L, 1 );
    string fName = lua_tostring( L, 2 );
    bool bOpen = (lua_toboolean( L, 3 ) != 0 ? true : false);

    CGroupSystemSetting::Instance().SetFunctionSwitch(index, bOpen);

    return 1;
}

int lua_SetAutoChatSendInterval( lua_State * L )
{
    unsigned int nSendInterval = 0;

    int nError = ParseLuaParam(L, nSendInterval);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    CWChatMgr::Instance().SetAutoChatSendInterval(nSendInterval);

    return 1;
}

int lua_LoadDanceGroupChallengeInfos( lua_State * L )
{
    unsigned int nInspirePercentForAll = 0, nInspirePercentForSelf = 0;
    unsigned int nInspireMaxCountForAll = 0, nInspireMaxCountForSelf = 0;
    int nInspireRewardGroupContribution = 0;
    int nResetWeekday = 0, nResetHour = 0;

    int nError = ParseLuaParam( L, 
        nInspirePercentForAll, nInspirePercentForSelf, 
        nInspireMaxCountForAll, nInspireMaxCountForSelf, 
        nInspireRewardGroupContribution, 
        nResetWeekday, nResetHour );

    if ( nError != EParseLuaParam_Success )
    {
        PARAM_ERROR
        return 0;
    }

    DanceGroupChallengeProcessor& processor = DanceGroupChallengeProcessor::Instance();
    processor.SetInspireInfos( nInspirePercentForAll, nInspirePercentForSelf, 
        nInspireMaxCountForAll, nInspireMaxCountForSelf, nInspireRewardGroupContribution );
    processor.SetResetInfos( nResetWeekday, nResetHour );

    return 1;
}

int lua_AddEnvelopeConfig( lua_State * L )
{
    RedEnvelopeConfig config;
    std::string strBeginTime;
    std::string strEndTime;
    std::string strShowBeginTime;
    std::string strShowEndTime;
    int nExhibit = 0;
    int nError = ParseLuaParam(L, nExhibit, config.m_nWeight, config.m_strRegularImageName, config.m_strThumbnailName, 
        config.m_nRedEnvelopeNum, config.m_nRedEnvelopeContinueTime, config.m_nRedEnvelopeGetAllDisappearTime,strBeginTime,
        strEndTime, strShowBeginTime, strShowEndTime, config.m_strTitle, config.m_strContent, config.m_strAnonymityName);
    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    config.m_bExhibit = nExhibit > 0 ? true : false;
    config.m_nActivityBeginTime = (unsigned int)SetTime(strBeginTime.c_str());
    config.m_nActivityEndTime = (unsigned int)SetTime(strEndTime.c_str());
    config.m_nShowBeginTime = (unsigned int)SetTime(strShowBeginTime.c_str());
    config.m_nShowEndTime = (unsigned int)SetTime(strShowEndTime.c_str());
    RedEnvelopeConfigManager::Instance().AddRedEnvelopeConfig(config);

    return 1;
}

int lua_AddRedEnvelopeRechargeConfig( lua_State * L )
{
    RedEnvelopeRechareConfig config;
    int nError = ParseLuaParam(L, config.m_nRechargeMCoin, config.m_nCurrencyType, config.m_nCurrencyValue, config.m_nMinRate, config.m_nFluctuateRate);
    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;
        return 0;
    }

    RedEnvelopeConfigManager::Instance().AddRedEnvelopeRechargeConfig(config);

    return 1;
}


int lua_AddGradePercent(lua_State * L)
{
    if (lua_gettop(L) == 5
        && lua_isnumber(L, 1)
        && lua_isnumber(L, 2)
        && lua_isnumber(L, 3)
        && lua_isnumber(L, 4)
        && lua_isnumber(L, 5))
    {
        CGradeInfoManager::Instance().AddGradePercentInfo(
            (int)lua_tonumber(L, 1),
            (int)lua_tonumber(L, 2),
            (int)lua_tonumber(L, 3),
            (int)lua_tonumber(L, 4),
            (int)lua_tonumber(L, 5));
    }
    else
    {
        PARAM_ERROR;
    }

    return 0;
}

int lua_AddGradeExp(lua_State * L)
{
    if (lua_gettop(L) == 7
        && lua_isnumber(L, 1)
        && lua_isnumber(L, 2)
        && lua_isnumber(L, 3)
        && lua_isnumber(L, 4)
        && lua_isnumber(L, 5)
        && lua_isnumber(L, 6)
        && lua_isnumber(L, 7))
    {
        CGradeInfoManager::Instance().AddGradeExpInfo(
            (int)lua_tonumber(L, 1),
            (int)lua_tonumber(L, 2),
            (int)lua_tonumber(L, 3),
            (int)lua_tonumber(L, 4),
            (int)lua_tonumber(L, 5),
            (int)lua_tonumber(L, 6),
            (int)lua_tonumber(L, 7));
    }
    else
    {
        PARAM_ERROR;
    }

    return 0;
}

int lua_AddPlaceExp(lua_State * L)
{
    if (lua_gettop(L) == 5
        && lua_isnumber(L, 1)
        && lua_isnumber(L, 2)
        && lua_isnumber(L, 3)
        && lua_isnumber(L, 4)
        && lua_isnumber(L, 5))
    {
        CGradeInfoManager::Instance().AddPlaceExpInfo(
            (int)lua_tonumber(L, 1),
            (int)lua_tonumber(L, 2),
            (int)lua_tonumber(L, 3),
            (int)lua_tonumber(L, 4),
            (int)lua_tonumber(L, 5));
    }
    else
    {
        PARAM_ERROR;
    }

    return 0;
}

int lua_AddRoomCheck(lua_State * L)
{
    bool bValid = (lua_gettop(L) == 4
        && lua_isnumber(L, 1)
        && lua_isnumber(L, 2)
        && lua_isnumber(L, 3)
        && lua_isnumber(L, 4)
        );

    if (!bValid) {
        PARAM_ERROR;
        return 0;
    }

    int nCheckMode = (int)lua_tonumber(L, 1);
    int nCheckTime = (int)lua_tonumber(L, 2);
    int nCheckNoMiss = (int)lua_tonumber(L, 3);
    int nCheckMark = (int)lua_tonumber(L, 4);

    CRoomCheckDataMgr::Instance().AddRoomCheckInfo(nCheckMode, nCheckTime, nCheckNoMiss, nCheckMark);

    return 1;
}


int lua_AddMusicBoard(lua_State * L)
{
    if (lua_gettop(L) == 3
        && lua_isnumber(L, 1)
        && lua_isboolean(L, 2)
        && lua_isboolean(L, 3))
    {
        CMusicBoardManager::Instance().AddBoardInfo(
            (unsigned short)lua_tonumber(L, 1),
            lua_toboolean(L, 2) != 0,
            lua_toboolean(L, 3) != 0);
    }
    else
    {
        PARAM_ERROR;
    }

    return 0;
}

int lua_AddFresherSong(lua_State * L)
{
    if (lua_gettop(L) == 2
        && lua_isnumber(L, 1)
        && lua_isnumber(L, 2))
    {
        CMusicBoardManager::Instance().AddFresherSongInfo(
            (unsigned char)lua_tonumber(L, 1),
            (int)lua_tonumber(L, 2));
    }
    else
    {
        PARAM_ERROR;
    }

    return 0;
}


int lua_AddBGMusic(lua_State *L)
{
    bool bValid = (lua_gettop(L) == 1 && lua_isnumber(L, 1));
    if (!bValid) {
        PARAM_ERROR;
        return 0;
    }

    unsigned short nMusicID = (unsigned short)lua_tonumber(L, 1);
    CBGMMgr::instance().AddMusic(nMusicID);

    return 1;
}


int lua_RoomKickSet(lua_State *L)
{
    bool bValid = (lua_gettop(L) == 2
        && lua_isnumber(L, 1)
        && lua_isnumber(L, 2)
        );

    if (!bValid) {
        PARAM_ERROR;
        return 0;
    }

    int nRoomStartWait = (int)lua_tonumber(L, 1);
    int nRoomEndWait = (int)lua_tonumber(L, 2);

    RoomSettingMgr::Instance().RoomKickSetInfo(nRoomStartWait, nRoomEndWait);

    return 1;
}

int lua_RoomMatchSet(lua_State *L)
{
    bool bValid = (lua_gettop(L) == 2
        && lua_isnumber(L, 1)
        && lua_isnumber(L, 2)
        );

    if (!bValid) {
        PARAM_ERROR;
        return 0;
    }

    int nAutoMatchCount = (int)lua_tonumber(L, 1);
    int nAutoMatchInterval = (int)lua_tonumber(L, 2);

    RoomSettingMgr::Instance().RoomMatchSetInfo(nAutoMatchCount, nAutoMatchInterval);

    return 1;
}

int lua_RoomMusicSet(lua_State *L)
{
    bool bValid = (lua_gettop(L) == 3
        && lua_isnumber(L, 1)
        && lua_isnumber(L, 2)
        && lua_isnumber(L, 3)
        );

    if (!bValid) {
        PARAM_ERROR;
        return 0;
    }

    int nDefalutScene = (int)lua_tonumber(L, 1);
    int nDefaultMode = (int)lua_tonumber(L, 2);
    int nDefaultLevel = (int)lua_tonumber(L, 3);

    RoomSettingMgr::Instance().RoomMusicSetInfo(nDefalutScene, nDefaultMode, nDefaultLevel);

    return 1;
}

int lua_LuaAddVIPInfo(lua_State *L)
{
    bool bValid = (lua_gettop(L) == 3
        && lua_isnumber(L, 1)
        && lua_isnumber(L, 2)
        && lua_isnumber(L, 3)
        );

    if (!bValid) {
        PARAM_ERROR;
        return 0;
    }

    int nMaxLevel = (int)lua_tonumber(L, 1);
    int nConsumePtExpBouns = (int)lua_tonumber(L, 2);
    int nSendAddExpMaxBill = (int)lua_tonumber(L, 3);
    CVIPDataMgr::Instance().AddVIPInfo(nMaxLevel, nConsumePtExpBouns, nSendAddExpMaxBill);

    return 1;
}

int lua_LuaAddVIPPrivInfo(lua_State *L)
{
    bool bValid = (lua_gettop(L) == 28
        && lua_isnumber(L, 1)
        && lua_isnumber(L, 2)
        && lua_isnumber(L, 3)
        && lua_isboolean(L, 4)
        && lua_isboolean(L, 5)
        && lua_isboolean(L, 6)
        && lua_isboolean(L, 7)
        && lua_isboolean(L, 8)
        && lua_isnumber(L, 9)
        && lua_isnumber(L, 10)
        && lua_isnumber(L, 11)
        && lua_isstring(L, 12)
        && lua_isstring(L, 13)
        && lua_isstring(L, 14)
        && lua_isnumber(L, 15)
        && lua_isboolean(L, 16)
        && lua_isnumber(L, 17)
        && lua_isnumber(L, 18)
        && lua_isnumber(L, 19)
        && lua_isnumber(L, 20)
        && lua_isnumber(L, 21)
        && lua_isnumber(L, 22)
        && lua_isstring(L, 23)
        && lua_isstring(L, 24)
        && lua_isnumber(L, 25)
        && lua_isnumber(L, 26)
        && lua_isnumber(L, 27)
        && lua_isnumber(L, 28)
        );

    if (!bValid) {
        PARAM_ERROR;
        return 0;
    }

    int nLevel = (int)lua_tonumber(L, 1);
    int nExp = (int)lua_tonumber(L, 2);
    int nDailyExpBonus = (int)lua_tonumber(L, 3);
    bool bSpeakerFontEffe = lua_toboolean(L, 4) != 0;
    bool bCanKickRoomPlayer = lua_toboolean(L, 5) != 0;
    bool bExclusiveSelfInfoBoard = lua_toboolean(L, 6) != 0;
    bool bExclusiveRoomAction = lua_toboolean(L, 7) != 0;
    bool bEnterRoomSoundEffe = lua_toboolean(L, 8) != 0;
    int nExpRateBounus = int(lua_tonumber(L, 9));
    int nRoleNameColor = int(lua_tonumber(L, 10));
    int nLevelUpAnnounce = int(lua_tonumber(L, 11));
    std::string strEnterRoomViewEffe = lua_tostring(L, 12);
    std::string strMaleReward = lua_tostring(L, 13);
    std::string strFemaleReward = lua_tostring(L, 14);
    int nMoney = int(lua_tonumber(L, 15));
    bool bOnlineAnnounce = lua_toboolean(L, 16) != 0;
    int nOnlineAnnounceCoolDown = int(lua_tonumber(L, 17));
    int nMaxFriendCount = int(lua_tonumber(L, 18));
    int nMaxBlackListCount = int(lua_tonumber(L, 19));
    int nConstellationMatchCount = int(lua_tonumber(L, 20));
    int nClothGroupCount = int(lua_tonumber(L, 21));
    int nDailyBindMBill = int(lua_tonumber(L, 22));


    std::list<CItem> listDailyReward;
    std::string strEveryDayItemReward = lua_tostring(L, 23);
    TransferItemList(strEveryDayItemReward.c_str(), listDailyReward);

    std::list<CItem> listMaleItem;
    if (!strMaleReward.empty()) {
        ::TransferItemList(strMaleReward.c_str(), listMaleItem);
    }

    std::list<CItem> listFemaleItem;
    if (!strFemaleReward.empty()) {
        ::TransferItemList(strFemaleReward.c_str(), listFemaleItem);
    }

    int nMagicBuyVitNum = int(lua_tonumber(L, 24));
    unsigned short nDanceGroupChallengeChance = (unsigned short)lua_tonumber(L, 25);
    int nStorageCapacity = int(lua_tonumber(L, 26));
    unsigned int nDanceGroupFairyLandExChance = (unsigned int)lua_tonumber(L, 27);
    unsigned int nDanceGroupFairyLandVipBuf = (unsigned int)lua_tonumber(L, 28);
    CVIPLevelUpReward levelUpReward;
    levelUpReward.m_nLevel = nLevel;
    levelUpReward.m_listMaleItem = listMaleItem;
    levelUpReward.m_listFemaleItem = listFemaleItem;
    levelUpReward.m_nMoney = nMoney;

    CVIPLevelPrivInfo *pVIPPrivInfo = new CVIPLevelPrivInfo;
    pVIPPrivInfo->m_nLevel = nLevel;
    pVIPPrivInfo->m_nExp = nExp;
    pVIPPrivInfo->m_nDailyExpBonus = nDailyExpBonus;
    pVIPPrivInfo->m_bSpeakerFontEffe = bSpeakerFontEffe;
    pVIPPrivInfo->m_bCanKickRoomPlayer = bCanKickRoomPlayer;
    pVIPPrivInfo->m_bExclusiveSelfInfoBoard = bExclusiveSelfInfoBoard;
    pVIPPrivInfo->m_bExclusiveRoomAction = bExclusiveRoomAction;
    pVIPPrivInfo->m_bEnterRoomSoundEffe = bEnterRoomSoundEffe;
    pVIPPrivInfo->m_nExpRateBonus = nExpRateBounus;
    pVIPPrivInfo->m_nRoleNameColor = nRoleNameColor;
    pVIPPrivInfo->m_nLevelUpAnnounce = nLevelUpAnnounce;
    pVIPPrivInfo->m_strEnterRoomViewEffe = strEnterRoomViewEffe;
    pVIPPrivInfo->m_VIPLevelUpReward = levelUpReward;
    pVIPPrivInfo->m_bOnlineAnnounce = bOnlineAnnounce;
    pVIPPrivInfo->m_nOnlineAnnounceCoolDown = nOnlineAnnounceCoolDown;
    pVIPPrivInfo->m_nMaxFriendCount = nMaxFriendCount;
    pVIPPrivInfo->m_nMaxBlackListCount = nMaxBlackListCount;
    pVIPPrivInfo->m_nConstellationMatchCount = nConstellationMatchCount;
    pVIPPrivInfo->m_nClothGroupCount = nClothGroupCount;
    pVIPPrivInfo->m_nDailyBindMBill = nDailyBindMBill;
    pVIPPrivInfo->m_listDailyItemReward = listDailyReward;
    pVIPPrivInfo->m_nVipBuyVitCount = nMagicBuyVitNum;
    pVIPPrivInfo->m_nDanceGroupChallengeExtraChance = nDanceGroupChallengeChance;
    pVIPPrivInfo->m_nStorageCapacity = nStorageCapacity;
    pVIPPrivInfo->m_nDanceGroupFairyLandExChance = nDanceGroupFairyLandExChance;
    pVIPPrivInfo->m_nDanceGroupFairyLandVipBuf = nDanceGroupFairyLandVipBuf;

    CVIPExpInfo *pExp = new CVIPExpInfo;
    pExp->m_nCurLevel = nLevel - 1;
    pExp->m_nExpLevepUpNeed = nExp;
    CVIPDataMgr::Instance().AddVIPExpInfo(pExp);
    CVIPDataMgr::Instance().AddVIPPrivInfo(pVIPPrivInfo);

    return 1;
}

int lua_LoadFairlyLandConfig(lua_State *L)
{
    std::string strMailSender;
    std::string strMailTitle;
    std::string strMailContext;
    std::string strStorageMailContext;
    int nStorageBoxRewardCycleWeek = 0;
    std::string strStorageBoxSendTime;
    std::string strEndAnnounceMentTime;
    std::string strEndAnnounceMentContext;
    std::string strStartTime;
    std::string strEndTime;
    std::string strAllowDanceBeginTime;
    std::string strAllowDanceEndTime;
    std::string strRewardSendTime;
    int nBlessBuff;
    std::string strRankRule;
    std::string strStorageBoxRule;
    std::string strBlessRule;
    int nBuffAddTime;
    int nBuffPrice;

    int nError = ParseLuaParam(L, strMailSender, strMailTitle, strMailContext, strStorageMailContext, nStorageBoxRewardCycleWeek,
        strStorageBoxSendTime, strEndAnnounceMentTime, strEndAnnounceMentContext, strStartTime, strEndTime, strAllowDanceBeginTime,
        strAllowDanceEndTime, strRewardSendTime, nBlessBuff, strRankRule, strStorageBoxRule, strBlessRule, nBuffAddTime, nBuffPrice);
    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;
        return 0;
    }

    CDanceGroupFairyLandCfgMgr::Instance().LoadCfgFromLua(strMailSender, strMailTitle, strMailContext, strStorageMailContext, nStorageBoxRewardCycleWeek,
        strStorageBoxSendTime, strEndAnnounceMentTime, strEndAnnounceMentContext, strStartTime, strEndTime, strAllowDanceBeginTime,
        strAllowDanceEndTime, strRewardSendTime, nBlessBuff, strRankRule, strStorageBoxRule, strBlessRule, nBuffAddTime, nBuffPrice);

    return 1;
}

void CInitLua::RegLuaServerConfigFuction()
{
	REGFN(GetServerPath);
	REGFN(AddMailGiftInfo);
	REGFN(AddGroupDanceLeveUpRequireResLua);
    REGFN(AddDanceGrupInfoLua);
    REGFN(SetCeremonyApplyInfo);
    REGFN(SetCeremonyPraiseInfo);
    REGFN(SetCeremonyBaseInfo);
    REGFN(AddCeremonyNewManPrize);
    REGFN(AddCeremonyNormalPrize);
    REGFN(SetBigMamaConfig);
    REGFN(AddBigMamaReward);
    REGFN(AddBigMamaLuckData);
    REGFN(SetBigMamaGuideReward);

	REGFN(SetLiveRoomGroupLua);
	REGFN( AddStarProgressLua );
	REGFN( AddStarPacketItem );//礼包信息
    REGFN(AddStarBaseInfo);
	REGFN(SetFindFriendInfo);
	REGFN(AddFindFriendTypeInfo);
	REGFN(AddFindFriendLevelInfo);
	REGFN(AddFindFriendSexInfo);
	REGFN(AddFindFriendAgeInfo);
	REGFN(AddFindFriendConstellationInfo);

    //特殊商店
    REGFN(AddGroupBuyingInfo);
    REGFN(AddGroupBuyingTimeRangeInfo);
    REGFN(AddLimitedEditionInfo);
    REGFN(AddLimitedEditionTimeRangeInfo);
    REGFN(AddAuctionInfo);
    REGFN(AddAuctionTimeRangeInfo);

    // 排行榜
    REGFN(LoadRankStartTimeInfo);
    REGFN( LoadRankMaxOfflineTime );
    REGFN( LoadRankMailInfo );
    REGFN(AddRankHeaderNameInfo);
    REGFN(AddRankHallInfo);
    REGFN( AddRankRewardInfo );

    // 系统开关
    REGFN(AddFunctionSwitch);

    // 聊天
    REGFN(SetAutoChatSendInterval);

    // 舞团试炼
    REGFN(LoadDanceGroupChallengeInfos);

    // 舞团红包
    REGFN(AddEnvelopeConfig);
    REGFN(AddRedEnvelopeRechargeConfig);

    REGFN(AddGradePercent);
    REGFN(AddGradeExp);
    REGFN(AddPlaceExp);
    REGFN(AddRoomCheck);
    REGFN(AddMusicBoard);
    REGFN(AddFresherSong);
    REGFN(RoomKickSet);
    REGFN(RoomMatchSet);
    REGFN(RoomMusicSet);
    REGFN(AddBGMusic);
    // Vip
    REGFN(LuaAddVIPInfo);
	REGFN(LuaAddVIPPrivInfo);

    // 舞团秘境
    REGFN(LoadFairlyLandConfig);
}



