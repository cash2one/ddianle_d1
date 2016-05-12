#include "../logic/LogicCirculator.h"
#include "../../datastructure/DataStruct_Game.h"
#include "../music/MusicBoardMgr.h"
#include "../grade/GradeInfoMgr.h"
#include "InitLua.h"
#include "../logic/CommandProcessor.h"
#include "../friend/RelationInfo.h"
#include "../payment/PaymentInfoMgr.h"
#include "../announcement/AnnouncementMgr.h"
#include "../music/BGMMgr.h"
#include "../CheckIn/CheckInfoMgr.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../Money/MoneyMgr.h"
#include "../activity/SystemActivityManager.h"
#include "../activity/InTimeOnlineActivity.h"
#include "../activity/LevelUpRewardInfoMgr.h"
#include "../activity/OnlineRewardInfoMgr.h"
#include "../util/CommonFunc.h"
#include "../../datastructure/DataStruct_Game.h"
#include "../activity/ExtraExpActivity.h"
#include "../activity/CumulativeRechargeMgr.h"
#include "../activity/ExchangeItemInfoMgr.h"
#include "../activity/MeetActivityMgr.h"
#include "../mall/MallDataMgr.h"

#include "../activity/InTimeOnlineInfoMgr.h"
#include "../room/RoomRewardDataMgr.h"
#include "../roomLib/RoomCheckDataMgr.h"
#include "../roomLib/RoomSettingMgr.h"
#include "../box/BoxInfoManager.h"
#include "../box/DynamciBoxInfoManager.h"
#include "../activity/PlainTextActivityInfoMgr.h"
#include "../activity/GiftCodeInfoMgr.h"
#include "../vip/VIPDataMgr.h"
#include "../../datastructure/DanceGroupDataMgr.h"
#include "../activity/XmasFreeRewardDataMgr.h"
#include "../activity/FestivalWishDataMgr.h"
#include "../activity/FestivalBlessActivityDataMgr.h"
#include "../activity/ActivityDataStruct.h"
#include "../activity/AllActivitiesConfigMgr.h"
#include "../activity/InvitationActivityMgr.h"
#include "../activity/CumulativeSpendMgr.h"
#include "../quest/SpecailQuestMgr.h"
#include "../PhotoRoom/PhotoRoomLuaDataManager.h"
#include "../PhotoRoom/PhotoRoomDataStruct.h"
#include "../activity/PuzzleDataMgr.h"
#include "../AmuseRoom/AmuseRoomDataManager.h"
#include "../Notification/PushRatingDataManager.h"
#include "../security/SecurityQuizDataManager.h"
#include "../activity/FresherActivityMgr.h"
#include "../quest/QuestDataManager.h"
#include "../room/LiveRoomDataMgr.h"
#include "../activity/ExtraActivityInfoMgr.h"
#include "../anti-addiction/AntiAddictionDataMgr.h"
#include "../activity/PlatformPrivilegeDataMgr.h"
#include "../activity/DanceGroupTimerActivity.h"
//#include "../activity/PacketsDataMgr.h"
#include "../Couple/CoupleDataStructrue.h"
#include "../AmuseRoom/ConstellationMatchDataManager.h"
#include "../attribute/guidereward.h"
#include "../../datastructure/FriendDataManager.h"
#include "LuaCallback.h"
#include "../Dungeon/DungeonDataMgr.h"
#include "../activity/MallShareMgr.h"
#include "../util/Split.h"
#include "../activity/SocialShareMgr.h"
#include "../MonthCard/MonthCardDataMgr.h"
#include "../Rank/RankConfigData.h"
#include "../chat/ChatManager.h"
#include "../activeness/ActivenessDataManager.h"
#include "../Pet/PetEggConfig/PetEggConfigManager.h"
#include "../Pet/PetEggConfig/RandomPetCollect.h"
#include "../Pet/PetConfig/PetConfigManager.h"
#include "../activity/LongactingCumulativeRechareMgr.h"
#include "../activity/BuyItemGotMedalMgr.h"
#include "../Medal/MedalCfgMgr.h"
#include "../dancegroup/DanceGroupChallengeManager.h"
#include "../dancegroup/BigMamaConfigManager.h"
#include "../activity/RedEnvelope/RedEnvelope.h"
#include "../activity/RedEnvelope/RedEnvelopeConfigManager.h"
#include "../item/BagConfigManager.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../DanceGroupFairyLand/DanceGroupFairyLandCfgMgr.h"
#include "../activity/EffectStoneExchange/EffectStoneExchangeManager.h"

extern string g_sPath;
extern int g_nLine;
extern CLogicCirculator* g_pCirculator;
int  lua_ServerGetLine( lua_State * L )
{
	lua_pushnumber(L,g_nLine);
	return 1;
}

int lua_SetServerName( lua_State * L )
{
	bool bValid  =  ( lua_gettop( L ) == 3
		&& lua_isstring(L, 1)
		&& lua_isboolean(L, 2)
		&& lua_isboolean(L, 3));
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	const char * pName = lua_tostring(L,1);
	bool isFresherLine = lua_toboolean(L, 2) ? true : false;
	bool isEnableAudition = lua_toboolean(L, 3) ? true : false;

	unsigned char nInstallerID = isFresherLine ? INSTALLER_FRESHER : INSTALLER_FULL;

	g_pCirculator->SetServerNameInfo( pName );
	g_pCirculator->SetServerInstallerID(nInstallerID);
	g_pCirculator->SetEnableAudition(isEnableAudition);
	return 0;
}

int lua_GetServerPath( lua_State * L )
{
	lua_pushstring( L , g_sPath.c_str() );
	return 1;
}

int lua_AddMusicBoard( lua_State * L )
{
	if ( lua_gettop( L ) == 3
		&& lua_isnumber( L, 1 )
		&& lua_isboolean( L, 2 )
		&& lua_isboolean( L, 3 ) )
	{
		CMusicBoardManager::Instance().AddBoardInfo(
			(unsigned short)lua_tonumber( L, 1 ), 
			lua_toboolean( L, 2 )!=0,
			lua_toboolean( L, 3 )!=0 );
	}
	else
	{
		PARAM_ERROR;
	}

	return 0;
}

int lua_AddFresherSong( lua_State * L )
{
	if ( lua_gettop( L ) == 2
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 ) )
	{
		CMusicBoardManager::Instance().AddFresherSongInfo(
			(unsigned char)lua_tonumber( L, 1 ), 
			(int)lua_tonumber( L, 2 ));
	}
	else
	{
		PARAM_ERROR;
	}

	return 0;
}

int lua_AddCmd(lua_State *L)
{
    if (lua_gettop(L) == 2 && lua_isstring(L, 1) && lua_isnumber(L,2))
    {
        g_pCirculator->GetCommandProcessor()->AddCommandPrivilege(
            (char *)lua_tostring(L,1), (unsigned int)lua_tonumber(L, 2));
    }
    else
    {
        PARAM_ERROR;
    }

    return 0;
}

int lua_AddGradePercent( lua_State * L )
{
	if ( lua_gettop( L ) == 5
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isnumber( L, 4 )
		&& lua_isnumber( L, 5 ) )
	{
		CGradeInfoManager::Instance().AddGradePercentInfo(
			(int)lua_tonumber( L, 1 ), 
			(int)lua_tonumber( L, 2 ),
			(int)lua_tonumber( L, 3 ),
			(int)lua_tonumber( L, 4 ),
			(int)lua_tonumber( L, 5 ) );
	}
	else
	{
		PARAM_ERROR;
	}

	return 0;
}

int lua_AddGradeExp( lua_State * L )
{
	if ( lua_gettop( L ) == 7
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isnumber( L, 4 )
		&& lua_isnumber( L, 5 )
		&& lua_isnumber( L, 6 )
		&& lua_isnumber( L, 7 ) )
	{
		CGradeInfoManager::Instance().AddGradeExpInfo(
			(int)lua_tonumber( L, 1 ), 
			(int)lua_tonumber( L, 2 ),
			(int)lua_tonumber( L, 3 ),
			(int)lua_tonumber( L, 4 ),
			(int)lua_tonumber( L, 5 ),
			(int)lua_tonumber( L, 6 ),
			(int)lua_tonumber( L, 7 ) );
	}
	else
	{
		PARAM_ERROR;
	}

	return 0;
}

int lua_AddPlaceExp( lua_State * L )
{
	if ( lua_gettop( L ) == 5
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isnumber( L, 4 )
		&& lua_isnumber( L, 5 ) )
	{
		CGradeInfoManager::Instance().AddPlaceExpInfo(
			(int)lua_tonumber( L, 1 ), 
			(int)lua_tonumber( L, 2 ),
			(int)lua_tonumber( L, 3 ),
			(int)lua_tonumber( L, 4 ),
			(int)lua_tonumber( L, 5 ) );
	}
	else
	{
		PARAM_ERROR;
	}

	return 0;
}

int lua_AddFriendRelationNameDef(lua_State *L)
{
	if (lua_gettop(L) == 3 && lua_isstring(L, 1)  && lua_isstring( L, 2 ) && lua_isnumber(L, 3) ) {
		CFriendRelationInfoManager::Instance().AddFriendRelationNameDefInfo((int)lua_tonumber(L, 1), lua_tostring( L, 2 ), (int)lua_tonumber(L, 3));	
	} 
	else {
		PARAM_ERROR;
	}

	return 0;
}

int lua_AddFriendRelationInfo(lua_State *L)
{
	if (lua_gettop(L) == 3 && lua_isnumber(L,1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)) {
		CFriendRelationInfoManager::Instance().AddFriendRelation((int)lua_tonumber( L, 1 ), (int)lua_tonumber( L, 2 ), (int)lua_tonumber(L, 3));
	} else {
		PARAM_ERROR;
	}

	return 0;
}

int lua_AddFirstRechargeInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 11
		&& lua_isboolean( L, 1 )	// version

		&& lua_isstring( L, 2 )		// Start Time
		&& lua_isstring( L, 3 )		// End Time

		&& lua_isstring( L, 4 )		// Male Reward
		&& lua_isstring( L, 5 )		// Female Reward

		&& lua_isstring( L, 6 )		// Mail Title
		&& lua_isstring( L, 7 )		// Mail Content

		&& lua_isnumber( L, 8 )		// Charge Levels
		&& lua_isstring( L, 9 )		// Charge Range
		&& lua_isstring( L, 10 )	// Charge Mcoin
		&& lua_isstring( L, 11 )	// Charge Money
		);
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	Version_Type vType = lua_toboolean( L, 1 ) != 0 ? Version_Free : Version_Charge;
	std::string strStartTime = lua_tostring( L, 2 );
	std::string strEndTime = lua_tostring( L, 3 );

	std::string strMaleItem = lua_tostring( L, 4 );
	std::string strFemaleItem = lua_tostring( L, 5 );

	std::string strMailTitle = lua_tostring( L, 6 );
	std::string strMaileContent = lua_tostring( L, 7 );

	int nChargeLevel = (unsigned int)lua_tonumber( L, 8 );
	std::string strChargeRange = lua_tostring( L, 9 );
	std::string strChargeMcoin = lua_tostring( L, 10 );
	std::string strChargeMoney = lua_tostring( L, 11 );


	std::list<CItem> listMaleItem;
	std::list<CItem> listFemaleItem;
	TransferItemList( strMaleItem.c_str( ), listMaleItem );
	TransferItemList( strFemaleItem.c_str( ), listFemaleItem );

	std::vector<CChargePlan> vecChargePlan;
	if (!AnalyzingChargePlan( nChargeLevel, strChargeRange.c_str( ), strChargeMcoin.c_str( ), strChargeMoney.c_str( ), vecChargePlan ))
	{
		PARAM_ERROR;
		return 0;
	}

	PaymentInfoMgr::Instance( ).ReadFirstPaymentReward( vType, listMaleItem, listFemaleItem, nChargeLevel, vecChargePlan,
		strMailTitle, strMaileContent, SetTime( strStartTime.c_str( ) ), SetTime( strEndTime.c_str( ) ), strMaleItem, strFemaleItem );

	return 1;
}

int lua_AddPrivateQuest( lua_State *L )
{
	bool bValid  =  ( lua_gettop( L ) == 1 
		&& lua_isstring(L, 1)
		);
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	unsigned short questID = (unsigned short)lua_tonumber(L, 1);

	CPrivateQuest privateQuest;
	privateQuest.m_nQuestID = questID;

	CSpecailQuestMgr::Instance().AddPrivateQuest(privateQuest);

	return 1;
}

int lua_AddAnnounceVersion(lua_State * L)
{
	bool bValid  =  ( lua_gettop( L ) == 1 && lua_isnumber(L,1));
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int nVersion = (int)lua_tonumber(L, 1);

	AnnouncementMgr::Instance().AddVersion(nVersion);

	return 0;
}

int lua_AddAnnouncement( lua_State *L )
{
	AnnouncementMgr::Instance().AddAnnouncementInfo();

	return 0;	
}

int lua_AddAnnounceBriefInfo( lua_State * L )
{
	bool bValid  =  
		( lua_gettop( L ) == 7 
		&& lua_isnumber(L,1) 
		&& lua_isstring(L,2) 
		&& lua_isstring(L, 3) 
		&& lua_isstring(L, 4) 
		&& lua_isnumber(L, 5) 
		&& lua_isnumber(L, 6)
		&& lua_isnumber(L, 7)
		);
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	unsigned char nType = (unsigned char)lua_tonumber( L , 1 );
	string strTitle = lua_tostring(L, 2);
	string strContent = lua_tostring(L, 3);
	string strDate = lua_tostring(L, 4);
	bool bHot = lua_tonumber(L, 5) == 1 ? true : false;
	unsigned char nLinkTo = (unsigned char)lua_tonumber( L , 6 );
	unsigned short nPID = (unsigned short)lua_tonumber( L , 7 );

	AnnouncementMgr::Instance().AddBriefInfo(nType, strTitle, strContent, strDate, bHot, nLinkTo, nPID);

	return 0;
}

int lua_AddAnnounceIcons( lua_State *L)
{
	bool bValid  =  ( lua_gettop( L ) == 1 && lua_isstring(L,1));
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	string strIcon = lua_tostring(L, 1);
	AnnouncementMgr::Instance().AddIcons(strIcon);

	return 0;
}

int lua_AddBGMusic( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 1 && lua_isnumber(L, 1) );
	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}
	
	unsigned short nMusicID = (unsigned short)lua_tonumber( L, 1 );
	CBGMMgr::instance().AddMusic(nMusicID);

	return 1;
}

int lua_AddCheckInConfig( lua_State * L )
{
    bool bValid = ( lua_gettop( L ) == 8
        && lua_isnumber( L, 1)
        && lua_isnumber( L, 2)
        && lua_isstring( L, 3) 
        && lua_isstring( L, 4)
        && lua_isnumber( L, 5)
        && lua_isnumber( L, 6)
        && lua_isnumber( L, 7)
        && lua_isnumber( L, 8));

    if (!bValid) 
    {
        PARAM_ERROR;
        return 0;
    }

    unsigned int nMonth = (unsigned int)lua_tonumber( L, 1);

    CCheckInConfig checkininfo;
    checkininfo.m_nDayIndex = (unsigned int)lua_tonumber( L, 2);
    std::string strmalereward = lua_tostring( L, 3 );
    ::TransferItem( strmalereward.c_str(), checkininfo.m_maleItemReward);
    std::string strfemalereward = lua_tostring( L, 4 );
    ::TransferItem( strfemalereward.c_str(), checkininfo.m_femaleItemReward);
    checkininfo.m_nMoney = (unsigned int)lua_tonumber( L, 5);
    checkininfo.m_nBindCoin = (unsigned int)lua_tonumber( L, 6);
    checkininfo.m_nVipRewardLevel = (unsigned int)lua_tonumber( L, 7);
    checkininfo.m_nVipRewardRate = (unsigned int)lua_tonumber( L, 8);

    CCheckInDataManager::Instance().AddCheckInConfig( nMonth, checkininfo );

    return 1;
}

int lua_AddCheckInDescConfig( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 1 
		&& lua_isstring( L, 1) 
		);

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}

	std::string strDesc = lua_tostring( L, 1 );
	CCheckInDataManager::Instance().AddCheckInDescConfig(strDesc);

	return true;
}

int lua_ReadMoneyRstInfo( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 7 
		&& lua_isnumber( L, 1) 
		&& lua_isnumber( L, 2) 
		&& lua_isnumber( L, 3) 
		&& lua_isnumber( L, 4)
		&& lua_isnumber( L, 5)
		&& lua_isstring( L, 6)
		&& lua_isstring( L, 7)
		);
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}

	int nDay = (int)lua_tonumber( L, 1);
	int nHour = (int)lua_tonumber( L, 2);
	int nMinute = (int)lua_tonumber( L, 3);
	int nSecond = (int)lua_tonumber( L, 4);
	int nRemaindDaysBeforeMoneyRst = (int)lua_tonumber( L, 5 );
	string strTitle = lua_tostring( L, 6);
	string strContent = lua_tostring( L, 7);

	CMoneyMgr::Instance().ReadMoneyRst( nDay, nHour, nMinute, nSecond, nRemaindDaysBeforeMoneyRst, strTitle, strContent );

	return 1;
}

int lua_ReadLevelUpRewardInfo( lua_State * L )
{
    bool bValid = ( lua_gettop( L ) == 6 
        && lua_isnumber( L, 1 ) 
        && lua_isstring( L, 2 )
        && lua_isstring( L, 3 )
        && lua_isnumber( L, 4 )
        && lua_isnumber( L, 5 )
        && lua_isboolean( L, 6 ));
    if (!bValid)
    {
        PARAM_ERROR;
        return 0;
    }

    unsigned short nLevel = (unsigned short)lua_tonumber( L, 1 );
    string strMaleItem = lua_tostring( L, 2 );
    string strFemaleItem = lua_tostring( L, 3 );
    int nMoney = (int)lua_tonumber( L, 4 );
    int nBindMCoin = (int)lua_tonumber( L, 5 );
    bool bVIPReward = lua_toboolean( L, 6 ) != 0 ? true : false;

    std::list<CItem> listMaleReward;
    ::TransferItemList(strMaleItem.c_str(), listMaleReward);

    std::list<CItem> listFemaleReward;
    ::TransferItemList(strFemaleItem.c_str(), listFemaleReward);

    CLevelRewardInfo reward;
    reward.m_nLevel = nLevel;
    reward.m_listMaleItem = listMaleReward;
    reward.m_listFemaleItem = listFemaleReward;
    reward.m_nMoney = nMoney;
    reward.m_nBindMCoin = nBindMCoin;

    CLevelUpInfoMgr::Instance().AddRewardInfo( nLevel, reward, bVIPReward);

    return 1;
}

int lua_ReadLevelUpRecommend( lua_State * L )
{
    bool bValid = ( lua_gettop( L ) == 3 
        && lua_isnumber( L, 1 ) 
        && lua_isnumber( L, 2 )
        && lua_isstring( L, 3 ));
    if (!bValid)
    {
        PARAM_ERROR;
        return 0;
    }

    unsigned short nLevel = (unsigned short)lua_tonumber( L, 1 );
    int nFuncID = (int)lua_tonumber( L, 2 );
    string strFuncName = lua_tostring( L, 3 );

    CLevelUpInfoMgr::Instance().AddLevelUpRecommend( nLevel, nFuncID, strFuncName.c_str());

    return 1;
}

int lua_AddInTimeOnlineActivityBriefInfo( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 10
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isstring( L, 5 )
		&& lua_isstring( L, 6 )
		&& lua_isstring( L, 7 )
		&& lua_isstring( L, 8 )
		&& lua_isstring( L, 9 )
		&& lua_isstring( L, 10 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	bool bExhibit = (int)lua_tonumber( L, 1 ) > 0 ? true : false;
	int	nWeight = (int)lua_tonumber( L, 2 );	
	std::string strRegularImageName = lua_tostring( L, 3 );
	std::string strThumbnailsImageName = lua_tostring( L, 4 );
	std::string strActivityName = lua_tostring(L, 5);
	std::string strActivityIntro = lua_tostring( L, 6 );
	std::string strShowBeginTime = lua_tostring( L, 7 );
	std::string strShowEndTime = lua_tostring( L, 8 );
	std::string strActivityStartAnnouce = lua_tostring( L, 9 );
	std::string strActivityStopAnnounce = lua_tostring( L, 10 );

	CInTimeOnlineInfoMgr::Instance().AddBriefInfo(bExhibit, nWeight, 
		strRegularImageName, strThumbnailsImageName,
		strActivityName, strActivityIntro,
		SetTime(strShowBeginTime.c_str()), SetTime(strShowEndTime.c_str()),
		strActivityStartAnnouce, strActivityStopAnnounce);

	return 1;
}

int lua_AddInTimeOnlineActivity( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 9 && lua_isnumber( L, 1) && lua_isstring(L,2) && lua_isstring(L,3) && lua_isstring(L,4) && lua_isstring(L,5)
		&& lua_isstring(L,6) && lua_isstring(L,7) && lua_isstring(L,8) && lua_isstring(L,9));
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}
	int nActivityID = (int)lua_tonumber(L, 1);
	const char * szBeginTime = lua_tostring(L, 2);
	const char * szEndTime = lua_tostring(L, 3);
	const char * szTriggeringTime = lua_tostring(L, 4);
	const char * szMaleReward = lua_tostring(L, 5);
	const char * szFemaleReward = lua_tostring(L, 6);
	unsigned int nRewardMoney = (unsigned int)lua_tonumber(L, 7);
	const char * szTitle = lua_tostring(L, 8);
	const char * szContent = lua_tostring(L, 9);

	CInTimeOnlineInfoMgr::Instance().AddActivity(nActivityID, szBeginTime, szEndTime, szTriggeringTime, szMaleReward, szFemaleReward, nRewardMoney, szTitle, szContent);
	
	CInTimeOnlineActivity * pActivity = new CInTimeOnlineActivity();
	pActivity->Init(nActivityID, szBeginTime, szEndTime, szTriggeringTime, szMaleReward, szFemaleReward, nRewardMoney, szTitle, szContent);
	CSystemActivityManager::Instance().AddTimerActivity(pActivity);

	return 1;
}

