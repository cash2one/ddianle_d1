#include "LiveRoomDataMgr.h"
#include "../util/CommonFunc.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../socket/Log.h"
#include "../../socket/Formatter.h"
#include <stdlib.h>

CLiveRoomDataManager::CLiveRoomDataManager()
{
	m_tLiveStartTime = 0;
	m_tLiveEndTime = 0;
	m_nLeaderBoardSize = 0;

	m_tAuditionStartTime = 0;
	m_tAuditionEndTime = 0;
	m_tAuditionDailyStartTime = 0;
	m_tAuditionDailyEndTime = 0;

	m_nBaseScore = 0;
	m_nWinScore = 0;
	m_nLossScore = 0;
	m_nMatchPlayerCount = 0;

	m_nDifficulty = 0;

	m_bEnableVote = false;
	m_nVoteItemID = 0;
	m_nVoteItemHotValue = 0;
	m_nRefreshInterval = 0;
    m_nPunishedTime = 0;
}
CLiveRoomDataManager::~CLiveRoomDataManager()
{

}

CLiveRoomDataManager& CLiveRoomDataManager::Instance()
{
	static CLiveRoomDataManager s_instance;
	return s_instance;
}

void CLiveRoomDataManager::SetLiveRoom(const char *title, const char *rule, 
									   const char *maleCloth, const char *femaleCloth,
									   const char *pStartTime, const char *pEndTime,
									   int nLeaderBoardSize)
{
	m_strTitle = title;
	m_strRule = rule;

	std::vector< std::string > vecItemInfo;
	SplitString(maleCloth, "|", vecItemInfo);
	for (size_t i = 0; i < vecItemInfo.size(); i++)
	{
		m_listMaleDefaultCloth.push_back((itemtype_t)atoi(vecItemInfo[i].c_str()));
	}

	vecItemInfo.clear();
	SplitString(femaleCloth, "|", vecItemInfo);
	for (size_t i = 0; i < vecItemInfo.size(); i++)
	{
		m_listFemaleDefaultCloth.push_back((itemtype_t)atoi(vecItemInfo[i].c_str()));
	}

	m_tLiveStartTime = SetTime(pStartTime);
	m_tLiveEndTime = SetTime(pEndTime);
	if (m_tLiveStartTime >= m_tLiveEndTime)
	{
		WriteLog(LOGLEVEL_ERROR, "ERROR: Critical error: live room match start time[%s] >= end time[%s] !!!!!", 
			pStartTime, pEndTime);
	}
	m_nLeaderBoardSize = nLeaderBoardSize;
}

void CLiveRoomDataManager::SetAudition(
		const char *pStartDate, const char *pEndDate, const char *pStartTime, const char *pEndTime,
		int nBaseScore, int nWinScore, int nLossScore,
		int nMatchPlayerCount, int nDifficulty, int nPunishedTime )
{
	std::string strTimeBuff;
	
	Formatter(strTimeBuff) << pStartDate << " " << pStartTime;
	m_tAuditionStartTime = SetTime(strTimeBuff.c_str());

	Formatter(strTimeBuff) << pEndDate << " " << pEndTime;
	m_tAuditionEndTime = SetTime(strTimeBuff.c_str());

	struct tm tmStartTime, tmEndTime;
	LocalTime(&m_tAuditionStartTime, &tmStartTime);
	LocalTime(&m_tAuditionEndTime, &tmEndTime);

	m_tAuditionDailyStartTime = tmStartTime.tm_hour * 3600 + tmStartTime.tm_min * 60 + tmStartTime.tm_sec;
	m_tAuditionDailyEndTime = tmEndTime.tm_hour * 3600 + tmEndTime.tm_min * 60 + tmEndTime.tm_sec;

	m_nBaseScore = nBaseScore;
	m_nWinScore = nWinScore;
	m_nLossScore = nLossScore;
	m_nMatchPlayerCount = nMatchPlayerCount;
	m_nDifficulty = nDifficulty;
    m_nPunishedTime = nPunishedTime;

}

