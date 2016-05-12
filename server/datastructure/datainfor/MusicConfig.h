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

    //���ƺ������൱��=
    bool Attach(MusicConfig*pSrc);

public:
    unsigned short         m_nMusicID;            //����id���
    std::string            m_strMusicName;        //��������
    std::string            m_strSingerName;       //��������
    std::string            m_strCollectionName;   //ר������
    char                   m_nType;               //��������
    unsigned short         m_nTime;               //���׸�����ʱ��
    unsigned short         m_nBpm;                //���׸���bpm
    std::string            m_strIcon;             //Icon
    std::string            m_strAtlas;            //Atlas
    std::string            m_strMusicSource;      //��Դ����
    std::string            m_strIntro;            //tips
    bool                   m_bIsFresher;          //�Ƿ������ְ����֣�����С������ɸѡ
};

#endif//__MUSICINFO_H__

