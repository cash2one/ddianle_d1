/********************************************************************
created:    2012/03/01
author:        meijiuhua

*********************************************************************/

#ifndef __MUSICSTAGE_H__
#define __MUSICSTAGE_H__

#ifdef WIN32
#pragma warning(disable:4018)
#pragma warning(disable:4100)
#endif

#include <string>

class CFileStream;

class MusicStageConfig
{
public:
    MusicStageConfig(void);
    ~MusicStageConfig(void);

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned short  m_nMusicID;   //音乐id编号
    char            m_nModeID;    //模式编号
    char            m_nLevel;     //难度
    char            m_nStar;      //星级
    bool            m_bNewPlayer; //是否是新手音乐：小包音乐筛选用，只是内存数据，不存在bytes文件中

};

#endif//__ITEMINFO_H__


//end file


