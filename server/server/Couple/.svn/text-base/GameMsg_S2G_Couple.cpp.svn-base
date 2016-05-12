#include "GameMsg_S2G_Couple.h"
#include "../../socket/ParamPool.h"
#include "CoupleMsgDef.h"

GameMsg_S2G_AcceptCoupleRequest::GameMsg_S2G_AcceptCoupleRequest()
:GameMsg_Base(MSG_S2G_AcceptCoupleRequst)
,m_nPairID(0)
,m_nMaleID(0)
,m_nFemaleID(0)
,m_nCoupleDate(0)
,m_nSmallRoleID(0)
{

}

GameMsg_S2G_AcceptCoupleRequest::~GameMsg_S2G_AcceptCoupleRequest()
{

}

bool GameMsg_S2G_AcceptCoupleRequest::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nPairID);
	IOBuff.AddUInt(m_nMaleID);
	IOBuff.AddUInt(m_nFemaleID);
	IOBuff.AddUInt(m_nCoupleDate);
    IOBuff.AddUInt(m_nSmallRoleID);

	return true;
}

bool GameMsg_S2G_AcceptCoupleRequest::doDecode( CParamPool &IOBuff )
{
	m_nPairID = IOBuff.GetUInt();
	m_nMaleID = IOBuff.GetUInt();
	m_nFemaleID = IOBuff.GetUInt();
	m_nCoupleDate = IOBuff.GetUInt();
    m_nSmallRoleID = IOBuff.GetUInt();

	return true;
}

////////////////////////////////////////////////////////////////
GameMsg_S2G_CoupleSeprate::GameMsg_S2G_CoupleSeprate()
:GameMsg_Base(MSG_S2G_CoupleSeparate)
,m_nPairID(0)
,m_nSeparateID(0)
,m_nToBeSeparateID(0)
,m_bNeedNotify(false)
{

}

GameMsg_S2G_CoupleSeprate::~GameMsg_S2G_CoupleSeprate()
{

}

bool GameMsg_S2G_CoupleSeprate::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUInt(m_nPairID);
	IOBuff.AddUInt(m_nSeparateID);
	IOBuff.AddUInt(m_nToBeSeparateID);
	IOBuff.AddBool(m_bNeedNotify);

	return true;
}

bool GameMsg_S2G_CoupleSeprate::doDecode( CParamPool &IOBuff )
{
    m_nPairID = IOBuff.GetUInt();
	m_nSeparateID = IOBuff.GetUInt();
	m_nToBeSeparateID = IOBuff.GetUInt();
	m_bNeedNotify = IOBuff.GetBool();

	return true;
}

////////////////////////////////////////////////////////////////
GameMsg_S2G_GetMarried::GameMsg_S2G_GetMarried()
:GameMsg_Base(MSG_S2G_GetMarried)
,m_nMaleRoleID(0)
,m_nFemaleRoleID(0)
,m_nMarriageDate(0)
,m_nBlessingValue(0)
{

}

GameMsg_S2G_GetMarried::~GameMsg_S2G_GetMarried()
{

}

bool GameMsg_S2G_GetMarried::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt( m_nPairID );
	IOBuff.AddUInt(m_nMaleRoleID);
	IOBuff.AddStringW( m_strMaleRoleName.c_str() );
	IOBuff.AddUInt(m_nFemaleRoleID);
	IOBuff.AddStringW( m_strFemaleRoleName.c_str() );
	IOBuff.AddUInt(m_nMarriageDate);
	IOBuff.AddUInt( m_nBlessingValue);

	return true;
}

bool GameMsg_S2G_GetMarried::doDecode( CParamPool &IOBuff )
{
	m_nPairID = IOBuff.GetUInt();
	m_nMaleRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strMaleRoleName );
	m_nFemaleRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strFemaleRoleName );
	m_nMarriageDate = IOBuff.GetUInt();
	m_nBlessingValue = IOBuff.GetUInt();
	
	return true;
}


////////////////////////////////////////////////////////////////
GameMsg_S2G_Divorced::GameMsg_S2G_Divorced()
:GameMsg_Base(MSG_S2G_Divorce)
,m_nPairID(0)
,m_nDivorceID(0)
,m_nBeDivorcedID(0)
,m_bNeedNotify(false)
{

}

GameMsg_S2G_Divorced::~GameMsg_S2G_Divorced()
{

}

bool GameMsg_S2G_Divorced::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt( m_nPairID );
	IOBuff.AddUInt(m_nDivorceID);
	IOBuff.AddUInt(m_nBeDivorcedID);
	IOBuff.AddBool(m_bNeedNotify);

	return true;
}

