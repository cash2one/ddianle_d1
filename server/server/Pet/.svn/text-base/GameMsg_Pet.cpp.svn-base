#include "GameMsg_Pet.h"


//////////////////////////////////////////////////////////////////////////

PetMsgInfo::PetMsgInfo()
    : m_nPetID(0)
    , m_nTypeID(0)
    , m_strDefaultName("")
    , m_strName("")
    , m_nPotentialID(0)
    , m_nAttributeType(0)
    , m_nLevel(0)
    , m_nMaxLevel(0)
    , m_nExp(0)
    , m_nMaxExp(0)
    , m_nEndurance(0)
    , m_nMaxEndurance(0)
    , m_fRecoveryTime(0)
    , m_bCanImproveQuality(false)
    , m_nQualityType(0)
    , m_bCanEvolution(false)
    , m_nEvolutionTimes(0)
    , m_nResetSkillCostType(0)
    , m_nResetSkillCostAmount(0)
    , m_nDanceSkillCount(0)
    , m_nFightSkillCount(0)
{
}

PetMsgInfo::~PetMsgInfo()
{
}

void PetMsgInfo::doEncode(CParamPool &IOBuff) const
{
    IOBuff.AddUInt(m_nPetID);
    IOBuff.AddUShort(m_nTypeID);
    IOBuff.AddStringW(m_strDefaultName);
    IOBuff.AddStringW(m_strName);
    IOBuff.AddUChar(m_nPotentialID);
    IOBuff.AddUChar(m_nAttributeType);
    IOBuff.AddUShort(m_nLevel);
    IOBuff.AddUShort(m_nMaxLevel);
    IOBuff.AddUInt(m_nExp);
    IOBuff.AddUInt(m_nMaxExp);
    IOBuff.AddUShort(m_nEndurance);
    IOBuff.AddUShort(m_nMaxEndurance);
    IOBuff.AddFloat(m_fRecoveryTime);
    IOBuff.AddBool(m_bCanImproveQuality);
    IOBuff.AddUChar(m_nQualityType);
    IOBuff.AddBool(m_bCanEvolution);
    IOBuff.AddUChar(m_nEvolutionTimes);
    IOBuff.AddUChar(m_nResetSkillCostType);
    IOBuff.AddUInt(m_nResetSkillCostAmount);
    IOBuff.AddInt(m_nDanceSkillCount);
    IOBuff.AddInt(m_nFightSkillCount);
    IOBuff << m_listDanceSkill;
    IOBuff << m_listFightSkill;
}

//////////////////////////////////////////////////////////////////////////

PetCommonMsgInfo::PetCommonMsgInfo()
    : m_nPetCount(0)
    , m_nGridCount(0)
    , m_nMaxGridCount(0)
    , m_nMaxUnlockCountOnce(0)
    , m_nUnlockCostType(0)
    , m_nUnlockCostAmount(0)
    , m_nChangeNameCostType(0)
    , m_nChangeNameCostAmount(0)
    , m_strPetSkillTip("PetSkillTip")
    , m_bCarriedPet(false)
{
}

PetCommonMsgInfo::~PetCommonMsgInfo()
{
}

void PetCommonMsgInfo::doEncode(CParamPool &IOBuff) const
{
    IOBuff.AddInt(m_nPetCount);
    IOBuff.AddInt(m_nGridCount);
    IOBuff.AddInt(m_nMaxGridCount);
    IOBuff.AddInt(m_nMaxUnlockCountOnce);
    IOBuff.AddUChar(m_nUnlockCostType);
    IOBuff.AddUInt(m_nUnlockCostAmount);
    IOBuff.AddUChar(m_nChangeNameCostType);
    IOBuff.AddUInt(m_nChangeNameCostAmount);
    IOBuff.AddStringW(m_strPetSkillTip);

    std::list<unsigned int>::const_iterator itrItem;

    IOBuff.AddUShort((unsigned short)m_listFeedExpItems.size() );
    for (itrItem = m_listFeedExpItems.begin(); itrItem != m_listFeedExpItems.end(); ++itrItem)
    {
        IOBuff.AddUInt(*itrItem);
    }

    IOBuff.AddUShort((unsigned short)m_listFeedEnduranceItems.size() );
    for (itrItem = m_listFeedEnduranceItems.begin(); itrItem != m_listFeedEnduranceItems.end(); ++itrItem)
    {
        IOBuff.AddUInt(*itrItem);
    }

    IOBuff.AddBool(m_bCarriedPet);

    if (m_bCarriedPet)
        m_carriedPetInfo.doEncode(IOBuff);
}

PlayerPetConfig::PlayerPetConfig()
    : m_nPetType(0)
    , m_nPetAttributeType(0)
    , m_nMaxLevel(0)
    , m_nMaxQualityType(0)
    , m_nMaxEvolutionTimes(0)
    , m_nMaxEndurance(0)
{
}

