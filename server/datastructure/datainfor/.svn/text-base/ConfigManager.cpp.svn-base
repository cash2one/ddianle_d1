#include "ConfigManager.h"
#include "PhotoRoomConfig.h"
#include "../../server/DanceGroupFairyLand/DanceGroupFairyLandCfgMgr.h"

void CheckReturn(string &str)
{
    string::size_type i = str.find("\\n");

    while(i != string::npos)
    {
        str.replace(i,2,"\r\n");
        i = str.find("\\n");
    }
}

ConfigManager::ConfigManager(void)
{
    m_bServer = false;
    m_bEditer = false;

}

ConfigManager::~ConfigManager(void)
{
    DestroyAllData();
}
string GetTxtDataFileName(const char* pszFileName)
{
    string strTemp;
    strTemp = pszFileName;
    int nPos = (int)strTemp.find(".dat");
    if(nPos > 0)
        strTemp.replace(nPos,4,".txt");
    return strTemp;
}

ConfigManager& ConfigManager::Instance()
{
    static ConfigManager stcDataMgr;
    return stcDataMgr;
}

ItemConfigManager & ConfigManager::GetItemConfigManager()
{
    return  m_itemInfoMgr;
}

MusicConfigManager & ConfigManager::GetMusicConfigManager()
{
    return m_musicInfoMgr;
}

MusicStageManager & ConfigManager::GetMusicStageManager()
{
    return m_musicStageMgr;
}

SceneMapManager & ConfigManager::GetSceneModeManager()
{
    return m_aSceneModelMgr;
}

CQuestMan & ConfigManager::GetQuestManager()
{
    return m_QuestMgr;
}
CQuestMan & ConfigManager::GetFresherQuestManager()
{
    return m_FresherQuestMgr;
}

CQuestManNew & ConfigManager::GetQuestNewManager()
{
    return m_QuestNewMgr;
}

AchievementConfigManager & ConfigManager::GetAchievementManager()
{
    return m_AchievementMgr;
}

ExpConfigManager& ConfigManager::GetExpMgr()
{
    return m_ExpDataInfoMgr;
}

CAmuseRoomConfigManager& ConfigManager::GetAmuseMgr()
{
    return m_AmuseDataInfoMgr;
}

EffectManager & ConfigManager::GetEffectConfigMgr()
{
    return m_HornManager;
}

ConstellationConfigManager &  ConfigManager::GetConstellationMgr()
{
    return m_ConstellationMgr;
}

EffectManager& ConfigManager::GetHornMgr()
{
    return m_HornManager;
}

CMagicLampConfigMgr & ConfigManager::GetMagicLampConfigMgr()
{
    return m_MagicLampConfigMgr;
}

WeddingRoomConfigMgr & ConfigManager::GetWeddingRoomConfigMgr()
{
    return m_WeddingRoomConfigMgr;
}

MagicArrayConfigManager & ConfigManager::GetMagicArrayConfigMgr()
{
	return m_MagicArrayConfigMgr;
}

BigMamaConfigManager & ConfigManager::GetBigMamaConfigMgr()
{
    return m_BigMamaConfigMgr;
}


