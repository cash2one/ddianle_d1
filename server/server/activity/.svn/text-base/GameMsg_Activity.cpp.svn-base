#include "GameMsg_Activity.h"
#include "../share/ServerMsgDef.h"
#include "ActivityDataStruct.h"
#include "SocialShareMgr.h"


CCumulativeRechargeRewardSend::CCumulativeRechargeRewardSend()
:m_nLevel(0)
,m_nRequiredNum(0)
,m_nMoney(0)
,m_nBindMCoin(0)
{

}

CCumulativeRechargeRewardSend::~CCumulativeRechargeRewardSend()
{

}

void CCumulativeRechargeRewardSend::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar((unsigned char)m_nLevel);
	IOBuff.AddInt(m_nRequiredNum);
	IOBuff.AddUShort((unsigned short)m_listItem.size());
	for (std::list<CItem>::iterator itr = m_listItem.begin(); itr != m_listItem.end(); ++itr) {
		itr->doEncode(IOBuff);
	}
	IOBuff.AddInt(m_nMoney);
    IOBuff.AddInt(m_nBindMCoin);
	
	IOBuff.AddBool(m_bHasReceived);
}

CCumulativeSpendRewardSend::CCumulativeSpendRewardSend()
	:m_nLevel(0)
	,m_nRequiredNum(0)
	,m_nMoney(0)
	,m_nBindMCoin(0)
{

}

CCumulativeSpendRewardSend::~CCumulativeSpendRewardSend()
{

}

void CCumulativeSpendRewardSend::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar((unsigned char)m_nLevel);
	IOBuff.AddInt(m_nRequiredNum);
	IOBuff.AddUShort((unsigned short)m_listItem.size());
	for (std::list<CItem>::iterator itr = m_listItem.begin(); itr != m_listItem.end(); ++itr) {
		itr->doEncode(IOBuff);
	}
	IOBuff.AddInt(m_nMoney);
	IOBuff.AddInt(m_nBindMCoin);
	IOBuff.AddBool(m_bHasReceived);
}

CExchangeItemRewardSend::CExchangeItemRewardSend()
:m_nLevel(0)
,m_nRequiredItemType(0)
,m_nRequiredItemNum(0)
,m_nMoney(0)
{

}

CExchangeItemRewardSend::~CExchangeItemRewardSend()
{

}

void CExchangeItemRewardSend::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar((unsigned char)m_nLevel);
	IOBuff.AddUInt(m_nRequiredItemType);
	IOBuff.AddInt(m_nRequiredItemNum);
	IOBuff.AddUShort((unsigned short)m_listItem.size());
	for (std::list<CItem>::iterator itr = m_listItem.begin(); itr != m_listItem.end(); ++itr) {
		itr->doEncode(IOBuff);
	}
	IOBuff.AddInt(m_nMoney);
}

CInTimeOnlineActivitySend::CInTimeOnlineActivitySend()
:m_tActivityBeginTime(0)
,m_tActivityEndTime(0)
,m_nTriggerHour(0)
,m_nTriggerMin(0)
,m_nMoney(0)
{

}

CInTimeOnlineActivitySend::~CInTimeOnlineActivitySend()
{

}

void CInTimeOnlineActivitySend::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUInt((unsigned int)m_tActivityBeginTime);
	IOBuff.AddUInt((unsigned int)m_tActivityEndTime);
	IOBuff.AddInt(m_nTriggerHour);
	IOBuff.AddInt(m_nTriggerMin);

	IOBuff.AddUShort((unsigned short)m_listRewards.size());
	for (std::list<CItem>::iterator itr = m_listRewards.begin(); itr != m_listRewards.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddInt(m_nMoney);
}

CExchangeItemActivityInfo::CExchangeItemActivityInfo()
:m_nActivityID(EActivity_None)
,m_bExhibit(false)
,m_nWeight(0)
,m_strRegularImageName("")
,m_strThumbnailsName("")
,m_nActivityBeginTime(0)
,m_nActivityEndTime(0)
,m_strActivityName("")
,m_strActivityIntro("")
,m_nRequiredItemType(0)
,m_nCurRequiredItemNum(0)
{

}

CExchangeItemActivityInfo::~CExchangeItemActivityInfo()
{

}

void CExchangeItemActivityInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar((unsigned char)m_nActivityID);
	if (m_nActivityID > EActivity_None) {
		IOBuff.AddBool(m_bExhibit);
		IOBuff.AddInt(m_nWeight);
		IOBuff.AddStringW(m_strRegularImageName.c_str());
		IOBuff.AddStringW(m_strThumbnailsName.c_str());
		IOBuff.AddUInt(m_nActivityBeginTime);
		IOBuff.AddUInt(m_nActivityEndTime);
		IOBuff.AddStringW(m_strActivityName.c_str());
		IOBuff.AddStringW(m_strActivityIntro.c_str());
		IOBuff.AddUInt(m_nRequiredItemType);
		IOBuff.AddInt(m_nCurRequiredItemNum);
		IOBuff.AddUShort((unsigned short)m_listExchangeItemReward.size());	
		for (std::list<CExchangeItemRewardSend>::iterator itr = m_listExchangeItemReward.begin(); itr != m_listExchangeItemReward.end(); ++itr) {
			itr->doEncode(IOBuff);
		}
	}
}


CShowInTimeOnlineActivity::CShowInTimeOnlineActivity()
:m_nActivityID(EActivity_None)
,m_bExhibit(false)
,m_nWeight(0)
,m_strRegularImageName("")
,m_strThumbnailsName("")
,m_strActivityName("")
,m_strActivityIntro("")
{

}

CShowInTimeOnlineActivity::~CShowInTimeOnlineActivity()
{

}

void CShowInTimeOnlineActivity::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar((unsigned char)m_nActivityID);
	if (m_nActivityID > EActivity_None) {
		IOBuff.AddBool(m_bExhibit);
		IOBuff.AddInt(m_nWeight);
		IOBuff.AddStringW(m_strRegularImageName.c_str());
		IOBuff.AddStringW(m_strThumbnailsName.c_str());
		IOBuff.AddStringW(m_strActivityName.c_str());
		IOBuff.AddStringW(m_strActivityIntro.c_str());
		IOBuff.AddUShort((unsigned short)m_listActivities.size());	
		for (std::list<CInTimeOnlineActivitySend>::iterator itr = m_listActivities.begin(); itr != m_listActivities.end(); ++itr) {
			itr->doEncode(IOBuff);
		}
	}
}

CPlainTextActivityRewardSend::CPlainTextActivityRewardSend()
:m_strRewardName("")
,m_nMoney(0)
{

}

CPlainTextActivityRewardSend::~CPlainTextActivityRewardSend()
{

}

void CPlainTextActivityRewardSend::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddStringW(m_strRewardName.c_str());
	IOBuff.AddUShort((unsigned short)m_listItems.size());
	for (std::list<CItem>::iterator itr = m_listItems.begin(); itr != m_listItems.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddInt(m_nMoney);
}

///////////////////////////////////////////////
CPlainTextActivityInfo::CPlainTextActivityInfo()
:m_nActivityID(EActivity_None)
,m_bExhibit(false)
,m_nWeight(0)
,m_strRegularImageName("")
,m_strThumbnailsName("")
,m_strActivityTitle("")
,m_strActivityContent("")
{

}

CPlainTextActivityInfo::~CPlainTextActivityInfo()
{

}

void CPlainTextActivityInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar((unsigned char)m_nActivityID);
	if (m_nActivityID == EActivity_PlainTextAcitivity) {
		IOBuff.AddBool(m_bExhibit);
		IOBuff.AddInt(m_nWeight);
		IOBuff.AddStringW(m_strRegularImageName.c_str());
		IOBuff.AddStringW(m_strThumbnailsName.c_str());
		IOBuff.AddStringW(m_strActivityTitle.c_str());
		IOBuff.AddStringW(m_strActivityContent.c_str());
		IOBuff.AddUShort((unsigned short)m_listRewards.size());	
		for (std::list<CPlainTextActivityRewardSend>::iterator itr = m_listRewards.begin(); itr != m_listRewards.end(); ++itr) {
			itr->doEncode(IOBuff);
		}
	}
}

////////////////////////////////////////////////
CGiftCodeActivityInfo::CGiftCodeActivityInfo()
:m_nActivityID(EActivity_None)
,m_bExhibit(false)
,m_nWeight(0)
,m_strRegularImageName("")
,m_strThumbnailsName("")
,m_strActivityTitle("")
,m_strActivityContent("")
{

}

CGiftCodeActivityInfo::~CGiftCodeActivityInfo()
{

}

void CGiftCodeActivityInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar((unsigned char)m_nActivityID);
	if (m_nActivityID == EActivity_GiftCodeAcitivity) {
		IOBuff.AddBool(m_bExhibit);
		IOBuff.AddInt(m_nWeight);
		IOBuff.AddStringW(m_strRegularImageName.c_str());
		IOBuff.AddStringW(m_strThumbnailsName.c_str());
		IOBuff.AddStringW(m_strActivityTitle.c_str());
		IOBuff.AddStringW(m_strActivityContent.c_str());
	}
}

//--------------------------------------------------CProprietaryActivityInfo
CProprietaryActivityInfo::CProprietaryActivityInfo()
:m_eActivityId( EActivity_None )
,m_bExhibit( false )
,m_nWeight( 0 )
,m_strRegularImageName( "" )
,m_strThumbnailsName( "" )
,m_bHasReward(true)
,m_strActivityTitle( "" )
,m_strActivityContent( "" )
,m_bFreeOrCharge( false )
,m_nSex(0)
{

}

void CProprietaryActivityInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar( (unsigned char )m_eActivityId );
	if( m_eActivityId == EActivity_ProprietaryAcitivty )
	{
		IOBuff.AddBool( m_bExhibit );
		IOBuff.AddInt( m_nWeight );
		IOBuff.AddStringW( m_strRegularImageName.c_str() );
		IOBuff.AddStringW( m_strThumbnailsName.c_str() );

		IOBuff.AddBool(m_bHasReward);

		IOBuff.AddStringW( m_strActivityTitle.c_str() );
		IOBuff.AddStringW( m_strActivityContent.c_str() );

		if (m_nSex == (unsigned char)ESexType_Male)
		{
			IOBuff.AddUShort( (unsigned short)m_listReward.m_listMaleReward.size() );
			std::list<CItem>::iterator iter = m_listReward.m_listMaleReward.begin(), iterEnd = m_listReward.m_listMaleReward.end();
			for( ; iter != iterEnd; ++iter )
			{
				iter->doEncode( IOBuff );
			}
		}
		else
		{
			IOBuff.AddUShort( (unsigned short)m_listReward.m_listFemaleReward.size() );
			std::list<CItem>::iterator iter = m_listReward.m_listFemaleReward.begin(), iterEnd = m_listReward.m_listFemaleReward.end();
			for( ; iter != iterEnd; ++iter )
			{
				iter->doEncode( IOBuff );
			}
		}

		IOBuff.AddUInt( (unsigned int)m_listReward.m_nMoney );
		IOBuff.AddBool( m_bFreeOrCharge );
	}
}

//--------------------------------------------------CXmasRewardSend
CXmasRewardSend::CXmasRewardSend()
:m_nIndex(0)
,m_nState(0)
{

}

CXmasRewardSend::~CXmasRewardSend()
{

}

void CXmasRewardSend::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar(m_nIndex);
	IOBuff.AddUChar(m_nState);
}

//--------------------------------------------------CXmasFreeRewardActivitySend
CXmasFreeRewardActivitySend::CXmasFreeRewardActivitySend()
:m_nActivity(EActivity_None)
{

}

CXmasFreeRewardActivitySend::~CXmasFreeRewardActivitySend()
{

}

void CXmasFreeRewardActivitySend::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar(m_nActivity);
	if (m_nActivity == EActivity_XmaxFreeReward) {
		IOBuff.AddUShort((unsigned short)m_listFreeRewardInfo.size());
		for (std::list<CXmasRewardSend>::iterator itr = m_listFreeRewardInfo.begin(); itr != m_listFreeRewardInfo.end(); ++itr) {
			itr->doEncode(IOBuff);
		}
	}
}

////////////////////////////////////////////////
CFestivalWishInfo::CFestivalWishInfo()
:m_nActivityID(EActivity_None)
,m_nWishRemainTimes(0)
,m_nWishItemCount(0)
,m_nMoney(0)
,m_bMysteriousReward(false)
{

}

CFestivalWishInfo::~CFestivalWishInfo()
{

}

void CFestivalWishInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar((unsigned char)m_nActivityID);
	if (m_nActivityID == EActivity_FestivalWish) {
		IOBuff.AddUShort(m_nWishRemainTimes);
		IOBuff.AddUShort(m_nWishItemCount);
		IOBuff.AddUShort((unsigned short)m_listItems.size());
		for (std::list<CItem>::iterator itr = m_listItems.begin(); itr != m_listItems.end(); ++itr) {
			itr->doEncode(IOBuff);
		}
		IOBuff.AddInt(m_nMoney);
		IOBuff.AddBool(m_bMysteriousReward);
	}
}
//--------------------------------------------------------------------CFestivalBlessRewardSend
CFestivalBlessRewardSend::CFestivalBlessRewardSend()
:m_nLevel(0)
,m_nBeBlessedCount(0)
,m_nMoney(0)
,m_bAlreadyGot(false)
{

}

CFestivalBlessRewardSend::~CFestivalBlessRewardSend()
{

}

void CFestivalBlessRewardSend::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar(m_nLevel);
	IOBuff.AddUInt(m_nBeBlessedCount);
	IOBuff.AddUShort((unsigned short)m_listRewards.size());
	for (std::list<CItem>::iterator itr = m_listRewards.begin(); itr != m_listRewards.end(); ++itr) {
		itr->doEncode(IOBuff);
	}
	IOBuff.AddInt(m_nMoney);
	IOBuff.AddBool(m_bAlreadyGot);
}

//----------------------------------------------COnlineRewardInfoSend
COnlineRewardInfoSend::COnlineRewardInfoSend()
:m_nIndex(0)
,m_nRequireTime(0)
,m_nMoney(0)
,m_bAlreadyGot(false)
{

}

COnlineRewardInfoSend::~COnlineRewardInfoSend()
{
	
}

void COnlineRewardInfoSend::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_nIndex);
	IOBuff.AddUInt(m_nRequireTime);
	IOBuff.AddUShort((unsigned short)m_listRewards.size());
	for (std::list<CItem>::iterator itr = m_listRewards.begin(); itr != m_listRewards.end(); ++itr) {
		itr->doEncode(IOBuff);
	}
	IOBuff.AddInt(m_nMoney);
	IOBuff.AddBool(m_bAlreadyGot);
}

//---------------------------------------------COnlineRewardActivity
COnlineRewardActivity::COnlineRewardActivity()
:m_nActivityID(EActivity_None)
,m_bExhibit(false)
,m_nWeight(0)
,m_strRegularImageName("")
,m_strThumbnailName("")
,m_nActivityBeginTime(0)
,m_nActivityEndTime(0)
,m_strActivityName("")
,m_strContent("")
{

}

COnlineRewardActivity::~COnlineRewardActivity()
{

}

void COnlineRewardActivity::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar(m_nActivityID);
	if (m_nActivityID == EActivity_OnlineReward) 
    {
		IOBuff.AddBool(m_bExhibit);
		IOBuff.AddInt(m_nWeight);
		IOBuff.AddStringW(m_strRegularImageName.c_str());
		IOBuff.AddStringW(m_strThumbnailName.c_str());
		IOBuff.AddUInt(m_nActivityBeginTime);
		IOBuff.AddUInt(m_nActivityEndTime);
		IOBuff.AddStringW(m_strActivityName.c_str());
		IOBuff.AddStringW(m_strContent.c_str());
		m_nextRewardInfo.doEncode(IOBuff);
		IOBuff.AddUShort((unsigned short)m_listOnlineRewardInfo.size());
		for (std::list<COnlineRewardInfoSend>::iterator itr = m_listOnlineRewardInfo.begin(); itr != m_listOnlineRewardInfo.end(); ++itr) 
        {
			itr->doEncode(IOBuff);
		}
	}
}

//----------------------------------------------COnlineRewardSend
COnlineRewardSend::COnlineRewardSend()
{

}

COnlineRewardSend::~COnlineRewardSend()
{

}

void COnlineRewardSend::doEncode(CParamPool & IOBuff) 
{
	m_onlineRewardInfo.doEncode(IOBuff);
}

//----------------------------------------------CRolePuzzleSync
CRolePuzzleSync::CRolePuzzleSync()
:m_nPuzzleID(0)
,m_nFinishTime(0)
,m_nMoneyReward(0)
{
	memset( m_arFragments, '0', PUZZLE_FRAGMENTS_COUNT );
	m_arFragments[PUZZLE_FRAGMENTS_COUNT] = '\0';
}

CRolePuzzleSync::~CRolePuzzleSync()
{

}

void CRolePuzzleSync::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddInt( m_nPuzzleID );
	IOBuff.AddStringW( m_strTitle.c_str() );
	IOBuff.AddStringW( m_strTexture.c_str() );

	m_NormalCost.doEncode( IOBuff );
	m_SpecialCost.doEncode( IOBuff );

	IOBuff.AddInt( m_nFinishTime );

	IOBuff.AddUShort( PUZZLE_FRAGMENTS_COUNT );
	for( int i = 0; i < PUZZLE_FRAGMENTS_COUNT; ++i )
	{
		IOBuff.AddChar( m_arFragments[i] );
	}

	unsigned short nItemCount = (unsigned short)m_lstItemReward.size();
	IOBuff.AddUShort( nItemCount );

	for( std::list<CItem>::iterator it = m_lstItemReward.begin(); it != m_lstItemReward.end(); ++it )
	{
		it->doEncode( IOBuff );
	}

	IOBuff.AddInt( m_nMoneyReward );
}

GameMsg_S2C_UpdateOnlineRewardInfo::GameMsg_S2C_UpdateOnlineRewardInfo():
GameMsg_Base(MSG_S2C_UpdateOnlineRewardInfo)
{

}

GameMsg_S2C_UpdateOnlineRewardInfo::~GameMsg_S2C_UpdateOnlineRewardInfo()
{

}

bool GameMsg_S2C_UpdateOnlineRewardInfo::doEncode( CParamPool & IOBuff )
{
	m_onlieReward.doEncode(IOBuff);

	return true;
}

GameMsg_C2S_GetOnlineReward::GameMsg_C2S_GetOnlineReward()
:GameMsg_Base(MSG_C2S_GetOnlineReward)
{

}

GameMsg_C2S_GetOnlineReward::~GameMsg_C2S_GetOnlineReward()
{

}

bool GameMsg_C2S_GetOnlineReward::doDecode( CParamPool &IOBuff )
{
	return true;
}

//--------------------------------------------GameMsg_S2C_GetOnlineRewardSuc
GameMsg_S2C_GetOnlineRewardSuc::GameMsg_S2C_GetOnlineRewardSuc()
:GameMsg_Base(MSG_S2C_GetOnlineRewardSuc)
{

}

GameMsg_S2C_GetOnlineRewardSuc::~GameMsg_S2C_GetOnlineRewardSuc()
{

}

bool GameMsg_S2C_GetOnlineRewardSuc::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUShort(m_nCurIndex);
	m_NextRewardInfo.doEncode(IOBuff);

	return true;
}


GameMsg_S2C_GetOnlineRewardFail::GameMsg_S2C_GetOnlineRewardFail():
GameMsg_Base(MSG_S2C_GetOnlineRewardFail)
{

}

GameMsg_S2C_GetOnlineRewardFail::~GameMsg_S2C_GetOnlineRewardFail()
{

}

bool GameMsg_S2C_GetOnlineRewardFail::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}



////////////////////////////////////////////////
CCummulativeRechareActivityInfo::CCummulativeRechareActivityInfo()
:m_nActivityID(EActivity_None)
,m_bExhibit(false)
,m_nWeight(0)
,m_strRegularImageName("")
,m_strThumbnailsName("")
,m_nRechargeStartTime(0)
,m_nRechargeStopTime(0)
,m_strActivityName("")
,m_strDetail("")
,m_nCurRechargeNum(0)
{

}

