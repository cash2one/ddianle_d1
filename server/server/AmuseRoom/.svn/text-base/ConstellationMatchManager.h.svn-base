#ifndef _CONSTELLATION_MATCH_MANAGER_H_
#define _CONSTELLATION_MATCH_MANAGER_H_

#include <map>
#include <set>

#include "AmuseRoomComponent.h"
#include "../../datastructure/Macro_Define.h"

class ISQLLog;

class ConstellationMatchManager
{
public:
	ConstellationMatchManager(void);
	~ConstellationMatchManager(void);

	static ConstellationMatchManager &Instance();

	void OnUpdate(int nTimeElapse);

	void OnLogin(CAmuseRoomComponent &rComponent);
	void OnLogout(CAmuseRoomComponent &rComponent);

	void OnEnterConstellationLobby(GameMsg_Base &rMsg, CAmuseRoomComponent &rComponent);
	void OnQuitConstellationLobby(GameMsg_Base &rMsg, CAmuseRoomComponent &rComponent);

	void OnMatch(GameMsg_Base &rMsg, CAmuseRoomComponent &rComponent);
	void OnEnterConstellationRoom(GameMsg_Base &rMsg, CAmuseRoomComponent &rComponent);

private:
	void Match();
	unsigned int GetMatchRoleID(unsigned int nRoleID, int nMatchConstellation);
	void OnMatchSuc(unsigned int nRoleID, unsigned int nMatchRoleID);
	void OnMatchFail(unsigned int nRoleID);

	void AddMatch(unsigned nRoleID, int nConstellation, int nMatchConstellation);
	void QuitMatch(unsigned int nRoleID);

	void AddQuitMatchList(unsigned int nRoleID);
	bool InQuitMatchList(unsigned int nRoleID) const;

	void LogRoleEnterLobby(unsigned int nRoleID, const std::string &strRoleName);
	void LogRoleUseCard(unsigned int nRoleID, const std::string &strRoleName, int nCardConstellation);
	void LogRoleMatchFail(unsigned int nRoleID, const std::string &strRoleName);
	void LogRoleMatchSuc(unsigned int nRoleID, const std::string &strRoleName, int nCardConstellation, 
		unsigned int nMatchRoleID, const std::string &strMatchRoleName, int nSceneID, const std::string &strSceneName);
	void LogRoleAction(ISQLLog *pLog);

protected:
	// all player in constellation love lobby, key - role ID, value - role constellation
	std::map<unsigned int, unsigned char> m_LobbyPlayer;

	// all matches in lobby, value - role id, array index - to match constellation
	std::set<unsigned int> m_MatchSet[MAX_CONSTELLATION_MATCH];
	// all matches in lobby, value - role id, array index - self constellation
	std::set<unsigned int> m_SelfSet[MAX_CONSTELLATION_COUNT];
	// all matches in lobby, value - role id
	std::set<unsigned int> m_AllMatchSet;

	std::set<unsigned int> m_QuitMatchRoleID;

	// match time
	int m_nMatchTime;

	// activity open notice time
	unsigned int m_nNextNoticeTime;
};

#endif

