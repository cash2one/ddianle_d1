#include "ActivityDataStruct.h"
#include "../../datastructure/DataStruct_Base.h"

CRoleXmasFreeRewardInfo::CRoleXmasFreeRewardInfo()
:m_nLastGetRewardTime(0)
{
	m_vecFreeReward.assign(XMAS_FREEREWARD_LEVEL, 0);
}

CRoleXmasFreeRewardInfo::~CRoleXmasFreeRewardInfo()
{

}

void CRoleXmasFreeRewardInfo::Reset()
{
	m_nLastGetRewardTime = 0;
	m_vecFreeReward.assign(XMAS_FREEREWARD_LEVEL, 0);
}


//-------------------------------------------------------CRoleFestivalWishActivityInfo
CRoleFestivalWishActivityInfo::CRoleFestivalWishActivityInfo()
:m_nWishedCount(0)
,m_nLastGetRewardTime(0)
{

}

CRoleFestivalWishActivityInfo::~CRoleFestivalWishActivityInfo()
{

}

void CRoleFestivalWishActivityInfo::Reset()
{
	m_nWishedCount = 0;	
	m_nLastGetRewardTime = 0;
}

//-----------------------------------------------------CRoleBlessActivityInfo
CRoleBlessActivityInfo::CRoleBlessActivityInfo()
:m_nBeBlessedCount(0)
,m_bHasSentInfoToClient(false)
{
	m_vecRewards.assign(BLESS_REWARD_LEVEL, 0);
}

CRoleBlessActivityInfo::~CRoleBlessActivityInfo()
{

}

//-----------------------------------------------CFestivalBlessWords
CFestivalBlessWords::CFestivalBlessWords()
:m_nIndex(0)
,m_strBlessWords("")
{

}

CFestivalBlessWords::~CFestivalBlessWords()
{

}

void CFestivalBlessWords::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar((unsigned char)m_nIndex);
	IOBuff.AddStringW(m_strBlessWords.c_str());
}

//-----------------------------------------------CFestivalSendBlessReward
CFestivalSendBlessReward::CFestivalSendBlessReward()
:m_nMoney(0)
{

}

CFestivalSendBlessReward::~CFestivalSendBlessReward()
{

}

//-----------------------------------------------CFestivalBlessedReward
CFestivalBlessedReward::CFestivalBlessedReward()
:m_nLevel(0)
,m_nBeBlessedCount(0)
,m_nMoney(0)
{

}

CFestivalBlessedReward::~CFestivalBlessedReward()
{

}

//-----------------------------------------------CRolePuzzleActivityInfo
CRolePuzzleActivityInfo::CRolePuzzleActivityInfo()
:m_nOwnerID(0)
,m_nPuzzleID(0)
,m_nUpdateTime(0)
,m_nFinishTime(0)
{
	memset( m_arFragments, '0', PUZZLE_FRAGMENTS_COUNT );
	m_arFragments[PUZZLE_FRAGMENTS_COUNT] = '\0';
}

CRolePuzzleActivityInfo::~CRolePuzzleActivityInfo()
{

}


// 新玩家招募活动对应的奖励
CInvitationAcceptorReward::CInvitationAcceptorReward():
m_nMoney(0)
{
}
CInvitationAcceptorReward::~CInvitationAcceptorReward()
{
}


bool CInvitationAcceptorReward::doEncode(CParamPool& buff) const
{
	buff.AddUShort((unsigned short)m_listItem.size());
	for (list<CItem>::const_iterator itr = m_listItem.begin(); 
		itr != m_listItem.end(); ++itr)
	{
		itr->doEncode(buff);
	}

	buff.AddInt(m_nMoney);

	return true;
}



CInvitationInviterReward::CInvitationInviterReward():
m_nLevel(0), m_nCumulation(0), m_nMoney(0)
{
}
CInvitationInviterReward::~CInvitationInviterReward()
{
}


bool CInvitationInviterReward::doEncode(CParamPool& buff) const
{
	buff.AddUChar((unsigned char)m_nLevel);
	buff.AddUInt(m_nCumulation);

	buff.AddUShort((unsigned short)m_listItem.size());
	for (list<CItem>::const_iterator itr = m_listItem.begin(); 
		itr != m_listItem.end(); ++itr)
	{
		itr->doEncode(buff);
	}

	buff.AddInt(m_nMoney);

	return true;
}


CInvitationActivityInfo::CInvitationActivityInfo()
:m_nActivityID(EActivity_None)
,m_bExhibit(false)
,m_nWeight(0)
,m_strRegularImageName("")
,m_strThumbnailImageName("")
,m_nActivityBeginTime(0)
,m_nActivityEndTime(0)
,m_strActivityName("")
,m_strActivityContent("")
{
}

