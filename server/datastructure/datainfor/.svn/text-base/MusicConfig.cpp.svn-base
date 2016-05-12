#include "./MusicConfig.h"
#include "./FileStream.h"
#include "../../socket/Utility.h"
#include "../../socket/Windefine.h"

MusicConfig::MusicConfig(void)
{
    m_nMusicID = 0;
    m_strMusicName = "";
    m_strSingerName = "";
    m_strCollectionName = "";
    m_nType = 0;
    m_nTime = 0;
    m_nBpm = 0;

    m_strIcon = "";
    m_strAtlas = "";
    m_strMusicSource = "";
    m_strIntro = "";
}

MusicConfig::~MusicConfig(void)
{
}

bool MusicConfig::Load(CFileStream&file)
{
    file.ReadUShort(m_nMusicID);

    unsigned short nSize = 0;
    file.ReadUShort(nSize);
    file.ReadString(m_strMusicName, nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strSingerName, nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strCollectionName, nSize);

    file.ReadChar(m_nType);

    file.ReadUShort(m_nTime);
    file.ReadUShort(m_nBpm);

    file.ReadUShort(nSize);
    file.ReadString(m_strIcon, nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strAtlas, nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strMusicSource, nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strIntro, nSize);
    file.ReadBool(m_bIsFresher);

    return true;
}


bool MusicConfig::Save(CFileStream&file)
{
    file.WriteUShort(m_nMusicID);

    file.WriteUShort((unsigned short)m_strMusicName.length());
    file.WriteString(m_strMusicName);

    file.WriteUShort((unsigned short)m_strSingerName.length());
    file.WriteString(m_strSingerName);

    file.WriteUShort((unsigned short)m_strCollectionName.length());
    file.WriteString(m_strCollectionName);

    file.WriteChar(m_nType);

    file.WriteUShort(m_nTime);
    file.WriteUShort(m_nBpm);

    file.WriteUShort((unsigned short)m_strIcon.length());
    file.WriteString(m_strIcon);

    file.WriteUShort((unsigned short)m_strAtlas.length());
    file.WriteString(m_strAtlas);

    file.WriteUShort((unsigned short)m_strMusicSource.length());
    file.WriteString(m_strMusicSource);

    file.WriteUShort((unsigned short)m_strIntro.length());
    file.WriteString(m_strIntro);
    file.WriteBool(m_bIsFresher);

    return true;
}

bool MusicConfig::Attach(MusicConfig*pSrc)
{
    if (pSrc == this || pSrc == NULL)
        return false;

    m_nMusicID = pSrc->m_nMusicID;

    m_strMusicName = pSrc->m_strMusicName;              //音乐名字
    m_strSingerName = pSrc->m_strSingerName;            //歌手名字
    m_strCollectionName = pSrc->m_strCollectionName;
    m_nType = pSrc->m_nType;                            //歌曲类型
    m_nTime = pSrc->m_nTime;                            //这首歌曲的时长
    m_nBpm = pSrc->m_nBpm;

    m_strIntro = pSrc->m_strIntro;
    m_strAtlas = pSrc->m_strAtlas;
    m_strMusicSource = pSrc->m_strMusicSource;
    m_strIntro = pSrc->m_strIntro;                      //tips

    return true;
}

