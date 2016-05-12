// NetDataStruct.cpp: implementation of the CClientInterface class.
//
/////////////////////////////////////////////////////////////////////
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include "DataStruct_Base.h"
#include "datainfor/AchievementConfig.h"
#include "../server/item/EntityItemComponent.h"
#include "../server/util/CommonFunc.h"
#include "../nsytworld/QuestMan.h"
#include "../server/player/PlayerSettingDefine.h"
#include <sstream>

unsigned int s_nBlockTime[8] = 
{//账号被封的时间，单位是秒
	//10min, 1hour   1day    7day    15day    30day    90day    永久
	600,     3600,   86400,  604800, 1296000, 2592000, 7776000, 0x7FFFFFFF

};

//////////////////////////////////////////////////////////////////////////

CreateRoleInfo::CreateRoleInfo()
{
    nAccount = 0;
    m_nVersionType = Version_Free;
    memset(szRoleName,0,MAX_NAME_LEN);
    nSex = ESexType_Male;
    m_strBirthday.clear();
    memset(szSignature,0,MAX_SIGNATURE_LEN);
    nColor = BODY_COLOR_YELLOW;
    nInstallerId = 0;
}

void CreateRoleInfo::SetBirthday(int nBirthday)
{
    std::stringstream ss;
    ss << nBirthday;
    ss >> m_strBirthday;

    if (m_strBirthday.size() > (size_t)g_ServerConfig.m_nMaxBirthdayLength)
        m_strBirthday.resize(g_ServerConfig.m_nMaxBirthdayLength);
}

void CreateRoleInfo::SetBirthday(const std::string &rBirthday)
{
    m_strBirthday = rBirthday;

    if (m_strBirthday.size() > (size_t)g_ServerConfig.m_nMaxBirthdayLength)
        m_strBirthday.resize(g_ServerConfig.m_nMaxBirthdayLength);
}

//////////////////////////////////////////////////////////////////////////
RoleImageOfflineData::RoleImageOfflineData()
    : m_nRoleID(0)
    , m_strRoleName("")
    , m_nSex(0)
    , m_nSkin(0)
    , m_nLevel(0)
    , m_bVIP(false)
    , m_nVIPLevel(0)
    , m_bHasCarriedPet(false)
{
}

RoleImageOfflineData::~RoleImageOfflineData()
{

}

RoleImageOfflineData & RoleImageOfflineData::operator=(const RoleImageOfflineData &src)
{
    m_nRoleID           = src.m_nRoleID;
    m_strRoleName       = src.m_strRoleName;
    m_nSex              = src.m_nSex;
    m_nSkin             = src.m_nSkin;
    m_nLevel            = src.m_nLevel;
    m_bVIP              = src.m_bVIP;
    m_nVIPLevel         = src.m_nVIPLevel;
    m_vecRoleItem       = src.m_vecRoleItem;
    m_bHasCarriedPet    = src.m_bHasCarriedPet;
    m_playerPet         = src.m_playerPet;
    m_playerPetSkill    = src.m_playerPetSkill;
    m_mapClothEffect    = src.m_mapClothEffect;
    m_mapClothEffectSevenColor = src.m_mapClothEffectSevenColor;

    return *this;
}

void RoleImageOfflineData::Reset()
{
    m_nRoleID = 0;
    m_strRoleName = "";
    m_nSex        = 0;
    m_nSkin       = 0;
    m_nLevel      = 0;
    m_bVIP        = false;
    m_nVIPLevel   = 0;
    m_vecRoleItem.clear();
    m_bHasCarriedPet = false;
    m_playerPet.Reset();
    m_playerPetSkill.Reset();
    m_mapClothEffectSevenColor.clear();
    m_mapClothEffect.clear();
}

void RoleImageOfflineData::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_nRoleID << stringw( m_strRoleName ) << m_nSex 
        << m_nSkin << m_nLevel << m_bVIP << m_nVIPLevel;

    IOBuff.AddUShort( (unsigned short)m_vecRoleItem.size() );
    for ( std::vector<DBItem>::iterator itr = m_vecRoleItem.begin(); 
        itr != m_vecRoleItem.end(); ++itr )
    {
        itr->doEncode( IOBuff );
    }

    IOBuff << m_bHasCarriedPet;

    if ( m_bHasCarriedPet )
    {
        m_playerPet.doEncode( IOBuff );
        m_playerPetSkill.doEncode( IOBuff );
    }

    IOBuff.AddUShort((unsigned short)m_mapClothEffect.size());
    for (std::map<itemid_t,unsigned int>::iterator it = m_mapClothEffect.begin();
        it != m_mapClothEffect.end(); ++it)
    {
        IOBuff.AddInt64(it->first);
        IOBuff.AddUInt(it->second);
    }

    IOBuff.AddUShort((unsigned short)m_mapClothEffectSevenColor.size());
    for (std::map<unsigned short,std::set<ESevenColorType> >::iterator it = m_mapClothEffectSevenColor.begin();
        it != m_mapClothEffectSevenColor.end(); ++it)
    {
        IOBuff.AddUShort(it->first);
        IOBuff.AddUShort((unsigned short)it->second.size());
        for (std::set<ESevenColorType>::iterator itColor = it->second.begin();
            itColor != it->second.end(); ++itColor)
        {
            IOBuff<<(unsigned char)*itColor;
        }
    }
}

void RoleImageOfflineData::doDecode( CParamPool &IOBuff )
{
    stringw strRoleName;
    IOBuff >> m_nRoleID >> strRoleName >> m_nSex 
        >> m_nSkin >> m_nLevel >> m_bVIP >> m_nVIPLevel;

    m_strRoleName = strRoleName.c_str();

    unsigned short nCount = IOBuff.GetUShort();
    for ( unsigned short i = 0; i < nCount; ++i )
    {
        DBItem info;
        info.doDecode( IOBuff );

        m_vecRoleItem.push_back( info );
    }

    IOBuff >> m_bHasCarriedPet;

    if ( m_bHasCarriedPet )
    {
        m_playerPet.doDecode( IOBuff );
        m_playerPetSkill.doDecode( IOBuff );
    }

    nCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nCount; ++i)
    {
        itemid_t nItemID = IOBuff.GetInt64();
        m_mapClothEffect[nItemID] = IOBuff.GetUInt();
    }

    nCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nCount; ++i)
    {
        unsigned short nSuitID = IOBuff.GetUShort();
        unsigned short nClothPosCount = IOBuff.GetUShort();
        for (unsigned short j = 0; j < nClothPosCount; ++j)
        {
            m_mapClothEffectSevenColor[nSuitID].insert((ESevenColorType)IOBuff.GetUChar());
        }
    }
}


//////////////////////////////////////////////////////////////////////////

RoleBaseInfo::RoleBaseInfo()
{
    m_nRoleID = 0;
    m_strName.clear();
}

RoleBaseInfo::RoleBaseInfo(unsigned int nRoleID)
{
    m_nRoleID = nRoleID;
    m_strName.clear();
}

RoleBaseInfo::RoleBaseInfo(const std::string &rName)
{
    m_nRoleID = 0;
    SetName(rName);
}

RoleBaseInfo::RoleBaseInfo(const RoleBaseInfo &rRoleBaseInfo)
{
    *this = rRoleBaseInfo;
}

RoleBaseInfo& RoleBaseInfo::operator=(const RoleBaseInfo &rRoleBaseInfo)
{
    if (this == &rRoleBaseInfo)
        return *this;

    m_nRoleID = rRoleBaseInfo.m_nRoleID;
    SetName(rRoleBaseInfo.m_strName);

    return *this;
}

bool RoleBaseInfo::operator==(const RoleBaseInfo &rRoleBaseInfo) const
{
    if (this == &rRoleBaseInfo)
        return true;

    return FindByID(rRoleBaseInfo.m_nRoleID) || FindByName(rRoleBaseInfo.GetName() );
}

void RoleBaseInfo::SetName(const std::string &rName)
{
    m_strName = rName;

    if (m_strName.size() > (size_t)g_ServerConfig.m_MaxNameLimit)
        m_strName.resize(g_ServerConfig.m_MaxNameLimit);
}

bool RoleBaseInfo::FindByID(unsigned int nRoleID) const
{
    return m_nRoleID == nRoleID;
}

bool RoleBaseInfo::FindByName(const std::string &rName) const
{
    return m_strName == rName;
}

void RoleBaseInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strName);
}

void RoleBaseInfo::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();

    std::string strTemp;
    IOBuff.GetStringW(strTemp);
    SetName(strTemp);
}

//////////////////////////////////////////////////////////////////////////

RoleAttrInfo::RoleAttrInfo()
{
    m_nPID = 0;
    m_nSex = 0;
    m_nMoney = 0;
    m_nExp = 0;
    m_nLevel = 0;
    m_nHot = 0;
    m_nPre = 0;
    m_nInt = 0;
    m_nAPt = 0;
    m_nConstellation = 0;
    m_nSkinColor = 0;
    m_nPriv = 0;
    m_nFamilyId = 0;
    memset(m_szFamilyName, 0, MAX_FAMILY_NAME);
    memset(m_szSignature, 0, MAX_SIGNATURE_LEN);
    memset(m_anBeiBaoKuozhan, 0, BEIBAOKUOZHAN_MAX_ITEM_TYPE_NUM);
    m_nBadgeGridNum = 0;
    m_nRoleCreateTime = 0;
    m_nLastLoginTime = 0;
    m_nLastLogoutTime = 0;
    m_nTimeOnline = 0;
    m_nAddictionTime = 0;
    m_nAddictionOfflineTime = 0;
    m_nQuestDone = 0;
    m_nLastMoneyResetTime = 0;
    m_nLastMoneyResetMailRemindTime = 0;
    m_nLastMoneyResetMallRemindTime = 0;
    m_nDanceGroupID = 0;
    memset(m_szInvitationCode, 0, MAX_INVITATION_CODE_LEN);
    m_nClothGroupId = 0;
    m_nDivorceAgreementID = 0;
    m_nAccountCreateTime = 0;
    m_strBirthday.clear();
}