CCummulativeRechareActivityInfo::~CCummulativeRechareActivityInfo()
{

}

void CCummulativeRechareActivityInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar((unsigned char)m_nActivityID);
	if (m_nActivityID > EActivity_None) {
		IOBuff.AddBool(m_bExhibit);
		IOBuff.AddInt(m_nWeight);
		IOBuff.AddStringW(m_strRegularImageName.c_str());
		IOBuff.AddStringW(m_strThumbnailsName.c_str());
		IOBuff.AddUInt(m_nRechargeStartTime);
		IOBuff.AddUInt(m_nRechargeStopTime);
		IOBuff.AddStringW(m_strActivityName.c_str());
		IOBuff.AddStringW(m_strDetail.c_str());
		IOBuff.AddInt(m_nCurRechargeNum);
		IOBuff.AddUShort((unsigned short)m_listCumulativeReward.size());	

		for (std::list<CCumulativeRechargeRewardSend>::iterator itr = m_listCumulativeReward.begin(); itr != m_listCumulativeReward.end(); ++itr) 
        {
			itr->doEncode(IOBuff);
		}
	}
}

////////////////////////////////////////////////
CCummulativeSpendActivityInfo::CCummulativeSpendActivityInfo()
	:m_nActivityID(EActivity_None)
	,m_bExhibit(false)
	,m_nWeight(0)
	,m_strRegularImageName("")
	,m_strThumbnailsName("")
	,m_nSpendStartTime(0)
	,m_nSpendStopTime(0)
	,m_strActivityName("")
	,m_strDetail("")
	,m_nCurRechargeNum(0)
{

}

CCummulativeSpendActivityInfo::~CCummulativeSpendActivityInfo()
{

}

void CCummulativeSpendActivityInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar((unsigned char)m_nActivityID);
	if (m_nActivityID > EActivity_None) {
		IOBuff.AddBool(m_bExhibit);
		IOBuff.AddInt(m_nWeight);
		IOBuff.AddStringW(m_strRegularImageName.c_str());
		IOBuff.AddStringW(m_strThumbnailsName.c_str());
		IOBuff.AddChar(m_nSpendType);
		IOBuff.AddUInt(m_nSpendStartTime);
		IOBuff.AddUInt(m_nSpendStopTime);
		IOBuff.AddStringW(m_strActivityName.c_str());
		IOBuff.AddStringW(m_strDetail.c_str());
		IOBuff.AddInt(m_nCurRechargeNum);
		IOBuff.AddUShort((unsigned short)m_listCumulativeReward.size());	
		for (std::list<CCumulativeSpendRewardSend>::iterator itr = m_listCumulativeReward.begin(); itr != m_listCumulativeReward.end(); ++itr) {
			itr->doEncode(IOBuff);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

CumulativeSpendGotMedalRewardData::CumulativeSpendGotMedalRewardData()
    : m_nIndex( 0 )
    , m_nRequiredNum( 0 )
    , m_bHasReceived( false )
{
}

bool CumulativeSpendGotMedalRewardData::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nIndex << m_nRequiredNum << m_listReward << m_bHasReceived;

    return true;
}

CumulativeSpendGotMedalActivityData::CumulativeSpendGotMedalActivityData()
    : m_nActivityID( EActivity_None )
    , m_bExhibit( false )
    , m_nWeight( 0 )
    , m_strBigImage( "" )
    , m_strSmallImage( "" )
    , m_nOpenTime( 0 )
    , m_nCloseTime( 0 )
    , m_strTitle( "" )
    , m_strContent( "" )
    , m_nCumulativeType( 1 )
    , m_nCumulativeAmount( 0 )
{
}

bool CumulativeSpendGotMedalActivityData::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << (unsigned char)m_nActivityID;

    if ( m_nActivityID > EActivity_None )
    {
        IOBuff << m_bExhibit << m_nWeight << stringw( m_strBigImage ) 
            << stringw( m_strSmallImage ) << m_nOpenTime << m_nCloseTime 
            << stringw( m_strTitle ) << stringw( m_strContent ) 
            << m_nCumulativeType << m_nCumulativeAmount << m_listReward;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

BuyItemGotMedalRewardData::BuyItemGotMedalRewardData()
    : m_nIndex( 0 )
    , m_nItemType( 0 )
    , m_nItemCount( 0 )
    , m_nCurrentCount( 0 )
    , m_bHasReceived( false )
{
}

bool BuyItemGotMedalRewardData::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nIndex << m_nItemType << m_nItemCount 
        << m_nCurrentCount << m_listReward << m_bHasReceived;

    return true;
}

BuyItemGotMedalActivityData::BuyItemGotMedalActivityData()
    : m_nActivityID( EActivity_None )
    , m_bExhibit( false )
    , m_nWeight( 0 )
    , m_strBigImage( "" )
    , m_strSmallImage( "" )
    , m_nOpenTime( 0 )
    , m_nCloseTime( 0 )
    , m_strTitle( "" )
    , m_strContent( "" )
{
}

bool BuyItemGotMedalActivityData::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << (unsigned char)m_nActivityID;

    if ( m_nActivityID > EActivity_None )
    {
        IOBuff << m_bExhibit << m_nWeight << stringw( m_strBigImage ) 
            << stringw( m_strSmallImage ) << m_nOpenTime << m_nCloseTime 
            << stringw( m_strTitle ) << stringw( m_strContent ) 
            << m_listReward;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_GetAllActivitiesInfo::GameMsg_C2S_GetAllActivitiesInfo()
:GameMsg_Base(MSG_C2S_GetAllActivitiesInfo)
{

}

GameMsg_C2S_GetAllActivitiesInfo::~GameMsg_C2S_GetAllActivitiesInfo()
{

}

bool GameMsg_C2S_GetAllActivitiesInfo::doDecode( CParamPool &IOBuff )
{
	return true;
}

GameMsg_C2S_GetCDKeyInfo::GameMsg_C2S_GetCDKeyInfo()
	:GameMsg_Base(MSG_C2S_GetCDKeyInfo)
{

}

GameMsg_C2S_GetCDKeyInfo::~GameMsg_C2S_GetCDKeyInfo()
{

}

bool GameMsg_C2S_GetCDKeyInfo::doDecode( CParamPool &IOBuff )
{
	return true;
}

GameMsg_C2S_GetBindActivationInfo::GameMsg_C2S_GetBindActivationInfo()
	:GameMsg_Base(MSG_C2S_GetBindActivationInfo)
{

}

GameMsg_C2S_GetBindActivationInfo::~GameMsg_C2S_GetBindActivationInfo()
{

}

bool GameMsg_C2S_GetBindActivationInfo::doDecode( CParamPool &IOBuff )
{
	return true;
}

GameMsg_S2C_GetCDKeyInfoSuc::GameMsg_S2C_GetCDKeyInfoSuc()
	:GameMsg_Base(MSG_S2C_GetCDKeyInfoSuc)
{
}

GameMsg_S2C_GetCDKeyInfoSuc::~GameMsg_S2C_GetCDKeyInfoSuc()
{

}

bool GameMsg_S2C_GetCDKeyInfoSuc::doEncode( CParamPool & IOBuff )
{
	m_GiftCodeActivity.doEncode(IOBuff);
	return true;
}

GameMsg_S2C_GetBindActivationInfoSuc::GameMsg_S2C_GetBindActivationInfoSuc()
	:GameMsg_Base(MSG_S2C_GetBindInfoSuc)
{
}

GameMsg_S2C_GetBindActivationInfoSuc::~GameMsg_S2C_GetBindActivationInfoSuc()
{

}

bool GameMsg_S2C_GetBindActivationInfoSuc::doEncode( CParamPool & IOBuff )
{
	m_InvitationActivity.doEncode(IOBuff);
	return true;
}

GameMsg_S2C_GetAllActivitiesInfoSuc::GameMsg_S2C_GetAllActivitiesInfoSuc()
:GameMsg_Base(MSG_S2C_GetAllActivitiesInfoSuc)
{
    m_bFemale = false;
}

GameMsg_S2C_GetAllActivitiesInfoSuc::~GameMsg_S2C_GetAllActivitiesInfoSuc()
{

}

bool GameMsg_S2C_GetAllActivitiesInfoSuc::doEncode( CParamPool & IOBuff )
{
	m_CumulativeRechargeActivity.doEncode(IOBuff);
	m_ExchangeItemActivity.doEncode(IOBuff);
	m_ShowInTimeOnlineActivity.doEncode(IOBuff);
	m_plainTextActivity.doEncode(IOBuff);
	//m_GiftCodeActivity.doEncode(IOBuff);
	m_OnlineRewardActivity.doEncode(IOBuff);
	//m_InvitationActivity.doEncode(IOBuff);
	m_FresherActivity.doEncode(IOBuff);
	m_CumulativeSpendActivity.doEncode(IOBuff);
	m_MallActivity.doEncode(IOBuff, m_bFemale);
	m_PhotoActivity.doEncode(IOBuff, m_bFemale);
    IOBuff << m_cumulativeSpendGotMedalActivity 
        << m_buyItemGotMedalActivity;

	m_proprietaryActivity.doEncode( IOBuff );

    // red envelope
    m_redenvelopeActivity.doEncode(IOBuff);
    m_effectStoneExchangeActivity.doEncode(IOBuff);

	return true;
}

GameMsg_S2C_GetAllActivitiesInfoFail::GameMsg_S2C_GetAllActivitiesInfoFail()
:GameMsg_Base(MSG_S2C_GetAllActivitiesInfoFail)
,m_strError("")
{

}

GameMsg_S2C_GetAllActivitiesInfoFail::~GameMsg_S2C_GetAllActivitiesInfoFail()
{

}

bool GameMsg_S2C_GetAllActivitiesInfoFail::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}


GameMsg_S2C_UpdateCumulativeActivityState::GameMsg_S2C_UpdateCumulativeActivityState()
:GameMsg_Base(MSG_S2C_UpdateCumulativeActivityState)
{

}

GameMsg_S2C_UpdateCumulativeActivityState::~GameMsg_S2C_UpdateCumulativeActivityState()
{

}

bool GameMsg_S2C_UpdateCumulativeActivityState::doEncode( CParamPool &IOBuff )
{
	m_CumulativeRechargeActivity.doEncode(IOBuff);

	return true;
}

GameMsg_C2S_GetCumulativeRechargeReward::GameMsg_C2S_GetCumulativeRechargeReward()
:GameMsg_Base(MSG_C2S_GetCumulativeRechargeReward)
,m_nLevel(-1)
{

}

GameMsg_C2S_GetCumulativeRechargeReward::~GameMsg_C2S_GetCumulativeRechargeReward()
{

}

bool GameMsg_C2S_GetCumulativeRechargeReward::doDecode( CParamPool & IOBuff )
{
	m_nLevel = (char)IOBuff.GetUChar();

	return true;
}

GameMsg_C2S_GetCumulativeSpendReward::GameMsg_C2S_GetCumulativeSpendReward()
	:GameMsg_Base(MSG_C2S_GetCumulativeSpendReward)
	,m_nLevel(-1)
{

}

GameMsg_C2S_GetCumulativeSpendReward::~GameMsg_C2S_GetCumulativeSpendReward()
{

}

bool GameMsg_C2S_GetCumulativeSpendReward::doDecode( CParamPool & IOBuff )
{
	m_nLevel = (char)IOBuff.GetUChar();

	return true;
}

GameMsg_S2C_GetCumulativeRechargeRewardSuc::GameMsg_S2C_GetCumulativeRechargeRewardSuc()
:GameMsg_Base(MSG_S2C_GetCumulativeRechargeRewardSuc)
,m_nLevel(-1)
{

}

GameMsg_S2C_GetCumulativeRechargeRewardSuc::~GameMsg_S2C_GetCumulativeRechargeRewardSuc()
{

}

bool GameMsg_S2C_GetCumulativeRechargeRewardSuc::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddChar(m_nLevel);
	
	return true;
}

