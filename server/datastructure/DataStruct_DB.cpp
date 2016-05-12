#include "DataStruct_DB.h"



void BILLINFO::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUInt(nAccount);
	IOBuff.AddUInt(nPoint);
	IOBuff.AddUInt(nBindPoint);
	IOBuff.AddUInt(nChange);
	IOBuff.AddUInt(nQueryIdx);
	IOBuff.AddUInt(nTotalPoint);
	IOBuff.AddUInt(nTotalBindPoint);
	IOBuff.AddUInt(nTotalGift);
}

void BILLINFO::doDecode(CParamPool & IOBuff)
{
	nAccount = IOBuff.GetUInt();
	nPoint = IOBuff.GetUInt();
	nBindPoint = IOBuff.GetUInt();
	nChange = IOBuff.GetUInt();
	nQueryIdx = IOBuff.GetUInt();
	nTotalPoint = IOBuff.GetUInt();
	nTotalBindPoint = IOBuff.GetUInt();
	nTotalGift = IOBuff.GetUInt();
}

ROLEINFO_DB::ROLEINFO_DB()
{
    nAccount = 0;
    nHistoryClothNum = 0;
    nHistoryWishItemNum = 0;
    nGainWishItemNum = 0;
    nHistoryBadageNum = 0;
    nHistoryUseBouquetNum = 0;	
    nHistoryTaikoComboMusicNum = 0;
    nAutoMatchWithOppositeSexNum = 0;
    nMallSendItemCostBill = 0;
    nMallSendItemTime = 0;
    bCompleteTeachGuide = false;
    nCompleteProcGuide = 0;
    bCompleteFirstPaymentTag = false;
    bCanSendFirstPaymentReward = true;
    bReceivedInvitationReward = false;
    m_nFirstPaymentMoneyCount = 0;
    m_nFirstPaymentTime = 0;
    m_nGetFirstPaymentRewardTime = 0;
    m_nFresherBestMusicGrade = 0;
    m_nMallBuyCount = 0;
    m_bHasEnterConstellationLobby = false;
    m_nConstellationMatchCount = 0;
    m_nConstellationMatchTime = 0;
    m_nCosntellationRoomID = 0;
    m_nCosntellationRoomCreateTime = 0;
    m_nCosntellationRoomLine = 0;
    m_nVersionType = 0;
    m_bDanceGroupFirstAddPowerReward = false;
    m_nDanceGroupFirstAddPowerRewardTime = 0;
    m_nLastChangeBirthdayTime = 0;
    m_nRecommendRefreshCount = 0;
    m_nLastRecommendResetTime = 0;
    m_nDanceGroupChallengeChance = 0;
    m_nDanceGroupChallengeChanceResetTime = 0;
    m_nDanceGroupChallengeBuyChance = 0;
    m_nDanceGroupChallengeBuyChanceResetTime = 0;
    bCompleteAnimation = false;
    m_bPropretaryActivityReward = false;
    m_bReceiveProcGuideReward = false;

    m_nMedalWearScore = 0;
    m_nMedalCumulativeScore = 0;

    m_bKeyDataModified = false;
    m_nPetGuideStep = 0;
    m_nLongactingCumulativeRechargeIndex = 0;
	m_experiencedTimes = 0;
	m_experiencedStamp = 0;
	m_nGuideData = 0;
    m_bRedEnvelopeAnonymity = false;
}

ROLEINFO_DB::~ROLEINFO_DB()
{
}

