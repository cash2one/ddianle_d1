#include "GameMsg_CeremonyRoom.h"
#include "../dancegroup/DanceGroupMsgDef.h"
#include "../../socket/ParamPool.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "../../socket/inc/SocketLib.h"



CeremonyPraiseSendInfo::CeremonyPraiseSendInfo()
    : m_nRoleID(0)
    , m_bIsVIP(0)
    , m_nVIPLevel(0)
    , m_nPraiseCount(0)
{

}

void CeremonyPraiseSendInfo::DoEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddInt(m_nPraiseCount);

    return ;
}

void CeremonyPraiseSendInfo::DoDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRoleName);
    m_bIsVIP = IOBuff.GetBool();
    m_nVIPLevel = IOBuff.GetUShort();
    m_nPraiseCount = IOBuff.GetInt();

    return ;
}


CeremonyPraiseGainedInfo::CeremonyPraiseGainedInfo()
    : m_nRoleID(0)
    , m_bIsVIP(0)
    , m_nVIPLevel(0)
    , m_nPraiseCount(0)
{

}

void CeremonyPraiseGainedInfo::DoEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddInt(m_nPraiseCount);

    return ;
}

void CeremonyPraiseGainedInfo::DoDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRoleName);
    m_bIsVIP = IOBuff.GetBool();
    m_nVIPLevel = IOBuff.GetUShort();
    m_nPraiseCount = IOBuff.GetInt();

    return ;
}



CeremonyRecordInfo::CeremonyRecordInfo()
    : m_nApplicantID(0)
    , m_bIsVIP(false)
    , m_nVIPLevel(0)
    , m_nCeremonyDate(0)
{

}
bool CeremonyRecordInfo::DoEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nLogID);
    IOBuff.AddUInt(m_nApplicantID);
    IOBuff.AddStringW(m_strApplicantName);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddUInt(m_nCeremonyDate);

    IOBuff.AddUShort((unsigned short)m_vecPraiseInfo.size());
    for (size_t i = 0; i < m_vecPraiseInfo.size(); ++i)
    {
        m_vecPraiseInfo[i].DoEncode(IOBuff);
    }

    return true;
}

bool CeremonyRecordInfo::DoDecode(CParamPool &IOBuff)
{
    m_nLogID = IOBuff.GetUInt();
    m_nApplicantID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strApplicantName);
    m_bIsVIP = IOBuff.GetBool();
    m_nVIPLevel = IOBuff.GetUShort();
    m_nCeremonyDate = IOBuff.GetUInt();

    unsigned short len = IOBuff.GetUShort();
    for (unsigned short i = 0; i < len; ++i)
    {
        CeremonyPraiseGainedInfo g;
        g.DoDecode(IOBuff);
        m_vecPraiseInfo.push_back(g);
    }

    return true;
}


GameMsg_S2G_CeremonyApplyCreate::GameMsg_S2G_CeremonyApplyCreate()
	: GameMsg_Base(MSG_S2G_CeremonyApplyCreate)
	, m_nDanceGroupID(0)
	, m_nRoleID(0)
    , m_nLine(0)
{

}
bool GameMsg_S2G_CeremonyApplyCreate::doDecode(CParamPool &IOBuff)
{
	m_nDanceGroupID = IOBuff.GetUInt();
	m_nRoleID = IOBuff.GetUInt();
    m_nLine = IOBuff.GetInt();

    return true;
}

bool GameMsg_S2G_CeremonyApplyCreate::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nDanceGroupID);
	IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddInt(m_nLine);

    return true;
}


GameMsg_G2S_CeremonyApplyCreateRet::GameMsg_G2S_CeremonyApplyCreateRet()
	: GameMsg_Base(MSG_G2S_CeremonyApplyCreateRet)
	, m_nDanceGroupID(0)
	, m_nRoleID(0)
	, m_nErrorType(0)
{

}

bool GameMsg_G2S_CeremonyApplyCreateRet::doDecode(CParamPool &IOBuff)
{
	m_nDanceGroupID = IOBuff.GetUInt();
	m_nRoleID = IOBuff.GetUInt();
	m_nErrorType = IOBuff.GetUChar();

	return true;
}

bool GameMsg_G2S_CeremonyApplyCreateRet::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nDanceGroupID);
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUChar(m_nErrorType);

	return true;
}