bool GameMsg_S2G_Divorced::doDecode( CParamPool &IOBuff )
{
	m_nPairID    = IOBuff.GetUInt();
	m_nDivorceID = IOBuff.GetUInt();
	m_nBeDivorcedID = IOBuff.GetUInt();
	m_bNeedNotify = IOBuff.GetBool();

	return true;
}

////////////////////////////////////////////////////////////////
GameMsg_S2G_WeddingRoomStartingNofication::GameMsg_S2G_WeddingRoomStartingNofication()
:GameMsg_Base(MSG_S2G_WeddingRoomStartingNofication)
,m_nGroomID(0)
,m_nBrideID(0)
,m_nLine(-1)
,m_nRoomID(0)
,m_nStartTime(0)
{

}

GameMsg_S2G_WeddingRoomStartingNofication::~GameMsg_S2G_WeddingRoomStartingNofication()
{

}

bool GameMsg_S2G_WeddingRoomStartingNofication::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nGroomID);
	IOBuff.AddUInt(m_nBrideID);
	IOBuff.AddInt(m_nLine);
	IOBuff.AddStringW(m_strLineName);
	IOBuff.AddUInt(m_nRoomID);
	IOBuff.AddUInt(m_nStartTime);

	return true;
}

bool GameMsg_S2G_WeddingRoomStartingNofication::doDecode( CParamPool &IOBuff )
{
	m_nGroomID = IOBuff.GetUInt();
	m_nBrideID = IOBuff.GetUInt();
	m_nLine = IOBuff.GetInt();
	IOBuff.GetStringW(m_strLineName);
	m_nRoomID = IOBuff.GetUInt();
	m_nStartTime = IOBuff.GetUInt();

	return true;
}

////////////////////////////////////////////////////////////////
GameMsg_S2G_AutoStartWeddingFail::GameMsg_S2G_AutoStartWeddingFail()
:GameMsg_Base(MSG_S2G_AutoStartWeddingRoomFail)
,m_nGroomID(0)
,m_nBrideID(0)
,m_nError(0)
,m_strError("")
{

}

GameMsg_S2G_AutoStartWeddingFail::~GameMsg_S2G_AutoStartWeddingFail()
{
	
}

bool GameMsg_S2G_AutoStartWeddingFail::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nGroomID);
	IOBuff.AddUInt(m_nBrideID);
	IOBuff.AddUChar(m_nError);
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

bool GameMsg_S2G_AutoStartWeddingFail::doDecode( CParamPool &IOBuff )
{
	m_nGroomID = IOBuff.GetUInt();
	m_nBrideID = IOBuff.GetUInt();
	m_nError = IOBuff.GetUChar();
	IOBuff.GetStringW(m_strError);

	return true;
}


////////////////////////////////////////////////////////////////
GameMsg_S2G_UpdateCoupleIntimacy::GameMsg_S2G_UpdateCoupleIntimacy()
:GameMsg_Base(MSG_S2G_UpdateCoupleIntimacy)
, m_nPairID(0)
,m_nTriggerID(0)
,m_nCoupleID(0)
,m_nChangedValue(0)
, m_bMarried(false)
, m_bMale(false)
, m_nSmallRoleID(0)
{

}

GameMsg_S2G_UpdateCoupleIntimacy::~GameMsg_S2G_UpdateCoupleIntimacy()
{

}

bool GameMsg_S2G_UpdateCoupleIntimacy::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUInt(m_nPairID);
	IOBuff.AddUInt(m_nTriggerID);
	IOBuff.AddUInt(m_nCoupleID);
	IOBuff.AddInt(m_nChangedValue);	
    IOBuff.AddBool(m_bMarried);
    IOBuff.AddBool(m_bMale);
    IOBuff.AddUInt(m_nSmallRoleID);

	return true;
}

bool GameMsg_S2G_UpdateCoupleIntimacy::doDecode( CParamPool &IOBuff )
{
    m_nPairID = IOBuff.GetUInt();
	m_nTriggerID = IOBuff.GetUInt();
	m_nCoupleID = IOBuff.GetUInt();
	m_nChangedValue = IOBuff.GetInt();
    m_bMarried = IOBuff.GetBool();
    m_bMale = IOBuff.GetBool();
    m_nSmallRoleID = IOBuff.GetUInt();

	return true;
}

