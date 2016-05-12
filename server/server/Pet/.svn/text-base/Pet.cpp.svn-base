#include "Pet.h"

#include "PetConfig/PetConfigManager.h"

#include "../util/Split.h"
#include "../../datastructure/DataStruct_DB.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////////
PetSkill::PetSkill() 
    : m_nSkillIndex(0)
    , m_nSkillID(0)
    , m_nLevel(0)
    , m_nSkillValue(0)
{
}

PetSkill::~PetSkill()
{
}

//////////////////////////////////////////////////////////////////////////
Pet::Pet(void)
    : m_nID(0)
    , m_nState(ePetState_Rest)
    , m_nLevel(1)
    , m_nExp(0)
    , m_nQualityType(ePetQualityType_None)
    , m_nEvolutionTimes(0)
    , m_nAttributeType(ePetAttributeType_None)
    , m_nTypeID(0)
    , m_nPotentialID(1)
    , m_strName("pet name")
    , m_nEndurance(0)
    , m_nEnduranceRecoveryTime(0)
    , m_nResetSkillTime(0)
    , m_nResetSkillTimes(0)
    , m_nMaxEndurance(0)
    , m_pBaseConfig(NULL)
    , m_pPotentialConfig(NULL)
    , m_pQualityConfig(NULL)
    , m_pEvolutionConfig(NULL)
{
}

Pet::~Pet(void)
{
    ClearSkill();
}

void Pet::SetTypeID(unsigned short nTypeID)
{
    m_nTypeID = nTypeID;
    m_pBaseConfig = PetConfigManager::Instance().GetPetBaseConfig(m_nTypeID);
    m_pQualityConfig = PetConfigManager::Instance().GetPetQualityInfo(m_nTypeID);
    m_pEvolutionConfig = PetConfigManager::Instance().GetPetEvolutionInfo(m_nTypeID);
}

void Pet::SetPotentialID(unsigned char nPotentialID)
{
    m_nPotentialID = nPotentialID;
    m_pPotentialConfig = PetConfigManager::Instance().GetPetPotential(m_nPotentialID);
}

bool Pet::IsResting() const
{
    return m_nState == ePetState_Rest;
}

bool Pet::IsCarrying() const
{
    return m_nState == ePetState_Carrying;
}

bool Pet::IsDestroyed() const
{
    return m_nState == ePetState_Destroy;
}

void Pet::SetState(unsigned char nState)
{
    if (nState < ePetState_Max)
        m_nState = nState;
}

bool Pet::CanImproveQuality() const
{
    return (m_nQualityType < GetMaxQuality()
        && m_pQualityConfig != NULL
        && m_pQualityConfig->CanImproveQuality(m_nQualityType, m_nLevel));
}

unsigned char Pet::GetMaxQuality() const
{
    if (m_pPotentialConfig == NULL || m_pQualityConfig == NULL)
        return 0;

    unsigned char nMaxQuality = m_pQualityConfig->GetMaxQualityType();
    if (nMaxQuality > m_pPotentialConfig->GetTopQuality())
        nMaxQuality = m_pPotentialConfig->GetTopQuality();

    return nMaxQuality;
}

void Pet::ImproveQuality()
{
    if ((m_nQualityType + 1) < ePetQualityType_Max)
    {
        SetQualityType(m_nQualityType + 1);

        m_nMaxEndurance = ResetMaxEndurance();
    }
}

bool Pet::CanEvolution() const
{
    return (m_nEvolutionTimes < GetMaxEvolutionTimes()
        && m_pEvolutionConfig != NULL
        && m_pEvolutionConfig->CanEvolution(m_nEvolutionTimes + 1, m_nLevel));
}

unsigned char Pet::GetMaxEvolutionTimes() const
{
    if (m_pPotentialConfig == NULL || m_pEvolutionConfig == NULL)
        return 0;

    unsigned char nMaxEvolutionTimes = m_pPotentialConfig->GetTopEvolutionTimes();
    if (nMaxEvolutionTimes > m_pEvolutionConfig->GetMaxEvolutionTimes())
        nMaxEvolutionTimes = m_pEvolutionConfig->GetMaxEvolutionTimes();

    return nMaxEvolutionTimes;
}

