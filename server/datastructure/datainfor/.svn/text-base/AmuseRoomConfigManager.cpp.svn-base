#include "AmuseRoomConfigManager.h"
#include "./FileStream.h"
#include "../../server/util/CommonFunc.h"

CAmuseRoomConfigManager::CAmuseRoomConfigManager()
{

}

CAmuseRoomConfigManager::~CAmuseRoomConfigManager()
{
    for( std::map<int, CAmuseRoomSceneConfig*>::iterator it_Scene = m_AmuseSceneInfo.begin(); it_Scene != m_AmuseSceneInfo.end(); ++it_Scene )
    {
        delete it_Scene->second;
    }

    for( std::map<int, CAmuseRoomPoseConfig*>::iterator it_Action = m_AmusePoseInfo.begin(); it_Action != m_AmusePoseInfo.end(); ++it_Action )
    {
        delete it_Action->second;
    }

    m_AmuseSceneInfo.clear();
    m_AmusePoseInfo.clear();
}

void CAmuseRoomConfigManager::AddAmuseSceneInfo(CAmuseRoomSceneConfig* pSceneInfo)
{
    if ( pSceneInfo != NULL )
    {
        m_AmuseSceneInfo.insert( std::make_pair( pSceneInfo->m_nSceneID, pSceneInfo ) );
        m_vecAmuseSceneID.push_back( pSceneInfo->m_nSceneID );
    }
}

void CAmuseRoomConfigManager::AddAmusePoseInfo(CAmuseRoomPoseConfig* pPoseInfo)
{
    if ( pPoseInfo != NULL )
    {
        m_AmusePoseInfo.insert( std::make_pair( pPoseInfo->m_nPoseID, pPoseInfo ) );
    }
}

CAmuseRoomSceneConfig* CAmuseRoomConfigManager::GetAmuseSceneInfo(int nSceneID)
{
    CAmuseRoomSceneConfig* pSceneInfo = NULL;

    std::map<int, CAmuseRoomSceneConfig*>::iterator it = m_AmuseSceneInfo.find( nSceneID );
    if ( it != m_AmuseSceneInfo.end() )
    {
        pSceneInfo = it->second;
    }

    return pSceneInfo;
}

CAmuseRoomPoseConfig* CAmuseRoomConfigManager::GetAmusePoseInfo(int nPoseID)
{
    CAmuseRoomPoseConfig* pPoseInfo = NULL;

    std::map<int, CAmuseRoomPoseConfig*>::iterator it = m_AmusePoseInfo.find( nPoseID );
    if ( it != m_AmusePoseInfo.end() )
    {
        pPoseInfo = it->second;
    }

    return pPoseInfo;
}

bool CAmuseRoomConfigManager::SaveAmuseInfo(const char* pFileName)
{
    CFileStream file;
    file.open( pFileName, "wb" );

    if( file.IsOpen() )
    {
        //场景
        unsigned short nSceneCount = (unsigned short)m_AmuseSceneInfo.size();
        file.WriteUShort( nSceneCount );

        for( std::map<int, CAmuseRoomSceneConfig*>::iterator it_Scene = m_AmuseSceneInfo.begin(); it_Scene != m_AmuseSceneInfo.end(); ++it_Scene )
        {
            it_Scene->second->Save( file );
        }

        //动作
        unsigned short nActionCount = (unsigned short)m_AmusePoseInfo.size();
        file.WriteUShort( nActionCount );

        for( std::map<int, CAmuseRoomPoseConfig*>::iterator it_Action = m_AmusePoseInfo.begin(); it_Action != m_AmusePoseInfo.end(); ++it_Action )
        {
            it_Action->second->Save( file );
        }

        file.close();
        return true;
    }

    return false;
}

bool CAmuseRoomConfigManager::LoadAmuseInfo(const char* pFileName)
{
    CFileStream file;
    file.open( pFileName, "rb" );

    if( file.IsOpen() )
    {
        //场景
        unsigned short nSceneCount = 0;
        file.ReadUShort( nSceneCount );

        for( unsigned short i = 0; i < nSceneCount; ++i )
        {
            CAmuseRoomSceneConfig* pSceneInfo = new CAmuseRoomSceneConfig();
            pSceneInfo->Load( file );

            AddAmuseSceneInfo( pSceneInfo );
        }

        //动作
        unsigned short nPoseCount = 0;
        file.ReadUShort( nPoseCount );

        for( unsigned short i = 0; i < nPoseCount; ++i )
        {
            CAmuseRoomPoseConfig* pPoseInfo = new CAmuseRoomPoseConfig();
            pPoseInfo->Load( file );

            AddAmusePoseInfo( pPoseInfo );
        }

        file.close();
        return true;
    }

    return false;
}

bool CAmuseRoomConfigManager::ExistAmuseScene(int nSceneID) const
{
    return (m_AmuseSceneInfo.find( nSceneID ) != m_AmuseSceneInfo.end());
}

int CAmuseRoomConfigManager::RandomAmuseScene() const
{
    if (m_vecAmuseSceneID.empty())
    {
        return 0;
    }

    int nIndex = Random(0, (int)(m_vecAmuseSceneID.size() - 1));
    return m_vecAmuseSceneID[nIndex];
}

