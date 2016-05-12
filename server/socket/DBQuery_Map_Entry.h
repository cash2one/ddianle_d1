/*
*2011-11-10
*by wangshijun
*数据库消息处理框架。
*/
#ifndef __DBQUERY_MAP_ENTRY_H__
#define __DBQUERY_MAP_ENTRY_H__
#include <map>
#include "DBQuery_Processor_Entry.h"
#include "Windefine.h"
#include "Utility.h"
class QueryBase;
typedef std::map< IQueryProcessorObjGet * , DBQuery_Processor_Entry* > DBQueryProcessorMap;
class CDBQuery_Map_Entry
{
public:
	CDBQuery_Map_Entry(void);
	~CDBQuery_Map_Entry(void);

	bool AddProcessor( IQueryProcessorObjGet * pObjectGet,Processor_DBQuery fProcessor);
	void RemoveProcessor( IQueryProcessorObjGet * pObjectGet );

	void ProcessMsg( QueryBase & rfQuery )
	{
		DBQueryProcessorMap::iterator it = m_mapProcessor.begin();
		DBQueryProcessorMap::iterator endit = m_mapProcessor.end();
		for( ;it != endit;it++ )
		{
			DBQuery_Processor_Entry * pProcessorEntry = it->second;
			if( pProcessorEntry )
			{
				pProcessorEntry->OnProcess( rfQuery );
			}
		}
	}

private:
	DBQueryProcessorMap m_mapProcessor;
};

#endif


//file end

