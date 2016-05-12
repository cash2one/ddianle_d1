#include "SystemSettingMgr.h"
#include "../activity/InvitationActivityMgr.h"
#include "../activity/MeetActivityMgr.h"
#include "../room/LiveRoomDataMgr.h"
#include "../../datastructure/Macro_Define.h"
#include "../util/Split.h"
#include "../Couple/CoupleDataStructrue.h"
#include "../attribute/guidereward.h"
#include "../mall/MallDataMgr.h"
#include "../roomLib/RoomSettingMgr.h"

extern unsigned int g_nMinContributionForDemise;        //舞团团长传位时新团长需要的最小贡献度

extern unsigned short g_nCoupleRequestLevel;

extern int g_nMinIntimacyMarriageRequired;

CSystemSettingMgr::CSystemSettingMgr()
: GameMsg_Base(0)
, m_bEnableFirstRechargeReward(true)
, m_bEnableRecharge(true)
, m_nMinContributionForDemise(500)
, m_nMinLevelCoupleRequired(10)
, m_nMinIntimacyMarriageRequired(100)
, m_nMinRoomRewardPlayerNum(2)
, m_nLobbyChatCDTime(0)
, m_nLobbyChatRequireLevel(10)
, m_nPhotoRoomStartTime(0)
, m_bInvitationIsOpen(false)
, m_bSecurityQuizOpen(false)
, m_strGuideMusic("")
, m_nDefaultMoveType(eMoveType_None)
, m_nUpgradeInstallerLevel(0)
, m_nUpdateLBSPosInterval(10)
, m_nBirthdayResetTime(0)
, m_bOpenSendGift(true)
, m_nSocialShareRewardBindMCoin(0)
, m_nDownloadMaxConnections(0)
, m_nDownloadMaxBps(0)
{
    for (unsigned short i = 1; i < EFunctionSwitch_Max; ++i)
    {
        m_mapFunctionSwitch[i] = true;
    }
}

CSystemSettingMgr::~CSystemSettingMgr()
{

}

CSystemSettingMgr & CSystemSettingMgr::Instance()
{
    static CSystemSettingMgr s_SystemSettingMgr;

    return s_SystemSettingMgr;
}

const char * CSystemSettingMgr::SerializeName()
{
    return "SystemSetting";
}

void CSystemSettingMgr::SetFirstPaymentReward(bool bEnablePayment)
{
    m_bEnableFirstRechargeReward = bEnablePayment;
}

bool CSystemSettingMgr::CanFirstPaymentReward() const
{
    return m_bEnableFirstRechargeReward;
}

void CSystemSettingMgr::SetRecharge(bool bEnableRecharge)
{
    m_bEnableRecharge = bEnableRecharge;
}

bool CSystemSettingMgr::CanRecharge() const
{
    return m_bEnableRecharge;
}

void CSystemSettingMgr::SerializeSystemSettingInfo(CParamPool & IOBuff)
{
    SerializeFunctionSwitch(IOBuff);
    IOBuff.AddBool(m_bEnableFirstRechargeReward);
    IOBuff.AddBool(m_bEnableRecharge);
    IOBuff.AddUInt(m_nMinContributionForDemise);
    IOBuff.AddUShort(m_nMinLevelCoupleRequired);
    IOBuff.AddInt(m_nMinIntimacyMarriageRequired);
    IOBuff.AddInt(PhotoRoomStartRemainTime());
    IOBuff.AddBool(CInvitationInviterConfigMgr::Instance().IsActivityOpen());
    IOBuff.AddStringW(m_strGuideMusic.c_str());
    IOBuff.AddUChar(m_nDefaultMoveType);

    SerializeLoadingADList(IOBuff);

    IOBuff.AddInt(RoomSettingMgr::Instance().m_nMinLevelLiveRoomRequired);
    IOBuff.AddUInt(CLiveRoomDataManager::Instance().m_nVoteItemID);
    SerializeLoadingBeckoningValue(IOBuff);
    IOBuff.AddUInt(CMeetActivityMgr::Instance().GetCostItemID());
    IOBuff.AddUShort(m_nUpdateLBSPosInterval);
    IOBuff.AddStringW(m_strSocialShareText);
    IOBuff.AddInt(m_nSocialShareRewardBindMCoin);
    IOBuff.AddInt(CCoupleConfigDataMgr::Instance().GetRedEnvelopeValue());

    // 引导选择服饰信息
    GuideRewardManager::Instance().SerializeSystemSettingInfo(IOBuff);

    // 体验卡每日最大体验次数
    IOBuff.AddUShort(CMallDataMgr::Instance().m_experienceCardTimesPerDay);

    // 体验卡可以购买的类型
    IOBuff.AddUShort((unsigned short)CMallDataMgr::Instance().m_vecExperienceType.size());
    for (size_t i = 0; i < CMallDataMgr::Instance().m_vecExperienceType.size(); ++i)
    {
        IOBuff.AddUChar(CMallDataMgr::Instance().m_vecExperienceType[i]);
    }
    IOBuff.AddUInt(CMallDataMgr::Instance().m_experienceCardItemType);

    IOBuff.AddInt(m_nDownloadMaxConnections);
    IOBuff.AddInt(m_nDownloadMaxBps);
}