CInvitationActivityInfo::~CInvitationActivityInfo()
{
}

void CInvitationActivityInfo::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUChar(m_nActivityID);
	if (m_nActivityID == EActivity_Invitation)
	{
		//IOBuff.AddBool(m_bIsOpen);
		IOBuff.AddBool(m_bExhibit);
		IOBuff.AddInt(m_nWeight);
		IOBuff.AddStringW(m_strRegularImageName.c_str());
		IOBuff.AddStringW(m_strThumbnailImageName.c_str());
		IOBuff.AddStringW(m_strActivityName.c_str());
		IOBuff.AddStringW(m_strActivityContent.c_str());
		m_reward.doEncode(IOBuff);
	}
}
CFresherReward::CFresherReward()
    :m_nDay(0)
    ,m_nMoney(0)
    ,m_nBindCoin(0)
    ,m_nVipMoney(0)
    ,m_nVipBindCoin(0)
    ,m_nRewardStatus(RewardStatus_Invalid)
{
}
CFresherReward::~CFresherReward()
{
}

void CFresherReward::doEncode(CParamPool &buff)
{
    buff.AddUChar((unsigned char)m_nDay);
    buff.AddUChar(m_nRewardStatus);

    buff.AddUShort(2);
    buff.AddUShort((unsigned short)m_listItem.size());
    for (list<CItem>::const_iterator itr = m_listItem.begin(); 
        itr != m_listItem.end(); ++itr)
    {
        itr->doEncode(buff);
    }
    buff.AddInt(m_nMoney);
    buff.AddInt(m_nBindCoin);
    buff.AddUChar(0);

    buff.AddUShort( (unsigned short)m_listVipItem.size() );
    for (list<CItem>::const_iterator itr = m_listVipItem.begin(); 
        itr != m_listVipItem.end(); ++itr)
    {
        itr->doEncode(buff);
    }
    buff.AddInt(m_nVipMoney);
    buff.AddInt(m_nVipBindCoin);
    buff.AddUChar(1);
}

CFresherActivityInfo::CFresherActivityInfo()
: m_bIsOpen(false)
, m_nDay(0)
, m_strActivityTitle("")
, m_strActivityContent("")
, m_nRecvAgainMoney(0)
{
}
CFresherActivityInfo::~CFresherActivityInfo()
{
}

void CFresherActivityInfo::doEncode(CParamPool &buff)
{
	buff.AddBool(m_bIsOpen);
	if (m_bIsOpen)
	{
		buff.AddUChar(m_nDay);
		buff.AddStringW(m_strActivityTitle.c_str());
		buff.AddStringW(m_strActivityContent.c_str());
		buff.AddInt(m_nRecvAgainMoney);

		buff.AddUShort((unsigned short)m_vecReward.size());
		for (size_t i = 0; i < m_vecReward.size(); ++i)
		{
			m_vecReward[i].doEncode(buff);
		}
	}
}
///////////////////////////////////////
CPlatofromPrivilegeReward::CPlatofromPrivilegeReward()
:nVipIndex( 0 )
,nLowLevel( 0 )
,nHighLevel( 0 )
{

}

void CPlatofromPrivilegeReward::doEncode(CParamPool & buff)
{
	//buff.AddUChar( ( unsigned char )nVipIndex );
	buff.AddUShort( ( unsigned short ) listItem.size() ) ;

	list<CItem>::iterator iter = listItem.begin(), iterEnd = listItem.end(); //2005不支持auto操作符
	for( ; iter != iterEnd; ++iter )
	{
		iter->doEncode( buff );
	}
	//buff.AddUChar(m_nRewardStatus);
}

bool CMallShareActivityData::doDecode( CParamPool &IOBuff )
{
	m_nActivityID = IOBuff.GetUChar();
	if (m_nActivityID > EActivity_None)
	{
		m_bExhibit = IOBuff.GetBool();
		m_nWeight = IOBuff.GetInt();
		IOBuff.GetStringW(m_strTitle);
		IOBuff.GetStringW(m_strContent);
		IOBuff.GetStringW(m_strBigImageName);
		IOBuff.GetStringW(m_strSmallImageName);
		IOBuff.GetStringW(m_strActionName);
		IOBuff.GetStringW(m_strShareTextMale);
		IOBuff.GetStringW(m_strShareTextFemale);
	}
	return true;
}

