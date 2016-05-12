#include "MusicStageManager.h"
#include "MusicStageConfig.h"
#include "FileStream.h"
#include "DataInfoDefine.h"
#include "../../socket/Windefine.h"
#include "../../socket/Utility.h"
#include "MusicConfig.h"
#include "MusicConfigManager.h"
#include "ConfigManager.h"
#include <algorithm>


MusicStageManager::MusicStageManager(void)
{
}

MusicStageManager::~MusicStageManager(void)
{
    Destroy();
}

bool MusicStageManager::LoadMusicStage(CFileStream&file)
{
    Destroy();

    if( file.IsOpen() )
    {
        unsigned short ulNumber = 0;
        file.ReadUShort( ulNumber );

        MusicStageConfig rMusicStage;
        for( unsigned short i=0; i < ulNumber; ++i )
        {
            rMusicStage.Load( file );
            _Construct( rMusicStage );
            _ConstructMode( rMusicStage );
            _ConstructLevel( rMusicStage );
        }

        return true;
    }

    return false;
}

bool MusicStageManager::SaveMusicStage(CFileStream&file)
{
    if( file.IsOpen() )
    {
        unsigned short ulNumber = (unsigned short)m_vecStage.size();
        file.WriteUShort( ulNumber );

        for( std::vector<MusicStageConfig*>::iterator it = m_vecStage.begin(); it != m_vecStage.end(); ++it )
        {
            (*it)->Save(file);
        }

        return true;
    }

    return false;
}

bool MusicStageManager::ExistMusic(unsigned short nMusicID,unsigned char nMode, unsigned char nLevel)
{
    ModeMusicMap::iterator it_Mode = m_mapStage.find( nMode );

    if ( it_Mode != m_mapStage.end() )
    {
        LevelMusicMap::iterator it_Level = it_Mode->second.find( nLevel );

        if ( it_Level != it_Mode->second.end() )
        {
            NewPlayerMusicMap::iterator it_NewPlayer = it_Level->second.begin();
            for ( ; it_NewPlayer != it_Level->second.end(); ++it_NewPlayer )
            {
                if ( it_NewPlayer->second.find( nMusicID) != it_NewPlayer->second.end() )
                {
                    return true;
                }
            }
        }
    }

    return false;
}

unsigned char MusicStageManager::RandomMode( unsigned short nMusicID,  unsigned char nExcludeModelID )
{
    if ( nMusicID == RAN_MUSIC_ID )
    {
        return ConfigManager::Instance().GetSceneModeManager().RandomMode(nExcludeModelID);
    }
    else
    {
        std::map<unsigned short,std::vector<unsigned char> >::iterator it = m_mapMusicIDMode.find( nMusicID );
        if ( it != m_mapMusicIDMode.end() && it->second.size() > 0 )
        {
            std::vector<unsigned char> tempMode( it->second );
            std::vector<unsigned char>::iterator iter = find( tempMode.begin(), tempMode.end(), nExcludeModelID );
            if ( iter != tempMode.end() )
            {
                tempMode.erase( iter );
            }

            if ( tempMode.size() > 0 )
            {
                int nModeCount = tempMode.size();
                int nRandomIndex =  Random()%nModeCount;
                return tempMode[nRandomIndex];
            }
        }

        return MusicMode_None;
    }
}

unsigned char MusicStageManager::RandomLevel( unsigned short nMusicID )
{
    if ( nMusicID == RAN_MUSIC_ID )
    {
        return (unsigned char)Random( MusicLevel_Easy, MusicLevel_Hard );
    }
    else
    {
        std::map<unsigned short,std::vector<unsigned char> >::iterator it = m_mapMusicIDLevel.find( nMusicID );
        if ( it != m_mapMusicIDLevel.end() && it->second.size() > 0 )
        {
            int nModeCount = it->second.size();
            int nRandomIndex =  Random()%nModeCount;

            return it->second[nRandomIndex];
        }

        return MusicLevel_None;
    }
}

