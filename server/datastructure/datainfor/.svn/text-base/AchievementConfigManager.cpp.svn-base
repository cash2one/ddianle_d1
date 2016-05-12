#include "AchievementConfigManager.h"
#include "FileStream.h"

AchievementConfigManager::AchievementConfigManager()
{
}

AchievementConfigManager::~AchievementConfigManager()
{
    Destroy();
}

bool AchievementConfigManager::Add(AchievementConfig *pAchievementInfo)
{
    if (pAchievementInfo != NULL && pAchievementInfo->m_nAchievementID > 0)
    {
        m_mapAchievement.insert(map<unsigned short, AchievementConfig*>::value_type(pAchievementInfo->m_nAchievementID, pAchievementInfo));
        map<unsigned short, list<AchievementConfig *> >::iterator itr = m_mapAchievementBySeriesID.find(pAchievementInfo->m_nSeriesID);
        if (itr != m_mapAchievementBySeriesID.end()) 
        {
            itr->second.push_back(pAchievementInfo);	
        }
        else 
        {
            list<AchievementConfig *> listAchievement;
            listAchievement.push_back(pAchievementInfo);
            m_mapAchievementBySeriesID.insert(make_pair(pAchievementInfo->m_nSeriesID, listAchievement));
        }

        return true;
    }

    if (pAchievementInfo != NULL)
    {
        delete pAchievementInfo;
        pAchievementInfo = NULL;
    }

    return false;
}

AchievementConfig * AchievementConfigManager::GetByID(unsigned short nAchievementID) const
{
    if (nAchievementID > 0)
    {
        std::map<unsigned short,AchievementConfig*>::const_iterator it= m_mapAchievement.find(nAchievementID);
        if(it != m_mapAchievement.end()) 
        {
            return it->second;
        }
    }

    return NULL;
}

bool AchievementConfigManager::SaveAchieveInfo(const char *pszFileName)
{
    CFileStream file;
    CFileStream fileTxt;

    file.open(pszFileName,"wb");
    if(!file.IsOpen())
    {
        return false;
    }

    unsigned short ulNumber = (unsigned short)m_mapAchievement.size();
    file.WriteUShort(ulNumber);

    std::map<unsigned short,AchievementConfig*>::const_iterator it = m_mapAchievement.begin();
    AchievementConfig*pAchievementInfo=NULL;
    for(; it != m_mapAchievement.end(); it++)
    {
        pAchievementInfo = it->second;
        if(pAchievementInfo != NULL)
        {
            pAchievementInfo->Save(file);
        }
    }
    file.close();

    return true;
}

bool AchievementConfigManager::LoadAchieveInfo(const char *pszFileName)
{
    Destroy();

    CFileStream file;
    file.open(pszFileName,"rb");
    bool bReturnValue = true;
    if(file.IsOpen())
    {
        unsigned short ulNumber = 0;
        file.ReadUShort(ulNumber);
        unsigned short i;
        if(ulNumber>0)
        {
            for( i=0;i<ulNumber;i++)
            {
                AchievementConfig *pAchievementInfo = new AchievementConfig;
                if(pAchievementInfo != NULL)
                {
                    pAchievementInfo->Load(file);
                    Add(pAchievementInfo);
                }
            }
        }

        file.close();
    }
    else
    {
        bReturnValue = false;
    }

    return bReturnValue;
}

void AchievementConfigManager::Destroy()
{
    std::map<unsigned short,AchievementConfig*>::const_iterator it = m_mapAchievement.begin();
    while(it != m_mapAchievement.end())
    {
        delete (AchievementConfig*)it->second;
        it++;
    };

    m_mapAchievement.clear();
    m_mapAchievementBySeriesID.clear();
}

unsigned short AchievementConfigManager::GetNumber() const 
{ 
    return (unsigned short )m_mapAchievement.size(); 
}



//end file



