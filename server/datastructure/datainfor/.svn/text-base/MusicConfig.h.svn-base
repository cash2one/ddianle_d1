/********************************************************************
created:    2011/01/14
author:        meijiuhua

*********************************************************************/

#ifndef __MUSICINFO_H__
#define __MUSICINFO_H__

#ifdef WIN32
#pragma warning(disable:4018)
#pragma warning(disable:4100)
#endif

#include <string>

class CFileStream;

class MusicConfig
{
public:
    MusicConfig(void);
    ~MusicConfig(void);

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

    //复制函数，相当于=
    bool Attach(MusicConfig*pSrc);

public:
    unsigned short         m_nMusicID;            //音乐id编号
    std::string            m_strMusicName;        //音乐名字
    std::string            m_strSingerName;       //歌手名字
    std::string            m_strCollectionName;   //专辑名字
    char                   m_nType;               //歌曲类型
    unsigned short         m_nTime;               //这首歌曲的时长
    unsigned short         m_nBpm;                //这首歌曲bpm
    std::string            m_strIcon;             //Icon
    std::string            m_strAtlas;            //Atlas
    std::string            m_strMusicSource;      //资源名称
    std::string            m_strIntro;            //tips
    bool                   m_bIsFresher;          //是否是新手包音乐：用于小包音乐筛选
};

#endif//__MUSICINFO_H__