int lua_AddOnlineTimeRewardBrief( lua_State * L )
{
	int i = 1;
	bool bValid = ( lua_gettop( L ) == 11 && 
					lua_isnumber( L, i++ ) && 
					lua_isnumber( L, i++ ) && 
					lua_isstring( L, i++ ) && 
					lua_isstring( L, i++ ) && 
					lua_isstring( L, i++ ) && 
					lua_isstring( L, i++ ) && 
					lua_isstring( L, i++ ) && 
					lua_isstring( L, i++ ) && 
					lua_isnumber( L, i++ ) && 
					lua_isnumber( L, i++ ) && 
					lua_isnumber( L, i++ ) );
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}
	i = 1;
	int nWeight = (int)lua_tonumber( L, i++ );
	bool bExhibit = (int)lua_tonumber( L, i++ ) > 0;
	string strShowBeginTime = lua_tostring( L, i++ );
	string strShowEndTime = lua_tostring(L, i++);
	string strRegularImageName = lua_tostring(L, i++);
	string strthumbnailName = lua_tostring(L, i++);
	string strActivityName = lua_tostring(L, i++);
	string strContent = lua_tostring(L, i++);
	int nHour = (int)lua_tonumber( L, i++ );
	int nMin = (int)lua_tonumber( L, i++ );
	int nSec = (int)lua_tonumber( L, i++ );
	
	COnlineRewardInfoMgr::Instance().ReadBriefInfo(nWeight, bExhibit, ::SetTime(strShowBeginTime.c_str()), ::SetTime(strShowEndTime.c_str()), strRegularImageName, strthumbnailName, strActivityName, strContent, nHour, nMin, nSec );

	return 1;
}

int lua_AddOnlineRewardInfo( lua_State *L )
{
	int i = 1;
	bool bValid = ( lua_gettop( L ) == 8 
		&& lua_isnumber( L, i++ ) 
		&& lua_isstring( L, i++ ) 
		&& lua_isstring( L, i++ ) 
		&& lua_isnumber( L, i++ ) 
		&& lua_isnumber( L, i++ ) 
		&& lua_isstring( L, i++ ) 
		&& lua_isstring( L, i++ ) 
		&& lua_isnumber( L, i++ )); 

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}
	i = 1;
	int nActivityIDX = (int)lua_tonumber( L, i++ );
	string activity_start_time = lua_tostring( L, i++ );
	string activity_end_time = lua_tostring( L, i++ );
	int nIndex = (int)lua_tonumber( L, i++ );
	unsigned int nRequireTime = (unsigned int)lua_tonumber( L, i++ );
	string strMaleReward = lua_tostring( L, i++ );
	string strFemaleReward = lua_tostring( L, i++ );
	int nMoney = (int)lua_tonumber( L, i++ );

	list<CItem> listMaleItem;
	TransferItemList(strMaleReward.c_str(), listMaleItem);
	list<CItem> listFemaleItem;
	TransferItemList(strFemaleReward.c_str(), listFemaleItem);

	COnlineRewardInfo info;
	info.m_nActivityIndex = nActivityIDX;
	info.m_nIndex = nIndex;
	info.m_nRequireTime = nRequireTime;
	info.m_listMaleReward = listMaleItem;
	info.m_listFemaleReward = listFemaleItem;
	info.m_nMoney = nMoney;
	info.m_tStartTime = ::SetTime(activity_start_time.c_str());
	info.m_tEndTime = ::SetTime(activity_end_time.c_str());

	COnlineRewardInfoMgr::Instance().ReadOnlineRewardInfo(info);

	return 1;
}

int lua_AddMeetRewardInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 5 
		&& lua_isnumber( L, 1 ) 
		&& lua_isstring( L, 2 ) 
		&& lua_isstring( L, 3 ) 
		&& lua_isstring( L, 4 ) 
		&& lua_isstring( L, 5)); 

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}

	int nIndex = (int)lua_tonumber( L, 1 );
	
	string strIndexName    = lua_tostring( L, 2 );
	string strMaleReward = lua_tostring( L, 3 );
	string strFemaleReward = lua_tostring( L, 4 );
	string strCostItem     = lua_tostring( L, 5 );

	list<CItem> listMaleItem;
	TransferItemList(strMaleReward.c_str(), listMaleItem);
	list<CItem> listFemaleItem;
	TransferItemList(strFemaleReward.c_str(), listFemaleItem);
	list<CItem> listCostItem;
	TransferItemList(strCostItem.c_str(), listCostItem);



	CMeetConfig info;
	info.m_nIndex = nIndex;
	info.m_strIndexName     = strIndexName;
	info.m_listMaleReward = listMaleItem;
	info.m_listFemaleReward = listFemaleItem;
	info.m_listCostItem     = listCostItem;

	CMeetActivityMgr::Instance().AddRewardInfo( nIndex, info );

	return 1;
}

int lua_AddMeetConfigInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 5
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 ) 
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isnumber( L, 5 )); 

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}

	int nPhotoIndexCount    = (int)lua_tonumber( L, 1 );
	int nWaitTime           = (int)lua_tonumber( L, 2 );
	string strMailTitle     = lua_tostring( L, 3 );
	string strMailContent   = lua_tostring( L, 4 );
	int nAmuseSceneID       = (int)lua_tonumber( L, 5 );
	

	CMeetActivityMgr::Instance().AddConfig( nPhotoIndexCount, nWaitTime, nAmuseSceneID, strMailTitle, strMailContent );

	return 1;
}

// int lua_AddDungeonListConfig( lua_State *L )
// {
// 	bool bValid = ( lua_gettop( L ) == 6
// 		&& lua_isnumber( L, 1 )
// 		&& lua_isnumber( L, 2 ) 
// 		&& lua_isstring( L, 3 )
//  		&& lua_tostring( L, 4 )
// 		&& lua_isnumber( L, 5 )
// 		&& lua_tostring( L, 6 )); 
// 
// 	if (!bValid) 
// 	{
// 		PARAM_ERROR;
// 		return 0;
// 	}
// 
// 	DungeonInfo info;
// 	info.nID			  = (unsigned int)lua_tonumber( L, 2 );
// 	info.strName	      = lua_tostring( L, 3 );
// 	info.strIconName      = lua_tostring( L, 4 );
// 	info.nStoryID		  = (int)lua_tonumber( L, 2 );
// 	
// 	std::string strParcloseList = lua_tostring( L, 6 );
// 	TransferList( strParcloseList.c_str(), info.listParclose );
// 
// 	CDungeonDataMgr::instance().AddDungeonListConfig( info );
// 
// 	return 1;
// }

int lua_AddSkipParcloseItem( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 1
		&& lua_tostring( L, 1 )); 

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}

	std::list<CItem> items;
	std::string strSkipParcloseItem = lua_tostring( L, 1 );
	TransferItemList( strSkipParcloseItem.c_str(), items );

	CDungeonDataMgr::instance().AddSkipParcloseItem( items );

	return 1;
}

// int lua_AddParcloseInfoConfig( lua_State *L )
// {
// 	bool bValid = ( lua_gettop( L ) == 16
// 		&& lua_isnumber( L, 1 )
// 		&& lua_isnumber( L, 2 ) 
// 		&& lua_isnumber( L, 3 )
// 		&& lua_isnumber( L, 4 )
// 		&& lua_isnumber( L, 5 )
//  		&& lua_isstring( L, 6 )
// 		&& lua_isnumber( L, 7 )
// 		&& lua_isnumber( L, 8 )
// 		&& lua_isstring( L, 9 )
// 		&& lua_isnumber( L, 10 )
// 		&& lua_isnumber( L, 11 )
// 		&& lua_isnumber( L, 12 )
// 		&& lua_isstring( L, 13 )
// 		&& lua_isnumber( L, 14 )
// 		&& lua_isnumber( L, 15 )
// 		&& lua_isstring( L, 16 )); 
// 
// 	if (!bValid) 
// 	{
// 		PARAM_ERROR;
// 		return 0;
// 	}
// 
// 	ParcloseInfo info;
// 	info.nID			  = (unsigned int)lua_tonumber( L, 2 );
// 	info.nCostCrystal	  = (unsigned int)lua_tonumber( L, 3 );
// 	info.nSceneID		  = (unsigned int)lua_tonumber( L, 4 );
// 	info.nMusicID         = (unsigned int)lua_tonumber( L, 5 );
// 	info.strMusicName     = lua_tostring( L, 6 );
// 	info.nMusicMode      = (unsigned int)lua_tonumber( L, 7 );
// 	info.nMusicLevel      = (unsigned int)lua_tonumber( L, 8 );
// 	info.nIsBoss		  = (unsigned int)lua_tonumber( L, 10 );
// 	info.fPosX            = (float)lua_tonumber( L, 11 );
// 	info.fPosY            = (float)lua_tonumber( L, 12 );
// 	info.strName          = lua_tostring( L, 13 );
// 	info.nStoryBeginID    = (unsigned int)lua_tonumber( L, 14 );
// 	info.nStoryEndID      = (unsigned int)lua_tonumber( L, 15 );
// 	info.strScorestand    = lua_tostring( L, 16 );
// 
// 	std::string strTargets  = lua_tostring( L, 9 );
//     TransferList( strTargets.c_str(), info.Targets );
// 	
// 	CDungeonDataMgr::instance().AddParcloseInfoConfig( info );
// 
// 	return 1;
// }

int lua_AddParcloseItemList( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 1 
		&& lua_isstring( L, 1 )); 

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}


	std::list<CItem> ItemList;
	string strItemIDList   = lua_tostring( L, 1 );
	TransferItemList( strItemIDList.c_str(), ItemList );

	CDungeonDataMgr::instance().AddParcloseItemList( ItemList );

	return 1;
}

int lua_AddStarRewardConfig( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 6 
		&& lua_isnumber( L, 1 ) 
		&& lua_isnumber( L, 2 ) 
		&& lua_isnumber( L, 3 ) 
		&& lua_isstring( L, 4 ) 
		&& lua_isstring( L, 5 )
		&& lua_isnumber( L, 6 )); 

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}

	CStarRewardConfig info;
	info.m_nID			   = (unsigned int)lua_tonumber( L, 2 );
	info.m_nCostStar		   = (unsigned int)lua_tonumber( L, 3 );
	string strMaleReward   = lua_tostring( L, 4 );
	TransferItemList( strMaleReward.c_str(), info.m_listMaleReward );
	string strFemaleReward = lua_tostring( L, 5 );
	TransferItemList(strFemaleReward.c_str(), info.m_listFemaleReward );
	info.m_nMoney			   = (unsigned int)lua_tonumber( L, 6 );

	CDungeonDataMgr::instance().AddStarRewardConfig( info );

	return 1;
}

int lua_AddGiveGetVitConfig( lua_State * L )
{
    bool bValid = ( lua_gettop( L ) == 3 
        && lua_isnumber( L, 1 ) 
        && lua_isnumber( L, 2 ) 
        && lua_isnumber( L, 3 )); 

    if (!bValid) 
    {
        PARAM_ERROR;
        return 0;
    }

    unsigned int nOnceGiveVitNum = (unsigned int)lua_tonumber( L, 1 );
    unsigned int nGiveGetVitResetTime = (unsigned int)lua_tonumber( L, 2 );
    unsigned int nGetVitMaxNum = (unsigned int)lua_tonumber( L, 3 );

    CDungeonDataMgr::instance().AddGiveGetVitConfig(nOnceGiveVitNum, nGiveGetVitResetTime, nGetVitMaxNum);

    return 1;
}

int lua_AddQuickFinishConfig( lua_State * L )
{
    bool bValid = ( lua_gettop( L ) == 2  
        && lua_isboolean( L, 1 ) 
        && lua_isstring( L, 2 )); 

    if (!bValid) 
    {
        PARAM_ERROR;
        return 0;
    }

    bool bQuickFinishToNotVip = lua_toboolean( L, 1 )? true : false;
    std::string strCostItem = lua_tostring( L, 2 );
    CItem quickFinishCostItem;
    ::TransferItem(strCostItem.c_str(), quickFinishCostItem);

    CDungeonDataMgr::instance().AddQuickFinishConfig(bQuickFinishToNotVip, quickFinishCostItem);

    return 1;
}

int lua_AddCrystalConfig( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 5 
		&& lua_isnumber( L, 1 ) 
		&& lua_isnumber( L, 2 ) 
		&& lua_isnumber( L, 3 )
		&& lua_isstring( L, 4 )
        && lua_isstring( L, 5 )); 

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}

	CCrystalConfig info;
	info.m_nInitialValue		   = (unsigned int)lua_tonumber( L, 1 );
	info.m_nMaxValue			   = (unsigned int)lua_tonumber( L, 2 );
	info.m_nRestoreTime		   = (unsigned int)lua_tonumber( L, 3 );
	std::string strDustCount = lua_tostring( L, 4 );
	TransferString( strDustCount.c_str(), info.m_nMinDustCount, info.m_nMaxDustCount );
    info.m_nFavorableBuyVitCount = (unsigned int)lua_tonumber( L, 5 ); 

	CDungeonDataMgr::instance().addCrystalConfig( info );

	return 1;
}



// int lua_AddTargetInfoConfig( lua_State * L )
// {
// 	bool bValid = ( lua_gettop( L ) == 5 
// 		&& lua_isnumber( L, 1 ) 
// 		&& lua_isnumber( L, 2 )
// 		&& lua_isstring( L, 3 )
// 		&& lua_isnumber( L, 4 )
// 		&& lua_isstring( L, 5 )); 
// 
// 	if (!bValid) 
// 	{
// 		PARAM_ERROR;
// 		return 0;
// 	}
// 
// 	ParcloseTargetInfo info;
// 	info.nID              = (unsigned int)lua_tonumber( L, 2 );
// 	info.strTargetName    = lua_tostring( L, 3 );
// 	info.nFlag            = (unsigned int)lua_tonumber( L, 4 );
// 	info.strValue         = lua_tostring( L, 5 );
// 
// 	CDungeonDataMgr::instance().AddTargetInfoConfig( info );
// 
// 	return 1;
// }

int lua_AddBuyCrystalInfoConfig( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 6 
		&& lua_isnumber( L, 1 ) 
		&& lua_isnumber( L, 2 )
		&& lua_isstring( L, 3 ) 
		&& lua_isnumber( L, 4 )
		&& lua_isnumber( L, 5 )
        && lua_isnumber( L, 6 )); 

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}

	CBuyCrystalConfig info;
	info.m_nIndex              = (unsigned int)lua_tonumber( L, 2 );
	info.m_strIconName			   = lua_tostring( L, 3 );
	info.m_nCrystalCount		   = (unsigned int)lua_tonumber( L, 4 );
	info.m_nPrice		   = (unsigned int)lua_tonumber( L, 5 );
    info.m_nFavorablePrice = (unsigned int)lua_tonumber( L, 6 );

	CDungeonDataMgr::instance().AddBuyCrystalConfig( info );

	return 1;
}

int lua_AddDustRewardConfig( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 7 
		&& lua_isnumber( L, 1 ) 
		&& lua_isstring( L, 2 ) 
		&& lua_isstring( L, 3 ) 
		&& lua_isnumber( L, 4 )
		&& lua_isstring( L, 5 )
		&& lua_isstring( L, 6 )
		&& lua_isnumber( L, 7 )); 

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}

	CDustRewardConfig info;
	info.m_nCostDust		   = (unsigned int)lua_tonumber( L, 1 );
	string strMaleReward   = lua_tostring( L, 2 );
	TransferItemList( strMaleReward.c_str(), info.m_listMaleReward );
	string strFemaleReward = lua_tostring( L, 3 );
	TransferItemList( strFemaleReward.c_str(), info.m_listFemaleReward );
	info.m_nMoney			   = (unsigned int)lua_tonumber( L, 4 );
	info.m_strActivityName   = lua_tostring( L, 5 );
	info.m_strIconName       = lua_tostring( L, 6 );
	info.m_nStoryID          = (unsigned int)lua_tonumber( L, 7 );

	
	CDungeonDataMgr::instance().AddDustRewardConfig( info );

	return 1;
}

int lua_AddMagicLampRewardMail( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 6 
		&& lua_isstring( L, 1 ) 
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 ) 
		&& lua_isstring( L, 4 )
		&& lua_isstring( L, 5 )
		&& lua_isstring( L, 6 )); 

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}

	CMagicLampRewardMail mail;
	mail.m_strStarRewardTitle     =      lua_tostring( L, 1 );
	mail.m_strStarRewardContent   =      lua_tostring( L, 2 );
	mail.m_strDustRewardTitle     =      lua_tostring( L, 3 );
	mail.m_strDustRewardContent   =      lua_tostring( L, 4 );
	mail.m_strParcloseDropTitle   =      lua_tostring( L, 5 );
	mail.m_strParcloseDropContent =      lua_tostring( L, 6 );

	CDungeonDataMgr::instance().AddRewardMailInfo( mail );

	return 1;
}

int lua_AddMagicLampDesc( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 3 
		&& lua_isstring( L, 1 )
		&& lua_isstring( L, 2 )
        && lua_isnumber( L, 3 )
        ); 

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}

	CMagicLampDesc desc;
	desc.m_strTitle     =      lua_tostring( L, 1 );
	desc.m_strContent   =      lua_tostring( L, 2 );
    desc.m_nMaxInviteFriendNum = (unsigned int)lua_tonumber( L, 3);

	CDungeonDataMgr::instance().AddDesc( desc );

	return 1;
}

int lua_AddGradeExpConfig( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 7 
		&& lua_isnumber( L, 1 ) 
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 ) 
		&& lua_isnumber( L, 4 )
		&& lua_isnumber( L, 5 )
		&& lua_isnumber( L, 6 )
		&& lua_isnumber( L, 7 )); 

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}

	
    int nLevel = (int)lua_tonumber( L, 1 );

	std::map<int,int> gradeExp;
	int nExp = (int)lua_tonumber( L, 2 );
	gradeExp[RoomDanceGrade_SS] = nExp;

	nExp =  (int)lua_tonumber( L, 3 );
	gradeExp[RoomDanceGrade_S] = nExp;

	nExp =  (int)lua_tonumber( L, 4 );
	gradeExp[RoomDanceGrade_A] = nExp;

	nExp =  (int)lua_tonumber( L, 5 );
	gradeExp[RoomDanceGrade_B] = nExp;

	nExp =  (int)lua_tonumber( L, 6 );
	gradeExp[RoomDanceGrade_C] = nExp;

	nExp =  (int)lua_tonumber( L, 7 );
	gradeExp[RoomDanceGrade_D] = nExp;


	CDungeonDataMgr::instance().AddParcloseGradeExpConfig( nLevel, gradeExp );

	return 1;
}

int lua_AddPublicityPictureConfig( lua_State * L )
{
    bool bValid = ( lua_gettop( L ) == 3 
        && lua_isstring( L, 2 ) 
        && lua_isstring( L, 3 )); 

    if (!bValid) 
    {
        PARAM_ERROR;
        return 0;
    }

    CPublicityPictureConfig config;
    config.m_strIcon = lua_tostring( L, 2);
    config.m_strAtlas = lua_tostring( L, 3);

    CDungeonDataMgr::instance().AddPublicityPictureConfig( config );

    return 1;
}

int lua_AddBaseExpConfig( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 2 
		&& lua_isnumber( L, 1 ) 
		&& lua_isnumber( L, 2 )); 

	if (!bValid) 
	{
		PARAM_ERROR;
		return 0;
	}


	unsigned int nFailExp = (unsigned int)lua_tonumber( L, 1 );
	unsigned int nWinBaseExp = (unsigned int)lua_tonumber( L, 2 );

	CDungeonDataMgr::instance().AddParcloseBaseExpConfig( nFailExp, nWinBaseExp );

	return 1;
}

int lua_AddExtraExpActivity( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 6 && lua_isnumber( L, 1) && lua_isstring(L,2) && lua_isstring(L,3) && lua_isstring(L,4) && lua_isstring(L,5) && lua_isnumber(L,6));
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}
	int nActivityID = (int)lua_tonumber(L, 1);
	const char * szBeginTime = lua_tostring(L, 2);
	const char * szEndTime = lua_tostring(L, 3);
	const char * szStartTime = lua_tostring(L, 4);
	const char * szStopTime = lua_tostring(L, 5);
	unsigned int nRewardExpRate = (unsigned int)lua_tonumber(L, 6);

	CExtraExpActivity * pActivity = new CExtraExpActivity();
	pActivity->Init(nActivityID, szBeginTime, szEndTime, szStartTime, szStopTime, (unsigned short)nRewardExpRate);
	CSystemActivityManager::Instance().AddTimerActivity(pActivity);
	return 1;
}

int lua_AddGiftInfo( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 8 && lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isstring(L, 3) && lua_isstring(L, 4) && lua_isnumber(L, 5) && lua_isstring(L, 6) && lua_isstring(L, 7) && lua_isstring(L, 8));
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}
	int nGiftID = (int)lua_tonumber(L, 1);
	bool bSendOnLogin = ((int)lua_tonumber(L, 2)) != 0;
	const char * szMaleReward = lua_tostring(L, 3);
	const char * szFemaleReward = lua_tostring(L, 4);
	unsigned int nRewardMoney = (unsigned int)lua_tonumber(L, 5);
	const char * szMessage = lua_tostring(L, 6);
	const char * szTitle = lua_tostring(L, 7);
	const char * szContent = lua_tostring(L, 8);

	CGiftInfo * pGiftInfo = new CGiftInfo();
	pGiftInfo->Init(nGiftID, bSendOnLogin, szMaleReward, szFemaleReward, nRewardMoney, szMessage, szTitle, szContent);

	CSystemActivityManager::Instance().GiftSystem().AddGift(pGiftInfo);
	return 1;
}

int lua_RoomKickSet( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 2
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nRoomStartWait = (int)lua_tonumber( L, 1 );
	int nRoomEndWait = (int)lua_tonumber( L, 2 );

    RoomSettingMgr::Instance().RoomKickSetInfo(nRoomStartWait, nRoomEndWait);

	return 1;
}

int lua_RoomMatchSet( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 2
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nAutoMatchCount = (int)lua_tonumber( L, 1 );
	int nAutoMatchInterval = (int)lua_tonumber( L, 2 );

    RoomSettingMgr::Instance().RoomMatchSetInfo(nAutoMatchCount, nAutoMatchInterval);

	return 1;
}

