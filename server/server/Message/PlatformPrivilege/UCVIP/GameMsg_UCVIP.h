#ifndef _GAMEMSG_UCVIP_H__
#define _GAMEMSG_UCVIP_H__
#pragma once

#include "../../../../socket/GameMsg_Base.h"
#include "../../../../datastructure/DataStruct_DB.h"
#include <list>

class CParamPool;

////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetUCVIPRewardStatue : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetUCVIPRewardStatue);
	GameMsg_C2S_GetUCVIPRewardStatue();
	~GameMsg_C2S_GetUCVIPRewardStatue(){}

public:
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned short m_nUCVIPLevel;

};

////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetUCVIPRewardStatueSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_GetUCVIPRewardStatueSuc();
	~GameMsg_S2C_GetUCVIPRewardStatueSuc(){}
public:
	bool doEncode(CParamPool & IOBuff);

public:
	std::map<int, unsigned char > mapRewardState; //索引，状态
	//std::list<unsigned char> listRewardState;  //有对应的权限ID，说明权限奖励可领取
};

////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetUCVIPRewardStatueFail : public GameMsg_Base
{
public:
	GameMsg_S2C_GetUCVIPRewardStatueFail(); 
	~GameMsg_S2C_GetUCVIPRewardStatueFail(){}

public:
	bool doEncode(CParamPool & IOBuff);

public:
	//unsigned char m_nErrorCode;
	std::string strError;
};
////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetUCVIPRewardDetial : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetUCVIPRewardDetial);
	GameMsg_C2S_GetUCVIPRewardDetial();
	~GameMsg_C2S_GetUCVIPRewardDetial(){}

public:
	bool doDecode(CParamPool &IOBuff);

public:
	int m_nRewardIndex;
};

class GameMsg_S2C_GetUCVIPRewardDetialSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_GetUCVIPRewardDetialSuc( std::list<CItem> &rRewardList, int nLowLevel, int nRewardIndex );
	~GameMsg_S2C_GetUCVIPRewardDetialSuc(){}

public:
	bool doEncode(CParamPool & IOBuff);

private:
	int nRewardIndex;
	int nLowLevel;
	std::list<CItem> m_rewardList;
};

class GameMsg_S2C_GetUCVIPRewardDetialFail : public GameMsg_Base
{
public:
	GameMsg_S2C_GetUCVIPRewardDetialFail();
	~GameMsg_S2C_GetUCVIPRewardDetialFail(){}

public:
	bool doEncode(CParamPool & IOBuff);
public:
	//unsigned char m_nErrorCode;
	std::string strError;
};

////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GainUCVIPReward : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GainUCVIPReward);
	GameMsg_C2S_GainUCVIPReward();
	~GameMsg_C2S_GainUCVIPReward(){}

public:
	bool doDecode(CParamPool &IOBuff);

public:
	int m_nRewardIndex;
};

////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GainUCVIPRewardSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_GainUCVIPRewardSuc();
	~GameMsg_S2C_GainUCVIPRewardSuc(){}

public:
	bool doEncode(CParamPool & IOBuff);

public:
	int m_nRewardInex;
};

////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GainUCVIPRewardFail : public GameMsg_Base
{
public:
	GameMsg_S2C_GainUCVIPRewardFail();
	~GameMsg_S2C_GainUCVIPRewardFail(){}

public:
	bool doEncode(CParamPool & IOBuff);

public:
	//unsigned char m_nErrorCode;
	std::string strError;
};


#endif //_GAMEMSG_UCVIP_H__
