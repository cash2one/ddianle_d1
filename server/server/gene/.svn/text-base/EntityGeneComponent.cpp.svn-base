#include "EntityGeneComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../item/EntityItemComponent.h"
#include "../room/RoomComponent.h"
#include "../room/Room.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../Couple/WeddingRoom.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../PhotoRoom/PhotoRoom.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../AmuseRoom/AmuseRoom.h"
#include "../lua/InitLua.h"
#include "../login/GameMsg_Player_CreateRoleRes.h"
#include <time.h>
#include "../player/GameMsg_Buff.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../logic/EntityTimerComponent.h"
#include "../logic/EntityComponentEventID.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../CeremonyRoom/ceremonyRoom.h"
#include "../roomLib/GameMsgNewRoom.h"
#include "../roomLib/GameMsgRoomBroadcast.h"

CEntityGeneComponent::CEntityGeneComponent(void)
:CEntityComponent(true,true)
,m_pAttrCOM(NULL)
,m_pNetCOM(NULL)
,m_pItemCOM(NULL)
,m_pStageRoomCOM(NULL)
,m_pCoupleCOM(NULL)
,m_pPhotoRoomCOM(NULL)
,m_pAmuseRoomCOM(NULL)
,m_pRoleDanceGroup(NULL)
{
}

CEntityGeneComponent::~CEntityGeneComponent(void)
{
	for ( GeneDataMap::iterator it = m_AllGene.begin(); it != m_AllGene.end(); ++it )
	{
		if ( it->second != NULL )
		{
			delete it->second;
			it->second = NULL;
		}
	}

	m_AllGene.clear();
}

void CEntityGeneComponent::Start()
{
	m_pAttrCOM = GetComponent<CRoleAttribute>();
	m_pNetCOM = GetComponent<CRoleNet>();
	m_pItemCOM = GetComponent<CRoleItem>();
	m_pStageRoomCOM = GetComponent<CRoleRoom>();
	m_pCoupleCOM = GetComponent<CRoleCouple>();
	m_pPhotoRoomCOM = GetComponent<CRolePhotoRoom>();
	m_pAmuseRoomCOM = GetComponent<CRoleAmuseRoom>();
    m_pRoleDanceGroup = GetComponent<CRoleDanceGroup>();
}

bool CEntityGeneComponent::CreateFromDB(ROLEINFO_DB* pRoleDBInfo)
{
	m_AllGene.clear();

	if ( pRoleDBInfo != NULL )
	{
		int nNowTime = (int)time( NULL );

		for ( std::list<GeneData>::iterator it = pRoleDBInfo->listGene.begin(); it != pRoleDBInfo->listGene.end(); ++it )
		{
			if ( m_AllGene.find( it->nGeneId ) == m_AllGene.end() )
			{
				GENE_TYPE eGeneType = _GetGeneType( it->nGeneId );

				if ( _IsDurationGeneByType(eGeneType) )
				{
					if ( it->nParam2 > nNowTime )		//not expired
					{
						GeneData* pGeneData = new GeneData();
						*pGeneData = *it;
						pGeneData->nType = (unsigned char)eGeneType;

						m_AllGene.insert( std::make_pair( pGeneData->nGeneId, pGeneData ) );
					}
					else
					{
						_HandleGeneDB( QUERY_DeleteRoleGene, *it );
					}
				}
				else if ( eGeneType == GeneType_Matchs )
				{
					if ( it->nParam2 > 0 )				//not expired
					{
						GeneData* pGeneData = new GeneData();
						*pGeneData = *it;
						pGeneData->nType = (unsigned char)eGeneType;

						m_AllGene.insert( std::make_pair( pGeneData->nGeneId, pGeneData ) );
					}
					else
					{
						_HandleGeneDB( QUERY_DeleteRoleGene, *it );
					}
				}
			}
		}
	}

	return true;
}

