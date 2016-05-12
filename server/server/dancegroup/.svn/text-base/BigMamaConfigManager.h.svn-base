#ifndef __BIGMAMACONFIGMGR_H__
#define __BIGMAMACONFIGMGR_H__

#include <string>
#include <map>
#include <vector>
#include "../../datastructure/Macro_Define.h"
#include "BigMamaConfig.h"

class CFileStream;
class BigMamaConfigManager
{
public:
    BigMamaConfigManager();
    virtual ~BigMamaConfigManager();

    static BigMamaConfigManager & Instance();

public:
    unsigned int StartTime() const;
    unsigned int EndTime() const;
    unsigned char MaxRound() const;
    unsigned int XiaoLianID() const;
    unsigned int XiaoRuanID() const;
    std::string AnnouncementString() const;
    unsigned int AnnouncementSpace() const;
//     bool IsOpen() const;
    std::string IntroductString() const;

    // 没有则返回空
    BigMamaReward* GetBigMamaReward(EBigMamaRewardsType rewardType);
    // 取得关卡，如果关卡为空，则取得小恋或者小软
    BigMamaStageInfo* RankStage(unsigned short nDanceGroupLevel, unsigned int &npcID);
    BigMamaStageInfo* GetStageInfo(unsigned short nStageID);
    BigMamaExchange* GetExchange(ESexType sex, unsigned char index); // 兑换链表，可能返回为空
    std::vector<BigMamaExchange>& GetExchangeList(ESexType sex);

    BigMamaBossConfig* GetBigMamaBossConfig(int bossID);

public:
    bool LoadConfig(const char* pszFileName);
    bool LoadBigMamaStageInfo(CFileStream &file);
    bool LoadBigMamaExchange(CFileStream &file);
    bool LoadBigMamaBossConfig(CFileStream &file);

    void SetBigMamaConfig(unsigned int nBeginTime, unsigned int nEndTime, unsigned char nMaxRound, unsigned int nXiaoLian, unsigned int nXiaoRuan, std::string strAnnouncement, unsigned int nAnnouncementSpace, const std::string &strIntroduce);
    void AddBigMamaReward(const BigMamaReward &reward);
    void AddBigMamaLuckData(const BigMamaLuckData &luckData);
    void SetBigMamaGuideReward(std::list<CItem> &maleReard, std::list<CItem> &femaleReward);
    
    std::list<CItem>& GetGuideMaleItem();
    std::list<CItem>& GetGuideFemaleItem();
private:
//     bool m_bOpen;

    unsigned int m_MaxStageInfoProb;
    std::vector<BigMamaStageInfo> m_vecBigMamaStageInfo;
    std::vector<BigMamaExchange> m_vecMaleBigMamaExchange;
    std::vector<BigMamaExchange> m_vecFemaleBigMamaExchange;
    std::map<int, BigMamaBossConfig> m_mapBigMamaBossConfig;

    unsigned int m_nBeginTime;
    unsigned int m_nEndTime;
    unsigned char m_nMaxRound;
    unsigned int m_nXiaoLianID;
    unsigned int m_nXiaoRuanID;

    std::string m_strAnnouncement;
    unsigned int m_nAnnouncementSpace;
    std::string m_strIntroduce;

    typedef std::map<EBigMamaRewardsType, BigMamaReward> BigMamaRewardMap;
    BigMamaRewardMap m_mapBigMamaReward;

    std::list<CItem> m_maleGuideItem;
    std::list<CItem> m_femaleGuidItem;

    typedef std::vector<BigMamaLuckData> BigMamaLuckDataVector;
    BigMamaLuckDataVector m_vecBigMamaLuckData;
};

#endif // __BIGMAMACONFIGMGR_H__
