/********************************************************************
    created:    2010-11-09
    filename:   DataStructure\datainfor\ConfigManager.h
    author:     mei jiuhua

    purpose:    ����ļ���Ϊ�������ݶ�����ܽӿڣ������ܶ��������
                һ������£�һ�������ļ���Ӧһ��manager�࣬�ӵ����
                ������Ϊ��Ա��������������ش���,��ο�CGeneInfoManager
*********************************************************************/


#ifndef __CONFIGMANAGER_H__
#define __CONFIGMANAGER_H__

typedef unsigned long		DWORD;
typedef void* LPVOID;

#include "ItemConfigManager.h"
#include "MusicConfigManager.h"
#include "MusicStageManager.h"
#include "AchievementConfigManager.h"
#include "SceneMapManager.h"
#include "../../nsytworld/QuestMan.h"
#include "ExpConfigManager.h"
#include "AmuseRoomConfigManager.h"
#include "EffectConfigManager.h"
#include "ConstellationConfigManager.h"
#include "EffectConfigManager.h"
#include "MagicLampConfigMgr.h"
#include "WeddingRoomConfigMgr.h"
#include "PhotoRoomConfigManager.h"
#include "MagicArrayConfigManager.h"
#include "../../server/dancegroup/BigMamaConfigManager.h"

#define ExtendItemFileName          "data/bytes/ItemExtend.bytes"
#define ItemFileName                "data/bytes/iteminfo.bytes"
#define MusicFileName               "data/bytes/musicinfo.bytes"
#define QuestFileName               "data/bytes/quest.bytes"	
#define QuestFileNameNew            "data/bytes/questnew.bytes"
#define AchievementFileName         "data/bytes/achievement.bytes"
#define ExpFileName                 "data/bytes/LevExp.bytes"
#define PhotoStudioInfo             "data/bytes/PhotoStudioInfo.bytes"
#define AmuseDataInfoName           "data/bytes/Amusement.bytes"
#define FresherQuestFileName        "data/bytes/FresherQuest.bytes"
#define ConstellationFileName       "data/bytes/Constellation.bytes"
#define GeneInfoFileName            "data/bytes/EffectInfo.bytes"
#define TransformFileName           "data/bytes/Transform.bytes"
#define MagicLampFileName           "data/bytes/MagicLamp.bytes"
#define TransformAniFileName        "data/bytes/TransformAni.bytes"
#define MagicArrayDataFileName	    "data/bytes/MagicArray.bytes"
#define BigMamaDataFileName         "data/bytes/BigMamaServer.bytes"

class CDanceGroupFairyLandCfgMgr;

class ConfigManager
{
protected:
    ConfigManager(void);
    ~ConfigManager(void);

public:
    //��ʼ���������ݣ�szPathΪ·�������·��Ϊ����ӵ�ǰĿ¼�¶�ȡ
    static ConfigManager& Instance();

    // ��Ʒ
    ItemConfigManager & GetItemConfigManager();

    // music
    MusicConfigManager & GetMusicConfigManager();

    // music stage
    MusicStageManager & GetMusicStageManager();

    // scene
    SceneMapManager & GetSceneModeManager();

    // quest
    CQuestMan & GetQuestManager();

    // fresher quest
    CQuestMan & GetFresherQuestManager();

    // new quest
    CQuestManNew & GetQuestNewManager();

    // �ɾ�
    AchievementConfigManager & GetAchievementManager();

    // ����
    ExpConfigManager& GetExpMgr();

    // ���ų���
    CAmuseRoomConfigManager& GetAmuseMgr();

    // ��Ч
    EffectManager &GetEffectConfigMgr();

    // constellation
    ConstellationConfigManager &GetConstellationMgr();

    // horn
    EffectManager& GetHornMgr();

    // ħ�����
    CMagicLampConfigMgr & GetMagicLampConfigMgr();

    // ��鷿��
    WeddingRoomConfigMgr & GetWeddingRoomConfigMgr();

	// ����ħ����
	MagicArrayConfigManager & GetMagicArrayConfigMgr();

    // �㳡����
    BigMamaConfigManager & GetBigMamaConfigMgr();

    // 
    CDanceGroupFairyLandCfgMgr& GetDanceGroupFairyLandCfgMgr();

    // ��ʼ������
    bool InitAllData(const char* szPath = NULL,bool bServer = false);

    // ��������
    void DestroyAllData();

protected:
    // ���߲���,
    ItemConfigManager m_itemInfoMgr;

    // ���ֲ���,
    MusicConfigManager m_musicInfoMgr;

    // ���ֹؿ�����,
    MusicStageManager m_musicStageMgr;

    // ������ģʽ����,
    SceneMapManager m_aSceneModelMgr;

    // ���񲿷�
    CQuestMan m_QuestMgr;
    CQuestManNew m_QuestNewMgr;

    // ����7��Ŀ��
    CQuestMan m_FresherQuestMgr;//7��Ŀ�������lua����

    // �ɾ�
    AchievementConfigManager m_AchievementMgr;

    //����
    ExpConfigManager m_ExpDataInfoMgr;

    //���ų���
    CAmuseRoomConfigManager m_AmuseDataInfoMgr;

    //��Ч�����ݼ���
    EffectManager m_effectInfoMgr;

    // ����
    ConstellationConfigManager m_ConstellationMgr;

    // ����
    EffectManager  m_HornManager;

    // Bossװ��
    CMagicLampConfigMgr m_MagicLampConfigMgr;

    // �����鷿
    WeddingRoomConfigMgr m_WeddingRoomConfigMgr;

	// ����ħ����
	MagicArrayConfigManager	m_MagicArrayConfigMgr;

    // �㳡����
    BigMamaConfigManager m_BigMamaConfigMgr;

private:
    bool m_bServer;     //��ʾ�Ƿ��ڷ�������
    bool m_bEditer;     //��ʾ�Ƿ�༭��
};

#define g_BigMamaConfigManager ConfigManager::Instance().GetBigMamaConfigMgr()

#endif//__DATAMANAGER_H__



//end file


