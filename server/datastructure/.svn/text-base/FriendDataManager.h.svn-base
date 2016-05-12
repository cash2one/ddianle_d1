#ifndef _FRIEND_DATA_MANAGER_H_
#define _FRIEND_DATA_MANAGER_H_

#include <string>
#include <map>

class CParamPool;

class CFindFriendCondition
{
public:
    CFindFriendCondition();

    void doEncode(CParamPool &ioBuff);

public:
    int m_nIndex;
    std::string m_strDesc;
    int m_nConditionMin;
    int m_nConditionMax;
};

enum eFindFriendType
{
	eFindFriendType_RoleID = 0,
	eFindFriendType_RoleName = 1,
};

class CFriendDataManager
{
public:
    CFriendDataManager();
    ~CFriendDataManager();

    static CFriendDataManager & Instance();

    void SetFindFriend(int nCoolDown, int nPlayerCount);
    void AddFindFriendType(int nIndex, const char *strDesc, int nType);
    void AddFindFriendLevel(int nIndex, const char *strDesc, int nLevelMin, int nLevelMax);
    void AddFindFriendSex(int nIndex, const char *strDesc, int nSex);
    void AddFindFriendAge(int nIndex, const char *strDesc, int nAgeMin, int nAgeMax);
    void AddFindFriendConstellation(int nIndex, const char *strDesc, int nConstellation);
    void SetRecommendFriend(unsigned int nResetTime, unsigned int nCD, unsigned int nRefreshCount);

    CFindFriendCondition *GetTypeCondition(int nIndex);
    CFindFriendCondition *GetLevelCondition(int nIndex);
    CFindFriendCondition *GetSexCondition(int nIndex);
    CFindFriendCondition *GetAgeCondition(int nIndex);
    CFindFriendCondition *GetConstellationCondition(int nIndex);

    unsigned int GetRecommendResetSeconds() const { return m_nRecommendResetSeconds; }
    unsigned int GetRecommendRefreshCount() const { return m_nRecommendRefreshCount; }
    unsigned int GetRecommendCD() const { return m_nRecommendCD; }

protected:
    void AddFindFriendCondition(std::map<int, CFindFriendCondition> &mapCondition,
        int nIndex, const char *strDesc, int nCondMin, int nCondMax);

public:
    int m_nFindFriendCoolDown;
    int m_nFindFriendMaxCount;
    std::map<int, CFindFriendCondition> m_FindFriendType;
    std::map<int, CFindFriendCondition> m_FindFriendLevel;
    std::map<int, CFindFriendCondition> m_FindFriendSex;
    std::map<int, CFindFriendCondition> m_FindFriendAge;
    std::map<int, CFindFriendCondition> m_FindFriendConstellation;

private:
    unsigned int    m_nRecommendResetSeconds;
    unsigned int    m_nRecommendRefreshCount;
    unsigned int    m_nRecommendCD;
};


#endif //_FRIEND_DATA_MANAGER_H_

