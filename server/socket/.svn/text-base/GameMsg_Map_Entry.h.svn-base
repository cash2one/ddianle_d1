/*
*2011-10-22
*by wangshijun
*网络消息创建与处理框架。
*/
#ifndef __GAMEMSG_MAP_ENTRY_H__
#define __GAMEMSG_MAP_ENTRY_H__
#include<map>
#include "GameMsg_Processor_Entry.h"
#include"Windefine.h"
#include"Utility.h"

#ifdef WIN32
#pragma warning(disable: 4127)
#endif // WIN32

class GameMsg_Base;
typedef GameMsg_Base* ( *Creator_MSG )(void);

typedef std::map< long , GameMsg_Processor_Entry* > MSGProcessorMap;

class GameMsg_Map_Entry
{
public:
	GameMsg_Map_Entry(Creator_MSG fCreateor);
	~GameMsg_Map_Entry(void);
	bool AddProcessor( IProcessorObjGet * pObjectGet,Processor_MSG fProcessor);
	void RemoveProcessor( IProcessorObjGet * pObjectGet );

	void ProcessMsg( GameMsg_Base & rfMsg , CSlotPeer & SlotPee )
	{
		MSGProcessorMap::iterator it = m_mapProcessor.begin();
		MSGProcessorMap::iterator endit = m_mapProcessor.end();
		for( ;it != endit;it++ )
		{
			GameMsg_Processor_Entry * pProcessorEntry = it->second;
			if( pProcessorEntry )
			{
				pProcessorEntry->OnProcess( rfMsg,SlotPee );
			}
		}
	}

	GameMsg_Base * CreateMsg( )
	{
		if( m_fCreator != NULL )
		{
			return m_fCreator( );
		}
		ASSERT( 0 );
		TRACE("发现有人注册了空的Creator_MSG函数");
		return NULL;
	}
private:
	Creator_MSG m_fCreator;
	MSGProcessorMap m_mapProcessor;
};

#endif


//file end

