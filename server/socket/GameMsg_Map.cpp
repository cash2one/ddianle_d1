#include "GameMsg_Map.h"
#include "GameMsg_Base.h"
#include "Log.h"

GameMsg_Map * GameMsg_Map::s_pInstance = NULL;
GameMsg_Map::GameMsg_Map(void)
{
}

GameMsg_Map::~GameMsg_Map(void)
{
}

void GameMsg_Map::AddMsgCreator( unsigned short nType,Creator_MSG creator )
{
	if( m_mapEntry.find( nType ) != m_mapEntry.end() )
	{
		ASSERT( 0 );
		TRACE( "�ظ�ע����ͬһ�����͵Ĵ�������!" );
	}
	else
	{
		m_mapEntry[nType] = new GameMsg_Map_Entry( creator );
	}
}

bool GameMsg_Map::AddMsgProcessor( unsigned short nType ,IProcessorObjGet * pProcssorGet , Processor_MSG fProcssor)
{
	if( nType != 0 )
	{
		MSGEntryCollection::iterator it = m_mapEntry.find( nType );
		if(  it == m_mapEntry.end() )
		{
			ASSERT( 0 );
			TRACE( "��û��ע�������Ϣ���͵Ĵ���������ע�ᴦ������������Ч��" );
		}
		else
		{
			GameMsg_Map_Entry * pEntry = it->second;
			if( pEntry )
			{
				return pEntry->AddProcessor( pProcssorGet,fProcssor);
			}
			else
			{
				ASSERT( 0 );
				TRACE( "���Բ�Ӧ���п�" );
			}
		}
	}
	else
	{
		ASSERT( 0 );
		TRACE( "��Ӧ����0 type����Ϣ��" );
	}
	return false;
}

void GameMsg_Map::RemoveMsgProcessor( unsigned short nType ,IProcessorObjGet * pProcssorGet )
{
	if( nType != 0 )
	{
		MSGEntryCollection::iterator it = m_mapEntry.find( nType );
		if(  it == m_mapEntry.end() )
		{
			ASSERT( 0 );
			TRACE( "��û��ע�������Ϣ���͵Ĵ�����������ô��Removeȥ�ˡ�" );
		}
		else
		{
			GameMsg_Map_Entry * pEntry = it->second;
			if( pEntry )
			{
				pEntry->RemoveProcessor( pProcssorGet );
			}
			else
			{
				ASSERT( 0 );
				TRACE( "���Բ�Ӧ���п�" );
			}
		}
	}
	else
	{
		ASSERT( 0 );
		TRACE( "��Ӧ����0 type����Ϣ��" );
	}
}
GameMsg_Base * GameMsg_Map::CreateMsgFromType( unsigned short nMsg )
{
	GameMsg_Base * pMsg = NULL;
	MSGEntryCollection::iterator it = m_mapEntry.find( nMsg );
	if( it != m_mapEntry.end() )
	{
		GameMsg_Map_Entry * pEntry = it->second;
		if( pEntry )
		{
			pMsg = pEntry->CreateMsg();
		}
	}

	return pMsg;
}
void GameMsg_Map::ProcessMsg( GameMsg_Base & refMsg ,CSlotPeer & SlotPee )
{
	MSGEntryCollection::iterator it = m_mapEntry.find( refMsg.nMsg );
	if( it != m_mapEntry.end() )
	{
		GameMsg_Map_Entry * pEntry = it->second;
		if( pEntry )
		{
			pEntry->ProcessMsg( refMsg,SlotPee );
		}
	}
	else
	{
		WriteLog(LOGLEVEL_WARNING, "WARNING: GameMsg_Map::ProcessMsg. Get unknown msg. MsgID=%u.", refMsg.nMsg);
	}
}

//file end

