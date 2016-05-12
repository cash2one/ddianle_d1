#ifndef		__GAMEMSG_S2C_GETQUESTREWARDRESULT_H__ 
#define		__GAMEMSG_S2C_GETQUESTREWARDRESULT_H__ 

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include <string>

class CParamPool;
using namespace std;

class GameMsg_S2C_GetQuestRewardResult:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetQuestRewardResult(unsigned short nQuestID = 0, unsigned char nResult = eGetQuestReward_Success, string strResult = "");
	~GameMsg_S2C_GetQuestRewardResult();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned short m_nQuestID;
	unsigned char m_nReuslt ;
	string m_strResult;
};


class GameMsg_S2C_GetFresherQuestRewardSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetFresherQuestRewardSuc();
	~GameMsg_S2C_GetFresherQuestRewardSuc();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned short m_nQuestID;
};

class GameMsg_S2C_GetFresherQuestRewardFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetFresherQuestRewardFail();
	~GameMsg_S2C_GetFresherQuestRewardFail();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	string m_strError;
};

#endif
// end of file


