#ifndef __BIGMAMACONFIG_H__
#define __BIGMAMACONFIG_H__

#include <vector>
#include <list>

#include "../../datastructure/DataStruct_Base.h"


class CFileStream;

class BigMamaStageInfo
{
public:
    BigMamaStageInfo();
    virtual ~BigMamaStageInfo();

public:
    bool Load(CFileStream & file);
//    void doEncode(CParamPool & IOBuff);

//    [ExcelHeader("关卡ID")]
    unsigned short m_nStageID;
//        [ExcelHeader("歌曲ID")]
    unsigned short m_nMusicID;
//        [ExcelHeader("模式ID")]
    unsigned char m_nModeID;
//        [ExcelHeader("难度")]
    unsigned char m_nLevel;
//        [ExcelHeader("NPCID")]
    unsigned char m_nNpcID;
//        [ExcelHeader("TargetScore")]
    unsigned int m_nTargetScore;

//        [ExcelHeader("场景ID")]
    unsigned char m_nSceneID;
//        [ExcelHeader("随机概率")]
    unsigned int m_nRatio;
};

class BigMamaExchange
{
public:
    BigMamaExchange();
    virtual ~BigMamaExchange();

public:
    bool Load(CFileStream & file);
//    void doEncode(CParamPool & IOBuff);
//    [ExcelHeader("性别")]
    unsigned char m_nSex; // 0:通用，1:男用，2:女用
//    [ExcelHeader("兑换物品ID")]
    unsigned int m_nTargetItemID;
//        [ExcelHeader("兑换物品时效")]
    int m_nTargetMatune;
//        [ExcelHeader("兑换物品数量")]
    unsigned int m_nTargetCount;

//        [ExcelHeader("材料1物品ID")]
    unsigned int m_nM1ItemID;
//        [ExcelHeader("材料1物品时效")]
    int m_nM1Matune;
//        [ExcelHeader("材料1物品数量")]
    unsigned int m_nM1Count;

//        [ExcelHeader("材料2物品ID")]
    unsigned int m_nM2ItemID;
//        [ExcelHeader("材料2物品时效")]
    int m_nM2Matune;
//        [ExcelHeader("材料2物品数量")]
    unsigned int m_nM2Count;

//        [ExcelHeader("材料3物品ID")]
    unsigned int m_nM3ItemID;
//        [ExcelHeader("材料2物品时效")]
    int m_nM3Matune;
//        [ExcelHeader("材料2物品数量")]
    unsigned int m_nM3Count;
    
//        [ExcelHeader("舞团贡献")]
    unsigned int m_nDanceGroupContibute;

//        [ExcelHeader("金券")]
    unsigned int m_nMoneyCount;
//        [ExcelHeader("绑定M币")]
    unsigned int m_nBindMCoinCount;
//        [ExcelHeader("M币")]
    unsigned int m_nMCoinCount;

};



// boss配置
class BigMamaBossConfig
{
public:
    BigMamaBossConfig();
    ~BigMamaBossConfig(){};

public:
    bool Load(CFileStream&file);
//    bool Save(CFileStream&file);

public:
    int         m_nBossID;
    std::string m_strName;
    unsigned char        m_cSex;
    int         m_cSKinColor;
    std::map<EItemClothType, itemtype_t>   m_Equips;
};


class BigMamaLuckData
{
public:
    BigMamaLuckData();
    ~BigMamaLuckData();

    unsigned int m_nNpcID;
    unsigned int m_nBaseProb;
    unsigned int m_nProbGrowRatio;
};

enum EBigMamaRewardsType
{
    EBigMamaRewardsType_UnJoin = 1,
    EBigMamaRewardsType_Join = 2,
    EBigMamaRewardsType_Killer = 3,
    EBigMamaRewardsType_XiaoLian = 4,
    EBigMamaRewardsType_XiaoRuan = 5,
};


class BigMamaReward
{
public:
    BigMamaReward();
    ~BigMamaReward();


    EBigMamaRewardsType m_rewardType;
    std::list<CItem> m_vecMaleItem;
    std::list<CItem> m_vecFemaleItem;
    unsigned int m_nMoney;
    unsigned int m_nBindCoin;
    unsigned int m_nDanceGroupContribution;
};

#endif