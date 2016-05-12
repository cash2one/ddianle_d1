#include "ExpConfig.h"
#include "./FileStream.h"

bool ExpConfig::Load(CFileStream&file)
{
    file.ReadUShort(m_nLev) ;
    file.ReadInt(m_nExp) ;
    return true ;
}

bool ExpConfig::Save(CFileStream&file)
{
    file.WriteUShort(m_nLev) ;
    file.WriteInt(m_nExp) ;
    return true ;
}

