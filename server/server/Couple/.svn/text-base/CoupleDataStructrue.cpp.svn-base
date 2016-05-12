#include "CoupleDataStructrue.h"
#include "../../socket/ParamPool.h"
#include "CoupleMacroDef.h"
#include "../../datastructure/Macro_Define.h"
#include "../item/EntityItemComponent.h"
#include "../gene/EntityGeneComponent.h"
#include "../Pet/PetComponent.h"
#include "../Pet/PetService.h"
#include "../Medal/EntityMedalComponent.h"
#include "../Horse/EntityHorseComponent.h"


CCoupleRequest::CCoupleRequest()
:m_nRoleID(0)
, m_strName("")
{

}

CCoupleRequest::~CCoupleRequest()
{

}

//////////////////////////////////////////////////////////////////////////

CCoupleRequestReplay::CCoupleRequestReplay()
: m_nPairID(0)
, m_nTargetID(0)
, m_strTargetName("")
, m_nCoupleDate(0)
, m_bAgree(false)
, m_strReason("")
, m_nSmallRoleID(0)
{
}

void CCoupleRequestReplay::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nPairID);
    IOBuff.AddUInt(m_nTargetID);
    IOBuff.AddStringW(m_strTargetName);
    IOBuff.AddUInt(m_nCoupleDate);
    IOBuff.AddBool(m_bAgree);
    IOBuff.AddStringW(m_strReason);
    IOBuff.AddUInt(m_nSmallRoleID);
}

///////////////////////////////////////////////////////
CWeddingRoomPlayerInfo::CWeddingRoomPlayerInfo()
: m_nRoleID(0)
, m_strRoleName("")
, m_eRoleSex(ESexType_No)
, m_nRoleSkin(1)
, m_strDanceGroupName("")
, m_nGroupTitle(-1)
, m_nDanceGroupBadge(0)
, m_nDanceGroupEffect(0)
, m_bIsHost(false)
, m_eRoleType(EWeddingRoomRoleType_None)
, m_nIsVip(0)
, m_nVipLv(0)
, m_nTransFormID(0)
, m_pRoleItem(NULL)
, m_pRoleGene(NULL)
, m_pRolePet(NULL)
, m_pRoleMedal(NULL)
, m_pRoleHorse(NULL)
{
}

CWeddingRoomPlayerInfo::~CWeddingRoomPlayerInfo()
{
}

void CWeddingRoomPlayerInfo::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName.c_str());
    IOBuff.AddUChar(m_eRoleSex);
    IOBuff.AddUChar(m_nRoleSkin);
    IOBuff.AddStringW(m_strDanceGroupName.c_str());
    IOBuff.AddChar(m_nGroupTitle);
    IOBuff.AddUShort(m_nDanceGroupBadge);
    IOBuff.AddUShort(m_nDanceGroupEffect);
    IOBuff.AddBool(m_bIsHost);
    IOBuff.AddUChar(m_eRoleType);
    IOBuff.AddBool(m_nIsVip);
    IOBuff.AddUShort(m_nVipLv);
    m_playerPosition.doEncode(IOBuff);
    IOBuff.AddUShort(m_nTransFormID);

    if (m_pRoleItem != NULL)
    {
        m_pRoleItem->PackItemForOther(IOBuff);
    }

    if (m_pRoleGene != NULL)
    {
        m_pRoleGene->PackGeneForOther(IOBuff);
    }

    if (m_pRolePet != NULL)
    {
        Pet *pet = m_pRolePet->GetCarriedPet();
        IOBuff.AddBool(pet != NULL ? true : false);
        if (pet != NULL)
        {
            PlayerPet petInfo;
            PetService::Instance().EncodePetBrief(*pet, petInfo);
            petInfo.doEncode(IOBuff);
        }
    }

    if (m_pRoleMedal != NULL)
    {
        m_pRoleMedal->SyncAllDisplayMedalsForOther(IOBuff);
    }

    if (m_pRoleHorse != NULL)
    {
        m_pRoleHorse->SyncAllDisplayHorse2Other(IOBuff);
    }
}

///////////////////////////////////////////////////////
CWeddingRoomBriefInfo::CWeddingRoomBriefInfo()
:m_nRoomID(0)
, m_strRoomName("")
, m_bHasPassword(false)
, m_nWeddingType(EWeddingType_None)
, m_nGroomID(0)
, m_strGroomName("")
, m_nBrideID(0)
, m_strBrideName("")
{
    memset(m_arrPlayerSex, 0, C_WEDDINGROOM_GEUST_COUNT);
}