void CSystemSettingMgr::SerializeLoadingADList(CParamPool & IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listLoadingAD.size());
    for (std::list<std::string>::iterator itr = m_listLoadingAD.begin(); itr != m_listLoadingAD.end(); ++itr)
    {
        IOBuff.AddStringW(itr->c_str());
    }
}

void CSystemSettingMgr::SerializeLoadingBeckoningValue(CParamPool & IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_mapBeckoningEffect.size());
    for (std::map<int, int>::iterator itr = m_mapBeckoningEffect.begin(); itr != m_mapBeckoningEffect.end(); ++itr)
    {
        IOBuff.AddUChar((unsigned char)itr->first);
        IOBuff.AddInt(itr->second);
    }
}

//设置舞团团长传位时新团长需要的最小贡献度
void CSystemSettingMgr::SetMinContributionForDemise(unsigned int nMinContributionForDemise)
{
    m_nMinContributionForDemise = nMinContributionForDemise;
    g_nMinContributionForDemise = m_nMinContributionForDemise;
}

void CSystemSettingMgr::SetMinLevelCoupleRequired(unsigned short nMinLevelCoupleRequired)
{
    m_nMinLevelCoupleRequired = nMinLevelCoupleRequired;
    g_nCoupleRequestLevel = nMinLevelCoupleRequired;
}

unsigned short CSystemSettingMgr::MinLevelCoupleRequired()
{
    return m_nMinLevelCoupleRequired;
}

void CSystemSettingMgr::SetMinIntimacyMarriageRequired(int nIntimacyMarriageRequired)
{
    m_nMinIntimacyMarriageRequired = nIntimacyMarriageRequired;
    g_nMinIntimacyMarriageRequired = nIntimacyMarriageRequired;
}

int CSystemSettingMgr::MinIntimacyMarriageRequired()
{
    return m_nMinIntimacyMarriageRequired;
}

//设置房间奖励需要的最小玩家数
void CSystemSettingMgr::SetMinRoomRewardPlayerNum(int nMinRoomRewardPlayerNum)
{
    m_nMinRoomRewardPlayerNum = nMinRoomRewardPlayerNum;
}

//获取房间奖励需要的最小玩家数
int CSystemSettingMgr::GetMinRoomRewardPlayerNum()
{
    return m_nMinRoomRewardPlayerNum;
}

void CSystemSettingMgr::SetLobbyChatCDTime(int nChatCDTime)
{
    m_nLobbyChatCDTime = nChatCDTime;
}

int CSystemSettingMgr::LobbyChatCDTime()
{
    return m_nLobbyChatCDTime;
}

void CSystemSettingMgr::SetLobbyChatRequirLeve(int nLobbyChatRequirLeve)
{
    m_nLobbyChatRequireLevel = nLobbyChatRequirLeve;
}

int CSystemSettingMgr::LobbyChatRequireLevel()
{
    return m_nLobbyChatRequireLevel;
}

void CSystemSettingMgr::SetPhotoRoomStartTime(int nPhotoRoomStartTime)
{
    m_nPhotoRoomStartTime = nPhotoRoomStartTime;
}

int CSystemSettingMgr::PhotoRoomStartTime()
{
    return m_nPhotoRoomStartTime;
}

int CSystemSettingMgr::PhotoRoomStartRemainTime()
{
    int nTimeRemain = (int)m_nPhotoRoomStartTime - (int)time(NULL);

    return nTimeRemain > 0 ? nTimeRemain : 0;
}

void CSystemSettingMgr::SetInvitationStatus(bool status)
{
    m_bInvitationIsOpen = status;
}

bool  CSystemSettingMgr::IsInvitationOpen()
{
    return m_bInvitationIsOpen;
}

void CSystemSettingMgr::SetGuideMusic(const std::string &music)
{
    m_strGuideMusic = music;
}

void CSystemSettingMgr::SetDefaultMoveTypeInfo(int nDefaultMoveType)
{
    m_nDefaultMoveType = (unsigned char)nDefaultMoveType;
}

unsigned char CSystemSettingMgr::DefaultMoveType() const
{
    return m_nDefaultMoveType;
}

void CSystemSettingMgr::SetUpgradeInstallerLevelInfo(int nUpgradeInstallerLevel)
{
    m_nUpgradeInstallerLevel = nUpgradeInstallerLevel;
}

int CSystemSettingMgr::UpgradeInstallerLevel() const
{
    return m_nUpgradeInstallerLevel;
}

void CSystemSettingMgr::SetSecurityQuizStatus(bool status)
{
    m_bSecurityQuizOpen = status;
}

bool  CSystemSettingMgr::IsSecurityQuizOpen()
{
    return m_bSecurityQuizOpen;
}