RoleAttrInfo::RoleAttrInfo(const RoleAttrInfo &rRoleAttrInfo)
{
    *this = rRoleAttrInfo;
}

const RoleAttrInfo& RoleAttrInfo::operator=(const RoleAttrInfo &rRoleAttrInfo)
{
    if (this == &rRoleAttrInfo)
        return *this;

    RoleBaseInfo::operator=(rRoleAttrInfo);

    m_nPID = rRoleAttrInfo.m_nPID;
    m_nSex = rRoleAttrInfo.m_nSex;
    m_nMoney = rRoleAttrInfo.m_nMoney;
    m_nExp = rRoleAttrInfo.m_nExp;
    m_nLevel = rRoleAttrInfo.m_nLevel;
    m_nHot = rRoleAttrInfo.m_nHot;
    m_nPre = rRoleAttrInfo.m_nPre;
    m_nInt = rRoleAttrInfo.m_nInt;
    m_nAPt = rRoleAttrInfo.m_nAPt;
    m_nConstellation = rRoleAttrInfo.m_nConstellation;
    m_nSkinColor = rRoleAttrInfo.m_nSkinColor;
    m_nPriv = rRoleAttrInfo.m_nPriv;
    m_nFamilyId = rRoleAttrInfo.m_nFamilyId;
    strcpy(m_szFamilyName, rRoleAttrInfo.m_szFamilyName);
    strcpy(m_szSignature, rRoleAttrInfo.m_szSignature);
    strcpy((char *)m_anBeiBaoKuozhan, (char *)rRoleAttrInfo.m_anBeiBaoKuozhan);
    m_nBadgeGridNum = rRoleAttrInfo.m_nBadgeGridNum;
    m_nRoleCreateTime = rRoleAttrInfo.m_nRoleCreateTime;
    m_nLastLoginTime = rRoleAttrInfo.m_nLastLoginTime;
    m_nLastLogoutTime = rRoleAttrInfo.m_nLastLogoutTime;
    m_nTimeOnline = rRoleAttrInfo.m_nTimeOnline;
    m_nAddictionTime = rRoleAttrInfo.m_nAddictionTime;
    m_nAddictionOfflineTime = rRoleAttrInfo.m_nAddictionOfflineTime;
    m_nQuestDone = rRoleAttrInfo.m_nQuestDone;
    m_nLastMoneyResetTime = rRoleAttrInfo.m_nLastMoneyResetTime;
    m_nLastMoneyResetMailRemindTime = rRoleAttrInfo.m_nLastMoneyResetMailRemindTime;
    m_nLastMoneyResetMallRemindTime = rRoleAttrInfo.m_nLastMoneyResetMallRemindTime;
    m_nDanceGroupID = rRoleAttrInfo.m_nDanceGroupID;
    strcpy(m_szInvitationCode, rRoleAttrInfo.m_szInvitationCode);
    m_nClothGroupId = rRoleAttrInfo.m_nClothGroupId;
    m_nDivorceAgreementID = rRoleAttrInfo.m_nDivorceAgreementID;
    m_nAccountCreateTime = rRoleAttrInfo.m_nAccountCreateTime;
    SetBirthday(rRoleAttrInfo.m_strBirthday);

    return *this;
}

void RoleAttrInfo::SetBirthday(const std::string &rBirthday)
{
    m_strBirthday = rBirthday;

    if (m_strBirthday.size() > (size_t)g_ServerConfig.m_nMaxBirthdayLength)
        m_strBirthday.resize(g_ServerConfig.m_nMaxBirthdayLength);
}

void RoleAttrInfo::doEncode(CParamPool &IOBuff)
{
    RoleBaseInfo::doEncode(IOBuff);

    IOBuff.AddUShort(m_nPID);
    IOBuff.AddUChar(m_nSex);
    IOBuff.AddInt(m_nMoney);
    IOBuff.AddUInt(m_nExp);
    IOBuff.AddUShort(m_nLevel);

    IOBuff.AddUInt(m_nHot);
    IOBuff.AddUInt(m_nPre);
    IOBuff.AddUInt(m_nInt);
    IOBuff.AddUInt(m_nAPt);

    IOBuff.AddUChar(m_nConstellation);
    IOBuff.AddUChar(m_nSkinColor);

    IOBuff.AddUInt(m_nPriv);
    IOBuff.AddUInt(m_nFamilyId);

    IOBuff.AddString(m_szFamilyName);
    IOBuff.AddString(m_strBirthday);
    IOBuff.AddString(m_szSignature);

    for (int i = 0; i < BEIBAOKUOZHAN_MAX_ITEM_TYPE_NUM; ++i)
    {
        IOBuff.AddUChar(m_anBeiBaoKuozhan[i]);
    }

    IOBuff.AddUChar(m_nBadgeGridNum);

    IOBuff.AddUInt(m_nLastLoginTime);
    IOBuff.AddUInt(m_nTimeOnline);
    IOBuff.AddUInt(m_nAddictionTime);
    IOBuff.AddUInt(m_nAddictionOfflineTime);
    IOBuff.AddUInt(m_nQuestDone);

    IOBuff.AddString(m_szInvitationCode);
    IOBuff.AddUChar(m_nClothGroupId);
}

void RoleAttrInfo::doDecode(CParamPool &IOBuff)
{
    RoleBaseInfo::doDecode(IOBuff);

    m_nPID = IOBuff.GetUShort();
    m_nSex = IOBuff.GetUChar();
    m_nMoney = IOBuff.GetInt();
    m_nExp = IOBuff.GetUInt();
    m_nLevel = IOBuff.GetUShort();

    m_nHot = IOBuff.GetUInt();
    m_nPre = IOBuff.GetUInt();
    m_nInt = IOBuff.GetUInt();
    m_nAPt = IOBuff.GetUInt();

    m_nConstellation = IOBuff.GetUChar();
    m_nSkinColor = IOBuff.GetUChar();

    m_nPriv = IOBuff.GetUInt();
    m_nFamilyId = IOBuff.GetUInt();

    string strTemp;
    strTemp = IOBuff.GetString();
    if (strTemp.size() > MAX_FAMILY_NAME - 1)
    {
        strTemp.resize(MAX_FAMILY_NAME - 1);
    }
    memcpy(m_szFamilyName, strTemp.c_str(), strTemp.size() );

    IOBuff.GetStringW(strTemp);
    SetBirthday(strTemp);

    strTemp = IOBuff.GetString();
    if (strTemp.size() > (size_t)g_ServerConfig.m_SignatureLength - 1)
    {
        strTemp.resize(g_ServerConfig.m_SignatureLength - 1);
    }
    memcpy(m_szSignature, strTemp.c_str(), strTemp.size() );

    for (int i = 0; i < BEIBAOKUOZHAN_MAX_ITEM_TYPE_NUM; ++i)
    {
        m_anBeiBaoKuozhan[i] = IOBuff.GetUChar();
    }

    m_nBadgeGridNum = IOBuff.GetUChar();

    m_nLastLoginTime = IOBuff.GetUInt();
    m_nTimeOnline = IOBuff.GetUInt();
    m_nAddictionTime = IOBuff.GetUInt();
    m_nAddictionOfflineTime = IOBuff.GetUInt();
    m_nQuestDone = IOBuff.GetUInt();

    strTemp = IOBuff.GetString();
    if (strTemp.size() > MAX_INVITATION_CODE_LEN - 1)
    {
        strTemp.resize(MAX_INVITATION_CODE_LEN - 1);
    }
    memcpy(m_szInvitationCode, strTemp.c_str(), strTemp.size() );
    m_nClothGroupId = IOBuff.GetUChar();
}

//////////////////////////////////////////////////////////////////////////

list<TargetInfo>* QUESTINFO::GetTargetListBy(unsigned char nType) 
{
	if (nType <= TARGET_ITEMCOLLECTION){
		map< unsigned char, list<TargetInfo > >::iterator it = m_mapTarget.find(nType); 
		if (it != m_mapTarget.end()){
			return &(it->second);
		}
	}
	return NULL;
}

CRepeatedHistoryQuest::CRepeatedHistoryQuest(unsigned short nQuestID, unsigned int ttime, unsigned int nCount): 
m_nQuestID(nQuestID),
m_nTime(ttime),
m_nCount(nCount)
{
};

bool CRepeatedHistoryQuest::operator==(const CRepeatedHistoryQuest &rhs)
{
	if  (this == &rhs)
	{
		return true;
	}
	return rhs.m_nQuestID == m_nQuestID;
}
bool CRepeatedHistoryQuest::operator==(unsigned short nQuestID)
{
	return m_nQuestID == nQuestID;
}

CRepeatedHistoryQuest& CRepeatedHistoryQuest::operator=(const CRepeatedHistoryQuest& rhs){
	if (this == &rhs){
		return *this;
	}
	m_nQuestID = rhs.m_nQuestID;
	m_nTime = rhs.m_nTime;
	m_nCount = rhs.m_nCount;
	return *this;
}

void CRepeatedHistoryQuest::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nQuestID);
	IOBuff.AddUInt(m_nTime);
	IOBuff.AddUInt(m_nCount);
}

void CRepeatedHistoryQuest::doDecode(CParamPool &IOBuff)
{
	m_nQuestID = IOBuff.GetUShort();	
	m_nTime = IOBuff.GetUInt();
	m_nCount = IOBuff.GetUInt();
}

bool CTitleInfo::doEncode(CParamPool &IOBuff) const
{
	IOBuff.AddUInt(m_nTitleID);	
	IOBuff.AddUInt(m_nGainTitleTime);

    return true;
}

