#include <string.h>
#include "GMDataStruct.h"


GMAccountInfo::GMAccountInfo()
:m_nAccount(0)
, m_nPriv(0)
, m_nSessionID(0)
, m_nSlot(0)
{
    memset(m_szName, 0, GM_MAX_NAME_LEN);
    memset(m_szPasswd, 0, GM_MAX_PASSWD_LEN);
}

GMAccountInfo::~GMAccountInfo()
{
}

unsigned int GMAccountInfo::GetAccount()
{
    return m_nAccount;
}

int GMAccountInfo::GetPriv()
{
    return m_nPriv;
}

__int64 GMAccountInfo::GetSession()
{
    return m_nSessionID;
}

unsigned short GMAccountInfo::GetSlot()
{
    return m_nSlot;
}

CGMRoleInfoDB::CGMRoleInfoDB()
:m_nAccount(0)
, m_nSessionID(0)
, m_nSlot(0)
, m_nLastLoginTime(0)
, m_nPriv(0)
{
    memset(m_szName, 0, sizeof(m_szName));
    memset(m_szPassword, 0, sizeof(m_szPassword));
}

CGMRoleInfoDB::~CGMRoleInfoDB()
{
}

CGMSysMailLog::CGMSysMailLog()
:m_nGMAccount(0)
, m_strPUID("")
, m_strRoleName("")
, m_strMailTitle("")
, m_strMailContent("")
, m_nItemType(0)
, m_nItemCount(0)
, m_nValidTime(0)
, m_nMoney(0)
, m_nSendTime(0)
{
}

CGMSysMailLog::~CGMSysMailLog()
{
}


CAnnounceInfo::CAnnounceInfo()
:m_nPID(0)
, m_strContent("")
, m_bIsAdminChat(false)
, m_bIsSysChat(false)
, m_nStartTime(0)
, m_nEndTime(0)
, m_nSendInterval(0)
, m_nID(0)
, m_nLastSendTime(0)
, m_nState(ANNOUNCE_STATE_NOT_PROCESS)
{

}

CAnnounceInfo::~CAnnounceInfo()
{

}

void CAnnounceInfo::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUShort(m_nPID);
    IOBuff.AddStringW(m_strContent.c_str());
    IOBuff.AddBool(m_bIsAdminChat);
    IOBuff.AddBool(m_bIsSysChat);
    IOBuff.AddUInt(m_nStartTime);
    IOBuff.AddUInt(m_nEndTime);
    IOBuff.AddInt(m_nSendInterval);
}

void CAnnounceInfo::doDecode(CParamPool & IOBuff)
{
    m_nPID = IOBuff.GetUShort();
    IOBuff.GetStringW(m_strContent);
    m_bIsAdminChat = IOBuff.GetBool();
    m_bIsSysChat = IOBuff.GetBool();
    m_nStartTime = IOBuff.GetUInt();
    m_nEndTime = IOBuff.GetUInt();
    m_nSendInterval = IOBuff.GetInt();
}


QueryLogResult::QueryLogResult()
: m_nLogID(0)
, m_strGmaAccountName("")
, m_nOperTime(0)
, m_strOperParamJson("")
{
}

void QueryLogResult::Encode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nLogID);
    IOBuff.AddStringW(m_strGmaAccountName);
    IOBuff.AddUInt(m_nOperTime);
    IOBuff.AddStringW(m_strOperParamJson);

    return;
}


PlayerLockRequestDb::PlayerLockRequestDb()
: m_nPid(0)
, m_strPuid("")
, m_bIsLocked(true)
, m_nLockType(0)
, m_nLockReason(0)
, m_strOperatorName("")
{


}

PlayerLockRequestDb::~PlayerLockRequestDb()
{

}



PlayerLockedItem::PlayerLockedItem()
: m_nRoleID(0)
, m_nRoleLevel(0)
, m_nLockType(0)
, m_nLockReason(0)
, m_nLockTime(0)
{

}
PlayerLockedItem::~PlayerLockedItem()
{

}

bool PlayerLockedItem::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nRoleLevel = IOBuff.GetUShort();
    IOBuff.GetStringW(m_strRoleName);
    m_nLockType = IOBuff.GetUShort();
    m_nLockReason = IOBuff.GetUShort();
    IOBuff.GetStringW(m_strOperatorName);
    m_nLockTime = IOBuff.GetUInt();

    return true;
}

bool PlayerLockedItem::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUShort(m_nRoleLevel);
    IOBuff.AddStringW(m_strRoleName);
    IOBuff.AddUShort(m_nLockType);
    IOBuff.AddUShort(m_nLockReason);
    IOBuff.AddStringW(m_strOperatorName);
    IOBuff.AddUInt(m_nLockTime);

    return true;
}

PlayerLockQueryDb::PlayerLockQueryDb()
: m_nSlot(0)
, m_nStartTime(0)
, m_nEndTime(0)
{

}

PlayerLockQueryDb::~PlayerLockQueryDb()
{
    m_lstLockItem.clear();
}

RoleHarassInfo::RoleHarassInfo()
: m_nRoleID(0)
, m_nRoleLev(0)
, m_nHarassNum(0)
, m_nLockedTimes(0)
, m_bInited(false)
{

}

RoleHarassInfo::~RoleHarassInfo()
{

}

bool RoleHarassInfo::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nRoleLev = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRoleName);
    m_nHarassNum = IOBuff.GetUInt();
    m_nLockedTimes = IOBuff.GetUInt();

    return true;
}

bool RoleHarassInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddUInt(m_nRoleLev);
    IOBuff.AddStringW(m_strRoleName);
    IOBuff.AddUInt(m_nHarassNum);
    IOBuff.AddUInt(m_nLockedTimes);

    return true;
}


HarassReqDb::HarassReqDb()
: m_nStartTime(0)
, m_nEndTime(0)
{

}

HarassReqDb::~HarassReqDb()
{

}

RoleChatInfo::RoleChatInfo()
: m_nChannel(0)
, m_nChatTime(0)
{

}
RoleChatInfo::~RoleChatInfo()
{

}

bool RoleChatInfo::doDecode(CParamPool &IOBuff)
{
    m_nChannel = IOBuff.GetUInt();
    m_nChatTime = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strContent);

    return true;
}

bool RoleChatInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nChannel);
    IOBuff.AddUInt(m_nChatTime);
    IOBuff.AddStringW(m_strContent);

    return true;
}

HarassChatReqDb::HarassChatReqDb()
: m_nSlot(0)
, m_nRoleID(0)
, m_nStartTime(0)
, m_nEndTime(0)
{

}

HarassChatReqDb::~HarassChatReqDb()
{

}

DeviceRechargeInfo::DeviceRechargeInfo()
: m_strDeviceID("")
, m_nAccountCount(0)
, m_nRechargeCount(0)
, m_nAllAmount(0)
, m_bForbid(false)
{}

DeviceRechargeInfo::~DeviceRechargeInfo()
{}

void DeviceRechargeInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddStringW(m_strDeviceID);
    IOBuff.AddUInt(m_nAccountCount);
    IOBuff.AddUInt(m_nRechargeCount);
    IOBuff.AddUInt(m_nAllAmount);
    IOBuff.AddBool(m_bForbid);
}

IosForbidRechargeDB::IosForbidRechargeDB()
:m_nSlot(0)
, m_nStartTime(0)
, m_nEndTime(0)
, m_nQueryBeginIndex(0)
, m_nQueryCount(0)
{}

IosForbidRechargeDB::~IosForbidRechargeDB()
{}




