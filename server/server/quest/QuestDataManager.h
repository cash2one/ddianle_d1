#ifndef _FRESHER_QUEST_DATA_MANAGER_H_
#define _FRESHER_QUEST_DATA_MANAGER_H_

#include "../util/CommonFunc.h"
#include <list>
#include <map>
#include <string>
#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

class CFresherQuestDoc
{
public:
    CFresherQuestDoc();
    CFresherQuestDoc(
        int nDay, int nQuestID, int nTargetType, int nTarget, const char *pszDesc,
        const char *pszMaleItem, const char *pszFemaleItem, int nMoney, int nBingMCoin,
        const char * pszVipMaleItem, const char * pszVipFemaleItem, int nVipMoney, int nVipBindMCoin);
    ~CFresherQuestDoc();

    void doEncode(CParamPool &ioBuff, unsigned char nSex);

public:
    int m_nDay;
    int m_nQuestID;
    int m_nTargetType;
    int m_nTarget;
    std::string m_strQuestDesc;
    CReward m_MaleReward;
    CReward m_FemaleReward;
    CReward m_VipMaleReward;
    CReward m_VipFemaleReward;
};

class CFresherQuestDataManager
{
public:
    static CFresherQuestDataManager & Instance();

protected:
    CFresherQuestDataManager();
    ~CFresherQuestDataManager();

public:
    void SetFresherQuest(
        const char *pszName, const char *pszDesc, 
        const char *pszMailTitle, const char *pszMailContent, 
        int nDuration, const char *pszStartTime, const char *pszEndTime,
        const char *pszMailVipTitle, const char *pszMailVipContent);

    void AddFresherQuest(
        int nDay, int nQuestTargetType, int nQuestTarget, const char *pszDesc,
        const char *pszMaleItem, const char *pszFemaleItem, int nMoney, int nBindMCoin,
        const char * pszVipMaleItem , const char * pszVipFemaleItem, int nVipMoney, int nVipBindMCoin);
    CFresherQuestDoc *GetFresherQuest(int nDay);

    unsigned short GetFresherQuestID(int nDay);

    bool IsOpen() const;
    void LoadFresherQuestBinaryData();

    int ToDayIndex(int nQuestID);

    void doEncode(CParamPool &ioBuff, unsigned char nSex);

private:
    int MakeFresherQuestID(int nDay);

public:
    std::string m_strQuestName;
    std::string m_strQuestDescription;
    std::string m_strMailTitle;
    std::string m_strMailContent;
    std::string m_strVipRewardTitle;
    std::string m_strVipRewardContent;
    int m_nQuestDuration;

    unsigned int m_nStartTime;
    unsigned int m_nEndTime;

    std::map<int, CFresherQuestDoc* > m_mapFresherQuest;

};

#endif //_FRESHER_QUEST_DATA_MANAGER_H_

