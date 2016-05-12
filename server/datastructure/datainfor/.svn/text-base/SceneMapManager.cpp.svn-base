#include "SceneMapManager.h"
#include "SceneConfig.h"
#include "ModeConfig.h"
#include "FileStream.h"
#include "../../socket/Windefine.h"
#include "../../socket/Utility.h"
#include "../Macro_Define.h"


SceneMapManager::SceneMapManager(void)
{
}

SceneMapManager::~SceneMapManager(void)
{
    Destroy();
}

bool SceneMapManager::LoadScene(CFileStream&file)
{
    _DestroyScene();

    //CFileStream file;
    //file.open( pszFileName, "rb" );

    if( file.IsOpen() )
    {
        unsigned short ulNumber = 0;
        file.ReadUShort( ulNumber );

        if( ulNumber > 0 )
        {
            for( unsigned short i = 0; i < ulNumber; ++i )
            {
                SceneConfig* pScene = new SceneConfig;
                if( pScene != NULL )
                {
                    pScene->Load( file );
                    AddScene( pScene ); 
                }
            }
        }

        //file.close();
    }
    else
    {
        return false;
    }

    return true;
}

bool SceneMapManager::SaveScene(CFileStream&file)
{
    //CFileStream file;
    //file.open( pszFileName, "wb" );

    if( file.IsOpen() )
    {
        unsigned short ulNumber = (unsigned short)m_SceneMap.size();
        file.WriteUShort( ulNumber );

        for( SceneMap::iterator it = m_SceneMap.begin(); it != m_SceneMap.end(); ++it )
        {
            it->second->Save( file );
        }

        //file.close();
    }
    else
    {
        return false;
    }

    return true;
}


bool SceneMapManager::LoadMode(CFileStream&file)
{
    _DestroyMode();

    //CFileStream file;
    //file.open( pszFileName, "rb" );

    if( file.IsOpen() )
    {
        unsigned short ulNumber = 0;
        file.ReadUShort( ulNumber );

        if( ulNumber > 0 )
        {
            for( unsigned short i = 0; i < ulNumber; ++i )
            {
                ModeConfig * pMode = new ModeConfig;
                if( pMode != NULL )
                {
                    pMode->Load( file );
                    AddMode( pMode );
                }
            }
        }

        //file.close();
    }
    else
    {
        return false;
    }

    return true;
}

bool SceneMapManager::SaveMode(CFileStream&file)
{
    //CFileStream file;
    //file.open( pszFileName, "wb" );

    if( file.IsOpen() )
    {
        unsigned short ulNumber = (unsigned short)m_ModeMap.size();
        file.WriteUShort( ulNumber );

        for( ModeMap::iterator it = m_ModeMap.begin(); it != m_ModeMap.end(); ++it )
        {
            it->second->Save( file );
        }

        //file.close();
    }
    else
    {
        return false;
    }

    return true;
}

bool SceneMapManager::AddScene(SceneConfig* pSceneInfo)
{
    if ( pSceneInfo != NULL )
    {
        m_SceneMap.insert( std::make_pair( pSceneInfo->m_nSceneID, pSceneInfo ) );

        if ( pSceneInfo->m_nSceneID != RAN_SCENE_ID )
        {
            m_AllSceneID.push_back(pSceneInfo->m_nSceneID);
            if (pSceneInfo->m_bIsFresherScene)
            {
                m_FresherSceneID.push_back(pSceneInfo->m_nSceneID);
            }
        }

        return true;
    }

    return false;
}

bool SceneMapManager::AddMode(ModeConfig* pModeInfo)
{
    if ( pModeInfo != NULL )
    {
        m_ModeMap.insert( std::make_pair( pModeInfo->m_nModeID, pModeInfo ) );
        return true;
    }

    return false;
}


SceneConfig * SceneMapManager::GetSceneInfo(unsigned char nSceneID) const
{	
    // 	if (nSceneID > 0)
    // 	{
    SceneMap::const_iterator it = m_SceneMap.find( nSceneID );
    if( it != m_SceneMap.end() )
    {
        return it->second;
    }
    //}

    return NULL;
}

ModeConfig * SceneMapManager::GetModeInfo(unsigned char nModelID) const
{
    //if ( nModelID > 0 )
    //{
    ModeMap::const_iterator it = m_ModeMap.find( nModelID );
    if( it != m_ModeMap.end() )
    {
        return it->second;
    }
    //}

    return NULL;
}

unsigned char SceneMapManager::RandomScene(bool bOnlyFresherScene)
{
    unsigned char nSceneID = 0;

    if (!m_SceneMap.empty())
    {
        int nRandomSize = 0;
        if (bOnlyFresherScene)
        {
            nRandomSize = m_FresherSceneID.size();
        }
        else
        {
            nRandomSize = m_AllSceneID.size();
        }

        unsigned short nRandom =  (unsigned short)Random(0, nRandomSize - 1);

        if (bOnlyFresherScene)
        {
            nSceneID = m_FresherSceneID[nRandom];
        }
        else
        {
            nSceneID = m_AllSceneID[nRandom];
        }
    }

    return nSceneID;
}

unsigned char SceneMapManager::RandomMode(unsigned char nExcludeModelID)
{
    unsigned char nModeID = 0;

    ModeMap tempMode( m_ModeMap );
    ModeMap::iterator it = tempMode.find( nExcludeModelID );
    if ( it != tempMode.end() )
    {
        tempMode.erase( it );
    }

    it = tempMode.find( MusicMode_None );
    if ( it != tempMode.end() )
    {
        tempMode.erase( it );
    }

    if ( tempMode.size() > 0 )
    {
        unsigned short nRandom =  (unsigned short)Random( 0, (int)(tempMode.size() - 1) );
        ModeMap::iterator it_Mode = tempMode.begin();
        advance( it_Mode, nRandom );
        nModeID = it_Mode->first;
    }

    return nModeID;
}

void SceneMapManager::Destroy()
{
    _DestroyScene();
    _DestroyMode();
}

//释放所有对象
void SceneMapManager::_DestroyScene()
{
    for ( SceneMap::iterator it = m_SceneMap.begin(); it != m_SceneMap.end(); ++it )
    {
        delete it->second;
    }

    m_SceneMap.clear();
}

void SceneMapManager::_DestroyMode()
{
    for ( ModeMap::iterator it = m_ModeMap.begin(); it != m_ModeMap.end(); ++it )
    {
        delete it->second;
    }

    m_ModeMap.clear();
}

//end file