bool CEntityGeneComponent::PacketToDB(ROLEINFO_DB *pRoleDBInfo) const
{
	if ( pRoleDBInfo != NULL )
	{
		for ( GeneDataMap::const_iterator it = m_AllGene.begin(); it != m_AllGene.end(); ++it )
		{
			if ( it->second != NULL )
			{
				pRoleDBInfo->listGene.push_back( *(it->second) );
			}
		}

		return true;
	}

	return false;
}

bool CEntityGeneComponent::PacketToCache( ROLEINFO_DB* pRoleInforDB ) const
{
    if ( pRoleInforDB == NULL )
        return false;

    for ( GeneDataMap::const_iterator itr = m_AllGene.begin(); itr != m_AllGene.end(); ++itr )
    {
        if ( itr->second == NULL )
            continue;

        pRoleInforDB->listGene.push_back( *itr->second );
    }

    return true;
}

void CEntityGeneComponent::SerializeComponent(CParamPool &IOBuff)
{
	int nOldPos = IOBuff.GetCurPosition();
	IOBuff.AddUShort(0);

	int nGeneCount = 0;

	for ( GeneDataMap::iterator it = m_AllGene.begin(); it != m_AllGene.end(); ++it )
	{
		if( it->second != NULL )
		{
			it->second->doEncode( IOBuff );
			++nGeneCount;
		}
	}

	int nFinalPos = IOBuff.GetCurPosition();
	IOBuff.SetCurPosition( nOldPos );
	IOBuff.ReplaceShort( (short)nGeneCount );
	IOBuff.SetCurPosition( nFinalPos );
}

void CEntityGeneComponent::OnLogin()
{
	int nNowTime = (int)time( NULL );

	for ( GeneDataMap::iterator it = m_AllGene.begin(); it != m_AllGene.end(); ++it )
	{
		if( it->second != NULL )
		{
			if ( it->second->nGeneId == Gene_ID_Transform )
			{
				CEntityTimerComponent* pTimer = GetComponent<CRoleTimer>();
				if ( pTimer != NULL )
				{
					pTimer->AddTimer(nNowTime+15, eComponentEvent_DelayTransfrom, it->second->nGeneId);
					continue;
				}
			}
			GeneData geneData( *(it->second) );

			if (geneData.nType != GeneType_Equip  && geneData.nType != GeneType_Couple ) 
			{
				if ( _IsDurationGeneByType(geneData.nType) )
				{
					geneData.nParam2 -= nNowTime;
				}
				_AddGeneAction( geneData, m_pNetCOM->GetPlayerLogicIndex() );
			}
		}
	}
}

void CEntityGeneComponent::OnLogout()
{

}

void CEntityGeneComponent::OnUpdate(const unsigned long & nTimeElapsed)
{
	int nNowTime = (int)time( NULL );

	for ( GeneDataMap::iterator it = m_AllGene.begin(); it != m_AllGene.end(); )
	{
		GeneData* pGeneData = it->second;

		if ( pGeneData == NULL || ( _IsDurationGeneByType(pGeneData->nType) && pGeneData->nParam2 <= nNowTime ) )
		{
			if ( pGeneData != NULL )
			{
				_DelGeneAction( *pGeneData );
				_NotifyDelGene( pGeneData->nGeneId );

				_HandleGeneDB( QUERY_DeleteRoleGene, *pGeneData );
			}

			m_AllGene.erase( it++ );
			delete pGeneData;
		}
		else
		{
			++it;
		}
	}
}

GeneData* CEntityGeneComponent::FindGene(unsigned short nGeneID)
{
	GeneDataMap::iterator it = m_AllGene.find( nGeneID );
	if ( it != m_AllGene.end() )
	{
		return it->second;
	}

	return NULL;
}