int lua_RoomMusicSet( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 3
					&& lua_isnumber( L, 1 )
					&& lua_isnumber( L, 2 )
					&& lua_isnumber( L, 3 )
		           );

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nDefalutScene = (int)lua_tonumber( L, 1 );
	int nDefaultMode = (int)lua_tonumber( L, 2 );
	int nDefaultLevel = (int)lua_tonumber( L, 3 );

    RoomSettingMgr::Instance().RoomMusicSetInfo(nDefalutScene, nDefaultMode, nDefaultLevel);

	return 1;
}

int lua_AddCheckInReward( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 4
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isnumber( L, 4 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	return 1;
}

int lua_AddCumulativeBrief( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 12
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isstring( L, 5 )
		&& lua_isstring( L, 6 )
		&& lua_isstring( L, 7 )
		&& lua_isstring( L, 8 )
		&& lua_isstring( L, 9 )
		&& lua_isstring( L, 10 )
		&& lua_isstring( L, 11 )
		&& lua_isstring( L, 12 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	bool bExhibit = (int)lua_tonumber( L, 1 ) > 0 ? true : false;
	int nWeight = (int)lua_tonumber( L, 2 );
	std::string strRegularImageName = lua_tostring( L, 3 );
	std::string strThumbnailsName = lua_tostring( L, 4 );
	std::string strActivityName = lua_tostring( L, 5 );
	std::string strContent = lua_tostring( L, 6 );
	std::string strActivityBeginTime = lua_tostring( L, 7 );
	std::string strActivityEndTime = lua_tostring( L, 8 );
	std::string strRechargeStartTime = lua_tostring( L, 9 );
	std::string strRechargeStopTime = lua_tostring( L, 10 );
	std::string strRechargeStartAnnouce = lua_tostring( L, 11 );
	std::string strRechargeStopAnnounce = lua_tostring( L, 12 );

	CCumulativeRechargeMgr::Instance().AddCumulativeBriefInfo(bExhibit, nWeight, 
															  strRegularImageName, strThumbnailsName,
															  strActivityName, strContent, strActivityBeginTime, strActivityEndTime, 
															  strRechargeStartTime, strRechargeStopTime, 
															  strRechargeStartAnnouce, strRechargeStopAnnounce);

	return 1;
}

int lua_AddCummulativeRecharge( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 6
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isnumber( L, 5 )
        && lua_isnumber( L, 6 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nLevel = (int)lua_tonumber( L, 1 );
	int nRequiredNum= (int)lua_tonumber( L, 2 );
	string strMaleItemList = lua_tostring( L, 3 );
	string strFemaleItemList = lua_tostring( L, 4 );
	int nMoney = (int)lua_tonumber( L, 5 );
    int nBindMCoin = (int)lua_tonumber( L, 6 );

	CCumulativeReward reward;
	reward.m_nLevel = nLevel;
	reward.m_nRequiredNum = nRequiredNum;
	TransferItemList(strMaleItemList.c_str(), reward.m_listMaleReward);
	TransferItemList(strFemaleItemList.c_str(), reward.m_listFemaleReward);
	reward.m_nMoney = nMoney;
    reward.m_nBindMCoin = nBindMCoin;

	CCumulativeRechargeMgr::Instance().AddCummulativeRechargeInfo(reward);

	return 1;
}

int lua_AddCumulativeSpendRewardInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 6
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isnumber( L, 5 )
		&& lua_isnumber( L, 6 ));

	if ( !bValid ) 
	{
		PARAM_ERROR;
		return 0;
	}

	int nLevel = (int)lua_tonumber( L, 1 );
	int nRequiredNum= (int)lua_tonumber( L, 2 );
	string strMaleItemList = lua_tostring( L, 3 );
	string strFemaleItemList = lua_tostring( L, 4 );
	int nMoney = (int)lua_tonumber( L, 5 );
	int nBindMCoin = (int)lua_tonumber( L, 6 );

	CCumulativeSpendReward reward;
	reward.m_nLevel = nLevel;
	reward.m_nRequiredNum = nRequiredNum;
	TransferItemList(strMaleItemList.c_str(), reward.m_listMaleReward);
	TransferItemList(strFemaleItemList.c_str(), reward.m_listFemaleReward);
	reward.m_nMoney = nMoney;
	reward.m_nBindMCoin = nBindMCoin;

	CCumulativeSpendMgr::Instance().AddCummulativeSpendRewardInfo(reward);

	return 1;
}

int lua_AddCumulativeSpendInfo( lua_State *L )
{

	bool bValid = ( lua_gettop( L ) == 13
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isstring( L, 5 )
		&& lua_isstring( L, 6 )
		&& lua_isstring( L, 7 )
		&& lua_isstring( L, 8 )
		&& lua_isstring( L, 9 )
		&& lua_isstring( L, 10 )
		&& lua_isstring( L, 11 )
		&& lua_isstring( L, 12 )
		&& lua_isstring( L, 13 ) 
		);

	if ( !bValid ) 
	{
		PARAM_ERROR;
		return 0;
	}

	bool bExhibit = (int)lua_tonumber( L, 1 ) > 0 ? true : false;
	int	 nCurrencyType = (int)lua_tonumber( L, 2 );
	int nWeight = (int)lua_tonumber( L, 3 );
	std::string strRegularImageName = lua_tostring( L, 4 );
	std::string strThumbnailsName = lua_tostring( L, 5 );
	std::string strActivityName = lua_tostring( L, 6 );
	std::string strContent = lua_tostring( L, 7 );
	std::string strActivityBeginTime = lua_tostring( L, 8 );
	std::string strActivityEndTime = lua_tostring( L, 9 );
	std::string strRechargeStartTime = lua_tostring( L, 10 );
	std::string strRechargeStopTime = lua_tostring( L, 11 );
	std::string strRechargeStartAnnouce = lua_tostring( L, 12 );
	std::string strRechargeStopAnnounce = lua_tostring( L, 13 );

	CCumulativeSpendMgr::Instance().AddCumulativeSpendInfo(bExhibit, nCurrencyType, nWeight, 
		strRegularImageName, strThumbnailsName,
		strActivityName, strContent, strActivityBeginTime, strActivityEndTime, 
		strRechargeStartTime, strRechargeStopTime, 
		strRechargeStartAnnouce, strRechargeStopAnnounce);

	return 1;
}

int lua_AddExchangeItemBriefInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 13
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isnumber( L, 5 )
		&& lua_isstring( L, 6 )
		&& lua_isstring( L, 7 )
		&& lua_isstring( L, 8 )
		&& lua_isstring( L, 9 )
		&& lua_isstring( L, 10 )
		&& lua_isstring( L, 11 )
		&& lua_isstring( L, 12 )
		&& lua_isstring( L, 13 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	bool bExhibit = (int)lua_tonumber( L, 1 ) > 0 ? true : false;
	int	nWeight = (int)lua_tonumber( L, 2 );	
	std::string strRegularImageName = lua_tostring( L, 3 );
	std::string strThumbnailsImageName = lua_tostring( L, 4 );
	itemtype_t nItemType = (itemtype_t)lua_tonumber( L, 5 );
	std::string strExhibitBeginTime = lua_tostring( L, 6 );
	std::string strExhibitEndTime = lua_tostring( L, 7 );
	std::string strActivityBeginTime = lua_tostring( L, 8 );
	std::string strActivityEndTime = lua_tostring( L, 9 );
	std::string strActivityName = lua_tostring(L, 10);
	std::string strActivityIntro = lua_tostring( L, 11 );
	std::string strActivityStartAnnouce = lua_tostring( L, 12 );
	std::string strActivityStopAnnounce = lua_tostring( L, 13 );

	CExchangeItemInfoMgr::Instance().AddActivityBriefInfo(bExhibit, nWeight, 
														  strRegularImageName, strThumbnailsImageName,
														  nItemType, 
														  strExhibitBeginTime, strExhibitEndTime,
														  strActivityBeginTime, strActivityEndTime, 
														  strActivityName, strActivityIntro,
														  strActivityStartAnnouce, strActivityStopAnnounce);

	return 1;
}


int lua_AddExchangeItemReward( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 5
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isnumber( L, 5 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nLevel = (int)lua_tonumber( L, 1 );
	int nItemCount= (int)lua_tonumber( L, 2 );
	string strMaleItemList = lua_tostring( L, 3 );
	string strFemaleItemList = lua_tostring( L, 4 );
	int nMoney = (int)lua_tonumber( L, 5 );

	CExchangeItemInfo exchagneInfo;
	exchagneInfo.m_nLevel = nLevel;
	exchagneInfo.m_nRequiredItemNum= nItemCount;
	TransferItemList(strMaleItemList.c_str(), exchagneInfo.m_listMaleReward);
	TransferItemList(strFemaleItemList.c_str(), exchagneInfo.m_listFemaleReward);
	exchagneInfo.m_nMoney = nMoney;

	CExchangeItemInfoMgr::Instance().AddExchangeItemInfo(exchagneInfo);

	return 1;
}

int lua_AddMaleRecommendEquipmentInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 3
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nIndex = (int)lua_tonumber( L, 1 );
	std::string strName = lua_tostring( L, 2 );
	std::string strMaleEquipment = lua_tostring( L, 3 );

	CMallDataMgr::Instance().AddMaleRecommedEquipment(nIndex, strName, strMaleEquipment);

	return 1;
}

int lua_AddFemaleRecommendEquipmentInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 3
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nIndex = (int)lua_tonumber( L, 1 );
	std::string strName = lua_tostring( L, 2 );
	std::string strFemaleEquipment = lua_tostring( L, 3 );

	if (strFemaleEquipment != "") {
		CMallDataMgr::Instance().AddFemaleRecommedEquipment(nIndex, strName, strFemaleEquipment);
	}

	return 1;
}

int lua_SetMallTipInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 2 && lua_isstring( L, 1 ) && lua_isstring( L, 2 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	CMallDataMgr::Instance().SetMallTip(lua_tostring( L, 1 ), lua_tostring( L, 2 ));

	return 1;
}

int lua_SetExperienceCardProp( lua_State *L )
{
	unsigned short timesPerDay = 0;
	unsigned short dayCanExperienced = 0;
	itemtype_t experienceCardItemType = 0;
	std::string experienceTypes = "";

	int nError = ParseLuaParam(L, timesPerDay, dayCanExperienced, experienceCardItemType, experienceTypes);

	if (nError != EParseLuaParam_Success)
	{
		PARAM_ERROR;
		return 0;
	}

	CMallDataMgr::Instance().SetExperienceCardProp(timesPerDay, dayCanExperienced, experienceCardItemType, experienceTypes);

	return 1;
}


int lua_MinContributionForDemiseSet( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 1 && lua_isnumber( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	unsigned int nMinContributionForDemise = (unsigned int)lua_tonumber( L, 1 );

	CSystemSettingMgr::Instance().SetMinContributionForDemise(nMinContributionForDemise);

	return 1;
}

int lua_MinLevelCoupleRequiredSet( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 1 && lua_isnumber( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	unsigned short nMinLevelCoupleRequired = (unsigned short)lua_tonumber( L, 1 );

	CSystemSettingMgr::Instance().SetMinLevelCoupleRequired(nMinLevelCoupleRequired);

	return 1;
}

int lua_MinIntimacyMarriageRequiredSet( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 1 && lua_isnumber( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nMinIntimacyMarriageRequired = (int)lua_tonumber( L, 1 );

	CSystemSettingMgr::Instance().SetMinIntimacyMarriageRequired(nMinIntimacyMarriageRequired);

	return 1;
}
int lua_MinRoomRewardPlayerNumSet( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 1 && lua_isnumber( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nMinRoomRewardPlayerNum = (int)lua_tonumber( L, 1 );

	CSystemSettingMgr::Instance().SetMinRoomRewardPlayerNum(nMinRoomRewardPlayerNum);

	return 1;
}

int lua_RoomRewardBaseDataSet( lua_State *L )
{
	bool bValid = (lua_gettop(L) == 2 && lua_isstring(L, 1) && lua_isstring(L, 2));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	const char * szTitle = lua_tostring(L, 1);
	const char * szContent = lua_tostring(L, 2);

	CRoomRewardDataMgr::Instance().InitBaseData(szTitle, szContent);

	return 1;
}

int lua_AddRandRoomRewardCountInfo( lua_State *L )
{
	bool bValid = (lua_gettop(L) == 2 && lua_isnumber(L, 1) && lua_isnumber(L, 2));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nCount = (int)lua_tonumber(L, 1);
	int nRate = (int)lua_tonumber(L, 2);

	CRoomRewardDataMgr::Instance().AddRandCount(nCount, nRate);

	return 1;
}

int lua_AddCertainRoomRewardInfo( lua_State *L )
{
	bool bValid = (lua_gettop(L) == 3 && lua_isstring(L, 1) && lua_isstring(L, 2) && lua_isnumber(L, 3));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	const char * szMaleItem = lua_tostring(L, 1);
	const char * szFemaleItem = lua_tostring(L, 2);
	int nRate = (int)lua_tonumber(L, 3);

	CRoomRewardDataMgr::Instance().AddCertainItem(szMaleItem, szFemaleItem, nRate);

	return 1;
}

int lua_AddRandRoomRewardInfo( lua_State *L )
{
	bool bValid = (lua_gettop(L) == 4 && lua_isnumber(L, 1) && lua_isstring(L, 2) && lua_isstring(L, 3) && lua_isnumber(L, 4));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nMode = (int)lua_tonumber(L, 1);
	const char * szMaleItem = lua_tostring(L, 2);
	const char * szFemaleItem = lua_tostring(L, 3);
	int nRate = (int)lua_tonumber(L, 4);

	CRoomRewardDataMgr::Instance().AddModeRandItem(nMode, szMaleItem, szFemaleItem, nRate);

	return 1;
}
int lua_AddBoxGroupInfo( lua_State *L )
{
	bool bValid = (lua_gettop(L) == 4 && lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isboolean(L, 4) );

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nBoxID = (int)lua_tonumber(L, 1);
	int nGroupID = (int)lua_tonumber(L, 2);
	int nGroupRate = (int)lua_tonumber(L, 3);
	bool isMale = lua_toboolean(L, 4)!=0;

	CBoxInfoManager::Instance().AddBoxGroup( nBoxID, nGroupID, nGroupRate, isMale );

	return 1;
}

int lua_AddBoxItemInfo( lua_State *L )
{
	bool bValid = (lua_gettop(L) == 6 && lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isstring(L, 3) && lua_isboolean(L, 4) && lua_isnumber(L, 5) && lua_isboolean(L, 6) );

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nBoxID = (int)lua_tonumber(L, 1);
	int nGroupID = (int)lua_tonumber(L, 2);
	const char* pItemInfo = lua_tostring(L, 3);
	bool bAnnounce = lua_toboolean(L, 4)!=0;
	int nItemRate = (int)lua_tonumber(L, 5);
	bool isMale = lua_toboolean(L, 6)!=0;

	CBoxInfoManager::Instance().AddBoxItem( nBoxID, nGroupID, pItemInfo, bAnnounce, nItemRate, isMale );

	return 1;
}

int lua_AddDynamicBoxRelatedGroupInfo( lua_State *L )
{
	bool bValid = (lua_gettop(L) == 2 && lua_isnumber(L, 1) && lua_isstring(L, 2));

	if ( !bValid ) 
	{
		PARAM_ERROR;
		return 0;
	}

	int nBoxID = (int)lua_tonumber(L, 1);

	std::string strRelatedGroup = lua_tostring(L, 2);
	unsigned int nItemID = 0;
	unsigned int nRelatedRate = 0;
	unsigned int nItemRate = 0;
	TransferKeyDynamicBox( strRelatedGroup.c_str(), nItemID, nRelatedRate, nItemRate );
	
	CDynamicBoxInfoManager::Instance().AddDynamicBoxRelatedGroup( nBoxID, nItemID, nRelatedRate, nItemRate );

	return 1;
}

int lua_AddDynamicBoxGroupInfo( lua_State *L )
{
	bool bValid = (lua_gettop(L) == 4 && lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isboolean(L, 4) );

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nBoxID = (int)lua_tonumber(L, 1);
	int nGroupID = (int)lua_tonumber(L, 2);
	int nGroupRate = (int)lua_tonumber(L, 3);
	bool isMale = lua_toboolean(L, 4)!=0;

	CDynamicBoxInfoManager::Instance().AddDynamicBoxGroup( nBoxID, nGroupID, nGroupRate, isMale );

	return 1;
}

int lua_AddDynamicBoxItemInfo( lua_State *L )
{
	bool bValid = (lua_gettop(L) == 6 && lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isstring(L, 3) && lua_isboolean(L, 4) && lua_isnumber(L, 5) && lua_isboolean(L, 6) );

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nBoxID = (int)lua_tonumber(L, 1);
	int nGroupID = (int)lua_tonumber(L, 2);
	const char* pItemInfo = lua_tostring(L, 3);
	bool bAnnounce = lua_toboolean(L, 4)!=0;
	int nItemRate = (int)lua_tonumber(L, 5);
	bool isMale = lua_toboolean(L, 6)!=0;

	CDynamicBoxInfoManager::Instance().AddDynamicBoxItem( nBoxID, nGroupID, pItemInfo, bAnnounce, nItemRate, isMale );

	return 1;
}


int lua_AddPlainTextActivityReward( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 4
		&& lua_isstring( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isnumber( L, 4 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	std::string strRewardName = lua_tostring( L, 1 );
	std::string strMaleItem = lua_tostring( L, 2 );
	std::list<CItem> listMaleItems;
	TransferItemList(strMaleItem.c_str(), listMaleItems);
	std::string strFemaleItem = lua_tostring( L, 3 );
	std::list<CItem> listFemaleItems;
	TransferItemList(strFemaleItem.c_str(), listFemaleItems);
	int nMoney = (int)lua_tonumber( L, 4 );
	CPlainTextActivityInfoMgr::Instance().AddRewardInfo(strRewardName, listMaleItems, listFemaleItems, nMoney);

	return 1;
}

int lua_AddPlainTextActivityBriefInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 8
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isstring( L, 5 )
		&& lua_isstring( L, 6 )
		&& lua_isstring( L, 7 )
		&& lua_isstring( L, 8 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	bool bExhibit = (int)lua_tonumber( L, 1 ) > 0 ? true : false;
	int	nWeight = (int)lua_tonumber( L, 2 );	
	std::string strRegularImageName = lua_tostring( L, 3 );
	std::string strThumbnailsImageName = lua_tostring( L, 4 );
	std::string strActivityTitle = lua_tostring( L, 5 );
	std::string strActivityContent = lua_tostring( L, 6 );
	std::string strShowBeginTime = lua_tostring( L, 7 );
	std::string strShowEndTime = lua_tostring( L, 8 );

	CPlainTextActivityInfoMgr::Instance().AddBriefInfo(bExhibit, nWeight, 
		strRegularImageName, strThumbnailsImageName,
		strActivityTitle, strActivityContent,
		SetTime(strShowBeginTime.c_str()), SetTime(strShowEndTime.c_str()) );

	return 1;
}

int lua_AddGiftCodeBriefInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 10
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isstring( L, 5 )
		&& lua_isstring( L, 6 )
		&& lua_isstring( L, 7 )
		&& lua_isstring( L, 8 )
		&& lua_isstring( L, 9 )
		&& lua_isstring( L, 10 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	bool bExhibit = (int)lua_tonumber( L, 1 ) > 0 ? true : false;
	int	nWeight = (int)lua_tonumber( L, 2 );	
	std::string strRegularImageName = lua_tostring( L, 3 );
	std::string strThumbnailsImageName = lua_tostring( L, 4 );
	std::string strActivityTitle = lua_tostring( L, 5 );
	std::string strActivityContent = lua_tostring( L, 6 );
	std::string strShowBeginTime = lua_tostring( L, 7 );
	std::string strShowEndTime = lua_tostring( L, 8 );
	std::string strActivityBeginTime = lua_tostring( L, 9 );
	std::string strActivityEndTime = lua_tostring( L, 10 );

	CGiftCodeInfoMgr::Instance().AddBriefInfo(bExhibit, nWeight, 
		strRegularImageName, strThumbnailsImageName,
		strActivityTitle, strActivityContent,
		SetTime(strShowBeginTime.c_str()), SetTime(strShowEndTime.c_str()),
		SetTime(strActivityBeginTime.c_str()), SetTime(strActivityEndTime.c_str()));

	return 1;
};

int lua_LuaAddVIPPrivInfo( lua_State *L )
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

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nLevel = (int)lua_tonumber( L, 1 );
	int nExp = (int)lua_tonumber( L, 2 );
	int nDailyExpBonus = (int)lua_tonumber( L,3 );
	bool bSpeakerFontEffe = lua_toboolean( L, 4 )!=0;
	bool bCanKickRoomPlayer = lua_toboolean( L, 5 )!=0;
	bool bExclusiveSelfInfoBoard = lua_toboolean( L, 6 )!=0;
	bool bExclusiveRoomAction = lua_toboolean( L, 7 )!=0;
	bool bEnterRoomSoundEffe = lua_toboolean( L, 8 )!=0;
	int nExpRateBounus = int(lua_tonumber( L, 9 ));
	int nRoleNameColor  = int(lua_tonumber( L, 10 ));
	int nLevelUpAnnounce = int(lua_tonumber( L, 11 ));
	std::string strEnterRoomViewEffe = lua_tostring( L, 12 );
	std::string strMaleReward = lua_tostring( L, 13 );
	std::string strFemaleReward = lua_tostring( L, 14 );
	int nMoney = int(lua_tonumber( L, 15));
	bool bOnlineAnnounce = lua_toboolean(L, 16)!=0;
	int nOnlineAnnounceCoolDown = int(lua_tonumber(L, 17));
	int nMaxFriendCount = int(lua_tonumber(L, 18));
	int nMaxBlackListCount = int(lua_tonumber(L, 19));
	int nConstellationMatchCount = int(lua_tonumber(L, 20));
	int nClothGroupCount = int(lua_tonumber(L, 21));
    int nDailyBindMBill = int(lua_tonumber(L, 22));
    

    std::list<CItem> listDailyReward;
    std::string strEveryDayItemReward = lua_tostring(L, 23);
    TransferItemList( strEveryDayItemReward.c_str(), listDailyReward);

	std::list<CItem> listMaleItem;
	if (!strMaleReward.empty()) {
		::TransferItemList(strMaleReward.c_str(), listMaleItem);
	}

	std::list<CItem> listFemaleItem;
	if (!strFemaleReward.empty()) {
		::TransferItemList(strFemaleReward.c_str(), listFemaleItem);
	}

    int nMagicBuyVitNum = int(lua_tonumber(L, 24));
    unsigned short nDanceGroupChallengeChance = (unsigned short)lua_tonumber( L, 25 );
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

int lua_LuaAddVIPInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 3
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
        && lua_isnumber( L, 3)
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nMaxLevel = (int)lua_tonumber( L, 1 );
	int nConsumePtExpBouns = (int)lua_tonumber( L, 2 );
    int nSendAddExpMaxBill = (int)lua_tonumber( L, 3 );
	CVIPDataMgr::Instance().AddVIPInfo(nMaxLevel, nConsumePtExpBouns,nSendAddExpMaxBill);

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

int lua_AddXmasFreeActivityInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 2 && 
		lua_isstring(L, 1) &&
		lua_isstring(L, 2));
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}

	std::string strActivityBeginTime = lua_tostring(L, 1);
	std::string strActivityEndTime = lua_tostring(L, 2);

	CXmasFreeRewardDataMgr::Instance().AddFreeRewardActivityInfo(SetTime(strActivityBeginTime.c_str()), SetTime(strActivityEndTime.c_str()));

	return 1;
}

int lua_AddXmasFreeRewardInfoLua( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 4 && 
		lua_isnumber(L, 1) && 
		lua_isstring(L, 2) &&
		lua_isstring(L, 3) && 
		lua_isnumber(L, 4) );
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}

	unsigned int nLevel = (int)lua_tonumber(L, 1);
	std::string strMaleItems = lua_tostring( L, 2 );;
	std::list<CItem> listMaleItems;
	TransferItemList(strMaleItems.c_str(), listMaleItems);
	std::string strFemaleItems = lua_tostring( L, 3 );
	std::list<CItem> listFemaleItems;
	TransferItemList(strFemaleItems.c_str(), listFemaleItems);
	int nMoney = (int)lua_tonumber( L, 4 );

	CXmasFreeRewardInfo * pXmasRewardInfo = new CXmasFreeRewardInfo;
	pXmasRewardInfo->m_nLevel = nLevel;
	pXmasRewardInfo->m_listMaleReward = listMaleItems;
	pXmasRewardInfo->m_listFemaleReward = listFemaleItems;
	pXmasRewardInfo->m_nMoney = nMoney;

	CXmasFreeRewardDataMgr::Instance().AddFreeRewardInfo(pXmasRewardInfo);

	return 1;
}

int lua_AddXmasMakeWishActivityDataLua( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 2 && 
		lua_isstring(L, 1) && 
		lua_isstring(L, 2) );
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}

	std::string strActivityStartTime = lua_tostring(L, 1);;
	std::string strActivityEndime = lua_tostring(L, 2);;

	CFestivalWishDataMgr::Instance().AddActivityData(SetTime(strActivityStartTime.c_str()), SetTime(strActivityEndime.c_str()));
	
	return 1;
}

