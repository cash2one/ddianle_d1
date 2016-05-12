#include "PetEggConfigManager.h"
#include "PetIncubatorInfo.h"
#include "PetEggBaseInfo.h"
#include "RandomPetCollect.h"

#include "../../../socket/Log.h"

#include "../../../datastructure/datainfor/FileStream.h"

extern std::string g_sPath;

#define PET_EGG_FILE_NAME   "data/bytes/PetEgg.bytes"


PetEggConfigManager::PetEggConfigManager(void)
{
}

PetEggConfigManager::~PetEggConfigManager(void)
{
}

bool PetEggConfigManager::Initialize()
{
    LoadEggInfoFromFile();

    return CheckConfig();
}

void PetEggConfigManager::AddRandomPetInfo(unsigned int nEggTypeID, RandomPetInfo &rInfo, int nRate)
{
    std::map<unsigned int, RandomPetCollect>::iterator itr = m_mapRandomPetInfos.find(nEggTypeID);

    if (itr != m_mapRandomPetInfos.end() )
    {
        if (!itr->second.AddRandomPetInfo(rInfo, nRate) )
        {
            WriteLog(LOGLEVEL_ERROR, "Add random pet info error.");
        }
    }
    else
    {
        RandomPetCollect collect;

        if (collect.AddRandomPetInfo(rInfo, nRate) )
        {
            m_mapRandomPetInfos.insert(std::make_pair(nEggTypeID, collect) );
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "Add random pet info error.");
        }
    }
}

bool PetEggConfigManager::FindEggBaseInfo(unsigned int nEggTypeID) const
{
    std::map<unsigned int, PetEggBaseInfo>::const_iterator itr = m_mapEggInfos.find(nEggTypeID);

    if (itr == m_mapEggInfos.end() )
        return false;

    return true;
}

bool PetEggConfigManager::FindPetList(unsigned int nEggTypeID) const
{
    std::map<unsigned int, RandomPetCollect>::const_iterator itr = m_mapRandomPetInfos.find(nEggTypeID);

    if (itr == m_mapRandomPetInfos.end() )
        return false;

    return true;
}

bool PetEggConfigManager::GetPetIncubatorInfo(unsigned short nLevel, PetIncubatorInfo &rInfo) const
{
    const PetIncubatorInfo* pInfo = GetPetIncubatorInfo(nLevel);

    if (pInfo != NULL)
    {
        rInfo = *pInfo;

        return true;
    }

    return false;
}

const PetIncubatorInfo* PetEggConfigManager::GetPetIncubatorInfo(unsigned short nLevel) const
{
    std::map<unsigned short, PetIncubatorInfo>::const_iterator itr = m_mapIncubatorLevelUpCost.find(nLevel);

    if (itr != m_mapIncubatorLevelUpCost.end() )
        return &(itr->second);

    return NULL;
}

bool PetEggConfigManager::GetEggInfo(unsigned int nEggTypeID, PetEggBaseInfo &rInfo) const
{
    const PetEggBaseInfo* pInfo = GetEggInfo(nEggTypeID);

    if (pInfo != NULL)
    {
        rInfo = *pInfo;

        return true;
    }

    return false;
}

const PetEggBaseInfo* PetEggConfigManager::GetEggInfo(unsigned int nEggTypeID) const
{
    std::map<unsigned int, PetEggBaseInfo>::const_iterator itr = m_mapEggInfos.find(nEggTypeID);

    if (itr != m_mapEggInfos.end() )
        return &(itr->second);

    return NULL;
}

bool PetEggConfigManager::RandOnePet(unsigned int nEggTypeID, RandomPetInfo &rInfo) const
{
    std::map<unsigned int, RandomPetCollect>::const_iterator itr = m_mapRandomPetInfos.find(nEggTypeID);

    if (itr != m_mapRandomPetInfos.end() )
        return itr->second.RandomOnePet(rInfo);

    return false;
}

const RandomPetInfo* PetEggConfigManager::RandOnePet(unsigned int nEggTypeID) const
{
    std::map<unsigned int, RandomPetCollect>::const_iterator itr = m_mapRandomPetInfos.find(nEggTypeID);

    if (itr != m_mapRandomPetInfos.end() )
        return itr->second.RandomOnePet();

    return NULL;
}

void PetEggConfigManager::LoadEggInfoFromFile()
{
    std::string strFileName = g_sPath + PET_EGG_FILE_NAME;

    CFileStream file;
    file.open(strFileName.c_str(), "rb");

    if (file.IsOpen() )
    {
        m_mapIncubatorLevelUpCost.clear();
        m_mapEggInfos.clear();

        unsigned short i = 0;
        unsigned short nCount = 0;

        file.ReadUShort(nCount);
        for (i = 0; i < nCount; ++i)
        {
            PetIncubatorInfo incubatorInfo;

            if (incubatorInfo.LoadInfoFromFile(file) )
            {
                m_mapIncubatorLevelUpCost.insert(std::make_pair(incubatorInfo.GetLevel(), incubatorInfo) );
            }
        }

        file.ReadUShort(nCount);
        for (i = 0; i < nCount; ++i)
        {
            PetEggBaseInfo eggBaseInfo;

            if (eggBaseInfo.LoadInfoFromFile(file) )
            {
                m_mapEggInfos.insert(std::make_pair(eggBaseInfo.GetTypeID(), eggBaseInfo) );
            }
        }

        file.close();
    }
}

bool PetEggConfigManager::CheckConfig() const
{
    if (m_mapEggInfos.size() == 0)
        return false;
    else if (m_mapRandomPetInfos.size() == 0)
        return false;
    else if (m_mapIncubatorLevelUpCost.size() == 0)
        return false;
    else if (m_mapIncubatorLevelUpCost.rbegin()->first != (unsigned short)m_mapIncubatorLevelUpCost.size() )
        return false;

    return true;
}

