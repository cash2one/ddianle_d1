#include "PetComponent.h"

#include "Pet.h"
#include "PetEgg.h"
#include "PetService.h"
#include "PetEggService.h"

#include "PetEggConfig/RandomPetCollect.h"

#include "PetConfig/PetBaseInfo.h"
#include "PetConfig/PetQualityInfo.h"
#include "PetConfig/PetEvolutionInfo.h"
#include "PetConfig/PetConfigManager.h"

#include "../item/ItemProcess.h"

#include "../chat/ChatManager.h"
#include "../mall/CurrencyType.h"
#include "../Macros/Macros.h"

#include "../util/CommonFunc.h"
#include "../../datastructure/Localization.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/Formatter.h"

#include "../item/EntityItemComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../mall/EntityMallComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../Dungeon/EntityDungeonComponent.h"
#include "../room/RoomComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../room/Room.h"
#include "../Couple/WeddingRoom.h"
#include "../PhotoRoom/PhotoRoom.h"
#include "../AmuseRoom/AmuseRoom.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../CeremonyRoom/ceremonyRoom.h"


typedef std::map<unsigned int, Pet *, std::greater<unsigned int> >  PetMap;


PetComponent::PetComponent(void)
    : CEntityComponent(true, true)
    , m_pNetComponent(NULL)
    , m_pAttrComponent(NULL)
    , m_pMallComponent(NULL)
    , m_pItemComponent(NULL)
    , m_pRoomComponent(NULL)
    , m_pAmuseRoomComponent(NULL)
    , m_pDungeonComponent(NULL)
    , m_pPhotoRoomComponent(NULL)
    , m_pCoupleComponent(NULL)
    , m_pRoleDanceGroup(NULL)
    , m_bOpen(false)
    , m_pEgg(NULL)
    , m_nCarriedPetID(0)
    , m_nExtendPetGridCount(0)
    , m_nPetEggUpdateTime(0)
    , m_nPetEnduranceUpdateTime(0)
    , m_bEggLoadCompleted(false)
    , m_nIncubatorLevel(1)
    , m_nGuideStep(0)
    , m_nSceneID(0)
{
    m_bOpen = CSystemSettingMgr::Instance().IsFunctionOpen( EFunctionSwitch_Pet );
}

PetComponent::~PetComponent(void)
{
    SAFE_DELETE(m_pEgg);

    PetMap::iterator itr;
    for (itr = m_mapPet.begin(); itr != m_mapPet.end(); ++itr)
    {
        SAFE_DELETE(itr->second);
        itr->second = NULL;
    }
    m_mapPet.clear();

    for (itr = m_mapDestroyedPet.begin(); itr != m_mapDestroyedPet.end(); ++itr)
    {
        SAFE_DELETE(itr->second);
        itr->second = NULL;
    }
    m_mapDestroyedPet.clear();
}

void PetComponent::Start()
{
    m_pNetComponent     = GetComponent<CRoleNet>();
    m_pAttrComponent    = GetComponent<CRoleAttribute>();
    m_pMallComponent    = GetComponent<CRoleMall>();
    m_pItemComponent    = GetComponent<CRoleItem>();
    m_pRoomComponent    = GetComponent<CRoleRoom>();
    m_pAmuseRoomComponent = GetComponent<CRoleAmuseRoom>();
    m_pDungeonComponent = GetComponent<CRoleDungeon>();
    m_pPhotoRoomComponent = GetComponent<CRolePhotoRoom>();
    m_pCoupleComponent = GetComponent<CRoleCouple>();
    m_pRoleDanceGroup = GetComponent<CRoleDanceGroup>();
}

void PetComponent::OnLogin()
{
    if ( m_bOpen )
        LoadPetIncubatorFromDB();
}

void PetComponent::OnUpdate(const unsigned long &lTimeElapsed)
{
    if ( !m_bOpen )
        return;

    static const int kPetEggUpdateInterval = 1000;
    static const int kPetEnduranceRecoveryInterval = 1000;

    m_nPetEggUpdateTime += (int)lTimeElapsed;
    m_nPetEnduranceUpdateTime += (int)lTimeElapsed;
    
    if (m_nPetEggUpdateTime >= kPetEggUpdateInterval)
    {
        if (m_pEgg != NULL && m_pEgg->IsIncubating())
            m_pEgg->OnUpdate((unsigned int)time(NULL));
        m_nPetEggUpdateTime -= kPetEggUpdateInterval;
    }

    if (m_nPetEnduranceUpdateTime >= kPetEnduranceRecoveryInterval)
    {
        RecoveryEndurance();
        m_nPetEnduranceUpdateTime -= kPetEnduranceRecoveryInterval;
    }
}

bool PetComponent::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
    LoadPetFromDB(pRoleInforDB->m_mapPet);

    SetGuideStep(pRoleInforDB->m_nPetGuideStep);

    RecoveryEndurance();

    return true;
}

bool PetComponent::PacketToCache(ROLEINFO_DB *pRoleInfoCache) const
{
    PetDB db;
    std::map<unsigned int, Pet *, std::greater<unsigned int> >::const_iterator citer;
    for (citer = m_mapDestroyedPet.begin(); citer != m_mapDestroyedPet.end(); ++citer)
    {
        citer->second->ToPetDB(db);
        pRoleInfoCache->m_mapPet.insert(std::make_pair(citer->first, db));
    }

    for (citer = m_mapPet.begin(); citer != m_mapPet.end(); ++citer)
    {
        citer->second->ToPetDB(db);
        pRoleInfoCache->m_mapPet.insert(std::make_pair(citer->first, db));
    }

    return true;
}

void PetComponent::LoadPetFromDB(const std::map<unsigned int, PetDB, std::greater<unsigned int> > &mapPetDB)
{
    std::map<unsigned int, PetDB, std::greater<unsigned int> >::const_iterator it;
    for (it = mapPetDB.begin(); it != mapPetDB.end(); ++it)
    {
        Pet *pPet = Pet::Create();
        pPet->Init(it->second); 

        if (pPet != NULL)
        {
            if (pPet->IsResting())
            {
                m_mapPet[it->first] = pPet;
            }
            else if (pPet->IsDestroyed())
            {
                m_mapDestroyedPet[it->first] = pPet;
            }
            else if (pPet->IsCarrying())
            {
                m_mapPet[it->first] = pPet;
                if (GetCarriedPetID() == 0)
                {
                    SetCarriedPetID(pPet->GetID());
                }
                else
                {
                    WriteLog(LOGLEVEL_ERROR, "[PET] multi pet is carried[%u]", pPet->GetID());
                }
            }
        }
    }
}

void PetComponent::SerializeComponent(CParamPool &IOBuff)
{
    if ( !m_bOpen )
        return;

    Pet *pPet = GetCarriedPet();
    PetConfigManager &rConfig = PetConfigManager::Instance();

    PetCommonMsgInfo petMsgInfo;
    petMsgInfo.m_nPetCount = (int)m_mapPet.size();
    petMsgInfo.m_nGridCount = (int)GetPetGridCount();
    petMsgInfo.m_nMaxGridCount = rConfig.GetMaxGridCount();
    petMsgInfo.m_nMaxUnlockCountOnce = rConfig.GetMaxUnlockCountOnce();
    petMsgInfo.m_nUnlockCostType = rConfig.GetExtendGridCost().GetType();
    petMsgInfo.m_nUnlockCostAmount = rConfig.GetExtendGridCost().GetAmount();
    petMsgInfo.m_nChangeNameCostType = rConfig.GetChangeNameCost().GetType();
    petMsgInfo.m_nChangeNameCostAmount = rConfig.GetChangeNameCost().GetAmount();
    rConfig.GetFeedExpItems(petMsgInfo.m_listFeedExpItems);
    rConfig.GetFeedEnduranceItems(petMsgInfo.m_listFeedEnduranceItems);
    petMsgInfo.m_strPetSkillTip = rConfig.GetSkillRule();
    petMsgInfo.m_bCarriedPet = (pPet != NULL ? true : false);
    if (petMsgInfo.m_bCarriedPet)
        PetService::Instance().EncodePetMsgInfo(*pPet, petMsgInfo.m_carriedPetInfo);

    petMsgInfo.doEncode(IOBuff);

    IOBuff.AddUChar((unsigned char)m_nGuideStep);
}