GameMsg_S2G_CeremonyCreateRoomNotify::GameMsg_S2G_CeremonyCreateRoomNotify()
	: GameMsg_Base(MSG_S2G_CeremonyCreateRoomNotify)
	, m_nDanceGroupID(0)
	, m_nCreateTime(0)
    , m_nOldCeremonyTimes(0)
    , m_nLine(0)
{

}


bool GameMsg_S2G_CeremonyCreateRoomNotify::doDecode(CParamPool &IOBuff)
{
	m_nDanceGroupID = IOBuff.GetUInt();
	m_nCreateTime = IOBuff.GetUInt();
    m_nOldCeremonyTimes = IOBuff.GetUInt();
    m_nLine = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strLineName);

	return true;
}

bool GameMsg_S2G_CeremonyCreateRoomNotify::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nDanceGroupID);
	IOBuff.AddUInt(m_nCreateTime);
    IOBuff.AddUInt(m_nOldCeremonyTimes);
    IOBuff.AddUInt(m_nLine);
    IOBuff.AddStringW(m_strLineName);
	return true;
}


GameMsg_S2G_CeremonyApplyCreateCancel::GameMsg_S2G_CeremonyApplyCreateCancel()
	: GameMsg_Base(MSG_S2G_CeremonyApplyCreateCancel)
	, m_nDanceGroupID(0)
	, m_nCreateID(0)
{

}

bool GameMsg_S2G_CeremonyApplyCreateCancel::doDecode(CParamPool &IOBuff)
{
	m_nCreateID = IOBuff.GetUInt();
	m_nDanceGroupID = IOBuff.GetUInt();

	return true;
}

bool GameMsg_S2G_CeremonyApplyCreateCancel::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nCreateID);
	IOBuff.AddUInt(m_nDanceGroupID);

	return true;
}


GameMsg_S2G_CeremonyCloseRoomNotify::GameMsg_S2G_CeremonyCloseRoomNotify()
	: GameMsg_Base(MSG_S2G_CeremonyCloseRoomNotify)
	, m_nDanceGroupID(0)
	, m_nCloseReason(0)
{

}


bool GameMsg_S2G_CeremonyCloseRoomNotify::doDecode(CParamPool &IOBuff)
{
	m_nDanceGroupID = IOBuff.GetUInt();
	m_nCloseReason = IOBuff.GetUChar();

	return true;
}

bool GameMsg_S2G_CeremonyCloseRoomNotify::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nDanceGroupID);
	IOBuff.AddUChar(m_nCloseReason);

	return true;
}


MsgStateChange::MsgStateChange()
    : m_nDanceGroupID(0)
    , m_nNewState(0)
{

}
MsgStateChange::~MsgStateChange()
{}

void MsgStateChange::doDecode(CParamPool &IOBuff)
{
    m_nDanceGroupID = IOBuff.GetUInt();
    m_nNewState = IOBuff.GetUChar();

    return ;
}

void MsgStateChange::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nDanceGroupID);
    IOBuff.AddUChar(m_nNewState);

    return ;
}


GameMsg_S2G_CeremonyRoomStateChange::GameMsg_S2G_CeremonyRoomStateChange()
	: GameMsg_Base(MSG_S2G_CeremonyRoomStateChange)
{

}

bool GameMsg_S2G_CeremonyRoomStateChange::doDecode(CParamPool &IOBuff)
{
    m_msg.doDecode(IOBuff);
	return true;
}

bool GameMsg_S2G_CeremonyRoomStateChange::doEncode(CParamPool &IOBuff)
{
    m_msg.doEncode(IOBuff);
	return true;
}



GameMsg_G2S_CeremonyRoomStateChange::GameMsg_G2S_CeremonyRoomStateChange()
    : GameMsg_Base(MSG_G2S_CeremonyRoomStateChange)
{

}

bool GameMsg_G2S_CeremonyRoomStateChange::doDecode(CParamPool &IOBuff)
{
    m_msg.doDecode(IOBuff);
    return true;
}

bool GameMsg_G2S_CeremonyRoomStateChange::doEncode(CParamPool &IOBuff)
{
    m_msg.doEncode(IOBuff);
    return true;
}

GameMsg_S2G_CeremonyRoomNewMember::GameMsg_S2G_CeremonyRoomNewMember()
    : GameMsg_Base(MSG_S2G_CeremonyRoomNewMember)
    , m_nGroupID(0)
{

}