///////////////////////////////////////////////////
GameMsg_S2G_BlessingNotify::GameMsg_S2G_BlessingNotify()
:GameMsg_Base( MSG_S2G_BlessingNotify )
,m_nMaleRoleID(0)
,m_strManName("")
,m_nFemaleRoleID(0)
,m_strWomanName("")
,m_nPointIndex(0)
{

}

bool GameMsg_S2G_BlessingNotify::doDecode( CParamPool& IOBuff )
{
	m_nMaleRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strManName );
	m_nFemaleRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strWomanName );
	m_nPointIndex = IOBuff.GetUInt();

	return true;
}

bool GameMsg_S2G_BlessingNotify::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt(m_nMaleRoleID);
	IOBuff.AddStringW( m_strManName.c_str() );
	IOBuff.AddUInt(m_nFemaleRoleID);
	IOBuff.AddStringW( m_strWomanName.c_str() );
	IOBuff.AddUInt( m_nPointIndex );

	return true;
}

///////////////////////////////////////////////////
GameMsg_S2G_UpdateWeddingRecord::GameMsg_S2G_UpdateWeddingRecord()
:GameMsg_Base( MSG_S2G_UpdateWeddingRecord )
,m_nBlessingType(0)
,m_nPairID(0)
,m_nBlessingValue( 0 )
,m_nBlessingRoleID( 0 )
{

}

bool GameMsg_S2G_UpdateWeddingRecord::doDecode( CParamPool& IOBuff )
{
	m_nBlessingType = IOBuff.GetUInt();
	m_nPairID = IOBuff.GetUInt();
	m_nBlessingValue = IOBuff.GetUInt();
	m_nBlessingRoleID = IOBuff.GetUInt();
	return true;
}

bool GameMsg_S2G_UpdateWeddingRecord::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nBlessingType );
	IOBuff.AddUInt( m_nPairID );
	IOBuff.AddUInt( m_nBlessingValue );
	IOBuff.AddUInt( m_nBlessingRoleID );
	return true;
}


///////////////////////////////////////////////////
GameMsg_S2G_BlessingCouple::GameMsg_S2G_BlessingCouple()
:GameMsg_Base( MSG_S2G_BlessingPair )
,m_nRoleID( 0 )
,m_nSex( 0 )
,m_nPairID( 0 )
{

}

bool GameMsg_S2G_BlessingCouple::doDecode( CParamPool& IOBuff )
{
	m_nRoleID  = IOBuff.GetUInt();
	m_nSex     = IOBuff.GetUInt();
	m_nPairID  = IOBuff.GetUInt();

	return true;
}

bool GameMsg_S2G_BlessingCouple::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nRoleID );
	IOBuff.AddUInt( m_nSex );
	IOBuff.AddUInt( m_nPairID );

	return true;
}

//////////////////////////////////////////////////////
GameMsg_S2G_GetWeddingRecordList::GameMsg_S2G_GetWeddingRecordList()
:GameMsg_Base( MSG_S2G_GetWeddingRecordList )
,m_nRoleID( 0 )
{

}

bool GameMsg_S2G_GetWeddingRecordList::doDecode( CParamPool& IOBuff )
{
	m_nRoleID  = IOBuff.GetUInt();

	return true;
}

bool GameMsg_S2G_GetWeddingRecordList::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nRoleID );
	return true;
}

//////////////////////////////////////////////////////
GameMsg_S2G_AddWeddingRecord::GameMsg_S2G_AddWeddingRecord()
:GameMsg_Base( MSG_S2G_AddWeddingRecord )
{

}

bool GameMsg_S2G_AddWeddingRecord::doDecode( CParamPool& IOBuff )
{
	m_Record.m_nPairID        = IOBuff.GetUInt();
 	m_Record.m_nIsRunning     = IOBuff.GetUInt();
 	m_Record.m_nMaleID        = IOBuff.GetUInt();
	IOBuff.GetStringW( m_Record.m_strMaleName );
 	m_Record.m_nFemaleID      = IOBuff.GetUInt();
 	IOBuff.GetStringW( m_Record.m_strFemaleName );
 	int nCount = (int)IOBuff.GetUShort();
 	for( int i = 0; i < nCount; ++i )
 	{
 		m_Record.m_BlessingRoleIDList.push_back( IOBuff.GetUInt());
 	}
 	m_Record.m_nBlessingValue = IOBuff.GetUInt();
 	m_Record.m_nLine          = IOBuff.GetInt();
    m_Record.m_nSmallRoleID   = IOBuff.GetUInt();

	return true;
}