bool ConfigManager::InitAllData(const char* szPath,bool bServer)
{
    m_bServer = bServer;
    bool bReturnValue = true;
    string sPath;
    if(szPath != NULL)
    {
        sPath = szPath;
    }

    // load all data
    if(m_bServer || m_bEditer)
    {
        string strFileName;

        // item server
        strFileName = sPath + ItemFileName;
        if(!m_itemInfoMgr.LoadItemInfo( strFileName.c_str() ))
        {
            bReturnValue = false;
        }

        // extend binary data
        strFileName = sPath + ExtendItemFileName;
        if (!m_itemInfoMgr.LoadExtendItemBinData(strFileName.c_str())) {
            bReturnValue = false;
        }

        // extend item
        strFileName = sPath + ExtendItemFileName;
        if (!m_itemInfoMgr.LoadExtendItemInfo(strFileName.c_str())) {
            bReturnValue = false;
        }

        CFileStream file;
        strFileName = sPath + MusicFileName;
        file.open(strFileName.c_str(),"rb");
        if(file.IsOpen())
        {
            bReturnValue  = true;

            bReturnValue &= m_musicInfoMgr.LoadMusicInfo( file );
            bReturnValue &= m_musicStageMgr.LoadMusicStage( file );
            bReturnValue &= m_aSceneModelMgr.LoadScene( file );
            bReturnValue &= m_aSceneModelMgr.LoadMode( file );
        }
        file.close();

        // load quest data
        strFileName = sPath + QuestFileName;
        if(!m_QuestMgr.LoadQuest(strFileName.c_str())){
            bReturnValue = false;
        }

        strFileName = sPath + QuestFileNameNew;
        if (!m_QuestNewMgr.LoadQuest(strFileName.c_str()))
        {
            bReturnValue = false;
        }

        strFileName = sPath + QuestFileNameNew;
        if (!m_QuestNewMgr.LoadQuestBinData(strFileName.c_str()))
        {
            bReturnValue = false;
        }

        // load quest binary data
        strFileName = sPath + QuestFileName;
        if(!m_QuestMgr.LoadQuestBinaryData(strFileName.c_str())){
            bReturnValue = false;
        }

        //load Exp data
        strFileName = sPath + ExpFileName ;
        if(!m_ExpDataInfoMgr.LoadExpInfo(strFileName.c_str()))
        {
            bReturnValue = false;
        }

        strFileName = sPath + AchievementFileName;
        if (!m_AchievementMgr.LoadAchieveInfo(strFileName.c_str()))
        {
            bReturnValue = false;
        }

        strFileName = sPath + PhotoStudioInfo;
        if (!PhotoRoomConfigManager::Instance().LoadInfo(strFileName.c_str()))
        {
            bReturnValue = false;
        }

        strFileName = sPath + AmuseDataInfoName;
        if (!m_AmuseDataInfoMgr.LoadAmuseInfo(strFileName.c_str()))
        {
            bReturnValue = false;
        }

        strFileName = sPath + ConstellationFileName;
        if (!m_ConstellationMgr.Load(strFileName.c_str()))
        {
            bReturnValue = false;
        }

        strFileName = sPath + GeneInfoFileName;
        if (!m_HornManager.Load(strFileName))
        {
            bReturnValue = false;
        }

        strFileName = sPath + TransformFileName;
        if (!m_itemInfoMgr.LoadTransformInfo(strFileName.c_str()))
        {
            bReturnValue = false;
        }

        strFileName = sPath + MagicLampFileName;
        if (!m_MagicLampConfigMgr.LoadMagicLampConfig(strFileName.c_str()))
        {
            bReturnValue = false;
        }	 

        strFileName = sPath + WeddingRoomDataInfoName;
        if ( !m_WeddingRoomConfigMgr.LoadWeddingRoomConfig(strFileName.c_str()))
        {
            bReturnValue = false;
        }

		strFileName = sPath + MagicArrayDataFileName;
		if (!m_MagicArrayConfigMgr.LoadConfig(strFileName.c_str()))
		{
            bReturnValue = false;
		}

        strFileName = sPath + BigMamaDataFileName;
        if (!m_BigMamaConfigMgr.LoadConfig(strFileName.c_str()))
        {
            bReturnValue = false;
        }

        CDanceGroupFairyLandCfgMgr::Instance().Initialize();
    }

    return bReturnValue;
};

void ConfigManager::DestroyAllData()
{
    //请在这里加入清除数据的代码
    m_itemInfoMgr.Destroy();
    m_AchievementMgr.Destroy();
    m_musicInfoMgr.Destroy();
    m_musicStageMgr.Destroy();
    m_aSceneModelMgr.Destroy();
    m_QuestMgr.Destroy();
    m_QuestNewMgr.Destroy();
    m_ConstellationMgr.Destroy();
    m_HornManager.Destroy();
    m_MagicLampConfigMgr.Destroy();
    m_effectInfoMgr.Destroy();
    m_MagicLampConfigMgr.Destroy();
}

#include <algorithm>

class ItemInfor_Comp
{
public :
    bool operator()(const ItemConfig*pFirst,const ItemConfig*pSecond)
    {
        return pFirst->m_nType <= pSecond->m_nType;
    }
};

class MusicInfor_Comp
{
public :
    bool operator()(const MusicConfig*pFirst,const MusicConfig*pSecond)
    {
        return pFirst->m_nType <= pSecond->m_nType;
    }
};


//end file