CWeddingRoomBriefInfo::~CWeddingRoomBriefInfo()
{

}

void CWeddingRoomBriefInfo::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUInt(m_nRoomID);
    IOBuff.AddStringW(m_strRoomName.c_str());
    IOBuff.AddBool(m_bHasPassword);
    IOBuff.AddUChar(m_nWeddingType);
    for (int i = 0; i < C_WEDDINGROOM_GEUST_COUNT; ++i) {
        IOBuff.AddUChar(m_arrPlayerSex[i]);
    }
    IOBuff.AddUInt(m_nGroomID);
    IOBuff.AddStringW(m_strGroomName.c_str());
    IOBuff.AddUInt(m_nBrideID);
    IOBuff.AddStringW(m_strBrideName.c_str());
}

///////////////////////////////////////////////////////
CWeddingRoomWholeInfo::CWeddingRoomWholeInfo()
:m_nRoomID(0)
, m_strRoomName("")
, m_nPairID(0)
, m_nGroomID(0)
, m_nBrideID(0)
, m_bHasPwd(false)
, m_strRoomPwd("")
, m_bIsHost(false)
, m_nWeddingType(EWeddingType_None)
, m_nWeddingTimeRemaind(0)
{

}

CWeddingRoomWholeInfo::~CWeddingRoomWholeInfo()
{

}

void CWeddingRoomWholeInfo::doEncode(CParamPool & IOBufff)
{
    IOBufff.AddUInt(m_nRoomID);
    IOBufff.AddUInt(m_nPairID);
    IOBufff.AddStringW(m_strRoomName.c_str());
    IOBufff.AddUInt(m_nGroomID);
    IOBufff.AddUInt(m_nBrideID);
    IOBufff.AddBool(m_bHasPwd);
    IOBufff.AddStringW(m_strRoomPwd.c_str());
    IOBufff.AddBool(m_bIsHost);
    IOBufff.AddUChar(m_nWeddingType);
    IOBufff.AddUInt(m_nWeddingTimeRemaind);

    IOBufff.AddUShort((unsigned short)m_listPlayers.size());
    for (std::list<CWeddingRoomPlayerInfo>::iterator itr = m_listPlayers.begin(); itr != m_listPlayers.end(); ++itr) {
        itr->doEncode(IOBufff);
    }
}

CCoupleConfigDataMgr::CCoupleConfigDataMgr()
:m_nRedEnvelopeBlessing(0)
, m_nRedEnvelopeValue(0)
, m_nBlessingFeedBackItemID(0)
, m_nRedEnvelopeNoticeCount(0)
{

}

void CCoupleConfigDataMgr::AddCoupleConfig(int nBlessing, int nValue, unsigned int nBlessingFeedBackItem, const std::map<unsigned int, itemtype_t> &boxid, unsigned int nRedEnvelopeCount)
{
    m_nRedEnvelopeBlessing = nBlessing;
    m_nRedEnvelopeValue = nValue;
    m_nBlessingFeedBackItemID = nBlessingFeedBackItem;
    m_nRedEnvelopeNoticeCount = nRedEnvelopeCount;

    m_CoupleRedEnvelopeGift = boxid;
}

unsigned int  CCoupleConfigDataMgr::GetWeddingGiftBoxID(unsigned int nBlessingValue)
{
    std::map<unsigned int, itemtype_t>::reverse_iterator it = m_CoupleRedEnvelopeGift.rbegin();
    for (; it != m_CoupleRedEnvelopeGift.rend(); ++it)
    {
        if (it->first <= nBlessingValue)
        {
            return it->second;
        }
    }

    return 0;
}

void CDivorceConfigDataMgr::AddDivorceConfig(const CItem & divorceitem, const CItem & compulsoryitem, int nRemarryIntimacyRate, const std::string & strDesc)
{
    m_DivorceItem = divorceitem;
    m_CompulsoryItem = compulsoryitem;
    m_nRemarryIntimacyRate = nRemarryIntimacyRate;
    m_strDivorceDesc = strDesc;
}

void CDivorceConfigDataMgr::AddDivorceMusicConfig(const std::string & strMusicName)
{
    m_MusicList.push_back(strMusicName);
}

void CDivorceConfigDataMgr::AddDivorceGiftConfig(unsigned int nMaleGiftID, unsigned int nFemaleGiftID)
{
    m_MaleGiftList.push_back((unsigned short)nMaleGiftID);
    m_FemaleGiftList.push_back((unsigned short)nFemaleGiftID);
}

