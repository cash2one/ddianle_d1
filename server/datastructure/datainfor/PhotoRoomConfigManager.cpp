#include "PhotoRoomConfigManager.h"
#include "./FileStream.h"

PhotoRoomConfigManager::PhotoRoomConfigManager()
{

}

PhotoRoomConfigManager::~PhotoRoomConfigManager()
{
    for (std::map<unsigned int, PhotoRoomConfig *>::iterator itr = m_mapBG.begin(); itr != m_mapBG.end(); ++itr) {
        if (itr->second != NULL) {
            delete itr->second, itr->second = NULL;
        }
    }
    m_mapBG.clear();

    for (std::map<unsigned int, PhotoThemeBGConfig *>::iterator itr = m_mapThemeBG.begin(); itr != m_mapThemeBG.end(); ++itr) {
        if (itr->second != NULL) {
            delete itr->second, itr->second = NULL;
        }
    }
    m_mapThemeBG.clear();

    for (std::map<unsigned int, PhotoRoomSingleAction *>::iterator itr = m_mapSingleAction.begin(); itr != m_mapSingleAction.end(); ++itr) {
        if (itr->second != NULL) {
            delete itr->second, itr->second = NULL;
        }
    }
    m_mapSingleAction.clear();

    for (std::map<unsigned int, PhotoRoomGroupAction *>::iterator itr = m_mapGroupAction.begin(); itr != m_mapGroupAction.end(); ++itr) {
        if (itr->second != NULL) {
            delete itr->second, itr->second = NULL;
        }
    }
    m_mapGroupAction.clear();

    for (std::map<unsigned int, PhotoRoomThemeGroupAction *>::iterator itr = m_mapThemeGroupAction.begin(); itr != m_mapThemeGroupAction.end(); ++itr) {
        if (itr->second != NULL) {
            delete itr->second, itr->second = NULL;
        }
    }
    m_mapThemeGroupAction.clear();

    for (std::map<unsigned int, PhotoRoomTheme *>::iterator itr = m_mapTheme.begin(); itr != m_mapTheme.end(); ++itr) {
        if (itr->second != NULL) {
            delete itr->second, itr->second = NULL;
        }
    }
    m_mapTheme.clear();
}

PhotoRoomConfigManager & PhotoRoomConfigManager::Instance()
{
    static PhotoRoomConfigManager s_photoStudioDataMgr;

    return s_photoStudioDataMgr;
}

void PhotoRoomConfigManager::AddBGInfo( PhotoRoomConfig *pInfo )
{
    if (pInfo != NULL) {
        m_mapBG.insert(make_pair(pInfo->m_nID, pInfo));
    }
}

void PhotoRoomConfigManager::AddThemeBGInfo( PhotoThemeBGConfig *pInfo )
{
    if (pInfo != NULL) {
        m_mapThemeBG.insert(make_pair(pInfo->m_nID, pInfo));
    }
}

void PhotoRoomConfigManager::AddSingleActionInfo( PhotoRoomSingleAction *pInfo )
{
    if (pInfo != NULL) {
        m_mapSingleAction.insert(make_pair(pInfo->m_nID, pInfo));
    }
}


void PhotoRoomConfigManager::AddStudioGroupActionInfo( PhotoRoomGroupAction *pInfo )
{
    if (pInfo != NULL) {
        m_mapGroupAction.insert(make_pair(pInfo->m_nID, pInfo));
    }
}

void PhotoRoomConfigManager::AddStudioThemeGroupActionInfo( PhotoRoomThemeGroupAction *pInfo )
{
    if (pInfo != NULL) {
        m_mapThemeGroupAction.insert(make_pair(pInfo->m_nID, pInfo));
    }
}

void PhotoRoomConfigManager::AddThemeInfo( PhotoRoomTheme *pInfo )
{
    if (pInfo != NULL) {
        m_mapTheme.insert(make_pair(pInfo->m_nID, pInfo));
    }
}