void CSystemSettingMgr::AddLoadingAD(const std::string &strLoadingAD)
{
    m_listLoadingAD.push_back(strLoadingAD);
}


unsigned short CSystemSettingMgr::GetUpdateLBSPosInterval() const
{
    return m_nUpdateLBSPosInterval;
}

void CSystemSettingMgr::SetUpdateLBSPosInterval(unsigned short val)
{
    m_nUpdateLBSPosInterval = val;
}

unsigned int CSystemSettingMgr::GetBirthdayResetTime() const
{
    return m_nBirthdayResetTime;
}

void CSystemSettingMgr::SetBirthdayResetTime(unsigned int nBirthdayResetTime)
{
    m_nBirthdayResetTime = nBirthdayResetTime;
}

void CSystemSettingMgr::SetOpenSendGift(bool bOpenSendGift)
{
    m_bOpenSendGift = bOpenSendGift;
}

void CSystemSettingMgr::AddBeckoningValue(int nMode, int nRank, int nValue)
{
    m_mapBeckoningValue[nMode][nRank] = nValue;
}

void CSystemSettingMgr::AddBeckoningEffect(int nLevel, int nValue)
{
    m_mapBeckoningEffect[nLevel] = nValue;
}

int CSystemSettingMgr::GetBeckoningValue(int nMode, int nRank)
{
    std::map<int, std::map<int, int> >::iterator modeIt = m_mapBeckoningValue.find(nMode);
    if (modeIt != m_mapBeckoningValue.end())
    {
        std::map<int, int>::iterator rankIt = modeIt->second.find(nRank);
        if (rankIt != modeIt->second.end())
        {
            return rankIt->second;
        }
    }
    return 0;
}

const std::map<int, int>& CSystemSettingMgr::GetBeckoningEffectMap() const
{
    return m_mapBeckoningEffect;
}

const std::map<int, std::map<int, int> >& CSystemSettingMgr::GetBeckoningValueMap() const
{
    return m_mapBeckoningValue;
}

bool CSystemSettingMgr::IsFunctionOpen(int nFunction)
{
    std::map<unsigned short, bool>::iterator it = m_mapFunctionSwitch.find((unsigned short)nFunction);
    if (it == m_mapFunctionSwitch.end())
    {
        // lua中没定义开关的话，功能默认开放
        return true;
    }
    return it->second;
}

void CSystemSettingMgr::SetFunctionSwitch(int nFunction, bool bOpen)
{
    m_mapFunctionSwitch[(unsigned short)nFunction] = bOpen;
}

void CSystemSettingMgr::SerializeFunctionSwitch(CParamPool & IOBuff)
{
    IOBuff << m_mapFunctionSwitch;
}

int CSystemSettingMgr::GetBuffIconItem(const std::vector<int>& vecKey)
{
    int v = 0;
    std::map<int, int>::iterator itCount = m_mapBuffIconParamCount.find(vecKey[0]);
    if (itCount == m_mapBuffIconParamCount.end())
    {
        return v;
    }
    std::stringstream ss;
    for (int i = 0; i < itCount->second; ++i)
    {
        ss << vecKey[i];
        if (i + 1 < itCount->second)
        {
            ss << "|";
        }
    }
    std::map<string, int>::iterator it = m_mapBuffIcon.find(ss.str());
    if (it != m_mapBuffIcon.end())
    {
        v = it->second;
    }
    return v;
}

void CSystemSettingMgr::SetBuffIconItem(const std::string& k, int v)
{
    std::vector<int> vec;
    Split(k, "|", vec);
    if (vec.size() == 0)
    {
        return;
    }
    if (m_mapBuffIconParamCount.find(vec[0]) == m_mapBuffIconParamCount.end())
    {
        m_mapBuffIconParamCount[vec[0]] = vec.size();//获得组成key的参数个数（含基因id
    }
    m_mapBuffIcon[k] = v;
}

void CSystemSettingMgr::SetBuffCanRemove(int nGeneID)
{
    m_setBuffCanRemove.insert(nGeneID);
}

bool CSystemSettingMgr::CanRemoveBuff(int nGeneID)
{
    return m_setBuffCanRemove.find(nGeneID) != m_setBuffCanRemove.end();
}

void CSystemSettingMgr::SetClothEffectPos(std::list<unsigned short> & listClothPos)
{
    m_listClothOpenEffectPos = listClothPos;
}

bool CSystemSettingMgr::IsOpenClothEffectPos(unsigned short nClothPos)
{
    std::list<unsigned short>::iterator it =
        std::find(m_listClothOpenEffectPos.begin(), m_listClothOpenEffectPos.end(), nClothPos);

    if (it != m_listClothOpenEffectPos.end())
    {
        return true;
    }

    return false;
}

void CSystemSettingMgr::SetDownloadParam(int nMaxConnections, int nMaxBps)
{
    m_nDownloadMaxConnections = nMaxConnections;
    m_nDownloadMaxBps = nMaxBps;
}