bool CEntityGeneComponent::AddGene(unsigned short nGeneID, int nParam1, int nParam2, const char *szParam, int nTargetIndex)
{
	bool bRes = false;

	if ( nGeneID > 0 )
	{
		GeneDataMap::iterator it = m_AllGene.find( nGeneID );
		
		if ( it != m_AllGene.end() )
		{
			GeneData* pGeneData = it->second;

			if ( pGeneData != NULL )
			{
				if ( pGeneData->nType == GeneType_Duration || pGeneData->nType == GeneType_Matchs )
				{
					bRes = true;

					//fufeng: new rule, no matter param1 is the same or not, add the effect
					pGeneData->nParam2 += nParam2;

					_HandleGeneDB( QUERY_UpdateRoleGene, *pGeneData );
				}
				else if ( pGeneData->nType == GeneType_DurationByParam )//add by fanliangyuan
				{
					bRes = true;

					if ( pGeneData->nParam1 == nParam1 )
					{
						pGeneData->nParam2 += nParam2;

						_HandleGeneDB( QUERY_UpdateRoleGene, *pGeneData );
					}
					else
					{
						DelGene(nGeneID);
						bRes = AddGene(nGeneID, nParam1, nParam2, szParam, nTargetIndex);
					}
				}
			}
		}
		else
		{
			GeneData* pGeneData = new GeneData( nGeneID, nParam1, nParam2, szParam );

			if ( _AddGeneAction( *pGeneData, nTargetIndex ) )
			{
				bRes = true;

				if ( pGeneData->nType != GeneType_OneTime )
				{
					_NotifyAddGene( *pGeneData );

					if ( _IsDurationGeneByType(pGeneData->nType) )
					{
						pGeneData->nParam2 += (int)time( NULL );
					}

					m_AllGene.insert( std::make_pair( pGeneData->nGeneId, pGeneData ) );

					if ( _IsDurationGeneByType(pGeneData->nType) || pGeneData->nType == GeneType_Matchs )
					{
						_HandleGeneDB( QUERY_AddRoleGene, *pGeneData );
					}
				}
				else
				{
					delete pGeneData;
				}
			}
			else
			{
				delete pGeneData;
			}
		}
	}

	return bRes;
}

bool CEntityGeneComponent::AddGene(GeneInfo *pGeneInfo, int nTargetIndex)
{
    if (pGeneInfo != NULL && pGeneInfo->m_nGeneID > 0)
    {
        return AddGene(pGeneInfo->m_nGeneID, pGeneInfo->m_nParam1, pGeneInfo->m_nParam2, 
            pGeneInfo->m_strParam.c_str(), nTargetIndex);
    }

    return false;
}

void CEntityGeneComponent::DelGene(unsigned short nGeneID)
{
	GeneDataMap::iterator it = m_AllGene.find( nGeneID );

	if ( it != m_AllGene.end() )
	{
		GeneData* pGeneData = it->second;

		if ( pGeneData == NULL || pGeneData->nType != GeneType_OneTime )
		{
			_NotifyDelGene( nGeneID );

			if ( pGeneData != NULL )
			{
				_DelGeneAction( *pGeneData );

				if ( _IsDurationGeneByType(pGeneData->nType) || pGeneData->nType == GeneType_Matchs )
				{
					_HandleGeneDB( QUERY_DeleteRoleGene, *pGeneData );
				}
			}

			m_AllGene.erase( it );
			delete pGeneData;
		}
	}
}

void CEntityGeneComponent::ResetGene(unsigned short nGeneID, int nParam2)
{
	GeneDataMap::iterator it = m_AllGene.find( nGeneID );

	if ( it != m_AllGene.end() )
	{
		GeneData* pGeneData = it->second;

		if ( pGeneData == NULL || pGeneData->nType != GeneType_OneTime )
		{
			if ( pGeneData != NULL )
			{
				pGeneData->nParam2 = nParam2;

				if ( _IsDurationGeneByType(pGeneData->nType) || pGeneData->nType == GeneType_Matchs )
				{
					_HandleGeneDB( QUERY_UpdateRoleGene, *pGeneData );
				}
			}

			_NotifyResetGene( nGeneID, nParam2 );
		}
	}
}

