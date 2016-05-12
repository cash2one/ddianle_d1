#include "BGMMgr.h"
#include <algorithm>


CBGMMgr::CBGMMgr()
{

}

CBGMMgr::~CBGMMgr()
{

}

CBGMMgr & CBGMMgr::instance()
{
	static CBGMMgr s_BGMMgr;
	return s_BGMMgr;
}



void CBGMMgr::AddMusic(unsigned short nMusicID)
{
	if ( nMusicID > 0 ) {
		list<unsigned short>::iterator itr =  find(m_listMusic.begin(), m_listMusic.end(), nMusicID);
		if (itr == m_listMusic.end()) {
			m_listMusic.push_back(nMusicID);	
		}
	}
}

void CBGMMgr::SerializeBGMInfo(CParamPool & IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_listMusic.size());
	for (list<unsigned short>::iterator itr = m_listMusic.begin(); itr != m_listMusic.end(); ++itr) {
		IOBuff.AddUShort(*itr);
	}
}

const char* CBGMMgr::SerializeName()
{
	return "BGM";
}

const list<unsigned short> & CBGMMgr::GetBGMusiList()
{
	return m_listMusic;
}