bool GameMsg_S2G_AddWeddingRecord::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_Record.m_nPairID );
	IOBuff.AddUInt( m_Record.m_nIsRunning );
	IOBuff.AddUInt( m_Record.m_nMaleID );
	IOBuff.AddStringW( m_Record.m_strMaleName.c_str());
	IOBuff.AddUInt( m_Record.m_nFemaleID );
	IOBuff.AddStringW( m_Record.m_strFemaleName.c_str() );
    IOBuff.AddUShort((unsigned short)m_Record.m_BlessingRoleIDList.size());
	for( size_t i = 0; i < m_Record.m_BlessingRoleIDList.size(); ++i )
	{
		IOBuff.AddUInt( m_Record.m_BlessingRoleIDList[i]);
	}
    IOBuff.AddUInt( m_Record.m_nBlessingValue );
	IOBuff.AddUInt( m_Record.m_nLine );
    IOBuff.AddUInt( m_Record.m_nSmallRoleID );

	return true;
}

//////////////////////////////////////////////////////
GameMsg_S2G_UpdateWeddingRecordState::GameMsg_S2G_UpdateWeddingRecordState()
:GameMsg_Base( MSG_S2G_UpateWeddingRecordState )
,m_nDoType( 0 )
,m_nPairID( 0 )
,m_bIsRunning( false )
{

}

bool GameMsg_S2G_UpdateWeddingRecordState::doDecode( CParamPool& IOBuff )
{
	m_nDoType          = IOBuff.GetUInt();
	m_nPairID          = IOBuff.GetUInt();
	m_bIsRunning       = IOBuff.GetBool();
	return true;
}

bool GameMsg_S2G_UpdateWeddingRecordState::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nDoType );
	IOBuff.AddUInt( m_nPairID );
	IOBuff.AddBool( m_bIsRunning );
	return true;
}

//////////////////////////////////////////////////////
GameMsg_S2G_GetBlessingRankList::GameMsg_S2G_GetBlessingRankList()
:GameMsg_Base( MSG_S2G_GetBlessingRankList )
,m_nCurrentPage(0)
,m_nRoleID( 0 )
,m_nPairID( 0 )
,m_nPageCount(0)
{

}

bool GameMsg_S2G_GetBlessingRankList::doDecode( CParamPool& IOBuff )
{
	m_nCurrentPage = IOBuff.GetUShort();
	m_nRoleID          = IOBuff.GetUInt();
	m_nPairID          = IOBuff.GetUInt();
    m_nPageCount = IOBuff.GetUShort();
	return true;
}

bool GameMsg_S2G_GetBlessingRankList::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUShort( m_nCurrentPage );
	IOBuff.AddUInt( m_nRoleID );
	IOBuff.AddUInt( m_nPairID );
    IOBuff.AddUShort(m_nPageCount);
	return true;
}

//////////////////////////////////////////////////////
GameMsg_S2G_RedEnvelope::GameMsg_S2G_RedEnvelope()
:GameMsg_Base( MSG_S2G_MoneyBlessingPair )
,m_nRoleID( 0 )
,m_nPairID( 0 )
,m_nRedEnvelopeCount( 0 )
{

}

bool GameMsg_S2G_RedEnvelope::doDecode( CParamPool& IOBuff )
{
	m_nRoleID          = IOBuff.GetUInt();
	m_nPairID          = IOBuff.GetUInt();
	m_nRedEnvelopeCount           = IOBuff.GetUInt();
	return true;
}

bool GameMsg_S2G_RedEnvelope::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nRoleID );
	IOBuff.AddUInt( m_nPairID );
	IOBuff.AddUInt( m_nRedEnvelopeCount );
	return true;
}

//////////////////////////////////////////////////////
GameMsg_S2G_GetCoupleInfo::GameMsg_S2G_GetCoupleInfo()
:GameMsg_Base( MSG_S2G_GetCoupleInfo )
,m_nRoleID( 0 )
,m_nPairID( 0 )
{

}

bool GameMsg_S2G_GetCoupleInfo::doDecode( CParamPool& IOBuff )
{
	m_nRoleID          = IOBuff.GetUInt();
	m_nPairID           = IOBuff.GetUInt();
	return true;
}

bool GameMsg_S2G_GetCoupleInfo::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nRoleID );
	IOBuff.AddUInt( m_nPairID );
	return true;
}

//////////////////////////////////////////////////////
 GameMsg_S2G_GetBlessingValue::GameMsg_S2G_GetBlessingValue()