void ROLEINFO_DB::doEncode(CParamPool &IOBuff)
{
	RoleAttrInfo::doEncode(IOBuff);
	IOBuff.AddUInt(nAccount);

	IOBuff.AddUShort((unsigned short)listItem.size());
	for (list<DBItem>::iterator itr = listItem.begin(); itr != listItem.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddUShort((unsigned short)listFriend.size());
	for (list<FriendInfo>::iterator itr =listFriend.begin(); itr != listFriend.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddUShort((unsigned short)listBlackList.size());
	for (list<FriendInfo>::iterator itr =listBlackList.begin(); itr != listBlackList.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

// 	IOBuff.AddUShort((unsigned short)listQuest.size());
// 	for (list<QUESTINFO>::iterator itr =listQuest.begin(); itr != listQuest.end(); ++itr) {
// 		itr->doEncode(IOBuff);
// 	}

// 	IOBuff.AddUShort((unsigned short)mapQuestHistory.size());
// 	for (map<short, __int64>::iterator itr = mapQuestHistory.begin(); itr != mapQuestHistory.end(); ++itr) {
// 		IOBuff.AddShort(itr->first);
// 		IOBuff.AddInt64(itr->second);
// 	}

// 	IOBuff.AddUShort((unsigned short)listRepeatedQuest.size());
// 	for (list<CRepeatedHistoryQuest>::iterator itr = listRepeatedQuest.begin(); itr != listRepeatedQuest.end(); ++itr) {
// 		itr->doEncode(IOBuff);
// 	}

	IOBuff.AddUShort((unsigned short)listCoolDown.size());
	for (list<CoolDownData>::iterator itr = listCoolDown.begin(); itr != listCoolDown.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddUShort((unsigned short)listGene.size());
	for (list<GeneData>::iterator itr = listGene.begin(); itr != listGene.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddUShort((unsigned short)listTitle.size());
	for (list<CTitleInfo>::iterator itr = listTitle.begin(); itr != listTitle.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddUShort((unsigned short)listAchievement.size());
	for (list<CAchievementInfo>::iterator itr = listAchievement.begin(); itr != listAchievement.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddInt(nHistoryClothNum);
	IOBuff.AddInt(nHistoryWishItemNum);
	IOBuff.AddInt(nGainWishItemNum);
	IOBuff.AddInt(nHistoryBadageNum);
	IOBuff.AddInt(nHistoryUseBouquetNum);

	bigSpeakersUseNumWeekly.doEncode(IOBuff);
	smallSpeakersUseNumWeekly.doEncode(IOBuff);
	sendFlowersNumInSevenWeekly.doEncode(IOBuff);

	IOBuff.AddUShort(nHistoryTaikoComboMusicNum);
	IOBuff.AddInt(nAutoMatchWithOppositeSexNum);

	IOBuff.AddUShort((unsigned short)mapFightResult.size());
	if (mapFightResult.size() > 0) {
		for (map<int, map<int, CFightResult > >::iterator itr = mapFightResult.begin(); itr != mapFightResult.end(); ++itr) {
			IOBuff.AddUShort((unsigned short)itr->second.size());
			if (itr->second.size() > 0) {
				for (map< int, CFightResult >::iterator rItr = itr->second.begin(); rItr != itr->second.end(); ++rItr) {
					rItr->second.doEncode(IOBuff);	
				}
			}
		}
	}

	checkInInfo.doEncode(IOBuff);

// 	IOBuff.AddUShort((unsigned short)listCheckInGift.size());
// 	for (list<unsigned char>::iterator itr = listCheckInGift.begin(); itr != listCheckInGift.end(); ++itr) {
// 		IOBuff.AddUChar(*itr);
// 	}

	VIPInfo.doEncodeToGame(IOBuff);

	IOBuff.AddUShort((unsigned short)mapVIPCards.size());
	if (mapVIPCards.size() > 0) {
		for (map<unsigned char, itemtype_t>::iterator itr = mapVIPCards.begin(); itr != mapVIPCards.end(); ++itr) {
			IOBuff.AddUChar(itr->first);
			IOBuff.AddUInt(itr->second);
		}
	}

	bi.doEncode(IOBuff);
	IOBuff.AddBool(bCompleteTeachGuide);
	IOBuff.AddBool(bCompleteFirstPaymentTag);
	IOBuff.AddBool(bCanSendFirstPaymentReward);
	IOBuff.AddBool(bReceivedInvitationReward);
	IOBuff.AddBool( m_bDanceGroupFirstAddPowerReward );
	IOBuff.AddBool( m_bPropretaryActivityReward );

	IOBuff.AddUShort((unsigned short)listQuestNew.size());
	for (list<QUESTNEWINFO>::iterator itr = listQuestNew.begin() ; itr != listQuestNew.end() ; itr ++ )
	{
		itr->doEncode(IOBuff);
	}
	IOBuff << m_listClothGroup;
}

void ROLEINFO_DB::doDecode(CParamPool &IOBuff)
{
	RoleAttrInfo::doDecode(IOBuff);
	
	int nSize = 0;

	nAccount = IOBuff.GetUInt();

	nSize = IOBuff.GetUShort(); 
	while (nSize-- > 0) {
		DBItem info;
		info.doDecode(IOBuff);
		listItem.push_back(info);
	}

	nSize = IOBuff.GetUShort(); 
	while (nSize-- > 0) {
		FriendInfo info;
		info.doDecode(IOBuff);
		listFriend.push_back(info);
	}

	nSize = IOBuff.GetUShort(); 
	while (nSize-- > 0) {
		FriendInfo info;
		info.doDecode(IOBuff);
		listBlackList.push_back(info);
	}

// 	nSize = IOBuff.GetUShort(); 
// 	while (nSize-- > 0) {
// 		QUESTINFO info;
// 		info.doDecode(IOBuff);
// 		listQuest.push_back(info);
// 	}

// 	nSize = IOBuff.GetUShort();
// 	while (nSize-- > 0) {
// 		short nFirst = IOBuff.GetShort();
// 		__int64 nSecond = IOBuff.GetInt64();
// 		mapQuestHistory.insert(make_pair(nFirst, nSecond));
// 	}

// 	nSize = IOBuff.GetUShort(); 
// 	while (nSize-- > 0) {
// 		CRepeatedHistoryQuest info;
// 		info.doDecode(IOBuff);
// 		listRepeatedQuest.push_back(info);
// 	}

	nSize = IOBuff.GetUShort(); 
	while (nSize-- > 0) {
		CoolDownData info;
		info.doDecode(IOBuff);
		listCoolDown.push_back(info);
	}

	nSize = IOBuff.GetUShort(); 
	while (nSize-- > 0) {
		GeneData info;
		info.doDecode(IOBuff);
		listGene.push_back(info);
	}

	nSize = IOBuff.GetUShort(); 
	while (nSize-- > 0) {
		CTitleInfo info;
		info.doDecode(IOBuff);
		listTitle.push_back(info);
	}

	nSize = IOBuff.GetUShort(); 
	while (nSize-- > 0) {
		CAchievementInfo info;
		info.doDecode(IOBuff);
		listAchievement.push_back(info);
	}

	nHistoryClothNum = IOBuff.GetUInt();
	nHistoryWishItemNum = IOBuff.GetUInt();
	nGainWishItemNum = IOBuff.GetUInt();
	nHistoryBadageNum = IOBuff.GetUInt();
	nHistoryUseBouquetNum = IOBuff.GetUInt();

	bigSpeakersUseNumWeekly.doDecode(IOBuff);
	smallSpeakersUseNumWeekly.doDecode(IOBuff);
	sendFlowersNumInSevenWeekly.doDecode(IOBuff);

	nHistoryTaikoComboMusicNum = IOBuff.GetUShort();
	nAutoMatchWithOppositeSexNum = IOBuff.GetUInt();

	nSize = IOBuff.GetUShort();
	while (nSize-- > 0) {
		int nSecondSize = IOBuff.GetUShort();
		map< int, CFightResult > mapResult;
		CFightResult result;	
		while (nSecondSize-- > 0) {
			result.doDecode(IOBuff);
			mapResult.insert(make_pair(result.m_nDifficulty, result));
		}
		mapFightResult.insert(make_pair(result.m_nModeID, mapResult));
	}

	checkInInfo.doDecode(IOBuff);

// 	nSize = IOBuff.GetUShort();
// 	while (nSize-- > 0) {
// 		listCheckInGift.push_back(IOBuff.GetUChar());
// 	}

	VIPInfo.doDecodeFromGame(IOBuff);

	nSize = IOBuff.GetUShort();
	while (nSize-- > 0) {
		unsigned char nType = IOBuff.GetUChar();
		itemtype_t nItemID = IOBuff.GetUInt();
		mapVIPCards.insert(make_pair(nType, nItemID));
	}

	bi.doDecode(IOBuff);
	bCompleteTeachGuide = IOBuff.GetBool();
	bCompleteFirstPaymentTag = IOBuff.GetBool();
	bCanSendFirstPaymentReward = IOBuff.GetBool();
	bReceivedInvitationReward = IOBuff.GetBool();
	m_bDanceGroupFirstAddPowerReward = IOBuff.GetBool();
	m_bPropretaryActivityReward = IOBuff.GetBool();

	nSize = IOBuff.GetUShort();
	while(nSize-- > 0)
	{
		QUESTNEWINFO questNewInfo;
		questNewInfo.doDecode(IOBuff);
		listQuestNew.push_back(questNewInfo);
	}
	IOBuff >> m_listClothGroup;
}



bool ROLEINFO_DB::CacheEncodeQuestNew(CParamPool &IOBuff)
{
    bool bret = true;

    bret &= doEncodeQuestNew( IOBuff );

    return bret;
}

void ROLEINFO_DB::CacheDecodeQuestNew(CParamPool &IOBuff)
{
    doDecodeQuestNew( IOBuff );
}

bool ROLEINFO_DB::CacheEncodeItem( CParamPool &IOBuff )
{
    bool res = true;

    res &= doEncodeStorageCapacity( IOBuff );
    res &= doEncodeClothGroup( IOBuff );
    res &= doEncodeItem( IOBuff );
    res &= doEncodeCoolDown( IOBuff );
    res &= doEncodeGene( IOBuff );

    return res;
}

void ROLEINFO_DB::CacheDecodeItem( CParamPool &IOBuff )
{
    doDecodeStorageCapacity( IOBuff );
    doDecodeClothGroup( IOBuff );
    doDecodeItem( IOBuff );
    doDecodeCoolDown( IOBuff );
    doDecodeGene( IOBuff );
}

bool ROLEINFO_DB::CacheEncodeClothEffect( CParamPool &IOBuff )
{
    bool res = true;

    res &= doEncodeClothEffect( IOBuff );

    return res;
}

void ROLEINFO_DB::CacheDecodeClothEffect( CParamPool &IOBuff )
{
    doDecodeClothEffect( IOBuff );
}

bool ROLEINFO_DB::CacheEncodeSettings( CParamPool &IOBuff )
{
    bool res = true;

    res &= doEncodePlayerSetting( IOBuff );
    res &= doEncodeCheckIn( IOBuff );
    res &= doEncodeMonthCard( IOBuff );

    return res;
}

void ROLEINFO_DB::CacheDecodeSettings( CParamPool &IOBuff )
{
    doDecodePlayerSetting( IOBuff );
    doDecodeCheckIn( IOBuff );
    doDecodeMonthCard( IOBuff );
}

bool ROLEINFO_DB::CacheEncodeActivity( CParamPool &IOBuff )
{
    bool res = true;

    res &= doEncodeSocialShare( IOBuff );
    res &= doEncodePuzzle( IOBuff );
    res &= doEncodeGift( IOBuff );
    res &= doEncodeKeepOnlineReward( IOBuff );

    return res;
}

void ROLEINFO_DB::CacheDecodeActivity( CParamPool &IOBuff )
{
    doDecodeSocialShare( IOBuff );
    doDecodePuzzle( IOBuff );
    doDecodeGift( IOBuff );
    doDecodeKeepOnlineReward( IOBuff );
}

bool ROLEINFO_DB::CacheEncodeOther( CParamPool &IOBuff )
{
    bool res = true;

    res &= doEncodeExperienced( IOBuff );
    res &= doEncodeMagicArray( IOBuff );
    res &= doEncodeAchievment( IOBuff );
    res &= doEncodeInfoCenter( IOBuff );
    res &= doEncodeActiveness( IOBuff );
    res &= doEncodeDesiredItem( IOBuff );
    res &= doEncodePet( IOBuff );
    res &= doEncodeHandbook( IOBuff );
    res &= doEncodeCompensation( IOBuff );
    //res &= doEncodeMedal( IOBuff );
    res &= doEncodeMagicLamp( IOBuff );
    res &= doEncodeBigMamaRoleInfo(IOBuff);

    return res;
}

void ROLEINFO_DB::CacheDecodeOther( CParamPool &IOBuff )
{
    doDecodeExperienced( IOBuff );
    doDecodeMagicArray( IOBuff );
    doDecodeAchievment( IOBuff );
    doDecodeInfoCenter( IOBuff );
    doDecodeActiveness( IOBuff );
    doDecodeDesiredItem( IOBuff );
    doDecodePet( IOBuff );
    doDecodeHandbook( IOBuff );
    doDecodeCompensation( IOBuff );
    //doDecodeMedal( IOBuff );
    doDecodeMagicLamp( IOBuff );
    doDecodeBigMamaRoleInfo(IOBuff);

    return;
}

const ROLEINFO_DB::CacheStruct ROLEINFO_DB::m_cacheFuncMap[ERoleCacheInfoEnumMax] =
{
    CacheStruct( ERoleInfoCacheQuestNew, &ROLEINFO_DB::CacheEncodeQuestNew, &ROLEINFO_DB::CacheDecodeQuestNew ),
    CacheStruct( ERoleInfoCacheItem, &ROLEINFO_DB::CacheEncodeItem, &ROLEINFO_DB::CacheDecodeItem ),
    CacheStruct( ERoleInfoCacheClothEffect, &ROLEINFO_DB::CacheEncodeClothEffect, &ROLEINFO_DB::CacheDecodeClothEffect ),
    CacheStruct( ERoleInfoCacheSettings, &ROLEINFO_DB::CacheEncodeSettings, &ROLEINFO_DB::CacheDecodeSettings ),
    CacheStruct( ERoleInfoCacheActivity, &ROLEINFO_DB::CacheEncodeActivity, &ROLEINFO_DB::CacheDecodeActivity ),
    CacheStruct( ERoleInfoCacheOther, &ROLEINFO_DB::CacheEncodeOther, &ROLEINFO_DB::CacheDecodeOther ),
};

ROLEINFO_DB::CacheEncodeFunctor ROLEINFO_DB::GetCachEncode(ERoleInfoCache key)
{
    if (key >= ERoleCacheInfoEnumMax)
        return NULL;

    return m_cacheFuncMap[key].efun;
}

ROLEINFO_DB::CacheDecodeFunctor ROLEINFO_DB::GetCacheDecode(ERoleInfoCache key)
{
    if (key >= ERoleCacheInfoEnumMax)
        return NULL;

    return m_cacheFuncMap[key].dfun;
}

void ROLEINFO_DB::SetCachedFlag( size_t nFlag )
{
    if ( nFlag < m_cachedFlags.size() )
        m_cachedFlags.set( nFlag );
}

bool ROLEINFO_DB::IsFlagSet( size_t nFlag )
{
    return nFlag < m_cachedFlags.size() ? m_cachedFlags.test( nFlag ) : false;
}

bool ROLEINFO_DB::doEncodeQuestNew( CParamPool &IOBuff ) const
{
    return ::doEncode( IOBuff, listQuestNew );
}

bool ROLEINFO_DB::doDecodeQuestNew( CParamPool &IOBuff )
{
    return ::doDecode( IOBuff, listQuestNew );
}

bool ROLEINFO_DB::doEncodeStorageCapacity( CParamPool &IOBuff ) const
{
    return m_StorageCapacity.doEncode( IOBuff );
}

bool ROLEINFO_DB::doDecodeStorageCapacity( CParamPool &IOBuff )
{
    return m_StorageCapacity.doDecode( IOBuff );
}

bool ROLEINFO_DB::doEncodeClothGroup( CParamPool &IOBuff ) const
{
    return ::doEncode( IOBuff, m_listClothGroup );
}

bool ROLEINFO_DB::doDecodeClothGroup( CParamPool &IOBuff )
{
    return ::doDecode( IOBuff, m_listClothGroup );
}

bool ROLEINFO_DB::doEncodeItem( CParamPool &IOBuff ) const
{
    return ::doEncode( IOBuff, listItem );
}

bool ROLEINFO_DB::doDecodeItem( CParamPool &IOBuff )
{
    return ::doDecode( IOBuff, listItem );
}

bool ROLEINFO_DB::doEncodeCoolDown( CParamPool &IOBuff ) const
{
    return ::doEncode( IOBuff, listCoolDown );
}

bool ROLEINFO_DB::doDecodeCoolDown( CParamPool &IOBuff )
{
    return ::doDecode( IOBuff, listCoolDown );
}

bool ROLEINFO_DB::doEncodeGene( CParamPool &IOBuff ) const
{
    return ::doEncode( IOBuff, listGene );
}

bool ROLEINFO_DB::doDecodeGene( CParamPool &IOBuff )
{
    return ::doDecode( IOBuff, listGene );
}

bool ROLEINFO_DB::doEncodeClothEffect( CParamPool &IOBuff ) const
{
    bool res = true;

    res &= ::doEncode( IOBuff, mapClothEffect );
    res &= ::doEncode( IOBuff, setRoleEffect );
    res &= ::doEncode( IOBuff, mapHandbookProgress );
    res &= ::doEncode( IOBuff, mapColorProgress );

    return res;
}

bool ROLEINFO_DB::doDecodeClothEffect( CParamPool &IOBuff )
{
    ::doDecode( IOBuff, mapClothEffect );
    ::doDecode( IOBuff, setRoleEffect );
    ::doDecode( IOBuff, mapHandbookProgress );
    ::doDecode( IOBuff, mapColorProgress );

    return true;
}

bool ROLEINFO_DB::doEncodePlayerSetting( CParamPool &IOBuff ) const
{
    return m_playerSetting.doEncode( IOBuff );
}

bool ROLEINFO_DB::doDecodePlayerSetting( CParamPool &IOBuff )
{
    return m_playerSetting.doDecode( IOBuff );
}

bool ROLEINFO_DB::doEncodeCheckIn( CParamPool &IOBuff ) const
{
    return checkInInfo.doEncode( IOBuff );
}

bool ROLEINFO_DB::doDecodeCheckIn( CParamPool &IOBuff )
{
    return checkInInfo.doDecode( IOBuff );
}

bool ROLEINFO_DB::doEncodeMonthCard( CParamPool &IOBuff ) const
{
    return ::doEncode( IOBuff, m_mapMonthCard );
}

bool ROLEINFO_DB::doDecodeMonthCard( CParamPool &IOBuff )
{
    return ::doDecode( IOBuff, m_mapMonthCard );
}

bool ROLEINFO_DB::doEncodeSocialShare( CParamPool &IOBuff ) const
{
    return ::doEncode( IOBuff, m_mapSocialShareFirstTime );
}

bool ROLEINFO_DB::doDecodeSocialShare( CParamPool &IOBuff )
{
    return ::doDecode( IOBuff, m_mapSocialShareFirstTime );
}

bool ROLEINFO_DB::doEncodePuzzle( CParamPool &IOBuff ) const
{
    return ::doEncode( IOBuff, m_listPuzzleActivityDB );
}

bool ROLEINFO_DB::doDecodePuzzle( CParamPool &IOBuff )
{
    return ::doDecode( IOBuff, m_listPuzzleActivityDB );
}

bool ROLEINFO_DB::doEncodeGift( CParamPool &IOBuff ) const
{
    return ::doEncode( IOBuff, m_mapGift );
}

bool ROLEINFO_DB::doDecodeGift( CParamPool &IOBuff )
{
    return ::doDecode( IOBuff, m_mapGift );
}

bool ROLEINFO_DB::doEncodeKeepOnlineReward( CParamPool &IOBuff ) const
{
    return keepOnlineRewardInfo.doEncodeToCache( IOBuff );
}

bool ROLEINFO_DB::doDecodeKeepOnlineReward( CParamPool &IOBuff )
{
    return keepOnlineRewardInfo.doDecodeFromCache( IOBuff );
}

bool ROLEINFO_DB::doEncodeExperienced( CParamPool &IOBuff ) const
{
    bool res = true;

    res &= ::doEncode( IOBuff, m_vecExperiencedProducts );
    res &= IOBuff.AddUShort( m_experiencedTimes );
    res &= IOBuff.AddUInt( m_experiencedStamp );

    return res;
}

bool ROLEINFO_DB::doDecodeExperienced( CParamPool &IOBuff )
{
    ::doDecode( IOBuff, m_vecExperiencedProducts );
    m_experiencedTimes = IOBuff.GetUShort();
    m_experiencedStamp = IOBuff.GetUInt();

    return true;
}

bool ROLEINFO_DB::doEncodeMagicArray( CParamPool &IOBuff ) const
{
    bool res = true;

    res &= ::doEncode( IOBuff, m_mapMagicArrayRecord );
    res &= m_mapMagicArrayExchangeRecord.doEncode( IOBuff );

    return res;
}

bool ROLEINFO_DB::doDecodeMagicArray( CParamPool &IOBuff )
{
    ::doDecode( IOBuff, m_mapMagicArrayRecord );
    m_mapMagicArrayExchangeRecord.doDecode( IOBuff );

    return true;
}

bool ROLEINFO_DB::doEncodeAchievment( CParamPool &IOBuff ) const
{
    bool res = true;

    res &= ::doEncode( IOBuff, mapFightResult );
    res &= ::doEncode( IOBuff, listTitle );
    res &= ::doEncode( IOBuff, listAchievement );

    return res;
}

bool ROLEINFO_DB::doDecodeAchievment( CParamPool &IOBuff )
{
    ::doDecode( IOBuff, mapFightResult );
    ::doDecode( IOBuff, listTitle );
    ::doDecode( IOBuff, listAchievement );

    return true;
}

bool ROLEINFO_DB::doEncodeInfoCenter( CParamPool &IOBuff ) const
{
    IOBuff.AddUShort( (unsigned short)m_vecInfoCenterReaded.size() );
    for ( std::vector<std::pair<unsigned int, unsigned int> >::const_iterator itr = m_vecInfoCenterReaded.begin();
        itr != m_vecInfoCenterReaded.end(); ++itr )
    {
        IOBuff.AddUInt( itr->first );
        IOBuff.AddUInt( itr->second );
    }

    return true;
}

bool ROLEINFO_DB::doDecodeInfoCenter( CParamPool &IOBuff )
{
    unsigned short count = IOBuff.GetUShort();
    for ( unsigned short i = 0; i < count; ++i )
    {
        std::pair<unsigned int, unsigned int> p;
        p.first = IOBuff.GetUInt();
        p.second = IOBuff.GetUInt();
        m_vecInfoCenterReaded.push_back( p );
    }

    return true;
}

bool ROLEINFO_DB::doEncodeActiveness( CParamPool &IOBuff ) const
{
    return m_activeness.doEncode( IOBuff );
}

bool ROLEINFO_DB::doDecodeActiveness( CParamPool &IOBuff )
{
    return m_activeness.doDecode( IOBuff );
}

bool ROLEINFO_DB::doEncodeDesiredItem( CParamPool &IOBuff ) const
{
    return ::doEncode( IOBuff, m_listDesiredItem );
}

bool ROLEINFO_DB::doDecodeDesiredItem( CParamPool &IOBuff )
{
    return ::doDecode( IOBuff, m_listDesiredItem );
}

bool ROLEINFO_DB::doEncodePet( CParamPool &IOBuff ) const
{
    IOBuff.AddUShort( (unsigned short)m_mapPet.size() );
    for ( std::map<unsigned int, PetDB, std::greater<unsigned int> >::const_iterator itr = m_mapPet.begin();
        itr != m_mapPet.end(); ++itr )
    {
        itr->second.doEncode( IOBuff );
    }

    return true;
}

bool ROLEINFO_DB::doDecodePet( CParamPool &IOBuff )
{
    unsigned short count = IOBuff.GetUShort();
    for ( unsigned short i = 0; i < count; ++i )
    {
        PetDB pet;

        if ( pet.doDecode( IOBuff ) )
        {
            m_mapPet.insert( std::make_pair( pet.nID, pet ) );
        }
    }

    return true;
}

bool ROLEINFO_DB::doEncodeHandbook( CParamPool &IOBuff ) const
{
    bool res = true;

    res &= ::doEncode( IOBuff, m_MapCostumePersonalInfo );
    res &= ::doEncode( IOBuff, m_MapEffectPersonalInfo );

    return res;
}

bool ROLEINFO_DB::doDecodeHandbook( CParamPool &IOBuff )
{
    ::doDecode( IOBuff, m_MapCostumePersonalInfo );
    ::doDecode( IOBuff, m_MapEffectPersonalInfo );

    return true;
}

bool ROLEINFO_DB::doEncodeCompensation( CParamPool &IOBuff ) const
{
    return ::doEncode( IOBuff, m_CompensationReward );
}

bool ROLEINFO_DB::doDecodeCompensation( CParamPool &IOBuff )
{
    return ::doDecode( IOBuff, m_CompensationReward );
}

bool ROLEINFO_DB::doEncodeMedal( CParamPool &IOBuff ) const
{
    bool res = true;

    res &= ::doEncode( IOBuff, m_mapMedalUnit );
    res &= ::doEncode( IOBuff, m_mapMedalCondition );
    res &= ::doEncode( IOBuff, m_nMedalWearScore );
    res &= ::doEncode( IOBuff, m_nMedalCumulativeScore );

    return res;
}

bool ROLEINFO_DB::doDecodeMedal( CParamPool &IOBuff )
{
    ::doDecode( IOBuff, m_mapMedalUnit );
    ::doDecode( IOBuff, m_mapMedalCondition );
    ::doDecode( IOBuff, m_nMedalWearScore );
    ::doDecode( IOBuff, m_nMedalCumulativeScore );
    return true;
}

bool ROLEINFO_DB::doEncodeMagicLamp( CParamPool &IOBuff ) const
{
    bool res = true;

    res &= ::doEncode( IOBuff, m_MagicLamp );
    res &= ::doEncode( IOBuff, m_MainlineParcloseList );
    res &= ::doEncode( IOBuff, m_ThemeParcloseList );
    res &= ::doEncode( IOBuff, m_LuckStarReward );
    res &= ::doEncode( IOBuff, m_PlayerDebrisInfo );

    return res;
}

bool ROLEINFO_DB::doDecodeMagicLamp( CParamPool &IOBuff )
{
    ::doDecode( IOBuff, m_MagicLamp );
    ::doDecode( IOBuff, m_MainlineParcloseList );
    ::doDecode( IOBuff, m_ThemeParcloseList );
    ::doDecode( IOBuff, m_LuckStarReward );
    ::doDecode( IOBuff, m_PlayerDebrisInfo );

    return true;
}

bool ROLEINFO_DB::doEncodeBigMamaRoleInfo(CParamPool &IOBuff) const
{
    bool res = true;
    res &= IOBuff.AddUInt(m_bigMamaRoleInfo.m_nPrizeStamp);
    res &= IOBuff.AddUInt(m_bigMamaRoleInfo.m_nJoinStamp);

    return res;
}

bool ROLEINFO_DB::doDecodeBigMamaRoleInfo(CParamPool &IOBuff)
{
    m_bigMamaRoleInfo.m_nPrizeStamp = IOBuff.GetUInt();
    m_bigMamaRoleInfo.m_nJoinStamp = IOBuff.GetUInt();

    return true;
}

bool CoolDownData::doEncode(CParamPool &IOBuff)const
{
	IOBuff.AddUShort(m_nType);
	IOBuff.AddInt(m_nDuration);
	return true;
}

bool CoolDownData::doDecode(CParamPool &IOBuff)
{
	m_nType = IOBuff.GetUShort();
	m_nDuration = IOBuff.GetInt();
	return true;
}

bool GeneData::doEncode(CParamPool & IOBuff) const
{
	IOBuff.AddUShort(nGeneId);	
	IOBuff.AddUChar(nType);
	IOBuff.AddInt(nParam1);
	IOBuff.AddInt(nParam2);
	IOBuff.AddStringW(szCmdParam);

    return true;
}

bool GeneData::doDecode(CParamPool & IOBuff)
{
	nGeneId = IOBuff.GetUShort();
	nType = IOBuff.GetUChar();
	nParam1 = IOBuff.GetInt();
	nParam2 = IOBuff.GetInt();

	string strCmdParam = "";
	IOBuff.GetStringW( strCmdParam );
	if (strCmdParam.size() > MAX_GENE_PARAM_LEN - 1) {
		strCmdParam.resize(MAX_GENE_PARAM_LEN - 1);
	}
	memcpy( szCmdParam, strCmdParam.c_str(), strCmdParam.size() );

    return true;
}


MallGood::MallGood()
{
    m_nType = 0;
    m_nState = 0;
    m_nDiscount = 100;
    m_nIsVip = 0;
    m_nIsPrecious = 0;
    m_nIsNew = 0;
    m_nHot = 0;
    m_nIsCoupon = 0;
    m_nUnique = 0;
    m_nPayType = 0;
    m_nUseChangePercent = 0;

    m_nPrice = 0;
    m_nPrice1 = 0;
    m_nPrice2 = 0;
    m_nPrice3 = 0;

    m_nSell_time0 = 0;
    m_nSell_time1 = 0;
    m_nDiscount_time0 = 0;
    m_nDiscount_time1 = 0;

    m_nDesireCount = 0;
    m_nHotIdx = 0;
}

MallGood& MallGood::operator = (const MallGood& that)
{
    if(this == &that)
        return *this;

    m_nType = that.m_nType;
    m_nState = that.m_nState;
    m_nDiscount = that.m_nDiscount;
    m_nIsVip = that.m_nIsVip;
    m_nIsPrecious = that.m_nIsPrecious;
    m_nIsNew = that.m_nIsNew;
    m_nHot = that.m_nHot;
    m_nIsCoupon = that.m_nIsCoupon;
    m_nUnique = that.m_nUnique;
    m_nPayType = that.m_nPayType;
    m_nUseChangePercent = that.m_nUseChangePercent;
    m_nPrice = that.m_nPrice;
    m_nPrice1 = that.m_nPrice1;
    m_nPrice2 = that.m_nPrice2;
    m_nPrice3 = that.m_nPrice3;

    m_nSell_time0 = that.m_nSell_time0;
    m_nSell_time1 = that.m_nSell_time1;
    m_nDiscount_time0 = that.m_nDiscount_time0;
    m_nDiscount_time1 = that.m_nDiscount_time1;

    m_nDesireCount = that.m_nDesireCount;
    m_nHotIdx = that.m_nHotIdx;
    return *this;
};

bool MallGood::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nType);
    IOBuff.AddUChar(m_nState);
    IOBuff.AddUChar(m_nDiscount);
    IOBuff.AddUChar(m_nIsVip);
    IOBuff.AddUChar(m_nIsPrecious);
    IOBuff.AddUChar(m_nIsNew);
    IOBuff.AddUChar(m_nHot);
    IOBuff.AddUChar(m_nIsCoupon);
    IOBuff.AddUChar(m_nUnique);
    IOBuff.AddUChar(m_nPayType);
    IOBuff.AddUChar(m_nUseChangePercent);
    IOBuff.AddUInt(m_nPrice);
    IOBuff.AddUInt(m_nPrice1);
    IOBuff.AddUInt(m_nPrice2);
    IOBuff.AddUInt(m_nPrice3);

    return true;
}

void MallGood::AddDesireCount(int nAddCount)
{
    if (m_nState == 1)//商城贩卖该商品时才增加许愿次数
    {
        m_nDesireCount += nAddCount;
    }
}

int MallGood::DesireCount() const
{
    return m_nDesireCount;
}

//////////////////////////////////////////////////////////////////////////

FriendInfo::FriendInfo()
: m_nRoleID(0)
, m_nSex(ESexType_Male)
, m_strName("")
, m_strNotes("")
, m_nLevel(0)
, m_nRelationType(0)
, m_nCallBy(0)
, m_nCallTo(0)
, m_bIsVIP(false)
, m_nVIPLevel(0)
, m_bIsOnline(false)
, m_nOfflineTime(0)
{
}

FriendInfo::FriendInfo( unsigned int nRoleID )
    : m_nRoleID( nRoleID )
    , m_nSex( ESexType_No )
    , m_strName( "" )
    , m_strNotes( "" )
    , m_nLevel( 0 )
    , m_nRelationType( 0 )
    , m_nCallBy( 0 )
    , m_nCallTo( 0 )
    , m_bIsVIP( false )
    , m_nVIPLevel( 0 )
{
}

FriendInfo::FriendInfo( const FriendInfo &info )
    : m_nRoleID(info.m_nRoleID)
    , m_nSex(info.m_nSex)
    , m_strName(info.m_strName)
    , m_strNotes(info.m_strNotes)
    , m_nLevel(info.m_nLevel)
    , m_nRelationType(info.m_nRelationType)
    , m_nCallBy(info.m_nCallBy)
    , m_nCallTo(info.m_nCallTo)
    , m_bIsVIP(info.m_bIsVIP)
    , m_nVIPLevel(info.m_nVIPLevel)
    , m_bIsOnline(info.m_bIsOnline)
    , m_nOfflineTime(0)
{
}

FriendInfo::FriendInfo(unsigned int nRoleID, const std::string &rRoleName, const std::string &rNotes,
    unsigned char nSex, unsigned short nLeve, unsigned char nRelationType, unsigned char nCallBy,
    unsigned char nCallTo, bool bIsVIP, unsigned short nVIPLevel)
    : m_nRoleID(nRoleID)
    , m_nSex(nSex)
    , m_strName(rRoleName)
    , m_strNotes(rNotes)
    , m_nLevel(nLeve)
    , m_nRelationType(nRelationType)
    , m_nCallBy(nCallBy)
    , m_nCallTo(nCallTo)
    , m_bIsVIP(bIsVIP)
    , m_nVIPLevel(nVIPLevel)
{
    SetName(rRoleName);
}

bool FriendInfo::operator == ( const FriendInfo &rhs ) const
{
    if ( this == &rhs )
    {
        return true;
    }
    else
    {
        return rhs.m_nRoleID == m_nRoleID && m_nCallBy == rhs.m_nCallBy && m_nCallTo == rhs.m_nCallTo;
    }
}

bool FriendInfo::FindByID( unsigned int nRoleID ) const
{
    return m_nRoleID == nRoleID;
}

bool FriendInfo::FindByName( const std::string &rRoleName ) const
{
    return m_strName == rRoleName;
}

void FriendInfo::SetName( const std::string &rRoleName )
{
    m_strName = rRoleName;

    if ( m_strName.size() > MAX_NAME_LEN - 1 )
        m_strName.resize( MAX_NAME_LEN - 1 );
}

void FriendInfo::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nRoleID << stringw( m_strName ) << stringw( m_strNotes ) 
        << m_nSex << m_nLevel << m_nRelationType << m_nCallBy 
        << m_nCallTo << m_bIsVIP << (unsigned short)m_nVIPLevel;
}

void FriendInfo::doDecode( CParamPool &IOBuff )
{
    stringw strRoleName;
    stringw strNotes;
    unsigned short nVIPLevel;

    IOBuff >> m_nRoleID >> strRoleName >> strNotes >> m_nSex 
        >> m_nLevel >> m_nRelationType >> m_nCallBy 
        >> m_nCallTo >> m_bIsVIP >> nVIPLevel;

    SetName( strRoleName );
    m_strNotes = strNotes;
    m_nVIPLevel = nVIPLevel;
}

//////////////////////////////////////////////////////////////////////////

MailBaseInfo::MailBaseInfo()
{
    m_nMailID = 0;
    m_nType = 0;
    m_strSenderName.clear();
    m_strTitle.clear();
    m_bRead = 0;
    m_nHasItem = 0;
    m_nMoney = 0;
    m_nBindMCoin = 0;
    m_nSenderID = 0;
}

MailBaseInfo::MailBaseInfo(const MailBaseInfo &rMailBaseInfo)
{
    *this = rMailBaseInfo;
}

MailBaseInfo& MailBaseInfo::operator=(const MailBaseInfo &rMailBaseInfo)
{
    if(this == &rMailBaseInfo)
        return *this;

    m_nMailID = rMailBaseInfo.m_nMailID;
    m_nType = rMailBaseInfo.m_nType;
    SetSenderName(rMailBaseInfo.m_strSenderName);
    SetTitle(rMailBaseInfo.m_strTitle);
    m_nSendTime = rMailBaseInfo.m_nSendTime;
    m_bRead = rMailBaseInfo.m_bRead;
    m_nHasItem = rMailBaseInfo.m_nHasItem;
    m_nMoney = rMailBaseInfo.m_nMoney;
    m_nBindMCoin = rMailBaseInfo.m_nBindMCoin;
    m_nSenderID = rMailBaseInfo.m_nSenderID;

    return *this;
}

unsigned int MailBaseInfo::GetCurValidTime() const
{
    unsigned int nTimeNow = (unsigned int)time(NULL);
    unsigned int nValidTime = 0;

    if (m_nSendTime + MAX_MAIL_RESERVE_TIME > nTimeNow)
    {
        nValidTime = m_nSendTime + MAX_MAIL_RESERVE_TIME - nTimeNow;
    }

    return nValidTime;
}

void MailBaseInfo::SetSenderName(const std::string &rSenderName)
{
    m_strSenderName = rSenderName;

    if (m_strSenderName.size() > (size_t)g_ServerConfig.m_MaxNameLimit)
        m_strSenderName.resize(g_ServerConfig.m_MaxNameLimit);
}

void MailBaseInfo::SetTitle(const std::string &rTitle)
{
    m_strTitle = rTitle;

    if (m_strTitle.size() > (size_t)g_ServerConfig.m_nMaxMailTitleLength)
        m_strTitle.resize(g_ServerConfig.m_nMaxMailTitleLength);
}

void MailBaseInfo::doEncode(CParamPool &IOBuff) const
{
    IOBuff.AddInt64(m_nMailID);
    IOBuff.AddStringW(m_strSenderName);
    IOBuff.AddStringW(m_strTitle);
    IOBuff.AddUInt(m_nSendTime);
    IOBuff.AddUChar(m_bRead);
    IOBuff.AddUChar(m_nHasItem);
    IOBuff.AddInt(m_nMoney);
    IOBuff.AddInt( m_nBindMCoin );
    IOBuff.AddUInt(GetCurValidTime() );
    IOBuff.AddUInt(m_nSenderID);
}

void MailBaseInfo::doEncodeToServer(CParamPool &IOBuff) const
{
    IOBuff.AddInt64(m_nMailID);
    IOBuff.AddShort(m_nType);
    IOBuff.AddStringW(m_strSenderName);
    IOBuff.AddStringW(m_strTitle);
    IOBuff.AddUInt(m_nSendTime);
    IOBuff.AddUChar(m_bRead);
    IOBuff.AddUChar(m_nHasItem);
    IOBuff.AddInt(m_nMoney);
    IOBuff.AddInt( m_nBindMCoin );
    IOBuff.AddUInt(m_nSenderID);
}

void MailBaseInfo::doDecodeFromServer(CParamPool &IOBuff)
{
    m_nMailID = IOBuff.GetInt64();
    m_nType = IOBuff.GetShort();

    std::string strTemp;
    IOBuff.GetStringW(strTemp);
    SetSenderName(strTemp);

    IOBuff.GetStringW(strTemp);
    SetTitle(strTemp);

    m_nSendTime = IOBuff.GetUInt();
    m_bRead = IOBuff.GetUChar();
    m_nHasItem = IOBuff.GetUChar();
    m_nMoney = IOBuff.GetInt();
    m_nBindMCoin = IOBuff.GetInt();
    m_nSenderID = IOBuff.GetInt();
}

//////////////////////////////////////////////////////////////////////////

MailInfo::MailInfo()
{
    m_nSenderID = 0;
    m_strContent.clear();
    m_nMoney = 0;
}

MailInfo::MailInfo(const MailInfo &rMailInfo)
{
    *this = rMailInfo;
}

MailInfo& MailInfo::operator=(const MailInfo &rMailInfo)
{
    if (this == &rMailInfo)
        return *this;

    MailBaseInfo::operator=(rMailInfo);

    SetContent(rMailInfo.m_strContent);
    m_mapItems = rMailInfo.m_mapItems;

    return *this;
}

void MailInfo::SetContent(const std::string &rContent)
{
    m_strContent = rContent;

    if (m_strContent.size() > (size_t)g_ServerConfig.m_nMaxMailContentLength)
        m_strContent.resize(g_ServerConfig.m_nMaxMailContentLength);
}

bool MailInfo::doEncode(CParamPool &IOBuff) const
{
    MailBaseInfo::doEncode(IOBuff);
    IOBuff.AddStringW(m_strContent);

    if (m_nHasItem > 0)
    {
        IOBuff.AddUShort((unsigned short)m_mapItems.size() );
        mapMailItems::const_iterator it = m_mapItems.begin();
        while (it != m_mapItems.end() )
        {
            const MailItem & aItem = it->second;
            aItem.doEncode(IOBuff);
            it++;
        }
    }

    return true;
}

bool MailInfo::doEncodeToServer(CParamPool &IOBuff) const
{
    MailBaseInfo::doEncodeToServer(IOBuff);
    IOBuff.AddStringW(m_strContent);

    if (m_nHasItem > 0)
    {
        IOBuff.AddUShort((unsigned short)m_mapItems.size() );
        mapMailItems::const_iterator it = m_mapItems.begin();
        while (it != m_mapItems.end() )
        {
            const MailItem & aItem = it->second;
            aItem.doEncode(IOBuff);
            it++;
        }
    }

    return true;
}

bool MailInfo::doDecodeFromServer(CParamPool & IOBuff)
{
    MailBaseInfo::doDecodeFromServer(IOBuff);

    std::string strContent;
    IOBuff.GetStringW(strContent);
    SetContent(strContent);

    if (m_nHasItem > 0)
    {
        int nSize = IOBuff.GetUShort();

        while (nSize-- > 0)
        {
            MailItem item;
            item.doDecode(IOBuff);
            m_mapItems.insert(make_pair(item.m_nType, item) );
        }
    }

    return true;
}

bool MailInfo::doEncodeToServer1(CParamPool &IOBuff) const
{
    MailBaseInfo::doEncodeToServer(IOBuff);
    IOBuff.AddStringW(m_strContent);

    if (m_nHasItem > 0)
    {
        IOBuff.AddUShort((unsigned short)m_mapItems.size() );
        mapMailItems::const_iterator it = m_mapItems.begin();
        while (it != m_mapItems.end() )
        {
            const MailItem & aItem = it->second;
            aItem.doEncode(IOBuff);
            IOBuff.AddInt(aItem.m_nDynamicBoxRelatedValue);
            it++;
        }
    }

    return true;
}

bool MailInfo::doDecodeFromServer1(CParamPool & IOBuff)
{
    MailBaseInfo::doDecodeFromServer(IOBuff);

    std::string strContent;
    IOBuff.GetStringW(strContent);
    SetContent(strContent);

    if (m_nHasItem > 0)
    {
        int nSize = IOBuff.GetUShort();

        while (nSize-- > 0)
        {
            MailItem item;
            item.doDecode(IOBuff);
            item.m_nDynamicBoxRelatedValue = IOBuff.GetInt();
            m_mapItems.insert(make_pair(item.m_nType, item) );
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

CKeepOnlineReward::CKeepOnlineReward()
:m_nIndex(0)
,m_nRequireTime(0)
,m_nCumulativeTime(0)
,m_nTimingDay(0)
,m_bFinished(false)
,m_bValid(true)
,m_bGotReward(false)
{
}

CKeepOnlineReward::~CKeepOnlineReward()
{

}

int CKeepOnlineReward::GetRemainTime()
{
	int nLeftTime = m_nRequireTime - m_nCumulativeTime;

	return nLeftTime > 0 ? nLeftTime : 0;
}

void CKeepOnlineReward::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_nIndex);
	IOBuff.AddUInt(GetRemainTime());
}

bool CKeepOnlineReward::doEncodeToCache( CParamPool &IOBuff ) const
{
    IOBuff << m_nIndex << m_nCumulativeTime << m_nTimingDay << m_bGotReward;
    return true;
}

bool CKeepOnlineReward::doDecodeFromCache( CParamPool &IOBuff )
{
    IOBuff >> m_nIndex >> m_nCumulativeTime >> m_nTimingDay >> m_bGotReward;
    return true;
}

//////////////////////////////////////////////////////////////////////////

MailInfoDB::MailInfoDB()
{
    m_nReceiverID = 0;
    memset(m_szReceiverName, 0, MAX_NAME_LEN);
    m_nMoney = 0;
    m_nItemType = 0;
    m_nCount = 0;
    m_nValidTime = 0;
    m_strTitle.clear();
    m_strContent.clear();
}

MailInfoDB::MailInfoDB(unsigned int nReceiverID, std::string szReceiverName, std::string strTitle, std::string strContext,
    int nMoney, itemtype_t nItemType, itemcount_t nCount, int nValidTime)
{
    Set(nReceiverID, szReceiverName, strTitle, strContext, nMoney, nItemType, nCount, nValidTime);
}

void MailInfoDB::Set(unsigned int nReceiverID, std::string szReceiverName, std::string strTitle, std::string strContext,
    int nMoney, itemtype_t nItemType, itemcount_t nCount, int nValidTime)
{
    m_nReceiverID = nReceiverID;
    memcpy(m_szReceiverName, szReceiverName.c_str(), szReceiverName.size());
    m_strTitle = strTitle;
    m_strContent = strContext;
    m_nMoney = nMoney;
    m_nItemType = nItemType;
    m_nCount = nCount;
    m_nValidTime = nValidTime;
}

void MailInfoDB::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nReceiverID);
    IOBuff.AddPerFixSizeMemory(m_szReceiverName, MAX_NAME_LEN);
    IOBuff.AddStringW(m_strTitle);
    IOBuff.AddStringW(m_strContent);
    IOBuff.AddInt(m_nMoney);
    IOBuff.AddUInt(m_nItemType);
    IOBuff.AddUShort(m_nCount);
    IOBuff.AddInt(m_nValidTime);

    return;

}

void MailInfoDB::doDecode(CParamPool &IOBuff)
{
    m_nReceiverID = IOBuff.GetUInt();
    unsigned short namesize = 0;
    IOBuff.GetPerFixSizeMemory(m_szReceiverName, namesize);
    IOBuff.GetStringW(m_strTitle);
    IOBuff.GetStringW(m_strContent);
    m_nMoney = IOBuff.GetInt();
    m_nItemType = IOBuff.GetUInt();
    m_nCount = IOBuff.GetUShort();
    m_nValidTime = IOBuff.GetInt();

    return;
}


void MailInfoDB::SetTitle(const std::string &rTitle)
{
    m_strTitle = rTitle;

    if (m_strTitle.size() > (size_t)g_ServerConfig.m_nMaxMailTitleLength)
        m_strTitle.resize(g_ServerConfig.m_nMaxMailTitleLength);
}

void MailInfoDB::SetContent(const std::string &rContent)
{
    m_strContent = rContent;

    if (m_strContent.size() > (size_t)g_ServerConfig.m_nMaxMailContentLength)
        m_strContent.resize(g_ServerConfig.m_nMaxMailContentLength);
}

FixedChargeRecordSingle::FixedChargeRecordSingle()
	: m_nAccount(0)
	, m_nMCoin(0)
	, m_nChargeTime(0)
{

}

FixedChargeRecordItem::FixedChargeRecordItem()
	: m_nMCoin(0)
	, m_nChargeTime(0)
{

}

FixedChargeRecord::FixedChargeRecord()
	: m_nAccount(0)
{

}

//////////////////////////////////////////////////////////////////////////
ChargeAction::ChargeAction()
    :m_strPUID("")
    ,m_nPID(0)
    ,m_nRoleID(0)
    ,m_nAction(0)
    ,m_nMoney(0)
    ,m_strOrderID("")
{

}

ChargeAction::ChargeAction(const ChargeAction & action)
{
    m_strPUID = action.m_strPUID;
    m_nPID = action.m_nPID;
    m_nRoleID = action.m_nRoleID;
    m_nAction = action.m_nAction;
    m_nMoney = action.m_nMoney;
    m_strOrderID = action.m_strOrderID;
}

ChargeAction::~ChargeAction()
{

}

void ChargeAction::doDecode(CParamPool &IOBuff)
{
    m_nAction = IOBuff.GetUChar();
    m_nPID    = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strPUID);
    m_nRoleID = IOBuff.GetUInt();
    m_nMoney = IOBuff.GetInt();
    IOBuff.GetStringW(m_strOrderID);
}

//////////////////////////////////////////////////////////////////////////

CCumulativeRechargeDB::CCumulativeRechargeDB()
:m_nRechargeNum(0)
,m_nActivityBeginTime(0)
,m_nActivityEndTime(0)
,m_nRechargeStartTime(0)
,m_nRechargeStopTime(0)
{

}

CCumulativeRechargeDB::~CCumulativeRechargeDB()
{

}

CCumulativeSpendDB::CCumulativeSpendDB()
	:m_nSpendStartTime(0)
	,m_nSpendStopTime(0)
	,m_nBillNum(0)
	,m_nBindBillNum(0)
{

}

CCumulativeSpendDB::~CCumulativeSpendDB()
{

}

//////////////////////////////////////////////////////////////////////////

CumulativeSpendGotMedalDB::CumulativeSpendGotMedalDB()
    : m_nRoleID( 0 )
    , m_nOpenTime( 0 )
    , m_nCloseTime( 0 )
{
}

//////////////////////////////////////////////////////////////////////////

BuyItemGotMedalDB::BuyItemGotMedalDB()
    : m_nRoleID( 0 )
    , m_nOpenTime( 0 )
    , m_nCloseTime( 0 )
    , m_nIndex( 0 )
    , m_nBuyNum( 0 )
{
}

BuyItemGotMedalRewardedInfo::BuyItemGotMedalRewardedInfo()
    : m_nBuyNum( 0 )
    , m_bRewarded( false )
{
}

//////////////////////////////////////////////////////////////////////////

CXmasFreeRewardDB::CXmasFreeRewardDB()
:m_nRoleID(0)
,m_nLastGetRewardTime(0)
,m_nActivityStartTime(0)
,m_nActivityEndTime(0)
{
	m_vecReward.assign(XMAS_FREEREWARD_LEVEL, 0);
}

CXmasFreeRewardDB::~CXmasFreeRewardDB()
{

}

//--------------------------------------------CFestivalWishInfoDB
CFestivalWishInfoDB::CFestivalWishInfoDB()
:m_nRoleID(0)
,m_nWishCount(0)
,m_nLastGetRewardTime(0)
,m_nActivityStartTime(0)
,m_nActivityEndTime(0)
{

}

CFestivalWishInfoDB::~CFestivalWishInfoDB()
{

}

//------------------------------------------CRoleBlessActivityDB
CRoleBlessActivityDB::CRoleBlessActivityDB()
:m_nRoleID(0)
,m_nBlessStartTime(0)
,m_nBlessEndTime(0)
,m_nBeBlessedCount(0)
{
	m_vecRewards.assign(BLESS_REWARD_LEVEL, 0);
}

CRoleBlessActivityDB::~CRoleBlessActivityDB()
{

}

//-----------------------------------------CFestivalBlessInfoDB
CFestivalBlessInfoDB::CFestivalBlessInfoDB()
:m_nSenderID(0)
,m_nRcverID(0)
,m_nWordsIndex(0)
,m_nBlessTime(0)
{

}

CFestivalBlessInfoDB::~CFestivalBlessInfoDB()
{

}

//-----------------------------------------------CRolePuzzleActivityDB
CRolePuzzleActivityDB::CRolePuzzleActivityDB()
:m_nPuzzleID(0)
,m_nUpdateTime(0)
,m_nFinishTime(0)
{
	memset( m_arFragments, '0', PUZZLE_FRAGMENTS_COUNT );
	m_arFragments[PUZZLE_FRAGMENTS_COUNT] = '\0';
}

CRolePuzzleActivityDB::~CRolePuzzleActivityDB()
{

}

bool CRolePuzzleActivityDB::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nPuzzleID << m_nUpdateTime << m_nFinishTime;
    IOBuff.AddMemory( m_arFragments, PUZZLE_FRAGMENTS_COUNT + 1 );

    return true;
}

bool CRolePuzzleActivityDB::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nPuzzleID >> m_nUpdateTime >> m_nFinishTime;
    IOBuff.GetMemory( m_arFragments, PUZZLE_FRAGMENTS_COUNT + 1 );

    return true;
}

