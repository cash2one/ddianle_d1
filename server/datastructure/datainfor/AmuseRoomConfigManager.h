/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\AmuseRoomConfigManager.h
    author:     

    purpose:    对开放场景配置数据进行管理
*********************************************************************/

#ifndef __AMUSEMENTROOM_CONFIG_MGR_H__
#define __AMUSEMENTROOM_CONFIG_MGR_H__

#include "AmuseRoomConfig.h"
#include <map>
#include <vector>


class CAmuseRoomConfigManager
{
public:
    CAmuseRoomConfigManager();
    ~CAmuseRoomConfigManager();

public:
    void AddAmuseSceneInfo(CAmuseRoomSceneConfig* pSceneInfo);
    void AddAmusePoseInfo(CAmuseRoomPoseConfig* pPoseInfo);

    CAmuseRoomSceneConfig* GetAmuseSceneInfo(int nSceneID);
    CAmuseRoomPoseConfig* GetAmusePoseInfo(int nPoseID);

    bool SaveAmuseInfo(const char* pFileName);
    bool LoadAmuseInfo(const char* pFileName);

    bool ExistAmuseScene(int nSceneID) const;
    int RandomAmuseScene() const;

public:
    std::map<int, CAmuseRoomSceneConfig*> m_AmuseSceneInfo;
    std::map<int, CAmuseRoomPoseConfig*>  m_AmusePoseInfo;
    std::vector<int>                    m_vecAmuseSceneID;
};

#endif

