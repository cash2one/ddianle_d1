#include "RoomMarkInfo.h"
#include "RoomCheckDataMgr.h"
#include "../../socket/Log.h"
#include "../../socket/Utility.h"

#include <iterator>

#ifdef WIN32
#pragma warning(disable:4100)
#endif

//////////////////////////////////////////////////////////////////////////

CheckingMarkFailedInfo::CheckingMarkFailedInfo()
    : m_bFailed( false )
    , m_nRound( 0 )
    , m_nKeyRank( RoomRoundRank_None )
    , m_nLastSpecial( 0 )
    , m_nMark( 0 )
    , m_nCheckingMark( 0 )
{
}

void CheckingMarkFailedInfo::Reset()
{
    m_bFailed = false;
    m_nRound = 0;
    m_nKeyRank = RoomRoundRank_None;
    m_nLastSpecial = 0;
    m_nMark = 0;
    m_nCheckingMark = 0;
    m_strRoundRank.clear();
    m_ss.clear();
    m_ss.str( "" );
}

void CheckingMarkFailedInfo::SetFailedInfo( int nKeyRank, int nLastSpecial, 
    unsigned int nMark, unsigned int nCheckingMark, const std::vector<int> &rRoundRank )
{
    m_bFailed = true;
    m_nKeyRank = nKeyRank;
    m_nLastSpecial = nLastSpecial;
    m_nMark = nMark;
    m_nCheckingMark = nCheckingMark;
    SetRoundRank( rRoundRank );
}

void CheckingMarkFailedInfo::SetRoundRank( const std::vector<int> &rRoundRank )
{
    if ( rRoundRank.empty() )
    {
        m_strRoundRank.clear();

        return;
    }

    m_ss.clear();
    m_ss.str( "" );

    std::copy( rRoundRank.begin(), 
        rRoundRank.end(), 
        ostream_iterator<int>( m_ss, "|" ) );

    m_strRoundRank = m_ss.str();
}

//////////////////////////////////////////////////////////////////////////

CRoomMarkInfo::CRoomMarkInfo()
{
	ClearMark();
}

CRoomMarkInfo::~CRoomMarkInfo()
{
}

CRoomMarkInfo& CRoomMarkInfo::operator= ( const CRoomMarkInfo &ref )
{
    this->m_nPlace = ref.m_nPlace;
    this->m_nGrade = ref.m_nGrade;
    this->m_nExp = ref.m_nExp;
    this->m_nRound = ref.m_nRound;
    this->m_nMark = ref.m_nMark;
    this->m_nBeckoningValue = ref.m_nBeckoningValue;
    this->m_nBeckoningGroupId = ref.m_nBeckoningGroupId;
    this->m_nLastSpecial = ref.m_nLastSpecial;
    this->m_nSpecialCount = ref.m_nSpecialCount;
    this->m_nLastRank = ref.m_nLastRank;
    this->m_nLastTimes = ref.m_nLastTimes;
    this->m_nTotalRankBase = ref.m_nTotalRankBase;
    std::copy( ref.m_szTotalRankCount, ref.m_szTotalRankCount + RoomRoundRank_Max, this->m_szTotalRankCount );
    std::copy( ref.m_szContinuousRank, ref.m_szContinuousRank + RoomRoundRank_Max, this->m_szContinuousRank );
    this->m_mapRoundRank = ref.m_mapRoundRank;
    this->m_ParcloseGeneEffect = ref.m_ParcloseGeneEffect;
    this->m_ParcloseTargetPara = ref.m_ParcloseTargetPara;

    return *this;
}

bool CRoomMarkInfo::AddRoundMark(int nMode, int nRound, unsigned int nMark, int nKeyRank, const std::vector<int>& vecRoundRank)
{
    if ( nRound > m_nRound && nKeyRank > RoomRoundRank_None && nKeyRank < RoomRoundRank_Max )
    {
        if ( m_checkingFailedInfo.HasFailed() )
        {
            m_checkingFailedInfo.Reset();
        }

        m_checkingFailedInfo.SetRound( nRound );

        if ( _HandleRoundRank( nMode, nMark, nKeyRank, vecRoundRank ) )
        {
            m_nRound = nRound;
            //WriteLog("AddRoundMark %u+%u=%u", m_nMark, nMark, m_nMark+nMark);
            m_nMark += nMark;
        }
        /*else
        {
            WriteLog("xindong !_HandleRoundRank nMode=%d, nKeyRank=%d", nMode, nKeyRank);
        }*/
        return true;
    }

    return false;
}

