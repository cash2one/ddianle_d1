#ifndef __GAMEMSG_C2S_AWARD_MSG_H__
#define __GAMEMSG_C2S_AWARD_MSG_H__

#include "../../socket/GameMsg_Base.h"


///////////////////////////////////////////////////////
class GameMsg_C2S_ItemAward : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ItemAward);

	GameMsg_C2S_ItemAward(void);
	~GameMsg_C2S_ItemAward(void);

	virtual bool doDecode( CParamPool &IOBuff );

public:
	std::string m_strItemCode;
};

#endif


// end of file


