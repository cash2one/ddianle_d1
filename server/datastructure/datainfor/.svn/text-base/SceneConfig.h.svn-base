/********************************************************************
created:	2012/03/01
author:		meijiuhua

*********************************************************************/

#ifndef __SCENEINFO_H__
#define __SCENEINFO_H__

#ifdef WIN32
#pragma warning(disable:4018)
#pragma warning(disable:4100)
#endif

#include <string>

class CFileStream;


class SceneConfig
{
public:
    SceneConfig(void);
    ~SceneConfig(void);

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned char   m_nSceneID;             //id编号
    std::string     m_strSceneName;
    std::string     m_strChooseMusicIcon;
    std::string     m_strRoomSceneTexture;
    std::string     m_strSceneStage;
    bool            m_bIsFresherScene;      //是否是新手场景：用于小包场景筛选
};

#endif//__ITEMINFO_H__


//end file


