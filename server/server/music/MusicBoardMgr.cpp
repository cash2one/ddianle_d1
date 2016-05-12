#include "MusicBoardMgr.h"
#include "../../socket/Utility.h"


CMusicBoardManager::CMusicBoardManager(void)
{
}

CMusicBoardManager::~CMusicBoardManager(void)
{
}

CMusicBoardManager& CMusicBoardManager::Instance()
{
	static CMusicBoardManager stcBoardMgr;
	return stcBoardMgr;
}

void CMusicBoardManager::AddBoardInfo(unsigned short nMusicID, bool bNew, bool bHot)
{
	if ( bNew || bHot )
	{
		MusicBoardInfo boardInfo;
		boardInfo.m_nMusicID = nMusicID;
		boardInfo.m_bNew = bNew;
		boardInfo.m_bHot = bHot;

		m_BoardMap.insert( std::make_pair( nMusicID, boardInfo ) );
	}
}


void CMusicBoardManager::AddFresherSongInfo(unsigned char nMusicMode, int nMusicID)
{
	if (nMusicMode >= MusicMode_Max)
		return;
	m_fresherSong[nMusicMode].push_back(nMusicID);
}

int CMusicBoardManager::RandomFresherSong(unsigned char nMusicMode)
{
	int nMusicID = RAN_MUSIC_ID;

	if (nMusicMode < MusicMode_Max)
	{
		int nMusicCount = (int)m_fresherSong[nMusicMode].size();
		if (nMusicCount > 0)
		{
			int nRandomIndex = Random( 0, nMusicCount - 1);
			nMusicID = m_fresherSong[nMusicMode][nRandomIndex];
		}
	}

	return nMusicID;
}


const char* CMusicBoardManager::SerializeName()
{
	return "MusicBoard";
}

void CMusicBoardManager::SerializeBoardInfo(CParamPool& IOBuff)
{
	unsigned short nCount = (unsigned short)m_BoardMap.size();
	IOBuff.AddUShort( nCount );

	for ( MusicBoardInfoMap::iterator it = m_BoardMap.begin(); it != m_BoardMap.end(); ++it )
	{
		it->second.SerializeInfo( IOBuff );
	}

	nCount = 0;
	int i = (int)MusicMode_None + 1;
	while ( i < MusicMode_Max )
	{
		if ( m_fresherSong[i].size() > 0 )
		{
			nCount++;
		}
		i++;
	}

	IOBuff.AddUShort(nCount);
	i = (int)MusicMode_None + 1;
	while ( i < MusicMode_Max )
	{
		if ( m_fresherSong[i].size() > 0 )
		{
			IOBuff.AddUChar((unsigned char)i);
			unsigned short nCount = (unsigned short)m_fresherSong[i].size();
			IOBuff.AddUShort( nCount );

			for ( size_t j = 0;j < m_fresherSong[i].size(); j++)
			{
				IOBuff.AddUShort((unsigned short)m_fresherSong[i][j]);
			}
		}
		i++;
	}

}
//end file


