#include "MagicLampConfigMgr.h"
#include <algorithm>


CMagicLampConfigMgr::~CMagicLampConfigMgr()
{
    Destroy();
}

bool CMagicLampConfigMgr::LoadMagicLampConfig( const char * pFileName )
{
    CFileStream file;
    file.open( pFileName, "rb" );

    if( file.IsOpen() )
    {
        unsigned short nBossCount = 0;
        file.ReadUShort( nBossCount );
        for( unsigned short i = 0; i < nBossCount; ++i )
        {
            CBossConfig* pAIDataInfo = new CBossConfig();
            pAIDataInfo->Load( file );

            m_BossConfig[ pAIDataInfo->m_nBossID] = pAIDataInfo;
        }

        unsigned short nDungeonCount = 0;
        file.ReadUShort( nDungeonCount );
        for( unsigned short i = 0; i < nDungeonCount; ++i )
        {
            CDungeonConfig* pDungeonInfo = new CDungeonConfig();
            pDungeonInfo->Load( file );

            m_DungeonConfig[ pDungeonInfo->m_nDungeonID] = pDungeonInfo;
        }

        unsigned short nParcloseCount = 0;
        file.ReadUShort( nParcloseCount );
        for( unsigned short i = 0; i < nParcloseCount; ++i )
        {
            CParcloseConfig* pParcloseInfo = new CParcloseConfig();
            pParcloseInfo->Load( file );

            m_ParcloseConfig[ pParcloseInfo->m_nParcloseID] = pParcloseInfo;
        }

        unsigned short nMainlineDropCount = 0;
        file.ReadUShort( nMainlineDropCount );
        for ( unsigned short i = 0; i < nMainlineDropCount; ++i )
        {
            CMainlineDropConfig * pMainlineDrop = new CMainlineDropConfig();
            pMainlineDrop->Load( file );

            m_MainlineDropConfig[ pMainlineDrop->m_nParcloseID ] = pMainlineDrop;
        }

        unsigned short nMainlineRubbishCount = 0;
        file.ReadUShort( nMainlineRubbishCount );
        for ( unsigned short i = 0; i < nMainlineRubbishCount; ++i )
        {
            CRubbishDropConfig * pMainlineRubbishDrop = new CRubbishDropConfig();
            pMainlineRubbishDrop->Load( file );

            m_MainlineRubbishDropConfig[ pMainlineRubbishDrop->m_nRubbishDropID ] = pMainlineRubbishDrop;
        }

        unsigned short nThemeCount = 0;
        file.ReadUShort( nThemeCount );
        for ( unsigned short i = 0; i < nThemeCount; ++i )
        {
            CThemeConfig * pThemeInfo = new CThemeConfig();
            pThemeInfo->Load( file );

            m_ThemeConfig[ pThemeInfo->m_nThemeID ] = pThemeInfo;
        }

        unsigned short nThemeParcloseCount = 0;
        file.ReadUShort( nThemeParcloseCount );
        for ( unsigned short i = 0; i < nThemeParcloseCount; ++i )
        {
            CThemeParcloseConfig * pThemeParcloseInfo = new CThemeParcloseConfig();
            pThemeParcloseInfo->Load( file );

            m_ThemeParcloseConfig[pThemeParcloseInfo->m_nThemeID].insert( make_pair( pThemeParcloseInfo->m_nParcloseID, pThemeParcloseInfo));
        }

        unsigned short nThemeDropCount = 0;
        file.ReadUShort( nThemeDropCount );
        for ( unsigned short i = 0; i < nThemeDropCount; ++i )
        {
            CThemeDropConfig * pThemeDropInfo = new CThemeDropConfig();
            pThemeDropInfo->Load( file );

            m_ThemeDropConfig[pThemeDropInfo->m_nThemeID].insert( make_pair( pThemeDropInfo->m_nParcloseID, pThemeDropInfo));
        }

        unsigned short nThemeRubbishCount = 0;
        file.ReadUShort( nThemeRubbishCount );
        for ( unsigned short i = 0; i < nThemeRubbishCount; ++i )
        {
            CRubbishDropConfig * pThemeRubbishDrop = new CRubbishDropConfig();
            pThemeRubbishDrop->Load( file );

            m_ThemeRubbishDropConfig[ pThemeRubbishDrop->m_nRubbishDropID ] = pThemeRubbishDrop;
        }

        unsigned short nStoryCount = 0;
        file.ReadUShort( nStoryCount );
        for( unsigned short i = 0; i < nStoryCount; ++i )
        {
            CStoryConfig* pStoryInfo = new CStoryConfig();
            pStoryInfo->Load( file );

            m_StoryConfig[ pStoryInfo->m_nStoryID] = pStoryInfo;
        }

        unsigned short nTargetCount = 0;
        file.ReadUShort( nTargetCount );
        for( unsigned short i = 0; i < nTargetCount; ++i )
        {
            CTargetConfig* pTargetInfo = new CTargetConfig();
            pTargetInfo->Load( file );

            m_TargetConfig[ pTargetInfo->m_nTargetID ] = pTargetInfo;
        }

        unsigned short nDebrisCount = 0;
        file.ReadUShort( nDebrisCount );
        for( unsigned short i = 0; i < nDebrisCount; ++i )
        {
            CDebrisConfig* pDebrisInfo = new CDebrisConfig();
            pDebrisInfo->Load( file );

            m_DebrisConfig[ pDebrisInfo->m_nDebrisID ] = pDebrisInfo;
        }

        file.close();
        return true;
    }

    return false;
}

