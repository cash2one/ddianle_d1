#ifndef __LIVE_ROOM_DATA_MGR_H__
#define __LIVE_ROOM_DATA_MGR_H__

#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DataStruct_Base.h"

#include <time.h>
#include <string>
#include <list>
#include <vector>

class CLiveRoomDataManager
{
protected:
	CLiveRoomDataManager();
	~CLiveRoomDataManager();

public:
	static CLiveRoomDataManager& Instance();

	void SetLiveRoom(const char *pTitle, const char *pRule, 
		const char *pMaleCloth, const char *pFemaleCloth, 
		const char *pStartTime, const char *pEndTime,
		int nLeaderBoardSize);

	void SetAudition(
		const char *pStartDate, const char *pEndDate, const char *pStartTime, const char *pEndTime,
		int nBaseScore, int nWinScore, int nLossScore,
		int nMatchPlayerCount, int nDifficulty, int nPunishedTime );
	void AddAuditionMode(int nMode);
	void AddAuditionScence(int nScenceID);
	void AddAuditionMusic(int nMusicID);

	int RandomMode() const;
	int RandomScence() const;
	int RandomMusic() const;

	bool IsAuditionOpen() const;

	void SetLiveRoomVote(bool bEnable, itemtype_t nItemID, int nHotValue, int nRefreshInterval, int nAnounceItemCount);

	bool IsVoteOpen() const;


    unsigned int GetPunishedTimeByOffline(){ return m_nPunishedTime; }

public:
	// live room
	std::string m_strTitle;
	std::string m_strRule;
	std::list<itemtype_t> m_listMaleDefaultCloth;
	std::list<itemtype_t> m_listFemaleDefaultCloth;
	time_t m_tLiveStartTime, m_tLiveEndTime; // 整个比赛的开始结束时间
	int m_nLeaderBoardSize;

	// audition

	time_t m_tAuditionStartTime, m_tAuditionEndTime;// 海选开始结束时间
	time_t m_tAuditionDailyStartTime, m_tAuditionDailyEndTime;


	int m_nBaseScore;
	int m_nWinScore;
	int m_nLossScore;
	int m_nMatchPlayerCount;

	int m_nDifficulty;
	std::vector<int> m_vecMode;
	std::vector<int> m_vecScenceID;
	std::vector<int> m_vecMusicID;

	// vote info
	bool m_bEnableVote;
	itemtype_t m_nVoteItemID;
	int m_nVoteItemHotValue;
	int m_nRefreshInterval;
	int m_nAnounceItemCount;

    unsigned int m_nPunishedTime;    //海选中掉线惩罚时间
};


#endif