CRoleAllPuzzleDB::CRoleAllPuzzleDB()
:m_nRoleID(0)
,m_nPuzzleBeginTime(0)
,m_nPuzzleEndTime(0)
{
}

CRoleAllPuzzleDB::~CRoleAllPuzzleDB()
{
}

CRoleInvitationRewardDB::CRoleInvitationRewardDB()
:m_nLevel(0)
,m_bReceived(false)
,m_nReceivedTime(0)
{
}

CRoleInvitationRewardDB::~CRoleInvitationRewardDB()
{
}


CRoleAllInvitationInfoDB::CRoleAllInvitationInfoDB()
:m_nRoleID(0)
,m_strInvitationCode("")
,m_nInvitationBindCount(0)
{
}

CRoleAllInvitationInfoDB::~CRoleAllInvitationInfoDB()
{
}


CLiveRoomDancerDB::CLiveRoomDancerDB()
: m_bIsDanceGroup(false)
{
	Clear();
}

void CLiveRoomDancerDB::Clear()
{
	memset(m_nDancerID, 0, sizeof(m_nDancerID));
	for (int i =0; i < MAX_ROOM_PLAYER_COUNT; i++)
	{
		m_strDancerName[i].clear();
	}
}

CLiveRoomDancerDB::~CLiveRoomDancerDB()
{

}