void CRoomMarkInfo::ClearMark()
{
	m_nPlace = 0;
	m_nGrade = RoomDanceGrade_None;
	m_nExp = 0;

	m_nRound = 0;
	m_nMark = 0;
	m_nBeckoningValue = 0;
	m_nBeckoningGroupId = 0;

	m_nLastSpecial = 0;
	m_nSpecialCount = 0;

	m_nLastRank = RoomRoundRank_None;
	m_nLastTimes = 0;
	m_nTotalRankBase = 0;
	
	for ( int i = 0; i < RoomRoundRank_Max; ++i )
	{
		m_szTotalRankCount[i] = 0;
		m_szContinuousRank[i] = 0;
	}

	m_ParcloseTargetPara.Clear();
	m_ParcloseGeneEffect.Clear();

	ClearRoundRankMap();
}

void CRoomMarkInfo::WinPlace(int nPlace)
{
	m_nPlace = nPlace;
}

int CRoomMarkInfo::Place() const
{
	return m_nPlace;
}

void CRoomMarkInfo::WinGrade(int nGrade)
{
	m_nGrade = nGrade;
}

int CRoomMarkInfo::Grade() const
{
	return m_nGrade;
}

void CRoomMarkInfo::WinExp(int nExp)
{
	m_nExp = nExp;
}

int CRoomMarkInfo::Exp() const
{
	return m_nExp;
}

int CRoomMarkInfo::Round() const
{
	return m_nRound;
}

unsigned int CRoomMarkInfo::Mark() const
{
	return m_nMark;
}

void CRoomMarkInfo::AddMark(unsigned int nMark)
{
	m_nMark += nMark;
}

unsigned int CRoomMarkInfo::BeckoningValue() const
{
	return m_nBeckoningValue;
}

void CRoomMarkInfo::SetBeckoningValue( unsigned int nValue )
{
	m_nBeckoningValue = nValue;
}

unsigned int CRoomMarkInfo::GetBeckoningGroupId() const
{
	return m_nBeckoningGroupId;
}

void CRoomMarkInfo::SetBeckoningGroupId( unsigned int val )
{
	m_nBeckoningGroupId = val;
}

int CRoomMarkInfo::LastSpecialCount() const
{
	return m_nLastSpecial;
}

int CRoomMarkInfo::MaxSpecialCount() const
{
	return m_nSpecialCount;
}

int CRoomMarkInfo::LastRank() const
{
	return m_nLastRank;
}

int CRoomMarkInfo::TotalRankBase() const
{
	return m_nTotalRankBase;
}

int CRoomMarkInfo::TotalRankCount(int nRank) const
{
	if ( nRank > RoomRoundRank_None && nRank < RoomRoundRank_Max )
	{
		return m_szTotalRankCount[nRank];
	}

	return 0;
}

int CRoomMarkInfo::TotalRankPercent(int nRank) const
{
	int nPercent = 0;

	if ( nRank > RoomRoundRank_None && nRank < RoomRoundRank_Max && m_nTotalRankBase > 0 )
	{
		nPercent = m_szTotalRankCount[nRank] * 100 / m_nTotalRankBase;
	}

	return nPercent;
}

int CRoomMarkInfo::ContinuousRankCount(int nRank) const
{
	if ( nRank > RoomRoundRank_None && nRank < RoomRoundRank_Max )
	{
		return m_szContinuousRank[nRank];
	}

	return 0;
}

bool CRoomMarkInfo::IsLegalMark(int nMode) const
{
	bool bLegalMark = false;
	int nNoMissCount = 0;
	
	for ( int nRank = RoomRoundRank_None; nRank < RoomRoundRank_Max; ++nRank )
	{
		if ( nRank > RoomRoundRank_Miss )
		{
			nNoMissCount += m_szTotalRankCount[nRank];
		}
	}

	const CRoomCheck& rRoomCheck =  CRoomCheckDataMgr::Instance().GetRoomCheck( nMode );
	if ( nNoMissCount >= rRoomCheck.m_nCheckNoMiss && m_nMark >= (unsigned int)rRoomCheck.m_nCheckMark )
	{
		bLegalMark = true;
	}

	return bLegalMark;
}

