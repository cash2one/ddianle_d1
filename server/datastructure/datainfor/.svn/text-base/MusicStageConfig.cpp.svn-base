#include "./MusicStageConfig.h"
#include "./FileStream.h"
#include "../../socket/Utility.h"
#include "../../socket/Windefine.h"

MusicStageConfig::MusicStageConfig(void)
    :m_nMusicID(0)
    ,m_nModeID(0)
    ,m_nLevel(0)
    ,m_nStar(0)
    ,m_bNewPlayer(true)
{
}

MusicStageConfig::~MusicStageConfig(void)
{
}

bool MusicStageConfig::Load(CFileStream&file)
{
    file.ReadUShort(m_nMusicID);
    file.ReadChar(m_nModeID);
    file.ReadChar(m_nLevel);
    file.ReadChar(m_nStar);

    return true;
};

bool MusicStageConfig::Save(CFileStream&file)
{
    file.WriteUShort(m_nMusicID);
    file.WriteChar(m_nModeID);
    file.WriteChar(m_nLevel);
    file.WriteChar(m_nStar);

    return true;
};

//end file