void PetComponent::SetCarriedPetID(unsigned int nPetID)
{
    m_nCarriedPetID = nPetID;
}

void PetComponent::RecoveryEndurance()
{
    for (PetMap::const_iterator itr = m_mapPet.begin(); itr != m_mapPet.end(); ++itr)
    {
        Pet *pet = itr->second;
        if (pet != NULL)
        {
            RecoveryEndurance(pet);
        }
    }
}

void PetComponent::RecoveryEndurance(Pet *pPet, int nRecoveredEndurance)
{
    if (pPet == NULL)
        return;

    if (pPet->GetEndurance() == pPet->GetMaxEndurance())
        return;

    unsigned int nNow = (unsigned int)time(NULL);
    unsigned int nLastRecoveryTime = nNow;
    if (nRecoveredEndurance == 0)
    {
        unsigned int nEnduranceRecoveryTime = pPet->GetEnduranceRecoveryTime();
        unsigned int nRecoveryTime = nNow - nEnduranceRecoveryTime;
        if (nRecoveryTime <= 0)
        {
            return;
        }

        int nRecoveryUnitTime = PetConfigManager::Instance().GetEnduranceRecoveryTime();
        nRecoveredEndurance = (int)nRecoveryTime / nRecoveryUnitTime;
        if (nRecoveredEndurance <= 0)
        {
            return;
        }
        nLastRecoveryTime = nNow - nRecoveryTime%nRecoveryUnitTime;
    }

    AddEndurance(pPet, nRecoveredEndurance, nLastRecoveryTime);
}

bool PetComponent::CostEndurance(Pet *pPet, int nEndurance)
{
    if (pPet == NULL || nEndurance <= 0)
        return false;

    if (pPet->GetEndurance() < (unsigned short)nEndurance)
        return false;

    unsigned int nLastRecoveryTime = pPet->GetEnduranceRecoveryTime();
    if (pPet->GetEndurance() >= pPet->GetMaxEndurance())
    {
        nLastRecoveryTime = (unsigned int)time(NULL);
    }

    AddEndurance(pPet, -nEndurance, nLastRecoveryTime);

    return true;
}

void PetComponent::OnNotify(IParam &param)
{
    if (param.ExistParam("PetEggFinished") )
        SaveEggState();
}

bool PetComponent::HasPet() const
{
    return (!m_mapPet.empty() || !m_mapDestroyedPet.empty());
}

bool PetComponent::HasPet(unsigned int nPetID) const
{
    PetMap::const_iterator itr = m_mapPet.find(nPetID);

    if (itr != m_mapPet.end() && itr->second != NULL)
        return true;

    return false;
}

unsigned short PetComponent::GetPetGridCount() const
{
    unsigned short nPetGridCount = PetConfigManager::Instance().GetFreeGridCount() + m_nExtendPetGridCount;

    if (nPetGridCount < (unsigned short)m_mapPet.size() )
        nPetGridCount = (unsigned short)m_mapPet.size();

    return nPetGridCount;
}

bool PetComponent::IsPetGridFull() const
{
    unsigned short nPetGridCount = GetPetGridCount();

    if ((unsigned short)m_mapPet.size() < nPetGridCount)
        return false;

    return true;
}

void PetComponent::RegComponentNetMsgMap()
{
    if ( !m_bOpen )
        return;

    RegMsgCreatorAndProcessor(GameMsg_C2S_PetEnterIncubationRoom, &PetComponent::OnEnterIncubationRoom);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetIncubatorLevelUp, &PetComponent::OnIncubatorLevelUp);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetStartIncubation, &PetComponent::OnStartIncubation);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetIncubationRequest, &PetComponent::OnIncubationRequest);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetIncubationSpeedUp, &PetComponent::OnIncubationSpeedUp);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetOpenEgg, &PetComponent::OnOpenEgg);

    RegMsgCreatorAndProcessor(GameMsg_C2S_PetRequestList, &PetComponent::OnRequestList);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetUnlockGrid, &PetComponent::OnUnlockGrid);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetChangeName, &PetComponent::OnChangeName);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetChangeCarried, &PetComponent::OnChangeCarried);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetUnloadCarried, &PetComponent::OnUnloadCarried);

    RegMsgCreatorAndProcessor(GameMsg_C2S_PetRequestImproveCondition, &PetComponent::OnRequestImproveCondition);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetImprove, &PetComponent::OnImprove);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetRequestEvolutionCondition, &PetComponent::OnRequestEvolutionCondition);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetEvolution, &PetComponent::OnEvolution);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetFeeding, &PetComponent::OnFeeding);

    RegMsgCreatorAndProcessor(GameMsg_C2S_RequestTrainingSkill, &PetComponent::OnTrainSkill);
    RegMsgCreatorAndProcessor(GameMsg_C2S_RequestResetSkills, &PetComponent::OnResetSkill);

    RegMsgCreatorAndProcessor(GameMsg_C2S_PetGetExchangeList, &PetComponent::OnGetExchangeList);
    RegMsgCreatorAndProcessor(GameMsg_C2S_PetExchange, &PetComponent::OnExchange);
    RegMsgCreatorAndProcessor(GameMsg_C2S_GetPetLibraryTypeList, &PetComponent::OnGetPetLibraryTypeList);
    RegMsgCreatorAndProcessor(GameMsg_C2S_GetPetLibraryList, &PetComponent::OnGetPetLibraryList);
    RegMsgCreatorAndProcessor(GameMsg_C2S_ReportPetDanceEmotion, &PetComponent::OnReportPetDanceEmotion);

    RegMsgCreatorAndProcessor(GameMsg_C2S_PetGuideRequest, &PetComponent::OnPetGuideStep);

    RegQueryProcessor(QUERY_PetEgg_LoadIncubator, &PetComponent::OnLoadIncubatorCallback);
    RegQueryProcessor(QUERY_Pet_LoadExtendGridCount, &PetComponent::OnLoadExtendCountCallback);
    RegQueryProcessor(QUERY_PetEgg_Load, &PetComponent::OnLoadPetEggCallback);
}

unsigned int PetComponent::GetRoleID() const
{
    if (m_pAttrComponent != NULL)
        return m_pAttrComponent->GetRoleID();

    return 0;
}

void PetComponent::SendPlayerMsg(GameMsg_Base *pMsg)
{
    if (m_pNetComponent == NULL || pMsg == NULL)
        return;

    m_pNetComponent->SendPlayerMsg(pMsg);
}

bool PetComponent::PayForIncubatorLevelUp(std::map<unsigned char, unsigned int> &rMapBill)
{
    return Pay(rMapBill, 
        EChangeBillCause_Del_PetIncubatorLevelUp, 
        EChangeBindBillCause_Del_PetIncubatorLevelUp, 
        EChangeMoneyCause_Del_PetIncubatorLevelUp);
}

bool PetComponent::PayForStartIncubation(std::map<unsigned char, unsigned int> &rMapBill)
{
    return Pay(rMapBill, 
        EChangeBillCause_Del_PetStartIncubation, 
        EChangeBindBillCause_Del_PetStartIncubation, 
        EChangeMoneyCause_Del_PetStartIncubation);
}

bool PetComponent::PayForIncubationSpeedUp(std::map<unsigned char, unsigned int> &rMapBill)
{
    return Pay(rMapBill, 
        EChangeBillCause_Del_PetIncubationSpeedUp, 
        EChangeBindBillCause_Del_PetIncubationSpeedUp, 
        EChangeMoneyCause_Del_PetIncubationSpeedUp);
}

