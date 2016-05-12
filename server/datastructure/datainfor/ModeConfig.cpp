#include "./ModeConfig.h"
#include "./FileStream.h"
#include "../../socket/Utility.h"
#include "../../socket/Windefine.h"

ModeConfig::ModeConfig(void)
    :m_nModeID(0)
    ,m_strModelName("")
    ,m_strChooseMusicIcon("")
    ,m_strChooseMusicAtlas("")
{
}

ModeConfig::~ModeConfig(void)
{
}

bool ModeConfig::Load(CFileStream&file)
{
    file.ReadUChar(m_nModeID);

    unsigned short nSize = 0;

    file.ReadUShort(nSize);
    file.ReadString(m_strModelName,nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strChooseMusicIcon,nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strChooseMusicAtlas,nSize);	

    return true;
};

bool ModeConfig::Save(CFileStream&file)
{
    file.WriteUChar(m_nModeID);

    file.WriteUShort((unsigned short)m_strModelName.length());
    file.WriteString(m_strModelName);

    file.WriteUShort((unsigned short)m_strChooseMusicIcon.length());
    file.WriteString(m_strChooseMusicIcon);

    file.WriteUShort((unsigned short)m_strChooseMusicAtlas.length());
    file.WriteString(m_strChooseMusicAtlas);

    return true;
};




//end file