void CEntityGeneComponent::_RefreshEquipGene(const std::map<unsigned short, GeneData> & mapGeneData)
{
    std::list<GeneInfo*> listGeneInfo;
    for (std::map<unsigned short, GeneData>::const_iterator it = mapGeneData.begin(); it != mapGeneData.end(); ++it)
    {
        GeneInfo * pGeneInfo = new GeneInfo();
        pGeneInfo->m_nGeneID = it->first;
        pGeneInfo->m_nParam1 = it->second.nParam1;
        pGeneInfo->m_nParam2 = it->second.nParam2;
        pGeneInfo->m_strParam = it->second.szCmdParam;
        listGeneInfo.push_back(pGeneInfo);
    }

    RefreshEquipGene(listGeneInfo, false);

    for (std::list<GeneInfo*>::iterator it = listGeneInfo.begin(); it != listGeneInfo.end(); ++it)
    {
        delete (*it); (*it) = NULL;
    }
    listGeneInfo.clear();

}

void CEntityGeneComponent::RefreshEquipGene(std::list<GeneInfo*>& rNewGeneList, int nTargetIndex)
{
	std::map<unsigned short, GeneInfo*> newGeneMap;

	for ( std::list<GeneInfo*>::iterator it_New = rNewGeneList.begin(); it_New != rNewGeneList.end(); ++it_New )
	{
		GeneInfo* pNewGene = *it_New;

		if ( pNewGene != NULL )
		{
			if ( _GetGeneType( pNewGene->m_nGeneID ) == GeneType_Equip ||  _GetGeneType( pNewGene->m_nGeneID ) == GeneType_Couple )
			{
				std::map<unsigned short, GeneInfo*>::iterator it_Add = newGeneMap.find( pNewGene->m_nGeneID );
				if ( it_Add != newGeneMap.end() )
				{
					//check the priority
					if ( it_Add->second->m_nParam2 < pNewGene->m_nParam2)
					{
						it_Add->second = pNewGene;
					}
				}
				else
				{
					newGeneMap.insert( std::make_pair( pNewGene->m_nGeneID, pNewGene ) );
				}
			}
		}
	}

	std::map<unsigned short, GeneData*> oriGeneMap;

	for ( GeneDataMap::iterator it_Ori = m_AllGene.begin(); it_Ori != m_AllGene.end(); ++it_Ori )
	{
		if ( it_Ori->second != NULL && (it_Ori->second->nType == GeneType_Equip || it_Ori->second->nType == GeneType_Couple ))
		{
			oriGeneMap.insert( std::make_pair( it_Ori->first, it_Ori->second ) );
		}
	}

	for ( GeneDataMap::iterator it_Ori = oriGeneMap.begin(); it_Ori != oriGeneMap.end(); ++it_Ori )
	{
		std::map<unsigned short, GeneInfo*>::iterator it_Tar = newGeneMap.find( it_Ori->second->nGeneId );
		if ( it_Tar != newGeneMap.end() )
		{
			if ( it_Ori->second->nParam1 != it_Tar->second->m_nParam1 )
			{
				DelGene( it_Tar->second->m_nGeneID );
				AddGene( it_Tar->second->m_nGeneID, it_Tar->second->m_nParam1, it_Tar->second->m_nParam2, it_Tar->second->m_strParam.c_str(), nTargetIndex );
			}
			else
			{
				if ( it_Ori->second->nParam2 != it_Tar->second->m_nParam2 )
				{
					ResetGene( it_Tar->second->m_nGeneID, it_Tar->second->m_nParam2 );
				}
			}

			newGeneMap.erase( it_Tar );
		}
		else
		{
			DelGene( it_Ori->second->nGeneId );
		}
	}

    
    for (std::map<unsigned short, GeneInfo*>::iterator it_Add = newGeneMap.begin(); it_Add != newGeneMap.end(); ++it_Add)
	{
		AddGene( it_Add->second->m_nGeneID, it_Add->second->m_nParam1, it_Add->second->m_nParam2, it_Add->second->m_strParam.c_str(), nTargetIndex );
    }
}