bool PetComponent::PayForChangeName(std::map<unsigned char, unsigned int> &rMapBill)
{
    return Pay(rMapBill, 
        EChangeBillCause_Del_PetChangeName, 
        EChangeBindBillCause_Del_PetChangeName, 
        EChangeMoneyCause_Del_PetChangeName);
}

bool PetComponent::PayForUnlockGrid(std::map<unsigned char, unsigned int> &rMapBill)
{
    return Pay(rMapBill, 
        EChangeBillCause_Del_PetUnlockGrid, 
        EChangeBindBillCause_Del_PetUnlockGrid, 
        EChangeMoneyCause_Del_PetUnlockGrid);
}

bool PetComponent::PayForImproveQuality(std::map<unsigned char, unsigned int> &rMapBill, const CItem &rItem)
{
    if (m_pItemComponent == NULL)
        return false;
    else if (!CanUseItems(rItem) )
        return false;

    bool bSuccess = Pay(rMapBill, 
        EChangeBillCause_Del_PetImproveQuality, 
        EChangeBindBillCause_Del_PetImproveQuality, 
        EChangeMoneyCause_Del_PetImproveQuality);

    if (!bSuccess)
        return false;

    m_pItemComponent->RemoveItemByType(rItem.m_nItemType, (int)rItem.m_nItemCount, EItemAction_Del_PetImproveQuality);

    return true;
}

bool PetComponent::PayForEvolution(std::map<unsigned char, unsigned int> &rMapBill, const CItem &rItem, const CItem &rExtraItem)
{
    bool bUseExtraItem = false;

    if (rExtraItem.m_nItemType != 0 && rExtraItem.m_nItemCount != 0)
        bUseExtraItem = true;

    if (m_pItemComponent == NULL)
        return false;
    else if (!CanUseItems(rItem) )
        return false;
    else if (bUseExtraItem && !CanUseItems(rExtraItem) )
        return false;

    bool bSuccess = Pay(rMapBill, 
        EChangeBillCause_Del_PetEvolution, 
        EChangeBindBillCause_Del_PetEvolution, 
        EChangeMoneyCause_Del_PetEvolution);

    if (!bSuccess)
        return false;

    m_pItemComponent->RemoveItemByType(rItem.m_nItemType, (int)rItem.m_nItemCount, EItemAction_Del_PetEvolution);
    if (bUseExtraItem)
        m_pItemComponent->RemoveItemByType(rExtraItem.m_nItemType, (int)rExtraItem.m_nItemCount, EItemAction_Del_PetEvolution);

    return true;
}

bool PetComponent::PayForFeeding(const CItem &rItem)
{
    if (m_pItemComponent == NULL)
        return false;
    else if (!CanUseItems(rItem) )
        return false;

    m_pItemComponent->RemoveItemByType(rItem.m_nItemType, (int)rItem.m_nItemCount, EItemAction_Del_PetFeeding);

    return true;
}

const ItemBase* PetComponent::GetPetEggItem(unsigned short nIndex)
{
    const ItemBase *pItemBase = NULL;

    if (m_pItemComponent != NULL)
        pItemBase = m_pItemComponent->GetItem(EItemColumn_ExpandableBag, nIndex);

    return pItemBase;
}

void PetComponent::UseOnePetEggItem(unsigned short nIndex)
{
    if (m_pItemComponent == NULL)
        return;


	ItemBase * pItemBase = m_pItemComponent->GetItem(EItemColumn_ExpandableBag, nIndex);
	if (pItemBase != NULL)
	{
		m_pItemComponent->RemoveItemByType(pItemBase->GetType(), 1, EItemAction_Del_PetIncubation);
	}
}

void PetComponent::UnlockGrid(unsigned short nCount)
{
    if (nCount == 0)
        return;

    m_nExtendPetGridCount += nCount;

    if (m_pAttrComponent != NULL)
        AddQuery(QUERY_Pet_UpdateExtendGridCount, m_nExtendPetGridCount, NULL, m_pAttrComponent->GetRoleID() );
}

void PetComponent::ChangePetName(unsigned int nPetID, const std::string &rNewName)
{
    Pet *pPet = GetPet(nPetID);

    if (pPet == NULL || rNewName.empty())
        return;

    pPet->SetName(rNewName);

    std::string *pNewName = new std::string(rNewName);

    if (pNewName != NULL)
        AddQuery(QUERY_Pet_ChangeName, pPet->GetID(), pNewName);

    SyncPetChangeInfoToOthers(pPet);
}

void PetComponent::SyncPetChangeInfoToOthers( const Pet *pPet )
{
    if ( pPet == NULL || m_pAttrComponent == NULL )
        return;

    GameMsg_S2C_SyncPetChangedInfoToOthers msg;
    msg.m_nRoleID = m_pAttrComponent->GetRoleID();
    msg.m_bCarriedPet = pPet->IsCarrying();
    msg.m_nSceneID = GetSceneID();
    PetService::Instance().EncodePetBrief(*pPet, msg.m_pet);

    SendMsgToAll(msg);
}

void PetComponent::SyncPetChangeInfoToSelf( const Pet *pPet )
{
    if ( pPet == NULL )
        return;

    GameMsg_S2C_SyncPetChangeInfoToSelf msg;
    PetService::Instance().EncodePetBrief( *pPet, msg.m_pet );

    SendPlayerMsg( &msg );
}

void PetComponent::ChangeCarried(unsigned int nPetID, unsigned char nSceneID)
{
    Pet *pPet = NULL;

    SetSceneID(nSceneID);

    if (m_nCarriedPetID != 0)
    {
        pPet = GetPet(m_nCarriedPetID);
        if (pPet != NULL)
        {
            pPet->SetState(ePetState_Rest);
            SavePetState(m_nCarriedPetID, ePetState_Rest);
            SyncPetChangeInfoToOthers(pPet);
        }
    }

    if (nPetID != 0)
    {
        pPet = GetPet(nPetID);
        if (pPet != NULL)
        {
            pPet->SetState(ePetState_Carrying);
            SavePetState(nPetID, ePetState_Carrying);
            SyncPetChangeInfoToOthers(pPet);
        }
    }

    SetCarriedPetID(nPetID);
}

void PetComponent::IncubatorLevelUp()
{
    ++m_nIncubatorLevel;

    if (m_pAttrComponent != NULL)
        AddQuery(QUERY_PetEgg_UpdateIncubatorLevel, m_nIncubatorLevel, NULL, m_pAttrComponent->GetRoleID() );
}

void PetComponent::IncubationSpeedUp()
{
    if (!m_bEggLoadCompleted || m_pEgg == NULL)
        return;

    m_pEgg->SpeedUp();

    SaveEggState();
}

void PetComponent::ImproveQuality(unsigned int nPetID)
{
    Pet *pPet = GetPet(nPetID);
    if (pPet != NULL)
    {
        int nOldSkillCount = pPet->GetActivatedSkillCount();

        pPet->ImproveQuality();
        RecoveryEndurance(pPet, pPet->GetMaxEndurance() - pPet->GetEndurance());

        AddQuery(QUERY_Pet_UpdateQuality, pPet->GetQualityType(), NULL, pPet->GetID());

        int nNewSkillCount = pPet->GetActivatedSkillCount();
        if (nNewSkillCount != nOldSkillCount)
        {
            SyncSkillChanged(pPet);
        }
    }
}

void PetComponent::Evolution(unsigned int nPetID)
{
    Pet *pPet = GetPet(nPetID);
    if (pPet != NULL)
    {
        int nOldSkillCount = pPet->GetActivatedSkillCount();

        pPet->Evolution();
        RecoveryEndurance(pPet, pPet->GetMaxEndurance() - pPet->GetEndurance());

        AddQuery(QUERY_Pet_UpdateEvolution, pPet->GetEvolutionTimes(), NULL, pPet->GetID() );

        if (pPet->GetEvolutionTimes() == 1)
        {
            RandomFightSkill(pPet);
        }

        int nNewSkillCount = pPet->GetActivatedSkillCount();
        if (nNewSkillCount != nOldSkillCount)
        {
            SyncSkillChanged(pPet);
        }
    }
}

