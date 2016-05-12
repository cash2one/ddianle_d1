#ifndef	__PUZZLE_DATA_MGR_H__
#define	__PUZZLE_DATA_MGR_H__

#include "../item/ItemProcess.h"
#include <map>


class CPuzzleInfo
{
public:
	CPuzzleInfo();
	~CPuzzleInfo();

public:
	int m_nPuzzleID;
	bool m_bAnnounce;

	std::string m_strTitle;
	std::string m_strTexture;

	CItem m_NormalCost;
	CItem m_SpecialCost;

	int m_nMoneyReward;
	std::list<CItem> m_lstMaleReward;
	std::list<CItem> m_lstFemaleReward;
};


class CPuzzleDataMgr
{
public:
	CPuzzleDataMgr();
	~CPuzzleDataMgr();

public:
	static CPuzzleDataMgr& Instance();

	bool InitPuzzle();

	bool IsOpening() const;
	void SetIsOpening(bool bIsOpening);

	int PuzzleBeginTime() const;
	int PuzzleEndTime() const;

	void AddPuzzleBriefInfo(int nBeginTime, int nEndTime, const char* pHelpTitle, const char* pHelpContent);
	void AddPuzzleDetailInfo(int nPuzzleID, const char* pTitle, const char* pTextute, bool bAnnounce,
		int nNormalType, int nNormalCount, int nSpecialType, int nSpecialCount, const char* pMaleReward, const char* pFemaleReward, int nMoneyReward);

public:
	bool m_bIsOpening;

	int m_nPuzzleBeginTime;
	int m_nPuzzleEndTime;
	std::string m_strPuzzleHelpTitle;
	std::string m_strPuzzleHelpContent;

	std::map<int, CPuzzleInfo> m_mapAllPuzzle;
};

#endif


