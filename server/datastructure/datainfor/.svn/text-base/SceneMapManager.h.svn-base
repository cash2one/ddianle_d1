#ifndef __SMMANAGER_H_
#define __SMMANAGER_H_

#include <map>
#include <vector>
#include "./SceneConfig.h"
#include "./ModeConfig.h"

// 场景数据管理器
class SceneMapManager
{
public:
    SceneMapManager(void);
    virtual ~SceneMapManager(void);

    //从文件里读取所有unit的信息
    bool LoadMode(CFileStream&file);
    bool SaveMode(CFileStream&file);

    bool LoadScene(CFileStream&file);
    bool SaveScene(CFileStream&file);

    bool AddScene(SceneConfig* pSceneInfo);
    bool AddMode(ModeConfig* pModelInfo);

    SceneConfig* GetSceneInfo(unsigned char nSceneID) const;
    ModeConfig* GetModeInfo(unsigned char nModelID) const;

    unsigned char RandomScene(bool bOnlyFresherScene);
    unsigned char RandomMode(unsigned char nExcludeModelID = 0);

    void Destroy();

protected:
    void _DestroyScene();
    void _DestroyMode();

public:
    typedef std::map<unsigned char, SceneConfig*> SceneMap;  // key->scene id, value->scene
    typedef std::map<unsigned char, ModeConfig*> ModeMap;

private:
    SceneMap                    m_SceneMap;       // all scene map
    std::vector<unsigned char>  m_AllSceneID;     // all scene ID
    std::vector<unsigned char>  m_FresherSceneID; // fresher installer scene ID
    ModeMap                     m_ModeMap;


};


#endif//__MusicinfoMANAGER_H_


//end file


