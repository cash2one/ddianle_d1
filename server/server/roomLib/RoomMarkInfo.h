#ifndef __ROOM_MARK_INFO_H__
#define __ROOM_MARK_INFO_H__

#include "RoomMarkCalculator.h"
#include "../Macros/Macros.h"
#include "../../datastructure/Macro_Define.h"
#include <vector>
#include <map>
#include <sstream>

#ifdef WIN32
#pragma warning(disable:4625 4626)
#endif

typedef std::map<int, std::vector<int> > RoundRankMap;

struct ParcloseGeneEffect 
{
    int                        nAddMarkEffect;       // 基因加总分
    int                        nMissToCoolEffect;    // 基因概率把Miss判定为Cool
    int                        nCoolToPerfectEffect; // 基因概率把Miss判定为Cool

    ParcloseGeneEffect()
    {
        nAddMarkEffect = 0;
        nMissToCoolEffect = 0;
        nCoolToPerfectEffect = 0;
    }

    void Clear()
    {
        nAddMarkEffect = 0;
        nMissToCoolEffect = 0;
        nCoolToPerfectEffect = 0;
    }
};

struct ParcloseTargetPara
{
    unsigned int    nFriendID;
    unsigned int    nCoupleID;

    ParcloseTargetPara()
    {
        nFriendID = 0;
        nCoupleID = 0;
    }

    void Clear()
    {
        nFriendID = 0;
        nCoupleID = 0;
    }
};

//////////////////////////////////////////////////////////////////////////

class CheckingMarkFailedInfo
{
public:
    CheckingMarkFailedInfo();
    ~CheckingMarkFailedInfo() {}

public:
    void Reset();
    void SetFailedInfo( int nKeyRank, int nLastSpecial, unsigned int nMark, 
        unsigned int nCheckingMark, const std::vector<int> &rRoundRank );

protected:
    void SetRoundRank( const std::vector<int> &rRoundRank );

    CLASS_BOOL_PROPERTY_READONLY( m_bFailed, Failed );
    CLASS_PROPERTY( int, m_nRound, Round );
    CLASS_PROPERTY_READONLY( int, m_nKeyRank, KeyRank );
    CLASS_PROPERTY_READONLY( int, m_nLastSpecial, LastSpecial );
    CLASS_PROPERTY_READONLY_BY_REF( std::string, m_strRoundRank, RoundRank );
    CLASS_PROPERTY_READONLY( unsigned int, m_nMark, Mark );
    CLASS_PROPERTY_READONLY( unsigned int, m_nCheckingMark, CheckingMark );

private:
    std::ostringstream   m_ss;
};

//////////////////////////////////////////////////////////////////////////

class CRoomMarkInfo
{
public:
    CRoomMarkInfo();
    ~CRoomMarkInfo();

    CRoomMarkInfo& operator= ( const CRoomMarkInfo &ref );

public:
    bool AddRoundMark(int nMode, int nRound, unsigned int nMark, int nKeyRank, const std::vector<int>& vecRoundRank);
    void ClearMark();
    void ClearRoundRankMap();

    void WinPlace(int nPlace);
    int Place() const;

    void WinGrade(int nGrade);
    int Grade() const;

    void WinExp(int nExp);
    int Exp() const;

    int Round() const;
    unsigned int Mark() const;
    void AddMark(unsigned int nMark);
    unsigned int BeckoningValue() const;
    void SetBeckoningValue(unsigned int nValue);
    unsigned int GetBeckoningGroupId() const;
    void SetBeckoningGroupId(unsigned int val);

    int LastSpecialCount() const;
    int MaxSpecialCount() const;
    int LastRank() const;
    int TotalRankBase() const;

    int TotalRankCount(int nRank) const;
    int TotalRankPercent(int nRank) const;
    int ContinuousRankCount(int nRank) const;

    bool IsLegalMark(int nMode) const;

    RoundRankMap& GetRoundRankMap();
    void AddRoundRank(int nRound, const std::vector<int>& vecRoundRank);

    const CheckingMarkFailedInfo& GetCheckingMarkFailedInfo() { return m_checkingFailedInfo; }

    // 获取Combo个数
    unsigned int GetComboCount() const;

    // 获取目标值
    int GetTargetValue( eTargetFlag eFlag );

    // 设置魔法神灯目标好友ID
    void SetFriendID( unsigned int nFriendID ){ m_ParcloseTargetPara.nFriendID = nFriendID; }

    // 设置魔法神灯目标情侣ID
    void SetCoupleID( unsigned int nCoupleID ){ m_ParcloseTargetPara.nCoupleID = nCoupleID; }

    // 添加加总分基因效果
    void AddMarkGeneEffect( unsigned int nValue );

    // 添加Miss判定cool基因效果
    void AddMissToCoolGeneEffect( unsigned int nProbability ){ m_ParcloseGeneEffect.nMissToCoolEffect = nProbability; }

    // 添加Cool判定perfect基因效果
    void AddCoolToPerfectGeneEffect( unsigned int nProbability ){ m_ParcloseGeneEffect.nCoolToPerfectEffect = nProbability; }

protected:
    bool _HandleRoundRank(int nMode, unsigned int nMark, int nKeyRank, const std::vector<int>& vecRoundRank);
    bool _HandleTaikoRank(unsigned int nMark, int nKeyRank, const std::vector<int>& vecRoundRank);
    bool _HandleTraditonRank(int nKeyRank, const std::vector<int>& vecRoundRank);
    bool _HandleOsuRank(unsigned int nMark, int nKeyRank, const std::vector<int>& vecRoundRank);
    bool _HandleRhythmRank(int nKeyRank, const std::vector<int>& vecRoundRank);
    bool _HandleADBRank(int nKeyRank, const std::vector<int>& vecRoundRank);
    bool _HandleRainBowRank(unsigned int nMark, int nKeyRank, const std::vector<int>& vecRoundRank);
    bool _HandleSuperOsuRank(unsigned int nMark, int nKeyRank, const std::vector<int>& vecRoundRank);
    void _AddValidRank(int nRank);

protected:
    int m_nPlace;               // 排名
    int m_nGrade;               // 评定
    int m_nExp;

    int m_nRound;               //current round
    unsigned int m_nMark;       //mark till current round
    unsigned int m_nBeckoningValue;     //心动值 till current round
    unsigned int m_nBeckoningGroupId;

    int m_nLastSpecial;         // 连续数，太鼓下是combo，传统是连P
    int m_nSpecialCount;        // 连续数

    int m_nLastRank;
    int m_nLastTimes;

    int m_nTotalRankBase;
    int m_szTotalRankCount[RoomRoundRank_Max];      //total rank count till current round
    int m_szContinuousRank[RoomRoundRank_Max];      //continuous rank count till current round

    RoundRankMap m_mapRoundRank;//round id <=> ranks
    ParcloseGeneEffect m_ParcloseGeneEffect;        // 魔法神灯物品效果因子
    ParcloseTargetPara     m_ParcloseTargetPara;    // 魔法神灯目标参数

    // mark calculator
    TaikoMarkCalculator     m_taikoCalculator;
    OsuMarkCalculator       m_osuCalculator;
    RainBowMarkCalculator   m_rainbowCalculator;

    CheckingMarkFailedInfo  m_checkingFailedInfo;
};


#endif

