#include "./SceneConfig.h"
#include "./FileStream.h"
#include "../../socket/Utility.h"
#include "../../socket/Windefine.h"

SceneConfig::SceneConfig(void)
{
    m_nSceneID = 0;
    m_strSceneName = "";
    m_strChooseMusicIcon = "";
    m_strRoomSceneTexture = "";
    m_bIsFresherScene = true;
}

SceneConfig::~SceneConfig(void)
{
}

bool SceneConfig::Load(CFileStream&file)
{
    file.ReadUChar(m_nSceneID);

    unsigned short nSize = 0;

    file.ReadUShort(nSize);
    file.ReadString(m_strSceneName,nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strChooseMusicIcon,nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strRoomSceneTexture,nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strSceneStage,nSize);

    file.ReadBool(m_bIsFresherScene);

    return true;
};

bool SceneConfig::Save(CFileStream&file)
{
    file.WriteUChar(m_nSceneID);

    file.WriteUShort((unsigned short)m_strSceneName.length());
    file.WriteString(m_strSceneName);

    file.WriteUShort((unsigned short)m_strChooseMusicIcon.length());
    file.WriteString(m_strChooseMusicIcon);

    file.WriteUShort((unsigned short)m_strRoomSceneTexture.length());
    file.WriteString(m_strRoomSceneTexture);

    file.WriteUShort((unsigned short)m_strSceneStage.length());
    file.WriteString(m_strSceneStage);

    file.WriteBool(m_bIsFresherScene);

    return true;
};


//end file