CBossConfig * CMagicLampConfigMgr::GetAIBossInfo( unsigned int nBossID )
{
    std::map<int,CBossConfig*>::iterator it = m_BossConfig.find( nBossID );
    if ( it != m_BossConfig.end() )
    {
        return it->second;
    }

    return NULL;
}

unsigned int CMagicLampConfigMgr::GetMaxBossID()
{
    std::map<int,CBossConfig*>::reverse_iterator it = m_BossConfig.rbegin();
    if ( it != m_BossConfig.rend() )
    {
        return it->first;
    }

    return 0;
}

CDungeonConfig * CMagicLampConfigMgr::GetDungeonConfig( unsigned int nDungeonID )
{
    std::map<unsigned int,CDungeonConfig*>::iterator it = m_DungeonConfig.find( nDungeonID );
    if ( it != m_DungeonConfig.end() )
    {
        return it->second;
    }

    return NULL;
}

unsigned int CMagicLampConfigMgr::GetMaxMainlineID()
{
    std::map<unsigned int,CDungeonConfig*>::reverse_iterator it = m_DungeonConfig.rbegin();
    if ( it != m_DungeonConfig.rend() )
    {
        return it->first;
    }

    return 0;
}

CThemeConfig * CMagicLampConfigMgr::GetThemeConfig( unsigned int nThemeID )
{
    std::map<unsigned int,CThemeConfig*>::iterator it = m_ThemeConfig.find( nThemeID );
    if ( it != m_ThemeConfig.end() )
    {
        return it->second;
    }

    return NULL;
}

std::map<unsigned int,CThemeParcloseConfig*> * CMagicLampConfigMgr::GetThemeParcloseList( unsigned int nThemeID )
{
    std::map<unsigned int,std::map<unsigned int,CThemeParcloseConfig*> >::iterator it = m_ThemeParcloseConfig.find( nThemeID );
    if ( it != m_ThemeParcloseConfig.end())
    {
        return &(it->second);
    }

    return NULL;
}

unsigned int CMagicLampConfigMgr::GetMaxThemeID()
{
    std::map<unsigned int,CThemeConfig*>::reverse_iterator it = m_ThemeConfig.rbegin();
    if ( it != m_ThemeConfig.rend() )
    {
        return it->first;
    }

    return 0;
}

CThemeParcloseConfig * CMagicLampConfigMgr::GetThemeParcloseConfig( unsigned int nThemeID, unsigned int nParcloseID )
{
    std::map<unsigned int,std::map<unsigned int,CThemeParcloseConfig*> >::iterator it = m_ThemeParcloseConfig.find( nThemeID );
    if ( it != m_ThemeParcloseConfig.end())
    {
        std::map<unsigned int,CThemeParcloseConfig*>::iterator iter = it->second.find( nParcloseID );
        if ( iter != it->second.end())
        {
            return iter->second;
        }
    }

    return NULL;
}

unsigned int CMagicLampConfigMgr::GetDungeonIDByParcloseID( unsigned int nParcloseID )
{
    std::map<unsigned int,CDungeonConfig*>::iterator it = m_DungeonConfig.begin();
    for ( ; it != m_DungeonConfig.end(); ++it )
    {
        std::vector<unsigned int>::iterator iter  = std::find( it->second->m_ParcloseList.begin(), it->second->m_ParcloseList.end(), nParcloseID );
        if ( iter != it->second->m_ParcloseList.end() )
        {
            return it->first;
        }
    }

    return 0;
}

std::string CMagicLampConfigMgr::GetDungeonNameByParcloseID( unsigned int nParcloseID )
{
    std::map<unsigned int,CDungeonConfig*>::iterator it = m_DungeonConfig.begin();
    for ( ; it != m_DungeonConfig.end(); ++it )
    {
        std::vector<unsigned int>::iterator iter  = std::find( it->second->m_ParcloseList.begin(), it->second->m_ParcloseList.end(), nParcloseID );
        if ( iter != it->second->m_ParcloseList.end() )
        {
            return it->second->m_strDungeonName;
        }
    }

    return "";
}

