#ifndef __GAMEMSG_S2C_AWARD_MSG_H__
#define __GAMEMSG_S2C_AWARD_MSG_H__

#include "../../socket/GameMsg_Base.h"


///////////////////////////////////////////////////////
class GameMsg_S2C_ItemAwardSuc : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_ItemAwardSuc);

	GameMsg_S2C_ItemAwardSuc(void);
	~GameMsg_S2C_ItemAwardSuc(void);

	virtual bool doEncode( CParamPool &IOBuff );

public:
	int m_nItemID;
	int m_nItemCount;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_ItemAwardFail : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_ItemAwardFail);

	GameMsg_S2C_ItemAwardFail(void);
	~GameMsg_S2C_ItemAwardFail(void);

	virtual bool doEncode( CParamPool &IOBuff );

public:
	string m_strError;
};


#endif


// end of file


