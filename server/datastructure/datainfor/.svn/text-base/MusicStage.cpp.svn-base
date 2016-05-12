#include "./MusicStage.h"
#include "./FileStream.h"
#include "../../socket/Utility.h"
#include "../../socket/Windefine.h"

CMusicStage::CMusicStage(void)
{
    m_nMusicID = 0;
    m_nModeID = 0;
    m_nLevel = 0;
    m_nStar = 0;
    m_bNewPlayer = true;
}

CMusicStage::~CMusicStage(void)
{
}

bool CMusicStage::Load(CFileStream&file)
{
    file.ReadUShort(m_nMusicID);
    file.ReadChar(m_nModeID);
    file.ReadChar(m_nLevel);
    file.ReadChar(m_nStar);

    return true;
};

bool CMusicStage::Save(CFileStream&file)
{
    file.WriteUShort(m_nMusicID);
    file.WriteChar(m_nModeID);
    file.WriteChar(m_nLevel);
    file.WriteChar(m_nStar);

    return true;
};

//end file