bool CTitleInfo::doDecode(CParamPool &IOBuff)
{
	m_nTitleID = IOBuff.GetUInt();
	m_nGainTitleTime = IOBuff.GetUInt();

    return true;
}

QUESTINFO::QUESTINFO(CQuestDoc *pQuest,CQuestDocNew* pQuestNew):m_pQuest(pQuest),m_pQuestNew(pQuestNew)
{ 
}

QUESTINFO::~QUESTINFO() 
{
	m_pQuest = NULL;
}

QUESTINFO::QUESTINFO(const QUESTINFO &rhs)
{
	Init(rhs);
}

QUESTINFO& QUESTINFO::operator=(const QUESTINFO& rhs)
{
	if (this == &rhs){
		return *this;
	}
	Init(rhs);
	return *this;
}

bool QUESTINFO::operator==(const QUESTINFO &rhs) const
{
	if (this == &rhs){
		return true;
	}
	return m_nQuestID == rhs.m_nQuestID;
}

bool QUESTINFO::operator==(unsigned short nQuestID)const
{
	return m_nQuestID == nQuestID;
}

void QUESTINFO::Init(const QUESTINFO& rhs)
{
	m_nQuestID = rhs.m_nQuestID;
	m_nTime = rhs.m_nTime;
	m_mapTarget = rhs.m_mapTarget;

	m_nState = rhs.m_nState;
	m_pQuest = rhs.m_pQuest;//指向同一块静态数据块,析构设为NULL;
}

unsigned int QUESTINFO::GetTargetCompletedCountBy(unsigned char nType, unsigned int nIndex)
{
	list<TargetInfo>* pList = GetTargetListBy(nType);	
	if (pList != NULL){
		list<TargetInfo>::iterator itr = find(pList->begin(), pList->end(), TargetInfo(nType, nIndex));
		if (itr != pList->end()){
			return itr->m_nCount;
		}
	}

	return 0;
}

unsigned short QUESTINFO::GetQuestID()
{
	return m_nQuestID;
}

void QUESTINFO::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nQuestID);
	IOBuff.AddUInt(m_nTime);
	IOBuff.AddUChar((unsigned char)m_mapTarget.size());
	map<unsigned char, list< TargetInfo> >::iterator it = m_mapTarget.begin();
	for (; it != m_mapTarget.end(); it++)
	{
		IOBuff.AddUChar((unsigned char)(it->second.size()));
		list<TargetInfo>::iterator pos = it->second.begin();
		for (; pos != it->second.end(); pos++){
			pos->doEncode(IOBuff);
		}
	}
}

void QUESTINFO::doDecode(CParamPool &IOBuff)
{
	m_nQuestID = IOBuff.GetUShort();	
	m_nTime = IOBuff.GetUInt();
	int nSize = IOBuff.GetUChar();
	while(nSize-- > 0) {
		list<TargetInfo> listTargetInfo;
		int nListSize = IOBuff.GetUChar();
		TargetInfo info;
		while (nListSize-- > 0) 
        {
			info.doDecode(IOBuff);
			listTargetInfo.push_back(info);
		}
		m_mapTarget.insert(make_pair(info.m_nType, listTargetInfo));
	}
}

bool QUESTNEWINFO::doDecode(CParamPool& IOBuff)
{
	m_RoleID = IOBuff.GetUInt();
	m_QuestID = IOBuff.GetUShort();
	m_State = IOBuff.GetUShort();
	m_Times = IOBuff.GetUShort();
	m_CreateTime = IOBuff.GetUInt();
	m_UpdateTime = IOBuff.GetUInt();
	m_DayTimes = IOBuff.GetUShort();
	m_DestroyType = IOBuff.GetUShort();

	IOBuff.GetStringW(m_Musics);
	IOBuff.GetStringW(m_UseItems);
	IOBuff.GetStringW(m_Functions);
    IOBuff.GetStringW(m_strJsonQuestParam);

    return true;
}

bool QUESTNEWINFO::doEncode(CParamPool& IOBuff) const
{
	IOBuff.AddUInt(m_RoleID);
	IOBuff.AddUShort(m_QuestID);
	IOBuff.AddUShort(m_State);
	IOBuff.AddUShort(m_Times);
	IOBuff.AddUInt(m_CreateTime);
	IOBuff.AddUInt(m_UpdateTime);
	IOBuff.AddUShort(m_DayTimes);
	IOBuff.AddUShort(m_DestroyType);

	IOBuff.AddStringW(m_Musics.c_str());
	IOBuff.AddStringW(m_UseItems.c_str());
	IOBuff.AddStringW(m_Functions.c_str());
    IOBuff.AddStringW(m_strJsonQuestParam.c_str());

    return true;
}

CCheckInInfo::CCheckInInfo()
	:m_nDayIndex(0)
	,m_nCheckInTime(0)
	,m_nVipOutTimeDayIndex(0)
{
}


CCheckInInfo::~CCheckInInfo()
{
}

CCheckInInfo::CCheckInInfo(const CCheckInInfo &rhs)
{
	m_nDayIndex = rhs.m_nDayIndex;
	m_nCheckInTime = rhs.m_nCheckInTime;
	m_nVipOutTimeDayIndex = rhs.m_nVipOutTimeDayIndex;
}

CCheckInInfo& CCheckInInfo::operator= (const CCheckInInfo &rhs)
{
	if (&rhs == this)
	{
		return *this;
	}

	m_nDayIndex = rhs.m_nDayIndex;
	m_nCheckInTime = rhs.m_nCheckInTime;
	m_nVipOutTimeDayIndex = rhs.m_nVipOutTimeDayIndex;

	return *this;
}

bool CCheckInInfo::doEncode(CParamPool & IOBuff) const
{
	IOBuff.AddUInt( m_nDayIndex );
	IOBuff.AddUInt( m_nCheckInTime );
	IOBuff.AddUInt( m_nVipOutTimeDayIndex );

    return true;
}

bool CCheckInInfo::doDecode(CParamPool & IOBuff)
{
	m_nDayIndex = IOBuff.GetUInt();
	m_nCheckInTime = IOBuff.GetUInt();
	m_nVipOutTimeDayIndex = IOBuff.GetUInt();

    return true;
}


CFriendOnlineInfo::CFriendOnlineInfo()
:m_nRoleID(0)
,m_strName("")
,m_strNotes("")
,m_nSex(0)
,m_nLevel(0)
,m_nLine(-1)
,m_nRoomID(0)
,m_nState(0)
,m_nRelationType(0)
,m_nCallBy(0)
,m_nCallTo(0)
,m_bIsVIP(false)
,m_nVIPLevel(0)
,m_nOfflineTime(0)
{
}

CFriendOnlineInfo::~CFriendOnlineInfo()
{
}

void CFriendOnlineInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddStringW(m_strName.c_str());
	IOBuff.AddStringW(m_strNotes.c_str());
	IOBuff.AddUChar(m_nSex);
	IOBuff.AddUShort(m_nLevel);
	IOBuff.AddInt(m_nLine);
	IOBuff.AddStringW(m_strLineName);
	IOBuff.AddUInt(m_nRoomID);
	IOBuff.AddUChar(m_nState);
	IOBuff.AddUChar(m_nRelationType);
	IOBuff.AddUChar(m_nCallBy);
	IOBuff.AddUChar(m_nCallTo);
	IOBuff.AddBool(m_bIsVIP);
	IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddUInt(m_nOfflineTime);
}

void CFriendOnlineInfo::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strName);
	IOBuff.GetStringW(m_strNotes);
	m_nSex = IOBuff.GetUChar();
	m_nLevel = IOBuff.GetUShort();
	m_nLine = IOBuff.GetInt();
	IOBuff.GetStringW(m_strLineName);
	m_nRoomID = IOBuff.GetUInt();
	m_nState = IOBuff.GetUChar();
	m_nRelationType = IOBuff.GetUChar();
	m_nCallBy = IOBuff.GetUChar();
	m_nCallTo = IOBuff.GetUChar();
	m_bIsVIP = IOBuff.GetBool();
	m_nVIPLevel = IOBuff.GetUShort();
    m_nOfflineTime = IOBuff.GetUInt();
}

CFindPlayerInfo::CFindPlayerInfo()
{
	m_nRoleID = 0;
	m_strRoleName = "";
	m_nLevel = 0;
	m_nSex = 0;
	m_nConstellation = 0;
	m_bIsVip = false;
	m_nVipLevel = 0;
}

CFindPlayerInfo::~CFindPlayerInfo()
{

}

void CFindPlayerInfo::doEncode(CParamPool &ioBuff)
{
	ioBuff.AddUInt(m_nRoleID);
	ioBuff.AddStringW(m_strRoleName.c_str());
	ioBuff.AddUShort(m_nLevel);
	ioBuff.AddUChar(m_nSex);
	ioBuff.AddUChar(m_nConstellation);
	//ioBuff.AddBool(m_bIsVip);
	ioBuff.AddUShort(m_nVipLevel);
}

void CFindPlayerInfo::doDecode(CParamPool &ioBuff)
{
	m_nRoleID = ioBuff.GetUInt();
	ioBuff.GetStringW(m_strRoleName);
	m_nLevel = ioBuff.GetUShort();
	m_nSex = ioBuff.GetUChar();
	m_nConstellation = ioBuff.GetUChar();
	//m_bIsVip = ioBuff.GetBool();
	m_nVipLevel = ioBuff.GetUShort();
}


CWorldRoleUpdateAttrInfo::CWorldRoleUpdateAttrInfo()
{
	m_nRoleID = 0;
	memset(m_szRoleName, 0, MAX_NAME_LEN);
	m_nLevel = 0;
	m_nMoney = 0;
}

