#include "GameMsg_G2S_Couple.h"
#include "../../socket/ParamPool.h"
#include "CoupleMsgDef.h"

///////////////////////////////////////////////////////////////////////////
GameMsg_G2S_NotifyHasBeenSeparated::GameMsg_G2S_NotifyHasBeenSeparated()
:GameMsg_Base(MSG_G2S_NotifyHasBeenSeparated)
,m_nSeparatedBy(0)
,m_strSeparatedBy("")
{

}

GameMsg_G2S_NotifyHasBeenSeparated::~GameMsg_G2S_NotifyHasBeenSeparated()
{
}

bool GameMsg_G2S_NotifyHasBeenSeparated::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nSeparatedBy);
	IOBuff.AddStringW(m_strSeparatedBy.c_str());

	return true;
}

bool GameMsg_G2S_NotifyHasBeenSeparated::doDecode( CParamPool &IOBuff )
{
	m_nSeparatedBy = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strSeparatedBy);

	return true;
}

///////////////////////////////////////////////////////////////////////////
GameMsg_G2S_NotifyHasBeenDivorced::GameMsg_G2S_NotifyHasBeenDivorced()
:GameMsg_Base(MSG_G2S_NofiyHasBeenDivorced)
,m_nDivorcedBy(0)
,m_strDivorcedBy("")
{

}

GameMsg_G2S_NotifyHasBeenDivorced::~GameMsg_G2S_NotifyHasBeenDivorced()
{

}

bool GameMsg_G2S_NotifyHasBeenDivorced::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nDivorcedBy);
	IOBuff.AddStringW(m_strDivorcedBy.c_str());

	return true;
}

bool GameMsg_G2S_NotifyHasBeenDivorced::doDecode( CParamPool &IOBuff )
{
	m_nDivorcedBy = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strDivorcedBy);

	return true;
}

///////////////////////////////////////////////////////////////////////////
GameMsg_G2S_WeddingRoomStartingNofication::GameMsg_G2S_WeddingRoomStartingNofication()
:GameMsg_Base(MSG_G2S_WeddingRoomStaringNofication)
,m_nLine(-1)
,m_nRoomID(0)
,m_nStartTime(0)
{

}

GameMsg_G2S_WeddingRoomStartingNofication::~GameMsg_G2S_WeddingRoomStartingNofication()
{

}

bool GameMsg_G2S_WeddingRoomStartingNofication::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddInt(m_nLine);
	IOBuff.AddStringW(m_strLineName);
	IOBuff.AddUInt(m_nRoomID);
	IOBuff.AddUInt(m_nStartTime);
	
	return true;
}

bool GameMsg_G2S_WeddingRoomStartingNofication::doDecode( CParamPool &IOBuff )
{
	m_nLine = IOBuff.GetInt();
	IOBuff.GetStringW(m_strLineName);
	m_nRoomID = IOBuff.GetUInt();
	m_nStartTime = IOBuff.GetUInt();

	return true;
}

//////////////////////////////////////////////////////////////////
GameMsg_G2S_AutoStartWeddingFail::GameMsg_G2S_AutoStartWeddingFail()
:GameMsg_Base(MSG_G2S_AutoStartWeddingRoomFail)
,m_nError(0)
,m_strError("")
{

}

GameMsg_G2S_AutoStartWeddingFail::~GameMsg_G2S_AutoStartWeddingFail()
{

}

bool GameMsg_G2S_AutoStartWeddingFail::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar(m_nError);
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

bool GameMsg_G2S_AutoStartWeddingFail::doDecode( CParamPool &IOBuff )
{

	m_nError = IOBuff.GetUChar();
	IOBuff.GetStringW(m_strError);

	return true;
}

//////////////////////////////////////////////////////////////////
GameMsg_G2S_UpdateCoupleIntimacy::GameMsg_G2S_UpdateCoupleIntimacy()
:GameMsg_Base(MSG_G2S_UpdateCoupleIntimacy)
,m_nChangedValue(0)
{

}

GameMsg_G2S_UpdateCoupleIntimacy::~GameMsg_G2S_UpdateCoupleIntimacy()
{

}

bool GameMsg_G2S_UpdateCoupleIntimacy::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddInt(m_nChangedValue);

	return true;
}

bool GameMsg_G2S_UpdateCoupleIntimacy::doDecode( CParamPool &IOBuff )
{
	m_nChangedValue = IOBuff.GetInt();

	return true;
}

GameMsg_G2S_CoupleNameChanged::GameMsg_G2S_CoupleNameChanged()
:GameMsg_Base(MSG_G2S_CoupleNameChanged)
,m_strCoupleName("")
{

}