GameMsg_S2C_GetCumulativeSpendRewardSuc::GameMsg_S2C_GetCumulativeSpendRewardSuc()
	:GameMsg_Base(MSG_S2C_GetCumulativeSpendRewardSuc)
	,m_nLevel(-1)
{

}

GameMsg_S2C_GetCumulativeSpendRewardSuc::~GameMsg_S2C_GetCumulativeSpendRewardSuc()
{

}

bool GameMsg_S2C_GetCumulativeSpendRewardSuc::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddChar(m_nLevel);

	return true;
}

GameMsg_S2C_GetCumulativeRechargeRewardFail::GameMsg_S2C_GetCumulativeRechargeRewardFail()
:GameMsg_Base(MSG_S2C_GetCumulativeRechargeRewardFail)
,m_strError("")
{

}

GameMsg_S2C_GetCumulativeRechargeRewardFail::~GameMsg_S2C_GetCumulativeRechargeRewardFail()
{

}

bool GameMsg_S2C_GetCumulativeRechargeRewardFail::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

GameMsg_S2C_GetCumulativeSpendRewardFail::GameMsg_S2C_GetCumulativeSpendRewardFail()
	:GameMsg_Base(MSG_S2C_GetCumulativeSpendRewardFail)
	,m_strError("")
{

}

GameMsg_S2C_GetCumulativeSpendRewardFail::~GameMsg_S2C_GetCumulativeSpendRewardFail()
{

}

bool GameMsg_S2C_GetCumulativeSpendRewardFail::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_GetCumulativeSpendGotMedalReward::GameMsg_C2S_GetCumulativeSpendGotMedalReward()
    : GameMsg_Base( MSG_C2S_GetCumulativeSpendGotMedalReward )
    , m_nIndex( 0 )
{
}

GameMsg_C2S_GetCumulativeSpendGotMedalReward::~GameMsg_C2S_GetCumulativeSpendGotMedalReward()
{
}

bool GameMsg_C2S_GetCumulativeSpendGotMedalReward::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nIndex;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_GetCumulativeSpendGotMedalRewardSuc::GameMsg_S2C_GetCumulativeSpendGotMedalRewardSuc()
    : GameMsg_Base( MSG_S2C_GetCumulativeSpendGotMedalRewardSuc )
    , m_nIndex( 0 )
{
}

GameMsg_S2C_GetCumulativeSpendGotMedalRewardSuc::~GameMsg_S2C_GetCumulativeSpendGotMedalRewardSuc()
{
}

bool GameMsg_S2C_GetCumulativeSpendGotMedalRewardSuc::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_nIndex;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_GetCumulativeSpendGotMedalRewardFail::GameMsg_S2C_GetCumulativeSpendGotMedalRewardFail()
    : GameMsg_Base( MSG_S2C_GetCumulativeSpendGotMedalRewardFail )
    , m_strError( "" )
{
}

GameMsg_S2C_GetCumulativeSpendGotMedalRewardFail::~GameMsg_S2C_GetCumulativeSpendGotMedalRewardFail()
{
}

bool GameMsg_S2C_GetCumulativeSpendGotMedalRewardFail::doEncode( CParamPool &IOBuff )
{
    IOBuff << stringw( m_strError );

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_UpdateCumulativeSpendNum::GameMsg_S2C_UpdateCumulativeSpendNum()
    :GameMsg_Base(MSG_S2C_UpdateCumulativeSpendNum)
    ,m_nSpendNum(0)
{

}

GameMsg_S2C_UpdateCumulativeSpendNum::~GameMsg_S2C_UpdateCumulativeSpendNum()
{

}

bool GameMsg_S2C_UpdateCumulativeSpendNum::doEncode( CParamPool & IOBuff )
{
    IOBuff.AddInt(m_nSpendNum);

    return true;
}


GameMsg_C2S_GetProprietaryReward::GameMsg_C2S_GetProprietaryReward()
:GameMsg_Base( MSG_C2S_GetProprietaryReward )
{

}

bool GameMsg_C2S_GetProprietaryReward::doDecode(CParamPool & IOBuff)
{
	return true;
}

GameMsg_S2C_GetProprietaryRewardResult::GameMsg_S2C_GetProprietaryRewardResult()
:GameMsg_Base( MSG_S2C_GetProprietaryRewardResult )
,m_strError( "" )
{
}

bool GameMsg_S2C_GetProprietaryRewardResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );
	return true;
}

GameMsg_S2C_UpdateCumulativeRechargeNum::GameMsg_S2C_UpdateCumulativeRechargeNum()
:GameMsg_Base(MSG_S2C_UpdateCumulativeRechargeNum)
,m_nRechargeNum(0)
{

}

GameMsg_S2C_UpdateCumulativeRechargeNum::~GameMsg_S2C_UpdateCumulativeRechargeNum()
{

}

bool GameMsg_S2C_UpdateCumulativeRechargeNum::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddInt(m_nRechargeNum);

	return true;
}



GameMsg_C2S_ExchangeItem::GameMsg_C2S_ExchangeItem()
:GameMsg_Base(MSG_C2S_ExchangeItem)
,m_nLevel(0)
{

}

GameMsg_C2S_ExchangeItem::~GameMsg_C2S_ExchangeItem()
{

}

bool GameMsg_C2S_ExchangeItem::doDecode( CParamPool & IOBuff )
{
	m_nLevel = IOBuff.GetUChar();

	return true;
}


GameMsg_S2C_ExchangeItemSuc::GameMsg_S2C_ExchangeItemSuc()
:GameMsg_Base(MSG_S2C_ExchangeItemSuc)
,m_nLevel(0)
{

}

GameMsg_S2C_ExchangeItemSuc::~GameMsg_S2C_ExchangeItemSuc()
{

}

bool GameMsg_S2C_ExchangeItemSuc::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddChar(m_nLevel);

	return true;
}

GameMsg_S2C_ExchangeItemFail::GameMsg_S2C_ExchangeItemFail()
:GameMsg_Base(MSG_S2C_ExchangeItemFail)
,m_strError("")
{

}

GameMsg_S2C_ExchangeItemFail::~GameMsg_S2C_ExchangeItemFail()
{

}


bool GameMsg_S2C_ExchangeItemFail::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}


//------------------------------------------------------GameMsg_C2S_GetXmasFreeRewardActivityInfo
GameMsg_C2S_GetXmasFreeRewardActivityInfo::GameMsg_C2S_GetXmasFreeRewardActivityInfo()
:GameMsg_Base(MSG_C2S_GetXmasAcitivyInfo)
{

}

GameMsg_C2S_GetXmasFreeRewardActivityInfo::~GameMsg_C2S_GetXmasFreeRewardActivityInfo()
{

}

bool GameMsg_C2S_GetXmasFreeRewardActivityInfo::doDecode( CParamPool & IOBuff )
{
	return true;
}

//-----------------------------------------------------GameMsg_S2C_GetXmasActivityInfoSuc
GameMsg_S2C_GetXmasActivityInfoSuc::GameMsg_S2C_GetXmasActivityInfoSuc()
:GameMsg_Base(MSG_S2C_GetXmasActivityInfoSuc)
{

}

GameMsg_S2C_GetXmasActivityInfoSuc::~GameMsg_S2C_GetXmasActivityInfoSuc()
{

}

bool GameMsg_S2C_GetXmasActivityInfoSuc::doEncode( CParamPool &IOBuff )
{
	m_XmasFreeRewardActivity.doEncode(IOBuff);

	return true;
}

//------------------------------------------------------------GameMsg_C2S_GetXmasFreeReward
GameMsg_C2S_GetXmasFreeReward::GameMsg_C2S_GetXmasFreeReward()
:GameMsg_Base(MSG_C2S_GetXmaxFreeReward)
,m_nLevel(0)
{

}

GameMsg_C2S_GetXmasFreeReward::~GameMsg_C2S_GetXmasFreeReward()
{

}

bool GameMsg_C2S_GetXmasFreeReward::doDecode( CParamPool & IOBuff )
{
	m_nLevel = IOBuff.GetUChar();

	return true;
}

//-------------------------------------------------------------GameMsg_S2C_GetXmasFreeRewardSuc
GameMsg_S2C_GetXmasFreeRewardSuc::GameMsg_S2C_GetXmasFreeRewardSuc()
:GameMsg_Base(MSG_S2C_GetXmaxFreeRewardSuc)
,m_nLevel(0)
,m_nMoney(0)
{

}