CWorldRoleUpdateAttrInfo::~CWorldRoleUpdateAttrInfo()
{
}

void CWorldRoleUpdateAttrInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddStringW(m_szRoleName);
	IOBuff.AddUShort(m_nLevel);
	IOBuff.AddInt(m_nMoney);
}

void CWorldRoleUpdateAttrInfo::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();	
	std::string strName; 
	IOBuff.GetStringW(strName);
	if (strName.size() > MAX_NAME_LEN - 1) {
		strName.resize(MAX_NAME_LEN - 1);
	}
	memcpy(m_szRoleName, strName.c_str(), strName.size());
	m_nLevel = IOBuff.GetUShort();
	m_nMoney = IOBuff.GetInt();
}

CWorldRoleUpdateRoomInfo::CWorldRoleUpdateRoomInfo()
:m_nRoomID(0)
,m_nRoleRoomType(RoleRoomType_None)
,m_nRoleRoomState(RoleRoomState_None)
{
}

CWorldRoleUpdateRoomInfo::~CWorldRoleUpdateRoomInfo()
{
}


void CWorldRoleUpdateRoomInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUInt(m_nRoomID);
	IOBuff.AddInt(m_nRoleRoomType);
	IOBuff.AddInt(m_nRoleRoomState);
}

void CWorldRoleUpdateRoomInfo::doDecode(CParamPool & IOBuff)
{
	m_nRoomID = IOBuff.GetUInt();
	m_nRoleRoomType = IOBuff.GetInt();
	m_nRoleRoomState = IOBuff.GetInt();
}

CAchievementInfo::CAchievementInfo(unsigned short nAchievementID, unsigned int nCompletedTime, AchievementConfig *pAchievementDoc, unsigned char nState):
m_nAchievementID(nAchievementID),
m_nCompletedTime(nCompletedTime),
m_pAchievementDoc(pAchievementDoc),
m_nState(nState)
{
}

//bool CAchievementInfo::GreaterTime(CAchievementInfo &first, CAchievementInfo &second)
//{
//	return first.m_nCompletedTime > second.m_nCompletedTime;
//	return true;
//}


CAchievementInfo::~CAchievementInfo()
{
}

bool CAchievementInfo::operator==(unsigned int nAchievementID) const
{ 
	return m_nAchievementID == nAchievementID; 
}

void CAchievementInfo::SetAchievementDoc(AchievementConfig *pAchievementDoc)
{
	m_pAchievementDoc = pAchievementDoc;
}

time_t CAchievementInfo::GetCompletedTime() const
{
	return m_nCompletedTime;
}

AchievementConfig * CAchievementInfo::GetAchievementDoc() const
{ 
	return m_pAchievementDoc; 
}

bool CAchievementInfo::operator<(CAchievementInfo &rhs) const
{
	return m_nCompletedTime < rhs.m_nCompletedTime;
}

bool CAchievementInfo::doEncode(CParamPool & IOBuff) const
{
	IOBuff.AddUShort(m_nAchievementID);	
	IOBuff.AddUInt(m_nCompletedTime);
	IOBuff.AddUChar(m_nState);

    return true;
}

bool CAchievementInfo::doDecode(CParamPool & IOBuff)
{
	m_nAchievementID = IOBuff.GetUShort();
	m_nCompletedTime = IOBuff.GetUInt();
	m_nState = IOBuff.GetUChar();

    return true;
}

CWeeklyEvent::CWeeklyEvent(unsigned short nCount, unsigned int nLastEventTime):
m_nCount(nCount),
m_nLastEventTime(nLastEventTime)
{
}

CWeeklyEvent::~CWeeklyEvent()
{
}

void CWeeklyEvent::Clear()
{
	m_nCount = 0;
	m_nLastEventTime = 0;
}

void CWeeklyEvent::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUShort(m_nCount);	
	IOBuff.AddUInt(m_nLastEventTime);
}

void CWeeklyEvent::doDecode(CParamPool & IOBuff)
{
	m_nCount = IOBuff.GetUShort();
	m_nLastEventTime = IOBuff.GetUInt();
}

CTitleInfo::CTitleInfo(unsigned int nTitleID, unsigned int nGainTitleTime):
m_nTitleID(nTitleID),
m_nGainTitleTime(nGainTitleTime)
{
}

CTitleInfo::~CTitleInfo()
{
}

bool CTitleInfo::operator==(unsigned int nTitleID)
{
	return nTitleID == m_nTitleID;
}

bool CTitleInfo::operator==(const CTitleInfo &rhs)
{
	return m_nTitleID == rhs.m_nTitleID;
}

CFightResult::CFightResult()
{
	m_nModeID = 0;
	m_nDifficulty = 0;
	memset(m_nResult, 0, sizeof(m_nResult));
	m_nHistoryTotalScore = 0;
	m_nConPerfectNum = 0;
}

CFightResult::~CFightResult()
{
}

CFightResult & CFightResult::operator=(const CFightResult & rhs)
{
	if (this == &rhs)
	{
		return *this;
	}
	m_nModeID = rhs.m_nModeID;
	m_nDifficulty = rhs.m_nDifficulty;
	memcpy(m_nResult, rhs.m_nResult, sizeof(m_nResult));
	m_nHistoryTotalScore = rhs.m_nHistoryTotalScore;
	m_nConPerfectNum = rhs.m_nConPerfectNum;

	return *this;
}

bool CFightResult::doEncode(CParamPool &IOBuff) const
{
	IOBuff.AddUChar(m_nModeID);
	IOBuff.AddUChar(m_nDifficulty);
	for (int i = 0; i < 7; ++i) {
		IOBuff.AddUInt(m_nResult[i]);	
	}
	IOBuff.AddUInt(m_nHistoryTotalScore);	
	IOBuff.AddUInt(m_nConPerfectNum);	

    return true;
}

bool CFightResult::doDecode(CParamPool &IOBuff)
{
	m_nModeID = IOBuff.GetUChar();
	m_nDifficulty = IOBuff.GetUChar();
	for (int i = 0; i < 7; ++i) {
		m_nResult[i] = IOBuff.GetUInt();
	}

	m_nHistoryTotalScore = IOBuff.GetUInt();
	m_nConPerfectNum = IOBuff.GetUInt();

    return true;
}

CAutoAccount::CAutoAccount()
{
	m_nAccountID = 0;
	memset(m_szName, 0, MAX_NAME_LEN);
	memset(m_szPasswd, 0, MAX_PASS_LEN);
}

CAutoAccount::~CAutoAccount()
{
}

RebindAccountInfo::RebindAccountInfo()
: m_nResult(0)
, m_nSlot(0)
, m_nGateSessionID(0)
, m_nServerid(0)
, m_nPidOld(0)
, m_nPid(0)
, m_nAccount(0)
, m_nRoleid(0)
, m_strActcode("")
, m_strName("")
, m_strPass("")
, m_strPuidOld("")
, m_strPuid("")
{

}

RebindAccountInfo::~RebindAccountInfo()
{

}


CCheckInReward::CCheckInReward(unsigned char nDay, int nMoney)
:m_nDay(nDay)
,m_nMoney(nMoney)
{
}

CCheckInReward::~CCheckInReward()
{
}

void CCheckInReward::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nDay);
    IOBuff.AddInt(m_nMoney);

    IOBuff.AddUShort((unsigned short)m_listItem.size());
    if (m_listItem.size() > 0)
    {
        for (list<CItem>::iterator itr = m_listItem.begin(); itr != m_listItem.end(); itr++)
        {
            itr->doEncode(IOBuff);
        }
    }
}

TargetInfo::TargetInfo(unsigned char nType, unsigned int nIndex,unsigned short nCount):
m_nType(nType),m_nIndex(nIndex),m_nCount(nCount)
{
}

TargetInfo::TargetInfo(const TargetInfo &rhs){
	m_nType = rhs.m_nType;
	m_nIndex = rhs.m_nIndex;
	m_nCount = rhs.m_nCount;
}

TargetInfo::~TargetInfo()
{
}

bool TargetInfo::operator==(const TargetInfo &rhs)const
{
	if (this == &rhs)
	{
		return true;
	}
	return (m_nType == rhs.m_nType && m_nIndex == rhs.m_nIndex);
}

void TargetInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nType);
	IOBuff.AddUInt(m_nIndex);
	IOBuff.AddUShort(m_nCount);
}

void TargetInfo::doDecode(CParamPool &IOBuff)
{
	m_nType = IOBuff.GetUChar();
	m_nIndex = IOBuff.GetUInt();
	m_nCount = IOBuff.GetUShort();
}

//////////////////////////////////////////////////////////////////////////
PlayerPet::PlayerPet()
    : m_nPetID(0)
    , m_strName("")
    , m_strDefaultName("")
    , m_nPotentialID(0)
    , m_nPetAttributeType(0)
    , m_nPetType(0)
    , m_nLevel(0)
    , m_nMaxLevel(0)
    , m_nExp(0)
    , m_nMaxExpOfLevel(0)
    , m_nQualityType(0)
    , m_nEvolutionTimes(0)
    , m_nEndurance(0)
    , m_nEnduranceRecoveryTime(0)
    , m_nMaxEndurance(0)
{
}

PlayerPet::~PlayerPet()
{
}

void PlayerPet::Reset()
{
    m_nPetID = 0;
    m_strName = "";
    m_strDefaultName = "";
    m_nPotentialID = 0;
    m_nPetAttributeType = 0;
    m_nPetType = 0;
    m_nLevel = 0;
    m_nMaxLevel = 0;
    m_nExp = 0;
    m_nMaxExpOfLevel = 0;
    m_nQualityType = 0;
    m_nEvolutionTimes = 0;
    m_nEndurance = 0;
    m_nEnduranceRecoveryTime = 0;
    m_nMaxEndurance = 0;
}

