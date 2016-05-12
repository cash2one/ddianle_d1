#ifndef __GAMEMSG_COMPLETEGUIDE_H__
#define __GAMEMSG_COMPLETEGUIDE_H__

#include "../../socket/GameMsg_Base.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

using namespace std;

class GameMsg_C2S_CompleteTeachGuide
	:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_CompleteTeachGuide);
	GameMsg_C2S_CompleteTeachGuide();	
	~GameMsg_C2S_CompleteTeachGuide();
public:
	bool doDecode(CParamPool &IOBuff);
};


class GameMsg_S2C_CompleteGuideResult
	:public GameMsg_Base
{
public:
	GameMsg_S2C_CompleteGuideResult();	
	~GameMsg_S2C_CompleteGuideResult();
public:
	bool doEncode(CParamPool &IOBuff);
};

class GameMsg_C2S_CompleteProcGuide
	:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_CompleteProcGuide);
	GameMsg_C2S_CompleteProcGuide();	
	~GameMsg_C2S_CompleteProcGuide();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	char m_chProcStep;
};

class GameMsg_S2C_CompleteProcGuideResult
	:public GameMsg_Base
{
public:
	GameMsg_S2C_CompleteProcGuideResult();
	~GameMsg_S2C_CompleteProcGuideResult();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	char m_chProcStep;
};

class GameMsg_C2S_SelectGuideFace 
	:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SelectGuideFace);
	GameMsg_C2S_SelectGuideFace();
	~GameMsg_C2S_SelectGuideFace();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_Face;
};

class GameMsg_S2C_SelectGuideFaceRet
	:public GameMsg_Base
{
public:
	GameMsg_S2C_SelectGuideFaceRet();
	~GameMsg_S2C_SelectGuideFaceRet();
public:
	bool doEncode(CParamPool &IOBuff);
	void SetItems(std::list<CItem>& items);
public:
	unsigned char m_Result;
	std::list<CItem> m_Items;
};

class GameMsg_C2S_CompleteAnimation
	:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_CompleteAnimation);
	GameMsg_C2S_CompleteAnimation();
	~GameMsg_C2S_CompleteAnimation();
public:
	bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_CompleteAnimationResult
	:public GameMsg_Base
{
public:
	GameMsg_S2C_CompleteAnimationResult();
	~GameMsg_S2C_CompleteAnimationResult();
public:
	bool doEncode(CParamPool &IOBuff);
};


class GameMsg_C2S_ReciveGuideExperienceCard
	:public GameMsg_Base
{
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ReciveGuideExperienceCard);
public:
	GameMsg_C2S_ReciveGuideExperienceCard();
	~GameMsg_C2S_ReciveGuideExperienceCard();
public:
	bool doDecode(CParamPool &IOBuff);
};


class GameMsg_S2C_ReciveGuideExperienceCardRes
	:public GameMsg_Base
{
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_ReciveGuideExperienceCardRes);
public:
	GameMsg_S2C_ReciveGuideExperienceCardRes();
	~GameMsg_S2C_ReciveGuideExperienceCardRes();
public:
	bool doEncode(CParamPool &IOBuff);

	unsigned char m_nRes; // 0:OK, 1:已经领取
};


#endif