bool CMallShareActivityData::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar(m_nActivityID);
	if (m_nActivityID > EActivity_None)
	{
		IOBuff.AddBool(m_bExhibit);
		IOBuff.AddInt(m_nWeight);
		IOBuff.AddStringW(m_strTitle);
		IOBuff.AddStringW(m_strContent);
		IOBuff.AddStringW(m_strBigImageName);
		IOBuff.AddStringW(m_strSmallImageName);
		IOBuff.AddStringW(m_strActionName);
		IOBuff.AddStringW(m_strShareTextMale);
		IOBuff.AddStringW(m_strShareTextFemale);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////

CRewardTitleInfo::CRewardTitleInfo()
    : m_nIdx(0)
{
}

bool CRewardTitleInfo::doEncode(CParamPool &IOBuff, bool )
{
    IOBuff.AddUShort(m_nIdx);
    IOBuff.AddStringW(m_strTitle);

    return true;
}

CStringRewardInfo::CStringRewardInfo()
    : CRewardTitleInfo()
{
}

bool CStringRewardInfo::doEncode(CParamPool &IOBuff, bool bFemale /* = false */)
{
    CRewardTitleInfo::doEncode(IOBuff);

    if (bFemale)
        IOBuff.AddStringW(m_strFemaleRewardText);
    else
        IOBuff.AddStringW(m_strMaleRewardText);

    return true;
}

//////////////////////////////////////////////////////////////////////////

CItemRewardInfo::CItemRewardInfo()
    : CRewardTitleInfo()
{
}

bool CItemRewardInfo::doEncode(CParamPool &IOBuff, bool bFemale /* = false */)
{
    CRewardTitleInfo::doEncode(IOBuff);

    std::list<CReward> &rRewards = m_listMaleRewards;
    if (bFemale)
        rRewards = m_listFemaleRewards;

    IOBuff.AddUShort((unsigned short)rRewards.size() );
    for (std::list<CReward>::iterator itr = rRewards.begin(); 
         itr != rRewards.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

CSocialShareActivityData::CSocialShareActivityData()
{
    m_nActivityType = EActivity_None;
    m_bExhibit = 0;
    m_nWeight = 0;
    m_nBeginTime = 0;
    m_nEndTime = 0;
    m_bIsReward = false;
    m_bIsOpen = false;
    m_bIsShow = false;
}

CSocialShareActivityData::~CSocialShareActivityData()
{
    for (list<CRewardTitleInfo *>::iterator bItr = m_listReward.begin(), eItr = m_listReward.end(); 
         bItr != eItr; ++bItr)
    {
        delete (*bItr);
    }

    m_listReward.clear();
}

bool CSocialShareActivityData::doEncode(CParamPool &IOBuff, bool bFemale /* = false */)
{
    IOBuff.AddUChar(m_nActivityType);
    IOBuff.AddBool(m_bExhibit);
    IOBuff.AddInt(m_nWeight);
    IOBuff.AddUInt(m_nBeginTime);
    IOBuff.AddUInt(m_nEndTime);
    IOBuff.AddStringW(m_strTitle);
    IOBuff.AddStringW(m_strContent);
    IOBuff.AddStringW(m_strBigImageName);
    IOBuff.AddStringW(m_strSmallImageName);

    if (bFemale)
        IOBuff.AddStringW(m_strFemaleShareText);
    else
        IOBuff.AddStringW(m_strMaleShareText);

    IOBuff.AddUShort((unsigned short)m_listReward.size());
    for (list<CRewardTitleInfo *>::const_iterator bItr = m_listReward.begin(), eItr = m_listReward.end(); 
         bItr != eItr; ++bItr)
    {
        (*bItr)->doEncode(IOBuff, bFemale);
    }

    IOBuff.AddBool(m_bIsReward);
    IOBuff.AddBool(m_bIsOpen);
    IOBuff.AddBool(m_bIsShow);

    return true;
}

//////////////////////////////////////////////////////////////////////////

bool CMallSocialShareActivityData::doEncode(CParamPool &IOBuff, bool bFemale /* = false */)
{
    CSocialShareActivityData::doEncode(IOBuff, bFemale);

    if (bFemale)
        IOBuff.AddStringW(m_strFemaleActionName);
    else
        IOBuff.AddStringW(m_strMaleActionName);

    IOBuff.AddUShort((unsigned short)m_setClothPos.size());
    for (std::set<int>::const_iterator bItr = m_setClothPos.begin(), eIt = m_setClothPos.end(); 
         bItr != eIt; ++bItr)
    {
        IOBuff.AddInt(*bItr);
    }

    return true;
}

void CMallSocialShareActivityData::CopyForEncode(const CMallSocialShareActivityData *pData)
{
    if (NULL == pData)
        return;

    m_nActivityType = pData->m_nActivityType;
    m_bExhibit = pData->m_bExhibit;
    m_nWeight = pData->m_nWeight;
    m_nBeginTime = pData->m_nBeginTime;
    m_nEndTime = pData->m_nEndTime;
    m_strTitle = pData->m_strTitle;
    m_strContent = pData->m_strContent;
    m_strBigImageName = pData->m_strBigImageName;
    m_strSmallImageName = pData->m_strSmallImageName;
    m_bIsReward = false;
    m_bIsOpen = pData->m_bIsOpen;
    m_bIsShow = pData->m_bIsShow;
    m_strMaleShareText = pData->m_strMaleShareText;
    m_strFemaleShareText = pData->m_strFemaleShareText;
    m_strMaleActionName = pData->m_strMaleActionName;
    m_strFemaleActionName = pData->m_strFemaleActionName;
    m_setClothPos = pData->m_setClothPos;

    const CStringRewardInfo *pStringInfo = NULL;

    for (std::list<CRewardTitleInfo *>::const_iterator bItr = pData->m_listReward.begin(), eItr = pData->m_listReward.end(); 
         bItr != eItr; ++bItr)
    {
        pStringInfo = dynamic_cast<const CStringRewardInfo *>(*bItr);

        if (NULL != pStringInfo)
        {
            CStringRewardInfo *pInfo = new CStringRewardInfo(*pStringInfo);

            m_listReward.push_back(pInfo);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void CPhotoSocialShareActivityData::CopyForEncode(const CPhotoSocialShareActivityData *pData)
{
    if (NULL == pData)
        return;

    m_nActivityType = pData->m_nActivityType;
    m_bExhibit = pData->m_bExhibit;
    m_nWeight = pData->m_nWeight;
    m_nBeginTime = pData->m_nBeginTime;
    m_nEndTime = pData->m_nEndTime;
    m_strTitle = pData->m_strTitle;
    m_strContent = pData->m_strContent;
    m_strBigImageName = pData->m_strBigImageName;
    m_strSmallImageName = pData->m_strSmallImageName;
    m_bIsReward = false;
    m_bIsOpen = pData->m_bIsOpen;
    m_bIsShow = pData->m_bIsShow;
    m_strMaleShareText = pData->m_strMaleShareText;
    m_strFemaleShareText = pData->m_strFemaleShareText;

    const CItemRewardInfo *pItemRewardInfo = NULL;

    for (std::list<CRewardTitleInfo *>::const_iterator bItr = pData->m_listReward.begin(), eItr = pData->m_listReward.end(); 
        bItr != eItr; ++bItr)
    {
        pItemRewardInfo = dynamic_cast<const CItemRewardInfo *>(*bItr);

        if (NULL != pItemRewardInfo)
        {
            CItemRewardInfo *pInfo = new CItemRewardInfo(*pItemRewardInfo);

            m_listReward.push_back(pInfo);
        }
    }
}

LongactingCumulativeRechargeConfig::LongactingCumulativeRechargeConfig()
    :m_nRewardIndex(0)
    ,m_nNeedMCoin(0)
    ,m_nMoney(0)
    ,m_nBindMCoin(0)
{

}

LongactingCumulativeRechargeConfig::~LongactingCumulativeRechargeConfig()
{

};

void LongactingCumulativeRechargeConfig::doEncode(CParamPool &IOBuff, char cSex) const
{
    IOBuff.AddUInt(m_nRewardIndex);
    IOBuff.AddUInt(m_nNeedMCoin);

    if (cSex == ESexType_Male)
    {
        IOBuff.AddUShort((unsigned short)m_listMaleItemReward.size());
        std::list<CItem>::const_iterator it = m_listMaleItemReward.begin();
        for ( ; it != m_listMaleItemReward.end(); ++it)
        {
            it->doEncode(IOBuff);
        }
    }
    else
    {
        IOBuff.AddUShort((unsigned short)m_listFemaleItemReward.size());
        std::list<CItem>::const_iterator it = m_listFemaleItemReward.begin();
        for ( ; it != m_listFemaleItemReward.end(); ++it)
        {
            it->doEncode(IOBuff);
        }
    }

    IOBuff.AddUInt(m_nMoney);
    IOBuff.AddUInt(m_nBindMCoin);
}

LongactingRechargeRole::LongactingRechargeRole()
    :m_nSex(0)
    ,m_strRoleName("")
    ,m_nTotalMPoint(0)
{

}

LongactingRechargeRole::~LongactingRechargeRole()
{

}



