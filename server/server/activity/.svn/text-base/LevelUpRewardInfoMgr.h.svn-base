#ifndef __LEVELUPREWARDINFOMGR_H__
#define __LEVELUPREWARDINFOMGR_H__

#include <map>
#include <list>
#include "../../datastructure/DataStruct_Base.h"


class CLevelRewardInfo
{
public:
    CLevelRewardInfo();
    ~CLevelRewardInfo();
public:
    unsigned short m_nLevel;
    std::list<CItem> m_listMaleItem;
    std::list<CItem> m_listFemaleItem;
    int m_nMoney;
    int m_nBindMCoin;
};

class CLevelUpFunction
{
public:
    CLevelUpFunction();
    ~CLevelUpFunction();
public:
    unsigned short m_nLevel;
    int m_nFuncID;
    std::string m_strFuncName;
};

class CLevelUpInfoMgr
{
private:
    CLevelUpInfoMgr();
    ~CLevelUpInfoMgr();
public:
    static CLevelUpInfoMgr & Instance();	
public:
    void AddRewardInfo(unsigned short nLevel, const CLevelRewardInfo &rRewardInfo, bool bVIPReward);
    bool GetRewardInfo(unsigned short nLevel, CLevelRewardInfo &rRewardInfo);
    bool GetVIPRewardInfo(unsigned short nLevel, CLevelRewardInfo &rRewardInfo);

    bool IsRewardLevel(unsigned short nCurLevel);
    bool IsVIPRewardLevel(unsigned short nCurLevel);
    unsigned short GetNextRewardLevel(unsigned short nCurLevel);

    void AddLevelUpRecommend(unsigned short nLevel, int nFuncID, const char *strFuncName);
    bool GetLevelUpRecommend(unsigned short nLevel, int &nFuncID, std::string &strFuncName);

public:
    std::map<unsigned short, CLevelRewardInfo> m_mapLevelUpRewardInfo;     // key->level, value->reward
    std::map<unsigned short, CLevelRewardInfo> m_mapVIPLevelUpRewardInfo;  // key->level, value->reward
    std::map<unsigned short, CLevelUpFunction> m_mapOpenFunction;          // key->level, value->function
};



#endif //__LEVELUPREWARDINFOMGR_H__


