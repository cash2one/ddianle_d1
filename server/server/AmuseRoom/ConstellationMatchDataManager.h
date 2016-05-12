#ifndef _CONSTELLATION_MATCH_DATA_MANAGER_H_
#define _CONSTELLATION_MATCH_DATA_MANAGER_H_

#include <string>
#include <vector>
#include <map>

#include "../../datastructure/Macro_Define.h"

class ConstellationMatchDataManager
{
public:
	struct SOpenTimePair
	{
		int m_nWeekday;
		int m_nStartTime;
		int m_nEndTime;

		SOpenTimePair(int w = 0, int ns = 0, int ne = 0)
		{
			m_nWeekday = w;
			m_nStartTime = ns;
			m_nEndTime = ne;
		}
	};

public:
	ConstellationMatchDataManager(void);
	~ConstellationMatchDataManager(void);

	static ConstellationMatchDataManager &Instance(void);

public:
	void Init();

	void SetConstellationMatch(int nLevel, const char *strFisrtEnterTip, int nPriorityCardCount, int nPriorityCardCoolDown, int nMaxUseTimes, int nRoomCloseTime, int nMatchInteval, const char *strRule);

	void AddOpenTime(int nWeekday, const char *strStartTime, const char *strEndTime);
	void AddMatchTable(int nFisrtConstellation, int nSecondConstellation);
	
	void AddWishWords(const char *strWords);
	void AddMatchRate(int nFisrtConstellation, int nSecondConstellation, int nRate);
	
	void AddAmuseSceneID(int nScenceID);

public:
	bool IsOpen() const;
	bool IsInCurrentOpenTime(unsigned int nTime, unsigned int nNow) const;
	int LeftOpenTime() const;
	int WillOpenTime() const;

	const std::vector<int> *GetMatchConstellation(int nConstellation) const;
	void RandomMatchConstellation(int nConstellation, std::vector<int> &vecMatchConstellation) const;
	int GetMatchRate(int nConstellation, int nOtherConstellation) const;
	const char *RandomWishWords() const;
	int RandomAmuseScene() const;

protected:
	int _LeftOpenTime(unsigned int nTime) const;

public:
	std::vector<SOpenTimePair> m_vecOpenTime;

	int m_nRoleLevelRequired;
	std::string m_strFirstEnterTip;
	int m_nPriorityCardCount;
	int m_nPriorityCardCoolDown;
	int m_nPriorityCardMaxUseTimes;
	int m_nRoomCloseTime;
	int m_nMatchInterval;
	std::string m_strRule;

	std::vector<int> m_vecMatchTable[MAX_CONSTELLATION_COUNT];
	std::vector<std::string> m_vecWishWords;
	int m_szMatchRate[MAX_CONSTELLATION_COUNT][MAX_CONSTELLATION_COUNT];

	std::vector<int> m_vecSceneID;
	
};

#endif //_CONSTELLATION_LOVE_DATA_MANAGER_H_