bool PlayerPet::doEncode(CParamPool &IOBuff)
{
    stringw name, defaultName;
    name = m_strName;
    defaultName = m_strDefaultName;
    IOBuff << m_nPetID << name << defaultName << m_nPotentialID << m_nPetAttributeType << m_nPetType 
        << m_nLevel << m_nMaxLevel << m_nExp << m_nMaxExpOfLevel << m_nQualityType << m_nEvolutionTimes 
        << m_nEndurance << m_nEnduranceRecoveryTime << m_nMaxEndurance;
    return true;
}

bool PlayerPet::doDecode(CParamPool &IOBuff)
{
    stringw name, defaultName;
    IOBuff >> m_nPetID >> name >> defaultName >> m_nPotentialID >> m_nPetAttributeType >> m_nPetType 
        >> m_nLevel >> m_nMaxLevel >> m_nExp >> m_nMaxExpOfLevel >> m_nQualityType >> m_nEvolutionTimes 
        >> m_nEndurance >> m_nEnduranceRecoveryTime >> m_nMaxEndurance;
    m_strName = name.c_str();
    m_strDefaultName = name.c_str();
    return true;
}

PetMsgSkill::PetMsgSkill()
    : m_nSkillID(0)
    , m_strName("")
    , m_strAtlasName("")
    , m_strSpriteName("")
    , m_bActivated(false)
    , m_nLevel(0)
    , m_nMaxLevel(0)
    , m_nLevelUpCostType(0)
    , m_nLevelUpCostAmount(0)
    , m_strTip("")
    , m_strNextTip("")
    , m_strActivationTip("")
{
}

PetMsgSkill::~PetMsgSkill()
{
}

bool PetMsgSkill::doEncode(CParamPool &ioBuffer) const
{
    ioBuffer.AddUShort((unsigned short)m_nSkillID);
    ioBuffer.AddStringW(m_strName);
    ioBuffer.AddStringW(m_strAtlasName);
    ioBuffer.AddStringW(m_strSpriteName);
    ioBuffer << m_bActivated << m_nLevel << m_nMaxLevel << m_nLevelUpCostType << m_nLevelUpCostAmount;
    ioBuffer.AddStringW(m_strTip);
    ioBuffer.AddStringW(m_strNextTip);
    ioBuffer.AddStringW(m_strActivationTip);
    return true;
}

bool PetMsgSkill::doDecode(CParamPool &ioBuffer)
{
    m_nSkillID = (SkillID)ioBuffer.GetUShort();
    ioBuffer.GetStringW(m_strName);
    ioBuffer.GetStringW(m_strAtlasName);
    ioBuffer.GetStringW(m_strSpriteName);
    ioBuffer >> m_bActivated >> m_nLevel >> m_nMaxLevel >> m_nLevelUpCostType >> m_nLevelUpCostAmount;
    ioBuffer.GetStringW(m_strTip);
    ioBuffer.GetStringW(m_strNextTip);
    ioBuffer.GetStringW(m_strActivationTip);
    return true;
}

PlayerPetSkill::PlayerPetSkill()
    : m_nPetID(0)
    , m_nDanceSkillCount(0)
    , m_nFightSkillCount(0)
{
}

PlayerPetSkill::~PlayerPetSkill()
{
}

void PlayerPetSkill::Reset()
{
    m_nPetID = 0;
    m_nDanceSkillCount = 0;
    m_nFightSkillCount = 0;
    m_listDanceSkill.clear();
    m_listFightSkill.clear();
}

bool PlayerPetSkill::doEncode(CParamPool &IOBuff) const
{
    IOBuff << m_nPetID << m_nDanceSkillCount << m_nFightSkillCount
        << m_listDanceSkill << m_listFightSkill;
    return true;
}

bool PlayerPetSkill::doDecode(CParamPool &IOBuff)
{
    IOBuff >> m_nPetID >> m_nDanceSkillCount >> m_nFightSkillCount
        >> m_listDanceSkill >> m_listFightSkill;
    return true;
}

//////////////////////////////////////////////////////////////////////////

PlayerMedal::PlayerMedal()
    : m_nType( 0 )
    , m_nID( 0 )
{
}

bool PlayerMedal::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nType << m_nID;

    return true;
}

bool PlayerMedal::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nType >> m_nID;

    return true;
}

//////////////////////////////////////////////////////////////////////////
PlayerInfo::PlayerInfo()
{
    memset(m_szGeneBuff, 0, 10240);
    m_nGeneBuffLen = 0;

    memset(m_szItemBuff, 0, 65536);
    m_nItemBuffLen = 0;

    m_nRoleID = 0;
    m_nSex = 0;                                 //身体类型,即性别,Sex_Type
    m_nConstellation = 0;                       //星座
    m_nSkinColor = 0;                           //皮肤颜色
    m_bVIP = false;
    m_nLevel = 0;
    m_nTitle = 0 ;
    m_nHot = 0 ;
    m_nSocial = 0 ;
    m_nExp = 0;
    m_nCurLine = 0;
    m_nRoleID = 0;
    m_nRoomState = 0;
    m_nVIPLevel = 1;
    m_nAchScore = 0;
    m_nDanceGroupID = 0;
    m_nCoupleID = 0;
    m_nCoupleDate = 0;
    m_nMarriageDate = 0;
    m_nSmallRoleID = 0;

    m_strRoleName.clear();
    m_strBirthday.clear();
    m_strSignature.clear();
    m_strCurLineName.clear();
    m_strDanceGroupName.clear();
    m_strCoupleName.clear();

    m_bHasPet = false;
    m_nMedalScore = 0;

    m_nPerpetualEquipCount = 0;
    m_nPerpetualEffectScore = 0;
}

void PlayerInfo::SetRoleName(const std::string &rRoleName)
{
    m_strRoleName = rRoleName;
    if (m_strRoleName.size() > (size_t)g_ServerConfig.m_MaxNameLimit)
        m_strRoleName.resize(g_ServerConfig.m_MaxNameLimit);
}

void PlayerInfo::SetBirthday(const std::string &rBirthday)
{
    m_strBirthday = rBirthday;

    if (m_strBirthday.size() > (size_t)g_ServerConfig.m_nMaxBirthdayLength)
        m_strBirthday.resize(g_ServerConfig.m_nMaxBirthdayLength);
}

void PlayerInfo::SetSignature(const std::string &rSignature)
{
    m_strSignature = rSignature;

    if (m_strSignature.size() > (size_t)g_ServerConfig.m_SignatureLength)
        m_strSignature.resize(g_ServerConfig.m_SignatureLength);
}

void PlayerInfo::SetCurLineName(const std::string &rCurLineName)
{
    m_strCurLineName = rCurLineName;
}

void PlayerInfo::SetDanceGroupName(const std::string &rDanceGroupName)
{
    m_strDanceGroupName = rDanceGroupName;

    if (m_strDanceGroupName.size() > (size_t)g_ServerConfig.m_DanceGroupNameLength)
        m_strDanceGroupName.resize(g_ServerConfig.m_DanceGroupNameLength);
}

void PlayerInfo::SetCoupleName(const std::string &rCoupleName)
{
    m_strCoupleName = rCoupleName;

    if (m_strCoupleName.size() > (size_t)g_ServerConfig.m_MaxNameLimit)
        m_strCoupleName.resize(g_ServerConfig.m_MaxNameLimit);
}

void PlayerInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName);
    IOBuff.AddUChar(m_nSex);
    IOBuff.AddStringW(m_strBirthday);
    IOBuff.AddUChar(m_nConstellation);
    IOBuff.AddStringW(m_strSignature);
    IOBuff.AddUChar(m_nSkinColor);
    IOBuff.AddUChar(m_bVIP ? 1 : 0);
    IOBuff.AddUShort(m_nLevel);

    IOBuff.AddUShort(m_nTitle) ;	//称号
    IOBuff.AddUInt(m_nHot) ;		//人气
    IOBuff.AddUInt(m_nSocial);		//社交指数

    IOBuff.AddUInt(m_nExp);
    IOBuff.AddInt(m_nCurLine);
    IOBuff.AddStringW(m_strCurLineName);
    IOBuff.AddUShort(m_nRoomID);
    IOBuff.AddUChar(m_nRoomState);

    unsigned short nSize = (unsigned short)m_vecEquipedItem.size();
    IOBuff.AddUShort(nSize);
    for (vector<itemtype_t>::const_iterator itr = m_vecEquipedItem.begin(); itr != m_vecEquipedItem.end(); ++itr)
    {
        IOBuff.AddUInt(*itr);
    }

    nSize = (unsigned short)m_vecWishItem.size();
    IOBuff.AddUShort(nSize);
    for (vector<itemtype_t>::const_iterator itr = m_vecWishItem.begin(); itr != m_vecWishItem.end(); ++itr)
    {
        IOBuff.AddUInt(*itr);
    }

    IOBuff.AddUChar(m_nVIPLevel);
    IOBuff.AddUInt(m_nAchScore);

    nSize = (unsigned short)m_vecAch.size();
    IOBuff.AddUShort(nSize);
    for (vector<unsigned int>::const_iterator itr = m_vecAch.begin(); itr != m_vecAch.end(); ++itr)
    {
        IOBuff.AddUInt(*itr);
    }

    unsigned short nBuffLen = (unsigned short)m_nItemBuffLen;
    IOBuff.AddPerFixSizeMemory(m_szItemBuff, nBuffLen);

    nBuffLen = (unsigned short)m_nGeneBuffLen;
    IOBuff.AddPerFixSizeMemory(m_szGeneBuff, nBuffLen);

    IOBuff.AddBool(m_bHasPet);
    if (m_bHasPet)
    {
        m_playerPet.doEncode(IOBuff);
        m_playerPetSkill.doEncode(IOBuff);
    }

    IOBuff.AddUInt(m_nDanceGroupID);
    IOBuff.AddStringW(m_strDanceGroupName);

    IOBuff.AddUInt(m_nCoupleID);
    IOBuff.AddStringW(m_strCoupleName);
    IOBuff.AddUInt(m_nCoupleDate);
    IOBuff.AddUInt(m_nMarriageDate);
    IOBuff.AddUInt(m_nSmallRoleID);

    IOBuff << m_listActivityMedals << m_listShowMedals << m_nMedalScore;
    IOBuff << m_nPerpetualEquipCount << m_nPerpetualEffectScore;
}

