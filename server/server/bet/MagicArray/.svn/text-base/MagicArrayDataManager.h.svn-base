#ifndef __MAGICLUCKDRAW_DATAMANAGER_H__
#define __MAGICLUCKDRAW_DATAMANAGER_H__

#include <set>
#include <vector>
#include <map>
#include "../../../datastructure/DataStruct_Base.h"

class MagicArrayDataManager
{
public:
    MagicArrayDataManager();
    virtual ~MagicArrayDataManager(); 

    static MagicArrayDataManager & Instance();

public:
    void AddMagicLuckDrawReward(int nRewardID, const CItem & maleItem, const CItem & femaleItem, int nRate);
    void AddTenRollRate(int nRollIndex, int nRewardID, int nRate);
    void AddOnceRollRate(int nRate_1, int nRate_2, int nRate_3, int nRate_4);
    void AddOnceRollCost(int nCost_1, int nCost_2, int nCost_3, int nCost_4);
    void AddFirstRollRate(int nRate_1, int nRate_2, int nRate_3, int nRate_4);
    void AddMagicLuckDrawConfig(unsigned int nInterval, unsigned int nBastRollCount, unsigned int nResetTime, unsigned int nTenRollCost, bool bResetFirstFree);

public:
    unsigned int GetFreeIntervalTime();
    unsigned int GetTenRollCost();
    unsigned int GetOnceRollCost(unsigned int nRollCount);
    unsigned int GetAccumulatedCountConfig();
    int RandomInOnce();
    int RandomInFirst();
    void RandomInTen(std::vector<int> & vecGroupID);

private:
    int RollOnce(const vector<int> & vecRollRate);

private:
    unsigned int                m_nFreeInterval;
    unsigned int                m_nBastRollLuckDrawCount;
    unsigned int                m_nNextDayResetTime;
    std::vector<int>            m_vecOnceRollRate;
    std::map<int,std::vector<int> > m_mapTenRollRate;
    std::vector<int>            m_vecFirstRollRate;
    std::vector<int>            m_vecOnceRollCost;
    unsigned int                m_nTenRollCost;
    bool                        m_bResetFirstFree;
};

#endif // __LUCKDRAWCONFIGMANAGER_H__