GameMsg_G2S_CoupleNameChanged::~GameMsg_G2S_CoupleNameChanged()
{

}

bool GameMsg_G2S_CoupleNameChanged::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddStringW(m_strCoupleName.c_str());

	return true;
}

bool GameMsg_G2S_CoupleNameChanged::doDecode( CParamPool &IOBuff )
{
	IOBuff.GetStringW(m_strCoupleName);

	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_BlessingNotify::GameMsg_G2S_BlessingNotify()
:GameMsg_Base( MSG_G2S_BlessingNotify )
,m_strManName("")
,m_strWomanName("")
,m_nPointIndex(0)
{

}

bool GameMsg_G2S_BlessingNotify::doDecode( CParamPool& IOBuff )
{
	IOBuff.GetStringW( m_strManName );
	IOBuff.GetStringW( m_strWomanName );
	m_nPointIndex = IOBuff.GetUInt();

	return true;
}

bool GameMsg_G2S_BlessingNotify::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddStringW( m_strManName.c_str() );
	IOBuff.AddStringW( m_strWomanName.c_str() );
	IOBuff.AddUInt( m_nPointIndex );

	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_BlessingCouple::GameMsg_G2S_BlessingCouple()
:GameMsg_Base( MSG_G2S_BlessingCouple )
,m_nRoleID( 0 )
,m_nPairID( 0 )
,m_nSex( 0 )
{

}

bool GameMsg_G2S_BlessingCouple::doDecode( CParamPool& IOBuff )
{
	m_nRoleID = IOBuff.GetUInt();
	m_nPairID = IOBuff.GetUInt();
	m_nSex    = IOBuff.GetUInt();


	return true;
}

bool GameMsg_G2S_BlessingCouple::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nRoleID );
	IOBuff.AddUInt( m_nPairID );
	IOBuff.AddInt( m_nSex );

	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_GetWeddingRecordList::GameMsg_G2S_GetWeddingRecordList()
:GameMsg_Base( MSG_G2S_GetWeddingRecordList )
,m_nCount( 0 )
{

}

bool GameMsg_G2S_GetWeddingRecordList::doDecode( CParamPool& IOBuff )
{
    m_nCount = IOBuff.GetUInt();
    for (unsigned int i = 0; i < m_nCount; ++i)
    {
        WeddingRecordInfo record;
        record.m_nPairID = IOBuff.GetUInt();
        record.m_nIsRunning = IOBuff.GetUInt();
        record.m_nMaleID = IOBuff.GetUInt();
        IOBuff.GetStringW(record.m_strMaleName);
        record.m_nFemaleID = IOBuff.GetUInt();
        IOBuff.GetStringW(record.m_strFemaleName);

        int nRoleIDCount = (int)IOBuff.GetUShort();
        for (int j = 0; j < nRoleIDCount; ++j)
        {
            record.m_BlessingRoleIDList.push_back(IOBuff.GetUInt());
        }

        m_RecordList.push_back(record);
    }

    return true;
}

bool GameMsg_G2S_GetWeddingRecordList::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_RecordList.size() );
	for ( size_t i = 0; i < m_RecordList.size(); ++i )
	{
		IOBuff.AddUInt( m_RecordList[i].m_nPairID );
        IOBuff.AddUInt( m_RecordList[i].m_nIsRunning );
		IOBuff.AddUInt( m_RecordList[i].m_nMaleID );
		IOBuff.AddStringW( m_RecordList[i].m_strMaleName.c_str() );
		IOBuff.AddUInt( m_RecordList[i].m_nFemaleID );
		IOBuff.AddStringW( m_RecordList[i].m_strFemaleName.c_str() );
        IOBuff.AddUShort((unsigned short)m_RecordList[i].m_BlessingRoleIDList.size());
		for ( size_t j = 0; j < m_RecordList[i].m_BlessingRoleIDList.size(); ++j )
		{
			IOBuff.AddUInt( m_RecordList[i].m_BlessingRoleIDList[j] );
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_GetBlessingRankList::GameMsg_G2S_GetBlessingRankList()
:GameMsg_Base( MSG_G2S_GetBlessingRankList )
,m_nCurrentPage( 0 )
,m_nRankTotalCount( 0 )
,m_nPageCount(0)
{

}

bool GameMsg_G2S_GetBlessingRankList::doDecode( CParamPool& IOBuff )
{
	m_nCurrentPage = IOBuff.GetUInt();
    m_nPageCount = IOBuff.GetUShort();
	m_nRankTotalCount = IOBuff.GetUInt();

	unsigned short nRankCount = IOBuff.GetUShort();
	for ( unsigned short i = 0; i < nRankCount; ++i )
	{
		CBlessingValueRankInfo rankinfo;
		rankinfo.doDecode(IOBuff);
		
		m_BlessingValueRankVec.push_back( rankinfo );
	}

	return true;
}

bool GameMsg_G2S_GetBlessingRankList::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nCurrentPage );
    IOBuff.AddUShort(m_nPageCount);
	IOBuff.AddUInt( m_nRankTotalCount );

    IOBuff.AddUShort((unsigned short)m_BlessingValueRankVec.size());
	for ( size_t  i = 0; i < m_BlessingValueRankVec.size(); ++i )
	{
		m_BlessingValueRankVec[i].doEncode( IOBuff );
	}

	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_AddRedEnvelopeBlessingValue::GameMsg_G2S_AddRedEnvelopeBlessingValue()
:GameMsg_Base( MSG_G2S_AddRedEnvelopeBlessingValue )
,m_nPairID( 0 )
,m_nRedEnvelopeCount( 0 )
,m_nRoleID( 0 )
,m_strRoleName( "" )
{

}

bool GameMsg_G2S_AddRedEnvelopeBlessingValue::doDecode( CParamPool& IOBuff )
{
	m_nPairID = IOBuff.GetUInt();
	m_nRedEnvelopeCount  = IOBuff.GetUInt();
	m_nRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strRoleName );
	return true;
}

