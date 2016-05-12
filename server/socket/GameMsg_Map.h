/*
*2011-10-22
*by wangshijun
*网络消息创建与处理框架。
*/
#ifndef __GAMEMSG_MAP_H__
#define __GAMEMSG_MAP_H__

#include<map>
#include "GameMsg_Map_Entry.h"

class GameMsg_Map_Entry;
class GameMsg_Base;

#define GAMEMSG_REGISTERCREATOR(class) {\
	class msg;\
	GameMsg_Map::Instantce().AddMsgCreator(msg.nMsg,static_cast<Creator_MSG>(class::Creator_##class));\
}

#define RegMsgCreatorAndProcessor(msgclass, func) {\
	msgclass msg;\
	GameMsg_Map::Instantce().AddMsgCreator(msg.nMsg, static_cast<Creator_MSG>(msgclass::Creator_##msgclass));\
	RegMsgProcessorImpl(msg.nMsg, static_cast<Processor_MSG>(func));\
}

typedef std::map< unsigned short ,GameMsg_Map_Entry * > MSGEntryCollection;

class GameMsg_Map
{
public:
	GameMsg_Map(void);
	~GameMsg_Map(void);
	static GameMsg_Map& Instantce()
	{
		if( s_pInstance == NULL )
		{
			s_pInstance = new GameMsg_Map();
		}
		return *s_pInstance;
	}
	void ProcessMsg( GameMsg_Base & refMsg,CSlotPeer & SlotPeer );
public :
	GameMsg_Base * CreateMsgFromType( unsigned short nMsg );

	void AddMsgCreator( unsigned short nType,Creator_MSG creator );
	bool AddMsgProcessor( unsigned short nType ,IProcessorObjGet * pProcssorGet , Processor_MSG fProcssor);
	void RemoveMsgProcessor( unsigned short nType ,IProcessorObjGet * pProcssorGet );
private :
	static GameMsg_Map * s_pInstance;
	MSGEntryCollection m_mapEntry;
};

#endif


//file end