GameMsg_S2C_GetXmasFreeRewardSuc::~GameMsg_S2C_GetXmasFreeRewardSuc()
{

}

bool GameMsg_S2C_GetXmasFreeRewardSuc::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar(m_nLevel);
	IOBuff.AddUShort((unsigned short)m_listItems.size());
	for (std::list<CItem>::iterator itr = m_listItems.begin(); itr != m_listItems.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	IOBuff.AddInt(m_nMoney);
	
	return true;
}

//--------------------------------------------------------------------GameMsg_S2C_GetXmasFreeRewardFail
GameMsg_S2C_GetXmasFreeRewardFail::GameMsg_S2C_GetXmasFreeRewardFail()
:GameMsg_Base(MSG_S2C_GetXmaxFreeRewardFail)
,m_nError(EGetXmasFreeRewardErrno_Success)
{

}

GameMsg_S2C_GetXmasFreeRewardFail::~GameMsg_S2C_GetXmasFreeRewardFail()
{

}

bool GameMsg_S2C_GetXmasFreeRewardFail::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar(m_nError);

	return true;
}

//--------------------------------------------------------------------GameMsg_C2S_GetFestivalWishInfo
GameMsg_C2S_GetFestivalWishInfo::GameMsg_C2S_GetFestivalWishInfo()
:GameMsg_Base(MSG_C2S_GetFestivalWishInfo)
{

}

GameMsg_C2S_GetFestivalWishInfo::~GameMsg_C2S_GetFestivalWishInfo()
{

}

bool GameMsg_C2S_GetFestivalWishInfo::doDecode(CParamPool & IOBuff)
{
	return true;
}

//--------------------------------------------------------------------GameMsg_S2C_GetFestivalWishInfoSuc
GameMsg_S2C_GetFestivalWishInfoSuc::GameMsg_S2C_GetFestivalWishInfoSuc()
:GameMsg_Base(MSG_S2C_GetFestivalWishInfoSuc)
{

}

GameMsg_S2C_GetFestivalWishInfoSuc::~GameMsg_S2C_GetFestivalWishInfoSuc()
{

}

bool GameMsg_S2C_GetFestivalWishInfoSuc::doEncode(CParamPool &IOBuff)
{
	m_FestivalWishInfo.doEncode(IOBuff);

	return true;
}

//--------------------------------------------------------------------GameMsg_C2S_FestivalWish
GameMsg_C2S_FestivalWish::GameMsg_C2S_FestivalWish()
:GameMsg_Base(MSG_C2S_FestivalWish)
{

}

GameMsg_C2S_FestivalWish::~GameMsg_C2S_FestivalWish()
{

}

bool GameMsg_C2S_FestivalWish::doDecode( CParamPool & IOBuff )
{
	return true;
}

//--------------------------------------------------------------------GameMsg_S2C_FestivalWishSuc
GameMsg_S2C_FestivalWishSuc::GameMsg_S2C_FestivalWishSuc()
:GameMsg_Base(MSG_S2C_FestivalWishSuc)
,m_nCurMoney(0)
,m_nNextMoney(0)
,m_bNextMysterious(false)
,m_nWishTimesRemain(0)
,m_nNextWishItemCountNeed(0)
{

}

GameMsg_S2C_FestivalWishSuc::~GameMsg_S2C_FestivalWishSuc()
{

}

bool GameMsg_S2C_FestivalWishSuc::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUShort((unsigned short)m_listCurRewards.size());
	for (std::list<CItem>::iterator itr = m_listCurRewards.begin(); itr != m_listCurRewards.end(); ++itr) {
		itr->doEncode(IOBuff);
	}
	IOBuff.AddInt(m_nCurMoney);

	IOBuff.AddUShort((unsigned short)m_listNextReward.size());
	for (std::list<CItem>::iterator itr = m_listNextReward.begin(); itr != m_listNextReward.end(); ++itr) {
		itr->doEncode(IOBuff);
	}
	IOBuff.AddInt(m_nNextMoney);
	IOBuff.AddBool(m_bNextMysterious);

	IOBuff.AddUShort(m_nWishTimesRemain);
	IOBuff.AddUShort(m_nNextWishItemCountNeed);
	
	return true;
}

//--------------------------------------------------------------------GameMsg_S2C_FestivalWishFail
GameMsg_S2C_FestivalWishFail::GameMsg_S2C_FestivalWishFail()
:GameMsg_Base(MSG_S2C_FestivalWishFail)
,m_nError(EFestivalWishFailErrno_Success)
{

}

GameMsg_S2C_FestivalWishFail::~GameMsg_S2C_FestivalWishFail()
{

}

bool GameMsg_S2C_FestivalWishFail::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar(m_nError);
	
	return true;
}

//--------------------------------------------------------------------GameMsg_C2S_GetFestivalBlessActivityInfo
GameMsg_C2S_GetFestivalBlessActivityInfo::GameMsg_C2S_GetFestivalBlessActivityInfo()
:GameMsg_Base(MSG_C2S_GetFestivalBlessActivityInfo)
{

}

GameMsg_C2S_GetFestivalBlessActivityInfo::~GameMsg_C2S_GetFestivalBlessActivityInfo()
{

}

bool GameMsg_C2S_GetFestivalBlessActivityInfo::doDecode( CParamPool & IOBuff )
{
		return true;
}

//--------------------------------------------------------------------GameMsg_S2C_GetFestivalBlessActivityInfoSuc
GameMsg_S2C_GetFestivalBlessActivityInfoSuc::GameMsg_S2C_GetFestivalBlessActivityInfoSuc()
:GameMsg_Base(MSG_S2C_GetFestivalBlessInfoSuc)
,m_nActivityID(EActivity_None)
,m_nBeBlessedCount(0)
{

}

GameMsg_S2C_GetFestivalBlessActivityInfoSuc::~GameMsg_S2C_GetFestivalBlessActivityInfoSuc()
{

}

bool GameMsg_S2C_GetFestivalBlessActivityInfoSuc::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_nActivityID);
	if (m_nActivityID == EActivity_FestivalBless) {
		IOBuff.AddUInt(m_nBeBlessedCount);

		IOBuff.AddUShort((unsigned short)m_BeBlessWords.size());	
		for (std::list<CFestivalBlessEntry>::iterator itr = m_BeBlessWords.begin(); itr != m_BeBlessWords.end(); ++itr) {
			itr->doEncode(IOBuff);
		}

		IOBuff.AddUShort((unsigned short)m_listBlessRewardInfo.size());	
		for (std::list<CFestivalBlessRewardSend>::iterator itr = m_listBlessRewardInfo.begin(); itr != m_listBlessRewardInfo.end(); ++itr) {
			itr->doEncode(IOBuff);
		}	

		IOBuff.AddUShort((unsigned short)m_listBlessWordsData.size());	
		for (std::list<CFestivalBlessWords>::iterator itr = m_listBlessWordsData.begin(); itr != m_listBlessWordsData.end(); ++itr) {
			itr->doEncode(IOBuff);
		}
	}

	return true;
}

//---------------------------------------------------------GameMsg_C2S_BlessFriend
GameMsg_C2S_BlessFriend::GameMsg_C2S_BlessFriend()
:GameMsg_Base(MSG_C2S_BlessFriend)
,m_nFriendID(0)
,m_nWordsIndex(0)
{

}

GameMsg_C2S_BlessFriend::~GameMsg_C2S_BlessFriend()
{

}

bool GameMsg_C2S_BlessFriend::doDecode( CParamPool & IOBuff )
{
	m_nFriendID = IOBuff.GetUInt();
	m_nWordsIndex = IOBuff.GetUChar();

	return true;
}

//---------------------------------------------------------GameMsg_S2C_BlessFriendSuc
GameMsg_S2C_BlessFriendSuc::GameMsg_S2C_BlessFriendSuc()
:GameMsg_Base(MSG_S2C_BlessFriendSuc)
,m_nMoney(0)
{

}

GameMsg_S2C_BlessFriendSuc::~GameMsg_S2C_BlessFriendSuc()
{

}

bool GameMsg_S2C_BlessFriendSuc::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUShort((unsigned short)m_listRewards.size());
	for (std::list<CItem>::iterator itr = m_listRewards.begin(); itr != m_listRewards.end(); ++itr) {
		itr->doEncode(IOBuff);
	}
	IOBuff.AddInt(m_nMoney);
	return true;
}

//---------------------------------------------------------GameMsg_S2C_BlessFriendFail
GameMsg_S2C_BlessFriendFail::GameMsg_S2C_BlessFriendFail()
:GameMsg_Base(MSG_S2C_BlessFriendFail)
,m_nError(EBlessFriendError_Success)
{

}

GameMsg_S2C_BlessFriendFail::~GameMsg_S2C_BlessFriendFail()
{

}

bool GameMsg_S2C_BlessFriendFail::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar(m_nError);
	return true;
}

//---------------------------------------------------------GameMsg_S2C_BlessFriendFail
GameMsg_S2C_GetBlessFromFriend::GameMsg_S2C_GetBlessFromFriend()
:GameMsg_Base(MSG_S2C_GetBlessFromFriend)
,m_strSenderName("")
,m_nWordsIndex(0)
{

}

GameMsg_S2C_GetBlessFromFriend::~GameMsg_S2C_GetBlessFromFriend()
{

}

bool GameMsg_S2C_GetBlessFromFriend::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddStringW(m_strSenderName.c_str());
	IOBuff.AddUChar(m_nWordsIndex);

	return true;
}

//---------------------------------------------------------GameMsg_S2C_GetFestivalBlessReward
GameMsg_C2S_GetFestivalBlessReward::GameMsg_C2S_GetFestivalBlessReward()
:GameMsg_Base(MSG_C2S_GetFestivalBlessReward)
,m_nLevel(0)
{

}

GameMsg_C2S_GetFestivalBlessReward::~GameMsg_C2S_GetFestivalBlessReward()
{

}

bool GameMsg_C2S_GetFestivalBlessReward::doDecode( CParamPool &IOBuff )
{
	m_nLevel = IOBuff.GetUChar();

	return true;
}

