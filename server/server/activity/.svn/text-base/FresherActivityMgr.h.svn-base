#ifndef _FRESHER_ACTIVITY_MGR_H_
#define	_FRESHER_ACTIVITY_MGR_H_

#include <list>
#include <map>
#include <string>
#include "../../datastructure/DataStruct_Base.h"
#include "ActivityDataStruct.h"

class CFresherActivityManager
{
private:
    CFresherActivityManager();
    ~CFresherActivityManager();
public:
    static CFresherActivityManager & Instance();

public:
    void SetActivity(const char *pszName, const char *pszContent,
        const char *pszMailTitle, const char *pszMailContent,
        int nRecvAgainMoney, int nDurationDays, int nCumulationDays);

    void AddFresherReward(const CFresherReward & malereward, const CFresherReward & femalereward);
    void AddCumulationReward(const char *maleItem, const char *femaleItem, int nMoney, int nBindCoin, const char *vipMaleItem, const char *vipFemaleItem, int nVipMoney, int nVipBindCoin);

    const CFresherReward *GetFresherReward(int nDay, unsigned char m_nSex) const;
    const CFresherReward &GetCumulationReward(unsigned char m_nSex) const;

    bool IsOpen();

public:
    bool m_bIsOpen;

    std::string m_strActivityTitle;
    std::string m_strActivityContent;
    std::string m_strMailTitle;
    std::string m_strMailContent;

    int m_nRecvAgainMoney;	// 补收奖励需要的金券

    int m_nDurationDays;
    int m_nCumulationDays;

    std::map< int, CFresherReward > m_mapMaleReward;
    std::map< int, CFresherReward > m_mapFemaleReward;

    CFresherReward m_maleCumulationReward;
    CFresherReward m_femaleCumulationReward;
};

#endif //_FRESHER_ACTIVITY_MGR_H_

