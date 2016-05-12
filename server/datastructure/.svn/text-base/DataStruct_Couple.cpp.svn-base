#include "DataStruct_Couple.h"
#include "../socket/ParamPool.h"

unsigned short g_nCoupleRequestLevel = 10;
int g_nMinIntimacyMarriageRequired = 100;


CCoupleInfoDB::CCoupleInfoDB()
:m_nPairID(0)
,m_nMaleID(0)
,m_nFemaleID(0)
,m_nIntimacy(0)
,m_nCoupleDate(0)
,m_nMarriageDate(0)
,m_nSmallRoleID(0)
{

}

CCoupleInfoDB::~CCoupleInfoDB()
{

}

CCoupleInfo::CCoupleInfo()
:m_nPairID(0)
,m_nCoupleID(0)
,m_strCoupleName("")
,m_nIntimacy(0)
,m_nCoupleDate(0)
,m_nMarriageDate(0)
,m_strLoveDeclaration("")
,m_nBlessingValue(0)
,m_nMarriageGiftDate( 0 )
,m_nSmallRoleID(0)
{

}

CCoupleInfo::CCoupleInfo( const CCoupleInfo & coupleinfo )
{
	m_nPairID = coupleinfo.m_nPairID;
	m_nCoupleID = coupleinfo.m_nCoupleID;
	m_strCoupleName = coupleinfo.m_strCoupleName;
	m_nIntimacy = coupleinfo.m_nIntimacy;
	m_nCoupleDate = coupleinfo.m_nCoupleDate;
	m_nMarriageDate = coupleinfo.m_nMarriageDate;
	m_strLoveDeclaration = coupleinfo.m_strLoveDeclaration;
	m_nBlessingValue = coupleinfo.m_nBlessingValue;
	m_nMarriageGiftDate = coupleinfo.m_nMarriageDate;
    m_nSmallRoleID = coupleinfo.m_nSmallRoleID;
}

CCoupleInfo::~CCoupleInfo()
{

}

void CCoupleInfo::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUInt(m_nPairID);	
    IOBuff.AddUInt(m_nCoupleID);
    IOBuff.AddStringW(m_strCoupleName.c_str());
    IOBuff.AddInt(m_nIntimacy);
    IOBuff.AddUInt(m_nCoupleDate);
    IOBuff.AddUInt(m_nMarriageDate);
    IOBuff.AddStringW(m_strLoveDeclaration.c_str());
    IOBuff.AddUInt(m_nSmallRoleID);
}

void CCoupleInfo::doDecode( CParamPool & IOBuff )
{
    m_nPairID = IOBuff.GetUInt();
    m_nCoupleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strCoupleName);
    m_nIntimacy = IOBuff.GetInt();
    m_nCoupleDate = IOBuff.GetUInt();
    m_nMarriageDate = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strLoveDeclaration);
    m_nSmallRoleID = IOBuff.GetUInt();
}

void CCoupleInfo::Serialize(CParamPool & IOBuff)
{
    IOBuff.AddUInt(m_nPairID);	
    IOBuff.AddUInt(m_nCoupleID);
    IOBuff.AddStringW(m_strCoupleName.c_str());
    IOBuff.AddInt(m_nIntimacy);
    IOBuff.AddUInt(m_nCoupleDate);
    IOBuff.AddUInt(m_nMarriageDate);
    IOBuff.AddStringW(m_strLoveDeclaration.c_str());
    IOBuff.AddUInt(m_nSmallRoleID);
}

CBlessingValueRankInfo::CBlessingValueRankInfo()
:m_nPairID(0)
,m_nMaleRoleID(0)
,m_strMaleRoleName("")
,m_nFemaleRoleID(0)
,m_strFemaleRoleName("")
{

};

CBlessingValueRankInfo::CBlessingValueRankInfo( const CBlessingValueRankInfo & rankinfo )
{
	m_nPairID = rankinfo.m_nPairID;
	m_nMaleRoleID = rankinfo.m_nMaleRoleID;
	m_strMaleRoleName = rankinfo.m_strMaleRoleName;
	m_nFemaleRoleID = rankinfo.m_nFemaleRoleID;
	m_strFemaleRoleName = rankinfo.m_strFemaleRoleName;
	m_nBlessingValue = rankinfo.m_nBlessingValue;
}

CBlessingValueRankInfo::~CBlessingValueRankInfo()
{

}

void CBlessingValueRankInfo::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt( m_nPairID );
	IOBuff.AddUInt( m_nMaleRoleID );
	IOBuff.AddStringW( m_strMaleRoleName.c_str() );
	IOBuff.AddUInt( m_nFemaleRoleID );
	IOBuff.AddStringW( m_strFemaleRoleName.c_str() );
	IOBuff.AddUInt( m_nBlessingValue );
}

void CBlessingValueRankInfo::doDecode( CParamPool &IOBuff )
{
	m_nPairID = IOBuff.GetUInt();
	m_nMaleRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strMaleRoleName );
	m_nFemaleRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strFemaleRoleName );
	m_nBlessingValue = IOBuff.GetUInt();
}

COfflineCoupleInfo::COfflineCoupleInfo()
:m_nPairID(0)
,m_nMaleRoleID(0)
,m_nFemaleRoleID(0)
,m_nIntimacy(0)
,m_nCoupleDate(0)
,m_nMarriageDate(0)
,m_nBlessingValue(0)
,m_nSmallRoleID(0)
{ 
}

