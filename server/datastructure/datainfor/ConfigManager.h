/********************************************************************
    created:    2010-11-09
    filename:   DataStructure\datainfor\ConfigManager.h
    author:     mei jiuhua

    purpose:    这个文件作为所有数据对外的总接口，各功能定义在这里，
                一般情况下，一个数据文件对应一个manager类，加到这个
                类中作为成员变量，并增加相关代码,请参考CGeneInfoManager
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
    //初始化所有数据，szPath为路径，如果路径为空则从当前目录下读取
    static ConfigManager& Instance();

    // 物品
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

    // 成就
    AchievementConfigManager & GetAchievementManager();

    // 经验
    ExpConfigManager& GetExpMgr();

    // 开放场景
    CAmuseRoomConfigManager& GetAmuseMgr();

    // 特效
    EffectManager &GetEffectConfigMgr();

    // constellation
    ConstellationConfigManager &GetConstellationMgr();

    // horn
    EffectManager& GetHornMgr();

    // 魔法神灯
    CMagicLampConfigMgr & GetMagicLampConfigMgr();

    // 结婚房间
    WeddingRoomConfigMgr & GetWeddingRoomConfigMgr();

	// 幸运魔法阵
	MagicArrayConfigManager & GetMagicArrayConfigMgr();

    // 广场大妈
    BigMamaConfigManager & GetBigMamaConfigMgr();

    // 
    CDanceGroupFairyLandCfgMgr& GetDanceGroupFairyLandCfgMgr();

    // 初始化数据
    bool InitAllData(const char* szPath = NULL,bool bServer = false);

    // 销毁数据
    void DestroyAllData();

protected:
    // 道具部分,
    ItemConfigManager m_itemInfoMgr;

    // 音乐部分,
    MusicConfigManager m_musicInfoMgr;

    // 音乐关卡部分,
    MusicStageManager m_musicStageMgr;

    // 场景和模式部分,
    SceneMapManager m_aSceneModelMgr;

    // 任务部分
    CQuestMan m_QuestMgr;
    CQuestManNew m_QuestNewMgr;

    // 新手7日目标
    CQuestMan m_FresherQuestMgr;//7日目标任务从lua加载

    // 成就
    AchievementConfigManager m_AchievementMgr;

    //经验
    ExpConfigManager m_ExpDataInfoMgr;

    //开放场景
    CAmuseRoomConfigManager m_AmuseDataInfoMgr;

    //特效表数据加载
    EffectManager m_effectInfoMgr;

    // 星座
    ConstellationConfigManager m_ConstellationMgr;

    // 喇叭
    EffectManager  m_HornManager;

    // Boss装备
    CMagicLampConfigMgr m_MagicLampConfigMgr;

    // 创建婚房
    WeddingRoomConfigMgr m_WeddingRoomConfigMgr;

	// 幸运魔法阵
	MagicArrayConfigManager	m_MagicArrayConfigMgr;

    // 广场大妈
    BigMamaConfigManager m_BigMamaConfigMgr;

private:
    bool m_bServer;     //表示是否在服务器上
    bool m_bEditer;     //表示是否编辑器
};

#define g_BigMamaConfigManager ConfigManager::Instance().GetBigMamaConfigMgr()

#endif//__DATAMANAGER_H__



//end file