void PlayerInfo::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();

    std::string strTemp;
    IOBuff.GetStringW(strTemp);
    SetRoleName(strTemp);

    m_nSex = IOBuff.GetUChar();

    IOBuff.GetStringW(strTemp);
    SetBirthday(strTemp);

    m_nConstellation = IOBuff.GetUChar();

    IOBuff.GetStringW(strTemp);
    SetSignature(strTemp);

    m_nSkinColor = IOBuff.GetUChar();
    m_bVIP = IOBuff.GetUChar() > 0 ? true : false;
    m_nLevel = IOBuff.GetUShort();

    m_nTitle = IOBuff.GetUShort();
    m_nHot = IOBuff.GetUInt();
    m_nSocial = IOBuff.GetUInt();

    m_nExp = IOBuff.GetUInt();
    m_nCurLine = IOBuff.GetInt();

    IOBuff.GetStringW(strTemp);
    SetCurLineName(strTemp);

    m_nRoomID = IOBuff.GetUShort();
    m_nRoomState = IOBuff.GetUChar();

    unsigned short nSize = IOBuff.GetUShort();
    while (nSize-- > 0)
    {
        m_vecEquipedItem.push_back(IOBuff.GetUInt() );
    }

    nSize = IOBuff.GetUShort();
    while (nSize-- > 0)
    {
        m_vecWishItem.push_back(IOBuff.GetUInt() );
    }

    m_nVIPLevel = IOBuff.GetUChar();
    m_nAchScore = IOBuff.GetUInt();

    nSize = IOBuff.GetUShort();
    while (nSize-- > 0)
    {
        m_vecAch.push_back(IOBuff.GetUInt() );
    }

    unsigned short nBuffLen = 0;
    IOBuff.GetPerFixSizeMemory(m_szItemBuff, nBuffLen);
    m_nItemBuffLen = nBuffLen;

    nBuffLen = 0;
    IOBuff.GetPerFixSizeMemory(m_szGeneBuff, nBuffLen);
    m_nGeneBuffLen = nBuffLen;

    m_bHasPet = IOBuff.GetBool();
    if (m_bHasPet)
    {
        m_playerPet.doDecode(IOBuff);
        m_playerPetSkill.doDecode(IOBuff);
    }

    m_nDanceGroupID = IOBuff.GetUInt();

    IOBuff.GetStringW(strTemp);
    SetDanceGroupName(strTemp);

    m_nCoupleID = IOBuff.GetUInt();

    IOBuff.GetStringW(strTemp);
    SetCoupleName(strTemp);

    m_nCoupleDate = IOBuff.GetUInt();
    m_nMarriageDate = IOBuff.GetUInt();
    m_nSmallRoleID = IOBuff.GetUInt();

    IOBuff >> m_listActivityMedals >> m_listShowMedals >> m_nMedalScore;
    IOBuff >> m_nPerpetualEquipCount >> m_nPerpetualEffectScore;
}

//////////////////////////////////////////////////////////////////////////

CActivateGiftInfo::CActivateGiftInfo():
m_strCode(""),
m_nGiftID(0),
m_nGiftType(0),
m_nItemID(0),
m_nStartTime(0),
m_nEndTime(0),
m_nItemCount(0),
m_nOwnRoleID(0),
m_nDrawTime(0)	
{
}

CActivateGiftInfo::CActivateGiftInfo(const CActivateGiftInfo &rhs)
{
	m_strCode = rhs.m_strCode;
	m_nGiftID = rhs.m_nGiftID;
	m_nGiftType = rhs.m_nGiftType;
	m_nItemID = rhs.m_nItemID;
	m_nStartTime = rhs.m_nStartTime;
	m_nEndTime = rhs.m_nEndTime;
	m_nItemCount = rhs.m_nItemCount;
	m_nOwnRoleID = rhs.m_nOwnRoleID;
	m_nDrawTime = rhs.m_nDrawTime;
}

CActivateGiftInfo &CActivateGiftInfo::operator=(const CActivateGiftInfo &rhs)
{
	if (this == &rhs) {
		return *this;
	}

	m_strCode = rhs.m_strCode;
	m_nGiftID = rhs.m_nGiftID;
	m_nGiftType = rhs.m_nGiftType;
	m_nItemID = rhs.m_nItemID;
	m_nStartTime = rhs.m_nStartTime;
	m_nEndTime = rhs.m_nEndTime;
	m_nItemCount = rhs.m_nItemCount;
	m_nOwnRoleID = rhs.m_nOwnRoleID;
	m_nDrawTime = rhs.m_nDrawTime;
	
	return *this;
}

CActivateGiftInfo::~CActivateGiftInfo()
{
}

CFriendRelation::CFriendRelation( int nRelation, int nCallBy, int nCallTo )
:m_nRelation(nRelation)
,m_nCallBy(nCallBy)
,m_nCallTo(nCallTo)
{
}

bool CFriendRelation::operator==(const CFriendRelation &rhs) const
{
	if (&rhs == this) {
		return true;
	}

	return m_nCallBy == rhs.m_nCallBy && m_nCallTo == rhs.m_nCallTo/* && m_nRelation == rhs.m_nRelation */;
}


CFriendRelation::~CFriendRelation()
{
}

CCalling::CCalling(int nCodeNum, std::string strName, int reqGender)
:m_nCodeNum(nCodeNum)
,m_strName(strName)
,m_nReqGender(reqGender)
{
	
}

CCalling::~CCalling()
{
	
}

bool CCalling::operator==(const CCalling &rhs) const
{
	return m_nCodeNum == rhs.m_nCodeNum;	
}

CGameServerInfo::CGameServerInfo(int nLine, string strServerName, int nOnlinePlayersNum, int nLimitNum, unsigned char nInstallerID, bool bEnableAudition):
m_nLine(nLine),
m_strServerName(strServerName),
m_nOnlinePlayersNum(nOnlinePlayersNum),
m_nLimitNum(nLimitNum),
m_nInstallerID(nInstallerID),
m_bEnableAudition(bEnableAudition)
{
}

CGameServerInfo & CGameServerInfo::operator=(const CGameServerInfo &rhs)
{
	if (&rhs == this) {
		return *this;
	}

	m_nLine = rhs.m_nLine;
	m_strServerName = rhs.m_strServerName;
	m_nOnlinePlayersNum = rhs.m_nOnlinePlayersNum;
	m_nLimitNum = rhs.m_nLimitNum;
	m_nInstallerID = rhs.m_nInstallerID;
	m_bEnableAudition = rhs.m_bEnableAudition;

	return *this;
}

CGameServerInfo::CGameServerInfo(const CGameServerInfo & rhs)
{
	m_nLine = rhs.m_nLine;
	m_strServerName = rhs.m_strServerName;
	m_nOnlinePlayersNum = rhs.m_nOnlinePlayersNum;
	m_nLimitNum = rhs.m_nLimitNum;
	m_nInstallerID = rhs.m_nInstallerID;
	m_bEnableAudition = rhs.m_bEnableAudition;
}

CGameServerInfo::~CGameServerInfo()
{
}


void CGameServerInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddInt(m_nLine);
	IOBuff.AddStringW(m_strServerName.c_str());
	IOBuff.AddInt(m_nOnlinePlayersNum);
	IOBuff.AddInt(m_nLimitNum);
	IOBuff.AddUChar(m_nInstallerID);
	IOBuff.AddBool(m_bEnableAudition);
}

DBItem::DBItem()
    :m_nItemID(0)
    ,m_nTypeID(0)
    ,m_nIndex(0)
    ,m_nColumn(0)
    ,m_nCount(0)
    ,m_nUseTimes(0)
    ,m_nDuration(0)
    ,m_bEquiped(false)
    ,m_nDynamicBoxRelatedValue(0)
{
}

void DBItem::Initialize(itemid_t nItemID, itemtype_t nItemType, int nColumn, int nSlot, int nCount, int nUseTimes,
				int nDuration, int nDynamiceBoxRelatedValue, bool bEquiped)
{
	m_nItemID = nItemID;
	m_nTypeID = nItemType;
    m_nIndex = (unsigned short)nSlot;
    m_nColumn = (unsigned short)nColumn;
    m_nCount = (unsigned short)nCount;
    m_nUseTimes = (short)nUseTimes;
	m_nDuration = nDuration;
	m_nDynamicBoxRelatedValue = nDynamiceBoxRelatedValue;
	m_bEquiped = bEquiped;
}

bool DBItem::doEncode(CParamPool &IOBuff) const
{
    IOBuff.AddInt64(m_nItemID);	
    IOBuff.AddUInt(m_nTypeID);
    IOBuff.AddUShort(m_nIndex);
    IOBuff.AddUShort(m_nColumn);
    IOBuff.AddUShort(m_nCount);
    IOBuff.AddShort(m_nUseTimes);
    IOBuff.AddInt(m_nDuration);
    IOBuff.AddBool( m_bEquiped );
    IOBuff.AddInt( m_nDynamicBoxRelatedValue );

    return true;
}

