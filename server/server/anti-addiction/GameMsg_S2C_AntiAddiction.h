#ifndef __GAMEMSG_S2C_ANTIADDICTION_MSG_H__
#define __GAMEMSG_S2C_ANTIADDICTION_MSG_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"

///////////////////////////////////////////////////////
class GameMsg_S2C_AntiAddictionTip : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_AntiAddictionTip);

	GameMsg_S2C_AntiAddictionTip(void);
	~GameMsg_S2C_AntiAddictionTip(void);

	virtual bool doEncode( CParamPool &IOBuff );
public:
	std::string m_strTipText;
};
#endif