GameMsg_S2G_CeremonyRoomNewMember::~GameMsg_S2G_CeremonyRoomNewMember()
{

}

bool GameMsg_S2G_CeremonyRoomNewMember::doDecode(CParamPool &IOBuff)
{
    m_nGroupID = IOBuff.GetUInt();
    unsigned short len = IOBuff.GetUShort();
    for (unsigned short i = 0; i < len; ++i)
    {
        m_vecNewMember.push_back(IOBuff.GetUInt());
    }

    return true;
}

bool GameMsg_S2G_CeremonyRoomNewMember::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGroupID);
    IOBuff.AddUShort((unsigned short)m_vecNewMember.size());
    for (size_t i = 0; i < m_vecNewMember.size(); ++i)
    {
        IOBuff.AddUInt(m_vecNewMember[i]);
    }

    return true;
}


GameMsg_S2G_CeremonyRoomLogsStart::GameMsg_S2G_CeremonyRoomLogsStart()
    :GameMsg_Base(MSG_S2G_CeremonyRoomLogsStart)
    , m_nLine(0)                                        
    , m_nLogSeq(0)                                      
    , m_nGroupID(0)
{

}

GameMsg_S2G_CeremonyRoomLogsStart::~GameMsg_S2G_CeremonyRoomLogsStart()
{

}

bool GameMsg_S2G_CeremonyRoomLogsStart::doDecode(CParamPool &IOBuff)
{
    m_nLine = IOBuff.GetInt();
    m_nLogSeq = IOBuff.GetUInt();
    m_nGroupID = IOBuff.GetUInt();
    
    m_roomInfo.DoDecode(IOBuff);
    
    return true;
}

bool GameMsg_S2G_CeremonyRoomLogsStart::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nLine);
    IOBuff.AddUInt(m_nLogSeq);
    IOBuff.AddUInt(m_nGroupID);

    m_roomInfo.DoEncode(IOBuff);

    return true;
}


GameMsg_S2G_CeremonyRoomLogsItem::GameMsg_S2G_CeremonyRoomLogsItem()
    : GameMsg_Base(MSG_S2G_CeremonyRoomLogsItem)
    , m_nLine(0)
    , m_nLogSeq(0)
{

}
GameMsg_S2G_CeremonyRoomLogsItem::~GameMsg_S2G_CeremonyRoomLogsItem()
{

}

bool GameMsg_S2G_CeremonyRoomLogsItem::doDecode(CParamPool &IOBuff)
{
    m_nLine = IOBuff.GetInt();
    m_nLogSeq = IOBuff.GetUInt();

    m_logItem.DoDecode(IOBuff);

    return true;
}

bool GameMsg_S2G_CeremonyRoomLogsItem::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nLine);
    IOBuff.AddUInt(m_nLogSeq);
    m_logItem.DoEncode(IOBuff);

    return true;
}


GameMsg_S2G_CeremonyRoomLogsEnd::GameMsg_S2G_CeremonyRoomLogsEnd()
    : GameMsg_Base(MSG_S2G_CeremonyRoomLogsEnd)
    , m_nLine(0)
    , m_nLogSeq(0)
{

}

GameMsg_S2G_CeremonyRoomLogsEnd::~GameMsg_S2G_CeremonyRoomLogsEnd()
{

}

bool GameMsg_S2G_CeremonyRoomLogsEnd::doDecode(CParamPool &IOBuff)
{
    m_nLine = IOBuff.GetInt();
    m_nLogSeq = IOBuff.GetUInt();

    return true;
}

bool GameMsg_S2G_CeremonyRoomLogsEnd::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nLine);
    IOBuff.AddUInt(m_nLogSeq);

    return true;
}


GameMsg_C2S_RequestCeremonyRecord::GameMsg_C2S_RequestCeremonyRecord() 
    : GameMsg_Base(MSG_C2S_RequestCeremonyRecord)
    , m_nRecordIndex(0)
{
}

bool GameMsg_C2S_RequestCeremonyRecord::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nRecordIndex);

    return true;
}


bool GameMsg_C2S_RequestCeremonyRecord::doDecode(CParamPool &IOBuff)
{
    m_nRecordIndex = IOBuff.GetInt();

    return true ;
}