CLiveRoomScoreDB::CLiveRoomScoreDB()
: m_nRoleID(0)
, m_nPoint(0)
, m_nPointChanged(0)
, m_nPointUpdateTime(0)
, m_nHot(0)
, m_nHotChanged(0)
, m_nHotUpdateTime(0)
{
}

CLiveRoomScoreDB::~CLiveRoomScoreDB()
{
}

CAllLiveRoomScoreDB::CAllLiveRoomScoreDB()
{
}

CAllLiveRoomScoreDB::~CAllLiveRoomScoreDB()
{
}

CompensationInfo::CompensationInfo()
{
}

CompensationInfo::~CompensationInfo()
{
}

void CompensationInfo::EncodeInfo(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nID);
	IOBuff.AddInt(m_nEndTime);
	IOBuff.AddInt(m_nMoney);
	IOBuff.AddStringW(m_strTitle.c_str());
	IOBuff.AddStringW(m_strContent.c_str());
	IOBuff.AddStringW(m_strMaleItem.c_str());
	IOBuff.AddStringW(m_strFemaleItem.c_str());

	IOBuff.AddUShort((unsigned short)m_MaleItems.size());
	for (list<CItem>::iterator it_Male = m_MaleItems.begin();it_Male != m_MaleItems.end();++it_Male)
	{
		IOBuff.AddUInt(it_Male->m_nItemType);
		IOBuff.AddShort(it_Male->m_nItemCount);
		IOBuff.AddInt(it_Male->m_nValidTime);
	}

	IOBuff.AddUShort((unsigned short)m_FemaleItems.size() );
	for (list<CItem>::iterator it_Female = m_FemaleItems.begin();it_Female != m_FemaleItems.end();++it_Female)
	{
		IOBuff.AddUInt(it_Female->m_nItemType);
		IOBuff.AddShort(it_Female->m_nItemCount);
		IOBuff.AddInt(it_Female->m_nValidTime);
	}
}

