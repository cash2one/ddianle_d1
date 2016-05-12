#include "DBQuery_Processor_Entry.h"
#include "Query.h"
#include "Log.h"

DBQuery_Processor_Entry::DBQuery_Processor_Entry(IQueryProcessorObjGet * pIObjGet ,  Processor_DBQuery fProcessor)
{
	m_pObjGet = pIObjGet;
	m_fProcessor = fProcessor;
}

DBQuery_Processor_Entry::~DBQuery_Processor_Entry(void)
{
}

void DBQuery_Processor_Entry::OnProcess( QueryBase & rfQuery )
{
	if( m_pObjGet) 
	{
		GameMsg_Processor *pObj = m_pObjGet->ProcessorObjectGet( rfQuery );
		if( pObj && m_fProcessor )
		{
			( pObj->*m_fProcessor )( rfQuery );
		}
		else
		{
			size_t nSize = rfQuery.ReleaseMemberMemory();
			if ( nSize > 0 )
			{
				WriteLog(LOGLEVEL_ERROR, "call the callback of Query(type=%d) failed, release %u bytes", rfQuery.m_nQueryType, nSize);
			}
		}
	}
}


//file end