GameMsg_S2C_RequestCeremonyRecordRes::GameMsg_S2C_RequestCeremonyRecordRes() 
    : GameMsg_Base(MSG_S2C_RequestCeremonyRecordRes)
    , m_nCurIndex(0)
    , m_nTotleCount(0)
{
}

bool GameMsg_S2C_RequestCeremonyRecordRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nCurIndex);
    IOBuff.AddInt(m_nTotleCount);

    if (m_nTotleCount > 0 && m_nTotleCount > m_nCurIndex)
    {
        m_recordInfo.DoEncode(IOBuff);
    }

    return true;
}

bool GameMsg_S2C_RequestCeremonyRecordRes::doDecode(CParamPool &IOBuff)
{
    m_nCurIndex = IOBuff.GetInt();
    m_nTotleCount = IOBuff.GetInt();

    if (m_nTotleCount > 0 && m_nTotleCount > m_nCurIndex)
    {
        m_recordInfo.DoDecode(IOBuff);
    }

    return true;
}


GameMsg_C2S_RequestCeremonyPraise::GameMsg_C2S_RequestCeremonyPraise() 
    : GameMsg_Base(MSG_C2S_RequestCeremonyPraise)
    , m_nLogID(0)
    , m_nRoleID(0)
    , m_nPage(0)
    , m_nCount(0)
{
}

bool GameMsg_C2S_RequestCeremonyPraise::doDecode(CParamPool &IOBuff)
{
    m_nLogID = IOBuff.GetUInt();
    m_nRoleID = IOBuff.GetUInt();
    m_nPage = IOBuff.GetInt();
    m_nCount = IOBuff.GetInt();

    return true ;
}

bool GameMsg_C2S_RequestCeremonyPraise::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nLogID);
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddInt(m_nPage);
    IOBuff.AddInt(m_nCount);

    return true;
}


GameMsg_S2C_RequestCeremonyPraiseRes::GameMsg_S2C_RequestCeremonyPraiseRes() 
    : GameMsg_Base(MSG_S2C_RequestCeremonyPraiseRes)
    , m_logID(0)
    , m_nRoleID(0)
    , m_nCurPage(0)
    , m_nTotleCount(0)
{
}

bool GameMsg_S2C_RequestCeremonyPraiseRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_logID);
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddInt(m_nCurPage);
    IOBuff.AddInt(m_nTotleCount);

    IOBuff.AddUShort((unsigned short)m_vecPraiseInfo.size());
    for (size_t i = 0; i < m_vecPraiseInfo.size(); ++i)
    {
        m_vecPraiseInfo[i].DoEncode(IOBuff);
    }

    return true;
}

bool GameMsg_S2C_RequestCeremonyPraiseRes::doDecode(CParamPool &IOBuff)
{
    m_logID = IOBuff.GetUInt();
    m_nRoleID = IOBuff.GetUInt();
    m_nCurPage = IOBuff.GetInt();
    m_nTotleCount = IOBuff.GetInt();

    unsigned short cnt = IOBuff.GetUShort();
    for (size_t i = 0; i < cnt; ++i)
    {
        CeremonyPraiseSendInfo sinfo;
        sinfo.DoDecode(IOBuff);
        m_vecPraiseInfo.push_back(sinfo);
    }

    return true;
}


GameMsg_S2C_SyncCeremonyState::GameMsg_S2C_SyncCeremonyState() 
    : GameMsg_Base(MSG_S2C_SyncCeremonyState)
    , m_nCeremonyState(0)
    , m_nLineID(0)
{
}

bool GameMsg_S2C_SyncCeremonyState::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nCeremonyState);
    IOBuff.AddInt(m_nLineID);
    IOBuff.AddStringW(m_strLineName);

    return true;
}

bool GameMsg_S2C_SyncCeremonyState::doDecode(CParamPool &IOBuff)
{
    m_nCeremonyState = IOBuff.GetUChar();
    m_nLineID = IOBuff.GetInt();
    IOBuff.GetStringW(m_strLineName);

    return true;
}


GameMsg_S2C_SyncCeremonyConfig::GameMsg_S2C_SyncCeremonyConfig()
    : GameMsg_Base(MSG_S2C_SyncCeremonyConfig)
    , m_nFreeApplyTimes(0)
    , m_nApplyMoneyType(0)
    , m_nApplyMoneyCount(0)
    , m_nFreePraiseTimes(0)
    , m_nPraiseMoneyType(0)
    , m_nPraiseMoneyCount(0)
    , m_fStartDuration(0.0f)
{

}
GameMsg_S2C_SyncCeremonyConfig::~GameMsg_S2C_SyncCeremonyConfig()
{

}