void CompensationInfo::DecodeInfo(CParamPool &IOBuff)
{
	m_nID = IOBuff.GetInt();
	m_nEndTime = IOBuff.GetInt();
	m_nMoney = IOBuff.GetInt();
	IOBuff.GetStringW(m_strTitle);
	IOBuff.GetStringW(m_strContent);
	IOBuff.GetStringW(m_strMaleItem);
	IOBuff.GetStringW(m_strFemaleItem);

	int nItemCount = IOBuff.GetShort();
	for (int j = 0 ; j < nItemCount ; j++)
	{
		CItem item;
		item.m_nItemType = IOBuff.GetUInt();
		item.m_nItemCount = IOBuff.GetShort();
		item.m_nValidTime = IOBuff.GetInt();
		m_MaleItems.push_back(item);
	}

	nItemCount = IOBuff.GetShort();
	for (int j = 0 ; j < nItemCount ; j++)
	{
		CItem item;
		item.m_nItemType = IOBuff.GetUInt();
		item.m_nItemCount = IOBuff.GetShort();
		item.m_nValidTime = IOBuff.GetInt();
		m_FemaleItems.push_back(item);
	}
}

LongactingCumulativeRechargeBoardCast::LongactingCumulativeRechargeBoardCast()
    :m_nRoleID(0)
    ,m_nRewardIndex(0)
    ,m_strRoleName("")
{

}

