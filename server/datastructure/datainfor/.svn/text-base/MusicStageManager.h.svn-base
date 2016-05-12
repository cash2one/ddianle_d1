#ifndef __MUSICSTAGEMANAGER_H_
#define __MUSICSTAGEMANAGER_H_

#include <vector>
#include <set>
#include <map>
#include "./MusicStageConfig.h"


class MusicStageManager
{
public:
    MusicStageManager(void);
    virtual ~MusicStageManager(void);

public:
    //从文件里读取所有unit的信息
    bool LoadMusicStage(CFileStream&file);
    bool SaveMusicStage(CFileStream&file);
    bool ExistMusic(unsigned short nMusicID,unsigned char nMode, unsigned char nLevel);
    unsigned char RandomMode( unsigned short nMusicID, unsigned char nExcludeModelID );
    bool IsHaveMode( unsigned short nMusicID, unsigned char nMode );
    bool IsHaveLevel( unsigned short nMusicID, unsigned char nLevel );
    unsigned char RandomLevel( unsigned short nMusicID );
    unsigned short RandomMusic(unsigned char nMode, unsigned char nLevel);
    unsigned short RandomMusic(unsigned char nMode, unsigned char nLevel, bool bIsFresherMusic);
    bool Add(MusicStageConfig* pMusicStage);

    //释放所有对象
    void Destroy();

protected:
    void _Construct( MusicStageConfig& rMusicStage );
    void _ConstructMode( MusicStageConfig& rMusicStage );
    void _ConstructLevel( MusicStageConfig& rMusicStage );

public:
    typedef std::set< unsigned short > MusicSet;
    typedef std::map< bool, MusicSet > NewPlayerMusicMap;               //< bIsFresherMusic, musicID>
    typedef std::map< unsigned short, unsigned char > MusicStarMap;     //< musicID, star >
    typedef std::map< unsigned char, NewPlayerMusicMap > LevelMusicMap; //< level, < musicID,newplayer > >
    typedef std::map< unsigned char, LevelMusicMap > ModeMusicMap;      //< mode, < level, <musicID> > >

private:
    std::vector<MusicStageConfig*>                              m_vecStage;
    MusicStarMap                                                m_mapStar;
    ModeMusicMap                                                m_mapStage;
    std::map<unsigned short,std::vector<unsigned char> >        m_mapMusicIDMode;
    std::map<unsigned short,std::vector<unsigned char> >        m_mapMusicIDLevel;
};


#endif//__MusicinfoMANAGER_H_


//end file


