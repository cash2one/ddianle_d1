#ifndef		__GAMEMSG_C2S_GETQUESTLIST_H__
#define		__GAMEMSG_C2S_GETQUESTLIST_H__ 

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_C2S_GetQuestList : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetQuestList);
    GameMsg_C2S_GetQuestList();
    ~GameMsg_C2S_GetQuestList();

public:
    bool doDecode(CParamPool &IOBuff);
};

#endif