bool CRoomMarkInfo::_HandleRoundRank(int nMode, unsigned int nMark, int nKeyRank, const std::vector<int>& vecRoundRank)
{
	bool bRes = false;

	if ( nMode == MusicMode_Taiko )
	{
		bRes = _HandleTaikoRank( nMark, nKeyRank, vecRoundRank );
	}
	else if ( nMode == MusicMode_Tradition )
	{
		bRes = _HandleTraditonRank( nKeyRank, vecRoundRank );
	}
	else if ( nMode == MusicMode_Osu )
	{
		bRes = _HandleOsuRank( nMark, nKeyRank, vecRoundRank );
	}
	else if ( nMode == MusicMode_Rhythm )
	{
		bRes = _HandleRhythmRank( nKeyRank, vecRoundRank );
	}
	else if ( nMode == MusicMode_ADB )
	{
		bRes = _HandleADBRank( nKeyRank, vecRoundRank );
	}
	else if ( nMode == MusicMode_RainBow )
	{
		bRes = _HandleRainBowRank( nMark, nKeyRank, vecRoundRank );
	}
	else if ( nMode == MusicMode_SuperOsu )
	{
		bRes = _HandleSuperOsuRank( nMark, nKeyRank, vecRoundRank );
	}

	return bRes;
}

