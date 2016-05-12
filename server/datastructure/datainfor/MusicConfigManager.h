#ifndef __MUSICINFOMANAGER_H_
#define __MUSICINFOMANAGER_H_

#include <map>
#include "./MusicConfig.h"


class MusicConfigManager
{
public:
    MusicConfigManager(void);
    virtual ~MusicConfigManager(void);

public:
    bool LoadMusicInfo(CFileStream&file);
    bool SaveMusicInfo(CFileStream&file);

    // Add CMusicInfo;
    bool Add(MusicConfig* pMusicInfo);

    unsigned short GetNumber() const;

    MusicConfig * GetByID(unsigned short nType) const;

    MusicConfig * GetAnyFresherMusic() const;

    // 释放所有对象
    void Destroy();

private:
    // 物品数据存放
    std::map<unsigned short,MusicConfig*> m_MusicMap;
};


#endif



//end file