CParcloseConfig * CMagicLampConfigMgr::GetMainlineParcloseConfig( unsigned int nParcloseID )
{
    std::map<unsigned int,CParcloseConfig*>::iterator it = m_ParcloseConfig.find( nParcloseID );
    if ( it != m_ParcloseConfig.end() )
    {
        return it->second;
    }

    return NULL;
}

CParcloseConfig * CMagicLampConfigMgr::GetParcloseConfig( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID )
{
    switch(eType)
    {
    case EDungeonType_Mainline:
        {
            return GetMainlineParcloseConfig( nParcloseID );
        }
        break;
    case EDungeonType_Theme:
        {
            return GetThemeParcloseConfig( nDungeonID, nParcloseID );
        }
        break;
    }
    return NULL;
}

CMainlineDropConfig * CMagicLampConfigMgr::GetParcloseDropInfo( unsigned int nParclose )
{
    std::map<unsigned int,CMainlineDropConfig*>::iterator it = m_MainlineDropConfig.find( nParclose );
    if ( it != m_MainlineDropConfig.end() )
    {
        return it->second;
    }

    return NULL;
}

unsigned int CMagicLampConfigMgr::GetMainlineNextParcloseID( unsigned int nParcloseID )
{
    std::map<unsigned int,CParcloseConfig*>::iterator it = m_ParcloseConfig.find( nParcloseID );
    if ( it != m_ParcloseConfig.end() )
    {
        it++;
        if ( it != m_ParcloseConfig.end() )
        {
            return it->second->m_nParcloseID;
        }
    }

    return nParcloseID;
}

unsigned int CMagicLampConfigMgr::GetThemeNextParcloseID( unsigned int nThemeID, unsigned int nParcloseID )
{
    std::map<unsigned int,std::map<unsigned int,CThemeParcloseConfig*> >::iterator it = m_ThemeParcloseConfig.find( nThemeID );
    if ( it != m_ThemeParcloseConfig.end())
    {
        std::map<unsigned int,CThemeParcloseConfig*>::iterator iter = it->second.find( nParcloseID );
        if ( iter != it->second.end() )
        {
            iter++;
            if ( iter != it->second.end())
            {
                return iter->second->m_nParcloseID;
            }
        }
    }

    return nParcloseID;
}

unsigned int CMagicLampConfigMgr::GetNextParcloseID( EDungeonType eType, unsigned int nDungeonID, unsigned int nParcloseID )
{
    switch( eType )
    {
    case EDungeonType_Mainline:
        return GetMainlineNextParcloseID( nParcloseID );
    case EDungeonType_Theme:
        return GetThemeNextParcloseID( nDungeonID, nParcloseID );
    }

    return 0;
}

unsigned int CMagicLampConfigMgr::GetFirstParcloseID()
{
    std::map<unsigned int,CParcloseConfig*>::iterator it = m_ParcloseConfig.begin();
    if ( it != m_ParcloseConfig.end() )
    {
        return it->second->m_nParcloseID;
    }

    return 0;
}

unsigned int CMagicLampConfigMgr::GetThemeFirstParcloseID( unsigned int nThemeID )
{
    std::map<unsigned int,std::map<unsigned int,CThemeParcloseConfig*> >::iterator it = m_ThemeParcloseConfig.find( nThemeID );
    if ( it != m_ThemeParcloseConfig.end())
    {
        std::map<unsigned int,CThemeParcloseConfig*>::iterator iter = it->second.begin();
        if ( iter != it->second.end())
        {
            return iter->second->m_nParcloseID;
        }
    }

    return 0;
}

CStoryConfig * CMagicLampConfigMgr::GetStoryConfig(  unsigned int nStoryID )
{
    std::map<unsigned int,CStoryConfig*>::iterator it = m_StoryConfig.find( nStoryID );
    if ( it != m_StoryConfig.end() )
    {
        return it->second;
    }

    return NULL;
}

CTargetConfig * CMagicLampConfigMgr::GetTargetConfig( unsigned int nTargetID )
{
    std::map<unsigned int,CTargetConfig*>::iterator it = m_TargetConfig.find( nTargetID );
    if ( it != m_TargetConfig.end() )
    {
        return it->second;
    }

    return NULL;
}

CMainlineDropConfig * CMagicLampConfigMgr::GetMainlineDrop( unsigned int nParcloseID )
{
    std::map<unsigned int,CMainlineDropConfig*>::iterator it = m_MainlineDropConfig.find( nParcloseID );
    if ( it != m_MainlineDropConfig.end() )
    {
        return it->second;
    }

    return NULL;
}