PlayerPetConfig::~PlayerPetConfig()
{
}

bool PlayerPetConfig::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_nPetType << m_nPetAttributeType << m_nMaxLevel
        << m_nMaxQualityType << m_nMaxEvolutionTimes << m_nMaxEndurance;
    return true;
}
//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetEnterIncubationRoom::GameMsg_C2S_PetEnterIncubationRoom()
    : GameMsg_Base(MSG_C2S_Pet_EnterIncubationRoom)
{
}

GameMsg_C2S_PetEnterIncubationRoom::~GameMsg_C2S_PetEnterIncubationRoom()
{
}

bool GameMsg_C2S_PetEnterIncubationRoom::doDecode(CParamPool &IOBuff)
{
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetEnterIncubationRoomRes::GameMsg_S2C_PetEnterIncubationRoomRes()
    : GameMsg_Base(MSG_S2C_Pet_EnterIncubationRoomRes)
    , m_nErrorCode(ePetEnterIncubationRoomRes_Success)
    , m_nIncubatorLevel(1)
    , m_bHasEgg(false)
    , m_nEggTypeID(0)
    , m_nLastTime(0)
{
}

GameMsg_S2C_PetEnterIncubationRoomRes::~GameMsg_S2C_PetEnterIncubationRoomRes()
{
}

bool GameMsg_S2C_PetEnterIncubationRoomRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);
    IOBuff.AddUShort(m_nIncubatorLevel);
    IOBuff.AddBool(m_bHasEgg);
    IOBuff.AddUInt(m_nEggTypeID);
    IOBuff.AddUInt(m_nLastTime);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetIncubatorLevelUp::GameMsg_C2S_PetIncubatorLevelUp()
    : GameMsg_Base(MSG_C2S_Pet_IncubatorLevelUp)
{
}

GameMsg_C2S_PetIncubatorLevelUp::~GameMsg_C2S_PetIncubatorLevelUp()
{
}

