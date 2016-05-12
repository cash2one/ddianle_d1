/*
 * GameMsg_Rank_SG.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-2-3 11:50:55
 */

#ifndef __GAMEMSG_RANK_SG_H__
#define __GAMEMSG_RANK_SG_H__

#include "RankDefine.h"
#include "../../socket/GameMsg_Base.h"

class CParamPool;

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_GetRankHall : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetRankHall);
    GameMsg_S2G_GetRankHall();
    ~GameMsg_S2G_GetRankHall() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short          m_nCurrentPage;     // from 0
    unsigned char           m_nCount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_GetRankHallResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetRankHallResult);
    GameMsg_G2S_GetRankHallResult();
    ~GameMsg_G2S_GetRankHallResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short          m_nCurrentPage;
    unsigned short          m_nTotal;
    std::list<CRankTopOneInfo> m_listTopOneInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_GetRankDatas : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetRankDatas);
    GameMsg_S2G_GetRankDatas();
    ~GameMsg_S2G_GetRankDatas() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned char           m_nType;
    unsigned char           m_nDetailType;
    unsigned short          m_nCurrentPage;     // from 0
    unsigned char           m_nCount;
    unsigned int            m_nKey;             // roleID/groupID/pairID
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_GetRankDatasResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetRankDatasResult);
    GameMsg_G2S_GetRankDatasResult();
    ~GameMsg_G2S_GetRankDatasResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short          m_nMyRank;
    unsigned short          m_nCurrentPage;
    unsigned short          m_nTotal;
    std::list<CRankOfflineInfo> m_listOfflineInfo;
    unsigned short          m_nDataType;
    unsigned char           m_nDetailType;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2G_ChangeRankValue : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_ChangeRankValue);
    GameMsg_S2G_ChangeRankValue();
    ~GameMsg_S2G_ChangeRankValue() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short          m_nDataType;
    unsigned int            m_nKey;
    unsigned int            m_nChangeValue;
    unsigned int            m_nTotalValue;
    unsigned int            m_nStartTime;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_UpdateRankMinValue : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_UpdateRankMinValue);
    GameMsg_G2S_UpdateRankMinValue();
    ~GameMsg_G2S_UpdateRankMinValue() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short          m_nDataType;
    unsigned int            m_nValue;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_G2S_GetRankAllDataForMedal : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetRankAllDataForMedal);
    GameMsg_G2S_GetRankAllDataForMedal();
    ~GameMsg_G2S_GetRankAllDataForMedal() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    int     m_RankMCoin;                //消费
    int     m_RankHorn;                 //喇叭排行
    int     m_RankExp;                  //经验
    int     m_RankGroupHonor;           //荣誉舞团
    int     m_RankIntimacy;             //明星夫妻
    int     m_RankMedalScore;           //勋章积分
    int     m_RankClothEffect;          //闪亮值
    int     m_RankEquipCount;           //收藏总数
};


class GameMsg_S2G_GetRankAllDataForMedal : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetRankAllDataForMedal);
    GameMsg_S2G_GetRankAllDataForMedal();
    ~GameMsg_S2G_GetRankAllDataForMedal() {}
public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBUff);

    int m_nRoleId;
    int m_GroupId;
    int m_IntimacyId;
};

class GameMsg_S2G_GetRankAllData : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetRankAllData);
    GameMsg_S2G_GetRankAllData();
    ~GameMsg_S2G_GetRankAllData() {}
public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBUff);

    int m_nRoleId;
    int m_GroupId;
    int m_IntimacyId;
};

class GameMsg_G2S_GetRankAllData : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_G2S_GetRankAllData);
    GameMsg_G2S_GetRankAllData();
    ~GameMsg_G2S_GetRankAllData() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    int     m_RankMCoin;                //消费
    int     m_RankHorn;                 //喇叭排行
    int     m_RankExp;                  //经验
    int     m_RankGroupHonor;           //荣誉舞团
    int     m_RankIntimacy;             //明星夫妻
    int     m_RankMedalScore;           //勋章积分
    int     m_RankClothEffect;          //闪亮值
    int     m_RankEquipCount;           //收藏总数
};




#endif // __GAMEMSG_RANK_SG_H__

