#ifndef __GAMEMSG_TIMEEGG_MSG_H__
#define __GAMEMSG_TIMEEGG_MSG_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include <string>
#include <list>
using namespace std;

class CParamPool;
//查询埋蛋消耗及奖励
class GameMsg_C2S_QueryTimeEggReward : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_QueryTimeEggReward);
	GameMsg_C2S_QueryTimeEggReward();
	~GameMsg_C2S_QueryTimeEggReward();
public:
	virtual bool doDecode(CParamPool &IOBuff);
};
//查询埋蛋消耗及奖励返回
class GameMsg_S2C_QueryTimeEggReward : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_QueryTimeEggReward);
	GameMsg_S2C_QueryTimeEggReward();
	~GameMsg_S2C_QueryTimeEggReward();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	list<CItem> m_listTogetherReward;//一起开蛋的玩家奖励道具信息
	list<CItem> m_listReward;//玩家固定奖励道具信息
	int m_nFixedMoneyReward;//固定奖励金券
	int m_nExtraMoneyReward;//额外奖励金券
};
//时光蛋信息同步，此消息会在玩家的时光蛋列表有任意更新时发送
class GameMsg_S2C_SynRoleTimeEggInfo : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_SynRoleTimeEggInfo);
	GameMsg_S2C_SynRoleTimeEggInfo();
	~GameMsg_S2C_SynRoleTimeEggInfo();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::vector<TimeEggEntry*> m_eggs;
	string m_strHelpTitle;
	string m_strHelpContent;
};
#endif

