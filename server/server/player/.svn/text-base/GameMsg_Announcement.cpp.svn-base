#include "GameMsg_Announcement.h"
#include "../share/ServerMsgDef.h"

GameMsg_C2S_GetAnnouncement::GameMsg_C2S_GetAnnouncement(): 
GameMsg_Base(MSG_C2S_GetAnnoucement),
m_nVersion(0)
{
}

GameMsg_C2S_GetAnnouncement::~GameMsg_C2S_GetAnnouncement()
{
}

bool GameMsg_C2S_GetAnnouncement::doDecode(CParamPool &IOBuff)
{
	m_nVersion = IOBuff.GetInt();

	return true;
}


GameMsg_S2C_UPdateAnnouncement::GameMsg_S2C_UPdateAnnouncement():
GameMsg_Base(MSG_S2C_UpdateAnnouncement)
{
}

GameMsg_S2C_UPdateAnnouncement::~GameMsg_S2C_UPdateAnnouncement()
{
}

bool GameMsg_S2C_UPdateAnnouncement::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nVersion);
	IOBuff.AddUShort((unsigned short)m_vecAnnoucement.size());
	if (m_vecAnnoucement.size() > 0) {
		for (vector<CAnnouncement>::iterator itr = m_vecAnnoucement.begin(); itr != m_vecAnnoucement.end(); ++itr) {
			(*itr).doEncode(IOBuff);
		}
	}

	return true;
}

GameMsg_S2C_AnnouncementUpdated::GameMsg_S2C_AnnouncementUpdated():
GameMsg_Base(MSG_S2C_AnnoucementUpdated)
{
}

GameMsg_S2C_AnnouncementUpdated::~GameMsg_S2C_AnnouncementUpdated()
{
}

bool GameMsg_S2C_AnnouncementUpdated::doEncode(CParamPool &IOBuff)
{
	return true;
}