void PetComponent::AddEndurance(Pet *pPet, int nAddedEndurance, unsigned int nTime)
{
    if (pPet != NULL && nAddedEndurance != 0)
    {
        int nNewEndurance = (int)pPet->GetEndurance() + nAddedEndurance;
        if (nNewEndurance < 0)
            nNewEndurance = 0;
        if (nNewEndurance > (int)pPet->GetMaxEndurance())
            nNewEndurance = (int)pPet->GetMaxEndurance();
        SetEndurance(pPet, (unsigned short)nNewEndurance, nTime);
    }
}

void PetComponent::SetEndurance(Pet *pPet, unsigned short nEndurance, unsigned int nTime)
{
    if (pPet != NULL && (pPet->GetEndurance() != nEndurance || pPet->GetEnduranceRecoveryTime() != nTime))
    {
        pPet->SetEndurance(nEndurance);
        pPet->SetEnduranceRecoveryTime(nTime);

        unsigned int *pRecoveryTime = new unsigned int;
        *pRecoveryTime = nTime;

        AddQuery(QUERY_Pet_UpdateEndurance, nEndurance, pRecoveryTime, pPet->GetID());

        if (pPet->IsCarrying() && GetCarriedPetID() == pPet->GetID())
        {
            SyncPetChangeInfoToSelf( pPet );
        }
    }
}

void PetComponent::AddExp(unsigned int nPetID, unsigned int nAddExp)
{
    Pet *pPet = GetPet(nPetID);
    if (pPet == NULL || nAddExp == 0)
        return;

    int nOldSkillCount = pPet->GetActivatedSkillCount();
    unsigned int nExp = pPet->GetExp() + nAddExp;
    unsigned int nLevelUpExp = pPet->GetMaxExpOfLevel();
    while (nExp >= nLevelUpExp)
    {
        if (pPet->CanLevelUp())
        {
            nExp -= nLevelUpExp;
            pPet->LevelUp();
            OnPetLevelUp(pPet);

            nLevelUpExp = pPet->GetMaxExpOfLevel();
        }
        else
        {
            nExp = nLevelUpExp;

            break;
        }
    }

    pPet->SetExp(nExp);

    int nNewSkillCount = pPet->GetActivatedSkillCount();
    if (nNewSkillCount != nOldSkillCount)
    {
        SyncSkillChanged(pPet);
    }

    unsigned int *pExp = new unsigned int(nExp);
    AddQuery(QUERY_Pet_UpdateLevelAndExp, pPet->GetLevel(), pExp, pPet->GetID());

    SyncPetChangeInfoToSelf( pPet );
}

void PetComponent::OnPetLevelUp(Pet *pPet)
{
}

void PetComponent::SyncSkillChanged(Pet *pPet)
{
    if (pPet != NULL)
    {
        GameMsg_S2C_RefreshPetSkillList msg;
        PetService::Instance().EncodePetSkill(*pPet, msg.m_petSkill);
        SendPlayerMsg(&msg);
    }
}

void PetComponent::SendExchangeItems(const std::list<CItem> &rItemList)
{
    CItemProcess::AddItems(*(CRoleEntity *)Entity(), rItemList, 
        EItemAction_Add_PetExchange, 0, true);
}

void PetComponent::SetPetEgg(PetEgg *pEgg)
{
    if (pEgg == NULL)
        return;

    m_pEgg = pEgg;

    if (m_pEgg->IsIncubating())
        m_pEgg->AddSubscriber(this);
}

void PetComponent::OpenedPetEgg()
{
    if (!m_bEggLoadCompleted || m_pEgg == NULL)
        return;

    if (m_pEgg->IsFinished() )
    {
        m_pEgg->Opened();

        SaveEggState();

        SAFE_DELETE(m_pEgg);
    }
}

void PetComponent::AddPet(Pet *pPet)
{
    if (pPet == NULL)
        return;

    m_mapPet.insert(std::make_pair(pPet->GetID(), pPet));
}

Pet* PetComponent::GetPet(unsigned int nPetID) const
{
    if (nPetID == 0)
        return NULL;

    PetMap::const_iterator itr = m_mapPet.find(nPetID);
    if (itr != m_mapPet.end())
        return itr->second;

    return NULL;
}

void PetComponent::DestroyPet(unsigned int nPetID)
{
    PetMap::iterator itr = m_mapPet.find(nPetID);
    if (itr != m_mapPet.end())
    {
        SavePetState(nPetID, ePetState_Destroy);

        // 不删除宠物实例，只移动到另一个map中，作为宠物图鉴展示
        m_mapDestroyedPet.insert(std::make_pair(nPetID, itr->second));

        itr->second = NULL;
        m_mapPet.erase(itr);
    }
}

void PetComponent::SaveEggState()
{
    if (!m_bEggLoadCompleted || m_pEgg == NULL)
        return;

    AddQuery(QUERY_PetEgg_UpdateState, m_pEgg->GetState(), NULL, m_pEgg->GetID());
}

void PetComponent::SavePetState(unsigned int nPetID, unsigned char nState)
{
    if (nPetID != 0 && nState < ePetState_Max)
        AddQuery(QUERY_Pet_UpdateState, nState, NULL, nPetID);
}

bool PetComponent::Pay(std::map<unsigned char, unsigned int> &rMapBill, EChangeBillCause eChangeBillCause,
    EChangeBindBillCause eChangeBindBillCause, EChangeMoneyCause eChangeMoneyCause)
{
    if (m_pMallComponent == NULL)
        return false;
    else if (m_pAttrComponent == NULL)
        return false;
    else if (m_pMallComponent->CanBill(rMapBill) != MALL_OK)
        return false;

    CurrencyMap::const_iterator itrMCoin = rMapBill.find(EMallCurrencyType_MCoin);
    CurrencyMap::const_iterator itrBindMCoin = rMapBill.find(EMallCurrencyType_BindMCoin);
    CurrencyMap::const_iterator itrMoney = rMapBill.find(EMallCurrencyType_Money);

    if (itrMCoin == rMapBill.end() || itrBindMCoin == rMapBill.end() || itrMoney == rMapBill.end())
        return false;

    unsigned int nMCoin = itrMCoin->second;
    unsigned int nBindMCoin = itrBindMCoin->second;
    unsigned int nMoney = itrMoney->second;

    if (m_pAttrComponent->GetPoint() < nMCoin || m_pAttrComponent->GetBindPoint() < nBindMCoin
        || m_pAttrComponent->GetMoney() < (int)nMoney || (nMCoin + nBindMCoin + nMoney) == 0)
        return false;

    if (nMCoin != 0)
        m_pAttrComponent->ChangeBill((int)(nMCoin * -1), eChangeBillCause);

    if (nBindMCoin != 0)
        m_pAttrComponent->ChangeBindBill((int)(nBindMCoin * -1), eChangeBindBillCause);

    if (nMoney != 0)
        m_pAttrComponent->ChangeMoney((int)(nMoney * -1), eChangeMoneyCause, 0);

    return true;
}

bool PetComponent::CanUseItems(const CItem &rItem)
{
    if (m_pItemComponent == NULL)
        return false;

    unsigned int nExistItemCount = m_pItemComponent->GetItemCount(rItem.m_nItemType, false, false);

    if (rItem.m_nItemCount > nExistItemCount)
        return false;

    return true;
}

void PetComponent::LoadPetIncubatorFromDB()
{
    if (m_pAttrComponent != NULL)
    {
        unsigned int nRoleID = m_pAttrComponent->GetRoleID();

        AddQuery(QUERY_PetEgg_LoadIncubator, nRoleID);
        AddQuery(QUERY_Pet_LoadExtendGridCount, nRoleID);

        PetEgg *pEgg = PetEgg::Create();
        if (pEgg != NULL)
        {
            CDelHelper delHelper(pEgg);
            AddQuery(QUERY_PetEgg_Load, nRoleID, pEgg, 0, NULL, &delHelper);
        }
    }
}

