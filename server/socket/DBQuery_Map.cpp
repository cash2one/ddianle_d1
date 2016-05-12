#include "DBQuery_Map.h"
#include "Query.h"
CDBQuery_Map * CDBQuery_Map::s_pInstance = NULL;
CDBQuery_Map::CDBQuery_Map(void)
{
}

CDBQuery_Map::~CDBQuery_Map(void)
{
}

void CDBQuery_Map::ProcessMsg( QueryBase & refQeury )
{
	DBQueryEntryCollection::iterator it = m_mapEntry.find( (unsigned short)refQeury.m_nQueryType);
	if( it != m_mapEntry.end() )
	{
		CDBQuery_Map_Entry * pEntry = it->second;
		if( pEntry )
		{
			pEntry->ProcessMsg( refQeury );
		}
	}
}
bool CDBQuery_Map::AddMsgProcessor( unsigned short nType ,IQueryProcessorObjGet * pProcssorGet  , Processor_DBQuery fProcssor)
{
	if( nType != 0 )
	{
		DBQueryEntryCollection::iterator it = m_mapEntry.find( nType );
		CDBQuery_Map_Entry * pEntry = NULL;
		if(  it == m_mapEntry.end() )
		{
			pEntry = new CDBQuery_Map_Entry(  );
			m_mapEntry[nType] = pEntry;
		}
		else//目前不支持一个db query请求注册多个回调方法，因为在回调方法中涉及到query参数的内存释放，会降低服务器的稳定性
		{
			ASSERT(0);
			//pEntry = it->second;
		}

		if( pEntry )
		{
			return pEntry->AddProcessor( pProcssorGet,fProcssor);
		}
		else
		{
			ASSERT( 0 );
			TRACE( "决对不应该有空" );
		}

	}
	else
	{
		ASSERT( 0 );
		TRACE( "不应该有0 type的消息。" );
	}
	return false;
}
void CDBQuery_Map::RemoveMsgProcessor( unsigned short nType ,IQueryProcessorObjGet * pProcssorGet )
{
	if( nType != 0 )
	{
		DBQueryEntryCollection::iterator it = m_mapEntry.find( nType );
		if(  it == m_mapEntry.end() )
		{
			ASSERT( 0 );
			TRACE( "还没有注册这个消息类型的处理函数，怎么就Remove去了。" );
		}
		else
		{
			CDBQuery_Map_Entry * pEntry = it->second;
			if( pEntry )
			{
				pEntry->RemoveProcessor( pProcssorGet );
			}
			else
			{
				ASSERT( 0 );
				TRACE( "决对不应该有空" );
			}
		}
	}
	else
	{
		ASSERT( 0 );
		TRACE( "不应该有0 type的消息。" );
	}
}