bool GameMsg_G2S_AddRedEnvelopeBlessingValue::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nPairID );
	IOBuff.AddUInt( m_nRedEnvelopeCount );
	IOBuff.AddUInt( m_nRoleID );
	IOBuff.AddStringW( m_strRoleName.c_str() );

	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_RedEnvelopeDelMoney::GameMsg_G2S_RedEnvelopeDelMoney()
:GameMsg_Base( MSG_G2S_RedEnvelopeDelMoney )
,m_nRedEnvelopeCount( 0 )
,m_nPairID(0)
{

}

bool GameMsg_G2S_RedEnvelopeDelMoney::doDecode( CParamPool& IOBuff )
{
	m_nPairID = IOBuff.GetUInt();
	m_nRedEnvelopeCount  = IOBuff.GetUInt();
	return true;
}

bool GameMsg_G2S_RedEnvelopeDelMoney::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nPairID );
	IOBuff.AddUInt( m_nRedEnvelopeCount );

	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_GetCoupleInfo::GameMsg_G2S_GetCoupleInfo()
:GameMsg_Base( MSG_G2S_GetCoupleInfo )
,m_nRank(0)
{

}

bool GameMsg_G2S_GetCoupleInfo::doDecode( CParamPool& IOBuff )
{
	m_nRank = IOBuff.GetUInt();
	m_CoupleInfo.doDecode( IOBuff );
	return true;
}

bool GameMsg_G2S_GetCoupleInfo::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nRank );
	m_CoupleInfo.doEncode( IOBuff );
	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_GetBlessingValue::GameMsg_G2S_GetBlessingValue()
    :GameMsg_Base( MSG_G2S_GetBlessingValue )
    ,m_nBlessingValue( 0 )
    ,m_nLine(0)
{

}

bool GameMsg_G2S_GetBlessingValue::doDecode( CParamPool& IOBuff )
{
	m_nBlessingValue  = IOBuff.GetUInt();
    m_nLine = IOBuff.GetUShort();

	return true;
}

bool GameMsg_G2S_GetBlessingValue::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nBlessingValue );
    IOBuff.AddUShort(m_nLine);

	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_GetPersonInfo::GameMsg_G2S_GetPersonInfo()
:GameMsg_Base( MSG_G2S_GetPersonInfo )
{

}

bool GameMsg_G2S_GetPersonInfo::doDecode( CParamPool& IOBuff )
{
    m_OffLineData.doDecode( IOBuff );

    return true;
}