void PetComponent::SendOpenEggRes(unsigned char eErrorCode, unsigned short nPetTypeID /* = 0 */, const std::string &rDefaultName /* = "" */)
{
    GameMsg_S2C_PetOpenEggRes resMsg;
    resMsg.m_nErrorCode = eErrorCode;
    resMsg.m_nPetTypeID = nPetTypeID;
    resMsg.m_strPetDefaultName = rDefaultName;

    SendPlayerMsg(&resMsg);
}

void PetComponent::SendIncubationGetPetAnnounce(const std::string &rPetName)
{
    if (m_pAttrComponent == NULL || rPetName.empty() )
        return;

    std::string strAnnounce;
    SafeFormat(strAnnounce, CLocalization::Instance().GetString("Pet_IncubationGet_Announce"), 
        m_pAttrComponent->GetRoleName(), rPetName.c_str());

    CChatManager::Instance().SendAnnounce(true, true, strAnnounce);
}

void PetComponent::OnEnterIncubationRoom(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    GameMsg_S2C_PetEnterIncubationRoomRes resMsg;

    resMsg.m_nIncubatorLevel = m_nIncubatorLevel;
    if (m_bEggLoadCompleted)
    {
        resMsg.m_nErrorCode = ePetEnterIncubationRoomRes_Success;

        if (m_pEgg != NULL)
        {
            resMsg.m_bHasEgg = true;
            resMsg.m_nEggTypeID = m_pEgg->GetTypeID();

            if (m_pEgg->IsFinished() )
            {
                resMsg.m_nLastTime = 0;
            }
            else
            {
                resMsg.m_nLastTime = m_pEgg->GetFinishedTime() - (unsigned int)time(NULL);
            }
        }
    }
    else
    {
        resMsg.m_nErrorCode = ePetEnterIncubationRoomRes_LoadingInfos;
    }

    SendPlayerMsg(&resMsg);
}

void PetComponent::OnIncubatorLevelUp(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    PetEggService::Instance().DoIncubatorLevelUp(this);
}

void PetComponent::OnStartIncubation(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    PetEggService::Instance().DoStartIncubation(this, rMsg);
}

void PetComponent::OnIncubationRequest(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    GameMsg_S2C_PetIncubationRequestRes resMsg;

    if (m_pEgg == NULL)
    {
        resMsg.m_bFinished = false;
        resMsg.m_nLastTime = 0;
    }
    else if (m_pEgg->IsFinished() )
    {
        resMsg.m_bFinished = true;
        resMsg.m_nLastTime = 0;
    }
    else
    {
        resMsg.m_bFinished = false;
        resMsg.m_nLastTime = m_pEgg->GetFinishedTime() - (unsigned int)time(NULL);
    }

    SendPlayerMsg(&resMsg);
}

void PetComponent::OnIncubationSpeedUp(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    PetEggService::Instance().DoIncubationSpeedUp(this);
}

void PetComponent::OnOpenEgg(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    const RandomPetInfo *pPetInfo = PetEggService::Instance().DoOpenEgg(this);
    if (pPetInfo == NULL)
        return;

    PetService::Instance().DoCreatePet(this, pPetInfo->GetPetTypeID(), pPetInfo->GetPotentialID(),
        pPetInfo->HasAnnounce());
}

void PetComponent::OnRequestList(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    GameMsg_C2S_PetRequestList &rRequestList = (GameMsg_C2S_PetRequestList &)rMsg;

    GameMsg_S2C_PetRequestListRes resMsg;
    resMsg.m_nCurPage = rRequestList.m_nCurPage;
    resMsg.m_nPetCount = (int)m_mapPet.size();
    resMsg.m_nGridCount = (int)GetPetGridCount();

    if (m_nExtendPetGridCount < PetConfigManager::Instance().GetExtendGridCount())
        resMsg.m_bCanUnlock = true;
    else
        resMsg.m_bCanUnlock = false;

    int nStart = rRequestList.m_nCurPage * rRequestList.m_nCount;
    if (0 <= nStart && nStart < (int)m_mapPet.size())
    {
        PetMap::const_iterator itr = m_mapPet.begin();
        std::advance(itr, nStart);

        for (int count = 0; count < rRequestList.m_nCount && itr != m_mapPet.end(); ++count, ++itr)
        {
            Pet *pPet = itr->second;
            PetMsgInfo info;
            if (pPet != NULL)
            {
                PetService::Instance().EncodePetMsgInfo(*pPet, info);
                resMsg.m_listPetInfo.push_back(info);
            }
        }
    }

    if (resMsg.m_nGridCount <= nStart)
    {
        resMsg.m_nEmptyCount = 0;
    }
    else
    {
        int nLeft = resMsg.m_nGridCount - nStart;

        if (nLeft < rRequestList.m_nCount)
            resMsg.m_nEmptyCount = nLeft - (int)resMsg.m_listPetInfo.size();
        else
            resMsg.m_nEmptyCount = rRequestList.m_nCount - (int)resMsg.m_listPetInfo.size();
    }

    SendPlayerMsg(&resMsg);
}

void PetComponent::OnUnlockGrid(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    PetService::Instance().DoUnlockGrid(this, rMsg);
}

void PetComponent::OnChangeName(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    PetService::Instance().DoChangeName(this, rMsg);
}

void PetComponent::OnChangeCarried(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    PetService::Instance().DoChangeCarried(this, rMsg);
}

void PetComponent::OnUnloadCarried(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    PetService::Instance().DoUnloadCarried(this, rMsg);
}

void PetComponent::OnRequestImproveCondition(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    GameMsg_C2S_PetRequestImproveCondition &rImproveCondition = (GameMsg_C2S_PetRequestImproveCondition &)rMsg;

    GameMsg_S2C_PetRequestImproveConditionRes resMsg;
    resMsg.m_nPetID = rImproveCondition.m_nPetID;

    Pet *pPet = GetPet(resMsg.m_nPetID);
    if (pPet != NULL)
    {
        PetService::Instance().GetImproveQualityCondition(pPet->GetTypeID(), pPet->GetQualityType(),
            resMsg.m_nCostType, resMsg.m_nCostAmount, resMsg.m_itemImprove);
    }

    SendPlayerMsg(&resMsg);
}

void PetComponent::OnImprove(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    PetService::Instance().DoImprove(this, rMsg);
}

void PetComponent::OnRequestEvolutionCondition(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    GameMsg_C2S_PetRequestEvolutionCondition &rEvolutionCondition = (GameMsg_C2S_PetRequestEvolutionCondition &)rMsg;

    GameMsg_S2C_PetRequestEvolutionConditionRes resMsg;
    resMsg.m_nPetID = rEvolutionCondition.m_nPetID;

    Pet *pPet = GetPet(resMsg.m_nPetID);
    if (pPet != NULL)
    {
        PetService::Instance().GetEvolutionCondition(pPet->GetTypeID(), pPet->GetEvolutionTimes() + 1, 
            resMsg.m_nCostType, resMsg.m_nCostAmount, resMsg.m_itemEvolution, resMsg.m_itemExtra, resMsg.m_nBaseRate);
    }

    SendPlayerMsg(&resMsg);
}

void PetComponent::OnEvolution(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    PetService::Instance().DoEvolution(this, rMsg);
}

void PetComponent::OnFeeding(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    PetService::Instance().DoFeeding(this, rMsg);
}