void CLiveRoomDataManager::AddAuditionMode(int nMode)
{
	m_vecMode.push_back(nMode);
}

void CLiveRoomDataManager::AddAuditionScence(int scenceID)
{
	if (scenceID == 0)
	{
		m_vecScenceID.clear();
		m_vecScenceID.push_back(scenceID);
	}
	else
	{
		if (m_vecScenceID.empty() ||  m_vecScenceID[0] != 0)
		{
			m_vecScenceID.push_back(scenceID);
		}
	}
}

void CLiveRoomDataManager::AddAuditionMusic(int musicID)
{
	if (musicID == 0)
	{
		m_vecMusicID.clear();
		m_vecMusicID.push_back(musicID);
	}
	else
	{
		if (m_vecMusicID.empty() || m_vecMusicID[0] != 0)
		{
			m_vecMusicID.push_back(musicID);
		}
	}
}

int CLiveRoomDataManager::RandomMode() const
{
	if (m_vecMode.size() == 0)
	{
		return (int)MusicMode_Osu;
	}

	if (m_vecMode.size() == 1 && m_vecMode[0] != MusicMode_None)
	{
		return (int)m_vecMode[0];
	}

	if (m_vecMode.size() > 1)
	{
		return m_vecMode[(int)Random(0, (int)(m_vecMode.size()-1))];
	}

	return (int)MusicMode_Osu;
}

int CLiveRoomDataManager::RandomScence() const
{
	if (m_vecScenceID.size() == 0 || (m_vecScenceID.size() > 0 && m_vecScenceID[0] == 0))
	{
		return RAN_SCENE_ID;
	}

	int nScenceID = 0;
	if (m_vecScenceID.size() > 1)
	{
		nScenceID = m_vecScenceID[(int)Random(0, (int)(m_vecScenceID.size()-1))];
	}
	else if (m_vecScenceID.size() == 1)
	{
		nScenceID = m_vecScenceID[0];
	}
	SceneConfig *pScence = ConfigManager::Instance().GetSceneModeManager().GetSceneInfo((unsigned char)nScenceID);
	if (pScence != NULL)
	{
		return nScenceID;
	}
	return RAN_SCENE_ID;
}

int CLiveRoomDataManager::RandomMusic() const
{
	if (m_vecMusicID.size() == 0 || (m_vecMusicID.size() > 0 && m_vecMusicID[0] == 0))
	{
		return RAN_MUSIC_ID;
	}

	int nMusicID = 0;
	if (m_vecMusicID.size() > 1)
	{
		nMusicID = m_vecMusicID[(int)Random(0, (int)(m_vecScenceID.size()-1))];
	}
	else if (m_vecMusicID.size() == 1)
	{
		nMusicID = m_vecMusicID[0];
	}
	MusicConfig *pMusic = ConfigManager::Instance().GetMusicConfigManager().GetByID((unsigned short)nMusicID);
	if (pMusic != NULL)
	{
		return nMusicID;
	}
	return RAN_MUSIC_ID;
}

bool CLiveRoomDataManager::IsAuditionOpen() const
{
	time_t tNow = time(NULL);

	struct tm tmNow;
	LocalTime(&tNow, &tmNow);
	time_t tDailySec = tmNow.tm_hour * 3600 + tmNow.tm_min * 60 + tmNow.tm_sec;

	return (m_tAuditionStartTime <= tNow && m_tAuditionEndTime >= tNow)
		&& (m_tAuditionDailyStartTime <= tDailySec && m_tAuditionDailyEndTime >= tDailySec) ;
}

void CLiveRoomDataManager::SetLiveRoomVote(bool bEnable, itemtype_t nItemID, int nHotValue, int nRefreshInterval, int nAnounceItemCount)
{
	m_bEnableVote = bEnable;
	m_nVoteItemID = nItemID;
	m_nVoteItemHotValue = nHotValue;
	m_nRefreshInterval = nRefreshInterval;
	m_nAnounceItemCount = nAnounceItemCount;
}

bool CLiveRoomDataManager::IsVoteOpen() const
{
	return m_bEnableVote;
}

