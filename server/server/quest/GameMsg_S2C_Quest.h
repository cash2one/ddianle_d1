#ifndef		__GAMEMSG_S2C_QUEST_H__
#define		__GAMEMSG_S2C_QUEST_H__ 

#include "../../socket/GameMsg_Base.h"
#include "../../socket/SocketDef.h"
#include "../../nsytworld/QuestMan.h"

#include <string>
#include <list>

class CParamPool;

class GameMsg_S2C_SendQuestStaticBinData:
	public GameMsg_Base
{
public:
	GameMsg_S2C_SendQuestStaticBinData();
	~GameMsg_S2C_SendQuestStaticBinData();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned short m_nBinDataSize;
	char	m_szBinData[C_MAX_QUEST_DATA_SIZE];
};

class GameMsg_S2C_AutoPushQuest :
	public GameMsg_Base
{
public:
	GameMsg_S2C_AutoPushQuest();
	~GameMsg_S2C_AutoPushQuest();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned short m_nNewQuestCount;
};


class GameMsg_S2C_UnRewardQuest :
	public GameMsg_Base
{
public:
	GameMsg_S2C_UnRewardQuest();
	~GameMsg_S2C_UnRewardQuest();
public:
	bool doEncode(CParamPool &IOBuff);

	void SetIDList(std::vector<unsigned short>& ids);
public:
	std::vector<unsigned short> m_IDs;
};

class CQuestListEntry;


class GameMsg_C2S_GetFresherQuestList:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetFresherQuestList);
	GameMsg_C2S_GetFresherQuestList();
	~GameMsg_C2S_GetFresherQuestList();
public:
	bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_SendFresherQuest:
	public GameMsg_Base
{
public:
	GameMsg_S2C_SendFresherQuest();
	~GameMsg_S2C_SendFresherQuest();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	bool m_bIsOpen;
	std::string m_strError;
	unsigned short m_nFresherQuestID;			//当前的任务
	std::list<CQuestListEntry> m_listQuestInfo;	//玩家新手七天目标数据

	unsigned char m_nSex;
};

class GameMsg_S2C_UnRewardFresherQuest :
	public GameMsg_Base
{
public:
	GameMsg_S2C_UnRewardFresherQuest();
	~GameMsg_S2C_UnRewardFresherQuest();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned short m_nQuestID;
	int m_nTargetValue;
	unsigned char m_nQuestState;
};

class GameMsg_S2C_SendQuestNewStaticBinData : 
	public GameMsg_Base
{
public:
	GameMsg_S2C_SendQuestNewStaticBinData();
	GameMsg_S2C_SendQuestNewStaticBinData(bool lastPack, unsigned short binSize,const char* buff);
	~GameMsg_S2C_SendQuestNewStaticBinData();
public:
	bool doEncode(CParamPool &IOBuff);
public:
    bool m_bLastPack;
	unsigned short m_nBinDataSize;
	char*	m_szBinData;
};

class Game_S2C_AcceptNewQuest :
	public GameMsg_Base
{
public:
	Game_S2C_AcceptNewQuest();
	~Game_S2C_AcceptNewQuest();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned short id;
};

#endif