//---------------------------------------------------------GameMsg_S2C_GetFestivalBlessRewardSuc
GameMsg_S2C_GetFestivalBlessRewardSuc::GameMsg_S2C_GetFestivalBlessRewardSuc()
:GameMsg_Base(MSG_S2C_GetFestivalBlessRewardSuc)
,m_nLevel(0)
{

}

GameMsg_S2C_GetFestivalBlessRewardSuc::~GameMsg_S2C_GetFestivalBlessRewardSuc()
{

}

bool GameMsg_S2C_GetFestivalBlessRewardSuc::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar(m_nLevel);

	return true;
}

//---------------------------------------------------------GameMsg_S2C_GetFestivalBlessRewardFail
GameMsg_S2C_GetFestivalBlessRewardFail::GameMsg_S2C_GetFestivalBlessRewardFail()
:GameMsg_Base(MSG_S2C_GetFestivalBlessRewardFail)
,m_nError(EGetFestivalBlessRewarError_Success)
{

}

GameMsg_S2C_GetFestivalBlessRewardFail::~GameMsg_S2C_GetFestivalBlessRewardFail()
{

}

bool GameMsg_S2C_GetFestivalBlessRewardFail::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar(m_nError);

	return true;
}

//---------------------------------------------------------GameMsg_S2C_FillPuzzleSuc
GameMsg_S2C_SyncPuzzleData::GameMsg_S2C_SyncPuzzleData()
:GameMsg_Base(MSG_S2C_SyncPuzzleData)
{
}

GameMsg_S2C_SyncPuzzleData::~GameMsg_S2C_SyncPuzzleData()
{
}

bool GameMsg_S2C_SyncPuzzleData::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW( m_strPuzzleHelpTitle.c_str() );
	IOBuff.AddStringW( m_strPuzzleHelpContent.c_str() );

	unsigned short nPuzzleCount = (unsigned short)m_vecRolePuzzle.size();
	IOBuff.AddUShort( nPuzzleCount );

	for ( int i = 0; i < nPuzzleCount; ++i )
	{
		m_vecRolePuzzle[i].doEncode( IOBuff );
	}

	return true;
}

//---------------------------------------------------------GameMsg_C2S_FillPuzzle
GameMsg_C2S_FillPuzzle::GameMsg_C2S_FillPuzzle()
:GameMsg_Base(MSG_C2S_FillPuzzle)
,m_nPuzzleID(0)
,m_nPieceIndex(0)
,m_bUseSpecial(false)
{
}

GameMsg_C2S_FillPuzzle::~GameMsg_C2S_FillPuzzle()
{
}

bool GameMsg_C2S_FillPuzzle::doDecode(CParamPool &IOBuff)
{
	m_nPuzzleID = IOBuff.GetInt();
	m_nPieceIndex = IOBuff.GetInt();
	m_bUseSpecial = IOBuff.GetBool();

	return true;
}

//---------------------------------------------------------GameMsg_S2C_FillPuzzleSuc
GameMsg_S2C_FillPuzzleSuc::GameMsg_S2C_FillPuzzleSuc()
:GameMsg_Base(MSG_S2C_FillPuzzleSuc)
,m_nPuzzleID(0)
,m_nPieceIndex(0)
,m_nFinishTime(0)
{
}

GameMsg_S2C_FillPuzzleSuc::~GameMsg_S2C_FillPuzzleSuc()
{
}

bool GameMsg_S2C_FillPuzzleSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt( m_nPuzzleID );
	IOBuff.AddInt( m_nPieceIndex );
	IOBuff.AddInt( m_nFinishTime );

	return true;
}


//---------------------------------------------------------GameMsg_S2C_FillPuzzleFail
GameMsg_S2C_FillPuzzleFail::GameMsg_S2C_FillPuzzleFail()
:GameMsg_Base(MSG_S2C_FillPuzzleFail)
,m_nError(0)
{
}

GameMsg_S2C_FillPuzzleFail::~GameMsg_S2C_FillPuzzleFail()
{
}

bool GameMsg_S2C_FillPuzzleFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar( m_nError );

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 新玩家招募活动
// 1. GetInvitationInviterInfo
GameMsg_C2S_GetInvitationInviterInfo::GameMsg_C2S_GetInvitationInviterInfo()
:GameMsg_Base(MSG_C2S_GetInvitationInviterInfo)
{
}
GameMsg_C2S_GetInvitationInviterInfo::~GameMsg_C2S_GetInvitationInviterInfo()
{
}

bool GameMsg_C2S_GetInvitationInviterInfo::doDecode(CParamPool &IOBuff)
{
	return true;
}


CInvitationInviterRewardInfo::CInvitationInviterRewardInfo():
m_nReceivedFlag(2), m_nLevel(0), m_nCumulation(0), m_nMoney(0)
{
}
CInvitationInviterRewardInfo::~CInvitationInviterRewardInfo()
{
}


bool CInvitationInviterRewardInfo::doEncode(CParamPool& buff) const
{
	buff.AddUChar(m_nReceivedFlag);
	buff.AddUChar((unsigned char)m_nLevel);
	buff.AddInt(m_nCumulation);
	buff.AddInt(m_nMoney);

	buff.AddUShort((unsigned short)m_listItem.size());
	for (list<CItem>::const_iterator itr = m_listItem.begin(); 
		itr != m_listItem.end(); ++itr)
	{
		itr->doEncode(buff);
	}

	return true;
}

GameMsg_S2C_GetInvitationInviterInfoSuc::GameMsg_S2C_GetInvitationInviterInfoSuc()
:GameMsg_Base(MSG_S2C_GetInvitationInviterInfoSuc)
{
}

GameMsg_S2C_GetInvitationInviterInfoSuc::~GameMsg_S2C_GetInvitationInviterInfoSuc()
{
}

bool GameMsg_S2C_GetInvitationInviterInfoSuc::doEncode(CParamPool &buff)
{
	// 玩家信息
	buff.AddStringW(m_strInvitationCode.c_str());
	buff.AddInt(m_nInvitationCount);

	// 活动信息
	//buff.AddStringW(m_strActivityName.c_str());
	buff.AddStringW(m_strActivityContent.c_str());

	buff.AddUShort((unsigned short)m_cumulativeReward.size());
	for (CInvitationInviterRewardInfoMap::const_iterator j = m_cumulativeReward.begin();
		j != m_cumulativeReward.end(); j++)
	{
		j->second.doEncode(buff);
	}

	//随机的人物动画
	buff.AddStringW(m_strShareAnimation.c_str());

	return true;
}

GameMsg_S2C_GetInvitationInviterInfoFail::GameMsg_S2C_GetInvitationInviterInfoFail()
:GameMsg_Base(MSG_S2C_GetInvitationInviterInfoFail)
{
}

GameMsg_S2C_GetInvitationInviterInfoFail::~GameMsg_S2C_GetInvitationInviterInfoFail()
{
}

bool GameMsg_S2C_GetInvitationInviterInfoFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}

// 2. BindInvitationCode
GameMsg_C2S_BindInvitationCode::GameMsg_C2S_BindInvitationCode()
: GameMsg_Base(MSG_C2S_BindInvitationCode)
{
}

GameMsg_C2S_BindInvitationCode::~GameMsg_C2S_BindInvitationCode()
{
}

bool GameMsg_C2S_BindInvitationCode::doDecode(CParamPool &IOBuff)
{
	std::string temp;
	IOBuff.GetStringW(temp);
	if (temp.size() >= MAX_INVITATION_CODE_LEN)
		temp.resize(MAX_INVITATION_CODE_LEN - 1);
	strcpy(m_szInvitationCode, temp.c_str());
	return true;
}

GameMsg_S2C_BindInvitationCodeSuc::GameMsg_S2C_BindInvitationCodeSuc()
: GameMsg_Base(MSG_S2C_BindInvitationCodeSuc)
{
}
GameMsg_S2C_BindInvitationCodeSuc::~GameMsg_S2C_BindInvitationCodeSuc()
{
}
bool GameMsg_S2C_BindInvitationCodeSuc::doEncode(CParamPool &IOBuff)
{
	//m_reward.doEncode(IOBuff);
	return true;
}


GameMsg_S2C_BindInvitationCodeFail::GameMsg_S2C_BindInvitationCodeFail()
: GameMsg_Base(MSG_S2C_BindInvitationCodeFail)
{
}

GameMsg_S2C_BindInvitationCodeFail::~GameMsg_S2C_BindInvitationCodeFail()
{
}

bool GameMsg_S2C_BindInvitationCodeFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}


// 3. GetInvitationInviterReward
GameMsg_C2S_GetInvitationInviterReward::GameMsg_C2S_GetInvitationInviterReward()
:GameMsg_Base(MSG_C2S_GetInvitationInviterReward)
{
}

GameMsg_C2S_GetInvitationInviterReward::~GameMsg_C2S_GetInvitationInviterReward()
{
}

bool GameMsg_C2S_GetInvitationInviterReward::doDecode(CParamPool &IOBuff)
{
	m_nType = IOBuff.GetUChar();
	m_nLevel = IOBuff.GetUChar();
	return true;
}

GameMsg_S2C_GetInvitationInviterRewardSuc::GameMsg_S2C_GetInvitationInviterRewardSuc()
:GameMsg_Base(MSG_S2C_GetInvitationInviterRewardSuc)
{
}

GameMsg_S2C_GetInvitationInviterRewardSuc::~GameMsg_S2C_GetInvitationInviterRewardSuc()
{
}

bool GameMsg_S2C_GetInvitationInviterRewardSuc::doEncode(CParamPool &buff)
{
	buff.AddUChar(m_nType);
	buff.AddUChar(m_nLevel);
	buff.AddInt(m_nCumulation);

	return true;
}


GameMsg_S2C_GetInvitationInviterRewardFail::GameMsg_S2C_GetInvitationInviterRewardFail()
:GameMsg_Base(MSG_S2C_GetInvitationInviterRewardFail)
{
}

GameMsg_S2C_GetInvitationInviterRewardFail::~GameMsg_S2C_GetInvitationInviterRewardFail()
{
}

bool GameMsg_S2C_GetInvitationInviterRewardFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}
///////////////////////////////////////////////////////////////////////////////