void CEntityGeneComponent::PackGeneForOther(CParamPool& IOBuff)
{
	int nOldPos = IOBuff.GetCurPosition();
	IOBuff.AddUShort( 0 );

    _PackGeneBrief(IOBuff);

    if (IOBuff.GetCurPosition() - nOldPos - sizeof(unsigned short) != 0)
    {
        int nFinalPos = IOBuff.GetCurPosition();
        IOBuff.SetCurPosition(nOldPos);
        IOBuff.ReplaceShort((short)(nFinalPos - nOldPos - sizeof(unsigned short)));
        IOBuff.SetCurPosition(nFinalPos);
	}
}

void CEntityGeneComponent::PackGeneAll(RoleRoomImageData::GeneDataMap &geneMap)
{
    for (GeneDataMap::iterator it = m_AllGene.begin(); it != m_AllGene.end(); ++it)
    {
        if (it->second != NULL)
        {
            GeneData geneData = *(it->second);
            geneData.nParam2 = 0;
            memset(geneData.szCmdParam, 0x0, sizeof(geneData.szCmdParam));

            geneMap.insert(std::make_pair(geneData.nGeneId, geneData));
        }
    }

    return;
}


GENE_TYPE CEntityGeneComponent::_GetGeneType(unsigned short nGeneID)
{
	GENE_TYPE eGeneType = GeneType_None;

	if ( nGeneID > 0 )
	{
		CInitLua::GetInstance().DoString( "GetGeneType", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_NUMBER, 1, nGeneID, DOSTRING_PARAM_END );

		int nOutput = 0;
		bool bResult = CInitLua::GetInstance().GetRetNum( nOutput, 0 );

		if( bResult )
		{
			eGeneType = (GENE_TYPE)nOutput;
		}
	}

	return eGeneType;
}

bool CEntityGeneComponent::_IsDurationGeneById( unsigned short nGeneID )
{
	GENE_TYPE eType = _GetGeneType(nGeneID);
	return _IsDurationGeneByType(eType);
}

bool CEntityGeneComponent::_IsDurationGeneByType( int eType )
{
	return eType == GeneType_Duration || eType == GeneType_DurationByParam;
}

bool CEntityGeneComponent::_AddGeneAction(GeneData& rGeneData, int nTargetIndex)
{
	bool bResult = false;

	if ( rGeneData.nGeneId > 0 )
	{
		if ( nTargetIndex == 0 )
		{
			if ( m_pNetCOM != NULL )
			{
				nTargetIndex = m_pNetCOM->GetPlayerLogicIndex();
			}
		}

		if ( nTargetIndex > 0 )
		{
			CInitLua::GetInstance().DoString( "AddGene",
				enumSCRIPT_RETURN_NUMBER, 2,
				enumSCRIPT_PARAM_NUMBER, 1, nTargetIndex,
				enumSCRIPT_PARAM_NUMBER_UNSIGNED, 1, rGeneData.nGeneId,
				enumSCRIPT_PARAM_NUMBER, 2, rGeneData.nParam1, rGeneData.nParam2,
				enumSCRIPT_PARAM_STRING, 1, rGeneData.szCmdParam,
				DOSTRING_PARAM_END );

			int nOutput = 0;
			bResult = CInitLua::GetInstance().GetRetNum( nOutput, 0 );

			if( bResult )
			{
				bResult = ( nOutput > 0 ? true : false );

				if ( bResult )
				{
					nOutput = 0;
					CInitLua::GetInstance().GetRetNum( nOutput, 1 );

					rGeneData.nType = (unsigned char)nOutput;
				}
			}
		}
	}

	return bResult;
}