void PetComponent::OnTrainSkill(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    GameMsg_C2S_RequestTrainingSkill &request = (GameMsg_C2S_RequestTrainingSkill &)rMsg;
    unsigned int nPetID = request.m_nPetID;
    SkillID nSkillID = request.m_nSkillID;

    Pet *pPet = GetPet(nPetID);
    int nError = TrainSkill(pPet, nSkillID);

    GameMsg_S2C_RequestTrainingSkillRes response;
    response.m_nErrorCode = (unsigned char)nError;
    response.m_nPetID = nPetID;
    response.m_nSkillID = nSkillID;
    if (nError == 0 && pPet != NULL)
    {
        PetSkill *skill = pPet->GetSkill(nSkillID);
        if (skill != NULL)
        {
            PetService::Instance().EncodePetSkill(*pPet, *skill, response.m_PetSkill);
        }
    }

    SendPlayerMsg(&response);
}

int PetComponent::TrainSkill(Pet *pPet, SkillID nSkillID, bool bIsFisrtTime)
{
    if (pPet == NULL)
    {
        return eTrainSkillError_NoPet;
    }

    PetSkillConfig skillConfig;
    if (!PetConfigManager::Instance().GetPetSkill(nSkillID, skillConfig))
    {
        return eTrainSkillError_NoSkill;
    }

    // process cost
    if (!bIsFisrtTime)
    {
        CurrencyMap mapBills;
        skillConfig.GetLevelUpCost().AddIntoCurrencyMap(mapBills, true);
        if (!Pay(mapBills, EChangeBillCause_Del_PetSkillTrain, 
            EChangeBindBillCause_Del_PetSkillTrain, EChangeMoneyCause_Del_PetSkillTrain))
        {
            return eTrainSkillError_NotEnoughMoney;
        }
    }

    // skill process
    PetSkill *skill = pPet->GetSkill(nSkillID);
    if (skill == NULL)
    {
        skill = new PetSkill();
        skill->SetSkillID(nSkillID);
        skill->SetLevel(1);
        skill->SetSkillValue(skillConfig.RandomSkillValue());

        pPet->SetSkill(nSkillID, skill);
    }
    else
    {
        if (skill->GetLevel() >= skillConfig.GetMaxLevel())
        {
            return eTrainSkillError_AlreadyMaxLevel;
        }

        skill->SetLevel(skill->GetLevel() + 1);
        skill->SetSkillValue(skill->GetSkillValue() + skillConfig.GetLevelUpValue());
    }

    AddQuery(QUERY_Pet_UpdateSkill, 0, pPet);

    return 0;
}

int PetComponent::RandomFightSkill(Pet *pet)
{
    if (pet == NULL)
    {
        return eResetSkillError_NoPet;
    }

    const PetBaseConfig *petConfig = PetConfigManager::Instance().GetPetBaseConfig(pet->GetTypeID());
    if (petConfig == NULL)
    {
        return eResetSkillError_NoPet;
    }

    std::set<SkillID> newSkillIDSet;
    int nFightSkillCount = petConfig->GetFightSkillCount();
    PetConfigManager::Instance().RandomPetSkillList(newSkillIDSet, std::set<SkillID>(), 0, nFightSkillCount);

    for (std::set<SkillID>::const_iterator it = newSkillIDSet.begin(); it != newSkillIDSet.end(); ++it)
    {
        PetSkillConfig skillConfig;
        PetConfigManager::Instance().GetPetSkill(*it, skillConfig);
        PetSkill *skill = new PetSkill();
        skill->SetSkillID(skillConfig.GetSkillID());
        skill->SetLevel(1);
        skill->SetSkillValue(skillConfig.RandomSkillValue());
        pet->SetSkill(0, skill);
    }

    AddQuery(QUERY_Pet_UpdateSkill, 0, pet);

    return 0;
}

void PetComponent::OnResetSkill(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    GameMsg_C2S_RequestResetSkills &request = (GameMsg_C2S_RequestResetSkills &)rMsg;
    unsigned int nPetID = request.m_nPetID;
    std::set<SkillID> &setLockSkillID = request.m_setLockSkillID;

    Pet *pPet = GetPet(nPetID);
    int nError = ResetSkill(pPet, setLockSkillID);

    GameMsg_S2C_RequestResetSkillsRes response;
    response.m_nErrorCode = (unsigned char)nError;
    response.m_nPetID = nPetID;
    if (nError == 0 && pPet != NULL)
    {
        int nNextCostRatio = PetConfigManager::Instance().GetResetSkillCost(pPet->GetResetSkillTimes());
        const PetBaseConfig *pPetConfig = PetConfigManager::Instance().GetPetBaseConfig(pPet->GetTypeID());
        response.m_nResetSkillCostType = pPetConfig->GetResetSkillCost().GetType();
        response.m_nResetSkillCostAmount = pPetConfig->GetResetSkillCost().GetAmount() * (unsigned int)nNextCostRatio;
        response.m_nDanceSkillCount = pPet->GetDanceSkillCount();
        response.m_nFightSkillCount = pPet->GetFightSkillCount();
        PetService::Instance().EncodePetSkillList(*pPet, response.m_listDanceSkill, response.m_listFightSkill);
    }
    SendPlayerMsg(&response);
}

int PetComponent::ResetSkill(Pet *pPet, const std::set<SkillID> &lockSkillIDSet, bool bIsFisrtTime)
{
    if (pPet == NULL)
    {
        return eResetSkillError_NoPet;
    }

    for (std::set<SkillID>::const_iterator it = lockSkillIDSet.begin(); it != lockSkillIDSet.end(); ++it)
    {
        SkillID nSkillID = *it;
        PetSkill *skill = pPet->GetSkill(nSkillID);
        if (skill == NULL)
        {
            WriteLog(LOGLEVEL_ERROR, "[PET] ResetPetSkill SkillID=%d no such skill", (int)nSkillID);
            return eResetSkillError_NoSkill;
        }
    }

    if ((int)lockSkillIDSet.size() >= pPet->GetDanceSkillCount() + pPet->GetFightSkillCount())
    {
        return eResetSkillError_LockAllSkill;
    }

    // 刚获得宠物时，随机的技能不消耗道具，只有重置才消耗，这里处理非首次的消耗
    unsigned int nResetTimes = pPet->GetResetSkillTimes();
    if (!bIsFisrtTime)
    {
        if (IsTimeIsToday(pPet->GetResetSkillTime()))
        {
            nResetTimes++;
        }
        else
        {
            nResetTimes = 1;
        }

        int nError = PayResetSkillCost(pPet, (int)lockSkillIDSet.size(), nResetTimes);
        if (nError != 0)
        {
            WriteLog(LOGLEVEL_ERROR, "[PET] ResetPetSkill PayResetSkillCost failed");
            return nError;
        }
    }

    // 随机技能列表
    std::set<SkillID> newSkillIDSet;
    int nDanceSkillCount = pPet->GetDanceSkillCount();
    int nFightSkillCount = pPet->GetFightSkillCount(); // 刚获得宠物时，不会有战斗技能
    PetConfigManager::Instance().RandomPetSkillList(newSkillIDSet, lockSkillIDSet, nDanceSkillCount, nFightSkillCount);

    // 重置技能修改内存
    pPet->ClearSkillExclude(lockSkillIDSet);
    for (std::set<SkillID>::const_iterator it = newSkillIDSet.begin(); it != newSkillIDSet.end(); ++it)
    {
        PetSkillConfig skillConfig;
        PetConfigManager::Instance().GetPetSkill(*it, skillConfig);
        PetSkill *skill = new PetSkill();
        skill->SetSkillID(skillConfig.GetSkillID());
        skill->SetLevel(1);
        skill->SetSkillValue(skillConfig.RandomSkillValue());
        pPet->SetSkill(0, skill);
    }

    pPet->SetResetSkillTime((unsigned int)time(NULL));
    pPet->SetResetSkillTimes(nResetTimes);
    AddQuery(QUERY_Pet_ResetSkill, 0, pPet);

    return 0;
}

