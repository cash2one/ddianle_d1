/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\AchievementConfig.h
    author:     

    purpose:    Amusement.bytes配置中相对应的数据结构
*********************************************************************/

#ifndef __AMUSEMENTROOM_CONFIG_H__
#define __AMUSEMENTROOM_CONFIG_H__

#include <string>

class CFileStream;

// 开放场景场景配置
class CAmuseRoomSceneConfig
{
public:
    CAmuseRoomSceneConfig(){};
    ~CAmuseRoomSceneConfig(){};

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    int             m_nSceneID ;
    std::string     m_strIconName ;
    std::string     m_strUnityName ;
    std::string     m_strSceneName ;
};


// 开放场景动作配置
class CAmuseRoomPoseConfig
{
public:
    CAmuseRoomPoseConfig(){};
    ~CAmuseRoomPoseConfig(){};

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    int m_nPoseID ;
    std::string m_strPoseName ;
    std::string m_strBoyAnimationName ;
    std::string m_strGirlAnimationName ;
};

#endif