void LongactingCumulativeRechargeBoardCast::DecodeInfo(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRoleName);
    m_nRewardIndex = IOBuff.GetUInt();
}

void LongactingCumulativeRechargeBoardCast::EncodeInfo(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName);
    IOBuff.AddUInt(m_nRewardIndex);
}

ClothEffectDB::ClothEffectDB()
    :m_nRoleID(0)
    ,m_nItemID(0)
    ,m_nEffectID(0)
{

}

ClothEffectDB::~ClothEffectDB()
{

}

ClothEffectHandbookDB::ClothEffectHandbookDB()
    :m_nSuitID(0)
    ,m_nParam1(0)
    ,m_strParam2("")
{

}

ClothEffectHandbookDB::~ClothEffectHandbookDB()
{

}

MagicArrayRecord::MagicArrayRecord()
	: m_nMagicArrayID(0)
    , m_nAccumulatedCallCount(0)
    , m_nResetTime(0)
    , m_nFreeCallTime(0)
    , m_bHaveFirstCall(false)
    , m_nTodayOnceCallCount(0)
{
}

MagicArrayRecord::MagicArrayRecord(const MagicArrayRecord & magic)
{
	m_nMagicArrayID = magic.m_nMagicArrayID;
    m_nAccumulatedCallCount = magic.m_nAccumulatedCallCount;
    m_nTodayOnceCallCount = magic.m_nTodayOnceCallCount;
    m_nResetTime = magic.m_nResetTime;
    m_nFreeCallTime = magic.m_nFreeCallTime;
    m_bHaveFirstCall = magic.m_bHaveFirstCall;
}