void Pet::Evolution()
{
    SetEvolutionTimes(m_nEvolutionTimes + 1);

    m_nMaxEndurance = ResetMaxEndurance();
}

unsigned short Pet::GetMaxEndurance() const
{
    if (m_nMaxEndurance != 0)
    {
        return m_nMaxEndurance;
    }

    return ResetMaxEndurance();
}

unsigned short Pet::ResetMaxEndurance() const
{
    if (m_pQualityConfig == NULL || m_pEvolutionConfig == NULL)
        return 0;

    unsigned short nMaxEndurance = m_pQualityConfig->GetMaxEndurance(m_nQualityType);
    if (m_nEvolutionTimes > 0)
        nMaxEndurance = m_pEvolutionConfig->GetMaxEndurance(m_nEvolutionTimes);

    return nMaxEndurance;
}

bool Pet::CanLevelUp() const
{
    return m_nLevel < GetTopLevel();
}

unsigned short Pet::GetTopLevel() const
{
    if (m_pPotentialConfig == NULL || m_pBaseConfig == NULL)
        return 0;

    unsigned short nTopLevel = PetConfigManager::Instance().GetTopLevel();
    unsigned short nMaxQualityLevel = nTopLevel;
    unsigned short nMaxEvolutionLevel = nTopLevel;

    if ( m_pQualityConfig != NULL )
        nMaxQualityLevel = m_pQualityConfig->GetNextImproveLevel( m_nQualityType, nMaxQualityLevel );

    if ( m_pEvolutionConfig != NULL )
        nMaxEvolutionLevel = m_pEvolutionConfig->GetNextEvolutionLevel( m_nEvolutionTimes, nMaxEvolutionLevel );

    if (nTopLevel > m_pPotentialConfig->GetTopLevel())
        nTopLevel = m_pPotentialConfig->GetTopLevel();
    if (nTopLevel > m_pBaseConfig->GetTopLevel())
        nTopLevel = m_pBaseConfig->GetTopLevel();
    if ( nTopLevel > nMaxQualityLevel )
        nTopLevel = nMaxQualityLevel;
    if ( nTopLevel > nMaxEvolutionLevel )
        nTopLevel = nMaxEvolutionLevel;

    return nTopLevel;
}

void Pet::LevelUp()
{
    if (CanLevelUp())
        SetLevel(m_nLevel + 1);
}

unsigned int Pet::GetMaxExpOfLevel() const
{
    if (m_pBaseConfig == NULL)
        return 0;

    return PetConfigManager::Instance().GetLevelUpExp(m_nLevel, m_pBaseConfig->GetExpRatio());
}

void Pet::PrepareSkillIndex()
{
    int nSkillCount = GetDanceSkillCount() + GetFightSkillCount();
    for (int i =0; i < nSkillCount; ++i)
        m_petSkillIndex[i] = 0;
}

bool Pet::HasSkill(SkillID skillID) const
{
    if (m_petSkill.find(skillID) != m_petSkill.end())
    {
        return true;
    }

    return false;
}

bool Pet::HasSkill(SkillType nSkillType) const
{
    for (std::map<SkillID, PetSkill *>::const_iterator it = m_petSkill.begin();
        it != m_petSkill.end(); ++it)
    {
        if (it->second == NULL)
            continue;

        SkillID nSkillID = it->first;
        PetSkillConfig skillConfig;
        if (PetConfigManager::Instance().GetPetSkill(nSkillID, skillConfig)
            && skillConfig.GetType() == nSkillType)
        {
            return true;
        }
    }
    return false;
}