GameMsg_C2S_GetFresherReward::GameMsg_C2S_GetFresherReward()
: GameMsg_Base(MSG_C2S_GetFresherReward)
{
	m_nDay = 0;
}
GameMsg_C2S_GetFresherReward::~GameMsg_C2S_GetFresherReward()
{
}

bool GameMsg_C2S_GetFresherReward::doDecode(CParamPool &IOBuff)
{
	m_nDay = IOBuff.GetUChar();
	return true;
}


GameMsg_S2C_GetFresherRewardSuc::GameMsg_S2C_GetFresherRewardSuc()
: GameMsg_Base(MSG_S2C_GetFresherRewardSuc)
{
	m_nDay = 0;
}

GameMsg_S2C_GetFresherRewardSuc::~GameMsg_S2C_GetFresherRewardSuc()
{
}

bool GameMsg_S2C_GetFresherRewardSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nDay);
	return true;
}


GameMsg_S2C_GetFresherRewardFail::GameMsg_S2C_GetFresherRewardFail()
: GameMsg_Base(MSG_S2C_GetFresherRewardFail)
{
	m_strError = "";
}

GameMsg_S2C_GetFresherRewardFail::~GameMsg_S2C_GetFresherRewardFail()
{
}
bool GameMsg_S2C_GetFresherRewardFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}


GameMsg_S2C_GetFresherAllReward::GameMsg_S2C_GetFresherAllReward()
: GameMsg_Base(MSG_S2C_GetFresherAllReward)
{
}

GameMsg_S2C_GetFresherAllReward::~GameMsg_S2C_GetFresherAllReward()
{
}

bool GameMsg_S2C_GetFresherAllReward::doEncode(CParamPool &IOBuff)
{
	return true;
}

GameMsg_C2S_GetExtraActivityInfo::GameMsg_C2S_GetExtraActivityInfo()
: GameMsg_Base(MSG_C2S_GetExtraActivityInfo)
{

}

GameMsg_C2S_GetExtraActivityInfo::~GameMsg_C2S_GetExtraActivityInfo()
{

}

bool GameMsg_C2S_GetExtraActivityInfo::doDecode(CParamPool &IOBuff)
{
	return true;
}

GameMsg_S2C_GetExtraActivityInfoSuc::GameMsg_S2C_GetExtraActivityInfoSuc()
: GameMsg_Base(MSG_S2C_GetExtraActivityInfoSuc)
{
	m_nSceneID = 0;
}

GameMsg_S2C_GetExtraActivityInfoSuc::~GameMsg_S2C_GetExtraActivityInfoSuc()
{

}

bool GameMsg_S2C_GetExtraActivityInfoSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nSceneID);
	return true;
}

//////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_MeetMatch::GameMsg_C2S_MeetMatch()
: GameMsg_Base(MSG_C2S_MeetMatch)
{
}

GameMsg_C2S_MeetMatch::~GameMsg_C2S_MeetMatch()
{

}

bool GameMsg_C2S_MeetMatch::doDecode(CParamPool &IOBuff)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_MeetCancelMatch::GameMsg_C2S_MeetCancelMatch()
: GameMsg_Base(MSG_C2S_MeetCancelMatch)
{
}

GameMsg_C2S_MeetCancelMatch::~GameMsg_C2S_MeetCancelMatch()
{

}

bool GameMsg_C2S_MeetCancelMatch::doDecode(CParamPool &IOBuff)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_MeetLeaveScene::GameMsg_C2S_MeetLeaveScene()
: GameMsg_Base(MSG_C2S_MeetLeaveScene)
{
}

GameMsg_C2S_MeetLeaveScene::~GameMsg_C2S_MeetLeaveScene()
{

}

bool GameMsg_C2S_MeetLeaveScene::doDecode(CParamPool &IOBuff)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_MeetCameraEvent::GameMsg_C2S_MeetCameraEvent()
: GameMsg_Base(MSG_C2S_MeetCameraEvent)
,m_nIndex( 0 )
{
}

GameMsg_C2S_MeetCameraEvent::~GameMsg_C2S_MeetCameraEvent()
{

}

bool GameMsg_C2S_MeetCameraEvent::doDecode(CParamPool &IOBuff)
{
	m_nIndex = IOBuff.GetUInt();
	return true;
}

//////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_MeetEnd::GameMsg_C2S_MeetEnd()
: GameMsg_Base(MSG_C2S_MeetEnd)
{
}

GameMsg_C2S_MeetEnd::~GameMsg_C2S_MeetEnd()
{
}

bool GameMsg_C2S_MeetEnd::doDecode(CParamPool &IOBuff)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_MeetEnterAmuseScene::GameMsg_C2S_MeetEnterAmuseScene()
: GameMsg_Base(MSG_C2S_MeetEnterAmuseScene)
{
}

GameMsg_C2S_MeetEnterAmuseScene::~GameMsg_C2S_MeetEnterAmuseScene()
{
}

bool GameMsg_C2S_MeetEnterAmuseScene::doDecode(CParamPool &IOBuff)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MeetEndResult::GameMsg_S2C_MeetEndResult()
: GameMsg_Base(MSG_S2C_MeetEndResult)
{
}

GameMsg_S2C_MeetEndResult::~GameMsg_S2C_MeetEndResult()
{
}

bool GameMsg_S2C_MeetEndResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort( (unsigned short)m_photosInfo.size() );
	for ( size_t i = 0 ; i < m_photosInfo.size(); ++i )
	{
		IOBuff.AddUInt( m_photosInfo[i].nIndex );
		IOBuff.AddStringW( m_photosInfo[i].strName );
		IOBuff.AddBool( m_photosInfo[i].nIsMalePhoto );
		IOBuff.AddBool( m_photosInfo[i].nIsFemalePhoto );
	}

	IOBuff.AddUShort( (unsigned short)m_listItems.size() );
	std::list<CItem>::iterator it = m_listItems.begin();
	for ( ; it != m_listItems.end() ; ++it )
	{
		it->doEncode( IOBuff );
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MeetMatchSucceed::GameMsg_S2C_MeetMatchSucceed()
: GameMsg_Base(MSG_S2C_MeetMatchSucceed)
{
}

GameMsg_S2C_MeetMatchSucceed::~GameMsg_S2C_MeetMatchSucceed()
{
}

bool GameMsg_S2C_MeetMatchSucceed::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort( (unsigned short)m_MatchPlayers.size());
	for ( size_t i = 0; i < m_MatchPlayers.size(); ++i )
	{
		IOBuff.AddUInt( m_MatchPlayers[i].m_nRoleID );
		IOBuff.AddStringW( m_MatchPlayers[i].m_strRoleName.c_str() );
		IOBuff.AddUChar( m_MatchPlayers[i].m_eRoleSex );
		IOBuff.AddUChar( m_MatchPlayers[i].m_nRoleSkin );
		IOBuff.AddStringW( m_MatchPlayers[i].m_strDanceGroupName.c_str() );
		IOBuff.AddUChar( m_MatchPlayers[i].m_nGroupTitle );
		IOBuff.AddUShort( m_MatchPlayers[i].m_nDanceGroupBadge );
		IOBuff.AddUShort( m_MatchPlayers[i].m_nDanceGroupEffect );
		IOBuff.AddBool( m_MatchPlayers[i].m_bIsHost );
		IOBuff.AddUChar( m_MatchPlayers[i].m_eRoleType );
		IOBuff.AddBool( m_MatchPlayers[i].m_nIsVip );
		IOBuff.AddUShort( m_MatchPlayers[i].m_nVipLv );
		IOBuff.AddUChar( m_MatchPlayers[i].m_nMoveType );

		if( m_MatchPlayers[i].m_pRoleItem != NULL ) 
		{
			m_MatchPlayers[i].m_pRoleItem->PackItemForOther( IOBuff );
		}
		else
		{
			IOBuff.AddUShort( 0 );
		}

		if ( m_MatchPlayers[i].m_pRoleGene != NULL ) 
		{
			m_MatchPlayers[i].m_pRoleGene->PackGeneForOther( IOBuff );
		}
		else
		{
			IOBuff.AddUShort( 0 );
		}
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MeetMatchFailed::GameMsg_S2C_MeetMatchFailed()
: GameMsg_Base(MSG_S2C_MeetMatchFaild)
,m_strError("")
{
}

GameMsg_S2C_MeetMatchFailed::~GameMsg_S2C_MeetMatchFailed()
{
}

bool GameMsg_S2C_MeetMatchFailed::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW( m_strError );
	return true;
}

//////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MeetPhotoSucceed::GameMsg_S2C_MeetPhotoSucceed()
: GameMsg_Base(MSG_S2C_MeetPhotoSucceed)
,m_nPhotoIndex(0)
{
}

GameMsg_S2C_MeetPhotoSucceed::~GameMsg_S2C_MeetPhotoSucceed()
{
}

bool GameMsg_S2C_MeetPhotoSucceed::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt( m_nPhotoIndex );
	return true;
}

//////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_MeetPhotoFailed::GameMsg_S2C_MeetPhotoFailed()
: GameMsg_Base(MSG_S2C_MeetPhotoFaild)
,m_strError("")
{
}

GameMsg_S2C_MeetPhotoFailed::~GameMsg_S2C_MeetPhotoFailed()
{
}

bool GameMsg_S2C_MeetPhotoFailed::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW( m_strError );
	return true;
}

bool CMallSocialShareData::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_listTryCloth;
	return true;
}

bool CMallSocialShareData::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_listTryCloth;
	return true;
}

//////////////////////////////////////////////////////////////////////////

bool CPhotoSocialShareData::doDecode(CParamPool &IOBuff)
{
    IOBuff >> m_nRoomTheme;
    IOBuff >> m_nPeopleNum;

    return true;
}

bool CPhotoSocialShareData::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_nRoomTheme;
    IOBuff << m_nPeopleNum;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_SocialShare::GameMsg_C2S_SocialShare():GameMsg_Base(MSG_C2S_SocialShare)
{

}

GameMsg_C2S_SocialShare::~GameMsg_C2S_SocialShare()
{

}

bool GameMsg_C2S_SocialShare::doDecode( CParamPool &IOBuff )
{
	return true;
}

GameMsg_S2C_SocialShare::GameMsg_S2C_SocialShare():GameMsg_Base(MSG_S2C_SocialShare)
{

}

