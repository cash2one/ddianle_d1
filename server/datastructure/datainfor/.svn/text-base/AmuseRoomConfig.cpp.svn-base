#include "AmuseRoomConfig.h"
#include "./FileStream.h"


/////³¡¾°/////
bool CAmuseRoomSceneConfig::Save(CFileStream& file)
{
    file.WriteInt(m_nSceneID);

    file.WriteUShort((unsigned short)m_strIconName.size());
    file.WriteString(m_strIconName);

    file.WriteUShort((unsigned short)m_strUnityName.size());
    file.WriteString(m_strUnityName);

    file.WriteUShort((unsigned short)m_strSceneName.size());
    file.WriteString(m_strSceneName);

    return true;
}

bool CAmuseRoomSceneConfig::Load(CFileStream& file)
{
    file.ReadInt(m_nSceneID) ;

    unsigned short nSize = 0 ;
    file.ReadUShort(nSize) ;
    file.ReadString(m_strIconName, nSize) ;

    file.ReadUShort(nSize) ;
    file.ReadString(m_strUnityName, nSize) ;

    file.ReadUShort(nSize) ;
    file.ReadString(m_strSceneName, nSize) ;

    return true;
}


/////¶¯×÷/////
bool CAmuseRoomPoseConfig::Save( CFileStream& file)
{
    file.WriteInt(m_nPoseID);

    file.WriteUShort((unsigned short)m_strPoseName.size());
    file.WriteString(m_strPoseName);

    file.WriteUShort((unsigned short)m_strBoyAnimationName.size());
    file.WriteString(m_strBoyAnimationName);

    file.WriteUShort((unsigned short)m_strGirlAnimationName.size());
    file.WriteString(m_strGirlAnimationName);

    return true;
}

bool CAmuseRoomPoseConfig::Load( CFileStream& file )
{
    file.ReadInt(m_nPoseID) ;

    unsigned short nSize = 0 ;
    file.ReadUShort(nSize) ;
    file.ReadString(m_strPoseName, nSize) ;

    file.ReadUShort(nSize) ;
    file.ReadString(m_strBoyAnimationName, nSize) ;

    file.ReadUShort(nSize) ;
    file.ReadString(m_strGirlAnimationName, nSize) ;

    return true;
}