MagicArrayRecord::~MagicArrayRecord()
{

}

bool MagicArrayRecord::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nMagicArrayID << m_nAccumulatedCallCount << m_nFreeCallTime << m_bHaveFirstCall;
    return true;
}

bool MagicArrayRecord::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nMagicArrayID >> m_nAccumulatedCallCount >> m_nFreeCallTime >> m_bHaveFirstCall;
    return true;
}

MagicArrayExchangeRecord::MagicArrayExchangeRecord()
:m_nRefreshTime(0)
{}

MagicArrayExchangeRecord::MagicArrayExchangeRecord(const MagicArrayExchangeRecord & record)
{
    m_nRefreshTime = record.m_nRefreshTime;
    m_mapRefreshGroup = record.m_mapRefreshGroup;
}

MagicArrayExchangeRecord::~MagicArrayExchangeRecord()
{}

bool MagicArrayExchangeRecord::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nRefreshTime << m_mapRefreshGroup;
    return true;
}

bool MagicArrayExchangeRecord::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nRefreshTime >> m_mapRefreshGroup;
    return true;
}

MedalUnitDB::MedalUnitDB()
    : m_nMedalID( 0 )
    , m_nCurrentState( 0 )
    , m_nGetTime( 0 )
{
}

MedalUnitDB::~MedalUnitDB()
{
}