GameMsg_S2C_SocialShare::~GameMsg_S2C_SocialShare()
{

}

bool GameMsg_S2C_SocialShare::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddStringW(m_strTip);
	return true;
}

GameMsg_C2S_ActivitySocialShare::GameMsg_C2S_ActivitySocialShare():GameMsg_Base(MSG_C2S_ActivitySocialShare)
{
	m_nUI = 0;
	m_pData = NULL;
}

GameMsg_C2S_ActivitySocialShare::~GameMsg_C2S_ActivitySocialShare()
{
	if ( m_pData != NULL )
	{
		delete m_pData;
		m_pData = NULL;
	}
}

bool GameMsg_C2S_ActivitySocialShare::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_nUI;
	if ( m_nUI == EUISocialShare_Mall )
	{
		if ( m_pData == NULL )
		{
			m_pData = new CMallSocialShareData();
		}
		m_pData->doDecode(IOBuff);
	}
    else if (EUISocialShare_Photo == m_nUI)
    {
        if (NULL == m_pData)
            m_pData = new CPhotoSocialShareData();

        m_pData->doDecode(IOBuff);
    }
	return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_ActivitySocialShare::GameMsg_S2C_ActivitySocialShare()
    : GameMsg_Base(MSG_S2C_ActivitySocialShare)
    , m_nUI(0)
{
}

GameMsg_S2C_ActivitySocialShare::~GameMsg_S2C_ActivitySocialShare()
{
}

bool GameMsg_S2C_ActivitySocialShare::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nUI);

    IOBuff.AddUShort((unsigned short)m_listRewards.size() );
    for (std::list<CReward>::iterator itr = m_listRewards.begin(); 
         itr != m_listRewards.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_CanSocialShareFirst::GameMsg_S2C_CanSocialShareFirst():GameMsg_Base(MSG_S2C_CanSocialShareFirst)
{
    m_nUI = 0;
    m_bCanDoFirstShare = false;
}

GameMsg_S2C_CanSocialShareFirst::~GameMsg_S2C_CanSocialShareFirst()
{

}

bool GameMsg_S2C_CanSocialShareFirst::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_nUI << m_bCanDoFirstShare;
    IOBuff.AddStringW(m_strActionName);
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_RequestVIPExtraReward::GameMsg_C2S_RequestVIPExtraReward()
    : GameMsg_Base(MSG_C2S_RequestVipExtraReward)
    , m_nOpenVipFromType(EOpenVIPFromType_None)
{
}

bool GameMsg_C2S_RequestVIPExtraReward::doDecode(CParamPool &IOBuff)
{
    m_nOpenVipFromType = IOBuff.GetUChar();

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_RequestVIPExtraRewardRes::GameMsg_S2C_RequestVIPExtraRewardRes()
    : GameMsg_Base(MSG_S2C_RequestVipExtraRewardRes)
    , m_nErrorType(ERequestVIPExtraRewardErrorType_Success)
    , m_nOpenVipFromType(EOpenVIPFromType_None)
{
}

bool GameMsg_S2C_RequestVIPExtraRewardRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorType);
    IOBuff.AddUChar(m_nOpenVipFromType);

    IOBuff.AddUShort((unsigned short)m_listRewards.size() );
    for (std::list<CReward>::iterator itr = m_listRewards.begin(); 
         itr != m_listRewards.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    return true;
}

GameMsg_C2S_GetLongactingCumulativeRecharge::GameMsg_C2S_GetLongactingCumulativeRecharge()
    :GameMsg_Base( MSG_C2S_GetLongactingCumulativeRecharge )
{

}

GameMsg_C2S_GetLongactingCumulativeRecharge::~GameMsg_C2S_GetLongactingCumulativeRecharge()
{
}

bool GameMsg_C2S_GetLongactingCumulativeRecharge::doDecode(CParamPool &IOBuff)
{

    return true;
}

GameMsg_S2C_GetLongactingCumulativeRechargeResult::GameMsg_S2C_GetLongactingCumulativeRechargeResult()
    :GameMsg_Base( MSG_S2C_GetLongactingCumulativeRechargeResult)
    ,m_nHaveRewardIndex(0)
    ,m_cSex(0)
{

}

GameMsg_S2C_GetLongactingCumulativeRechargeResult::~GameMsg_S2C_GetLongactingCumulativeRechargeResult()
{
}

bool GameMsg_S2C_GetLongactingCumulativeRechargeResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listLongactingRechargeConfig.size());
    std::list<LongactingCumulativeRechargeConfig>::iterator it = m_listLongactingRechargeConfig.begin();
    for (; it != m_listLongactingRechargeConfig.end(); ++it)
    {
        it->doEncode(IOBuff, m_cSex);
    }

    IOBuff.AddUInt(m_nHaveRewardIndex);
    return true;
}

GameMsg_C2S_GetLongactingCumulativeRechargeReward::GameMsg_C2S_GetLongactingCumulativeRechargeReward()
    :GameMsg_Base( MSG_C2S_GetLongactingCumulativeRechargeReward )
    ,m_nRewardIndex(0)
{

}

GameMsg_C2S_GetLongactingCumulativeRechargeReward::~GameMsg_C2S_GetLongactingCumulativeRechargeReward()
{

}

bool GameMsg_C2S_GetLongactingCumulativeRechargeReward::doDecode(CParamPool &IOBuff)
{
    m_nRewardIndex = IOBuff.GetUInt();

    return true;
}

GameMsg_S2C_GetLongactingCumulativeRechargeRewardFail::GameMsg_S2C_GetLongactingCumulativeRechargeRewardFail()
    :GameMsg_Base( MSG_S2C_GetLongactingCumulativeRechargeFail )
    ,m_nErrorFlag(0)
{

}

GameMsg_S2C_GetLongactingCumulativeRechargeRewardFail::~GameMsg_S2C_GetLongactingCumulativeRechargeRewardFail()
{

}

bool GameMsg_S2C_GetLongactingCumulativeRechargeRewardFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nErrorFlag);

    return true;
}

GameMsg_S2C_GetLongactingCumulativeRechargeRewardScuess::GameMsg_S2C_GetLongactingCumulativeRechargeRewardScuess()
    :GameMsg_Base( MSG_S2C_GetLongactingCumulativeRechargeSucess )
{

}

GameMsg_S2C_GetLongactingCumulativeRechargeRewardScuess::~GameMsg_S2C_GetLongactingCumulativeRechargeRewardScuess()
{

}

bool GameMsg_S2C_GetLongactingCumulativeRechargeRewardScuess::doEncode(CParamPool &IOBuff)
{
    return true;
}


GameMsg_C2S_GetLongactingCumulativeRechargeBoardCast::GameMsg_C2S_GetLongactingCumulativeRechargeBoardCast()
    :GameMsg_Base( MSG_C2S_GetLongactingCumulativeRechargeBroadcase )
{

}

GameMsg_C2S_GetLongactingCumulativeRechargeBoardCast::~GameMsg_C2S_GetLongactingCumulativeRechargeBoardCast()
{

}

bool GameMsg_C2S_GetLongactingCumulativeRechargeBoardCast::doDecode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_GetLongactingCumulativeRechargeBCResult::GameMsg_S2C_GetLongactingCumulativeRechargeBCResult()
    :GameMsg_Base( MSG_S2C_GetLongactingCumulativeRechargeBCResult )
{

}

GameMsg_S2C_GetLongactingCumulativeRechargeBCResult::~GameMsg_S2C_GetLongactingCumulativeRechargeBCResult()
{

}

bool GameMsg_S2C_GetLongactingCumulativeRechargeBCResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listLongactingBoardCast.size());
    std::list<LongactingCumulativeRechargeBoardCast>::iterator it = m_listLongactingBoardCast.begin();
    for (; it != m_listLongactingBoardCast.end(); ++it)
    {
        it->EncodeInfo(IOBuff);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_RequestBuyItemGotMedalReward::GameMsg_C2S_RequestBuyItemGotMedalReward()
    : GameMsg_Base( MSG_C2S_RequestBuyItemGotMedalReward )
    , m_nIndex( 0 )
{
}

GameMsg_C2S_RequestBuyItemGotMedalReward::~GameMsg_C2S_RequestBuyItemGotMedalReward()
{
}

bool GameMsg_C2S_RequestBuyItemGotMedalReward::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nIndex;

    return true;
}

GameMsg_S2C_RequestBuyItemGotMedalRewardSuc::GameMsg_S2C_RequestBuyItemGotMedalRewardSuc()
    : GameMsg_Base( MSG_S2C_RequestBuyItemGotMedalRewardSuc )
    , m_nIndex( 0 )
{
}

GameMsg_S2C_RequestBuyItemGotMedalRewardSuc::~GameMsg_S2C_RequestBuyItemGotMedalRewardSuc()
{
}

bool GameMsg_S2C_RequestBuyItemGotMedalRewardSuc::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_nIndex;

    return true;
}

GameMsg_S2C_RequestBuyItemGotMedalRewardFail::GameMsg_S2C_RequestBuyItemGotMedalRewardFail()
    : GameMsg_Base( MSG_S2C_RequestBuyItemGotMedalRewardFail )
    , m_strError( "" )
{
}

GameMsg_S2C_RequestBuyItemGotMedalRewardFail::~GameMsg_S2C_RequestBuyItemGotMedalRewardFail()
{
}

bool GameMsg_S2C_RequestBuyItemGotMedalRewardFail::doEncode( CParamPool &IOBuff )
{
    IOBuff << stringw( m_strError );

    return true;
}

GameMsg_S2C_UpdateBuyItemGotMedalCurrentCount::GameMsg_S2C_UpdateBuyItemGotMedalCurrentCount()
    : GameMsg_Base( MSG_S2C_UpdateBuyItemGotMedalCurrentCount )
    , m_nIndex( 0 )
    , m_nCurrentCount( 0 )
{
}

GameMsg_S2C_UpdateBuyItemGotMedalCurrentCount::~GameMsg_S2C_UpdateBuyItemGotMedalCurrentCount()
{
}

bool GameMsg_S2C_UpdateBuyItemGotMedalCurrentCount::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_nIndex << m_nCurrentCount;

    return true;
}

