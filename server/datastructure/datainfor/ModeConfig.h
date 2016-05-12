/********************************************************************
created:	2012/03/01
author:		meijiuhua

*********************************************************************/

#ifndef __MODELINFO_H__
#define __MODELINFO_H__

#ifdef WIN32
#pragma warning(disable:4018)
#pragma warning(disable:4100)
#endif

#include <string>

class CFileStream;

class ModeConfig
{
public:
    ModeConfig(void);
    ~ModeConfig(void);

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned char        m_nModeID;  			//id±àºÅ
    std::string          m_strModelName;
    std::string          m_strChooseMusicIcon;
    std::string          m_strChooseMusicAtlas;

};

#endif//__ITEMINFO_H__


//end file