int PetComponent::PayResetSkillCost(Pet *pPet, int nLockSkillCount, int nResetTimes)
{
    // 锁定消耗
    CItem itemCost;
    if (nLockSkillCount > 0)
    {
        if (!PetConfigManager::Instance().GetLockSkillCost(nLockSkillCount, itemCost.m_nItemType, itemCost.m_nItemCount))
        {
            WriteLog(LOGLEVEL_ERROR, "PayResetSkillCost error, no lock item config nLockSkillCount=%d", nLockSkillCount);
            return eResetSkillError_NotEnoughItem;
        }
        else if (m_pItemComponent == NULL)
        {
            WriteLog(LOGLEVEL_ERROR, "PayResetSkillCost error, no item component");
            return eResetSkillError_NotEnoughItem;
        }
        else if (!CanUseItems(itemCost))
        {
            WriteLog(LOGLEVEL_ERROR, "PayResetSkillCost error, not enough lock item");
            return eResetSkillError_NotEnoughItem;
        }
    }

    // 重置消耗
    const PetBaseConfig *pPetConfig = PetConfigManager::Instance().GetPetBaseConfig(pPet->GetTypeID());
    if (pPetConfig == NULL)
    {
        WriteLog(LOGLEVEL_ERROR, "PayResetSkillCost error, no pet base config");
        return eResetSkillError_NoPet;
    }

    Currency costCurrency = pPetConfig->GetResetSkillCost();
    CurrencyMap mapBills;

    if (nResetTimes < 1)
        nResetTimes = 1;

    int nResetCostMoneyRatio = PetConfigManager::Instance().GetResetSkillCost(nResetTimes);
    costCurrency.SetAmount(costCurrency.GetAmount() * nResetCostMoneyRatio);
    costCurrency.AddIntoCurrencyMap(mapBills, true);

    if (!Pay(mapBills, EChangeBillCause_Del_PetResetSkill, EChangeBindBillCause_Del_PetResetSkill, 
        EChangeMoneyCause_Del_PetResetSkill))
    {
        WriteLog(LOGLEVEL_ERROR, "PayResetSkillCost error, not enough money, need currency=%d, amount=%u",
            (int)costCurrency.GetType(), costCurrency.GetAmount());
        return eResetSkillError_NotEnoughMoney;
    }

    // 扣除锁定消耗
    if (nLockSkillCount > 0)
    {
        m_pItemComponent->RemoveItemByType(itemCost.m_nItemType, (int)itemCost.m_nItemCount,EItemAction_Del_PetResetSkill);
    }

    return 0;
}

void PetComponent::OnGetExchangeList(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    GameMsg_C2S_PetGetExchangeList &rGetListMsg = (GameMsg_C2S_PetGetExchangeList &)rMsg;

    GameMsg_S2C_PetGetExchangeListRes resMsg;
    resMsg.m_nCurPage = rGetListMsg.m_nCurPage;
    resMsg.m_nPetCount = (unsigned short)m_mapPet.size();

    if (HasCarriedPet())
    {
        resMsg.m_nPetCount--;
    }

    unsigned int nStart = rGetListMsg.m_nCurPage * rGetListMsg.m_nCount;
    if (nStart < (unsigned int)m_mapPet.size())
    {
        PetMap::const_iterator itr = m_mapPet.begin();
        std::advance(itr, nStart);

        for (unsigned int nCount = 0; nCount < rGetListMsg.m_nCount && itr != m_mapPet.end(); ++nCount, ++itr)
        {
            Pet *pPet = itr->second;
            if (pPet != NULL && !pPet->IsCarrying()) // 兑换列表中要去除装备中的宠物
            {
                PetExchangeMsgInfo infos;
                PetService::Instance().EncodePetExchangeMsgInfo(*pPet, infos);

                resMsg.m_listExchangeInfos.push_back(infos);
            }
        }
    }

    SendPlayerMsg(&resMsg);
}

void PetComponent::OnExchange(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    PetService::Instance().DoExchange(this, rMsg);
}

void PetComponent::OnGetPetLibraryTypeList(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    unsigned int nRoleID = ((GameMsg_C2S_GetPetLibraryTypeList &)rMsg).m_nRoleID;

    GameMsg_S2C_GetPetLibraryTypeListRes response;
    response.m_nRoleID = nRoleID;

    std::map<unsigned short, unsigned char> mapPetType; // pet type id -> pet attribute id
    PetConfigManager::Instance().GetPetTypeMap(mapPetType);

    for (std::map<unsigned short, unsigned char>::iterator it = mapPetType.begin();
        it != mapPetType.end(); ++it)
    {
        PetLibraryMsg petTypeInfo;
        petTypeInfo.m_nPetTypeID = it->first;
        petTypeInfo.m_nAttributeType = it->second;

        const Pet *pPet = GetTopPet(petTypeInfo.m_nPetTypeID);
        if (pPet != NULL)
            petTypeInfo.m_nQualityType = pPet->GetQualityType();

        response.m_listPetType.push_back(petTypeInfo);
    }

    SendPlayerMsg(&response);
}

/*void PetComponent::GetPetTypeMap(std::map<unsigned short, unsigned char> mapPetType)
{
    PetMap::const_iterator it = m_mapPet.begin();
    for (; it != m_mapPet.end(); ++it)
    {
        Pet *pPet = it->second;
        if (pPet != NULL)
        {
            mapPetType[pPet->GetTypeID()] = pPet->GetAttributeType();
        }
    }

    for (it = m_mapDestroyedPet.begin(); it != m_mapDestroyedPet.end(); ++it)
    {
        Pet *pPet = it->second;
        if (pPet != NULL)
        {
            mapPetType[pPet->GetTypeID()] = pPet->GetAttributeType();
        }
    }
}*/

void PetComponent::OnGetPetLibraryList(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    unsigned int nRoleID = ((GameMsg_C2S_GetPetLibraryList &)rMsg).m_nRoleID;
    std::list<unsigned short> &listPetType = ((GameMsg_C2S_GetPetLibraryList &)rMsg).m_listPetType;

    GameMsg_S2C_GetPetLibraryListRes response;
    response.m_nRoleID = nRoleID;
    std::list<PetMsgInfo> &listPetMsg = response.m_listPet;

    for (std::list<unsigned short>::const_iterator it = listPetType.begin();
        it != listPetType.end(); ++it)
    {
        unsigned short nPetType = *it;
        PetMsgInfo petMsg;

        const Pet *pPet = GetTopPet(nPetType);
        if (pPet != NULL)
        {
            PetService::Instance().EncodePetMsgInfo(*pPet, petMsg);
            listPetMsg.push_back(petMsg);
        }
        else
        {
            const PetBaseConfig* petConfig = PetConfigManager::Instance().GetPetBaseConfig(nPetType);
            if (petConfig != NULL)
            {
                petMsg.m_nTypeID = petConfig->GetPetTypeID();
                petMsg.m_strDefaultName = petConfig->GetDefaultName();
                petMsg.m_nAttributeType = petConfig->GetAttributeType();
                petMsg.m_nMaxLevel = petConfig->GetTopLevel();
                petMsg.m_nResetSkillCostType = petConfig->GetResetSkillCost().GetType();
                petMsg.m_nResetSkillCostAmount = petConfig->GetResetSkillCost().GetAmount();

                const PetQualityInfo *pQuality = PetConfigManager::Instance().GetPetQualityInfo(nPetType);
                if (pQuality != NULL)
                {
                    petMsg.m_bCanImproveQuality = true;
                    petMsg.m_nQualityType = pQuality->GetMaxQualityType();
                    petMsg.m_nEndurance = pQuality->GetMaxEndurance(petMsg.m_nQualityType);
                    petMsg.m_nMaxEndurance = petMsg.m_nEndurance;
                }

                petMsg.m_nDanceSkillCount = petConfig->GetDanceSkillCount();
                petMsg.m_nFightSkillCount = petConfig->GetFightSkillCount();

                listPetMsg.push_back(petMsg);
            }
        }
    }

    SendPlayerMsg(&response);
}