:GameMsg_Base( MSG_S2G_GetBlessingValue )
,m_nRoleID( 0 )
,m_nPairID( 0 )
{

}

bool GameMsg_S2G_GetBlessingValue::doDecode( CParamPool& IOBuff )
{
	m_nRoleID = IOBuff.GetUInt();          
	m_nPairID = IOBuff.GetUInt();
	return true;
}

bool GameMsg_S2G_GetBlessingValue::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nRoleID );
	IOBuff.AddUInt( m_nPairID );
	return true;
}

//////////////////////////////////////////////////////
GameMsg_S2G_GetPersonInfo::GameMsg_S2G_GetPersonInfo()
:GameMsg_Base( MSG_S2G_GetPersonInfo )
,m_nSelfRoleID( 0 )
,m_nRoleID( 0 )
{

}

bool GameMsg_S2G_GetPersonInfo::doDecode( CParamPool& IOBuff )
{
	m_nSelfRoleID = IOBuff.GetUInt();
	m_nRoleID     = IOBuff.GetUInt();          
	return true;
}

bool GameMsg_S2G_GetPersonInfo::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nSelfRoleID );
	IOBuff.AddUInt( m_nRoleID );
	return true;
}

//////////////////////////////////////////////////////
GameMsg_S2G_GetDivorceAnimationPersonInfo::GameMsg_S2G_GetDivorceAnimationPersonInfo()
	:GameMsg_Base( MSG_S2G_GetDivorceAnimationPersonInfo )
	,m_nSelfRoleID( 0 )
	,m_nCoupleRoleID( 0 )
{

}

bool GameMsg_S2G_GetDivorceAnimationPersonInfo::doDecode( CParamPool& IOBuff )
{
	m_nSelfRoleID = IOBuff.GetUInt();
	m_nCoupleRoleID     = IOBuff.GetUInt();          
	return true;
}

bool GameMsg_S2G_GetDivorceAnimationPersonInfo::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nSelfRoleID );
	IOBuff.AddUInt( m_nCoupleRoleID );
	return true;
}

//////////////////////////////////////////////////////
GameMsg_S2G_WeddingNotice::GameMsg_S2G_WeddingNotice()
:GameMsg_Base( MSG_S2G_WeddingNotice )
,m_nRoleID(0)
{

}

bool GameMsg_S2G_WeddingNotice::doDecode( CParamPool& IOBuff )
{
	m_nRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strMaleRoleName );      
	IOBuff.GetStringW( m_strFeMaleRoleName );
	return true;
}

bool GameMsg_S2G_WeddingNotice::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nRoleID );
	IOBuff.AddStringW( m_strMaleRoleName.c_str() );
	IOBuff.AddStringW( m_strFeMaleRoleName.c_str() );

	return true;
}

//////////////////////////////////////////////////////
GameMsg_S2G_AddRedEnvelopeBlessingValue::GameMsg_S2G_AddRedEnvelopeBlessingValue()
	:GameMsg_Base( MSG_S2G_AddRedEnvelopeBlessingValue )
	,m_nBlessingRoleID(0)
	,m_nPairID(0)
	,m_nRedEnvelopeCount(0)
{

}

bool GameMsg_S2G_AddRedEnvelopeBlessingValue::doDecode( CParamPool& IOBuff )
{
	m_nBlessingRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strBlessingRoleName);
	m_nPairID = IOBuff.GetUInt();
	m_nRedEnvelopeCount = IOBuff.GetUInt();

	return true;
}

bool GameMsg_S2G_AddRedEnvelopeBlessingValue::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nBlessingRoleID );
	IOBuff.AddStringW( m_strBlessingRoleName.c_str());
	IOBuff.AddUInt( m_nPairID );
	IOBuff.AddUInt( m_nRedEnvelopeCount );

	return true;
}

//////////////////////////////////////////////////////
GameMsg_S2G_RefreshDivorceAction::GameMsg_S2G_RefreshDivorceAction()
	:GameMsg_Base( MSG_S2G_RefreshDivorceAction )
{

}

bool GameMsg_S2G_RefreshDivorceAction::doDecode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nRoleID );
	m_DivorceAgreementInfo.doDecode( IOBuff );
	m_action.doDecode( IOBuff );
	return true;
}

bool GameMsg_S2G_RefreshDivorceAction::doEncode( CParamPool& IOBuff )
{
	m_nRoleID = IOBuff.GetUInt();
	m_DivorceAgreementInfo.doEncode( IOBuff );
	m_action.doEncode( IOBuff );

	return true;
}
