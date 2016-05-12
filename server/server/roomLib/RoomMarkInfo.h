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
    int                        nAddMarkEffect;       // ������ܷ�
    int                        nMissToCoolEffect;    // ������ʰ�Miss�ж�ΪCool
    int                        nCoolToPerfectEffect; // ������ʰ�Miss�ж�ΪCool

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

    // ��ȡCombo����
    unsigned int GetComboCount() const;

    // ��ȡĿ��ֵ
    int GetTargetValue( eTargetFlag eFlag );

    // ����ħ�����Ŀ�����ID
    void SetFriendID( unsigned int nFriendID ){ m_ParcloseTargetPara.nFriendID = nFriendID; }

    // ����ħ�����Ŀ������ID
    void SetCoupleID( unsigned int nCoupleID ){ m_ParcloseTargetPara.nCoupleID = nCoupleID; }

    // ��Ӽ��ֻܷ���Ч��
    void AddMarkGeneEffect( unsigned int nValue );

    // ���Miss�ж�cool����Ч��
    void AddMissToCoolGeneEffect( unsigned int nProbability ){ m_ParcloseGeneEffect.nMissToCoolEffect = nProbability; }

    // ���Cool�ж�perfect����Ч��
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
    int m_nPlace;               // ����
    int m_nGrade;               // ����
    int m_nExp;

    int m_nRound;               //current round
    unsigned int m_nMark;       //mark till current round
    unsigned int m_nBeckoningValue;     //�Ķ�ֵ till current round
    unsigned int m_nBeckoningGroupId;

    int m_nLastSpecial;         // ��������̫������combo����ͳ����P
    int m_nSpecialCount;        // ������

    int m_nLastRank;
    int m_nLastTimes;

    int m_nTotalRankBase;
    int m_szTotalRankCount[RoomRoundRank_Max];      //total rank count till current round
    int m_szContinuousRank[RoomRoundRank_Max];      //continuous rank count till current round

    RoundRankMap m_mapRoundRank;//round id <=> ranks
    ParcloseGeneEffect m_ParcloseGeneEffect;        // ħ�������ƷЧ������
    ParcloseTargetPara     m_ParcloseTargetPara;    // ħ�����Ŀ�����

    // mark calculator
    TaikoMarkCalculator     m_taikoCalculator;
    OsuMarkCalculator       m_osuCalculator;
    RainBowMarkCalculator   m_rainbowCalculator;

    CheckingMarkFailedInfo  m_checkingFailedInfo;
};


#endif