bool PhotoRoomConfigManager:: SaveInfo(const char* pszFileName)
{
    CFileStream file;
    file.open(pszFileName,"wb");

    if( file.IsOpen() ) {

        //幕布
        unsigned short ulNumber = (unsigned short)m_mapBG.size();
        file.WriteUShort( ulNumber );

        for( std::map<unsigned int, PhotoRoomConfig *>::iterator itr = m_mapBG.begin(); itr != m_mapBG.end(); ++itr ) {
            itr->second->Save(file);
        }

        //幕布适用背景
        ulNumber = (unsigned short)m_mapThemeBG.size();
        file.WriteUShort( ulNumber );

        for( std::map<unsigned int, PhotoThemeBGConfig *>::iterator itr = m_mapThemeBG.begin(); itr != m_mapThemeBG.end(); ++itr ) {
            itr->second->Save(file);
        }

        //单人动作
        ulNumber = (unsigned short)m_mapSingleAction.size();
        file.WriteUShort( ulNumber );

        for( std::map<unsigned int, PhotoRoomSingleAction *>::iterator itr = m_mapSingleAction.begin(); itr != m_mapSingleAction.end(); ++itr ) {
            itr->second->Save(file);
        }

        //团体动作
        ulNumber = (unsigned short)m_mapGroupAction.size();
        file.WriteUShort( ulNumber );

        for( std::map<unsigned int, PhotoRoomGroupAction *>::iterator itr = m_mapGroupAction.begin(); itr != m_mapGroupAction.end(); ++itr ) {
            itr->second->Save(file);
        }

        //团体动作适用场景
        ulNumber = (unsigned short)m_mapThemeGroupAction.size();
        file.WriteUShort( ulNumber );

        for( std::map<unsigned int, PhotoRoomThemeGroupAction *>::iterator itr = m_mapThemeGroupAction.begin(); itr != m_mapThemeGroupAction.end(); ++itr ) {
            itr->second->Save(file);
        }

        //主题表
        ulNumber = (unsigned short)m_mapTheme.size();
        file.WriteUShort( ulNumber );

        for( std::map<unsigned int, PhotoRoomTheme *>::iterator itr = m_mapTheme.begin(); itr != m_mapTheme.end(); ++itr ) {
            itr->second->Save(file);
        }

        file.close();
        return true;
    }

    return false;
}

bool PhotoRoomConfigManager:: LoadInfo(const char* pszFileName)
{
    bool bRes = false;

    CFileStream file;
    file.open( pszFileName, "rb" );

    if( file.IsOpen() ) {

        //幕布
        unsigned short ulNumber = 0;
        file.ReadUShort( ulNumber );
        for( unsigned short i=0; i < ulNumber; ++i ) {
            PhotoRoomConfig *pPhotoBGInfo = new PhotoRoomConfig;
            pPhotoBGInfo->Load( file );
            AddBGInfo( pPhotoBGInfo );
        }

        //幕布适用背景
        ulNumber = 0;
        file.ReadUShort( ulNumber );
        for( unsigned short i=0; i < ulNumber; ++i ) {
            PhotoThemeBGConfig *pInfo = new PhotoThemeBGConfig;
            pInfo->Load( file );
            AddThemeBGInfo( pInfo );
        }

        //单人动作
        ulNumber = 0;
        file.ReadUShort( ulNumber );
        for( unsigned short i=0; i < ulNumber; ++i ) {
            PhotoRoomSingleAction *pInfo = new PhotoRoomSingleAction;
            pInfo->Load( file );
            AddSingleActionInfo( pInfo );
        }

        //团体动作
        ulNumber = 0;
        file.ReadUShort( ulNumber );
        for( unsigned short i=0; i < ulNumber; ++i ) {
            PhotoRoomGroupAction *pInfo = new PhotoRoomGroupAction;
            pInfo->Load( file );
            AddStudioGroupActionInfo( pInfo );
        }

        //团体动作适用场景
        ulNumber = 0;
        file.ReadUShort( ulNumber );
        for( unsigned short i=0; i < ulNumber; ++i ) {
            PhotoRoomThemeGroupAction *pInfo = new PhotoRoomThemeGroupAction;
            pInfo->Load( file );
            AddStudioThemeGroupActionInfo( pInfo );
        }

        //主题表
        ulNumber = 0;
        file.ReadUShort( ulNumber );
        for( unsigned short i=0; i < ulNumber; ++i ) {
            PhotoRoomTheme *pInfo = new PhotoRoomTheme;
            pInfo->Load( file );
            AddThemeInfo( pInfo );
        }

        file.close();

        bRes = _CheckData();
    }

    return bRes;
}