CThemeDropConfig * CMagicLampConfigMgr::GetThemeDrop( unsigned int nThemeID, unsigned int nParcloseID )
{
    std::map<unsigned int,std::map<unsigned int,CThemeDropConfig*> >::iterator it = m_ThemeDropConfig.find( nThemeID );
    if ( it != m_ThemeDropConfig.end() )
    {
        std::map<unsigned int,CThemeDropConfig*>::iterator iter = it->second.find( nParcloseID );
        if ( iter != it->second.end() )
        {
            return iter->second;
        }
    }

    return NULL;
}

CDebrisConfig * CMagicLampConfigMgr::GetDebrisConfig( unsigned int nDebrisID )
{
    std::map<unsigned int,CDebrisConfig*>::iterator it = m_DebrisConfig.find( nDebrisID );
    if ( it != m_DebrisConfig.end())
    {
        return it->second;
    }

    return NULL;
}

void CMagicLampConfigMgr::Destroy()
{
    for( std::map<int,CBossConfig*>::iterator it = m_BossConfig.begin(); it != m_BossConfig.end(); ++it )
    {
        delete it->second;
        it->second = NULL;
    }
    m_BossConfig.clear();

    std::map<unsigned int,CDungeonConfig*>::iterator itDungeon = m_DungeonConfig.begin();
    for ( ; itDungeon != m_DungeonConfig.end(); ++itDungeon )
    {
        delete itDungeon->second;itDungeon->second = NULL;
    }
    m_DungeonConfig.clear();

    std::map<unsigned int,CParcloseConfig*>::iterator itParclose = m_ParcloseConfig.begin();
    for ( ; itParclose != m_ParcloseConfig.end(); ++itParclose )
    {
        delete itParclose->second;itParclose->second = NULL;
    }
    m_ParcloseConfig.clear();

    std::map<unsigned int,CMainlineDropConfig*>::iterator itMainlineDrop = m_MainlineDropConfig.begin();
    for ( ; itMainlineDrop != m_MainlineDropConfig.end(); ++itMainlineDrop )
    {
        delete itMainlineDrop->second;
    }
    m_MainlineDropConfig.clear();

    std::map<unsigned int,CRubbishDropConfig*>::iterator itMainlineRubbish = m_MainlineRubbishDropConfig.begin();
    for ( ; itMainlineRubbish != m_MainlineRubbishDropConfig.end(); ++itMainlineRubbish )
    {
        delete itMainlineRubbish->second;
    }
    m_MainlineRubbishDropConfig.clear();

    std::map<unsigned int,CThemeConfig*>::iterator itTheme = m_ThemeConfig.begin();
    for ( ; itTheme != m_ThemeConfig.end(); ++itTheme )
    {
        delete itTheme->second;
    }
    m_ThemeConfig.clear();

    std::map<unsigned int,std::map<unsigned int,CThemeParcloseConfig*> >::iterator itThemeParclose = m_ThemeParcloseConfig.begin();
    for ( ; itThemeParclose != m_ThemeParcloseConfig.end(); ++itThemeParclose )
    {
        std::map<unsigned int,CThemeParcloseConfig*>::iterator iter = itThemeParclose->second.begin();
        for ( ; iter != itThemeParclose->second.end(); ++iter )
        {
            delete iter->second;
        }
    }
    m_ThemeParcloseConfig.clear();

    std::map<unsigned int,std::map<unsigned int,CThemeDropConfig*> >::iterator itThemeDrop = m_ThemeDropConfig.begin();
    for ( ; itThemeDrop != m_ThemeDropConfig.end(); ++itThemeDrop )
    {
        std::map<unsigned int,CThemeDropConfig*>::iterator iter = itThemeDrop->second.begin();
        for ( ; iter != itThemeDrop->second.end(); ++iter )
        {
            delete iter->second;
        }
    }
    m_ThemeDropConfig.clear();

    std::map<unsigned int,CRubbishDropConfig*>::iterator itThemeRubbish = m_ThemeRubbishDropConfig.begin();
    for ( ; itThemeRubbish != m_ThemeRubbishDropConfig.end(); ++itThemeRubbish )
    {
        delete itThemeRubbish->second;
    }
    m_ThemeRubbishDropConfig.clear();

    std::map<unsigned int,CStoryConfig*>::iterator itStory = m_StoryConfig.begin();
    for ( ; itStory != m_StoryConfig.end(); ++itStory )
    {
        delete itStory->second;itStory->second = NULL;
    }
    m_StoryConfig.clear();

    std::map<unsigned int,CTargetConfig*>::iterator itTarget = m_TargetConfig.begin();
    for ( ; itTarget != m_TargetConfig.end(); ++itTarget )
    {
        delete itTarget->second;itTarget->second = NULL;
    }
    m_TargetConfig.clear();

    std::map<unsigned int,CDebrisConfig*>::iterator itDebris = m_DebrisConfig.begin();
    for ( ; itDebris != m_DebrisConfig.end(); ++itDebris )
    {
        delete itDebris->second;
    }
    m_DebrisConfig.clear();
}