void CEntityGeneComponent::_DelGeneAction(const GeneData& rGeneData)
{
	if ( m_pNetCOM != NULL )
	{
		CInitLua::GetInstance().DoString( "DeletGene",
			enumSCRIPT_RETURN_NONE, 0,
			enumSCRIPT_PARAM_NUMBER, 1, m_pNetCOM->GetPlayerLogicIndex(),
			enumSCRIPT_PARAM_NUMBER_UNSIGNED, 1, rGeneData.nGeneId,
			enumSCRIPT_PARAM_NUMBER, 2, rGeneData.nParam1, rGeneData.nParam2,
			enumSCRIPT_PARAM_STRING, 1, rGeneData.szCmdParam,
			DOSTRING_PARAM_END );
	}
}

void CEntityGeneComponent::_HandleGeneDB(QUERY_TYPE eQueryType, const GeneData& rGeneData)
{
	if ( m_pAttrCOM != NULL )
	{
		GeneData* pDBData = new GeneData();
		*pDBData = rGeneData;

		CDelHelper delHelper(pDBData);
		AddQuery( eQueryType, m_pAttrCOM->GetRoleID(), pDBData, 0, NULL, &delHelper );
	}
}

void CEntityGeneComponent::_PackGeneBrief(CParamPool& IOBuff)
{
	int nOldPos = IOBuff.GetCurPosition();
	IOBuff.AddUShort(0);

	int nGeneCount = 0;

	for ( GeneDataMap::iterator it = m_AllGene.begin(); it != m_AllGene.end(); ++it )
	{
		if( it->second != NULL )
		{
			GeneData geneData = *(it->second);
			geneData.nParam2 = 0;
			memset( geneData.szCmdParam, 0x0, sizeof(geneData.szCmdParam) );

			geneData.doEncode( IOBuff );
			++nGeneCount;
		}
	}

	int nFinalPos = IOBuff.GetCurPosition();
	IOBuff.SetCurPosition( nOldPos );
	IOBuff.ReplaceShort( (short)nGeneCount );
	IOBuff.SetCurPosition( nFinalPos );
}

void CEntityGeneComponent::_NotifyAddGene(GeneData & rGeneData)
{
	if ( m_pNetCOM != NULL && m_pAttrCOM != NULL )
	{
		GameMsg_S2C_AddGene msg;
		msg.m_nRoleID = m_pAttrCOM->GetRoleID();
		msg.m_GeneData = rGeneData;
		m_pNetCOM->SendPlayerMsg( &msg );

        if (m_pStageRoomCOM != NULL && NULL != m_pStageRoomCOM->Room())
		{
			m_pStageRoomCOM->Room()->SendMsgToAll( &msg, m_pStageRoomCOM );
		}
		if ( m_pCoupleCOM != NULL && m_pCoupleCOM->WeddingRoom() != NULL )
		{
			m_pCoupleCOM->WeddingRoom()->SendMsgToAllBut( &msg, m_pCoupleCOM );
		}
		if ( m_pPhotoRoomCOM != NULL && m_pPhotoRoomCOM->PhotoRoom() != NULL )
		{
			m_pPhotoRoomCOM->PhotoRoom()->SendMsgToAllBut( &msg, m_pPhotoRoomCOM );
		}
		if ( m_pAmuseRoomCOM != NULL && m_pAmuseRoomCOM->AmuseRoom() != NULL )
		{
			m_pAmuseRoomCOM->AmuseRoom()->SendMsgToAll( &msg, m_pAmuseRoomCOM );
		}
        if ( m_pRoleDanceGroup != NULL && m_pRoleDanceGroup->GetCeremonyRoom() != NULL )
        {
            m_pRoleDanceGroup->GetCeremonyRoom()->RoomBroadcast(msg);
        }

        // sync to group
        GameMsg_S2G_RefreshGeneNotify notify;
        notify.m_eAction = EGeneNotifyAction_Add;
        notify.m_GeneData = rGeneData;
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&notify);
	}
}