bool MedalUnitDB::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nMedalID << m_nCurrentState << m_nGetTime;
    return true;
}

bool MedalUnitDB::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nMedalID >> m_nCurrentState >> m_nGetTime;
    return true;
}

MedalScoreDB::MedalScoreDB() :
m_nMedalWearScore(0),
m_nMedalCumulativeScore(0)
{
}

bool MedalScoreDB::doEncode(CParamPool &IOBuff) const
{
    IOBuff << m_nMedalWearScore << m_nMedalCumulativeScore;
    return true;
}

bool MedalScoreDB::doDecode(CParamPool &IOBuff)
{
    IOBuff >> m_nMedalWearScore >> m_nMedalCumulativeScore;
    return true;
}


MedalConditionDB::MedalConditionDB()
    : m_nID( 0 )
    , m_nCurProgress( 0 )
    , m_nAreadyReward( 0 )
{
}

MedalConditionDB::MedalConditionDB( int nID, int nCurProgress, int nAreadyReward )
    : m_nID( nID )
    , m_nCurProgress( nCurProgress )
    , m_nAreadyReward( nAreadyReward )
{
}

MedalConditionDB::~MedalConditionDB()
{
}

bool MedalConditionDB::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nID << m_nCurProgress << m_nAreadyReward;
    return true;
}

bool MedalConditionDB::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nID >> m_nCurProgress >> m_nAreadyReward;
    return true;
}

BigMamaRoleInfoDB::BigMamaRoleInfoDB()
    : m_nPrizeStamp(0)
    , m_nJoinStamp(0)
{

}
BigMamaRoleInfoDB::~BigMamaRoleInfoDB()
{}
