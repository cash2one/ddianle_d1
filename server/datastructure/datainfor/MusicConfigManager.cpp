#include "MusicConfigManager.h"
#include "MusicConfig.h"
#include "FileStream.h"
#include "../../socket/Windefine.h"
#include "../../socket/Utility.h"


MusicConfigManager::MusicConfigManager(void)
{	
}

MusicConfigManager::~MusicConfigManager(void)
{
    Destroy();
}

bool MusicConfigManager::Add(MusicConfig* pMusicInfo)
{
    if (pMusicInfo != NULL && pMusicInfo->m_nMusicID > 0)
    {
        m_MusicMap.insert(map<unsigned short, MusicConfig*>::value_type(pMusicInfo->m_nMusicID, pMusicInfo));
        return true;
    }

    if (pMusicInfo != NULL)
    {
        delete pMusicInfo;
        pMusicInfo = NULL;
    }

    return false;
}

unsigned short MusicConfigManager::GetNumber() const
{
    return (unsigned short )m_MusicMap.size();
}

bool MusicConfigManager::LoadMusicInfo(CFileStream&file)
{
    Destroy();

    bool bReturnValue = true;
    if(file.IsOpen()){
        unsigned short ulNumber = 0;
        file.ReadUShort(ulNumber);
        unsigned short i;
        if(ulNumber>0){
            for( i=0;i<ulNumber;i++){
                MusicConfig *pMusicInfo = new MusicConfig;
                if(pMusicInfo != NULL){
                    pMusicInfo->Load(file);
                    Add(pMusicInfo);
                }
            }
        }
        //file.close();
    }
    else{
        bReturnValue = false;
    }

    return bReturnValue;
}

bool MusicConfigManager::SaveMusicInfo(CFileStream&file)
{
    if(!file.IsOpen())
        return false;

    unsigned short ulNumber = (unsigned short)m_MusicMap.size();
    file.WriteUShort(ulNumber);

    std::map<unsigned short,MusicConfig*>::const_iterator it = m_MusicMap.begin();
    MusicConfig*pMusicInfo=NULL;
    for(;it != m_MusicMap.end();it++)
    {
        pMusicInfo = it->second;
        if(pMusicInfo != NULL){
            pMusicInfo->Save(file);
        }
    }

    return true;
}

//根据ID得到对应的CMusicInfo
MusicConfig * MusicConfigManager::GetByID(unsigned short nType) const
{
    if (nType > 0){
        std::map<unsigned short,MusicConfig*>::const_iterator it= m_MusicMap.find(nType);
        if(it == m_MusicMap.end()) {
            return NULL;
        }
        else {
            return it->second;
        }
    }
    return NULL;;
}

MusicConfig * MusicConfigManager::GetAnyFresherMusic() const
{
    std::map<unsigned short,MusicConfig*>::const_iterator it = m_MusicMap.begin();
    MusicConfig*pMusicInfo = NULL;
    for(; it != m_MusicMap.end(); ++it)
    {
        pMusicInfo = it->second;
        if(pMusicInfo != NULL && pMusicInfo->m_bIsFresher)
        {
            return pMusicInfo;
        }
    }
    return NULL;
}

//释放所有对象
void MusicConfigManager::Destroy()
{
    std::map<unsigned short,MusicConfig*>::const_iterator it = m_MusicMap.begin();
    while(it != m_MusicMap.end())
    {
        delete (MusicConfig*)it->second;
        it++;
    };
    m_MusicMap.clear();
}


//end file


