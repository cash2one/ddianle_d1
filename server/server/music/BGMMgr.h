#ifndef		__BGMMGR_H__
#define		__BGMMGR_H__

#include <list>
#include "../../socket/ParamPool.h"

using namespace std;

class CBGMMgr
{
private:
	CBGMMgr();
	~CBGMMgr();
public:
	static CBGMMgr & instance();
	void AddMusic(unsigned short nMusicID);
	void SerializeBGMInfo(CParamPool & IOBuff);
	const char *SerializeName();

	const list<unsigned short> & GetBGMusiList();
public:
	list<unsigned short>	m_listMusic;
};





#endif