bool MusicStageManager::IsHaveLevel( unsigned short nMusicID, unsigned char nLevel )
{
    std::map<unsigned short,std::vector<unsigned char> >::iterator it = m_mapMusicIDLevel.find( nMusicID );
    if ( it != m_mapMusicIDLevel.end() )
    {
        std::vector<unsigned char>::iterator iter = find( it->second.begin(), it->second.end(), nLevel );
        if ( iter != it->second.end() )
        {
            return true;
        }
    }

    return false;
}

unsigned short MusicStageManager::RandomMusic(unsigned char nMode, unsigned char nLevel)
{
    unsigned short nMusicID = 0;
    ModeMusicMap::iterator it_Mode = m_mapStage.find( nMode );

    if ( it_Mode != m_mapStage.end() )
    {
        LevelMusicMap::iterator it_Level = it_Mode->second.find( nLevel );

        if ( it_Level != it_Mode->second.end() && !it_Level->second.empty() )
        {
            MusicSet setTemp;
            NewPlayerMusicMap::iterator it_NewPlayer = it_Level->second.begin();
            for ( ; it_NewPlayer != it_Level->second.end(); ++it_NewPlayer )
            {
                setTemp.insert( it_NewPlayer->second.begin(), it_NewPlayer->second.end() );
            }

            unsigned short nRandom =  (unsigned short)Random( 0, (int)(setTemp.size() - 1) );

            MusicSet::iterator it_Music = setTemp.begin();
            advance( it_Music, nRandom );

            nMusicID = (*it_Music);
        }
    }

    return nMusicID;
}

unsigned short MusicStageManager::RandomMusic(unsigned char nMode, unsigned char nLevel, bool bIsFresherMusic)
{
    unsigned short nMusicID = 0;
    ModeMusicMap::iterator it_Mode = m_mapStage.find( nMode );

    if ( it_Mode != m_mapStage.end() )
    {
        LevelMusicMap::iterator it_Level = it_Mode->second.find( nLevel );

        if ( it_Level != it_Mode->second.end() )
        {
            NewPlayerMusicMap::iterator it_NewPlayer = it_Level->second.find( bIsFresherMusic );
            if ( it_NewPlayer != it_Level->second.end() && !it_NewPlayer->second.empty() )
            {
                unsigned short nRandom = (unsigned short)Random( 0, (int)(it_NewPlayer->second.size() - 1) );

                MusicSet::iterator it_Music = it_NewPlayer->second.begin();
                advance( it_Music, nRandom );

                nMusicID = (*it_Music);
            }
        }
    }

    return nMusicID;
}

bool MusicStageManager::Add(MusicStageConfig* pMusicStage)
{
    if ( pMusicStage != NULL )
    {
        m_vecStage.push_back( pMusicStage );

        return true;
    }

    return false;
}

void MusicStageManager::Destroy()
{
    for ( std::vector<MusicStageConfig*>::iterator it = m_vecStage.begin(); it != m_vecStage.end(); ++it )
    {
        delete (*it);
        (*it) = NULL;
    }

    m_vecStage.clear();
    m_mapStar.clear();
    m_mapStage.clear();

    m_mapMusicIDLevel.clear();
    m_mapMusicIDMode.clear();
}

void MusicStageManager::_ConstructMode( MusicStageConfig& rMusicStage )
{
    MusicConfig * pMusicInfo = ConfigManager::Instance().GetMusicConfigManager().GetByID( rMusicStage.m_nMusicID );
    if ( pMusicInfo == NULL )
    {
        return;
    }

    if ( rMusicStage.m_nMusicID >= MIN_MUSIC_ID )
    {
        std::map<unsigned short,std::vector<unsigned char> >::iterator it = m_mapMusicIDMode.find( rMusicStage.m_nMusicID );
        if ( it != m_mapMusicIDMode.end() )
        {
            std::vector<unsigned char>::iterator iter = find( it->second.begin(),it->second.end(),rMusicStage.m_nModeID);
            if ( iter != it->second.end() )
            {
                return;
            }
        }
        m_mapMusicIDMode[rMusicStage.m_nMusicID].push_back( rMusicStage.m_nModeID );
    }
}