bool Pet::IsActivatedSkill(const PetSkill &skill) const
{
    int nSkillIndex = skill.GetSkillIndex();
    const PetSkillUnlockConfig *skillConfig = PetConfigManager::Instance().GetPetSkillUnlockConfig(nSkillIndex);
    if (skillConfig != NULL
        && GetLevel() >= skillConfig->GetRequireLevel()
        && GetQualityType() >= skillConfig->GetRequireQualityType()
        && GetEvolutionTimes() >= skillConfig->GetRequireEvolutionTimes())
    {
        return true;
    }

    return false;
}

PetSkill *Pet::GetSkill(SkillID skillID) const
{
    PetSkill *skill = NULL;
    if (m_petSkill.find(skillID) != m_petSkill.end())
    {
        skill = m_petSkill.find(skillID)->second;
    }

    return skill;
}

PetSkill *Pet::GetSkill(SkillType nSkillType) const
{
    for (std::map<SkillID, PetSkill *>::const_iterator it = m_petSkill.begin();
        it != m_petSkill.end(); ++it)
    {
        SkillID nSkillID = it->first;
        PetSkill *skill = it->second;

        if (skill == NULL)
            continue;

        PetSkillConfig skillConfig;
        if (PetConfigManager::Instance().GetPetSkill(nSkillID, skillConfig)
            && skillConfig.GetType() == nSkillType)
        {
            return skill;
        }
    }

    return NULL;
}

void Pet::SetSkill(SkillID nOldSkillID, PetSkill *skill)
{
    RemoveSkill(nOldSkillID);

    for (int i = 0; i < (int)m_petSkillIndex.size(); ++i)
    {
        if (m_petSkillIndex[i] == 0)
        {
            skill->SetSkillIndex(i);
            m_petSkill[skill->GetSkillID()] = skill;
            m_petSkillIndex[i] = skill->GetSkillID();
            return;
        }
    }
}

void Pet::RemoveSkill(SkillID nSkillID)
{
    PetSkill *skill = GetSkill(nSkillID);
    if (skill != NULL)
    {
        m_petSkillIndex[skill->GetSkillIndex()] = 0;
        delete skill;
    }

    m_petSkill.erase(nSkillID);
}

void Pet::RemoveSkill(SkillType nSkillType)
{
    PetSkill *skill = GetSkill(nSkillType);
    if (skill != NULL)
    {
        RemoveSkill(skill->GetSkillID());
    }
}

int Pet::GetDanceSkillCount() const
{
    if (m_pBaseConfig == NULL || m_pPotentialConfig == NULL)
        return 0;

    int nCount = (int)m_pBaseConfig->GetDanceSkillCount();
    if (nCount > m_pPotentialConfig->GetMaxSkillCount())
        nCount = (int)m_pPotentialConfig->GetMaxSkillCount();

    return nCount;
}

int Pet::GetFightSkillCount() const
{
    if (m_pBaseConfig == NULL || m_pPotentialConfig == NULL)
        return 0;

    int nCount = 0;
    for (std::map<SkillID, PetSkill *>::const_iterator it = m_petSkill.begin();
        it != m_petSkill.end(); ++it)
    {
        PetSkill *skill = it->second;
        PetSkillConfig skillConfig;
        if (skill != NULL && PetConfigManager::Instance().GetPetSkill(skill->GetSkillID(), skillConfig)
            && skillConfig.IsFightSkill(skillConfig.GetType()))
        {
            nCount++;
        }
    }

    if (nCount > m_pPotentialConfig->GetMaxSkillCount())
        nCount = (int)m_pPotentialConfig->GetMaxSkillCount();

    return nCount;
}

void Pet::GetSkillList(std::list<PetSkill *> &skillList) const
{
    for (std::map<SkillID, PetSkill *>::const_iterator it = m_petSkill.begin();
        it != m_petSkill.end(); ++it)
    {
        PetSkill *skill = it->second;
        skillList.push_back(skill);
    }
}