void CEntityGeneComponent::_NotifyDelGene(unsigned short nGeneID)
{
	if ( m_pNetCOM != NULL && m_pAttrCOM != NULL )
	{
		GameMsg_S2C_DelGene msg;
		msg.m_nRoleID = m_pAttrCOM->GetRoleID();
		msg.m_nGeneID = nGeneID;	
		m_pNetCOM->SendPlayerMsg( &msg );

		if ( m_pStageRoomCOM != NULL && m_pStageRoomCOM->Room() != NULL )
		{
			m_pStageRoomCOM->Room()->SendMsgToAll( &msg, m_pStageRoomCOM );
		}
		if ( m_pCoupleCOM != NULL && m_pCoupleCOM->WeddingRoom() != NULL )
		{
			m_pCoupleCOM->WeddingRoom()->SendMsgToAllBut( &msg, m_pCoupleCOM );
		}
		if ( m_pPhotoRoomCOM != NULL && m_pPhotoRoomCOM->PhotoRoom() != NULL )
		{
			m_pPhotoRoomCOM->PhotoRoom()->SendMsgToAllBut( &msg, m_pPhotoRoomCOM );
		}
		if ( m_pAmuseRoomCOM != NULL && m_pAmuseRoomCOM->AmuseRoom() != NULL )
		{
			m_pAmuseRoomCOM->AmuseRoom()->SendMsgToAll( &msg, m_pAmuseRoomCOM );
		}
        if ( m_pRoleDanceGroup != NULL && m_pRoleDanceGroup->GetCeremonyRoom() != NULL )
        {
            m_pRoleDanceGroup->GetCeremonyRoom()->RoomBroadcast(msg);
        }

        // sync to group
        GameMsg_S2G_RefreshGeneNotify notify;
        notify.m_eAction = EGeneNotifyAction_Del;
        notify.m_GeneData.nGeneId = nGeneID;
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&notify);
	}
}

void CEntityGeneComponent::_NotifyResetGene(unsigned short nGeneID, int nParam2)
{
	if ( m_pNetCOM != NULL && m_pAttrCOM != NULL )
	{
		GameMsg_S2C_ResetGene msg;
		msg.m_nRoleID = m_pAttrCOM->GetRoleID();
		msg.m_nGeneID = nGeneID;	
		msg.m_nParam2 = nParam2;
		m_pNetCOM->SendPlayerMsg( &msg );

		if ( m_pStageRoomCOM != NULL && m_pStageRoomCOM->Room() != NULL )
		{
			m_pStageRoomCOM->Room()->SendMsgToAll( &msg, m_pStageRoomCOM );
		}
		if ( m_pCoupleCOM != NULL && m_pCoupleCOM->WeddingRoom() != NULL )
		{
			m_pCoupleCOM->WeddingRoom()->SendMsgToAllBut( &msg, m_pCoupleCOM );
		}
		if ( m_pPhotoRoomCOM != NULL && m_pPhotoRoomCOM->PhotoRoom() != NULL )
		{
			m_pPhotoRoomCOM->PhotoRoom()->SendMsgToAllBut( &msg, m_pPhotoRoomCOM );
		}
		if ( m_pAmuseRoomCOM != NULL && m_pAmuseRoomCOM->AmuseRoom() != NULL )
		{
			m_pAmuseRoomCOM->AmuseRoom()->SendMsgToAll( &msg, m_pAmuseRoomCOM );
		}
        if ( m_pRoleDanceGroup != NULL && m_pRoleDanceGroup->GetCeremonyRoom() != NULL )
        {
            m_pRoleDanceGroup->GetCeremonyRoom()->RoomBroadcast(msg);
        }

        // sync to group
        GameMsg_S2G_RefreshGeneNotify notify;
        notify.m_eAction = EGeneNotifyAction_Reset;
        notify.m_GeneData.nGeneId = nGeneID;
        notify.m_GeneData.nParam2 = nParam2;
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&notify);

	}
}