bool CRoomMarkInfo::_HandleOsuRank(unsigned int nMark, int nKeyRank, const std::vector<int>& vecRoundRank)
{
    if ( !vecRoundRank.empty() )
    {
        size_t nIndex = 0;
        for ( ; nIndex < vecRoundRank.size(); ++nIndex )
        {
            if ( vecRoundRank[nIndex] > RoomRoundRank_None && vecRoundRank[nIndex] < RoomRoundRank_Max)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        bool bAllCombo = true;

        if ( nIndex == vecRoundRank.size() )
        {
            int nLastSpecial = m_nLastSpecial;
            unsigned int nCheckingMark = 0;

            for ( size_t i = 0; i < vecRoundRank.size(); ++i )
            {
                if ( vecRoundRank[i] > RoomRoundRank_Bad )
                {
                    ++m_nLastSpecial;

                    if ( m_nSpecialCount < m_nLastSpecial )
                    {
                        m_nSpecialCount = m_nLastSpecial;
                    }
                }
                else
                {
                    if ( m_nSpecialCount < m_nLastSpecial )
                    {
                        m_nSpecialCount = m_nLastSpecial;
                    }

                    m_nLastSpecial = 0;
                    bAllCombo = false;
                }

                m_osuCalculator.SetComboCount( m_nLastSpecial );
                nCheckingMark += m_osuCalculator.CalcRankMark( vecRoundRank[i] );

                _AddValidRank( vecRoundRank[i] );
            }

            if ( nCheckingMark != nMark )
            {
                m_checkingFailedInfo.SetFailedInfo( nKeyRank, nLastSpecial, nMark, nCheckingMark, vecRoundRank );
            }

            if( bAllCombo )
            {
                m_nLastRank = 2;
            }
            else
            {
                m_nLastRank = 1;
            }

            return true;
        }
    }

    return false;
}

bool CRoomMarkInfo::_HandleTaikoRank(unsigned int nMark, int nKeyRank, const std::vector<int>& vecRoundRank)
{
    if ( !vecRoundRank.empty() && nKeyRank == vecRoundRank[vecRoundRank.size() - 1] )
    {
        size_t nIndex = 0;
        for ( ; nIndex < vecRoundRank.size(); ++nIndex )
        {
            if ( vecRoundRank[nIndex] > RoomRoundRank_None && vecRoundRank[nIndex] < RoomRoundRank_Max)
            {
                continue;
            }
            else
            {
                break;
            }
        }

        if ( nIndex == vecRoundRank.size() )
        {
            int nLastSpecial = m_nLastSpecial;
            unsigned int nCheckingMark = 0;

            for ( size_t i = 0; i < vecRoundRank.size(); ++i )
            {
                if ( vecRoundRank[i] > RoomRoundRank_Bad )
                {
                    ++m_nLastSpecial;

                    if ( m_nSpecialCount < m_nLastSpecial )
                    {
                        m_nSpecialCount = m_nLastSpecial;
                    }
                }
                else
                {
                    if ( m_nSpecialCount < m_nLastSpecial )
                    {
                        m_nSpecialCount = m_nLastSpecial;
                    }

                    m_nLastSpecial = 0;
                }

                m_taikoCalculator.SetComboCount( m_nLastSpecial );
                nCheckingMark += m_taikoCalculator.CalcRankMark( vecRoundRank[i] );

                _AddValidRank( vecRoundRank[i] );
            }

            if ( nCheckingMark != nMark )
            {
                m_checkingFailedInfo.SetFailedInfo( nKeyRank, nLastSpecial, nMark, nCheckingMark, vecRoundRank );
            }

            return true;
        }
    }

    return false;
}

bool CRoomMarkInfo::_HandleTraditonRank(int nKeyRank, const std::vector<int>& vecRoundRank)
{
	if ( vecRoundRank.size() == 1 && nKeyRank == vecRoundRank[0] )
	{
		if ( nKeyRank == RoomRoundRank_Perfect )
		{
			++m_nLastSpecial;

			if ( m_nSpecialCount < m_nLastSpecial )
			{
				m_nSpecialCount = m_nLastSpecial;
			}
		}
		else
		{
			if ( m_nSpecialCount < m_nLastSpecial )
			{
				m_nSpecialCount = m_nLastSpecial;
			}

			m_nLastSpecial = 0;
		}

		_AddValidRank( nKeyRank );

		return true;
	}

	return false;
}

bool CRoomMarkInfo::_HandleRhythmRank(int nKeyRank, const std::vector<int>& vecRoundRank)
{
	if ( !vecRoundRank.empty() && nKeyRank == vecRoundRank[vecRoundRank.size() - 1] )
	{
		// 检查集合中的值是否全部有效 
		size_t nIndex = 0;
		for ( ; nIndex < vecRoundRank.size(); ++nIndex )
		{
			if ( vecRoundRank[nIndex] > RoomRoundRank_None && vecRoundRank[nIndex] < RoomRoundRank_Max)
			{
				continue;
			}
			else
			{
				break;
			}
		}

		// 如果全部有效，则继续以下处理 
		if ( nIndex == vecRoundRank.size() )
		{
			for ( size_t i = 0; i < vecRoundRank.size(); ++i )
			{
				if ( vecRoundRank[i] > RoomRoundRank_Bad )
				{
					++m_nLastSpecial;

					if ( m_nSpecialCount < m_nLastSpecial )
					{
						m_nSpecialCount = m_nLastSpecial;
					}
				}
				else
				{
					if ( m_nSpecialCount < m_nLastSpecial )
					{
						m_nSpecialCount = m_nLastSpecial;
					}

					m_nLastSpecial = 0;
				}

				_AddValidRank( vecRoundRank[i] );
			}

			return true;
		}
	}

	return false;
}

bool CRoomMarkInfo::_HandleADBRank( int nKeyRank, const std::vector<int>& vecRoundRank )
{
	if ( vecRoundRank.size() == 1 && nKeyRank == vecRoundRank[0] )
	{
		if ( nKeyRank == RoomRoundRank_Perfect )
		{
			++m_nLastSpecial;

			if ( m_nSpecialCount < m_nLastSpecial )
			{
				m_nSpecialCount = m_nLastSpecial;
			}
		}
		else
		{
			if ( m_nSpecialCount < m_nLastSpecial )
			{
				m_nSpecialCount = m_nLastSpecial;
			}

			m_nLastSpecial = 0;
		}

		_AddValidRank( nKeyRank );

		return true;
	}

	return false;
}

bool CRoomMarkInfo::_HandleRainBowRank(unsigned int nMark, int nKeyRank, const std::vector<int>& vecRoundRank)
{
    if ( !vecRoundRank.empty() && nKeyRank == vecRoundRank[vecRoundRank.size() - 1] )
    {
        size_t nIndex = 0;
        for ( ; nIndex < vecRoundRank.size(); ++nIndex )
        {
            if ( vecRoundRank[nIndex] > RoomRoundRank_None && vecRoundRank[nIndex] < RoomRoundRank_Max)
            {
                continue;
            }
            else
            {
                break;
            }
        }

        if ( nIndex == vecRoundRank.size() )
        {
            int nLastSpecial = m_nLastSpecial;
            unsigned int nCheckingMark = 0;

            for ( size_t i = 0; i < vecRoundRank.size(); ++i )
            {
                if ( vecRoundRank[i] > RoomRoundRank_Bad )
                {
                    ++m_nLastSpecial;

                    if ( m_nSpecialCount < m_nLastSpecial )
                    {
                        m_nSpecialCount = m_nLastSpecial;
                    }
                }
                else
                {
                    if ( m_nSpecialCount < m_nLastSpecial )
                    {
                        m_nSpecialCount = m_nLastSpecial;
                    }

                    m_nLastSpecial = 0;
                }

                m_rainbowCalculator.SetComboCount( m_nLastSpecial );
                nCheckingMark += m_rainbowCalculator.CalcRankMark( vecRoundRank[i] );

                _AddValidRank( vecRoundRank[i] );
            }

            if ( nCheckingMark != nMark )
            {
                m_checkingFailedInfo.SetFailedInfo( nKeyRank, nLastSpecial, nMark, nCheckingMark, vecRoundRank );
            }

            return true;
        }
    }

    return false;
}

bool CRoomMarkInfo::_HandleSuperOsuRank(unsigned int nMark, int nKeyRank, const std::vector<int>& vecRoundRank)
{
    if ( !vecRoundRank.empty() )
    {
        size_t nIndex = 0;
        for ( ; nIndex < vecRoundRank.size(); ++nIndex )
        {
            if ( vecRoundRank[nIndex] > RoomRoundRank_None && vecRoundRank[nIndex] < RoomRoundRank_Max)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        bool bAllCombo = true;

        if ( nIndex == vecRoundRank.size() )
        {
            int nLastSpecial = m_nLastSpecial;
            unsigned int nCheckingMark = 0;

            for ( size_t i = 0; i < vecRoundRank.size(); ++i )
            {
                if ( vecRoundRank[i] > RoomRoundRank_Bad )
                {
                    ++m_nLastSpecial;

                    if ( m_nSpecialCount < m_nLastSpecial )
                    {
                        m_nSpecialCount = m_nLastSpecial;
                    }
                }
                else
                {
                    if ( m_nSpecialCount < m_nLastSpecial )
                    {
                        m_nSpecialCount = m_nLastSpecial;
                    }

                    m_nLastSpecial = 0;
                    bAllCombo = false;
                }

                m_osuCalculator.SetComboCount( m_nLastSpecial );
                nCheckingMark += m_osuCalculator.CalcRankMark( vecRoundRank[i] );

                _AddValidRank( vecRoundRank[i] );
            }

            if ( nCheckingMark != nMark )
            {
                m_checkingFailedInfo.SetFailedInfo( nKeyRank, nLastSpecial, nMark, nCheckingMark, vecRoundRank );
            }

            if( bAllCombo )
            {
                m_nLastRank = 2;
            }
            else
            {
                m_nLastRank = 1;
            }

            return true;
        }
    }

    return false;
}

void CRoomMarkInfo::_AddValidRank(int nRank)
{
	m_nTotalRankBase += 1;

	if ( nRank == RoomRoundRank_Miss && m_ParcloseGeneEffect.nMissToCoolEffect > 0 )
	{
		int RandomValue = Random( 1, 100 );
		if ( RandomValue >= m_ParcloseGeneEffect.nMissToCoolEffect )
		{
			nRank = RoomRoundRank_Cool;
		}
	}

	if ( nRank == RoomRoundRank_Cool && m_ParcloseGeneEffect.nCoolToPerfectEffect > 0 )
	{
		int RandomValue = Random( 1, 100 );
		if ( RandomValue >= m_ParcloseGeneEffect.nCoolToPerfectEffect )
		{
			nRank = RoomRoundRank_Perfect;
		}
	}

	m_szTotalRankCount[nRank] += 1;

	if ( nRank != m_nLastRank )
	{
		m_nLastRank = nRank;
		m_nLastTimes = 0;
	}

	++m_nLastTimes;

	if ( m_nLastTimes > m_szContinuousRank[m_nLastRank] )
	{
		m_szContinuousRank[m_nLastRank] = m_nLastTimes;
	}
}

RoundRankMap& CRoomMarkInfo::GetRoundRankMap()
{
	return m_mapRoundRank;
}

void CRoomMarkInfo::AddRoundRank(int nRound, const std::vector<int>& vecRoundRank)
{
	m_mapRoundRank[nRound] = vecRoundRank;
}

unsigned int CRoomMarkInfo::GetComboCount() const
{
    return m_nSpecialCount;
}

void CRoomMarkInfo::AddMarkGeneEffect( unsigned int nValue )
{
	if ( nValue > 100 )
	{
		return;
	}

	m_ParcloseGeneEffect.nAddMarkEffect = nValue;
}

void CRoomMarkInfo::ClearRoundRankMap()
{
	m_mapRoundRank.clear();
}

int  CRoomMarkInfo::GetTargetValue( eTargetFlag eFlag )
{
	switch( eFlag )
	{
	case  ETF_TOTALMARK :return m_nMark + m_ParcloseGeneEffect.nAddMarkEffect*m_nMark/100 ;
	case  ETF_COMBOCOUNT : return m_nSpecialCount;
	case  ETF_GRADE :return m_nGrade;
	case  ETF_FRIEND: return m_ParcloseTargetPara.nFriendID;
	case  ETF_LOVER: return m_ParcloseTargetPara.nCoupleID;
	default: break;
	}

	return 0;
}