int lua_AddXmasMakeWishItemDataRawLua( lua_State *L )
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

	int nStartTimes = (int)lua_tonumber(L, 1);	
	int nEndTimes = (int)lua_tonumber(L, 2);	
	itemtype_t nItemType = (unsigned short)lua_tonumber(L, 3);	
	unsigned short nItemCount = (unsigned short)lua_tonumber(L, 4);	

	CFestivalWishRequireData_Raw * pRawData = new CFestivalWishRequireData_Raw;
	pRawData->m_nWishStartTimes = nStartTimes;
	pRawData->m_nWishEndTimes = nEndTimes;
	pRawData->m_nItemType = nItemType;
	pRawData->m_nItemCount = nItemCount;

	CFestivalWishDataMgr::Instance().AddMakeWishRewardData_Raw(pRawData);

	return 1;
};

int lua_AddXmasMakeWishRewardDataLua( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 4 && 
		lua_isnumber(L, 1) &&
		lua_isstring(L, 2) &&
		lua_isstring(L, 3) && 
		lua_isnumber(L, 4) );
	if (!bValid) {
		PARAM_ERROR;
		return 0;
	}

	int nTimes = (int)lua_tonumber( L, 1 );
	std::string strMaleItems = lua_tostring( L, 2 );;
	std::list<CItem> listMaleItems;
	TransferItemList(strMaleItems.c_str(), listMaleItems);
	std::string strFemaleItems = lua_tostring( L, 3 );
	std::list<CItem> listFemaleItems;
	TransferItemList(strFemaleItems.c_str(), listFemaleItems);
	int nMoney = (int)lua_tonumber( L, 4 );

	CFestivalWishRewardData * pRewardInfo = new CFestivalWishRewardData;
	pRewardInfo->m_nTimes = nTimes;
	pRewardInfo->m_listMaleItems = listMaleItems;
	pRewardInfo->m_listFemaleItems = listFemaleItems;
	pRewardInfo->m_nMoney = nMoney;

	CFestivalWishDataMgr::Instance().AddRewardData(pRewardInfo);

	return 1;
};

