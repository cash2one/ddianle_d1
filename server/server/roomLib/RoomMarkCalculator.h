/*
 * RoomMarkCalculator.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-8-6 16:32:22
 */

#ifndef __ROOMMARKCALCULATOR_H__
#define __ROOMMARKCALCULATOR_H__

#include "../../datastructure/Macro_Define.h"

/**
* RoomMarkCalculator:
* 
*/

class RoomMarkCalculator
{
protected:
    RoomMarkCalculator();
    virtual ~RoomMarkCalculator() {}

public:
    unsigned int CalcRankMark( int nRoomRoundRank );

protected:
    virtual unsigned int CalcBaseMark( int nRoomRoundRank ) = 0;
    virtual unsigned int CalcBonus( unsigned int nBaseMark ) = 0;
};

//////////////////////////////////////////////////////////////////////////

/**
* TaikoMarkCalculator:
* 经典模式分值计算器
*/

class TaikoMarkCalculator : public RoomMarkCalculator
{
public:
    TaikoMarkCalculator();
    virtual ~TaikoMarkCalculator() {}

public:
    void SetComboCount( int nComboCount );

protected:
    virtual unsigned int CalcBaseMark( int nRoomRoundRank );
    virtual unsigned int CalcBonus( unsigned int nBaseMark );

private:
    unsigned int                m_nComboLevel;

    static const unsigned int   m_nComboBonusPercent;
    static const unsigned short m_nBaseMark[RoomRoundRank_Max - 1];
};

//////////////////////////////////////////////////////////////////////////

/**
* OsuMarkCalculator:
* 泡泡/超级泡泡模式分值计算器
*/

#define OSU_BASE_MARK_ROW   11
#define OSU_BASE_MARK_COL   3

class OsuMarkCalculator : public RoomMarkCalculator
{
public:
    OsuMarkCalculator();
    virtual ~OsuMarkCalculator() {}

public:
    void SetComboCount( int nComboCount );

protected:
    virtual unsigned int CalcBaseMark( int nRoomRoundRank );
    virtual unsigned int CalcBonus( unsigned int nBaseMark );

private:
    unsigned int                m_nComboLevel;

    static const unsigned short m_nBadBaseMark;
    static const unsigned short m_nBaseMark[OSU_BASE_MARK_ROW][OSU_BASE_MARK_COL];
};

//////////////////////////////////////////////////////////////////////////

/**
* RainBowMarkCalculator:
* 彩虹模式分值计算器
*/

class RainBowMarkCalculator : public RoomMarkCalculator
{
public:
    RainBowMarkCalculator();
    virtual ~RainBowMarkCalculator() {}

public:
    void SetComboCount( int nComboCount );

protected:
    virtual unsigned int CalcBaseMark( int nRoomRoundRank );
    virtual unsigned int CalcBonus( unsigned int nBaseMark );

private:
    unsigned int                m_nComboLevel;

    static const unsigned int   m_nComboBonusPercent;
    static const unsigned short m_nBaseMark[RoomRoundRank_Max - 1];
};

#endif // __ROOMMARKCALCULATOR_H__

