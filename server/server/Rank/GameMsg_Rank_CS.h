/*
 * GameMsg_Rank_CS.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-2-3 11:50:55
 */

#ifndef __GAMEMSG_RANK_CS_H__
#define __GAMEMSG_RANK_CS_H__

#include "RankDefine.h"
#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;
class CEntityItemComponent;
class CEntityGeneComponent;
class PetComponent;

//////////////////////////////////////////////////////////////////////////

class CRankPlayerInfo
{
public:
    CRankPlayerInfo();
    ~CRankPlayerInfo() {}

public:
    void doEncode(CParamPool &IOBuff);

public:
    unsigned short          m_nTransformID;
    RoleImageOfflineData         m_OfflineData;
    CEntityItemComponent*   m_pRoleItem;
    CEntityGeneComponent*   m_pRoleGene;
    PetComponent*           m_pRolePet;
};

//////////////////////////////////////////////////////////////////////////

class CRankRowInfo
{
public:
    CRankRowInfo();
    ~CRankRowInfo() {}

public:
    void doEncode(CParamPool &IOBuff);

public:
    unsigned short          m_nRank;
    std::list<std::string>  m_listOtherInfo;
    unsigned int            m_nSmallRoleID;
    std::list<CRankPlayerInfo> m_listPlayersInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_GetRankHall : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetRankHall);
    GameMsg_C2S_GetRankHall();
    ~GameMsg_C2S_GetRankHall() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short          m_nCurrentPage;     // from 0
    unsigned char           m_nCount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_GetRankHallResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetRankHallResult);
    GameMsg_S2C_GetRankHallResult();
    ~GameMsg_S2C_GetRankHallResult() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short          m_nCurrentPage;
    unsigned short          m_nTotal;
    std::list<CRankTopOneInfo> m_listTopOneInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_GetRankDatas : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetRankDatas);
    GameMsg_C2S_GetRankDatas();
    ~GameMsg_C2S_GetRankDatas() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned char           m_nType;
    unsigned char           m_nDetailType;
    unsigned short          m_nCurrentPage;     // from 0
    unsigned char           m_nCount;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_GetRankDatasResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetRankDatasResult);
    GameMsg_S2C_GetRankDatasResult();
    ~GameMsg_S2C_GetRankDatasResult() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned short          m_nMyRank;
    unsigned short          m_nCurrentPage;
    unsigned short          m_nTotal;
    std::list<CRankRowInfo> m_listRowInfo;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_GetRankRewardRule : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE( GameMsg_C2S_GetRankRewardRule );

public:
    bool doDecode( CParamPool &IOBuff );

public:
    unsigned char           m_nDataType;
};

//////////////////////////////////////////////////////////////////////////

class RankRewardsInfo
{
public:
    RankRewardsInfo();
    ~RankRewardsInfo() {}

public:
    bool doEncode( CParamPool &IOBuff ) const;

public:
    int                 m_nBindMCoin;
    int                 m_nMoney;
    unsigned short      m_nRangeBegin;
    unsigned short      m_nRangeEnd;
    std::list<CItem>    m_listItems;
};

class GameMsg_S2C_GetRankRewardRuleResult : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE( GameMsg_S2C_GetRankRewardRuleResult );

public:
    bool doEncode( CParamPool &IOBuff );

public:
    std::string m_strRewardRule;// 奖励规则
    std::string m_strScoreRule; // 积分规则
    std::list<RankRewardsInfo>  m_listRewards;
};

#endif // __GAMEMSG_RANK_CS_H__

