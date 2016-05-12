#include "GameMsg_Map_Entry.h"

GameMsg_Map_Entry::GameMsg_Map_Entry(Creator_MSG fCreateor)
{
	m_fCreator = fCreateor;
}

GameMsg_Map_Entry::~GameMsg_Map_Entry(void)
{
}
bool GameMsg_Map_Entry::AddProcessor( IProcessorObjGet * pObjectGet,Processor_MSG  fProcessor)
{
	if( m_mapProcessor.find( ( unsigned long )pObjectGet ) !=  m_mapProcessor.end() )
	{
		ASSERT(0);
		TRACE("同一个类例的消息一个实例只能注册一个处理函数");
	}
	else
	{
		m_mapProcessor[ ( unsigned long )pObjectGet ] = new GameMsg_Processor_Entry( pObjectGet,fProcessor );
		return true;
	}
	return false;
}
void GameMsg_Map_Entry::RemoveProcessor( IProcessorObjGet * pObjectGet )
{
	MSGProcessorMap::iterator it = m_mapProcessor.find( ( unsigned long )pObjectGet );
	if(  it !=  m_mapProcessor.end() )
	{
		GameMsg_Processor_Entry * pEntry = it->second ;
		m_mapProcessor.erase(it);
		delete pEntry;
	}
}


//file end