void CEntityGeneComponent::PacketBuff( std::list<BuffData>& listBuff )
{
	int nNow = (int)time(NULL);
	//arrGeneId与arrBuffType元素需要一一对应
	int arrGeneId[] = {Gene_ID_ItemExpGain, Gene_ID_EquipExpGain, Gene_ID_Transform, Gene_ID_ChatColor, Gene_ID_RoomColor, Gene_ID_RoomTop};
	unsigned char arrBuffType[] = {EBuffType_Exp, EBuffType_Exp, EBuffType_Transform, EBuffType_ChatColor, EBuffType_RoomColor, EBuffType_RoomTop};
	size_t size = sizeof(arrGeneId)/sizeof(arrGeneId[0]);
	for (size_t i = 0; i < size; ++i)
	{
		GeneData* pGeneData = FindGene((unsigned short)arrGeneId[i]);
		if ( pGeneData != NULL )
		{
			bool bNeedAdd = true;
			BuffData buff;
			buff.m_nBuffType = arrBuffType[i];
			buff.m_nBuffSrcType = EBuffSrcType_Gene;
			buff.m_nGeneID = pGeneData->nGeneId;
			if ( pGeneData->nGeneId == Gene_ID_ItemExpGain || pGeneData->nGeneId == Gene_ID_EquipExpGain )
			{
				buff.m_nPercent = pGeneData->nParam1;
			}
			if ( _IsDurationGeneByType(pGeneData->nType) )
			{
				buff.m_nLeftTime = pGeneData->nParam2 - nNow;
				if ( buff.m_nLeftTime < 0 )
				{
					bNeedAdd = false;
				}
			}
			if ( bNeedAdd )
			{
				std::vector<int> vecKey;
				vecKey.push_back(pGeneData->nGeneId);
				vecKey.push_back(pGeneData->nParam1);
				vecKey.push_back(pGeneData->nParam2);
				buff.m_nItemType = (unsigned short)CSystemSettingMgr::Instance().GetBuffIconItem(vecKey);
				buff.m_bCanRemove = CSystemSettingMgr::Instance().CanRemoveBuff(pGeneData->nGeneId);
				if ( ConfigManager::Instance().GetItemConfigManager().GetGeneItemClass(pGeneData->nGeneId) == eItemClassType_Equip )
				{
					buff.m_nLeftTime = -1;//装备的基因buff剩余时间都显示无限
				}
				listBuff.push_back(buff);
			}
		}
	}
}

void CEntityGeneComponent::OnEvent( CComponentEvent& rEvent )
{
	if ( rEvent.nID == eComponentEvent_DelayTransfrom )
	{
		GeneData* pGeneData = FindGene(Gene_ID_Transform);
		if ( pGeneData != NULL )
		{
			int nNowTime = (int)time( NULL );
			GeneData geneData( *pGeneData );

			if (geneData.nType != GeneType_Equip && geneData.nType != GeneType_Couple ) 
			{
				if ( _IsDurationGeneByType(geneData.nType) )
				{
					geneData.nParam2 -= nNowTime;
				}
				_AddGeneAction( geneData, m_pNetCOM->GetPlayerLogicIndex() );
			}
		}
	}
}

void CEntityGeneComponent::MergeGene(GeneInfo & tempGene, int nCount)
{
    switch (tempGene.m_nGeneID)
    {
    case EGeneID_AddExpRate:
    case EGeneID_AddMoneyRate:
        tempGene.m_nParam2 = (tempGene.m_nParam2*nCount);
        break;
    case EGeneID_AddVipExp:
        tempGene.m_nParam1 = (tempGene.m_nParam1*nCount);
        tempGene.m_nParam2 = (tempGene.m_nParam2*nCount);
        break;
    case EGeneID_AddExp:
    case EGeneID_AddMoney:
    case EGeneID_AddSuper:
    case EGeneID_AddIntimacy:
    case EGeneID_AddDanceGroupContribution:
        tempGene.m_nParam1 = (tempGene.m_nParam1*nCount);
        break;
    case EGeneID_TransForm:
        if (tempGene.m_nParam1 == 1)
        {
            tempGene.m_nParam2 = (tempGene.m_nParam2*nCount);
        }
        break;
    }
}