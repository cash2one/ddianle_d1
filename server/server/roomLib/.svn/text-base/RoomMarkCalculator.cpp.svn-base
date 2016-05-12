#include "RoomMarkCalculator.h"

//////////////////////////////////////////////////////////////////////////

RoomMarkCalculator::RoomMarkCalculator(void)
{
}

unsigned int RoomMarkCalculator::CalcRankMark( int nRoomRoundRank )
{
    unsigned int nBaseMark = CalcBaseMark( nRoomRoundRank );
    unsigned int nBonus = CalcBonus( nBaseMark );

    return nBaseMark + nBonus;
}

//////////////////////////////////////////////////////////////////////////

const unsigned int TaikoMarkCalculator::m_nComboBonusPercent = 10;

const unsigned short TaikoMarkCalculator::m_nBaseMark[] = 
{
    0,      // Miss
    250,    // Bad
    300,    // Good
    400,    // Cool
    500     // Perfect
};

TaikoMarkCalculator::TaikoMarkCalculator()
    : m_nComboLevel( 0 )
{
}

void TaikoMarkCalculator::SetComboCount( int nComboCount )
{
    if ( nComboCount < 10 )
        m_nComboLevel = 0;
    else if ( nComboCount < 100 )
        m_nComboLevel = nComboCount / 10;
    else
        m_nComboLevel = 10;
}

unsigned int TaikoMarkCalculator::CalcBaseMark( int nRoomRoundRank )
{
    int nIndex = nRoomRoundRank - 1;
    int nSize = sizeof( m_nBaseMark ) / sizeof( m_nBaseMark[0] );

    if ( 0 <= nIndex && nIndex < nSize )
        return m_nBaseMark[ nIndex ];

    return 0;
}

unsigned int TaikoMarkCalculator::CalcBonus( unsigned int nBaseMark )
{
    return nBaseMark * m_nComboLevel * m_nComboBonusPercent / 100;
}

//////////////////////////////////////////////////////////////////////////

const unsigned short OsuMarkCalculator::m_nBadBaseMark = 250;

const unsigned short OsuMarkCalculator::m_nBaseMark[OSU_BASE_MARK_ROW][OSU_BASE_MARK_COL] = 
{
// row: combo level
// col: good/cool/perfect
    { 300, 400, 500 },  // combo level 0
    { 330, 440, 550 }, 
    { 360, 480, 600 }, 
    { 390, 520, 650 }, 
    { 420, 560, 700 }, 
    { 450, 600, 750 }, 
    { 480, 640, 800 }, 
    { 510, 680, 850 }, 
    { 540, 720, 900 }, 
    { 570, 760, 950 }, 
    { 600, 800, 1000 }  // combo level 10
};

OsuMarkCalculator::OsuMarkCalculator()
    : m_nComboLevel( 0 )
{
}

void OsuMarkCalculator::SetComboCount( int nComboCount )
{
    if ( nComboCount < 10 )
        m_nComboLevel = 0;
    else if ( nComboCount < 100 )
        m_nComboLevel = nComboCount / 10;
    else
        m_nComboLevel = 10;
}

unsigned int OsuMarkCalculator::CalcBaseMark( int nRoomRoundRank )
{
    if ( nRoomRoundRank <= RoomRoundRank_Miss || nRoomRoundRank >= RoomRoundRank_Max )
    {
        return 0;
    }
    else if ( nRoomRoundRank == RoomRoundRank_Bad )
    {
        return m_nBadBaseMark;
    }

    int offset = nRoomRoundRank - RoomRoundRank_Good;

    if ( 0 <= offset && offset < OSU_BASE_MARK_COL && 
         m_nComboLevel < OSU_BASE_MARK_ROW )
    {
        return m_nBaseMark[ m_nComboLevel ][ offset ];
    }

    return 0;
}

unsigned int OsuMarkCalculator::CalcBonus( unsigned int )
{
    return 0;
}

//////////////////////////////////////////////////////////////////////////

const unsigned int RainBowMarkCalculator::m_nComboBonusPercent = 10;

const unsigned short RainBowMarkCalculator::m_nBaseMark[] = 
{
    0,      // Miss
    250,    // Bad
    300,    // Good
    400,    // Cool
    500     // Perfect
};

RainBowMarkCalculator::RainBowMarkCalculator()
    : m_nComboLevel( 0 )
{
}

void RainBowMarkCalculator::SetComboCount( int nComboCount )
{
    if ( nComboCount < 10 )
        m_nComboLevel = 0;
    else if ( nComboCount < 100 )
        m_nComboLevel = nComboCount / 10;
    else
        m_nComboLevel = 10;
}

unsigned int RainBowMarkCalculator::CalcBaseMark( int nRoomRoundRank )
{
    int nIndex = nRoomRoundRank - 1;
    int nSize = sizeof( m_nBaseMark ) / sizeof( m_nBaseMark[0] );

    if ( 0 <= nIndex && nIndex < nSize )
        return m_nBaseMark[ nIndex ];

    return 0;
}

unsigned int RainBowMarkCalculator::CalcBonus( unsigned int nBaseMark )
{
    return nBaseMark * m_nComboLevel * m_nComboBonusPercent / 100;
}

