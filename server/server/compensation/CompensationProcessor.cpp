#include "CompensationProcessor.h"
#include "../../socket/GameMsg_Map.h"
#include "../libServerFrame/QueryManager.h"
#include "../group/GroupServerCirculator.h"
#include "CompensationManager.h"

#include "../util/CommonFunc.h"
#include "../GM/GameMsg_GM_share.h"
#include "../GM/GameMsg_G2S_NewCompensation.h"

extern CQueryManager* g_pQueryMgr;


CCompensationProcessor::CCompensationProcessor()
:GameMsg_Processor(true, true)
,m_bHasLoad(false)
{
}

CCompensationProcessor::~CCompensationProcessor()
{
}

CCompensationProcessor& CCompensationProcessor::Instance()
{
	static CCompensationProcessor s_Instance;
	return s_Instance;
}

void CCompensationProcessor::InitCompensation()
{
	_RegisterMessage();
	_RegisterQuery();

	_LoadCompensation();
}

void CCompensationProcessor::OnGMUpdateCompensation(GameMsg_Base& msg, CSlotPeer& slotPeer)
{
	GameMsg_M2G_UpdateCompensation& rMsg = (GameMsg_M2G_UpdateCompensation&)msg;

	std::list<CItem> listMaleInfo;
	std::list<CItem> listFemaleInfo;

	bool bItemVaild = true;
	if ( !rMsg.m_strItemMale.empty() && !TransferItemList( rMsg.m_strItemMale.c_str(), listMaleInfo ) )
	{
		bItemVaild = false;
	}

	if ( bItemVaild && !rMsg.m_strItemFemale.empty() && !TransferItemList( rMsg.m_strItemFemale.c_str(), listFemaleInfo ) )
	{
		bItemVaild = false;
	}

	if ( bItemVaild )
	{
		CompensationInfo* pCompensationInfo = new CompensationInfo();
		pCompensationInfo->m_nEndTime = rMsg.m_nEndTime;
		pCompensationInfo->m_nMoney = rMsg.m_nMoney;
		pCompensationInfo->m_strTitle = rMsg.m_strTitle;
		pCompensationInfo->m_strContent = rMsg.m_strContent;
		pCompensationInfo->m_strMaleItem = rMsg.m_strItemMale;
		pCompensationInfo->m_strFemaleItem = rMsg.m_strItemFemale;
		pCompensationInfo->m_MaleItems = listMaleInfo;
		pCompensationInfo->m_FemaleItems = listFemaleInfo;

		g_pQueryMgr->AddQuery( Query_InsertCompensation, slotPeer.GetSlot(), pCompensationInfo );
	}
	else
	{
		GameMsg_G2M_UpdateCompensationRes resMsg;
		resMsg.m_strError.assign( "Valid item reward error!" );
		SendMsgToGMServer(&resMsg,slotPeer.GetSlot());
	}
}

//当GameServer连到GroupServer
void CCompensationProcessor::OnGameServerConnected(unsigned short nSlot)
{
	if ( m_bHasLoad )
	{
		_SyncCompensation( nSlot );
	}
}

void CCompensationProcessor::_RegisterMessage()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_M2G_UpdateCompensation);

	RegMsgProcessor( MSG_GM_M2G_UpdateCompensation, &CCompensationProcessor::OnGMUpdateCompensation );
}

void CCompensationProcessor::_RegisterQuery()
{
	RegQueryProcessor( Query_LoadCompensation, &CCompensationProcessor::_OnLoadCompensation );
	RegQueryProcessor( Query_InsertCompensation, &CCompensationProcessor::_OnInsertCompensation );
}

void CCompensationProcessor::_LoadCompensation()
{
	CompensationInfoDB* pDBInfo = new CompensationInfoDB();
	g_pQueryMgr->AddQuery( Query_LoadCompensation, 0, pDBInfo );
}

void CCompensationProcessor::_SyncCompensation(int nSlot)
{
	int nTimeNow = time( NULL );
	GameMsg_G2S_SyncCompensation sendMsg;

	CompensationMgr& cpsMgr = CompensationMgr::Instance();
	for ( std::map<int, CompensationInfo>::iterator it = cpsMgr.m_CompensationMap.begin(); it != cpsMgr.m_CompensationMap.end(); ++it )
	{
		if ( it->second.m_nEndTime > nTimeNow )
		{
			sendMsg.m_CompensationMap.insert( std::make_pair( it->second.m_nID, it->second ) );
		}
	}

	if ( !sendMsg.m_CompensationMap.empty() )
	{
		if ( nSlot >= 0 )
		{
			SendMsg2GameServerBySlot( &sendMsg, (unsigned short)nSlot );
		}
		else
		{
			BroadcastMsg2GameServer( &sendMsg );
		}
	}
}

void CCompensationProcessor::_OnLoadCompensation(QueryBase& query)
{
	CompensationInfoDB* pDBInfo = (CompensationInfoDB*)query.m_pAttachment;
	if ( pDBInfo != NULL )
	{
		for ( std::map<int, CompensationInfo>::iterator it = pDBInfo->m_Compensations.begin(); it != pDBInfo->m_Compensations.end(); ++it )
		{
			CompensationMgr::Instance().InsertCompensation( it->second );
		}

		delete pDBInfo,pDBInfo = NULL;
	}

	m_bHasLoad = true;
	_SyncCompensation( -1 );
}

void CCompensationProcessor::_OnInsertCompensation(QueryBase& query)
{
	std::string strError = "";
	CompensationInfo* pDBInfo = (CompensationInfo*)query.m_pAttachment;

	if (query.m_nResult == 0)
	{
		//将结果添加到本地数据中
		if (pDBInfo != NULL)
		{
			int nTimeNow = time( NULL );
			if ( pDBInfo->m_nEndTime > nTimeNow )
			{
				CompensationMgr::Instance().InsertCompensation( *pDBInfo );

				GameMsg_G2S_NewCompensation sendMsg;
				sendMsg.m_CompensationInfo = *pDBInfo;
				BroadcastMsg2GameServer( &sendMsg );
			}
		}
		else
		{
			strError.assign( "Unknow error!" );
		}
	}
	else
	{
		strError.assign( "Query fail!" );
	}

	delete pDBInfo,pDBInfo = NULL;

	GameMsg_G2M_UpdateCompensationRes resMsg;
	resMsg.m_strError = strError;
	unsigned short nSolt = (unsigned short )query.m_nAttachment;
	SendMsgToGMServer(&resMsg,nSolt);
}