int lua_AddFestivalBlessActivityInfoLua(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 5
		&& lua_isstring( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isnumber( L, 5 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	std::string strShowBeginTime = lua_tostring(L, 1);
	std::string strShowEndTime = lua_tostring(L, 2);
	std::string strBlessBeginTime = lua_tostring(L, 3);
	std::string strBlessEndTime = lua_tostring(L, 4);
	itemtype_t nRequireItemType = (itemtype_t)lua_tonumber(L, 5);

	CFestivalBlessActivityDataMgr::Instance().AddBlessActivityInfo(SetTime(strShowBeginTime.c_str()), SetTime(strShowEndTime.c_str()),
																   SetTime(strBlessBeginTime.c_str()), SetTime(strBlessEndTime.c_str()),
																   nRequireItemType);

	return 1;
}

int lua_AddFestivalBlessWordsLua(lua_State *L)
{
	bool bValid = ( lua_gettop( L ) == 2
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nIndex = (int)lua_tonumber(L, 1);
	std::string strBlessWords = lua_tostring(L, 2);

	CFestivalBlessWords *pBlessWords = new CFestivalBlessWords;
	pBlessWords->m_nIndex = nIndex;
	pBlessWords->m_strBlessWords = strBlessWords;

	CFestivalBlessActivityDataMgr::Instance().AddBlessWords(pBlessWords);

	return 1;
}

int lua_AddFestivalBlessRewardLua(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 5
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isnumber( L, 5 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nLevel = (int)lua_tonumber(L, 1);
	int nBeBlsssedCount = (int)lua_tonumber(L,2);
	std::string strMaleRewares = lua_tostring(L, 3);
	std::string strFemaleRewards = lua_tostring(L, 4);
	int nMoney = (int)lua_tonumber(L, 5);

	std::list<CItem> listMaleRewards;
	if (!strMaleRewares.empty()) {
		::TransferItemList(strMaleRewares.c_str(), listMaleRewards);
	}
	std::list<CItem> listFemaleRewards ;
	if (!strFemaleRewards.empty()) {
		::TransferItemList(strFemaleRewards.c_str(), listFemaleRewards);
	}

	CFestivalBlessedReward *pBlessReward = new CFestivalBlessedReward;
	pBlessReward->m_nLevel = nLevel;
	pBlessReward->m_nBeBlessedCount = nBeBlsssedCount;
	pBlessReward->m_listMaleReward = listMaleRewards;
	pBlessReward->m_listFemaleReward = listFemaleRewards;
	pBlessReward->m_nMoney = nMoney;

	CFestivalBlessActivityDataMgr::Instance().AddBlessRewards(pBlessReward);

	return 1;
}

int lua_AddMerriageDateGift(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 3
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	std::string strMaleRewares = lua_tostring(L, 2);
	std::string strFemaleRewards = lua_tostring(L, 3);

	std::list<CItem> listMaleRewards;
	if (!strMaleRewares.empty()) 
	{
		::TransferItemList(strMaleRewares.c_str(), listMaleRewards);
        CCoupleConfigDataMgr::Instance().AddMaleReward(listMaleRewards);
	}
	std::list<CItem> listFemaleRewards ;
	if (!strFemaleRewards.empty()) 
	{
		::TransferItemList(strFemaleRewards.c_str(), listFemaleRewards);
		 CCoupleConfigDataMgr::Instance().AddFemaleReward(listFemaleRewards);
	}

	return 1;
}

int lua_AddCoupleConfig(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 7
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isstring( L, 5 )
		&& lua_isstring( L, 6 )
		&& lua_isnumber( L, 7 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nBlessing = (int)lua_tonumber( L, 1 );
	int nValue = (int)lua_tonumber( L, 2 );
	unsigned int nBlessingFeedBackItem = (unsigned int)lua_tonumber(L, 3);

	std::map<unsigned int,itemtype_t> boxid;
	std::string strCommon = lua_tostring(L, 4);
	::TransferKeyValueList( strCommon.c_str(), boxid );

    std::string strExquisite = lua_tostring(L, 5);
    ::TransferKeyValueList( strExquisite.c_str(), boxid );

	std::string strLuxury = lua_tostring(L, 6);
	::TransferKeyValueList( strLuxury.c_str(), boxid );

	unsigned int nRedEnvelopeCount = (unsigned int)lua_tonumber(L, 7);
	
	CCoupleConfigDataMgr::Instance().AddCoupleConfig( nBlessing, nValue, nBlessingFeedBackItem, boxid, nRedEnvelopeCount );
	return 1;
}


int lua_AddFestivalSendBlessRewardLua(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 3
		&& lua_isstring( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isnumber( L, 3 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	std::string strMaleRewares = lua_tostring(L, 1);
	std::string strFemaleRewards = lua_tostring(L, 2);
	int nMoney = (int)lua_tonumber(L, 3);

	std::list<CItem> listMaleRewards;
	if (!strMaleRewares.empty()) {
		::TransferItemList(strMaleRewares.c_str(), listMaleRewards);
	}
	std::list<CItem> listFemaleRewards ;
	if (!strFemaleRewards.empty()) {
		::TransferItemList(strFemaleRewards.c_str(), listFemaleRewards);
	}

	CFestivalSendBlessReward SendBlessReward;
	SendBlessReward.m_listMaleRewards = listMaleRewards;
	SendBlessReward.m_listFemaleRewards = listFemaleRewards;
	SendBlessReward.m_nMoney = nMoney;

	CFestivalBlessActivityDataMgr::Instance().AddSendBlessReward(SendBlessReward);

	return 1;
}

int lua_AddXmasActivityTimeLua(lua_State * L)
{
    bool bValid = ( lua_gettop( L ) == 2
        && lua_isstring( L, 1 )
        && lua_isstring( L, 2 )
        );

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	std::string strStartTime = lua_tostring(L, 1);
	std::string strEndTime = lua_tostring(L, 2);

	CAllActivitiesConfigMgr::Instance().AddXmasActivityTime(::SetTime(strStartTime.c_str()), SetTime(strEndTime.c_str()));

	return 1;
}

int lua_SetChatRequirementLua(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 2
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nLobbyChatCDTime = (int)lua_tonumber( L, 1 );
	int nLobbyChatRequireLeve = (int)lua_tonumber(L, 2);

	CSystemSettingMgr::Instance().SetLobbyChatCDTime(nLobbyChatCDTime);
	CSystemSettingMgr::Instance().SetLobbyChatRequirLeve(nLobbyChatRequireLeve);

	return 1;
}

int lua_AddPhotoStudioDataLua(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 3
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		);


	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nMaxInvitePlayerCount = (int)lua_tonumber( L, 1 );
	int nInvitePlayerInterval = (unsigned short)lua_tonumber( L, 2 );
	int nInviteeResponseExpiredTime = (unsigned short)lua_tonumber( L, 3 );

	CPhotoRoomLuaDataManager::Instance().AddPhotoStudioData(nMaxInvitePlayerCount, nInvitePlayerInterval, nInviteeResponseExpiredTime);

	return 1;
}

int lua_AddPhotoStudioNotificationDataLua(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 3
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nLevel = (int)lua_tonumber( L, 1 );
	int nTimer = (int)lua_tonumber( L, 2 );
	int nNotificationType = (int)lua_tonumber( L, 3 );

	CPhotoRoomNotifiationTimer *pTimer = new CPhotoRoomNotifiationTimer;
	if (pTimer != NULL) {
		pTimer->m_nLevel = nLevel;
		pTimer->m_nTimeBeforeExpired = nTimer;
		pTimer->m_nNotifcationType = nNotificationType;
		CPhotoRoomLuaDataManager::Instance().AddNotificationTimer(pTimer);
	}

	return 1;
}

int lua_AddPhotoThemeNPCInfoLua(lua_State * L)
{
	int idx = 1;
	bool bValid = ( lua_gettop( L ) == 15
		&& lua_isnumber( L, idx++ )
		&& lua_isstring( L, idx++ )
		&& lua_isnumber( L, idx++ )
		&& lua_isnumber( L, idx++ )
		&& lua_isstring( L, idx++ )
		&& lua_isstring( L, idx++ )
		&& lua_isstring( L, idx++ )
		&& lua_isstring( L, idx++ )
		&& lua_isstring( L, idx++ )
		&& lua_isstring( L, idx++ )
		&& lua_isstring( L, idx++ )
		&& lua_isstring( L, idx++ )
		&& lua_isstring( L, idx++ )
		&& lua_isstring( L, idx++ )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	idx = 1;
	CPhotoRoomNPCInfo npcInfo;
	int nTheme = (int)lua_tonumber( L, idx++ );
	npcInfo.m_strName = lua_tostring( L, idx++ );
	npcInfo.m_nSexType = (unsigned char)lua_tonumber( L, idx++ );
	npcInfo.m_nSkin = (int)lua_tonumber( L, idx++ );
	npcInfo.m_strHeadwear = lua_tostring( L, idx++ );
	npcInfo.m_strHair = lua_tostring( L, idx++ );
	npcInfo.m_strFace = lua_tostring( L, idx++ );
	npcInfo.m_strShoulders = lua_tostring( L, idx++ );
	npcInfo.m_strGloves = lua_tostring( L, idx++ );
	npcInfo.m_strRighthand = lua_tostring( L, idx++ );
	npcInfo.m_strLefthand = lua_tostring( L, idx++ );
	npcInfo.m_strCloth = lua_tostring( L, idx++ );
	npcInfo.m_strShoes = lua_tostring( L, idx++ );
	npcInfo.m_strSuit = lua_tostring( L, idx++ );
	npcInfo.m_strLeg = lua_tostring( L, idx++ );

	CPhotoRoomLuaDataManager::Instance().AddNPCInfo(nTheme, npcInfo);

	return 1;
}

int lua_SetPhotoRoomInfoLua(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 1
		&& lua_isstring( L, 1 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	std::string strPhotoRoomStartTime = lua_tostring(L, 1);

	CSystemSettingMgr::Instance().SetPhotoRoomStartTime(::SetTime(strPhotoRoomStartTime.c_str()));

	return 1;
}

int lua_AddPuzzleBrief(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 4
		&& lua_isstring( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	const char* pPuzzleBeginTime = lua_tostring(L, 1);
	const char* pPuzzleEndTime = lua_tostring(L, 2);
	const char* pPuzzleHelpTitle = lua_tostring(L, 3);
	const char* pPuzzleHelpContent = lua_tostring(L, 4);

	int nBeginTime = SetTime( pPuzzleBeginTime );
	int nEndTime = SetTime( pPuzzleEndTime );
	CPuzzleDataMgr::Instance().AddPuzzleBriefInfo( nBeginTime, nEndTime, pPuzzleHelpTitle, pPuzzleHelpContent );

	return 1;
}

int lua_AddPuzzleDetail(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 11
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isboolean( L, 4 )
		&& lua_isnumber( L, 5 )
		&& lua_isnumber( L, 6 )
		&& lua_isnumber( L, 7 )
		&& lua_isnumber( L, 8 )
		&& lua_isstring( L, 9 )
		&& lua_isstring( L, 10 )
		&& lua_isnumber( L, 11 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nPuzzleID = (int)lua_tonumber(L, 1);
	const char* pTitle = lua_tostring(L, 2);
	const char* pTextute = lua_tostring(L, 3);
	bool bAnnounce = lua_toboolean(L, 4)!=0;

	int nNormalType = (int)lua_tonumber(L, 5);
	int nNormalCount = (int)lua_tonumber(L, 6);
	int nSpecialType = (int)lua_tonumber(L, 7);
	int nSpecialCount = (int)lua_tonumber(L, 8);

	const char* pMaleReward = lua_tostring(L, 9);
	const char* pFemaleReward = lua_tostring(L, 10);
	int nMoneyReward = (int)lua_tonumber(L, 11);

	CPuzzleDataMgr::Instance().AddPuzzleDetailInfo( nPuzzleID, pTitle, pTextute, bAnnounce,
		nNormalType, nNormalCount, nSpecialType, nSpecialCount, pMaleReward, pFemaleReward, nMoneyReward );

	return 1;
}

int lua_AddRoomCheck(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 4
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isnumber( L, 4 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nCheckMode = (int)lua_tonumber(L, 1);
	int nCheckTime = (int)lua_tonumber(L, 2);
	int nCheckNoMiss = (int)lua_tonumber(L,3);
	int nCheckMark = (int)lua_tonumber(L, 4);

	CRoomCheckDataMgr::Instance().AddRoomCheckInfo( nCheckMode, nCheckTime, nCheckNoMiss, nCheckMark );

	return 1;
}


int lua_AddInvitationActivity(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 14
		&& lua_isnumber( L, 1)
		&& lua_isnumber( L, 2)
		&& lua_isstring( L, 3)
		&& lua_isstring( L, 4)
		&& lua_isstring( L, 5 )
		&& lua_isstring( L, 6 )
		&& lua_isstring( L, 7 )
		&& lua_isstring( L, 8 )
		&& lua_isstring( L, 9 )
		&& lua_isstring( L, 10 )
		&& lua_isnumber( L, 11)
		&& lua_isstring( L, 12)
		&& lua_isstring( L, 13)
		&& lua_isnumber( L, 14));

	if ( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int weight = (unsigned int)lua_tonumber(L, 1);
	bool exhibit = (int)lua_tonumber(L, 2) ? true : false;
	const char* strActivityImage = lua_tostring( L, 3 );
	const char* strActivityThumbnail = lua_tostring( L, 4 );
	const char* strActivityBeginTime = lua_tostring( L, 5 );
	const char* strActivityEndTime = lua_tostring( L, 6 );
	const char* strActivityName = lua_tostring( L, 7 );
	const char* strContent = lua_tostring( L, 8 );
	const char* maleReward = lua_tostring(L, 9 );
	const char* femaleReward = lua_tostring(L, 10 );
	unsigned int money = (unsigned int)lua_tonumber(L, 11);
	const char* pszRewardMailTitle = lua_tostring(L, 12);
	const char* pszRewardMailContent = lua_tostring(L, 13);
	int visibleLevel = (unsigned int)lua_tonumber(L, 14);

	time_t tBeginTime = SetTime(strActivityBeginTime);
	time_t tEndTime = SetTime(strActivityEndTime);

	std::list<CItem> maleItemList, femaleItemList;

	TransferItemList(maleReward, maleItemList);
	TransferItemList(femaleReward, femaleItemList);

	CInvitationActivityMgr::Instance().AddActivity(
		weight, exhibit, strActivityImage, strActivityThumbnail,
		tBeginTime, tEndTime, strActivityName, strContent,
		maleItemList, femaleItemList, money,
		pszRewardMailTitle, pszRewardMailContent, visibleLevel);

	return 1;
}

int lua_AddInvitationInviterActivity(lua_State * L)
{
	int i = 1;
	bool bValid = ( lua_gettop( L ) == 7
		&& lua_isstring( L, i++)
		&& lua_isstring( L, i++)
		&& lua_isnumber( L, i++)
		&& lua_isstring( L, i++)
		&& lua_isstring( L, i++)
		&& lua_isstring( L, i++)
		&& lua_isstring( L, i++));

	if ( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	i = 1;
	const char* pstrActivityName = lua_tostring(L, i++);
	const char* pstrActivityContent = lua_tostring(L, i++);
	int nLevel = (unsigned int)lua_tonumber(L, i++);
	const char* pszRewardMailTitle = lua_tostring(L, i++);
	const char* pstrRewardMailContent = lua_tostring(L, i++);
	const char* pstrActivityBeginTime = lua_tostring(L, i++);
	const char* pstrActivityEndTime = lua_tostring(L, i++);

	CInvitationInviterConfigMgr::Instance().AddActivity(
		pstrActivityName, pstrActivityContent, nLevel, 
		pszRewardMailTitle, pstrRewardMailContent,
		pstrActivityBeginTime, pstrActivityEndTime);

	CSystemSettingMgr::Instance().SetInvitationStatus(true);

	return 1;
}

int lua_AddInvitationInviterReward(lua_State * L)
{
	if (lua_gettop( L ) != 5 || !lua_isnumber(L, 1) || !lua_isnumber(L, 2)
		|| !lua_isstring(L, 3) || !lua_isstring(L, 4) || !lua_isnumber(L, 5))
	{
		PARAM_ERROR;
		return 0;
	}

	int level = (int)lua_tonumber(L, 1);
	int cumulation = (int)lua_tonumber(L, 2);
	const char* maleItem = lua_tostring(L, 3);
	const char* femaleItem = lua_tostring(L, 4);
	int money = (int)lua_tonumber(L, 5);
	

	CInvitationInviterReward maleReward, femaleReward;
	maleReward.m_nLevel = level;
	maleReward.m_nCumulation = cumulation;
	maleReward.m_nMoney = money;
	TransferItemList(maleItem, maleReward.m_listItem);

	femaleReward.m_nLevel = level;
	femaleReward.m_nCumulation = cumulation;
	femaleReward.m_nMoney = money;
	TransferItemList(femaleItem, femaleReward.m_listItem);

	CInvitationInviterConfigMgr::Instance().AddCumulativeReward(maleReward, ESexType_Male);
	CInvitationInviterConfigMgr::Instance().AddCumulativeReward(femaleReward, ESexType_Female);

	return 1;
}

int lua_AddInvitationInviterAnimation(lua_State * L)
{
	if (lua_gettop( L ) != 3 || !lua_isnumber(L, 1) 
		|| !lua_isstring(L, 2) || !lua_isstring(L, 3))
	{
		PARAM_ERROR;
		return 0;
	}

	//int index = (int)lua_tonumber(L, 1);
	const char* maleAnimation = lua_tostring(L, 2);
	const char* femaleAnimation = lua_tostring(L, 3);

	CInvitationInviterConfigMgr::Instance().AddShareAnimation(maleAnimation, femaleAnimation);

	return 1;
}

int lua_AddAmuseDeviceInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 4
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isnumber( L, 4 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nSceneID = (int)lua_tonumber(L, 1);
	int nDeviceID = (int)lua_tonumber(L, 2);
	int nCapacity = (int)lua_tonumber(L, 3);
	int nInviteType = (int)lua_tonumber(L, 4);

	CAmuseRoomDataManager::Instance().AddDeviceData( nSceneID, nDeviceID, nCapacity, nInviteType );

	return 1;
}

int lua_AddAmuseCarrouselEventInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 6
		&& lua_isnumber( L, 1 )
		&& lua_isboolean( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isstring( L, 5 )
		&& lua_isstring( L, 6 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nSceneID = (int)lua_tonumber(L, 1);
	bool bIsLoop = lua_toboolean(L, 2)!=0;
	int nInterval = (int)lua_tonumber(L, 3);

	std::vector<std::string> vecStartSong;

	std::string strStartSong1 = lua_tostring(L, 4);
	if ( !strStartSong1.empty() )
	{
		vecStartSong.push_back( strStartSong1 );
	}

	std::string strStartSong2 = lua_tostring(L, 5);
	if ( !strStartSong2.empty() )
	{
		vecStartSong.push_back( strStartSong2 );
	}

	std::string strStartSong3 = lua_tostring(L, 6);
	if ( !strStartSong3.empty() )
	{
		vecStartSong.push_back( strStartSong3 );
	}

	CAmuseRoomDataManager::Instance().AddCarrouselData( nSceneID, bIsLoop, nInterval, vecStartSong );

	return 1;
}

int lua_AddAmuseExpEventInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 4
		&& lua_isnumber( L, 1 )
		&& lua_isboolean( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isnumber( L, 4 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nSceneID = (int)lua_tonumber(L, 1);
	bool bIsLoop = lua_toboolean(L, 2)!=0;
	int nInterval = (int)lua_tonumber(L, 3);
	int nAddExp = (int)lua_tonumber(L, 4);

	CAmuseRoomDataManager::Instance().AddExpData( nSceneID, bIsLoop, nInterval, nAddExp );

	return 1;
}

int lua_AddAmuseSwingEventInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 4
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isnumber( L, 4 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nSceneID = (int)lua_tonumber(L, 1);
	int nDeviceID = (int)lua_tonumber(L, 2);
	int nPushTime = (int)lua_tonumber(L, 3);
	int nSwingTime = (int)lua_tonumber(L, 4);

	CAmuseRoomDataManager::Instance().AddSwingData( nSceneID, nDeviceID, nPushTime, nSwingTime );

	return 1;
}

int lua_AddAmuseBoatInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 4
		&& lua_isnumber( L, 1 )
		&& lua_isboolean( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isnumber( L, 4 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nSceneID = (int)lua_tonumber(L, 1);
	bool bIsLoop = lua_toboolean(L, 2)!=0;
	int nInterval = (int)lua_tonumber(L, 3);
	int nBoatDock = (int)lua_tonumber(L, 4);

	CAmuseRoomDataManager::Instance().AddBoatData( nSceneID, bIsLoop, nInterval, nBoatDock );

	return 1;
}

int lua_AddAmuseEggInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 2
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nSceneID = (int)lua_tonumber(L, 1);
	int nDeviceID = (int)lua_tonumber(L, 2);

	CAmuseRoomDataManager::Instance().AddEggData(nSceneID, nDeviceID);

	return 1;
}

int lua_SetGuideMusicNameLua(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 1
		&& lua_isstring( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	std::string strGuideMusic = lua_tostring(L, 1);

	CSystemSettingMgr::Instance().SetGuideMusic(strGuideMusic);

	return 1;
}

int lua_SetDefaultMoveType(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 1
		&& lua_isnumber( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	unsigned char nDefaultMoveType = (unsigned char)lua_tonumber(L, 1);
	CSystemSettingMgr::Instance().SetDefaultMoveTypeInfo(nDefaultMoveType);

	return 1;
}

int lua_SetPushRatingSystem(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 9
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isboolean( L, 3 )
		&& lua_isnumber( L, 4 )
		&& lua_isnumber( L, 5 )
		&& lua_isnumber( L, 6 )
		&& lua_isboolean( L, 7 )
		&& lua_isboolean( L, 8 )
		&& lua_isboolean( L, 9 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nInterval = (int)lua_tonumber(L, 1);
	const char *pszMessage = lua_tostring(L, 2);
	bool bEnable = lua_toboolean(L, 3) != 0 ? true : false;
	int nMaxCount = (int)lua_tonumber(L, 4);
	int nResetWeekday = (int)lua_tonumber(L, 5);
	CPushRatingDataManager::Instance().SetPushRatingSystem(pszMessage, nInterval, bEnable, nMaxCount, nResetWeekday);
	int nOnlineTime = (int)lua_tonumber(L, 6);
	bool bEnableOnlineTimePush = lua_toboolean(L, 7) != 0 ? true : false;
	CPushRatingDataManager::Instance().SetPushOnlineTime(nOnlineTime, bEnableOnlineTimePush);
	bool bEnableFinishFisrtDailyQuest = lua_toboolean(L, 8) != 0 ? true : false;
	CPushRatingDataManager::Instance().SetPushFinishFirstDailyQuest(bEnableFinishFisrtDailyQuest);
	bool bEnableFinishFisrtDailyShoping = lua_toboolean(L, 9) != 0 ? true : false;

	CPushRatingDataManager::Instance().SetPushFinishFirstDailyShoping(bEnableFinishFisrtDailyShoping);

	return 1;
}

int lua_SetPushRatingLevel(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 1 
		&& lua_isnumber( L, 1 ) );

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nLevel = (int)lua_tonumber(L, 1);
	CPushRatingDataManager::Instance().AddPushLevel(nLevel);

	return 1;
}


int lua_AddSecurityQuiz(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 2
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	unsigned char nQuizID = (unsigned char)lua_tonumber(L, 1);
	const char *pszQuiz = lua_tostring(L, 2);
	SecurityQuizDataManager::instance().AddSecurityQuiz(nQuizID, pszQuiz);

	return 1;
}

int lua_AddSecurityQuizReward(lua_State * L)
{
	
	bool bValid = ( lua_gettop( L ) == 5
		&& lua_isstring( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isstring( L, 5 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	const char *pszMaleItem = lua_tostring(L, 1);
	const char *pszFemaleItem = lua_tostring(L, 2);
	int money = (int)lua_tonumber(L, 3);
	const char *pszMailTitle =  lua_tostring(L, 4);
	const char *pszMailContent = lua_tostring(L, 5);

	SecurityQuizDataManager::instance().SetSecurityQuizReward(pszMaleItem, pszFemaleItem, money, pszMailTitle, pszMailContent);

	CSystemSettingMgr::Instance().SetSecurityQuizStatus(true);

	return 1;
}



int lua_AddFresherActivity(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 7
		&& lua_isstring( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isnumber( L, 5 )
		&& lua_isnumber( L, 6 )
		&& lua_isnumber( L, 7 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	const char *name = lua_tostring(L, 1);
	const char *content = lua_tostring(L, 2);
	const char *title = lua_tostring(L, 3);
	const char *mail = lua_tostring(L, 4);
	int money = (int)lua_tonumber(L, 5);
	int duration = (int)lua_tonumber(L, 6);
	int cumulation = (int)lua_tonumber(L, 7);

	CFresherActivityManager::Instance().SetActivity(name, content, title, mail, money, duration, cumulation);

	return 1;
}

int lua_AddFresherActivityReward(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 9
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isnumber( L, 4 )
        && lua_isnumber( L, 5 )
        && lua_isstring( L, 6 )
        && lua_isstring( L, 7 )
        && lua_isnumber( L, 8 )
        && lua_isnumber( L, 9 )
        );

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

    std::string strMaleItem = lua_tostring(L, 2);
    std::string strFemaleItem = lua_tostring(L, 3);
    std::string strVipMaleItem = lua_tostring(L, 6);
    std::string strVipFemaleIem = lua_tostring(L, 7);

    CFresherReward malereward;
    malereward.m_nDay = (int)lua_tonumber(L, 1);
    malereward.m_nMoney = (int)lua_tonumber(L, 4);
    malereward.m_nBindCoin = (int)lua_tonumber(L, 5);
    malereward.m_nVipMoney = (int)lua_tonumber(L, 8);
    malereward.m_nVipBindCoin = (int)lua_tonumber(L, 9);
    ::TransferItemList( strMaleItem.c_str(), malereward.m_listItem );
    ::TransferItemList( strVipMaleItem.c_str(), malereward.m_listVipItem );

    CFresherReward femalereward;
    femalereward.m_nDay = malereward.m_nDay;
    femalereward.m_nMoney = malereward.m_nMoney;
    femalereward.m_nBindCoin = malereward.m_nBindCoin;
    femalereward.m_nVipMoney = malereward.m_nVipMoney;
    femalereward.m_nVipBindCoin = malereward.m_nVipBindCoin;
    ::TransferItemList( strFemaleItem.c_str(), femalereward.m_listItem );
    ::TransferItemList( strVipFemaleIem.c_str(), femalereward.m_listVipItem );


	CFresherActivityManager::Instance().AddFresherReward(malereward, femalereward);

	return 1;
}

int lua_AddFresherActivityCumulationReward(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 8
		&& lua_isstring( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isnumber( L, 3 )
        && lua_isnumber( L, 4 )
        && lua_isstring( L, 5 )
        && lua_isstring( L, 6 )
        && lua_isnumber( L, 7 )
        && lua_isnumber( L, 8 ));

	if ( !bValid ) 
    {
		PARAM_ERROR;
		return 0;
	}

	const char *maleItem = lua_tostring(L, 1);
	const char *femaleItem = lua_tostring(L, 2);
	int nMoney = (int)lua_tonumber(L, 3);
    int nBindCoin = (int)lua_tonumber(L, 4);
    const char *vipMaleItem = lua_tostring(L, 5);
    const char *vipFemaleItem = lua_tostring(L, 6);
    int nVipmoney = (int)lua_tonumber(L, 7);
    int nVipBindCoin = (int)lua_tonumber(L, 8);


	CFresherActivityManager::Instance().AddCumulationReward(maleItem, femaleItem, nMoney,nBindCoin,vipMaleItem,vipFemaleItem,nVipmoney,nVipBindCoin);

	return 1;
}

int lua_SetUpgradeInstallerLevel(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 1
		&& lua_isnumber( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	unsigned char nUpgradeInstallerLevel = (unsigned char)lua_tonumber(L, 1);
	CSystemSettingMgr::Instance().SetUpgradeInstallerLevelInfo(nUpgradeInstallerLevel);

	return 1;
}

int lua_AddLoadingAdInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 1
		&& lua_isstring( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	std::string strLoadingAD = lua_tostring( L, 1 );

	CSystemSettingMgr::Instance().AddLoadingAD(strLoadingAD);

	return 1;
}


int lua_SetFresherQuest(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 9
		&& lua_isstring( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isnumber( L, 5 )
		&& lua_isstring( L, 6 )
		&& lua_isstring( L, 7 )
        && lua_isstring( L, 8 )
        && lua_isstring( L, 9 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	const char *name = lua_tostring(L, 1);
	const char *description = lua_tostring(L, 2);
	const char *title = lua_tostring(L, 3);
	const char *mail = lua_tostring(L, 4);
	int duration = (int)lua_tonumber(L, 5);
	const char *beginTime = lua_tostring(L, 6);
	const char *endTime = lua_tostring(L, 7);
    const char *vipMailTitle = lua_tostring(L, 8);
    const char *vipMailContent = lua_tostring(L, 9);

	CFresherQuestDataManager::Instance().SetFresherQuest(
		name, description, title, mail, duration, beginTime, endTime, vipMailTitle, vipMailContent);

	return 1;
}

int lua_AddFresherQuest(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 12
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isstring( L, 5 )
		&& lua_isstring( L, 6 )
		&& lua_isnumber( L, 7 )
        && lua_isnumber( L, 8 )
        && lua_isstring( L, 9 )
        && lua_isstring( L, 10 )
        && lua_isnumber( L, 11 )
        && lua_isnumber( L, 12 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nDay = (int)lua_tonumber( L, 1);
	int nTargetType = (int)lua_tonumber( L, 2);
	int nTarget = (int)lua_tonumber( L, 3);
	const char *questDesc = lua_tostring( L, 4);
	const char *maleItem = lua_tostring( L, 5);
	const char *femaleItem = lua_tostring( L, 6);
	int nMoney = (int)lua_tonumber( L, 7);
    int nBindMCoin = (int)lua_tonumber( L,8);
    const char *vipMaleItem = lua_tostring( L, 9);
    const char *vipFemaleItem = lua_tostring( L, 10);
    int nVipMoney = (int)lua_tonumber( L, 11);
    int nVipBindMCoin = (int)lua_tonumber( L, 12);

	CFresherQuestDataManager::Instance().AddFresherQuest(
		nDay, nTargetType, nTarget, questDesc, maleItem, femaleItem, nMoney, nBindMCoin,
        vipMaleItem, vipFemaleItem, nVipMoney, nVipBindMCoin);

	return 1;
}

int lua_LoadLiveRoomConfigInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 5
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isnumber( L, 4 )
		&& lua_isnumber( L, 5 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

    RoomSettingMgr::Instance().LoadLiveRoomConfig(
		(int)lua_tonumber(L, 1), (int)lua_tonumber(L, 2), 
		(int)lua_tonumber(L, 3), (int)lua_tonumber(L, 4),
		(int)lua_tonumber(L, 5));

	return 1;
}

int lua_SetLiveRoomInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 7
		&& lua_isstring( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isstring( L, 5 )
		&& lua_isstring( L, 6 )
		&& lua_isnumber( L, 7 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	CLiveRoomDataManager::Instance().SetLiveRoom(
		lua_tostring(L, 1), lua_tostring(L, 2),
		lua_tostring(L, 3), lua_tostring(L, 4),
		lua_tostring(L, 5), lua_tostring(L, 6),
		(int)lua_tonumber(L, 7));

	return 1;
}


int lua_SetAuditionInfo(lua_State * L)
{
	int i = 1;
	bool bValid = ( lua_gettop(L) == 10
		&& lua_isstring(L, i++)
		&& lua_isstring(L, i++)
		&& lua_isstring(L, i++)
		&& lua_isstring(L, i++)
		&& lua_isnumber(L, i++)
		&& lua_isnumber(L, i++)
		&& lua_isnumber(L, i++)
		&& lua_isnumber(L, i++)
		&& lua_isnumber(L, i++)
        && lua_isnumber(L, i++));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	i = 1;
	const char *startDate = lua_tostring(L, i++);
	const char *endDate = lua_tostring(L, i++);
	const char *startTime = lua_tostring(L, i++);
	const char *endTime = lua_tostring(L, i++);

	int baseScore = (int)lua_tonumber(L, i++);
	int winScore = (int)lua_tonumber(L, i++);
	int lossScore = (int)lua_tonumber(L, i++);

	int matchPlayerCount = (int)lua_tonumber(L, i++);
	int difficulty = (int)lua_tonumber(L, i++);
    int nPunishedTime = (int)lua_tonumber(L, i++);

	CLiveRoomDataManager::Instance().SetAudition(
		startDate, endDate, startTime, endTime, 
		baseScore, winScore, lossScore, 
		matchPlayerCount, difficulty, nPunishedTime);

	return 1;
}

int lua_AddAuditionModeInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 1 && lua_isnumber( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	CLiveRoomDataManager::Instance().AddAuditionMode((int)lua_tonumber(L, 1));

	return 1;
}

int lua_AddAuditionScenceInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 1 && lua_isnumber( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	CLiveRoomDataManager::Instance().AddAuditionScence((int)lua_tonumber(L, 1));

	return 1;
}

int lua_AddAuditionMusicInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 1 && lua_isnumber( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	CLiveRoomDataManager::Instance().AddAuditionMusic((int)lua_tonumber(L, 1));

	return 1;
}

int lua_SetLiveRoomVoteInfo(lua_State * L)
{
	int i = 1;
	bool bValid = ( lua_gettop( L ) == 5
		&& lua_isboolean( L, i++ )
		&& lua_isnumber( L, i++ )
		&& lua_isnumber( L, i++ )
		&& lua_isnumber( L, i++ )
		&& lua_isnumber( L, i++ ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	i = 1;
	bool enableVote = lua_toboolean(L, i++)!=0;
	itemtype_t voteItem = (itemtype_t)lua_tonumber(L, i++);
	int voteItemHotValue = (int)lua_tonumber(L, i++);
	int voteRefreshInterval = (int)lua_tonumber(L, i++);
	int anounceItemCount = (int)lua_tonumber(L, i++);

	CLiveRoomDataManager::Instance().SetLiveRoomVote(
		enableVote, voteItem, voteItemHotValue, voteRefreshInterval, anounceItemCount);

	return 1;
}

int lua_SetExtraActivityInfoLua(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 1
		&& lua_isnumber( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nAmuseSceneID = (int)lua_tonumber(L, 1);
	CExtraActivityInfoMgr::Instance().SetAmuseSceneID(nAmuseSceneID);

	return 1;
}


int lua_AddTaikoBeckoningInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 2
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nRank = (int)lua_tonumber(L, 1);
	int nValue = (int)lua_tonumber(L, 2);
	if (nRank < RoomRoundRank_None || nRank >= RoomRoundRank_Max)
	{
		WriteLog(LOGLEVEL_ERROR, "lua_AddTaikoBeckoningInfo error nRank=%d", nRank);
	}
	else
	{
		CSystemSettingMgr::Instance().AddBeckoningValue(MusicMode_Taiko, nRank, nValue);
	}

	return 1;
}

int lua_AddTraditionBeckoningInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 2
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nRank = (int)lua_tonumber(L, 1);
	int nValue = (int)lua_tonumber(L, 2);
	if (nRank < RoomRoundRank_None || nRank >= RoomRoundRank_Max)
	{
		WriteLog(LOGLEVEL_ERROR, "lua_AddTraditionBeckoningInfo error nRank=%d", nRank);
	}
	else
	{
		CSystemSettingMgr::Instance().AddBeckoningValue(MusicMode_Tradition, nRank, nValue);
	}

	return 1;
}

int lua_AddBeckoningEffectInfo(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 2
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nLevel = (int)lua_tonumber(L, 1);
	int nValue = (int)lua_tonumber(L, 2);
	CSystemSettingMgr::Instance().AddBeckoningEffect(nLevel, nValue);

	return 1;
}

int lua_SetOpenAntiaddictionLua(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 1
		&& lua_isnumber( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int openAntiaddiction = (int)lua_tonumber(L, 1);
	CAntiAddictionDataMgr::Instance().SetOpenAntiaddiction(openAntiaddiction!=0);

	return 1;
}

int lua_SetOfflineMinutesLua(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 1
		&& lua_isnumber( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int n = (int)lua_tonumber(L, 1);
	CAntiAddictionDataMgr::Instance().SetOfflineMinutes(n);

	return 1;
}


int lua_AddAntiaddictionConfigLua(lua_State * L)
{
	int i = 1;
	bool bValid = ( lua_gettop( L ) == 9
		&& lua_isnumber( L, i++ )
		&& lua_isstring( L, i++ )
		&& lua_isnumber( L, i++ )
		&& lua_isstring( L, i++ )
		&& lua_isnumber( L, i++ )
		&& lua_isnumber( L, i++ )
		&& lua_isnumber( L, i++ )
		&& lua_isnumber( L, i++ )
		&& lua_isnumber( L, i++ )
		);

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	i = 1;

	AntiAddictionConfigItem cfg;
	int nConfigID = (int)lua_tonumber(L, i++);
	std::string strTimeRange = lua_tostring(L, i++);
	std::vector<std::string> vec;
	SplitString(strTimeRange, ",", vec);
	if (vec.size() == 2)
	{
		cfg.timeRange = std::make_pair(atoi(vec[0].c_str()), atoi(vec[1].c_str()));
		ASSERT(cfg.timeRange.second > cfg.timeRange.first);
	}
	else if (vec.size() == 1)
	{
		cfg.timeRange = std::make_pair(atoi(vec[0].c_str()), 0x0fffffff);//不用0x7fffffff，排序时会做加法，防止溢出
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "ERROR lua_AddAntiaddictionConfigLua error timeRange:%s", strTimeRange.c_str());
	}
	cfg.tipDiffMinutes = (int)lua_tonumber(L, i++);
	cfg.tipText = lua_tostring(L, i++);
	cfg.decrPercent[eAddicEffect_DanceExp] = (float)lua_tonumber(L, i++);
	cfg.decrPercent[eAddicEffect_AmuseExp] = (float)lua_tonumber(L, i++);
	cfg.decrPercent[eAddicEffect_QuestExp] = (float)lua_tonumber(L, i++);
	cfg.decrPercent[eAddicEffect_QuestGoldTick] = (float)lua_tonumber(L, i++);
	cfg.decrPercent[eAddicEffect_FightItemDropCount] = (float)lua_tonumber(L, i++);
	for (int i = 0; i < eAddicEffect_Max; ++i)
	{
		if (cfg.decrPercent[i] < 0 || cfg.decrPercent[i] > 1.0f)
		{
			WriteLog(LOGLEVEL_ERROR, "ERROR lua_AddAntiaddictionConfigLua error decrPercent:%f", cfg.decrPercent[i]);
		}
	}
	CAntiAddictionDataMgr::Instance().AddAntiAddictionConfig(nConfigID, &cfg);

	return 1;
}

int lua_SetPlatformAntiaddictionConfigLua(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 2
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int nCfgID = (int)lua_tonumber(L, 1);
	std::string strPID = lua_tostring(L, 2);
	std::vector<std::string> vec;
	SplitString(strPID, ",", vec);
	for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		int nPID = atoi(it->c_str());
		CAntiAddictionDataMgr::Instance().AddPlatformConfig(nPID, nCfgID);
	}

	return 1;
}

////////////UCVIP
int lua_AddPlatformPrivilege( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 4 
		&& lua_isstring( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		);

	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	const char *chName = lua_tostring( L, 1 );
	const char *chContent = lua_tostring( L, 2 );
	const char *chMailTitle = lua_tostring( L, 3 );
	const char *chMailContent = lua_tostring( L, 4 );

	CPlatformPrivilegeDataMgr::Instance().SetPlatformPrivilege( chName, chContent, chMailTitle, chMailContent );

	return 1;
}

int lua_AddPlatformPrivilegeReward( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 6 
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isstring( L, 5 )
		&& lua_isnumber( L, 6 )
		);

	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int nVipIndex = (int)lua_tonumber( L, 1 ); //特权编号
	int lowLevel = (int)lua_tonumber( L, 2 );
	int highLevel = (int)lua_tonumber( L, 3 );
	const char *maleItem = lua_tostring( L, 4 );
	const char *femaleItem = lua_tostring( L, 5 );
	int nPlatofromPrivilegeType = (int)lua_tonumber( L, 6 ); //特权类别

	CPlatformPrivilegeDataMgr::Instance().AddPlatformPrivilegeReward( nVipIndex, lowLevel, highLevel, maleItem, femaleItem, nPlatofromPrivilegeType );
	return 1;
}

////////////Packet
int lua_AddPacketsGroup(lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 4 
		&& lua_isnumber( L, 1 ) 
		&& lua_isnumber( L, 2 )
		&& lua_isstring( L, 3 ) 
		&& lua_isstring( L, 4 ) 
		);
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
//	int nId = (int)lua_tonumber( L, 1 );
//	int nMoney = (int)lua_tonumber( L, 2 );
//	const char *maleItem = lua_tostring( L, 3 );
//	const char *femaleItem = lua_tostring( L, 4 );

//	CPacketsDataMgr::Instance().AddPacketsData( nId, nMoney, maleItem, femaleItem );
	return 1;
}//end

//////////////////////////////////////////
////////////////////DnaceGroup舞团每天消耗操作
int lua_AddDanceGroupTimerInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 3
		&& lua_isstring( L, 1 ) 
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 ) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	const char *szBeginTime = lua_tostring( L, 1 );
	const char *szEndTime = lua_tostring( L, 2 );
	const char *szTriggerTime = lua_tostring( L, 3 );

	//舞团每天消耗
	CDanceGroupActivity *pDGActivity = new CDanceGroupActivity();
	pDGActivity->Init( szBeginTime, szEndTime, szTriggerTime );
	CSystemActivityManager::Instance().AddTimerActivity( pDGActivity );

	//舞团记录
	CDanceGroupRecord *pDGRecord = new CDanceGroupRecord();
	pDGRecord->Init( szBeginTime, szEndTime, szTriggerTime );
	CSystemActivityManager::Instance().AddTimerActivity( pDGRecord );


	return 1;
}//end

int lua_AddStarBaseInfo( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 14
		&& lua_isstring( L, 1 ) && lua_isstring(L, 2 )
		&& lua_isnumber( L, 3 ) && lua_isnumber( L, 4 )
		&& lua_isnumber( L, 5 ) && lua_isnumber( L, 6 )
		&& lua_isnumber( L, 7 ) && lua_isnumber( L, 8 ) 
		&& lua_isnumber( L, 9) && lua_isnumber( L, 10 ) 
		&& lua_isnumber( L, 11) && lua_isnumber( L, 12) 
		&& lua_isnumber( L, 13 ) && lua_isstring( L, 14) );
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	//每天首次点击“点一下”后的奖励
	const char* szMale = lua_tostring( L, 1 );
	const char* szFemale = lua_tostring( L, 2 );
	//点一下
	unsigned short nNormalCount = (unsigned short)lua_tonumber( L, 3 );
	int nNormalPower = (int)lua_tonumber( L, 4 ); 
	int nNormalContribution = (int)lua_tonumber( L, 5 );
	//话费金券
	unsigned short nMoneyCount = (unsigned short)lua_tonumber( L, 6 );
	int nMoneyNum = (int)lua_tonumber( L, 7 );
	int nMoneyPower = (int)lua_tonumber( L, 8 );
	int nMoneyContribution = (int)lua_tonumber( L, 9 );
	//用力推(Point)
	unsigned short nPointCount = (unsigned short)lua_tonumber( L, 10 );
	int nPointNum = (int)lua_tonumber( L, 11 );
	int nPointPower = (int)lua_tonumber( L, 12 );
	int nPointContribution = (int)lua_tonumber( L, 13 );
	string strRuleContent = lua_tostring( L, 14 );

	CDanceGroupDataMgr::Instance().SetBaseInfo( szMale, szFemale,
		nNormalCount, nNormalPower, nNormalContribution, 
		nMoneyCount, nMoneyNum, nMoneyPower, nMoneyContribution,
		nPointCount, nPointNum, nPointPower, nPointContribution, strRuleContent );

	return 1;
}//end

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

	int nDay = (int)lua_tonumber( L, 1 );
	int nIndex = (int)lua_tonumber( L, 2 );
	const char* szMale = lua_tostring( L, 3 );
	const char* szFemale = lua_tostring( L, 4 );

    CDanceGroupDataMgr::Instance().AddStarPacket((unsigned short)nDay, (unsigned short)nIndex, szMale, szFemale);

	return 1;
}//end

int lua_AddStarProgress( lua_State *L )
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

int lua_AddDanceGroupShopInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 6 
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isstring( L, 3 )
		&& lua_isstring( L, 4 )
		&& lua_isnumber( L, 5 )
		&& lua_isnumber( L, 6 )
		);
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	
    int nDayIndex = (int)lua_tonumber( L, 1 );
    int nItemIndex = (int)lua_tonumber( L, 2 );

    std::string strMaleItem = lua_tostring( L, 3 );
    std::string strFemaleItem = lua_tostring( L, 4 );
    int nExchangCount = (int)lua_tonumber( L, 5 );
    int nNeedContributionValue = (int)lua_tonumber( L, 6 );

	
	CDGShopItemInfo shopItemInfo;
    shopItemInfo.m_nId = (unsigned short)nItemIndex;
    shopItemInfo.m_nExchangeCount = (unsigned short)nExchangCount;
    shopItemInfo.m_nNeedContributionValue = (unsigned short)nNeedContributionValue;
	TransferItem( strMaleItem.c_str(), shopItemInfo.m_maleItem );
	TransferItem( strFemaleItem.c_str(), shopItemInfo.m_femaleItem );

	CDanceGroupDataMgr::Instance().AddDGShopItem( shopItemInfo, nDayIndex );
	
	return 1;
}

int lua_SetTimeEggSettingInfo(lua_State * L)
{
	int i = 1;
	bool bValid = ( lua_gettop( L ) == 13
		&& lua_isnumber( L, i++ )
		&& lua_isnumber( L, i++ )
		&& lua_isstring( L, i++ )
		&& lua_isstring( L, i++ )
		&& lua_isstring( L, i++ )
		&& lua_isstring( L, i++ )
		&& lua_isstring( L, i++ )
		&& lua_isstring( L, i++ )
		&& lua_isnumber( L, i++ )
		&& lua_isnumber( L, i++ )
		&& lua_isnumber( L, i++ )
		&& lua_isstring( L, i++ )
		&& lua_isstring( L, i++ )
		);


	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	i = 1;

	unsigned int nTimeEggOpenDuration = (unsigned int)lua_tonumber(L, i++);
	unsigned int nTimeEggDestroyDuration = (unsigned int)lua_tonumber(L, i++);
	string strCostItem = lua_tostring(L, i++);
	string strGiftItem = lua_tostring(L, i++);
	string strTogetherMaleItem = lua_tostring(L, i++);
	string strTogetherFemaleItem = lua_tostring(L, i++);
	string strMaleItem = lua_tostring(L, i++);
	string strFemaleItem = lua_tostring(L, i++);
	int nFixedMoneyReward = (int)lua_tonumber(L, i++);
	int nExtraMoneyReward = (int)lua_tonumber(L, i++);
	int nGiftDiscount = (int)lua_tonumber(L, i++);
	string strHelpTitle = lua_tostring(L, i++);
	string strHelpContent = lua_tostring(L, i++);
	bool bRes = CAmuseRoomDataManager::Instance().ParseSetting(
		nTimeEggOpenDuration, nTimeEggDestroyDuration,
		strCostItem, strGiftItem, strTogetherMaleItem, 
		strTogetherFemaleItem, strMaleItem, strFemaleItem,
		nFixedMoneyReward, nExtraMoneyReward, nGiftDiscount, strHelpTitle, strHelpContent);
	if (!bRes)
	{
		WriteLog(LOGLEVEL_ERROR, "time egg ParseSetting error");
	}

	return 1;
}
int lua_AddConstellationMatchOpenTime(lua_State *L)
{
	bool bValid = ( lua_gettop( L ) == 3 
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isstring( L, 3 )
		);

	if (!bValid)
	{
		PARAM_ERROR;
		return 0;
	}

	int nWeekday = (int)lua_tonumber( L, 1 );
	const char *strStartTime = lua_tostring( L, 2 );
	const char *strEndTime = lua_tostring( L, 3 );

	ConstellationMatchDataManager::Instance().AddOpenTime(nWeekday, strStartTime, strEndTime);

	return 1;
}

int lua_AddConstellationMatchTableInfo(lua_State *L)
{
	bool bValid = ( lua_gettop( L ) == 2 
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		);

	if (!bValid)
	{
		PARAM_ERROR;
		return 0;
	}

	int nConstellation = (int)lua_tonumber( L, 1 ) - 1;
	int nMatchConstellation = (int)lua_tonumber( L, 2 ) - 1;

	ConstellationMatchDataManager::Instance().AddMatchTable(nConstellation, nMatchConstellation);

	return 1;
}

int lua_AddConstellationMatchAmuseSceneIDInfo(lua_State *L)
{
	bool bValid = ( lua_gettop( L ) == 2 
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		);

	if (!bValid)
	{
		PARAM_ERROR;
		return 0;
	}

	int nScenceID = (int)lua_tonumber( L, 2 );

	ConstellationMatchDataManager::Instance().AddAmuseSceneID(nScenceID);

	return 1;
}

int lua_SetConstellationMatchInfo(lua_State *L)
{
	bool bValid = ( lua_gettop( L ) == 8 
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isnumber( L, 4 )
		&& lua_isnumber( L, 5 )
		&& lua_isnumber( L, 6 )
		&& lua_isnumber( L, 7 )
		&& lua_isstring( L, 8 )
		);

	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	int nRequiredRoleLevel = (int)lua_tonumber( L, 1 );
	const char *strFirstEnterTip = lua_tostring( L, 2 );
	int nPriorityCardCount = (int)lua_tonumber( L, 3 );
	int nPriorityCardCoolDown = (int)lua_tonumber( L, 4 );
	int nMaxUseTimes = (int)lua_tonumber( L, 5 );
	int nRoomCloseTime = (int)lua_tonumber( L, 6 );
	int nMatchInterval = (int)lua_tonumber( L, 7 );
	const char *strRule = lua_tostring( L, 8 );

	ConstellationMatchDataManager::Instance().SetConstellationMatch(nRequiredRoleLevel, 
		strFirstEnterTip, nPriorityCardCount, nPriorityCardCoolDown, 
		nMaxUseTimes, nRoomCloseTime, nMatchInterval, strRule);

	return 1;
}

int lua_AddGuideReward(lua_State* L)
{
	bool bValid = ( lua_gettop( L ) == 4 
		&& lua_isnumber( L, 1 )
		&& lua_isnumber( L, 2 )
		&& lua_isnumber( L, 3 )
		&& lua_isstring( L, 4 ));

	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	int nSetp = (int)lua_tonumber(L,1);
	int nSex = (int)lua_tonumber(L,2);
	int nIndex = (int)lua_tonumber(L,3);
	std::string strReward = lua_tostring(L,4);

	GuideRewardManager::Instance().AddGuideReward(nSetp,nSex,nIndex,strReward);
	return 1;
}

int lua_AddGuideContent(lua_State* L)
{
	bool bValid = ( lua_gettop( L ) == 2 
		&& lua_isstring( L, 1 )
		&& lua_isstring( L, 2 ));

	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}
	std::string strTitle = lua_tostring(L,1);
	std::string strContent = lua_tostring(L,2);
	GuideRewardManager::Instance().AddMailInfo(strTitle,strContent);
	return 1;
}

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

int lua_SetRecommendFriendInfo(lua_State * L)
{
    int i = 1;
    bool bValid = (lua_gettop( L ) == 3 
        && lua_isnumber( L, i++ )   // reset time
        && lua_isnumber( L, i++ )   // cd
        && lua_isnumber( L, i++ )   // refresh count
        );

    if ( !bValid )
    {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    unsigned int nResetTime = (unsigned int)lua_tonumber( L, i++ );
    unsigned int nCD = (unsigned int)lua_tonumber( L, i++ );
    unsigned int nRefreshCount = (unsigned int)lua_tonumber( L, i++ );

    CFriendDataManager::Instance().SetRecommendFriend(nResetTime, nCD, nRefreshCount);

    return 1;
}

int lua_AddRechargeInfo( lua_State *L )
{
	bool bValid = ( lua_gettop( L ) == 9
		&& lua_isboolean( L, 1 )	// version

		&& lua_isstring( L, 2 )		// Start Time
		&& lua_isstring( L, 3 )		// End Time

		&& lua_isnumber( L, 4 )		// Charge Levels
		&& lua_isstring( L, 5 )		// Charge Range
		&& lua_isstring( L, 6 )		// Charge Mcoin
		&& lua_isstring( L, 7 )		// Charge Money
		&& lua_isstring( L, 8 )		// Mail Title
		&& lua_isstring( L, 9 )		// Mail Content
		);
	if( !bValid )
	{
		PARAM_ERROR;
		return 0;
	}

	Version_Type versionType = lua_toboolean( L, 1 ) != 0 ? Version_Free : Version_Charge;
	std::string strStartTime = lua_tostring( L, 2 );
	std::string strEndTime = lua_tostring( L, 3 );

	int nChargeLevel = (int)lua_tonumber( L, 4 );
	std::string strChargeRange = lua_tostring( L, 5 );
	std::string strChargeMcoin = lua_tostring( L, 6 );
	std::string strChargeMoney = lua_tostring( L, 7 );
	std::string strMailTitle = lua_tostring( L, 8 );
	std::string strMailContent = lua_tostring( L, 9 );

	std::vector<CChargePlan> vecChargePlan;
	if (!AnalyzingChargePlan( nChargeLevel, strChargeRange.c_str( ), strChargeMcoin.c_str( ), strChargeMoney.c_str( ), vecChargePlan ))
	{
		PARAM_ERROR;
		return 0;
	}

	PaymentInfoMgr::Instance( ).ReadRechargeReward( versionType, nChargeLevel, vecChargePlan, SetTime( strStartTime.c_str( ) ), SetTime( strEndTime.c_str( ) ), strMailTitle, strMailContent );

	return 1;
}


int lua_AddProprietaryCodeInfo_Lua( lua_State *L )
{
	bool bValid =( lua_gettop(L) == 11 &&
		lua_isnumber( L, 1 ) &&lua_isnumber( L, 2 )
		&& lua_isstring( L, 3 ) && lua_isstring( L , 4 )
		&& lua_isstring( L , 5) && lua_isstring( L , 6 )
		&& lua_isstring( L , 7 ) && lua_isstring( L, 8)
		&& lua_isstring( L, 9 ) && lua_isstring( L, 10 ) && lua_isnumber( L , 11));
	if (!bValid)
	{
		PARAM_ERROR;
		return 0;
	}

	int nExhibit = (int)lua_tonumber( L, 1 );
	int nWeight = (int)lua_tonumber( L, 2 );
	std::string strRegularImageName = lua_tostring( L, 3 );
	std::string strThumbnailName = lua_tostring( L, 4 );
	std::string strActivityTitle = lua_tostring( L, 5 );
	std::string strActivityContent = lua_tostring( L, 6 );
	std::string strBeginTime = lua_tostring( L, 7 );
	std::string strEndTime = lua_tostring( L, 8 );
	std::string strMaleItem = lua_tostring( L, 9 );
	std::string strFemaleItem = lua_tostring( L, 10 );
	int nMoney = (int)lua_tonumber( L, 11 );

	std::list<CItem> maleItemList;
	TransferItemList( strMaleItem.c_str(), maleItemList );
	std::list<CItem> femaleItemList;
	TransferItemList( strFemaleItem.c_str(), femaleItemList );

	CProprietaryActivityInfoMgr::Instance().AddInfo( ( (nExhibit > 0) ? true : false ), nWeight,
		strRegularImageName, strThumbnailName,
		strActivityTitle, strActivityContent,
		SetTime(strBeginTime.c_str() ), SetTime( strEndTime.c_str()),
		maleItemList, femaleItemList, nMoney );

	return 1;
}

int lua_SetUpdateLBSPosInterval(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 1
		&& lua_isnumber( L, 1 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	unsigned short secds = (unsigned short)lua_tonumber(L, 1);
	CSystemSettingMgr::Instance().SetUpdateLBSPosInterval(secds);

	return 1;
}

int lua_SetBirthdayResetTimeLua(lua_State * L)
{
    bool bValid = ( lua_gettop( L ) == 1
        && lua_isstring( L, 1 ));

    if ( !bValid ) {
        PARAM_ERROR;
        return 0;
    }

    string strBirthdayResetTime = lua_tostring(L, 1);
    int resetTime = ::SetTime(strBirthdayResetTime.c_str());

    if (resetTime < 0)
        resetTime = 0;

    CSystemSettingMgr::Instance().SetBirthdayResetTime(resetTime);

    return 1;
}

int lua_SetOpenSendGift( lua_State * L )
{
    bool bValid = ( lua_gettop( L ) == 1 
        && lua_isboolean( L, 1 ) 
        );

    if ( !bValid )
    {
        PARAM_ERROR;

        return 0;
    }

    bool bOpenSendGift = (lua_toboolean( L, 1 ) != 0) ? true : false;

    CSystemSettingMgr::Instance().SetOpenSendGift(bOpenSendGift);

    return 1;
}

int lua_SetClothEffectPos( lua_State * L )
{
    bool bValid = ( lua_gettop( L ) == 1 
        && lua_isstring( L, 1 ) 
        );

    if ( !bValid )
    {
        PARAM_ERROR;
        return 0;
    }

    std::list<unsigned short> listOpenSlot;
    std::string strSlot = lua_tostring(L, 1);
    ::TransferList(strSlot.c_str(), listOpenSlot);
    CSystemSettingMgr::Instance().SetClothEffectPos(listOpenSlot);

    return 1;
}

int lua_AddFunctionSwitch(lua_State * L)
{
	bool bValid = ( lua_gettop( L ) == 3
		&& lua_isnumber( L, 1 )
		&& lua_isstring( L, 2 )
		&& lua_isboolean( L, 3 ));

	if ( !bValid ) {
		PARAM_ERROR;
		return 0;
	}

	int index = (int)lua_tonumber(L, 1);
	string fName = lua_tostring(L, 2);
	bool bOpen = (lua_toboolean(L, 3) != 0 ? true : false);

	CSystemSettingMgr::Instance().SetFunctionSwitch(index, bOpen);

	return 1;
}

int lua_AddBuffIcon( lua_State *L )
{
	bool bValid = (lua_gettop(L) == 2 && lua_isstring(L,1) && lua_isnumber(L,2));
	if (!bValid)
	{
		PARAM_ERROR;
		return 0;
	}

	string k = lua_tostring(L,1);
	int v = (int)lua_tonumber(L,2);
	CSystemSettingMgr::Instance().SetBuffIconItem(k, v);
	return 1;
}

int lua_AddBuffCanRemove( lua_State *L )
{
	bool bValid = (lua_gettop(L) == 1 && lua_isnumber(L,1));
	if (!bValid)
	{
		PARAM_ERROR;
		return 0;
	}

	int nGeneID = (int)lua_tonumber(L,1);
	CSystemSettingMgr::Instance().SetBuffCanRemove(nGeneID);
	return 1;
}

int lua_SetSocialShareInfo( lua_State *L )
{
	int i = 1;
	bool bValid = (lua_gettop(L) == 3 
		&& lua_isstring(L,i++)
		&& lua_isnumber(L,i++)
		&& lua_isstring(L,i++)
		);
	if (!bValid)
	{
		PARAM_ERROR;
		return 0;
	}
	i = 1;
	string strText = lua_tostring(L, i++);
	int nRewardBindMCoin = (int)lua_tonumber(L,i++);
	string strRewardDesc = lua_tostring(L, i++);
	CSystemSettingMgr::Instance().SetSocialShareText(strText);
	CSystemSettingMgr::Instance().SetSocialShareRewardBindMCoin(nRewardBindMCoin);
	CSystemSettingMgr::Instance().SetSocialShareRewardDesc(strRewardDesc);
	return 1;
}

int lua_AddMallSocialShareActivityInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 18
        && lua_isnumber( L, i++ )   // exhibit
        && lua_isnumber( L, i++ )   // weight
        && lua_isstring( L, i++ )   // title
        && lua_isstring( L, i++ )   // content
        && lua_isstring( L, i++ )   // big image name
        && lua_isstring( L, i++ )   // small image name
        && lua_isstring( L, i++ )   // begin time
        && lua_isstring( L, i++ )   // end time
        && lua_isstring( L, i++ )   // begin show time
        && lua_isstring( L, i++ )   // end show time
        && lua_isstring( L, i++ )   // male share text
        && lua_isstring( L, i++ )   // female share text
        && lua_isstring( L, i++ )   // mail title
        && lua_isstring( L, i++ )   // mail content
        && lua_isstring( L, i++ )   // male action name
        && lua_isstring( L, i++ )   // female action name
        && lua_isnumber( L, i++ )   // reward item time
        && lua_isstring( L, i++ )   // cloth position
        );

    if ( !bValid ) {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    bool bExhibit = (int)lua_tonumber( L, i++ ) > 0;
    int nWeight = (int)lua_tonumber( L, i++ );
    string title = lua_tostring( L, i++ );
    string content = lua_tostring( L, i++ );
    string bigImage = lua_tostring( L, i++ );
    string smallImage = lua_tostring( L, i++ );
    string beginTime = lua_tostring( L, i++ );
    string endTime = lua_tostring( L, i++ );
    string beginShowTime = lua_tostring( L, i++ );
    string endShowTime = lua_tostring( L, i++ );
    string maleShareText = lua_tostring( L, i++ );
    string femaleShareText = lua_tostring( L, i++ );
    string mailTitle = lua_tostring( L, i++ );
    string mailContent = lua_tostring( L, i++ );
    string maleActionName = lua_tostring( L, i++ );
    string femaleActionName = lua_tostring( L, i++ );
    int nItemTime = (int)lua_tonumber( L, i++ );
    string strClothPos = lua_tostring( L, i++ );

    CMallSocialShareActivityData data;
    data.m_bExhibit = bExhibit;
    data.m_nWeight = nWeight;
    data.m_strTitle = title;
    data.m_strContent = content;
    data.m_strBigImageName = bigImage;
    data.m_strSmallImageName = smallImage;
    data.m_nBeginTime = ::SetTime(beginTime.c_str());
    data.m_nEndTime = ::SetTime(endTime.c_str());
    data.m_strMaleShareText = maleShareText;
    data.m_strFemaleShareText = femaleShareText;
    data.m_strMailTitle = mailTitle;
    data.m_strMailContent = mailContent;
    data.m_listReward.clear();
    data.m_bIsReward = false;
    data.m_strMaleActionName = maleActionName;
    data.m_strFemaleActionName = femaleActionName;
    data.m_nRewardItemTime = nItemTime;
    Split(strClothPos, " |", data.m_setClothPos);

    time_t nBeginShowTime = ::SetTime(beginShowTime.c_str());
    time_t nEndShowTime = ::SetTime(endShowTime.c_str());

    CMallSocialShareActivity mallShareActivity(data, nBeginShowTime, nEndShowTime);

    CSocialShareMgr::Instance().AddSocialShareActivity(mallShareActivity);

    return 1;
}

int lua_AddMallShareRewardInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 4
        && lua_isnumber( L, i++ )   // index
        && lua_isstring( L, i++ )   // title
        && lua_isstring( L, i++ )   // male reward text
        && lua_isstring( L, i++ )   // female reward text
        );

    if ( !bValid ) {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    unsigned short idx = (unsigned short)lua_tonumber( L, i++ );
    string title = lua_tostring( L, i++ );
    string maleRewardText = lua_tostring( L, i++ );
    string femaleText = lua_tostring( L, i++ );

    CStringRewardInfo info;
    info.m_nIdx = idx;
    info.m_strTitle = title;
    info.m_strMaleRewardText = maleRewardText;
    info.m_strFemaleRewardText = femaleText;

    CSocialShareMgr::Instance().AddRewardItemInfo(EActivity_MallSocialShare, &info);

    return 1;
}

int lua_AddMallShareTimeRangeInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 6
        && lua_isstring( L, i++ )   // begin time
        && lua_isstring( L, i++ )   // end time
        && lua_isnumber( L, i++ )   // open time type
        && lua_isstring( L, i++ )   // open date
        && lua_isstring( L, i++ )   // begin hour
        && lua_isstring( L, i++ )   // end hour
        );

    if ( !bValid ) {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    string beginTime = lua_tostring( L, i++ );
    string endTime = lua_tostring( L, i++ );
    unsigned int type = (unsigned int)lua_tonumber( L, i++ );
    string strDate = lua_tostring( L, i++ );
    string beginHour = lua_tostring( L, i++ );
    string endHour = lua_tostring( L, i++ );

    time_t nBeginTime = ::SetTime(beginTime.c_str());
    time_t nEndTime = ::SetTime(endTime.c_str());

    list<unsigned int> listDate;
    Split(strDate.c_str(), " |", listDate);

    CSocialShareMgr::Instance().AddTimeRangeInfo(EActivity_MallSocialShare, nBeginTime, nEndTime, type, listDate, beginHour, endHour);

    return 1;
}

int lua_AddPhotoSocialShareActivityInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 15
        && lua_isnumber( L, i++ )   // exhibit
        && lua_isnumber( L, i++ )   // weight
        && lua_isstring( L, i++ )   // title
        && lua_isstring( L, i++ )   // content
        && lua_isstring( L, i++ )   // big image name
        && lua_isstring( L, i++ )   // small image name
        && lua_isstring( L, i++ )   // begin time
        && lua_isstring( L, i++ )   // end time
        && lua_isstring( L, i++ )   // begin show time
        && lua_isstring( L, i++ )   // end show time
        && lua_isstring( L, i++ )   // male share text
        && lua_isstring( L, i++ )   // female share text
        && lua_isstring( L, i++ )   // mail title
        && lua_isstring( L, i++ )   // mail content
        && lua_isnumber( L, i++ )   // people number
        );

    if ( !bValid ) {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    bool bExhibit = (int)lua_tonumber( L, i++ ) > 0;
    int nWeight = (int)lua_tonumber( L, i++ );
    string title = lua_tostring( L, i++ );
    string content = lua_tostring( L, i++ );
    string bigImage = lua_tostring( L, i++ );
    string smallImage = lua_tostring( L, i++ );
    string beginTime = lua_tostring( L, i++ );
    string endTime = lua_tostring( L, i++ );
    string beginShowTime = lua_tostring( L, i++ );
    string endShowTime = lua_tostring( L, i++ );
    string maleShareText = lua_tostring( L, i++ );
    string femaleShareText = lua_tostring( L, i++ );
    string mailTitle = lua_tostring( L, i++ );
    string mailContent = lua_tostring( L, i++ );
    int nPeopleNumber = (int)lua_tonumber( L, i++ );

    CPhotoSocialShareActivityData data;
    data.m_bExhibit = bExhibit;
    data.m_nWeight = nWeight;
    data.m_strTitle = title;
    data.m_strContent = content;
    data.m_strBigImageName = bigImage;
    data.m_strSmallImageName = smallImage;
    data.m_nBeginTime = ::SetTime(beginTime.c_str());
    data.m_nEndTime = ::SetTime(endTime.c_str());
    data.m_strMaleShareText = maleShareText;
    data.m_strFemaleShareText = femaleShareText;
    data.m_strMailTitle = mailTitle;
    data.m_strMailContent = mailContent;
    data.m_listReward.clear();
    data.m_bIsReward = false;
    data.m_nPeopleNumber = nPeopleNumber;

    time_t nBeginShowTime = ::SetTime(beginShowTime.c_str());
    time_t nEndShowTime = ::SetTime(endShowTime.c_str());

    CPhotoSocialShareActivity photoShareActivity(data, nBeginShowTime, nEndShowTime);

    CSocialShareMgr::Instance().AddSocialShareActivity(photoShareActivity);

    return 1;
}

int lua_AddPhotoShareRewardInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 7
        && lua_isnumber( L, i++ )   // index
        && lua_isstring( L, i++ )   // title
        && lua_isstring( L, i++ )   // male reward items
        && lua_isstring( L, i++ )   // female reward items
        && lua_isnumber( L, i++ )   // money
        && lua_isnumber( L, i++ )   // binding money
        && lua_isnumber( L, i++ )   // reward type
        );

    if ( !bValid ) {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    unsigned short idx = (unsigned short)lua_tonumber( L, i++ );
    string title = lua_tostring( L, i++ );
    string strMaleReward = lua_tostring( L, i++ );
    string strFemaleReward = lua_tostring( L, i++ );
    int money = (int)lua_tonumber( L, i++ );
    int bmoney = (int)lua_tonumber( L, i++ );
    unsigned char nRewardType = (unsigned char)lua_tonumber( L, i++ );

    list<CItem> listMaleItem;
    TransferItemList(strMaleReward.c_str(), listMaleItem);
    list<CItem> listFemaleItem;
    TransferItemList(strFemaleReward.c_str(), listFemaleItem);

    CReward maleReward;
    maleReward.m_listItems = listMaleItem;
    maleReward.m_nMoney = money;
    maleReward.m_nBindMCoin = bmoney;
    maleReward.m_nRewardType = nRewardType;

    CReward femaleReward;
    femaleReward.m_listItems = listFemaleItem;
    femaleReward.m_nMoney = money;
    femaleReward.m_nBindMCoin = bmoney;
    femaleReward.m_nRewardType = nRewardType;

    CItemRewardInfo info;
    info.m_nIdx = idx;
    info.m_strTitle = title;
    info.m_listMaleRewards.push_back(maleReward);
    info.m_listFemaleRewards.push_back(femaleReward);

    CSocialShareMgr::Instance().AddRewardItemInfo(EActivity_PhotoSocialShare, &info);

    return 1;
}

int lua_AddPhotoShareTimeRangeInfo( lua_State * L )
{
    int i = 1;
    bool bValid = ( lua_gettop( L ) == 6
        && lua_isstring( L, i++ )   // begin time
        && lua_isstring( L, i++ )   // end time
        && lua_isnumber( L, i++ )   // open time type
        && lua_isstring( L, i++ )   // open date
        && lua_isstring( L, i++ )   // begin hour
        && lua_isstring( L, i++ )   // end hour
        );

    if ( !bValid ) {
        PARAM_ERROR;

        return 0;
    }

    i = 1;
    string beginTime = lua_tostring( L, i++ );
    string endTime = lua_tostring( L, i++ );
    unsigned int type = (unsigned int)lua_tonumber( L, i++ );
    string strDate = lua_tostring( L, i++ );
    string beginHour = lua_tostring( L, i++ );
    string endHour = lua_tostring( L, i++ );

    time_t nBeginTime = ::SetTime(beginTime.c_str());
    time_t nEndTime = ::SetTime(endTime.c_str());

    list<unsigned int> listDate;
    Split(strDate.c_str(), " |", listDate);

    CSocialShareMgr::Instance().AddTimeRangeInfo(EActivity_PhotoSocialShare, nBeginTime, nEndTime, type, listDate, beginHour, endHour);

    return 1;
}

int lua_AddMonthCardInfo( lua_State * L )
{
	int i = 1;
	bool bValid = ( lua_gettop( L ) == 13
		&& lua_isnumber( L, 1 )   // begin time
		&& lua_isnumber( L, 2 )   // end time
		&& lua_isnumber( L, 3 )   // open time type
		&& lua_isnumber( L, 4 )   // open date
		&& lua_isnumber( L, 5 )   // begin hour
		&& lua_isstring( L, 6 )   // end hour
		&& lua_isstring( L, 7 )   // end hour
		&& lua_isnumber( L, 8 )   // end hourO
		&& lua_isnumber( L, 9 )   // end hour
		&& lua_isstring( L, 10 )   // end hour
		&& lua_isstring( L, 11 )   // end hour
		&& lua_isnumber( L, 12 )   // end hour
		&& lua_isstring( L, 13 )   // end hour
		);

	if ( !bValid ) {
		PARAM_ERROR;

		return 0;
	}

	MonthCardInfo info;
	info.nType = (EMonthCardType)((int)lua_tonumber( L, i++ ));
	info.nPrice = (int)lua_tonumber( L, i++ );
	info.nOpenDays = 24*3600*(unsigned int)lua_tonumber( L, i++ );
	info.nRenewDays = 24*3600*(unsigned int)lua_tonumber( L, i++ );
	info.nOpenMBillReward = (int)lua_tonumber( L, i++ );

	std::string strMaleOpenItemReward =  lua_tostring( L, i++ );
	TransferItemList( strMaleOpenItemReward.c_str(), info.listOpenMaleItemReward );
	std::string strFemaleOpenItemReward =  lua_tostring( L, i++ );
	TransferItemList( strFemaleOpenItemReward.c_str(), info.listOpenFemaleItemReward );
	
	info.nOpenMoneyReward = (int)lua_tonumber( L, i++ );
	info.nEveryDayMBillReward = (int)lua_tonumber( L, i++ );

	std::string strMaleEveryDayItemReward =  lua_tostring( L, i++ );
	TransferItemList( strMaleEveryDayItemReward.c_str(), info.listEveryDayMaleItemReward );

	std::string strFemaleEveryDayItemReward =  lua_tostring( L, i++ );
	TransferItemList( strFemaleEveryDayItemReward.c_str(), info.listEveryDayFemaleItemReward );

	info.nEveryDayMoneyReward = (int)lua_tonumber( L, i++ );
	info.strDesc = lua_tostring( L, i++ );

	CMonthCardDataMgr::Instance().AddMonthCardConfig( info );

	return 1;
}

int lua_AddDivorceConfig( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 4
		&& lua_isstring( L, 1 )  
		&& lua_isstring( L, 2 )   
		&& lua_isnumber( L, 3 )   
		&& lua_isstring( L, 4 )   
		);

	if ( !bValid ) {
		PARAM_ERROR;

		return 0;
	}

	
	string strDesc = lua_tostring( L, 1 );
	
	CItem compulsoryitem;
	string strCompulsoryItem = lua_tostring( L, 2 );
	TransferItem( strCompulsoryItem.c_str(), compulsoryitem);

	int	nRemarryIntimacyRate = (int)lua_tonumber( L, 3 );

	CItem divorceitem;
	string strDivorceItem = lua_tostring( L, 4 );
	TransferItem( strDivorceItem.c_str(), divorceitem);

	CDivorceConfigDataMgr::Instance().AddDivorceConfig( divorceitem, compulsoryitem, nRemarryIntimacyRate, strDesc );

	return 1;
}

int lua_AddDivorceMusicConfig( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 1
		&& lua_isstring( L, 1 )  
		);

	if ( !bValid ) {
		PARAM_ERROR;

		return 0;
	}


	string strMusic = lua_tostring( L, 1 );

	CDivorceConfigDataMgr::Instance().AddDivorceMusicConfig( strMusic );

	return 1;
}

int lua_AddDivorceGiftConfig( lua_State * L )
{
	bool bValid = ( lua_gettop( L ) == 2
		&& lua_isnumber( L, 1 )  
		&& lua_isnumber( L, 2 )
		);

	if ( !bValid ) {
		PARAM_ERROR;

		return 0;
	}

	unsigned short nMaleGiftID = (unsigned short)lua_tonumber( L, 1);
	unsigned short nFemaleGiftID = (unsigned short)lua_tonumber( L, 2);
	

	CDivorceConfigDataMgr::Instance().AddDivorceGiftConfig( nMaleGiftID, nFemaleGiftID );

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

    CRankConfigData::Instance().LoadStartTime(nStartWeekDay, nStartHour);

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

    CRankConfigData::Instance().LoadMaxOfflineTime( nMaxOfflineTime * 24 * 3600 - 5 * 60 );

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

    unsigned short nType = (unsigned short)lua_tonumber( L, 2 );

    CRankConfigData::Instance().AddRankHall(nType);

    return 1;
}

int lua_LoadRankRule( lua_State * L )
{
    string strRewardRule, strScoreRule;

    int nError = ParseLuaParam( L, strRewardRule, strScoreRule );
    if ( nError != EParseLuaParam_Success )
    {
        PARAM_ERROR;
        return 0;
    }

    CRankConfigData::Instance().LoadRule( strRewardRule, strScoreRule );

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

    CRankConfigData::Instance().AddRankReward( nType, nSecond, listItems, money, bindmcoin );

    return 1;
}

int lua_SetActivenessInfo(lua_State * L)
{
    std::string strName, strDesc, strStartTime, strEndTime;
    std::string strMailSender, strMailTitle, strMailContent, strVIPMailTitle, strVIPMailContent;

    int nError = ParseLuaParam(L, strName, strDesc, strStartTime, strEndTime, 
        strMailSender, strMailTitle, strMailContent, strVIPMailTitle, strVIPMailContent);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;
        return 0;
    }

    ActivenessDataManager::Instance().SetActiveness(
        strName.c_str(), strDesc.c_str(), strStartTime.c_str(), strEndTime.c_str(),
        strMailSender.c_str(), strMailTitle.c_str(), strMailContent.c_str(),
        strVIPMailTitle.c_str(), strVIPMailContent.c_str());

    return 1;
}

int lua_AddActivenessTargetInfo(lua_State * L)
{
    int nTargetType = 0;
    int nTargetValue = 0;
    int nTargetScore = 0;
    std::string strDesc;

    int nError = ParseLuaParam(L, nTargetType, nTargetValue, nTargetScore, strDesc);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;
        return 0;
    }

    ActivenessDataManager::Instance().AddActivenessTarget(
        nTargetType, nTargetValue, nTargetScore, strDesc.c_str());

    return 1;
}

int lua_AddActivenessRewardInfo(lua_State * L)
{
    int nRequireScore = 0;
    int nMoney = 0;
    int nBindMCoin = 0;
    int nVIPMoney = 0;
    int nVIPBindMCoin = 0;

    std::string strMaleItem, strFemaleItem, strVIPMaleItem, strVIPFemaleItem;

    int nError = ParseLuaParam(L, nRequireScore, strMaleItem, strFemaleItem, nMoney, nBindMCoin,
        strVIPMaleItem, strVIPFemaleItem, nVIPMoney, nVIPBindMCoin);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;
        return 0;
    }

    ActivenessDataManager::Instance().AddActivenessReward(
        nRequireScore, strMaleItem.c_str(), strFemaleItem.c_str(), nMoney, nBindMCoin,
        strVIPMaleItem.c_str(), strVIPFemaleItem.c_str(), nVIPMoney, nVIPBindMCoin);

    return 1;
}

int lua_AddRandomPetInfo( lua_State * L )
{
    unsigned int nEggTypeID = 0;
    unsigned short nPetTypeID = 0;
    unsigned char nPetPotentialID = 0;
    bool bAnnounce = false;
    int nRate = 0;

    int nError = ParseLuaParam(L, nEggTypeID, nPetTypeID, nPetPotentialID, bAnnounce, nRate);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    RandomPetInfo randomPetInfo(nPetTypeID, nPetPotentialID, bAnnounce);

    PetEggConfigManager::Instance().AddRandomPetInfo(nEggTypeID, randomPetInfo, nRate);

    return 1;
}

int lua_LoadPetSetting( lua_State * L )
{
    unsigned short nFreeGridCount = 0;
    unsigned short nExtendGridCount = 0;
    unsigned char nCostType = 1;
    unsigned int nCostAmount = 0;
    unsigned short nMaxUnlockCountOnce = 0;

    unsigned char nChangeNameCostType = 1;
    unsigned int nChangeNameCostAmount = 0;

    std::string strSkillRule;

    int nDanceBaseExp = 0;
    int nDanceDungeonSuccessExp = 0;
    int nDanceDungeonFailExp = 0;

    unsigned short nDanceCostEndurance = 0;
    int nEnduranceRecoveryTime = 0;

    int nFreeIncubationGuideStep = 0;
    int nFreeAccelerateGuideStep = 0;
    int nRewardPetEggGuideStep = 0;
    std::string strRewardPetEgg;

    Currency cost;

    int nError = ParseLuaParam(L, nFreeGridCount, nExtendGridCount, nCostType, nCostAmount, nMaxUnlockCountOnce,
        nChangeNameCostType, nChangeNameCostAmount, strSkillRule, nDanceBaseExp, nDanceDungeonSuccessExp,
        nDanceDungeonFailExp, nDanceCostEndurance, nEnduranceRecoveryTime, 
        nFreeIncubationGuideStep, nFreeAccelerateGuideStep, nRewardPetEggGuideStep, strRewardPetEgg);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    cost.SetType(nCostType);
    cost.SetAmount(nCostAmount);
    PetConfigManager::Instance().SetGridInfo(nFreeGridCount, nExtendGridCount, nMaxUnlockCountOnce, cost);

    cost.SetType(nChangeNameCostType);
    cost.SetAmount(nChangeNameCostAmount);
    PetConfigManager::Instance().SetChangeNameCost(cost);

    PetConfigManager::Instance().SetSkillRule(strSkillRule);

    PetConfigManager::Instance().SetDanceBaseExp(nDanceBaseExp);
    PetConfigManager::Instance().SetDanceDungeonExp(nDanceDungeonSuccessExp, nDanceDungeonFailExp);
    PetConfigManager::Instance().SetDanceCostEndurance(nDanceCostEndurance);
    PetConfigManager::Instance().SetEnduranceRecoveryTime(nEnduranceRecoveryTime);
    PetConfigManager::Instance().SetFreeIncubationGuideStep(nFreeIncubationGuideStep);
    PetConfigManager::Instance().SetFreeAccelerateGuideStep(nFreeAccelerateGuideStep);
    PetConfigManager::Instance().SetRewardPetEggGuideStep(nRewardPetEggGuideStep);
    PetConfigManager::Instance().SetRewardPetEgg(strRewardPetEgg);

    return 1;
}

int lua_AddPetFeedExpItem( lua_State * L )
{
    unsigned int nItemType = 0;

    int nError = ParseLuaParam(L, nItemType);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    PetConfigManager::Instance().AddFeedExpItem(nItemType);

    return 1;
}

int lua_AddPetFeedEnduranceItem( lua_State * L )
{
    unsigned int nItemType = 0;

    int nError = ParseLuaParam(L, nItemType);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    PetConfigManager::Instance().AddFeedEnduranceItem(nItemType);

    return 1;
}

int lua_AddLockSkillCostInfo( lua_State * L )
{
    int nSkillCount = 0;
    itemtype_t nItemType = 0;
    int nItemCount = 0;

    int nError = ParseLuaParam(L, nSkillCount, nItemType, nItemCount);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    PetConfigManager::Instance().AddLockSkillCost(nSkillCount, nItemType, nItemCount);

    return 1;
}

int lua_AddResetSkillCostInfo( lua_State * L )
{
    int nResetTimes = 0;
    int nRatio = 0;

    int nError = ParseLuaParam(L, nResetTimes, nRatio);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    PetConfigManager::Instance().AddResetSkillCost(nResetTimes, nRatio);

    return 1;
}

int lua_AddLongActingRechargeConfig( lua_State * L )
{
    bool bValid = ( lua_gettop( L ) == 6
        && lua_isnumber( L, 1)
        && lua_isnumber( L, 2)
        && lua_isstring( L, 3)
        && lua_isstring( L, 4)
        && lua_isnumber( L, 5)
        && lua_isnumber( L, 6));

    if ( !bValid )
    {
        PARAM_ERROR;
        return 0;
    }

    LongactingCumulativeRechargeConfig config;
    config.m_nRewardIndex = (unsigned int)lua_tonumber(L, 1);
    config.m_nNeedMCoin = (unsigned int)lua_tonumber(L, 2);

    std::string strMaleItem = lua_tostring(L, 3);
    ::TransferItemList(strMaleItem.c_str(), config.m_listMaleItemReward);

    std::string strFemaleItem = lua_tostring( L, 4);
    ::TransferItemList(strFemaleItem.c_str(), config.m_listFemaleItemReward);

    config.m_nMoney = (unsigned int)lua_tonumber( L, 5 );
    config.m_nBindMCoin = (unsigned int)lua_tonumber( L, 6 );

    LongactingCumulativeRechargeMgr::Instance().AddLongactingCumulativeRechargeConfig(config);

    return 1;
}

int lua_AddLongActingRechargeMail( lua_State * L )
{
    bool bValid = ( lua_gettop( L ) == 2
        && lua_isstring( L, 1)
        && lua_isstring( L, 2));

    if ( !bValid )
    {
        PARAM_ERROR;
        return 0;
    }

    std::string strMailTitle= lua_tostring( L, 1);
    std::string strMailContent = lua_tostring( L, 2);

    LongactingCumulativeRechargeMgr::Instance().AddLongactingCumulativeRechargeMail(strMailTitle, strMailContent);

    return 1;
}

int lua_SetAutoChatInfo( lua_State * L )
{
    unsigned int nInterval = 0;
    unsigned int nMaxPauseTime = 0;

    int nError = ParseLuaParam(L, nInterval, nMaxPauseTime);

    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;

        return 0;
    }

    CChatManager::Instance().SetAutoChatInfos(nInterval, nMaxPauseTime);

    return 1;
}

int lua_LoadMedalMoney(lua_State *L)
{
    unsigned int nMedalMoney = 0;
    unsigned int nVersionMedalType = 0;
    std::string strDescribe;

    bool bValid = (lua_gettop(L) == 3
        && lua_isnumber(L, 1)
        && lua_isnumber(L, 2)
        && lua_isstring(L, 3)
        );

    if (!bValid)
    {
        PARAM_ERROR;
        return 0;
    }

    nMedalMoney = (unsigned int)lua_tonumber(L, 1);
    nVersionMedalType = (unsigned int)lua_tonumber(L, 2);
    strDescribe = lua_tostring(L, 3);

    CMedalCfgMgr::Instance().SetMedalMoneyID(nMedalMoney);
    CMedalCfgMgr::Instance().SetVersionMedalType(nVersionMedalType);
    CMedalCfgMgr::Instance().SetMedalScoreDescribe(strDescribe);

    return 1;
}

int lua_AddCumulativeSpendGotMedalInfo( lua_State * L )
{
    int nExhibit = 0, nWeight = 0;
    std::string strBeginTime, strEndTime, strOpenTime, strCloseTime;
    std::string strTitle, strContent, strSmallImage, strBigImage;
    unsigned char nCumulativeType = 1;

    int nError = ParseLuaParam( L, nExhibit, nWeight, strBeginTime, strEndTime, strOpenTime, 
        strCloseTime, strTitle, strContent, strSmallImage, strBigImage, nCumulativeType);

    if ( nError != EParseLuaParam_Success )
    {
        PARAM_ERROR

        return 0;
    }

    bool bExhibit = ( nExhibit != 0 );
    unsigned int nBeginTime = SetTime( strBeginTime.c_str() );
    unsigned int nEndTime = SetTime( strEndTime.c_str() );
    unsigned int nOpenTime = SetTime( strOpenTime.c_str() );
    unsigned int nCloseTime = SetTime( strCloseTime.c_str() );

    CumulativeSpendGotMedalMgr::Instance().InitInfoByLua( bExhibit, nWeight, nBeginTime, nEndTime, 
        nOpenTime, nCloseTime, strTitle, strContent, strSmallImage, strBigImage, nCumulativeType );

    return 1;
}

int lua_AddCumulativeSpendGotMedalRewards( lua_State * L )
{
    int nIndex = 0, nRequiredNum = 0;
    std::string strMaleReward, strFemaleReward;

    int nError = ParseLuaParam( L, nIndex, nRequiredNum, strMaleReward, strFemaleReward );

    if ( nError != EParseLuaParam_Success )
    {
        PARAM_ERROR

        return 0;
    }

    std::list<int> listMaleReward;
    std::list<int> listFemaleReward;

    Split( strMaleReward, "|", listMaleReward );
    Split( strFemaleReward, "|", listFemaleReward );

    CumulativeSpendGotMedalMgr::Instance().AddRewardInfo( nIndex - 1, nRequiredNum, listMaleReward, listFemaleReward );

    return 1;
}

int lua_AddBuyItemGotMedalInfo( lua_State * L )
{
    int nExhibit = 0, nWeight = 0;
    std::string strBeginTime, strEndTime, strOpenTime, strCloseTime;
    std::string strTitle, strContent, strSmallImage, strBigImage;

    int nError = ParseLuaParam( L, nExhibit, nWeight, strBeginTime, strEndTime, strOpenTime, 
        strCloseTime, strTitle, strContent, strSmallImage, strBigImage);

    if ( nError != EParseLuaParam_Success )
    {
        PARAM_ERROR

        return 0;
    }

    bool bExhibit = ( nExhibit != 0 );
    unsigned int nBeginTime = SetTime( strBeginTime.c_str() );
    unsigned int nEndTime = SetTime( strEndTime.c_str() );
    unsigned int nOpenTime = SetTime( strOpenTime.c_str() );
    unsigned int nCloseTime = SetTime( strCloseTime.c_str() );

    BuyItemGotMedalMgr::Instance().InitInfoByLua( bExhibit, nWeight, nBeginTime, nEndTime, 
        nOpenTime, nCloseTime, strTitle, strContent, strSmallImage, strBigImage );

    return 1;
}

int lua_AddBuyItemGotMedalRewards( lua_State * L )
{
    unsigned char nIndex = 0;
    unsigned int nItemType = 0;
    unsigned short nItemCount = 0;
    std::string strMaleReward, strFemaleReward;

    int nError = ParseLuaParam( L, nIndex, nItemType, nItemCount, strMaleReward, strFemaleReward );

    if ( nError != EParseLuaParam_Success )
    {
        PARAM_ERROR

        return 0;
    }

    std::list<int> listMaleReward;
    std::list<int> listFemaleReward;

    Split( strMaleReward, "|", listMaleReward );
    Split( strFemaleReward, "|", listFemaleReward );

    BuyItemGotMedalMgr::Instance().AddRewardInfo( nIndex - 1, nItemType, 
        nItemCount, listMaleReward, listFemaleReward );

    return 1;
}


int lua_AddFixedChargeInfo(lua_State *L)
{
	unsigned int mCoin = 0;
	unsigned int giftCoin = 0;
    std::string item;
    if (EParseLuaParam_Success != ParseLuaParam(L, mCoin, giftCoin, item))
	{
		PARAM_ERROR ;
		return 0;
	}

	PaymentInfoMgr::Instance().AddFixedChargeInfo(mCoin, giftCoin, item);
	return 1; 
}

int lua_SetFixedChargeConfig(lua_State *L)
{
	std::string strStartTime;
	std::string strEndTime;

	if (EParseLuaParam_Success != ParseLuaParam(L, strStartTime, strEndTime))
	{
		PARAM_ERROR ;
		return 0;
	}

	PaymentInfoMgr::Instance().AddFixedChargeConfig((unsigned int)::SetTime(strStartTime.c_str()), (unsigned int)::SetTime(strEndTime.c_str()));

	return 1; 
}

int lua_LoadDanceGroupChallengeRandItemCount( lua_State * L )
{
    short ranking = 0;
    unsigned char count = 0;

    int nError = ParseLuaParam( L, ranking, count );

    if ( nError != EParseLuaParam_Success )
    {
        PARAM_ERROR
        return 0;
    }

    DanceGroupChallengeManager::Instance().SetRandItemCount( ranking, count );

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


int lua_LoadDanceGroupChallengeInfos( lua_State * L )
{
    unsigned char nFreeChance = 0;
    unsigned short nOpenLevel = 0;
    unsigned char nInspireCostType = 0;
    unsigned int nInspireCostAmount = 0;
    unsigned short nInspireMaxCount = 0;
    int nResetWeekday = 0, nResetHour = 0;
    unsigned int nLockTime = 0, nAnnouncementTime = 0;
    string strMailSender, strMailTitle, strMailContent, strHelp;

    int nError = ParseLuaParam( L, 
        nFreeChance, nOpenLevel, 
        nInspireCostType, nInspireCostAmount, nInspireMaxCount, 
        nResetWeekday, nResetHour, nLockTime, nAnnouncementTime, 
        strMailSender, strMailTitle, strMailContent, 
        strHelp );

    if ( nError != EParseLuaParam_Success )
    {
        PARAM_ERROR
        return 0;
    }

    Currency inspireCost;
    inspireCost.SetType( nInspireCostType );
    inspireCost.SetAmount( nInspireCostAmount );

    DanceGroupChallengeManager& mgr = DanceGroupChallengeManager::Instance();
    mgr.SetFreeChance( nFreeChance );
    mgr.SetOpenLevel( nOpenLevel );
    mgr.SetInspireInfos( inspireCost, nInspireMaxCount );
    mgr.SetResetInfos( nResetWeekday, nResetHour, nLockTime * 60, nAnnouncementTime * 60 );
    mgr.SetMailInfo( strMailSender, strMailTitle, strMailContent );
    mgr.SetHelp( strHelp );

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

int lua_SetDownloadParam(lua_State * L)
{
    int nMaxConnections = 0, nMaxBps = 0;
    int nError = ParseLuaParam(L, nMaxConnections, nMaxBps);
    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;
        return 0;
    }

    CSystemSettingMgr::Instance().SetDownloadParam(nMaxConnections, nMaxBps);
    return 1;
}

int lua_AddBagConfig(lua_State * L)
{
    int nMaxStorageSize = 0, nMinStorageCapacity = 0, nCurrencyType = 0, nCurrencyValue = 0;
    std::string strDescribe;
    int nError = ParseLuaParam(L, nMaxStorageSize, nMinStorageCapacity, nCurrencyType, nCurrencyValue, strDescribe);
    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;
        return 0;
    }

    BagConfigManager::Instance().AddBagConfig(nMaxStorageSize, nMinStorageCapacity, strDescribe, nCurrencyType, nCurrencyValue);
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


int lua_AddEffectStoneExchangeConfig(lua_State * L)
{
    std::string strBeginTime, strEndTime, strTitle, strContent, strRegularImageName, strThumbnailName;
    int nError = ParseLuaParam(L, strBeginTime, strEndTime, strTitle, strContent, strRegularImageName, strThumbnailName);
    if (nError != EParseLuaParam_Success)
    {
        PARAM_ERROR;
        return 0;
    }

    EffectStoneExchangeManager::Instance().AddActivityConfig(strTitle, strContent, (unsigned int)::SetTime(strBeginTime.c_str()), 
        (unsigned int)::SetTime(strEndTime.c_str()), strRegularImageName, strThumbnailName);

    return true;
}

int lua_AddEffectStoneExchangeItem(lua_State * L)
{
    bool bValid = (lua_gettop(L) == 2
        && lua_isstring(L, 1)
        && lua_isstring(L, 2));

    if (!bValid) 
    {
        PARAM_ERROR;
        return 0;
    }


    string strOldItem = lua_tostring(L, 1);
    string strNewItem = lua_tostring(L, 2);

    CItem olditem;
    TransferItem(strOldItem.c_str(), olditem);

    CItem newitem;
    TransferItem(strNewItem.c_str(), newitem);

    EffectStoneExchangeManager::Instance().AddExchangeItem(olditem.m_nItemType, newitem);

    return true;
}



void CInitLua::RegLuaServerConfigFuction()
{
    REGFN(ServerGetLine);
    REGFN(SetServerName);
    REGFN(GetServerPath);
    REGFN(AddMusicBoard);
    REGFN(AddFresherSong);
    REGFN(AddCmd);
    REGFN(AddGradePercent);
    REGFN(AddGradeExp);
    REGFN(AddPlaceExp);
    REGFN(AddFriendRelationInfo);
    REGFN(AddFriendRelationNameDef);
    REGFN(AddFirstRechargeInfo);
    REGFN(AddRechargeInfo);
    REGFN(AddAnnounceVersion);
    REGFN(AddAnnouncement);
    REGFN(AddAnnounceBriefInfo);
    REGFN(AddAnnounceIcons);
    REGFN(AddBGMusic);
    REGFN(AddCheckInConfig);
    REGFN(AddCheckInDescConfig);
    REGFN(ReadMoneyRstInfo);
    REGFN(AddInTimeOnlineActivity);
    REGFN(AddInTimeOnlineActivityBriefInfo);
    REGFN(ReadLevelUpRewardInfo);
    REGFN(ReadLevelUpRecommend);
    REGFN(AddExtraExpActivity);
    REGFN(AddOnlineTimeRewardBrief);
    REGFN(AddOnlineRewardInfo);

    REGFN(AddGiftInfo);
    REGFN(RoomKickSet);
    REGFN(RoomMatchSet);
    REGFN(RoomMusicSet);

    REGFN(AddExchangeItemReward);
    REGFN(AddExchangeItemBriefInfo);
    REGFN(AddMaleRecommendEquipmentInfo);
    REGFN(AddFemaleRecommendEquipmentInfo);
    REGFN(AddCumulativeSpendInfo);
    REGFN(AddCumulativeSpendRewardInfo);
    REGFN(SetMallTipInfo);
	REGFN(SetExperienceCardProp);
    REGFN(MinContributionForDemiseSet);
    REGFN(RoomRewardBaseDataSet);
    REGFN(AddRandRoomRewardCountInfo);
    REGFN(AddCertainRoomRewardInfo);
    REGFN(AddRandRoomRewardInfo);

    REGFN(AddBoxGroupInfo);
    REGFN(AddBoxItemInfo);

    REGFN(AddDynamicBoxRelatedGroupInfo);
    REGFN(AddDynamicBoxGroupInfo);
    REGFN(AddDynamicBoxItemInfo);

    REGFN(AddPlainTextActivityReward);
    REGFN(AddPlainTextActivityBriefInfo);
    REGFN(MinLevelCoupleRequiredSet);
    REGFN(MinIntimacyMarriageRequiredSet);
    REGFN(AddGiftCodeBriefInfo);
    REGFN(MinRoomRewardPlayerNumSet);
    REGFN(LuaAddVIPPrivInfo);
    REGFN(LuaAddVIPInfo);
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

    REGFN(AddXmasFreeActivityInfo);
    REGFN(AddXmasFreeRewardInfoLua);
    REGFN(AddXmasMakeWishActivityDataLua);
    REGFN(AddXmasMakeWishItemDataRawLua);
    REGFN(AddXmasMakeWishRewardDataLua);
    REGFN(AddFestivalBlessActivityInfoLua);
    REGFN(AddFestivalBlessWordsLua);
    REGFN(AddFestivalBlessRewardLua);
    REGFN(AddFestivalSendBlessRewardLua);
    REGFN(AddXmasActivityTimeLua);
    REGFN(AddPrivateQuest);
    REGFN(SetChatRequirementLua);
    REGFN(AddPhotoStudioDataLua);
    REGFN(AddPhotoStudioNotificationDataLua);	
    REGFN(AddPhotoThemeNPCInfoLua);
    REGFN(SetPhotoRoomInfoLua);
    REGFN(AddPuzzleBrief);
    REGFN(AddPuzzleDetail);
    REGFN(AddRoomCheck);
    REGFN(AddInvitationActivity);
    REGFN(AddInvitationInviterActivity);
    REGFN(AddInvitationInviterReward);
    REGFN(AddInvitationInviterAnimation);
    REGFN(AddAmuseDeviceInfo);
    REGFN(AddAmuseCarrouselEventInfo);
    REGFN(AddAmuseExpEventInfo);
    REGFN(AddAmuseSwingEventInfo);
    REGFN(AddAmuseBoatInfo);
    REGFN(AddAmuseEggInfo);
    REGFN(SetGuideMusicNameLua);
    REGFN(SetDefaultMoveType);
    REGFN(SetUpgradeInstallerLevel);
    REGFN(SetPushRatingSystem);
    REGFN(SetPushRatingLevel);
    REGFN(AddSecurityQuiz);
    REGFN(AddSecurityQuizReward);
    REGFN(AddLoadingAdInfo);
    REGFN(AddFresherActivity);
    REGFN(AddFresherActivityReward);
    REGFN(AddFresherActivityCumulationReward);
    REGFN(SetFresherQuest);
    REGFN(AddFresherQuest);
    REGFN(LoadLiveRoomConfigInfo);
    REGFN(SetLiveRoomInfo);
    REGFN(SetAuditionInfo);
    REGFN(AddAuditionScenceInfo);
    REGFN(AddAuditionModeInfo);
    REGFN(AddAuditionMusicInfo);
    REGFN(SetLiveRoomVoteInfo);
    REGFN(SetExtraActivityInfoLua);
    REGFN(AddTaikoBeckoningInfo);
    REGFN(AddTraditionBeckoningInfo);
    REGFN(AddBeckoningEffectInfo);

    REGFN(SetOpenAntiaddictionLua);
    REGFN(SetOfflineMinutesLua);
    REGFN(AddAntiaddictionConfigLua);
    REGFN(SetPlatformAntiaddictionConfigLua);

    REGFN(AddMeetRewardInfo);
    REGFN(AddMeetConfigInfo);

    // 魔法神灯
    REGFN(AddStarRewardConfig);
    REGFN(AddDustRewardConfig);
    REGFN(AddParcloseItemList);
    REGFN(AddCrystalConfig);
    REGFN(AddBuyCrystalInfoConfig);
    REGFN(AddSkipParcloseItem);
    REGFN(AddGradeExpConfig);
    REGFN(AddBaseExpConfig);
    REGFN(AddMagicLampRewardMail);
    REGFN(AddMagicLampDesc);
    REGFN(AddPublicityPictureConfig);
    REGFN(AddGiveGetVitConfig);
    REGFN(AddQuickFinishConfig);

    REGFN(AddGuideReward);
    REGFN(AddGuideContent);

    REGFN(SetBirthdayResetTimeLua);

    REGFN(SetOpenSendGift);
    REGFN(SetClothEffectPos);

    //UCVIP
    REGFN( AddPlatformPrivilege );
    REGFN( AddPlatformPrivilegeReward );

    //Packet礼包
    REGFN( AddPacketsGroup );

    //时光蛋
    REGFN( SetTimeEggSettingInfo );

    //舞团
    REGFN( AddDanceGroupTimerInfo ); //舞团每天消耗操作
    REGFN( AddStarBaseInfo );        //舞团活动（摘星星）
    REGFN( AddStarPacketItem );      //礼包信息
    REGFN( AddStarProgress );        //进度信息
    REGFN( AddDanceGroupShopInfo );  //舞团商店
    REGFN( AddEnvelopeConfig );
    REGFN( AddRedEnvelopeRechargeConfig );

    // 星恋魔法阵
    REGFN(AddConstellationMatchOpenTime);
    REGFN(AddConstellationMatchTableInfo);
    REGFN(AddConstellationMatchAmuseSceneIDInfo);
    REGFN(SetConstellationMatchInfo);

    // 搜索好友、推荐好友
    REGFN(SetFindFriendInfo);
    REGFN(AddFindFriendTypeInfo);
    REGFN(AddFindFriendLevelInfo);
    REGFN(AddFindFriendSexInfo);
    REGFN(AddFindFriendAgeInfo);
    REGFN(AddFindFriendConstellationInfo);
    REGFN(SetRecommendFriendInfo);

    //ios专有活动
    REGFN(AddProprietaryCodeInfo_Lua);
    REGFN(SetUpdateLBSPosInterval);
    REGFN(AddFunctionSwitch);
    REGFN(AddBuffIcon);
    REGFN(AddBuffCanRemove);
    REGFN(SetSocialShareInfo);
    REGFN(AddMerriageDateGift);
    REGFN(AddCoupleConfig);
    REGFN(AddMonthCardInfo);

    //分享活动
    REGFN(AddMallSocialShareActivityInfo);
    REGFN(AddMallShareRewardInfo);
    REGFN(AddMallShareTimeRangeInfo);
    REGFN(AddPhotoSocialShareActivityInfo);
    REGFN(AddPhotoShareRewardInfo);
    REGFN(AddPhotoShareTimeRangeInfo);

    // 累计充值活动
    REGFN(AddCumulativeBrief);
    REGFN(AddCummulativeRecharge);


    // 离婚
    REGFN(AddDivorceConfig);
    REGFN(AddDivorceMusicConfig);
    REGFN(AddDivorceGiftConfig);

    // 排行榜
    REGFN(LoadRankStartTimeInfo);
    REGFN( LoadRankMaxOfflineTime );
    REGFN(AddRankHallInfo);
    REGFN( LoadRankRule );
    REGFN( AddRankRewardInfo );

    // 活跃度系统
    REGFN(SetActivenessInfo);
    REGFN(AddActivenessTargetInfo);
    REGFN(AddActivenessRewardInfo);

    // 宠物系统
    REGFN(AddRandomPetInfo);
    REGFN(LoadPetSetting);
    REGFN(AddPetFeedExpItem);
    REGFN(AddPetFeedEnduranceItem);
    REGFN(AddLockSkillCostInfo);
    REGFN(AddResetSkillCostInfo);

    // 长效累冲
    REGFN(AddLongActingRechargeConfig);
    REGFN(AddLongActingRechargeMail);

    // 聊天
    REGFN(SetAutoChatInfo);

    //勋章
    REGFN(LoadMedalMoney);
    REGFN(AddCumulativeSpendGotMedalInfo);
    REGFN(AddCumulativeSpendGotMedalRewards);
    REGFN(AddBuyItemGotMedalInfo);
    REGFN(AddBuyItemGotMedalRewards);

    REGFN( AddFixedChargeInfo );
    REGFN( SetFixedChargeConfig );

    // 舞团试炼
    REGFN(LoadDanceGroupChallengeRandItemCount);
    REGFN(LoadDanceGroupChallengeInfos);

    // 动态下载参数
    REGFN(SetDownloadParam);

    // 舞团秘境
    REGFN(LoadFairlyLandConfig);

    // 背包配置
    REGFN(AddBagConfig);

    // 光效石兑换
    REGFN(AddEffectStoneExchangeConfig);
    REGFN(AddEffectStoneExchangeItem);
}

