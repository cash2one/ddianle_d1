/*
*2011-11-10
*by wangshijun
*数据库消息处理框架。
*/
#ifndef __DBQUERY_MAP_H__
#define __DBQUERY_MAP_H__
#include<map>
#include "DBQuery_Map_Entry.h"
class CDBQuery_Map_Entry;
class QueryBase;
typedef std::map< unsigned short ,CDBQuery_Map_Entry * > DBQueryEntryCollection;

class CDBQuery_Map
{
public:
	CDBQuery_Map(void);
	~CDBQuery_Map(void);

	static CDBQuery_Map& Instantce()
	{
		if( s_pInstance == NULL )
		{
			s_pInstance = new CDBQuery_Map();
		}
		return *s_pInstance;
	}
	void ProcessMsg( QueryBase & refQeury );

	bool AddMsgProcessor( unsigned short nType ,IQueryProcessorObjGet * pProcssorGet  , Processor_DBQuery fProcssor);
	void RemoveMsgProcessor( unsigned short nType ,IQueryProcessorObjGet * pProcssorGet );
private :
	static CDBQuery_Map * s_pInstance;
	DBQueryEntryCollection m_mapEntry;
};
#endif

//file end

