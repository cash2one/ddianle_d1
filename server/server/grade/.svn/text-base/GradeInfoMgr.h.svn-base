/********************************************************************
	created:	2011/01/14
	author:		fufeng

*********************************************************************/

#ifndef __GRADE_INFO_MGR_H__
#define __GRADE_INFO_MGR_H__

#include "../../datastructure/Macro_Define.h"
#include <map>


class CGradeInfoManager
{
protected:
	CGradeInfoManager(void);
	~CGradeInfoManager(void);

public:
	static CGradeInfoManager& Instance();

	void AddGradePercentInfo(int nPerfect, int nCool, int nGood, int nBad, int nMiss);
	int GetGradePercent(int nRank);

	void AddGradeExpInfo(int nLevel, int nSSExp, int nSExp, int nAExp, int nBExp, int nCExp, int nDExp);
	int GetGradeExp(int nLevel, int nGrade);

	void AddPlaceExpInfo(int nPlace, int nExpCount4, int nExpCount3, int nExpCount2, int nExpCount1);
	int GetPlaceExp(int nPlace, int nCount);

public:
	int m_szGradePercent[RoomRoundRank_Max];

	int m_szGradeExp[MusicLevel_Max][RoomDanceGrade_Max];			//index1: level, index2: grade
	int m_szPlaceExp[MAX_ROOM_PLAYER_COUNT][MAX_ROOM_PLAYER_COUNT];		//index1: place, index2: dancer count
};


#endif//__MUSIC_BOARD_MGR_H__