void MusicStageManager::_ConstructLevel( MusicStageConfig& rMusicStage )
{
    MusicConfig * pMusicInfo = ConfigManager::Instance().GetMusicConfigManager().GetByID( rMusicStage.m_nMusicID );
    if ( pMusicInfo == NULL )
    {
        return;
    }

    if ( rMusicStage.m_nMusicID >= MIN_MUSIC_ID )
    {
        std::map<unsigned short,std::vector<unsigned char> >::iterator it = m_mapMusicIDLevel.find( rMusicStage.m_nMusicID );
        if ( it != m_mapMusicIDLevel.end() )
        {
            std::vector<unsigned char>::iterator iter = find( it->second.begin(),it->second.end(),rMusicStage.m_nLevel);
            if ( iter != it->second.end() )
            {
                return;
            }
        }
        m_mapMusicIDLevel[rMusicStage.m_nMusicID].push_back( rMusicStage.m_nLevel );
    }
}

bool MusicStageManager::IsHaveMode( unsigned short nMusicID, unsigned char nMode )
{
    std::map<unsigned short,std::vector<unsigned char> >::iterator it = m_mapMusicIDMode.find( nMusicID );
    if ( it != m_mapMusicIDMode.end() )
    {
        std::vector<unsigned char>::iterator iter = find( it->second.begin(), it->second.end(), nMode );
        if ( iter != it->second.end() )
        {
            return true;
        }
    }

    return false;
}

void MusicStageManager::_Construct( MusicStageConfig& rMusicStage)
{
    MusicConfig * pMusicInfo = ConfigManager::Instance().GetMusicConfigManager().GetByID( rMusicStage.m_nMusicID );
    if ( pMusicInfo == NULL )
    {
        return;
    }

    rMusicStage.m_bNewPlayer = pMusicInfo->m_bIsFresher;

    if( rMusicStage.m_nMusicID >= MIN_MUSIC_ID )
    {
        ModeMusicMap::iterator it_Mode = m_mapStage.find( rMusicStage.m_nModeID );

        if ( it_Mode == m_mapStage.end() )
        {
            MusicSet setMusic;
            setMusic.insert( rMusicStage.m_nMusicID );

            NewPlayerMusicMap mapNewplayer;
            mapNewplayer.insert(std::make_pair(rMusicStage.m_bNewPlayer, setMusic));

            LevelMusicMap mapLevel;
            mapLevel.insert( std::make_pair( rMusicStage.m_nLevel, mapNewplayer ) );

            m_mapStage.insert( std::make_pair( rMusicStage.m_nModeID, mapLevel ) );
        }
        else
        {
            LevelMusicMap::iterator it_Level = it_Mode->second.find( rMusicStage.m_nLevel );

            if ( it_Level == it_Mode->second.end() )
            {
                MusicSet setMusic;
                setMusic.insert( rMusicStage.m_nMusicID );

                NewPlayerMusicMap mapNewplayer;
                mapNewplayer.insert(std::make_pair(rMusicStage.m_bNewPlayer, setMusic));

                it_Mode->second.insert( std::make_pair( rMusicStage.m_nLevel, mapNewplayer ) );
            }
            else
            {
                NewPlayerMusicMap::iterator it_NewPlayer = it_Level->second.find( rMusicStage.m_bNewPlayer );
                if ( it_NewPlayer == it_Level->second.end())
                {
                    MusicSet setMusic;
                    setMusic.insert( rMusicStage.m_nMusicID );

                    it_Level->second.insert( std::make_pair(rMusicStage.m_bNewPlayer, setMusic));
                }
                else
                {
                    it_NewPlayer->second.insert( rMusicStage.m_nMusicID );
                }
            }
        }

        m_mapStar.insert( std::make_pair( rMusicStage.m_nMusicID, rMusicStage.m_nStar ) );
    }
}

//end file


