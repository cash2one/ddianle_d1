/*
 * DanceRoom.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-10-23 21:12:07
 */

#ifndef __DANCEROOM_H__
#define __DANCEROOM_H__

#include "RoomBase.h"
#include "../roomLib/RoomMarkInfo.h"

#define INVALID_DANCE_ROOM_POS  -1
#define MIN_DANCE_ROOM_POS      0

/**
* RoomDancer:
* Common class of dancer and audience
*/

class RoomDancer : public RoomRoleBase
{
public:
    RoomDancer( unsigned int nRoleID );
    virtual ~RoomDancer();

    virtual void SendMessage( GameMsg_Base &msg );

public:
    bool RecvRoundMark( int nMode, int nRound, unsigned int nMark, 
        char nKeyRank, const std::vector<int>& rRoundRank );
    void ClearMark();

    void SumTotalMark( int nRank, int nRankMark );
    void CalcGrade();

    unsigned int GetTotalScore() const;

    CLASS_PROPERTY(int, m_nPosition, Position)
    CLASS_PROPERTY(ERoleRoomState, m_eState, State)
    CLASS_BOOL_PROPERTY(m_bHost, Host)
    CLASS_BOOL_PROPERTY(m_bDancer, Dancer)
    CLASS_PROPERTY_READONLY_BY_REF(CRoomMarkInfo, m_markInfo, MarkInfo)

private:
    int m_nTotalMark;
};

//////////////////////////////////////////////////////////////////////////

/**
* DanceRoomPosManager:
* manager dance room position
*/

class DanceRoomPosManager
{
public:
    explicit DanceRoomPosManager( int nMaxCount );
    virtual ~DanceRoomPosManager();

    void Clear();

    int GetIdelPos() const;
    int GetFirstDancerPos() const;

    bool AddDancer( int nPos, unsigned int nRoleID );
    void RemoveDancer( int nPos );

    unsigned int GetDancer( int nPos ) const;

    bool MoveDancerPos( int nOldPos, int nNewPos );

    bool ClosePos( int nPos );
    void OpenPos( int nPos );

    bool IsPosClosed( int nPos ) const;
    bool IsPosOpend( int nPos ) const;

    bool HasFull() const { return (m_nDancerCount >= m_nMaxCount); }
    bool HasEmpty() const { return (m_nDancerCount == 0); }

private:
    int FindPos( int nState ) const;

    bool IsValidPos( int nPos ) const;

    CLASS_PROPERTY_READONLY(int, m_nMaxCount, MaxCount)
    CLASS_PROPERTY_READONLY(int, m_nDancerCount, DancerCount)

private:
    std::vector<unsigned int> m_vectDancer;
    std::vector<int> m_vectPosState;
};

//////////////////////////////////////////////////////////////////////////

/**
* DanceBaseRoom:
* basic dance room
* only dancer
*/

class MusicConfig;

enum EDanceRoomState
{
    EDanceRoomState_Idle = 0,
    EDanceRoomState_Preparing,
    EDanceRoomState_Playing,
    EDanceRoomState_End
};

class DanceBaseRoom : public RoomBase
{
public:
    DanceBaseRoom( ERoomType eType, unsigned int nRoomID, 
        const std::string &rName = "", const std::string &rPassword = "" );
    virtual ~DanceBaseRoom();

    virtual void Prepare();
    virtual void Start();
    virtual void End();

public:
    void Clear();

    void ChangeChoosedMusicInfo( int nMusic, unsigned char nMusicMode = MusicMode_None, 
        unsigned char nMusicLevel = MusicLevel_None );

    bool DancerIn( RoomDancer *dancer );
    void DancerOut( unsigned int nRoleID );

    int GetDancerPos( unsigned int nRoleID );
    bool MoveDancerPos( int nOldPos, int nNewPos );

    bool HasStart() const;
    bool HasLegalEndTime() const;

    const RoomDancer* GetHost();
    const RoomDancer* AssignHost();
    const RoomDancer* AssignHost( int nPos );

    void PrepareMusicAndScene();

    bool RecvRoundMark( unsigned int nRoleID, int nMode, int nRound, 
        unsigned int nMark, char nKeyRank, const std::vector<int>& rRoundRank );

    void CalcDanceResult();

    unsigned int GetDancerScore( unsigned int nRoleID ) const;
    void GetDancerMark( unsigned int nRoleID, CRoomMarkInfo &rMark ) const;

protected:
    RoomDancer* FindDancerByPos( int nPos ) const;

    void SetAllDancerState( ERoleRoomState eState );

    void SumRankMark( RoomDancer *pDancer ) const;
    void DanceResultRanking( RoomDancer *pDancer );

    CLASS_PROPERTY_READONLY(EDanceRoomState, m_eState, State)

    CLASS_PROPERTY(int, m_nChoosedMusic, ChoosedMusic)
    CLASS_PROPERTY(unsigned char, m_nChoosedMusicMode, ChoosedMusicMode)
    CLASS_PROPERTY(unsigned char, m_nChoosedMusicLevel, ChoosedMusicLevel)

    CLASS_PROPERTY_READONLY(int, m_nPlayScene, PlayScene)
    CLASS_PROPERTY_READONLY(int, m_nPlayMusic, PlayMusic)
    CLASS_PROPERTY_READONLY(unsigned char, m_nPlayMusicMode, PlayMusicMode)
    CLASS_PROPERTY_READONLY(unsigned char, m_nPlayMusicLevel, PlayMusicLevel)

    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strCheckCode, CheckCode)
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strMusicSource, MusicSource)

    CLASS_PROPERTY(unsigned int, m_nCheckTime, CheckTime)

private:
    int m_nHostPos;
    unsigned int m_nStartTime;
    DanceRoomPosManager m_posMgr;
};

#endif // __DANCEROOM_H__