bool GameMsg_C2S_PetIncubatorLevelUp::doDecode(CParamPool &IOBuff)
{
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetIncubatorLevelUpRes::GameMsg_S2C_PetIncubatorLevelUpRes()
    : GameMsg_Base(MSG_S2C_Pet_IncubatorLevelUpRes)
    , m_nErrorCode(ePetIncubatorLevelUpRes_Success)
{
}

GameMsg_S2C_PetIncubatorLevelUpRes::~GameMsg_S2C_PetIncubatorLevelUpRes()
{
}

bool GameMsg_S2C_PetIncubatorLevelUpRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetStartIncubation::GameMsg_C2S_PetStartIncubation()
    : GameMsg_Base(MSG_C2S_Pet_StartIncubation)
    , m_nIndex(0)
{
}

GameMsg_C2S_PetStartIncubation::~GameMsg_C2S_PetStartIncubation()
{
}

bool GameMsg_C2S_PetStartIncubation::doDecode(CParamPool &IOBuff)
{
    m_nIndex = IOBuff.GetUShort();

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetStartIncubationRes::GameMsg_S2C_PetStartIncubationRes()
    : GameMsg_Base(MSG_S2C_Pet_StartIncubationRes)
    , m_nErrorCode(ePetStartIncubationRes_Success)
{
}

GameMsg_S2C_PetStartIncubationRes::~GameMsg_S2C_PetStartIncubationRes()
{
}

bool GameMsg_S2C_PetStartIncubationRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetIncubationRequest::GameMsg_C2S_PetIncubationRequest()
    : GameMsg_Base(MSG_C2S_Pet_IncubationRequest)
{
}

GameMsg_C2S_PetIncubationRequest::~GameMsg_C2S_PetIncubationRequest()
{
}

bool GameMsg_C2S_PetIncubationRequest::doDecode(CParamPool &IOBuff)
{
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetIncubationRequestRes::GameMsg_S2C_PetIncubationRequestRes()
    : GameMsg_Base(MSG_S2C_Pet_IncubationRequestRes)
    , m_bFinished(false)
    , m_nLastTime(0)
{
}

GameMsg_S2C_PetIncubationRequestRes::~GameMsg_S2C_PetIncubationRequestRes()
{
}

bool GameMsg_S2C_PetIncubationRequestRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bFinished);
    IOBuff.AddUInt(m_nLastTime);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetIncubationSpeedUp::GameMsg_C2S_PetIncubationSpeedUp()
    : GameMsg_Base(MSG_C2S_Pet_SpeedUpIncubation)
{
}

GameMsg_C2S_PetIncubationSpeedUp::~GameMsg_C2S_PetIncubationSpeedUp()
{
}

bool GameMsg_C2S_PetIncubationSpeedUp::doDecode(CParamPool &IOBuff)
{
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetIncubationSpeedUpRes::GameMsg_S2C_PetIncubationSpeedUpRes()
    : GameMsg_Base(MSG_S2C_Pet_SpeedUpIncubationRes)
    , m_nErrorCode(ePetIncubationSpeedUpRes_Success)
{
}

GameMsg_S2C_PetIncubationSpeedUpRes::~GameMsg_S2C_PetIncubationSpeedUpRes()
{
}

bool GameMsg_S2C_PetIncubationSpeedUpRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetOpenEgg::GameMsg_C2S_PetOpenEgg()
    : GameMsg_Base(MSG_C2S_Pet_OpenEgg)
{
}

GameMsg_C2S_PetOpenEgg::~GameMsg_C2S_PetOpenEgg()
{
}

bool GameMsg_C2S_PetOpenEgg::doDecode(CParamPool &IOBuff)
{
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetOpenEggRes::GameMsg_S2C_PetOpenEggRes()
    : GameMsg_Base(MSG_S2C_Pet_OpenEggRes)
    , m_nErrorCode(ePetOpenEggRes_Success)
    , m_nPetTypeID(0)
    , m_strPetDefaultName("")
{
}

GameMsg_S2C_PetOpenEggRes::~GameMsg_S2C_PetOpenEggRes()
{
}

bool GameMsg_S2C_PetOpenEggRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);
    IOBuff.AddUShort(m_nPetTypeID);
    IOBuff.AddStringW(m_strPetDefaultName);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetSyncCommonInfo::GameMsg_S2C_PetSyncCommonInfo()
    : GameMsg_Base(MSG_S2C_Pet_SyncCommonInfos)
{
}

GameMsg_S2C_PetSyncCommonInfo::~GameMsg_S2C_PetSyncCommonInfo()
{
}

bool GameMsg_S2C_PetSyncCommonInfo::doEncode(CParamPool &IOBuff)
{
    m_commonInfo.doEncode(IOBuff);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetRequestList::GameMsg_C2S_PetRequestList()
    : GameMsg_Base(MSG_C2S_Pet_RequestList)
    , m_nCurPage(0)
    , m_nCount(0)
{
}

GameMsg_C2S_PetRequestList::~GameMsg_C2S_PetRequestList()
{
}

bool GameMsg_C2S_PetRequestList::doDecode(CParamPool &IOBuff)
{
    m_nCurPage = IOBuff.GetInt();
    m_nCount = IOBuff.GetInt();

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetRequestListRes::GameMsg_S2C_PetRequestListRes()
    : GameMsg_Base(MSG_S2C_Pet_RequestListRes)
    , m_nCurPage(0)
    , m_nPetCount(0)
    , m_nGridCount(0)
    , m_nEmptyCount(0)
    , m_bCanUnlock(false)
{
}

GameMsg_S2C_PetRequestListRes::~GameMsg_S2C_PetRequestListRes()
{
}

bool GameMsg_S2C_PetRequestListRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nCurPage);
    IOBuff.AddInt(m_nPetCount);
    IOBuff.AddInt(m_nGridCount);
    IOBuff.AddInt(m_nEmptyCount);

    IOBuff.AddUShort((unsigned short)m_listPetInfo.size() );

    for (std::list<PetMsgInfo>::const_iterator itr = m_listPetInfo.begin(); 
        itr != m_listPetInfo.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    IOBuff.AddBool(m_bCanUnlock);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetUnlockGrid::GameMsg_C2S_PetUnlockGrid()
    : GameMsg_Base(MSG_C2S_Pet_UnlockGrid)
    , m_nCount(0)
{
}

GameMsg_C2S_PetUnlockGrid::~GameMsg_C2S_PetUnlockGrid()
{
}

bool GameMsg_C2S_PetUnlockGrid::doDecode(CParamPool &IOBuff)
{
    m_nCount = IOBuff.GetInt();

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetUnlockGridRes::GameMsg_S2C_PetUnlockGridRes()
    : GameMsg_Base(MSG_S2C_Pet_UnlockGridRes)
    , m_nErrorCode(ePetUnlockGridRes_Success)
    , m_nGridCount(0)
{
}

GameMsg_S2C_PetUnlockGridRes::~GameMsg_S2C_PetUnlockGridRes()
{
}

bool GameMsg_S2C_PetUnlockGridRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);
    IOBuff.AddInt(m_nGridCount);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetChangeName::GameMsg_C2S_PetChangeName()
    : GameMsg_Base(MSG_C2S_Pet_ChangeName)
    , m_nPetID(0)
    , m_strNewName("")
{
}

GameMsg_C2S_PetChangeName::~GameMsg_C2S_PetChangeName()
{
}

bool GameMsg_C2S_PetChangeName::doDecode(CParamPool &IOBuff)
{
    m_nPetID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strNewName);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetChangeNameRes::GameMsg_S2C_PetChangeNameRes()
    : GameMsg_Base(MSG_S2C_Pet_ChangeNameRes)
    , m_nErrorCode(ePetChangeNameRes_Success)
    , m_nPetID(0)
    , m_strNewName("")
{
}

GameMsg_S2C_PetChangeNameRes::~GameMsg_S2C_PetChangeNameRes()
{
}

bool GameMsg_S2C_PetChangeNameRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);
    IOBuff.AddUInt(m_nPetID);
    IOBuff.AddStringW(m_strNewName);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetChangeCarried::GameMsg_C2S_PetChangeCarried()
    : GameMsg_Base(MSG_C2S_Pet_ChangeCarried)
    , m_nPetID(0)
    , m_nSceneID(0)
{
}

GameMsg_C2S_PetChangeCarried::~GameMsg_C2S_PetChangeCarried()
{
}

bool GameMsg_C2S_PetChangeCarried::doDecode(CParamPool &IOBuff)
{
    m_nPetID = IOBuff.GetUInt();
    m_nSceneID = IOBuff.GetUChar();

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetChangeCarriedRes::GameMsg_S2C_PetChangeCarriedRes()
    : GameMsg_Base(MSG_S2C_Pet_ChangeCarriedRes)
    , m_nErrorCode(ePetChangeCarriedRes_Success)
    , m_nSceneID(0)
{
}

GameMsg_S2C_PetChangeCarriedRes::~GameMsg_S2C_PetChangeCarriedRes()
{
}

bool GameMsg_S2C_PetChangeCarriedRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);
    IOBuff.AddUChar(m_nSceneID);
    m_petInfos.doEncode(IOBuff);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetUnloadCarried::GameMsg_C2S_PetUnloadCarried()
    : GameMsg_Base(MSG_C2S_Pet_UnloadCarried)
    , m_nSceneID(0)
{
}

GameMsg_C2S_PetUnloadCarried::~GameMsg_C2S_PetUnloadCarried()
{
}

bool GameMsg_C2S_PetUnloadCarried::doDecode(CParamPool &IOBuff)
{
    m_nSceneID = IOBuff.GetUChar();
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetUnloadCarriedRes::GameMsg_S2C_PetUnloadCarriedRes()
    : GameMsg_Base(MSG_S2C_Pet_UnloadCarriedRes)
    , m_nErrorCode(ePetUnloadCarriedRes_Success)
    , m_nSceneID(0)
{
}

GameMsg_S2C_PetUnloadCarriedRes::~GameMsg_S2C_PetUnloadCarriedRes()
{
}

bool GameMsg_S2C_PetUnloadCarriedRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);
    IOBuff.AddUChar(m_nSceneID);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetRequestImproveCondition::GameMsg_C2S_PetRequestImproveCondition()
    : GameMsg_Base(MSG_C2S_Pet_RequestImproveCondition)
    , m_nPetID(0)
{
}

GameMsg_C2S_PetRequestImproveCondition::~GameMsg_C2S_PetRequestImproveCondition()
{
}

bool GameMsg_C2S_PetRequestImproveCondition::doDecode(CParamPool &IOBuff)
{
    m_nPetID = IOBuff.GetUInt();

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetRequestImproveConditionRes::GameMsg_S2C_PetRequestImproveConditionRes()
    : GameMsg_Base(MSG_S2C_Pet_RequestImproveConditionRes)
    , m_nPetID(0)
    , m_nCostType(0)
    , m_nCostAmount(0)
{
}

GameMsg_S2C_PetRequestImproveConditionRes::~GameMsg_S2C_PetRequestImproveConditionRes()
{
}

bool GameMsg_S2C_PetRequestImproveConditionRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nPetID);
    IOBuff.AddUInt(m_itemImprove.m_nItemType);
    IOBuff.AddInt((int)m_itemImprove.m_nItemCount);
    IOBuff.AddUChar(m_nCostType);
    IOBuff.AddUInt(m_nCostAmount);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetImprove::GameMsg_C2S_PetImprove()
    : GameMsg_Base(MSG_C2S_Pet_Improve)
    , m_nPetID(0)
{
}

GameMsg_C2S_PetImprove::~GameMsg_C2S_PetImprove()
{
}

bool GameMsg_C2S_PetImprove::doDecode(CParamPool &IOBuff)
{
    m_nPetID = IOBuff.GetUInt();

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetImproveRes::GameMsg_S2C_PetImproveRes()
    : GameMsg_Base(MSG_S2C_Pet_ImproveRes)
    , m_nErrorCode(ePetImproveRes_Success)
    , m_nPetID(0)
    , m_nNextCostType(0)
    , m_nNextCostAmount(0)
{
}

GameMsg_S2C_PetImproveRes::~GameMsg_S2C_PetImproveRes()
{
}

bool GameMsg_S2C_PetImproveRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);
    IOBuff.AddUInt(m_nPetID);

    if (m_nErrorCode == ePetImproveRes_Success)
    {
        IOBuff.AddUInt(m_nextItemImprove.m_nItemType);
        IOBuff.AddInt((int)m_nextItemImprove.m_nItemCount);
        IOBuff.AddUChar(m_nNextCostType);
        IOBuff.AddUInt(m_nNextCostAmount);
        m_petInfo.doEncode(IOBuff);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetRequestEvolutionCondition::GameMsg_C2S_PetRequestEvolutionCondition()
    : GameMsg_Base(MSG_C2S_Pet_RequestEvolutionCondition)
    , m_nPetID(0)
{
}

GameMsg_C2S_PetRequestEvolutionCondition::~GameMsg_C2S_PetRequestEvolutionCondition()
{
}

bool GameMsg_C2S_PetRequestEvolutionCondition::doDecode(CParamPool &IOBuff)
{
    m_nPetID = IOBuff.GetUInt();

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetRequestEvolutionConditionRes::GameMsg_S2C_PetRequestEvolutionConditionRes()
    : GameMsg_Base(MSG_S2C_Pet_RequestEvolutionConditionRes)
    , m_nPetID(0)
    , m_nCostType(0)
    , m_nCostAmount(0)
    , m_nBaseRate(0)
{
}

GameMsg_S2C_PetRequestEvolutionConditionRes::~GameMsg_S2C_PetRequestEvolutionConditionRes()
{
}

bool GameMsg_S2C_PetRequestEvolutionConditionRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nPetID);
    IOBuff.AddUInt(m_itemEvolution.m_nItemType);
    IOBuff.AddInt((int)m_itemEvolution.m_nItemCount);
    IOBuff.AddUInt(m_itemExtra.m_nItemType);
    IOBuff.AddInt((int)m_itemExtra.m_nItemCount);
    IOBuff.AddUChar(m_nCostType);
    IOBuff.AddUInt(m_nCostAmount);
    IOBuff.AddInt(m_nBaseRate);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetEvolution::GameMsg_C2S_PetEvolution()
    : GameMsg_Base(MSG_C2S_Pet_Evolution)
    , m_nPetID(0)
    , m_nExtraItemCount(0)
{
}

GameMsg_C2S_PetEvolution::~GameMsg_C2S_PetEvolution()
{
}

bool GameMsg_C2S_PetEvolution::doDecode(CParamPool &IOBuff)
{
    m_nPetID = IOBuff.GetUInt();
    m_nExtraItemCount = IOBuff.GetInt();

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetEvolutionRes::GameMsg_S2C_PetEvolutionRes()
    : GameMsg_Base(MSG_S2C_Pet_EvolutionRes)
    , m_nErrorCode(ePetEvolutionRes_Success)
    , m_nPetID(0)
    , m_nNextCostType(0)
    , m_nNextCostAmount(0)
    , m_nNextBaseRate(0)
{
}

GameMsg_S2C_PetEvolutionRes::~GameMsg_S2C_PetEvolutionRes()
{
}

bool GameMsg_S2C_PetEvolutionRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);
    IOBuff.AddUInt(m_nPetID);

    if (m_nErrorCode == ePetEvolutionRes_Success)
    {
        IOBuff.AddUInt(m_nextItemEvolution.m_nItemType);
        IOBuff.AddInt((int)m_nextItemEvolution.m_nItemCount);
        IOBuff.AddUInt(m_nextItemExtra.m_nItemType);
        IOBuff.AddInt((int)m_nextItemExtra.m_nItemCount);
        IOBuff.AddUChar(m_nNextCostType);
        IOBuff.AddUInt(m_nNextCostAmount);
        IOBuff.AddInt(m_nNextBaseRate);
        m_petInfos.doEncode(IOBuff);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetFeeding::GameMsg_C2S_PetFeeding()
    : GameMsg_Base(MSG_C2S_Pet_Feeding)
    , m_nPetID(0)
    , m_nItemType(0)
    , m_nItemCount(0)
{
}

GameMsg_C2S_PetFeeding::~GameMsg_C2S_PetFeeding()
{
}

bool GameMsg_C2S_PetFeeding::doDecode(CParamPool &IOBuff)
{
    m_nPetID = IOBuff.GetUInt();
    m_nItemType = IOBuff.GetUInt();
    m_nItemCount = IOBuff.GetInt();

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetFeedingRes::GameMsg_S2C_PetFeedingRes()
    : GameMsg_Base(MSG_S2C_Pet_FeedingRes)
    , m_nErrorCode(ePetFeedingRes_Success)
    , m_nPetID(0)
{
}

GameMsg_S2C_PetFeedingRes::~GameMsg_S2C_PetFeedingRes()
{
}

bool GameMsg_S2C_PetFeedingRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);
    IOBuff.AddUInt(m_nPetID);

    if (m_nErrorCode == ePetFeedingRes_Success)
    {
        m_petInfo.doEncode(IOBuff);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_RequestTrainingSkill::GameMsg_C2S_RequestTrainingSkill()
    : GameMsg_Base(MSG_C2S_Pet_TrainingSkill)
    , m_nPetID(0)
    , m_nSkillID(0)
{
}

GameMsg_C2S_RequestTrainingSkill::~GameMsg_C2S_RequestTrainingSkill()
{
}


bool GameMsg_C2S_RequestTrainingSkill::doDecode(CParamPool &IOBuff)
{
    m_nPetID = IOBuff.GetUInt();
    m_nSkillID = (SkillID)IOBuff.GetUShort();
    return true;
}


GameMsg_S2C_RequestTrainingSkillRes::GameMsg_S2C_RequestTrainingSkillRes()
    : GameMsg_Base(MSG_S2C_Pet_TrainingSkillRes)
    , m_nErrorCode(0)
    , m_nPetID(0)
    , m_nSkillID(0)
{
}

GameMsg_S2C_RequestTrainingSkillRes::~GameMsg_S2C_RequestTrainingSkillRes()
{
}

bool GameMsg_S2C_RequestTrainingSkillRes::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_nErrorCode << m_nPetID << (unsigned short)m_nSkillID << m_PetSkill;
    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_RequestResetSkills::GameMsg_C2S_RequestResetSkills()
    : GameMsg_Base(MSG_C2S_Pet_ResetSkills)
    , m_nPetID(0)
{
}

GameMsg_C2S_RequestResetSkills::~GameMsg_C2S_RequestResetSkills()
{
}

bool GameMsg_C2S_RequestResetSkills::doDecode(CParamPool &IOBuff)
{
    IOBuff >> m_nPetID;
    int nCount = (int)IOBuff.GetUShort();
    for (int i = 0; i < nCount; i++)
    {
        SkillID nSkillID = (SkillID)IOBuff.GetUShort();
        m_setLockSkillID.insert(nSkillID);
    }
    return true;
}


GameMsg_S2C_RequestResetSkillsRes::GameMsg_S2C_RequestResetSkillsRes()
    : GameMsg_Base(MSG_S2C_Pet_ResetSkillsRes)
    , m_nErrorCode(0)
    , m_nPetID(0)
    , m_nResetSkillCostType(0)
    , m_nResetSkillCostAmount(0)
    , m_nDanceSkillCount(0)
    , m_nFightSkillCount(0)
{
}

GameMsg_S2C_RequestResetSkillsRes::~GameMsg_S2C_RequestResetSkillsRes()
{
}

bool GameMsg_S2C_RequestResetSkillsRes::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_nErrorCode << m_nPetID << m_nResetSkillCostType << m_nResetSkillCostAmount
        << m_nDanceSkillCount << m_nFightSkillCount;
    IOBuff << m_listDanceSkill << m_listFightSkill;
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetGetExchangeList::GameMsg_C2S_PetGetExchangeList()
    : GameMsg_Base(MSG_C2S_Pet_GetExchangeList)
    , m_nCurPage(0)
    , m_nCount(0)
{
}

GameMsg_C2S_PetGetExchangeList::~GameMsg_C2S_PetGetExchangeList()
{
}

bool GameMsg_C2S_PetGetExchangeList::doDecode(CParamPool &IOBuff)
{
    m_nCurPage = IOBuff.GetUShort();
    m_nCount = IOBuff.GetUShort();

    return true;
}

//////////////////////////////////////////////////////////////////////////

PetExchangeMsgInfo::PetExchangeMsgInfo()
    : m_nPetID(0)
    , m_nTypeID(0)
    , m_strName("")
    , m_nAttributeType(0)
    , m_nLevel(0)
    , m_nQualityType(0)
    , m_nEvolutionTimes(0)
    , m_nPotentialID(0)
{
}

PetExchangeMsgInfo::~PetExchangeMsgInfo()
{
}

void PetExchangeMsgInfo::doEncode(CParamPool &IOBuff) const
{
    IOBuff.AddUInt(m_nPetID);
    IOBuff.AddUShort(m_nTypeID);
    IOBuff.AddStringW(m_strName);
    IOBuff.AddUChar(m_nAttributeType);
    IOBuff.AddUShort(m_nLevel);
    IOBuff.AddUChar(m_nQualityType);
    IOBuff.AddUChar(m_nEvolutionTimes);
    IOBuff.AddUChar(m_nPotentialID);

    std::list<PetMsgSkill>::const_iterator itr;

    IOBuff.AddUShort( (unsigned short)m_listDanceSkill.size() );
    for (itr = m_listDanceSkill.begin(); itr != m_listDanceSkill.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    IOBuff.AddUShort( (unsigned short)m_listFightSkill.size() );
    for (itr = m_listFightSkill.begin(); itr != m_listFightSkill.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    IOBuff.AddUShort((unsigned short)m_listExchangeItems.size() );
    for (std::list<CItem>::const_iterator itr = m_listExchangeItems.begin(); 
        itr != m_listExchangeItems.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }
}

GameMsg_S2C_PetGetExchangeListRes::GameMsg_S2C_PetGetExchangeListRes()
    : GameMsg_Base(MSG_S2C_Pet_GetExchangeListRes)
    , m_nCurPage(0)
    , m_nPetCount(0)
{
}

GameMsg_S2C_PetGetExchangeListRes::~GameMsg_S2C_PetGetExchangeListRes()
{
}

bool GameMsg_S2C_PetGetExchangeListRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_nCurPage);
    IOBuff.AddUShort(m_nPetCount);

    IOBuff.AddUShort((unsigned short)m_listExchangeInfos.size() );
    for (std::list<PetExchangeMsgInfo>::const_iterator itr = m_listExchangeInfos.begin(); 
        itr != m_listExchangeInfos.end(); ++itr)
    {
        itr->doEncode(IOBuff);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_C2S_PetExchange::GameMsg_C2S_PetExchange()
    : GameMsg_Base(MSG_C2S_Pet_Exchange)
{
}

GameMsg_C2S_PetExchange::~GameMsg_C2S_PetExchange()
{
}

bool GameMsg_C2S_PetExchange::doDecode(CParamPool &IOBuff)
{
    unsigned int nPetID = 0;
    unsigned short nCount = 0;
    nCount = IOBuff.GetUShort();

    for (unsigned short i = 0; i < nCount; ++i)
    {
        nPetID = IOBuff.GetUInt();

        m_listPetID.push_back(nPetID);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_PetExchangeRes::GameMsg_S2C_PetExchangeRes()
    : GameMsg_Base(MSG_S2C_Pet_ExchangeRes)
    , m_nErrorCode(ePetExchangeRes_Success)
{
}

GameMsg_S2C_PetExchangeRes::~GameMsg_S2C_PetExchangeRes()
{
}

bool GameMsg_S2C_PetExchangeRes::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUChar(m_nErrorCode);

    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetPetLibraryTypeList::GameMsg_C2S_GetPetLibraryTypeList()
    : GameMsg_Base(MSG_C2S_Pet_GetPetLibraryTypeList)
    , m_nRoleID(0)
{
}

GameMsg_C2S_GetPetLibraryTypeList::~GameMsg_C2S_GetPetLibraryTypeList()
{
}

bool GameMsg_C2S_GetPetLibraryTypeList::doDecode(CParamPool &IOBuff)
{
    IOBuff >> m_nRoleID;
    return true;
}

PetLibraryMsg::PetLibraryMsg()
    : m_nPetTypeID(0)
    , m_nAttributeType(0)
    , m_nQualityType(0)
{
}

PetLibraryMsg::~PetLibraryMsg()
{
}

bool PetLibraryMsg::doEncode(CParamPool &IOBuff) const
{
    IOBuff << m_nPetTypeID << m_nAttributeType << m_nQualityType;
    return true;
}

GameMsg_S2C_GetPetLibraryTypeListRes::GameMsg_S2C_GetPetLibraryTypeListRes()
    : GameMsg_Base(MSG_S2C_Pet_GetPetLibraryTypeListRes)
    , m_nRoleID(0)
{
}

GameMsg_S2C_GetPetLibraryTypeListRes::~GameMsg_S2C_GetPetLibraryTypeListRes()
{
}

bool GameMsg_S2C_GetPetLibraryTypeListRes::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_nRoleID << m_listPetType;
    return true;
}

GameMsg_C2S_GetPetLibraryList::GameMsg_C2S_GetPetLibraryList()
    : GameMsg_Base(MSG_C2S_Pet_GetPetLibraryList)
    , m_nRoleID(0)
{
}

GameMsg_C2S_GetPetLibraryList::~GameMsg_C2S_GetPetLibraryList()
{
}

bool GameMsg_C2S_GetPetLibraryList::doDecode(CParamPool &IOBuff)
{
    IOBuff >> m_nRoleID >> m_listPetType;
    return true;
}

GameMsg_S2C_GetPetLibraryListRes::GameMsg_S2C_GetPetLibraryListRes()
    : GameMsg_Base(MSG_S2C_Pet_GetPetLibraryListRes)
    , m_nRoleID(0)
{
}

GameMsg_S2C_GetPetLibraryListRes::~GameMsg_S2C_GetPetLibraryListRes()
{
}

bool GameMsg_S2C_GetPetLibraryListRes::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_nRoleID << (unsigned short)m_listPet.size();

    for (std::list<PetMsgInfo>::const_iterator it = m_listPet.begin();
        it != m_listPet.end(); ++it)
    {
        (*it).doEncode(IOBuff);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_SyncPetChangedInfoToOthers::GameMsg_S2C_SyncPetChangedInfoToOthers()
    : GameMsg_Base(MSG_S2C_Pet_SyncPetChangedInfoToOthers)
    , m_nRoleID(0)
    , m_bCarriedPet(false)
    , m_nSceneID(0)
{
}

GameMsg_S2C_SyncPetChangedInfoToOthers::~GameMsg_S2C_SyncPetChangedInfoToOthers()
{
}

bool GameMsg_S2C_SyncPetChangedInfoToOthers::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_nRoleID << m_bCarriedPet << m_nSceneID;
    if (m_bCarriedPet)
    {
        m_pet.doEncode(IOBuff);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_SyncPetChangeInfoToSelf::GameMsg_S2C_SyncPetChangeInfoToSelf()
    : GameMsg_Base( MSG_S2C_Pet_SyncPetChangedInfoToSelf )
{
}

GameMsg_S2C_SyncPetChangeInfoToSelf::~GameMsg_S2C_SyncPetChangeInfoToSelf()
{
}

bool GameMsg_S2C_SyncPetChangeInfoToSelf::doEncode( CParamPool &IOBuff )
{
    m_pet.doEncode( IOBuff );

    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_ReportPetDanceEmotion::GameMsg_C2S_ReportPetDanceEmotion()
    : GameMsg_Base(MSG_C2S_Pet_ReportPetDanceEmotion)
    , m_nEmotionType(0)
{
}

GameMsg_C2S_ReportPetDanceEmotion::~GameMsg_C2S_ReportPetDanceEmotion()
{
}

bool GameMsg_C2S_ReportPetDanceEmotion::doDecode(CParamPool &IOBuff)
{
    IOBuff >> m_nEmotionType;
    return true;
}

GameMsg_S2C_SyncPetDanceEmotion::GameMsg_S2C_SyncPetDanceEmotion()
    : GameMsg_Base(MSG_S2C_Pet_SyncPetDanceEmotion)
    , m_nRoleID(0)
    , m_nEmotionType(0)
{
}

GameMsg_S2C_SyncPetDanceEmotion::~GameMsg_S2C_SyncPetDanceEmotion()
{
}

bool GameMsg_S2C_SyncPetDanceEmotion::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_nRoleID << m_nEmotionType;
    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_CastPetDanceSkill::GameMsg_S2C_CastPetDanceSkill()
    : GameMsg_Base(MSG_S2C_Pet_CastPetDanceSkill)
    , m_nRoleID(0)
    , m_nPetID(0)
{
}

GameMsg_S2C_CastPetDanceSkill::~GameMsg_S2C_CastPetDanceSkill()
{
}

bool GameMsg_S2C_CastPetDanceSkill::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_nRoleID << m_nPetID << m_listSkillList;
    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_PetGuideRequest::GameMsg_C2S_PetGuideRequest()
    : GameMsg_Base(MSG_C2S_Pet_GuideRequest)
    , m_nGuideStep(0)
{
}
GameMsg_C2S_PetGuideRequest::~GameMsg_C2S_PetGuideRequest()
{
}

bool GameMsg_C2S_PetGuideRequest::doDecode(CParamPool &IOBuff)
{
    IOBuff >> m_nGuideStep;
    return true;
}

GameMsg_S2C_PetGuideResponse::GameMsg_S2C_PetGuideResponse()
    : GameMsg_Base(MSG_S2C_Pet_GuideResponse)
    , m_nPetEggType(0)
{
}

GameMsg_S2C_PetGuideResponse::~GameMsg_S2C_PetGuideResponse()
{
}

bool GameMsg_S2C_PetGuideResponse::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_nPetEggType;
    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RefreshPetSkillList::GameMsg_S2C_RefreshPetSkillList()
    : GameMsg_Base(MSG_S2C_Pet_RefreshPetSkillList)
{
}

GameMsg_S2C_RefreshPetSkillList::~GameMsg_S2C_RefreshPetSkillList()
{
}

bool GameMsg_S2C_RefreshPetSkillList::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_petSkill;
    return true;
}