bool GameMsg_S2C_SyncCeremonyConfig::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddChar(m_nFreeApplyTimes);
    IOBuff.AddUChar(m_nApplyMoneyType);
    IOBuff.AddUInt(m_nApplyMoneyCount);

    IOBuff.AddUChar(m_nFreePraiseTimes);
    IOBuff.AddUChar(m_nPraiseMoneyType);
    IOBuff.AddUInt(m_nPraiseMoneyCount);

    IOBuff.AddFloat(m_fStartDuration);
    return true;
}

GameMsg_S2C_SyncCermonyApplyTimes::GameMsg_S2C_SyncCermonyApplyTimes()
    : GameMsg_Base(MSG_S2C_SyncCermonyApplyTimes)
    , m_nUsedTimes(0)
{

}

GameMsg_S2C_SyncCermonyApplyTimes::~GameMsg_S2C_SyncCermonyApplyTimes() {}
bool GameMsg_S2C_SyncCermonyApplyTimes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nUsedTimes);

    return true;
}

GameMsg_S2G_CeremonyRoomStatics::GameMsg_S2G_CeremonyRoomStatics()
    : GameMsg_Base(MSG_S2G_CeremonyRoomStatics)
{

}

GameMsg_S2G_CeremonyRoomStatics::~GameMsg_S2G_CeremonyRoomStatics()
{
    
}

bool GameMsg_S2G_CeremonyRoomStatics::doEncode(CParamPool &IOBuff)
{
    return true;
}

bool GameMsg_S2G_CeremonyRoomStatics::doDecode(CParamPool &IOBuff)
{
    return true;
}


GameMsg_G2S_CeremonyRoomStatics::GameMsg_G2S_CeremonyRoomStatics()
    : GameMsg_Base(MSG_G2S_CeremonyRoomStatics)
{

}

GameMsg_G2S_CeremonyRoomStatics::~GameMsg_G2S_CeremonyRoomStatics()
{

}

bool GameMsg_G2S_CeremonyRoomStatics::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strStatics);

    return true;
}

bool GameMsg_G2S_CeremonyRoomStatics::doDecode(CParamPool &IOBuff)
{
    IOBuff.GetStringW(m_strStatics);

    return true;
}

CeremonyRoomSyncInfo::CeremonyRoomSyncInfo()
    : m_nRoomID(0)
    , m_state(0)
    , m_createTime(0)
{

}

bool CeremonyRoomSyncInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoomID);
    IOBuff.AddUChar(m_state);
    IOBuff.AddUInt(m_createTime);

    return true;
}
bool CeremonyRoomSyncInfo::doDecode(CParamPool &IOBuff)
{
    m_nRoomID = IOBuff.GetUInt();
    m_state = IOBuff.GetUChar();
    m_createTime = IOBuff.GetUInt();

    return true;
}

GameMsg_S2G_CeremonySyncRoomInfo::GameMsg_S2G_CeremonySyncRoomInfo()
    : GameMsg_Base(Msg_S2G_CeremonySyncRoomInfo)
    , m_nLine(0)
{

}

GameMsg_S2G_CeremonySyncRoomInfo::~GameMsg_S2G_CeremonySyncRoomInfo()
{

}

bool GameMsg_S2G_CeremonySyncRoomInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nLine);
    IOBuff.AddStringW(m_strLineName);

    IOBuff.AddUShort((unsigned short)m_vecRooms.size());
    for (size_t i = 0; i < m_vecRooms.size(); ++i)
    {
        m_vecRooms[i].doEncode(IOBuff);
    }

    return true;
}

bool GameMsg_S2G_CeremonySyncRoomInfo::doDecode(CParamPool &IOBuff)
{
    m_nLine = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strLineName);

    unsigned short len = IOBuff.GetUShort();
    for (unsigned short i = 0; i < len; ++i)
    {
        CeremonyRoomSyncInfo crsi;
        crsi.doDecode(IOBuff);
        m_vecRooms.push_back(crsi);
    }
    
    return true;
}