void Pet::GetOrderedSkillList(std::list<PetSkill *> &skillList) const
{
    for (std::map<int, SkillID>::const_iterator it = m_petSkillIndex.begin();
        it != m_petSkillIndex.end(); ++it)
    {
        SkillID nSkillID = it->second;
        if (nSkillID != 0)
        {
            PetSkill *skill = GetSkill(nSkillID);
            if (skill != NULL)
                skillList.push_back(skill);
        }
    }
}

void Pet::GetDanceSkillList(std::list<PetSkill *> &skillList) const
{
    for (std::map<SkillID, PetSkill *>::const_iterator it = m_petSkill.begin();
        it != m_petSkill.end(); ++it)
    {
        PetSkill *skill = it->second;
        PetSkillConfig skillConfig;
        if (skill != NULL && PetConfigManager::Instance().GetPetSkill(skill->GetSkillID(), skillConfig)
            && skillConfig.IsDanceSkill(skillConfig.GetType()))
        {
            skillList.push_back(skill);
        }
    }
}

void Pet::GetActivatedDanceSkillList(std::list<PetSkill *> &skillList) const
{
    for (std::map<SkillID, PetSkill *>::const_iterator it = m_petSkill.begin();
        it != m_petSkill.end(); ++it)
    {
        PetSkill *skill = it->second;
        PetSkillConfig skillConfig;
        if (skill != NULL && PetConfigManager::Instance().GetPetSkill(skill->GetSkillID(), skillConfig)
            && skillConfig.IsDanceSkill(skillConfig.GetType()))
        {
            if (IsActivatedSkill(*skill))
                skillList.push_back(skill);
        }
    }
}

int Pet::GetActivatedSkillCount() const
{
    int nCount = 0;

    std::list<PetSkill *> skillList;
    GetActivatedDanceSkillList(skillList);

    nCount += (int)skillList.size();

    skillList.clear();
    GetFightSkillList(skillList);

    nCount += (int)skillList.size();

    return nCount;
}

void Pet::GetFightSkillList(std::list<PetSkill *> &skillList) const
{
    for (std::map<SkillID, PetSkill *>::const_iterator it = m_petSkill.begin();
        it != m_petSkill.end(); ++it)
    {
        PetSkill *skill = it->second;
        PetSkillConfig skillConfig;
        if (skill != NULL && PetConfigManager::Instance().GetPetSkill(skill->GetSkillID(), skillConfig)
            && skillConfig.IsFightSkill(skillConfig.GetType()))
        {
            skillList.push_back(skill);
        }
    }
}

void Pet::ClearSkill()
{
    while (!m_petSkill.empty())
    {
        PetSkill *skill = m_petSkill.begin()->second;
        delete skill;
        m_petSkill.erase(m_petSkill.begin());
    }
    m_petSkill.clear();

    for (std::map<int, SkillID>::iterator it = m_petSkillIndex.begin();
        it != m_petSkillIndex.end(); ++it)
    {
        it->second = 0;
    }
}

void Pet::ClearSkillExclude(const std::set<SkillID> &excludeSkillList)
{
    if (excludeSkillList.empty())
    {
        ClearSkill();
    }
    else
    {
        std::map<SkillID, PetSkill *>::iterator it = m_petSkill.begin();
        while (it != m_petSkill.end())
        {
            SkillID nSkillID = it->first;
            PetSkill *skill = it->second;
            if (skill != NULL && excludeSkillList.find(nSkillID) == excludeSkillList.end())
            {
                m_petSkillIndex[skill->GetSkillIndex()] = 0;
                delete skill;
                m_petSkill.erase(it++);
            }
            else
            {
                ++it;
            }
        }
    }
}