bool DBItem::doDecode(CParamPool &IOBuff)
{
    m_nItemID = IOBuff.GetInt64();
    m_nTypeID = IOBuff.GetUInt();
    m_nIndex = IOBuff.GetUShort();
    m_nColumn = IOBuff.GetUShort();
    m_nCount = IOBuff.GetUShort();
    m_nUseTimes = IOBuff.GetShort();
    m_nDuration = IOBuff.GetInt();
    m_bEquiped = IOBuff.GetBool();
    m_nDynamicBoxRelatedValue = IOBuff.GetInt();

    return true;
}

CRoleVIPInfo::CRoleVIPInfo()
:m_nLevel(0)
,m_nOpenTime(0)
,m_nExp(0)
,m_nExpiredTime(0)
,m_nLastGetRewardTime(0)
,m_nLastOnlineAnnounceTime(0)
,m_nConsumedPt(0)
,m_nExchangedPt(0)
,m_bValid(false)
{
}

CRoleVIPInfo::~CRoleVIPInfo()
{
}

bool CRoleVIPInfo::IsVIP()
{
	return m_nExpiredTime > (long long)time(NULL);
}

bool CRoleVIPInfo::IsExpiredToday() const
{
    if ( m_nExpiredTime <= 0 )
        return false;

    time_t nNow = time( NULL );
    long long nTodayStartTime = (long long)GetTodayStartTime( nNow );
    long long nNextStartTime = nTodayStartTime + 24 * 60 * 60;

    if ( nTodayStartTime <= m_nExpiredTime && m_nExpiredTime < nNextStartTime )
        return true;

    return false;
}

bool CRoleVIPInfo::CanGetDailyReward()
{
	return (!IsTimeIsToday(m_nLastGetRewardTime) && m_nLastGetRewardTime < (unsigned int)time(NULL));
}

void CRoleVIPInfo::SetConsumedPt(int nConsumedPt)
{
	m_nConsumedPt = nConsumedPt;
}

int CRoleVIPInfo::ConsumedPt()
{
	return m_nConsumedPt;
}

void CRoleVIPInfo::SetExchangedPt(int nExchangedPt)
{
	m_nExchangedPt = nExchangedPt;
}

int CRoleVIPInfo::ExchangedPt()
{
	return m_nExchangedPt;
}

void CRoleVIPInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nLevel);
    IOBuff.AddUShort((unsigned short)m_nExp);
	IOBuff.AddInt64(m_nExpiredTime);
}

void CRoleVIPInfo::doEncodeToGame(CParamPool & IOBuff)
{
}

void CRoleVIPInfo::doDecodeFromGame(CParamPool & IOBuff)
{
}

CAnnouncement::CAnnouncement():
m_nType(0),
m_strTitle(""),
m_strContent(""),
m_strDate(""),
m_bHot(false)
,m_nLinkTo(0)
,m_nPID(0)
{
}

CAnnouncement::~CAnnouncement()
{
}

void CAnnouncement::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nType);
	IOBuff.AddStringW(m_strTitle.c_str());
	IOBuff.AddStringW(m_strContent.c_str());
	IOBuff.AddStringW(m_strDate.c_str());
	IOBuff.AddBool(m_bHot);

	IOBuff.AddUShort((unsigned short)m_vecStrIcon.size());
	if (m_vecStrIcon.size() > 0) {
		for (vector<string>::iterator itr = m_vecStrIcon.begin(); itr != m_vecStrIcon.end();++itr) {
			IOBuff.AddStringW((*itr).c_str());
		}
	}
	IOBuff.AddUChar(m_nLinkTo);
}

CMailInfo::CMailInfo()
:m_strTitle("")
,m_strContent("")
,m_nItemType(0)
,m_nItemCount(0)
,m_nItemDuration(0)
,m_nMoney(0)
{
}

CMailInfo::~CMailInfo()
{
}

void CMailInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strTitle.c_str());
	IOBuff.AddStringW(m_strContent.c_str());
	IOBuff.AddUInt(m_nItemType);
	IOBuff.AddInt(m_nItemCount);
	IOBuff.AddInt(m_nItemDuration);
	IOBuff.AddInt(m_nMoney);
}

void CMailInfo::doDecode(CParamPool &IOBuff)
{
	IOBuff.GetStringW(m_strTitle);
	IOBuff.GetStringW(m_strContent);
	m_nItemType = IOBuff.GetUInt();
	m_nItemCount = IOBuff.GetInt();
	m_nItemDuration = IOBuff.GetInt();
	m_nMoney = IOBuff.GetInt();
}

CQueryGiftInfo::CQueryGiftInfo()
:m_nGiftID(0)
,m_strStartTime("")
,m_strEndTime("")
,m_nActivatedNum(0)
{
}

CQueryGiftInfo::~CQueryGiftInfo()
{
}

CRcvSysMailRoleInfo::CRcvSysMailRoleInfo()
:m_nPid(0)
,m_strPUID("")
,m_bSuc(false)
,m_strRoleName("")
,m_nRoleID(0)
{

}

CRcvSysMailRoleInfo::~CRcvSysMailRoleInfo()
{
}

void CRcvSysMailRoleInfo::doEncodeServ(CParamPool & IOBuff)
{
	IOBuff.AddUShort(m_nPid);
	IOBuff.AddStringW(m_strPUID.c_str());
	IOBuff.AddBool(m_bSuc);
	IOBuff.AddString(m_strRoleName.c_str());
	m_MailInfo.doEncode(IOBuff);
}

void CRcvSysMailRoleInfo::doDecodeServ(CParamPool & IOBuff)
{
	m_nPid = IOBuff.GetUShort();
	IOBuff.GetStringW(m_strPUID);
	if (m_strPUID.size() > MAX_PUID_LEN)
	{
		m_strPUID.resize(MAX_PUID_LEN);
	}
	m_bSuc = IOBuff.GetBool();
	m_strRoleName = IOBuff.GetString();
	m_MailInfo.doDecode(IOBuff);
}

void CRcvSysMailRoleInfo::doEncodeCli( CParamPool & IOBuff )
{
	IOBuff.AddUShort(m_nPid);
	IOBuff.AddStringW(m_strPUID.c_str());
	IOBuff.AddBool(m_bSuc);
	m_MailInfo.doEncode(IOBuff);
}

void CRcvSysMailRoleInfo::doDecodeCli( CParamPool & IOBuff )
{
	m_nPid = IOBuff.GetUShort();
	IOBuff.GetStringW(m_strPUID);
	if (m_strPUID.size() > MAX_PUID_LEN)
	{
		m_strPUID.resize(MAX_PUID_LEN);
	}
	m_MailInfo.doDecode(IOBuff);
}

CQueryGiftState::CQueryGiftState()
:m_nGiftID(0)
,m_nDrawTime(0)
,m_nRoleID(0)
,m_nStartTime(0)
,m_nEndTime(0)
,m_bValid(false)
,m_strGiftCode("")
,m_strDrawTime("")
{
}

CQueryGiftState::~CQueryGiftState()
{
}

bool CQueryGiftState::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddStringW(m_strGiftCode.c_str());
	IOBuff.AddUInt(m_nGiftID);
	IOBuff.AddStringW(m_strDrawTime.c_str());

	IOBuff.AddUInt(m_nDrawTime);
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUInt(m_nStartTime);
	IOBuff.AddUInt(m_nEndTime);
	IOBuff.AddBool(m_bValid);

	return true;
}

bool CQueryGiftState::doDecode( CParamPool & IOBuff )
{
	IOBuff.GetStringW(m_strGiftCode);	
	m_nGiftID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strDrawTime);

	m_nDrawTime = IOBuff.GetUInt();
	m_nRoleID = IOBuff.GetUInt();
	m_nStartTime = IOBuff.GetUInt();
	m_nEndTime = IOBuff.GetUInt();
	m_bValid = IOBuff.GetBool();

	return true;
}

CAskMallGoodInfo::CAskMallGoodInfo():
m_nSellGoodType(0),
m_nSellIndex(0)
{

}

CAskMallGoodInfo::~CAskMallGoodInfo()
{

}

void CAskMallGoodInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUInt(m_nSellGoodType);
	IOBuff.AddUChar(m_nSellIndex);
}

void CAskMallGoodInfo::doDecode(CParamPool & IOBuff)
{
	m_nSellGoodType = IOBuff.GetUInt();
	m_nSellIndex = IOBuff.GetUChar();
}

CAnnounceBase::CAnnounceBase()
:m_bShowInChatWindow(false)
,m_bIsAdminChat(false)
,m_strContent("")
,m_nPID(PID_NONE)
{

}

CAnnounceBase::~CAnnounceBase()
{

}

void CAnnounceBase::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddBool(m_bShowInChatWindow);
	IOBuff.AddBool(m_bIsAdminChat);
	IOBuff.AddStringW(m_strContent.c_str());
	IOBuff.AddUShort(m_nPID);
}

void CAnnounceBase::doDecode( CParamPool & IOBuff )
{
	m_bShowInChatWindow = IOBuff.GetBool();
	m_bIsAdminChat = IOBuff.GetBool();
	IOBuff.GetStringW(m_strContent);
	m_nPID = IOBuff.GetUShort();
}


CPlayerSetting::CPlayerSetting()
{
	m_mapOption[EPlayerSetting_ReceiveRoomInvitation] = 1;
	m_mapOption[EPlayerSetting_LBS] = 1;
	m_mapOption[EPlayerSetting_WifiDownload] = 1;
}

CPlayerSetting::~CPlayerSetting()
{

}

bool CPlayerSetting::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_mapOption;
    return true;
}

bool CPlayerSetting::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_mapOption;
    return true;
}

CPlayerPosition::CPlayerPosition()
:m_ESceneID(ESceneMove_None)
,m_nMoveType(0)
,m_fOrient(0.0f)
,m_fPosX(0.0f)
,m_fPosY(0.0f)
,m_fPosZ(0.0f)
{

}

CPlayerPosition::~CPlayerPosition()
{

}

