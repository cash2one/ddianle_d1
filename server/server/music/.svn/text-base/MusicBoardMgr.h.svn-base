/********************************************************************
	created:	2011/01/14
	author:		fufeng

*********************************************************************/

#ifndef __MUSIC_BOARD_MGR_H__
#define __MUSIC_BOARD_MGR_H__

#include "../../socket/ParamPool.h"
#include "../../datastructure/Macro_Define.h"
#include <map>
#include <vector>

class CParamPool;


struct MusicBoardInfo
{
	unsigned short m_nMusicID;
	bool m_bNew;
	bool m_bHot;

	MusicBoardInfo()
	{
		m_nMusicID = 0;
		m_bNew = false;
		m_bHot = false;
	}

	void SerializeInfo(CParamPool& IOBuff)
	{
		IOBuff.AddUShort( m_nMusicID );
		IOBuff.AddBool( m_bNew );
		IOBuff.AddBool( m_bHot );
	}
};

typedef std::map<unsigned short, MusicBoardInfo> MusicBoardInfoMap;
typedef std::vector<int> FresherMusicVec;


class CMusicBoardManager
{
protected:
	CMusicBoardManager(void);
	~CMusicBoardManager(void);

public:
	static CMusicBoardManager& Instance();

	void AddBoardInfo(unsigned short nMusicID, bool bNew, bool bHot);

	void AddFresherSongInfo(unsigned char nMusicMode, int nMusicID);
	int RandomFresherSong(unsigned char nMusicMode);

	const char* SerializeName();
	void SerializeBoardInfo(CParamPool& IOBuff);

public:
	MusicBoardInfoMap m_BoardMap;

	FresherMusicVec m_fresherSong[MusicMode_Max];
};


#endif//__MUSIC_BOARD_MGR_H__