void Pet::Init(unsigned int nPetID, unsigned char nState, const std::string &strName, unsigned char nPotentialID,
    unsigned short nLevel, unsigned int nExp, unsigned short nEndurance, unsigned int nEnduranceRecoveryTime,
    unsigned char nAttributeType, unsigned char nQualityType, unsigned char nEvolutionTimes,
    unsigned short nPetTypeID, const std::string &strSkillList, unsigned int nResetSkillTime, int nResetSkillTimes)
{
    SetID(nPetID);
    SetState(nState);
    SetName(strName);
    SetAttributeType(nAttributeType);
    SetTypeID(nPetTypeID);
    SetPotentialID(nPotentialID);
    SetQualityType(nQualityType);
    SetEvolutionTimes(nEvolutionTimes);
    SetLevel(nLevel);
    SetExp(nExp);
    SetEndurance(nEndurance);
    SetEnduranceRecoveryTime(nEnduranceRecoveryTime);
    LoadSkill(strSkillList);
    SetResetSkillTime(nResetSkillTime);
    SetResetSkillTimes(nResetSkillTimes);

    m_nMaxEndurance = ResetMaxEndurance();
}

void Pet::Init(const PetDB &petDb)
{
    Init(petDb.nID, petDb.nState, petDb.strName, petDb.nPotentialID,
        petDb.nLevel, petDb.nExp, petDb.nEndurance, petDb.nEnduranceRecoveryTime,
        petDb.nAttributeType, petDb.nQualityType, petDb.nEvolutionTimes,
        petDb.nTypeID, petDb.strSkillList, petDb.nResetSkillTime, petDb.nResetSkillTimes);
}

void Pet::ToPetDB(PetDB &petDb)
{
    petDb.nID = GetID();                         
    petDb.nState = GetState();                   
    petDb.nLevel = GetLevel();                   
    petDb.nExp = GetExp();                       
    petDb.nQualityType = GetQualityType();       
    petDb.nEvolutionTimes = GetEvolutionTimes(); 
    petDb.nAttributeType = GetAttributeType();   
    petDb.nTypeID = GetTypeID();                 
    petDb.nPotentialID = GetPotentialID();       
    petDb.strName = GetName();                   
    petDb.nEndurance = GetEndurance();           
    petDb.nEnduranceRecoveryTime = GetEndurance();
    petDb.nResetSkillTime = GetResetSkillTime();
    petDb.nResetSkillTimes = GetResetSkillTimes();
    SaveSkill(petDb.strSkillList);
}


void Pet::LoadSkill(const std::string &strSkillList)
{
    // skill list format: [index, id, level, value]|[index, id, level, value]
    std::list<std::string> listSkill;
    Split(strSkillList, std::string("|"), listSkill);

    for (std::list<std::string>::iterator it = listSkill.begin(); it != listSkill.end(); ++it)
    {
        std::vector<int> vecSkillTuple;
        std::string strSkill = *it;
        if (Split(strSkill, std::string(","), vecSkillTuple))
        {
            PetSkill *skill = new PetSkill();
            skill->SetSkillIndex(vecSkillTuple[0]);
            skill->SetSkillID((SkillID)vecSkillTuple[1]);
            skill->SetLevel(vecSkillTuple[2]);
            skill->SetSkillValue(vecSkillTuple[3]);

            m_petSkillIndex[skill->GetSkillIndex()] = skill->GetSkillID();
            m_petSkill[skill->GetSkillID()] = skill;
        }
    }
}

void Pet::SaveSkill(std::string &strSkillList)
{
    std::list<std::string> listSkill;
    for (std::map<SkillID, PetSkill *>::iterator it = m_petSkill.begin(); it != m_petSkill.end(); ++it)
    {
        PetSkill *skill = it->second;
        if (skill != NULL && skill->GetSkillID() > 0)
        {
            std::vector<int> vecSkillTuple;
            vecSkillTuple.push_back(skill->GetSkillIndex());
            vecSkillTuple.push_back((int)skill->GetSkillID());
            vecSkillTuple.push_back(skill->GetLevel());
            vecSkillTuple.push_back(skill->GetSkillValue());

            std::string strSkill;
            ConcatToString(vecSkillTuple, std::string(","), strSkill);
            listSkill.push_back(strSkill);
        }
    }

    ConcatToString(listSkill, std::string("|"), strSkillList);
}