bool PhotoRoomConfigManager::_CheckData()
{
    int nCount = 0;
    for (std::map<unsigned int, PhotoRoomTheme *>::iterator itrTheme = m_mapTheme.begin(); itrTheme != m_mapTheme.end(); ++itrTheme) {
        if (itrTheme->second != NULL) {
            for (std::map<unsigned int, PhotoThemeBGConfig *>::iterator itrBG = m_mapThemeBG.begin(); itrBG != m_mapThemeBG.end(); ++itrBG) {
                if (itrBG->second != NULL) {
                    if (itrTheme->second->m_nID == itrBG->second->m_ThemeID && itrTheme->second->DefaultThemeBG() == itrBG->second->m_nBGID) {
                        ++nCount;
                    } 
                }
            }
        }
    }

    return nCount == (int)m_mapTheme.size();
}

bool PhotoRoomConfigManager::ThemeHasBG(unsigned int nBGID) const
{
    for (std::map<unsigned int, PhotoThemeBGConfig *>::const_iterator itr = m_mapThemeBG.begin(); itr != m_mapThemeBG.end(); ++itr) {
        if (itr->second != NULL) {
            if (itr->second->m_nBGID == nBGID) {
                return true;
            }
        }
    }

    return false;
}

bool PhotoRoomConfigManager::ThemeHasGroupAction(unsigned int nGroupActionID) const
{
    for (std::map<unsigned int, PhotoRoomThemeGroupAction *>::const_iterator itr = m_mapThemeGroupAction.begin(); itr != m_mapThemeGroupAction.end(); ++itr) {
        if (itr->second != NULL) {
            if (itr->second->m_GroupActionID == nGroupActionID) {
                return true;
            }
        }
    }

    return false;
}

const PhotoRoomConfig * PhotoRoomConfigManager::PhotoRoomBG(unsigned int nBGID) const
{
    std::map<unsigned int, PhotoRoomConfig *>::const_iterator itr = m_mapBG.find(nBGID);
    if (itr != m_mapBG.end()) {
        if (itr->second != NULL) {
            return itr->second;
        }
    }

    return NULL;
}

const PhotoRoomGroupAction * PhotoRoomConfigManager::GroupActionInfo(unsigned int nActionID) const
{
    std::map<unsigned int, PhotoRoomGroupAction *>::const_iterator itr = m_mapGroupAction.find(nActionID);
    if (itr != m_mapGroupAction.end()) {
        if (itr->second != NULL) {
            return itr->second;
        }
    }

    return NULL;
}

const std::map<unsigned int, PhotoRoomTheme *> & PhotoRoomConfigManager::PhotoRoomThemeMap() const
{
    return m_mapTheme;
}

const PhotoRoomTheme * PhotoRoomConfigManager::GetPhotoRoomTheme( unsigned int nThemeID ) const
{
    std::map<unsigned int, PhotoRoomTheme *>::const_iterator itr = m_mapTheme.find(nThemeID);	
    if (itr != m_mapTheme.end() && itr->second != NULL) {
        return itr->second;
    }

    return NULL;
}