const Pet *PetComponent::GetTopPet(unsigned short nPetType) const
{
    Pet *pTopPet = NULL;
    Pet *pPet = NULL;

    PetMap::const_iterator it = m_mapPet.begin();
    for (; it != m_mapPet.end(); ++it)
    {
        pPet = it->second;
        if ( (pPet != NULL && pPet->GetTypeID() == nPetType)
            && (pTopPet == NULL
            || pPet->GetPotentialID() > pTopPet->GetPotentialID()
            || pPet->GetQualityType() > pTopPet->GetQualityType()
            || pPet->GetEvolutionTimes() > pTopPet->GetEvolutionTimes()
            || pPet->GetLevel() > pTopPet->GetLevel()
            || pPet->GetExp() > pTopPet->GetExp()
            || pPet->GetDanceSkillCount() > pTopPet->GetDanceSkillCount()
            || pPet->GetFightSkillCount() > pTopPet->GetFightSkillCount()
            || pPet->IsCarrying()
            || pPet->GetState() < pTopPet->GetState()) )
        {
            pTopPet = pPet;
        }
    }

    for (it = m_mapDestroyedPet.begin(); it != m_mapDestroyedPet.end(); ++it)
    {
        pPet = it->second;
        if ( (pPet != NULL && pPet->GetTypeID() == nPetType)
            && (pTopPet == NULL
            || pPet->GetPotentialID() > pTopPet->GetPotentialID()
            || pPet->GetQualityType() > pTopPet->GetQualityType()
            || pPet->GetEvolutionTimes() > pTopPet->GetEvolutionTimes()
            || pPet->GetLevel() > pTopPet->GetLevel()
            || pPet->GetExp() > pTopPet->GetExp()
            || pPet->GetDanceSkillCount() > pTopPet->GetDanceSkillCount()
            || pPet->GetFightSkillCount() > pTopPet->GetFightSkillCount()
            || pPet->IsCarrying()
            || pPet->GetState() < pTopPet->GetState()) )
        {
            pTopPet = pPet;
        }
    }

    return pTopPet;
}

void PetComponent::OnReportPetDanceEmotion(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    GameMsg_S2C_SyncPetDanceEmotion msg;
    msg.m_nRoleID = m_pAttrComponent->GetRoleID();
    msg.m_nEmotionType = ((GameMsg_C2S_ReportPetDanceEmotion &)rMsg).m_nEmotionType;

    SendMsgToAll(msg);
}

void PetComponent::OnPetGuideStep(GameMsg_Base &rMsg, CSlotPeer &rSlotPeer)
{
    GameMsg_S2C_PetGuideResponse response;
    int nStep = (int)((GameMsg_C2S_PetGuideRequest &)rMsg).m_nGuideStep;
    bool bSucc = ChangeGuideStep(nStep);
    if (bSucc)
    {
        if (nStep == PetConfigManager::Instance().GetRewardPetEggGuideStep())
        {
            std::list<CItem> listPetEgg = PetConfigManager::Instance().GetRewardPetEgg();
            if (!listPetEgg.empty())
            {
                CItemProcess::AddItems(*((CRoleEntity *)Entity()), listPetEgg, EItemAction_Add_PetGuide, 0, true);
                response.m_nPetEggType = listPetEgg.front().m_nItemType;
            }
        }
    }
    SendPlayerMsg(&response);
}

void PetComponent::OnLoadIncubatorCallback(QueryBase &rQuery)
{
    if (rQuery.m_nResult == Query_Res_OK)
        m_nIncubatorLevel = (unsigned short)rQuery.m_nAttachment;
}

void PetComponent::OnLoadExtendCountCallback(QueryBase &rQuery)
{
    if (rQuery.m_nResult == Query_Res_OK)
        m_nExtendPetGridCount = (unsigned short)rQuery.m_nAttachment;
}

void PetComponent::OnLoadPetEggCallback(QueryBase &rQuery)
{
    m_bEggLoadCompleted = true;
    PetEgg *pEgg = (PetEgg *)rQuery.m_pAttachment;

    if (rQuery.m_nResult != Query_Res_OK)
    {
        SAFE_DELETE(pEgg);

        return;
    }

    SetPetEgg(pEgg);
}

unsigned int PetComponent::GetCarriedPetID() const
{
    return m_nCarriedPetID;
}

bool PetComponent::HasCarriedPet() const
{
    return (m_nCarriedPetID != 0);
}

Pet *PetComponent::GetCarriedPet() const
{
    if (m_nCarriedPetID > 0)
    {
        return GetPet(m_nCarriedPetID);
    }
    return NULL;
}

bool PetComponent::CastPetDanceSkill(std::map<SkillID, int> &mapPetSkill)
{
    Pet *carriedPet = GetCarriedPet();
    if (carriedPet == NULL)
    {
        return false;
    }

    int nCostEndruance = PetConfigManager::Instance().GetDanceCostEndurance();
    if (!CostEndurance(carriedPet, nCostEndruance))
    {
        WriteLog(LOGLEVEL_DEBUG, "[PET] cast dance skill: not enough endurance");
        return false;
    }

    std::list<PetSkill *> listPetSkill;
    carriedPet->GetActivatedDanceSkillList(listPetSkill);

    // 技能消息广播
    GameMsg_S2C_CastPetDanceSkill castSkillMsg;
    castSkillMsg.m_nRoleID = m_pAttrComponent->GetRoleID();
    castSkillMsg.m_nPetID = carriedPet->GetID();

    for (std::list<PetSkill *>::iterator it = listPetSkill.begin(); it != listPetSkill.end(); ++it)
    {
        PetSkill *skill = *it;
        castSkillMsg.m_listSkillList.push_back(skill->GetSkillID());

        mapPetSkill[skill->GetSkillID()] = skill->GetSkillValue();
    }

    SendMsgToAll(castSkillMsg, false);

    WriteLog(LOGLEVEL_DEBUG, "[PET] roleid=%u petid=%u cast skill count=%d", 
        m_pAttrComponent->GetRoleID(),carriedPet->GetID(), (int)listPetSkill.size());

    return true;
}

void PetComponent::SendMsgToAll(GameMsg_Base &rMsg, bool bNotSelf/* = true*/)
{
    if (m_pRoomComponent != NULL && m_pRoomComponent->Room() != NULL)
    {
        m_pRoomComponent->Room()->SendMsgToAll(&rMsg, bNotSelf ? m_pRoomComponent : NULL);
    }
    if (m_pAmuseRoomComponent != NULL && m_pAmuseRoomComponent->AmuseRoom() != NULL)
    {
        m_pAmuseRoomComponent->AmuseRoom()->SendMsgToAll(&rMsg, bNotSelf ? m_pAmuseRoomComponent : NULL);
    }
    if (m_pPhotoRoomComponent != NULL && m_pPhotoRoomComponent->PhotoRoom() != NULL)
    {
        m_pPhotoRoomComponent->PhotoRoom()->SendMsgToAllBut(&rMsg, bNotSelf ? m_pPhotoRoomComponent : NULL);
    }
    if (m_pCoupleComponent != NULL && m_pCoupleComponent->WeddingRoom() != NULL)
    {
        m_pCoupleComponent->WeddingRoom()->SendMsgToAllBut(&rMsg, bNotSelf ? m_pCoupleComponent : NULL);
    }
    if (m_pDungeonComponent != NULL && m_pDungeonComponent->GetDungeon() != NULL)
    {
        m_pDungeonComponent->GetDungeon()->SendMsgToAll(&rMsg, bNotSelf ? m_pAttrComponent->GetRoleID() : 0);
    }
    if ( m_pRoleDanceGroup != NULL && m_pRoleDanceGroup->GetCeremonyRoom() != NULL )
    {
        m_pRoleDanceGroup->GetCeremonyRoom()->RoomBroadcast(rMsg);
    }
}

bool PetComponent::ChangeGuideStep(int nStep)
{
    if (nStep > GetGuideStep())
    {
        SetGuideStep(nStep);

        AddQuery(QUERY_Pet_UpdateGuideStep, (unsigned int)m_nGuideStep, NULL, m_pAttrComponent->GetRoleID());

        return true;
    }

    return false;
}