void CPlayerPosition::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar(m_nMoveType);
	IOBuff.AddFloat(m_fOrient);
	IOBuff.AddFloat(m_fPosX);
	IOBuff.AddFloat(m_fPosY);
	IOBuff.AddFloat(m_fPosZ);
}

void CPlayerPosition::doDecode(CParamPool & IOBuff)
{
    m_nMoveType = IOBuff.GetUChar();
    m_fOrient = IOBuff.GetFloat();
    m_fPosX = IOBuff.GetFloat();
    m_fPosY = IOBuff.GetFloat();
    m_fPosZ = IOBuff.GetFloat();

    return ;
}


void CPlayerPosition::Reset()
{
	m_ESceneID = ESceneMove_None;
	m_fOrient = 0.0f;
	m_fPosX = 0.0f;
	m_fPosY = 0.0f;
	m_fPosZ = 0.0f;
}


//----------------------------------------------CFestivalBlessEntry
CFestivalBlessEntry::CFestivalBlessEntry()
:m_nSenderID(0)
,m_strSenderName("")
,m_nWordsIndex(0)
,m_nBlessTime(0)
{

}

CFestivalBlessEntry::~CFestivalBlessEntry()
{

}

void CFestivalBlessEntry::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddStringW(m_strSenderName.c_str());
	IOBuff.AddUChar((unsigned char)m_nWordsIndex);
}

bool TimeEggEntry::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nEggID);
	IOBuff.AddUInt(m_nSceneID);
	IOBuff.AddUInt(m_nBeginTime);
	IOBuff.AddUInt(m_nEndTime);
	IOBuff.AddUInt(m_nDestroyTime);
	IOBuff.AddUInt(m_nRelation);
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUInt(m_nOpened);
	return true;
}

bool TimeEggEntry::doDecode( CParamPool &IOBuff )
{
	m_nEggID = IOBuff.GetUInt();
	m_nSceneID = IOBuff.GetUInt();
	m_nBeginTime = IOBuff.GetUInt();
	m_nEndTime = IOBuff.GetUInt();
	m_nDestroyTime = IOBuff.GetUInt();
	m_nRelation = IOBuff.GetUInt();
	m_nRoleID = IOBuff.GetUInt();
	m_nOpened = IOBuff.GetUInt();
	return true;
}

bool TimeEggSetting::doEncode( CParamPool & IOBuff ) const
{
	IOBuff << m_nTimeEggOpenDuration
		<< m_nTimeEggDestroyDuration
		<< m_listCostItem
		<< m_listGiftItem
		<< m_listTogetherMaleItem
		<< m_listTogetherFemaleItem
		<< m_listMaleItem
		<< m_listFemaleItem
		<< m_nFixedMoneyReward
		<< m_nExtraMoneyReward
		<< m_nGiftDiscount
		<< m_strHelpTitle
		<< m_strHelpContent;
	return true;
}

bool TimeEggSetting::doDecode( CParamPool & IOBuff )
{
	IOBuff >> m_nTimeEggOpenDuration
		>> m_nTimeEggDestroyDuration
		>> m_listCostItem
		>> m_listGiftItem
		>> m_listTogetherMaleItem
		>> m_listTogetherFemaleItem
		>> m_listMaleItem
		>> m_listFemaleItem
		>> m_nFixedMoneyReward
		>> m_nExtraMoneyReward
		>> m_nGiftDiscount
		>> m_strHelpTitle
		>> m_strHelpContent;
	return true;
}

MagicLamp::MagicLamp()
    :m_nCrytsal(0)
    ,m_nLuckStar(0)
    ,m_nMagicDust(0)
    ,m_nMaxMainline(0)
    ,m_nMaxTheme(0)
    ,m_nCrystalRestoreTime(0)
    ,m_nNextDayResetTime(0)
    ,m_nTodayHaveGetVitNum(0)
    ,m_nFavorableBuyVitCount(0)
{
}

MagicLamp::MagicLamp(const MagicLamp & record)
{
    m_nCrytsal = record.m_nCrytsal;
    m_nLuckStar = record.m_nLuckStar;
    m_nMagicDust = record.m_nMagicDust;
    m_nMaxMainline = record.m_nMaxMainline;
    m_nMaxTheme = record.m_nMaxTheme;
    m_nCrystalRestoreTime = record.m_nCrystalRestoreTime;
    m_nNextDayResetTime = record.m_nNextDayResetTime;
    m_nTodayHaveGetVitNum = record.m_nTodayHaveGetVitNum;
    m_nFavorableBuyVitCount = record.m_nFavorableBuyVitCount;
}

MagicLamp::~MagicLamp()
{
};

bool MagicLamp::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nCrytsal << m_nLuckStar << m_nMagicDust << m_nMaxMainline << m_nMaxTheme
        << m_nCrystalRestoreTime << m_nNextDayResetTime << m_nTodayHaveGetVitNum << m_nFavorableBuyVitCount;
    return true;
}

bool MagicLamp::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nCrytsal >> m_nLuckStar >> m_nMagicDust >> m_nMaxMainline >> m_nMaxTheme
        >> m_nCrystalRestoreTime >> m_nNextDayResetTime >> m_nTodayHaveGetVitNum >> m_nFavorableBuyVitCount;
    return true;
}

//////////////////////////////////////////////////////////////////////////

CBonus::CBonus()
    : m_nMoney(0)
    , m_nBindMCoin(0)
{
}

void CBonus::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listItems.size() );
    for (std::list<CItem>::const_iterator itr = m_listItems.begin(); 
         itr != m_listItems.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    IOBuff.AddInt(m_nMoney);
    IOBuff.AddInt(m_nBindMCoin);
}

StorageCapacityRecord::StorageCapacityRecord()
:m_nVipLevel(0)
, m_nAddCapacity(0)
{

}

StorageCapacityRecord::StorageCapacityRecord(const StorageCapacityRecord & record)
{
    m_nVipLevel = record.m_nVipLevel;
    m_nAddCapacity = record.m_nAddCapacity;
}

StorageCapacityRecord::~StorageCapacityRecord()
{

}

bool StorageCapacityRecord::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nVipLevel << m_nAddCapacity;
    return true;
}

bool StorageCapacityRecord::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nVipLevel >> m_nAddCapacity;
    return true;
}

//////////////////////////////////////////////////////////////////////////

CReward::CReward()
    : CBonus()
    , m_nRewardType(ERewardType_Normal)
{
}

void CReward::doEncode(CParamPool &IOBuff)
{
    CBonus::doEncode(IOBuff);

    IOBuff.AddUChar(m_nRewardType);
}

FriendContactInfo::FriendContactInfo()
    :m_nFriendRoleID(0)
    ,m_nContactNum(0)
    ,m_nGiveVitNum(0)
    ,m_nBeGiveVitNum(0)
    ,m_nGiveVitTime(0)
    ,m_nBeGiveVitTime(0)
{

}

void FriendContactInfo::doDecode(CParamPool &IOBuff)
{
    m_nFriendRoleID = IOBuff.GetUInt();
    m_nContactNum = IOBuff.GetUInt();
    m_nGiveVitNum = IOBuff.GetUInt();
    m_nGiveVitTime = IOBuff.GetUInt();
    m_nBeGiveVitNum = IOBuff.GetUInt();
    m_nBeGiveVitTime = IOBuff.GetUInt();
}

void FriendContactInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nFriendRoleID);
    IOBuff.AddUInt(m_nContactNum);
    IOBuff.AddUInt(m_nGiveVitNum);
    IOBuff.AddUInt(m_nGiveVitTime);
    IOBuff.AddUInt(m_nBeGiveVitNum);
    IOBuff.AddUInt(m_nBeGiveVitTime);
}

FriendContactInfo::FriendContactInfo(const FriendContactInfo & contactinfo)
{
    m_nFriendRoleID = contactinfo.m_nFriendRoleID;
    m_nContactNum = contactinfo.m_nContactNum;
    m_nGiveVitNum = contactinfo.m_nGiveVitNum;
    m_nGiveVitTime = contactinfo.m_nGiveVitTime;
    m_nBeGiveVitNum = contactinfo.m_nBeGiveVitNum;
    m_nBeGiveVitTime = contactinfo.m_nBeGiveVitTime;
}

FriendContactInfo::~FriendContactInfo()
{

}

//////////////////////////////////////////////////////////////////////////

ClothGroup::ClothGroup()
    : m_nGroupID( 0 )
{
}

ClothGroup::~ClothGroup()
{
}

bool ClothGroup::IsPosEmpty( unsigned char nItemPos ) const
{
    std::map<unsigned char, itemtype_t>::const_iterator itr = m_mapClothes.find( nItemPos );
    if ( itr == m_mapClothes.end() )
        return true;
    else
        return false;
}

void ClothGroup::AddCloth( unsigned char nItemPos, itemtype_t nItemType )
{
    if ( nItemType > 0 )
        m_mapClothes[nItemPos] = nItemType;
}

itemtype_t ClothGroup::GetClothItemType( unsigned char nItemPos ) const
{
    std::map<unsigned char, itemtype_t>::const_iterator itr = m_mapClothes.find( nItemPos );
    if ( itr == m_mapClothes.end() )
        return 0;
    else
        return itr->second;
}

RoleClothGroup::RoleClothGroup()
    : m_nVIPRequest( 0 )
{
}

RoleClothGroup::~RoleClothGroup()
{
}

bool RoleClothGroup::doEncode( CParamPool &IOBuff ) const
{
    IOBuff << m_nGroupID << (stringw &)m_strGroupName << m_nVIPRequest << m_mapClothes;
    return true;
}

bool RoleClothGroup::doDecode( CParamPool &IOBuff )
{
    IOBuff >> m_nGroupID >> (stringw &)m_strGroupName >> m_nVIPRequest >> m_mapClothes;
    return true;
}

