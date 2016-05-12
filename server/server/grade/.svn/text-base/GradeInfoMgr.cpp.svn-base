#include "GradeInfoMgr.h"


CGradeInfoManager::CGradeInfoManager(void)
{
	for ( int i = 0; i < RoomRoundRank_Max; ++i )
	{
		m_szGradePercent[i] = 0;
	}

	for ( int i = 0; i < MusicLevel_Max; ++i )
	{
		for ( int j = 0; j < RoomDanceGrade_Max; ++j )
		{
			m_szGradeExp[i][j] = 0;
		}
	}

	for ( int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i )
	{
		for ( int j = 0; j < MAX_ROOM_PLAYER_COUNT; ++j )
		{
			m_szPlaceExp[i][j] = 0;
		}
	}
}

CGradeInfoManager::~CGradeInfoManager(void)
{
}

CGradeInfoManager& CGradeInfoManager::Instance()
{
	static CGradeInfoManager stcGradeMgr;
	return stcGradeMgr;
}

void CGradeInfoManager::AddGradePercentInfo(int nPerfect, int nCool, int nGood, int nBad, int nMiss)
{
	m_szGradePercent[RoomRoundRank_Perfect] = nPerfect;
	m_szGradePercent[RoomRoundRank_Cool] = nCool;
	m_szGradePercent[RoomRoundRank_Good] = nGood;
	m_szGradePercent[RoomRoundRank_Bad] = nBad;
	m_szGradePercent[RoomRoundRank_Miss] = nMiss;
}

int CGradeInfoManager::GetGradePercent(int nRank)
{
	if ( nRank > RoomRoundRank_None && nRank < RoomRoundRank_Max )
	{
		return m_szGradePercent[nRank];
	}

	return 0;
}

void CGradeInfoManager::AddGradeExpInfo(int nLevel, int nSSExp, int nSExp, int nAExp, int nBExp, int nCExp, int nDExp)
{
	if ( nLevel > MusicLevel_None && nLevel < MusicLevel_Max )
	{
		m_szGradeExp[nLevel][RoomDanceGrade_SS] = nSSExp;
		m_szGradeExp[nLevel][RoomDanceGrade_S] = nSExp;
		m_szGradeExp[nLevel][RoomDanceGrade_A] = nAExp;
		m_szGradeExp[nLevel][RoomDanceGrade_B] = nBExp;
		m_szGradeExp[nLevel][RoomDanceGrade_C] = nCExp;
		m_szGradeExp[nLevel][RoomDanceGrade_D] = nDExp;
	}
}

int CGradeInfoManager::GetGradeExp(int nLevel, int nGrade)
{
	if ( nLevel > MusicLevel_None && nLevel < MusicLevel_Max && nGrade > RoomDanceGrade_None && nGrade < RoomDanceGrade_Max )
	{
		return m_szGradeExp[nLevel][nGrade];
	}

	return 0;
}

void CGradeInfoManager::AddPlaceExpInfo(int nPlace, int nExpCount4, int nExpCount3, int nExpCount2, int nExpCount1)
{
	if ( nPlace > 0 && nPlace <= MAX_ROOM_PLAYER_COUNT )
	{
		int nIndex = nPlace - 1;

		m_szPlaceExp[nIndex][0] = nExpCount1;
		m_szPlaceExp[nIndex][1] = nExpCount2;
		m_szPlaceExp[nIndex][2] = nExpCount3;
		m_szPlaceExp[nIndex][3] = nExpCount4;
	}
}

int CGradeInfoManager::GetPlaceExp(int nPlace, int nCount)
{
	if ( nPlace > 0 && nPlace <= MAX_ROOM_PLAYER_COUNT && nCount > 0 && nCount <= MAX_ROOM_PLAYER_COUNT )
	{
		return m_szPlaceExp[nPlace - 1][nCount - 1];
	}

	return 0;
}

//end file


