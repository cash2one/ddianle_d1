/*
*2011-11-10
*by wangshijun
*数据库消息处理框架。
*/
#ifndef __DBQUERY_PROCESSOR_ENTRY_H__
#define __DBQUERY_PROCESSOR_ENTRY_H__
#include "GameMsg_Processor.h"
#include "IQueryProcessorObjGet.h"

class QueryBase;
class GameMsg_Processor;
class DBQuery_Processor_Entry
{
public:
	DBQuery_Processor_Entry(IQueryProcessorObjGet * pIObjGet ,  Processor_DBQuery fProcessor);
	~DBQuery_Processor_Entry(void);

	void OnProcess( QueryBase & rfQuery );

private:
	IQueryProcessorObjGet * m_pObjGet;
	Processor_DBQuery m_fProcessor;
};
#endif


//file end

