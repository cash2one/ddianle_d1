/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\PhotoRoomConfigManager
    author:     

    purpose:    对摄影棚配置数据管理
*********************************************************************/

#ifndef     __PHOTOROOMCONFIG_MANAGER_H__
#define     __PHOTOROOMCONFIG_MANAGER_H__

#include <string>
#include <map>
#include "../DataStruct_Base.h"
#include "PhotoRoomConfig.h"


class PhotoRoomConfigManager
{
private:
    PhotoRoomConfigManager();
    ~PhotoRoomConfigManager();

public:
    static PhotoRoomConfigManager & Instance();

    void AddBGInfo(PhotoRoomConfig *pInfo);
    void AddThemeBGInfo(PhotoThemeBGConfig *pInfo);
    void AddSingleActionInfo(PhotoRoomSingleAction *pInfo);
    void AddStudioGroupActionInfo(PhotoRoomGroupAction *pInfo);
    void AddStudioThemeGroupActionInfo(PhotoRoomThemeGroupAction *pInfo);
    void AddThemeInfo(PhotoRoomTheme *pInfo);
    bool SaveInfo(const char* pszFileName);
    bool LoadInfo(const char* pszFileName);

public: 
    bool ThemeHasBG(unsigned int nBGID) const;
    bool ThemeHasGroupAction(unsigned int nGroupActionID) const;
    const PhotoRoomConfig * PhotoRoomBG(unsigned int nBGID) const;
    const PhotoRoomGroupAction * GroupActionInfo(unsigned int nActionID) const;
    const std::map<unsigned int, PhotoRoomTheme *> & PhotoRoomThemeMap() const;
    const PhotoRoomTheme * GetPhotoRoomTheme(unsigned int nThemeID) const;

protected:
    bool _CheckData();

public:
    std::map<unsigned int, PhotoRoomConfig *> m_mapBG;
    std::map<unsigned int, PhotoThemeBGConfig *> m_mapThemeBG;
    std::map<unsigned int, PhotoRoomSingleAction *> m_mapSingleAction;
    std::map<unsigned int, PhotoRoomGroupAction *> m_mapGroupAction;
    std::map<unsigned int, PhotoRoomThemeGroupAction *> m_mapThemeGroupAction;
    std::map<unsigned int, PhotoRoomTheme *> m_mapTheme;
};


#endif