void COfflineCoupleInfo::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt( m_nPairID );
	IOBuff.AddUInt( m_nMaleRoleID );
	IOBuff.AddStringW( m_strMaleRoleName.c_str() );
	IOBuff.AddUInt( m_nFemaleRoleID );
	IOBuff.AddStringW( m_strFemaleRoleName.c_str() );
	IOBuff.AddUInt( m_nIntimacy );
	IOBuff.AddUInt( m_nCoupleDate );
	IOBuff.AddUInt( m_nMarriageDate );
	IOBuff.AddStringW( m_strLoveDeclaration.c_str() );
	IOBuff.AddUInt( m_nBlessingValue );
    IOBuff.AddUInt( m_nSmallRoleID );
}

void COfflineCoupleInfo::doDecode( CParamPool &IOBuff )
{
	m_nPairID = IOBuff.GetUInt();
	m_nMaleRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strMaleRoleName );
	m_nFemaleRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strFemaleRoleName );
	m_nIntimacy = IOBuff.GetUInt();
	m_nCoupleDate = IOBuff.GetUInt();
	m_nMarriageDate = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strLoveDeclaration );
	m_nBlessingValue = IOBuff.GetUInt();
    m_nSmallRoleID = IOBuff.GetUInt();
}


CDivorceAgreementInfo::CDivorceAgreementInfo()
	:m_nAgreementID(0)
	,m_nMaleRoleID(0)
	,m_strMaleRoleName("")
	,m_bMaleIsAgree(false)
	,m_nFemaleRoleID(0)
	,m_strFemaleRoleName("")
	,m_bFemaleIsAgree(false)
	,m_nProposerID(0)
	,m_eState( eDivorceState_None )
    ,m_nSmallRoleID(0)
{
}

CDivorceAgreementInfo::CDivorceAgreementInfo( const CDivorceAgreementInfo & info )
{
	m_nAgreementID = info.m_nAgreementID;
	m_nMaleRoleID = info.m_nMaleRoleID;
	m_bMaleIsAgree = info.m_bMaleIsAgree;
	m_strMaleRoleName = info.m_strMaleRoleName;
	m_nFemaleRoleID = info.m_nFemaleRoleID;
	m_bFemaleIsAgree = info.m_bFemaleIsAgree;
	m_strFemaleRoleName = info.m_strFemaleRoleName;
	m_nProposerID = info.m_nProposerID;
	m_eState = info.m_eState;
    m_nSmallRoleID = info.m_nSmallRoleID;
}

void CDivorceAgreementInfo::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt( m_nAgreementID );
	IOBuff.AddUInt( m_nMaleRoleID );
	IOBuff.AddBool( m_bMaleIsAgree );
	IOBuff.AddStringW( m_strMaleRoleName );
	IOBuff.AddUInt( m_nFemaleRoleID );
	IOBuff.AddBool( m_bFemaleIsAgree );
	IOBuff.AddStringW( m_strFemaleRoleName );
	IOBuff.AddUInt( m_nProposerID );
	IOBuff.AddChar( (char)m_eState );
}

void CDivorceAgreementInfo::doDecode( CParamPool &IOBuff )
{
	m_nAgreementID = IOBuff.GetUInt();
	m_nMaleRoleID = IOBuff.GetUInt();
	m_bMaleIsAgree = IOBuff.GetBool();
	IOBuff.GetStringW( m_strMaleRoleName );
	m_nFemaleRoleID = IOBuff.GetUInt();
	m_bFemaleIsAgree = IOBuff.GetBool();
	IOBuff.GetStringW( m_strFemaleRoleName );
	m_nProposerID = IOBuff.GetUInt();
	m_eState = (eDivorceState)IOBuff.GetUChar();
}

void CDivorceAgreementInfo::clear()
{
	m_nAgreementID = 0;
	m_nMaleRoleID = 0;
	m_strMaleRoleName = "";
	m_bMaleIsAgree = false;
	m_nFemaleRoleID = 0;
	m_strFemaleRoleName = "";
	m_bFemaleIsAgree = false;
	m_nProposerID = 0;
	m_eState = eDivorceState_None ;
    m_nSmallRoleID = 0;
}

CDivorceAction::CDivorceAction()
	:m_nAgreementID(0)
	,m_eType(eDivorceAction_None )
	,m_nProposerID(0)
	,m_strChatContent("")
	,m_nTime(0)
{
}

CDivorceAction::CDivorceAction( const CDivorceAction & action )
{
	m_nAgreementID = action.m_nAgreementID;
	m_nProposerID = action.m_nProposerID;
	m_eType = action.m_eType;
	m_strChatContent = action.m_strChatContent;
	m_SendItem = action.m_SendItem;
	m_nTime = action.m_nTime;
}

void CDivorceAction::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt( m_nAgreementID );
	IOBuff.AddUInt( m_nProposerID );
	IOBuff.AddChar( (char)m_eType );
	IOBuff.AddStringW( m_strChatContent );
	m_SendItem.doEncode( IOBuff );
	IOBuff.AddUInt( m_nTime );
}

void CDivorceAction::doDecode( CParamPool &IOBuff )
{
	m_nAgreementID = IOBuff.GetUInt();
	m_nProposerID = IOBuff.GetUInt();
	m_eType = (eDivorceAction)IOBuff.GetUChar();
	IOBuff.GetStringW( m_strChatContent );
	m_SendItem.doDecode( IOBuff );
	m_nTime = IOBuff.GetUInt();
}
