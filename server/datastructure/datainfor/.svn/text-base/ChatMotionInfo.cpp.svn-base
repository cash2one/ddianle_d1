#include "ChatMotionInfo.h"
#include "FileStream.h"

CChatMotionInfo::CChatMotionInfo()
{
	m_motionID = -1;
	m_spriteName = "";
	m_atlasName = "";
}

CChatMotionInfo::~CChatMotionInfo()
{

}

bool CChatMotionInfo::Load(CFileStream &file)
{
	file.ReadShort(m_motionID);

	unsigned short nSize = 0;

	file.ReadUShort(nSize);
	file.ReadString(m_spriteName, nSize);

	file.ReadUShort(nSize);
	file.ReadString(m_atlasName, nSize);

	return true;
}

bool CChatMotionInfo::Save(CFileStream &file)
{
	file.WriteShort((short)m_motionID);

	file.WriteUShort((unsigned short)m_spriteName.length());
	file.WriteString(m_spriteName);

	file.WriteUShort((unsigned short)m_atlasName.length());
	file.WriteString(m_atlasName);

	return true;
}

