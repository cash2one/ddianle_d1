#ifndef __GAMEMSG_G2S_CHATWINDOW_H__
#define __GAMEMSG_G2S_CHATWINDOW_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include <string>

using namespace std;
class CParamPool;

class GameMsg_G2S_ChatWindow :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_ChatWindow);
	GameMsg_G2S_ChatWindow(void);
	~GameMsg_G2S_ChatWindow(void);
public:
	string          m_strText;
public:
	virtual bool doEncode( CParamPool& IOBuff );
	virtual bool doDecode( CParamPool& IOBuff );
};

#endif

//file end

