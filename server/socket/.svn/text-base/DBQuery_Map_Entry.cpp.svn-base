#include "DBQuery_Map_Entry.h"

CDBQuery_Map_Entry::CDBQuery_Map_Entry(void)
{
}

CDBQuery_Map_Entry::~CDBQuery_Map_Entry(void)
{
}


bool CDBQuery_Map_Entry::AddProcessor( IQueryProcessorObjGet * pObjectGet,Processor_DBQuery  fProcessor)
{
	if( m_mapProcessor.find( pObjectGet ) !=  m_mapProcessor.end() )
	{
		ASSERT(0);
		TRACE("同一个类例的消息一个实例只能注册一个处理函数");
	}
	else
	{
		m_mapProcessor[ pObjectGet ] = new DBQuery_Processor_Entry( pObjectGet,fProcessor );
		return true;
	}
	return false;
}
void CDBQuery_Map_Entry::RemoveProcessor( IQueryProcessorObjGet * pObjectGet )
{
	DBQueryProcessorMap::iterator it = m_mapProcessor.find( pObjectGet );
	if(  it !=  m_mapProcessor.end() )
	{
		DBQuery_Processor_Entry * pEntry = it->second ;
		m_mapProcessor.erase(it);
		delete pEntry;
	}
}


//file end