bool GameMsg_G2S_GetPersonInfo::doEncode( CParamPool& IOBuff )
{
    m_OffLineData.doEncode( IOBuff );

    return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_GetDivorceeAnimationPersonInfo::GameMsg_G2S_GetDivorceeAnimationPersonInfo()
	:GameMsg_Base( MSG_G2S_GetDivorceAnimationPersonInfo )
{

}

bool GameMsg_G2S_GetDivorceeAnimationPersonInfo::doDecode( CParamPool& IOBuff )
{
	IOBuff.GetStringW( m_OffLineData.m_strRoleName );
	m_OffLineData.m_nSex   = IOBuff.GetUChar();
	m_OffLineData.m_nSkin  = IOBuff.GetUChar();
	unsigned short nCount = IOBuff.GetUShort();
	for ( int i = 0; i < nCount; ++i )
	{
		DBItem Info;
		Info.m_nItemID = IOBuff.GetInt64();
		Info.m_nColumn = IOBuff.GetUShort();
		Info.m_nTypeID = IOBuff.GetUShort();
		Info.m_nIndex  = IOBuff.GetUShort();
		Info.m_nUseTimes = IOBuff.GetUShort();
		Info.m_nCount    = IOBuff.GetUShort();
		Info.m_nDuration = IOBuff.GetUInt();
		Info.m_bEquiped = IOBuff.GetBool();

		m_OffLineData.m_vecRoleItem.push_back( Info );
	}
	return true;
}

bool GameMsg_G2S_GetDivorceeAnimationPersonInfo::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddStringW( m_OffLineData.m_strRoleName.c_str() );
	IOBuff.AddUChar( m_OffLineData.m_nSex );
	IOBuff.AddUChar( m_OffLineData.m_nSkin );
    IOBuff.AddUShort((unsigned short)m_OffLineData.m_vecRoleItem.size());
	for ( size_t i = 0; i < m_OffLineData.m_vecRoleItem.size(); ++i )
	{
		IOBuff.AddInt64(  m_OffLineData.m_vecRoleItem[i].m_nItemID );
		IOBuff.AddUShort( m_OffLineData.m_vecRoleItem[i].m_nColumn );
		IOBuff.AddUInt( m_OffLineData.m_vecRoleItem[i].m_nTypeID );
		IOBuff.AddUShort( m_OffLineData.m_vecRoleItem[i].m_nIndex );
		IOBuff.AddUShort( m_OffLineData.m_vecRoleItem[i].m_nUseTimes );
		IOBuff.AddUShort( m_OffLineData.m_vecRoleItem[i].m_nCount );
		IOBuff.AddUInt(   m_OffLineData.m_vecRoleItem[i].m_nDuration );
		IOBuff.AddBool(  m_OffLineData.m_vecRoleItem[i].m_bEquiped );
	}
	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_BlessingError::GameMsg_G2S_BlessingError()
:GameMsg_Base( MSG_G2S_BlessingError )
{

}

bool GameMsg_G2S_BlessingError::doDecode( CParamPool& IOBuff )
{   
	IOBuff.GetStringW( m_strError );
	return true;
}

bool GameMsg_G2S_BlessingError::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddStringW( m_strError.c_str() );
	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_BlessingCoupleScuess::GameMsg_G2S_BlessingCoupleScuess()
:GameMsg_Base( MSG_G2S_BlessingPairSuc )
{

}

bool GameMsg_G2S_BlessingCoupleScuess::doDecode( CParamPool& IOBuff )
{   
	return true;
}

bool GameMsg_G2S_BlessingCoupleScuess::doEncode( CParamPool& IOBuff )
{
	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_RedEnvelopeScuess::GameMsg_G2S_RedEnvelopeScuess()
:GameMsg_Base( MSG_G2S_MoneyBlessingPairSuc )
{

}

bool GameMsg_G2S_RedEnvelopeScuess::doDecode( CParamPool& IOBuff )
{   
	return true;
}

bool GameMsg_G2S_RedEnvelopeScuess::doEncode( CParamPool& IOBuff )
{
	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_WeddingNotice::GameMsg_G2S_WeddingNotice()
	:GameMsg_Base( MSG_G2S_WeddingNotice )
{

}

bool GameMsg_G2S_WeddingNotice::doDecode( CParamPool& IOBuff )
{   
	IOBuff.GetStringW( m_strMaleRoleName );      
	IOBuff.GetStringW( m_strFemaleRoleName );
	return true;
}

bool GameMsg_G2S_WeddingNotice::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddStringW( m_strMaleRoleName.c_str() );
	IOBuff.AddStringW( m_strFemaleRoleName.c_str() );
	return true;
}

/////////////////////////////////////////////////////////////////
GameMsg_G2S_RefreshDivorceAction::GameMsg_G2S_RefreshDivorceAction()
	:GameMsg_Base( MSG_G2S_RefreshDivorceAction )
{

}

bool GameMsg_G2S_RefreshDivorceAction::doDecode( CParamPool& IOBuff )
{   
	m_DivorceAgreementInfo.doDecode( IOBuff );
	m_Action.doDecode( IOBuff );
	return true;
}

bool GameMsg_G2S_RefreshDivorceAction::doEncode( CParamPool& IOBuff )
{
	m_DivorceAgreementInfo.doEncode( IOBuff );
	m_Action.doEncode( IOBuff );
	return true;
}
