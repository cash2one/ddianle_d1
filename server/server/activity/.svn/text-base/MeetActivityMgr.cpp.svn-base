#include "MeetActivityMgr.h"
#include "SystemActivityManager.h"
#include "GameMsg_Activity.h"
#include "../item/ItemProcess.h"
#include "../mail/MailMgr.h"
#include "../../datastructure/Localization.h"
#include "../AmuseRoom/AmuseRoomManager.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "EntityActivityComponent.h"

CMeetActivityMgr::CMeetActivityMgr()
:m_pMeetActivity(NULL),
m_nPhotoCount(0),
m_nWaitTime(0),
m_nAmuseSceneID(0),
m_strMailTitle(""),
m_strMailContent("")
{
}

CMeetActivityMgr::~CMeetActivityMgr()
{
}

CMeetActivityMgr & CMeetActivityMgr::Instance()
{
	static CMeetActivityMgr s_instance;
	return s_instance;
}

void CMeetActivityMgr::Init()
{
	m_pMeetActivity = new CMeetActivity( m_nPhotoCount, m_nWaitTime );
	CSystemActivityManager::Instance().AddTimerActivity( m_pMeetActivity );
}

void CMeetActivityMgr::AddRewardInfo( unsigned int nIndex, CMeetConfig & config )
{
	m_MeetConfig[nIndex] = config;
}

void CMeetActivityMgr::AddConfig( unsigned int nPhotoIndexCount, unsigned int nWaitTime, int nAmuseSceneID, const std::string & strTitle, const std::string & strContent )
{
    m_nPhotoCount    = nPhotoIndexCount;
	m_nWaitTime      = nWaitTime;
	m_nAmuseSceneID  = nAmuseSceneID;
	m_strMailTitle   = strTitle;
	m_strMailContent = strContent;
}

void CMeetActivityMgr::Logout( unsigned int nRoleID, unsigned int nSex )
{
	if ( m_pMeetActivity->IsOppositeWaitReward( nRoleID) )
	{
		SendRewardOutReward( nRoleID, nSex );
	}

	if ( m_pMeetActivity->IsCanDestoryAmuseScene( nRoleID) )
	{
		CAmuseRoomManager::Instance().DestroyMeetAmuseRoom( m_pMeetActivity->GetCoupleAmuseSceneID( nRoleID ));
	}

	WriteLog(LOGLEVEL_DEBUG, "Debug: MeetActivity.leave_1_logout roleid:%d", nRoleID );
	m_pMeetActivity->RemovePlayer( nRoleID );
}

void CMeetActivityMgr::OnMeetMatch( unsigned int nRoleID, unsigned int nSex, unsigned int nCoupleID )
{
	unsigned int nErr = m_pMeetActivity->Match( nRoleID, nSex, nCoupleID );
	if ( nErr != EMeetErr_Success )
	{
         if ( nErr == EMeetErr_InWaitList )
         {
			 return;
         }
 
		 SendMatchFailed( nErr, nRoleID );

		 // log
		 CRoleEntity *pRoleEntity   = CPlayerManager::Instance().GetEntityByRoleID( nRoleID );
		 if ( pRoleEntity != NULL )
		 {
			 CEntityActivityComponent * pActivityComponent = pRoleEntity->GetComponent<CRoleActivity>();
			 if ( pActivityComponent != NULL )
			 {
				 pActivityComponent->LogMeetAction( CMeetActivityLog::EMeetAction_Fail );
			 }
		 }
	}
	else
	{
		CRoleEntity *pRoleEntity   = CPlayerManager::Instance().GetEntityByRoleID( nRoleID );
		if ( pRoleEntity != NULL )
		{
			CAmuseRoomComponent *pMatchRoleAmuse = pRoleEntity->GetComponent<CRoleAmuseRoom>();
			if ( pMatchRoleAmuse != NULL  )
			{
				int nRoomID = 0;
				if ( CAmuseRoomManager::Instance().CreateMeetAmuseRoom( m_nAmuseSceneID, nRoomID, *pMatchRoleAmuse ).empty() )
				{
					unsigned int nCoupleRoleID = m_pMeetActivity->GetOppositeID( nRoleID );
					m_pMeetActivity->SetCoupleAmuseSceneID( nRoleID, nRoomID );
					SendMatchSucceed( nRoleID, nCoupleRoleID );

					// log
					CEntityActivityComponent * pActivityComponent = pRoleEntity->GetComponent<CRoleActivity>();
					if ( pActivityComponent != NULL )
					{
						pActivityComponent->LogMeetAction( CMeetActivityLog::EMeetAction_Sucess, nCoupleRoleID );
					}
				}
				else
				{
					SendMatchFailed( EMeetErr_CreateSceneErr, nRoleID );
				}
			}
		}		
	}
}

void CMeetActivityMgr::OnMeetCancel( unsigned int nRoleID, unsigned int nSex )
{
	WriteLog(LOGLEVEL_DEBUG, "Debug: MeetActivity.leave_1_MeetCancel roleid:%d", nRoleID );
	m_pMeetActivity->CancelMatch( nRoleID, nSex );
}

void CMeetActivityMgr::OnMeetLeaveScene( unsigned int nRoleID , unsigned int nSex )
{
	 WriteLog(LOGLEVEL_DEBUG, "Debug: MeetActivity.leave_1_OnMeetLeaveScene roleid:%d", nRoleID );
	 if ( m_pMeetActivity->IsOppositeWaitReward( nRoleID) )
	 {
		 SendRewardOutReward( nRoleID, nSex );
	 }

	 if ( m_pMeetActivity->IsCanDestoryAmuseScene( nRoleID) )
	 {
		 CAmuseRoomManager::Instance().DestroyMeetAmuseRoom( m_pMeetActivity->GetCoupleAmuseSceneID( nRoleID ));
	 }


	 m_pMeetActivity->RemovePlayer( nRoleID );
}

int CMeetActivityMgr::OnMeetCameraEvent( unsigned int nRoleID, unsigned int nIndex )
{
	CRoleEntity *pRoleEntity   = CPlayerManager::Instance().GetEntityByRoleID( nRoleID );
	if ( pRoleEntity == NULL )
	{
		return EMeetErr_NotFindPlayer;
	}

	const std::list<CItem> * pItems = GetCostItemList( nIndex );
	if ( pItems == NULL)
	{
        return EMeetErr_PhotoConfigErr;
	}

	std::list<CItem>::const_iterator it = pItems->begin();
	for ( ; it != pItems->end(); ++it )
	{
		if ( pRoleEntity->GetRoleItem()->GetItemCount( it->m_nItemType,false, false) < it->m_nItemCount )
		{
			return EMeetErr_PhotoNotEnoughItem;
		}
	}

	int nErr = m_pMeetActivity->PhotoEvent( nRoleID, nIndex );
	if ( nErr !=  EMeetErr_Success )
	{
		return nErr;
	}

	it = pItems->begin();
	for ( ; it != pItems->end(); ++it )
	{
		pRoleEntity->GetRoleItem()->RemoveItemByType( it->m_nItemType, it->m_nItemCount, EItemAction_Del_Meet );
	}

	return EMeetErr_Success;
}

void CMeetActivityMgr::GetCostItemList( unsigned int nIndex, std::list<CItem> & items )
{
	std::map<unsigned int,CMeetConfig>::iterator it = m_MeetConfig.find( nIndex );
	if ( it != m_MeetConfig.end() )
	{
		items.insert( items.end(), it->second.m_listCostItem.begin(), it->second.m_listCostItem.end() );
	}
}

const std::list<CItem> * CMeetActivityMgr::GetCostItemList( unsigned int nIndex )
{
	std::map<unsigned int,CMeetConfig>::iterator it = m_MeetConfig.find( nIndex );
	if ( it != m_MeetConfig.end() )
	{
		return &(it->second.m_listCostItem);
	}

	return NULL;
}

void CMeetActivityMgr::OnMeetEnterAmuseScene( unsigned int nRoleID )
{
    CRoleEntity *pRoleEntity   = CPlayerManager::Instance().GetEntityByRoleID( nRoleID );
	if ( pRoleEntity != NULL )
	{
		CAmuseRoomComponent *pMatchRoleAmuse = pRoleEntity->GetComponent<CRoleAmuseRoom>();
        int nAmuseID = m_pMeetActivity->GetCoupleAmuseSceneID( nRoleID );
		CAmuseRoomManager::Instance().EnterMeetAmuse( nAmuseID, *pMatchRoleAmuse );

		// log
		CEntityActivityComponent * pActivityComponent = pRoleEntity->GetComponent<CRoleActivity>();
		if ( pActivityComponent != NULL )
		{
			pActivityComponent->LogMeetAction( CMeetActivityLog::EMeetAction_InAmuse, 0, nAmuseID );
		}
	}
}

void CMeetActivityMgr::LeaveAmuseScene( unsigned int nRoleID )
{
	if ( m_pMeetActivity->IsCanDestoryAmuseScene( nRoleID) )
	{
		CAmuseRoomManager::Instance().DestroyMeetAmuseRoom( m_pMeetActivity->GetCoupleAmuseSceneID( nRoleID ));
	}

	WriteLog(LOGLEVEL_DEBUG, "Debug: MeetActivity.leave_1_LeaveAmuseScene roleid:%d", nRoleID );
	m_pMeetActivity->RemovePlayer( nRoleID );
}

itemtype_t CMeetActivityMgr::GetCostItemID()
{
	std::map<unsigned int,CMeetConfig>::iterator it = m_MeetConfig.begin();
	if ( it != m_MeetConfig.end() )
	{
		std::list<CItem>::iterator itCost = it->second.m_listCostItem.begin();
		if ( itCost != it->second.m_listCostItem.end() )
		{
			return itCost->m_nItemType;
		}
	}

	return 0;
}

void CMeetActivityMgr::OnMeetEnd( unsigned int nRoleID, unsigned int nSex )
{
	GameMsg_S2C_MeetEndResult  s2cEnd;
	unsigned int nSameIndexCount = 0;
	m_pMeetActivity->ActivityEnd( nRoleID, s2cEnd.m_photosInfo, nSameIndexCount );

	std::vector<MeetPhotoInfo>::iterator itName = s2cEnd.m_photosInfo.begin();
	for ( ; itName != s2cEnd.m_photosInfo.end(); ++itName )
	{
          std::map<unsigned int,CMeetConfig>::iterator itConfig = m_MeetConfig.find( itName->nIndex );
		  if ( itConfig != m_MeetConfig.end() )
		  {
			  itName->strName = itConfig->second.m_strIndexName;
		  }
	}

	std::map<unsigned int,CMeetConfig>::iterator it = m_MeetConfig.find( nSameIndexCount );
	if ( it != m_MeetConfig.end() )
	{
		if ( nSex == ESexType_Male )
		{
			s2cEnd.m_listItems = it->second.m_listMaleReward;
		}
		else
		{
			s2cEnd.m_listItems = it->second.m_listFemaleReward;
		}
	}

	int nIsMail = 0;
	unsigned int nOppositeID = 0;
	if ( m_pMeetActivity->IsBothCanReward( nRoleID, nOppositeID, nIsMail ))
	{
		CRoleEntity *pRoleEntity   = CPlayerManager::Instance().GetEntityByRoleID( nRoleID );
		if ( pRoleEntity != NULL )
		{
			CItemProcess::AddItems( *pRoleEntity, s2cEnd.m_listItems, EItemAction_Add_Meet, 0, true);
			pRoleEntity->SendPlayerMsg( &s2cEnd );
			
			// log
			CEntityActivityComponent * pActivityComponent = pRoleEntity->GetComponent<CRoleActivity>();
			if ( pActivityComponent != NULL )
			{
				pActivityComponent->LogMeetAction( CMeetActivityLog::EMeetAction_Reward );
			}

		}

		if ( !nIsMail )
		{
			CRoleEntity *pEntity   = CPlayerManager::Instance().GetEntityByRoleID( nOppositeID );
			if ( pEntity != NULL )
			{
				CItemProcess::AddItems( *pEntity, s2cEnd.m_listItems, EItemAction_Add_Meet, 0, true);
				pEntity->SendPlayerMsg( &s2cEnd );

				// log
				CEntityActivityComponent * pActivityComponent = pEntity->GetComponent<CRoleActivity>();
				if ( pActivityComponent != NULL )
				{
					pActivityComponent->LogMeetAction( CMeetActivityLog::EMeetAction_Reward );
				}
			}
		}
		else
		{
			// 发邮件
            CMailMgr::Instance().SendSystemMailToPlayer( nOppositeID, EMailType_Meet, m_strMailTitle, m_strMailContent, s2cEnd.m_listItems );
		}
	}	
}

void CMeetActivityMgr::SendMatchFailed( unsigned int nErr, unsigned int nRoleID )
{
	CRoleEntity *pTarget = CPlayerManager::Instance().GetEntityByRoleID( nRoleID );
	if ( pTarget != NULL )
	{
		std::string strErr = "";
		switch ( nErr )
		{
		case  EMeetErr_Waiting :
			{
				strErr = "EMeetErr_Waiting";
			}
			break;
		case  EMeetErr_TimeOut :
			{
				strErr = "EMeetErr_TimeOut";
			}
			break;
		case  EMeetErr_HaveMatch :
			{
				strErr = "EMeetErr_HaveMatch";
			}
			break;
		case EMeetErr_CreateSceneErr:
			{
				strErr = "EMeetErr_CreateSceneErr";
			}
			break;
		default:
			break;
		}

		GameMsg_S2C_MeetMatchFailed s2cMsg;
		s2cMsg.m_strError = strErr;
		pTarget->SendPlayerMsg( &s2cMsg );
	}
}

void CMeetActivityMgr::SendRewardOutReward( unsigned int nRoleID, unsigned int nSex )
{
	GameMsg_S2C_MeetEndResult  s2cEnd;
	unsigned int nSameIndexCount = 0;
	m_pMeetActivity->ActivityEnd( nRoleID, s2cEnd.m_photosInfo, nSameIndexCount );

	std::vector<MeetPhotoInfo>::iterator itName = s2cEnd.m_photosInfo.begin();
	for ( ; itName != s2cEnd.m_photosInfo.end(); ++itName )
	{
		std::map<unsigned int,CMeetConfig>::iterator itConfig = m_MeetConfig.find( itName->nIndex );
		if ( itConfig != m_MeetConfig.end() )
		{
			itName->strName = itConfig->second.m_strIndexName;
		}
	}

	std::map<unsigned int,CMeetConfig>::iterator it = m_MeetConfig.find( nSameIndexCount );
	if ( it != m_MeetConfig.end() )
	{
		if ( nSex == ESexType_Male )
		{
			s2cEnd.m_listItems = it->second.m_listMaleReward;
		}
		else
		{
			s2cEnd.m_listItems = it->second.m_listFemaleReward;
		}
	}


	// 发邮件
    CMailMgr::Instance().SendSystemMailToPlayer( nRoleID, EMailType_Meet, m_strMailTitle, m_strMailContent, s2cEnd.m_listItems );


	CRoleEntity *pRoleEntity   = CPlayerManager::Instance().GetEntityByRoleID( m_pMeetActivity->GetOppositeID(nRoleID) );
	if ( pRoleEntity != NULL )
	{
		CItemProcess::AddItems( *pRoleEntity, s2cEnd.m_listItems, EItemAction_Add_Meet, 0, true);
		pRoleEntity->SendPlayerMsg( &s2cEnd );

		// log
		CEntityActivityComponent * pActivityComponent = pRoleEntity->GetComponent<CRoleActivity>();
		if ( pActivityComponent != NULL )
		{
			pActivityComponent->LogMeetAction( CMeetActivityLog::EMeetAction_Reward );
		}
	}
}

void CMeetActivityMgr::SendMatchSucceed( unsigned int nSelfRoleID, unsigned int nOppositeID )
{
	CRoleEntity *pSelf   = CPlayerManager::Instance().GetEntityByRoleID( nSelfRoleID );
	CRoleEntity *pCouple = CPlayerManager::Instance().GetEntityByRoleID( nOppositeID );
	if ( pSelf != NULL && pCouple != NULL )
	{	
		PlayerShowMsg selfshow;

		selfshow.m_nRoleID           = pSelf->GetRoleID();
		selfshow.m_strRoleName       = pSelf->GetRoleName();
		selfshow.m_bIsHost           = false;
		selfshow.m_eRoleType         = 0;
		selfshow.m_eRoleSex          = pSelf->GetSex();
		selfshow.m_nRoleSkin         = pSelf->GetColor();
		selfshow.m_nIsVip            = pSelf->IsVIP();
		selfshow.m_nVipLv            = (unsigned short)pSelf->GetVIPLevel();
		selfshow.m_pRoleItem         = pSelf->GetRoleItem();
		selfshow.m_pRoleGene         = pSelf->GetRoleGene();
		pSelf->GetDanceGroupInfo( pSelf->GetRoleID(), selfshow.m_strDanceGroupName, selfshow.m_nGroupTitle, selfshow.m_nDanceGroupBadge, selfshow.m_nDanceGroupEffect );


		PlayerShowMsg coupleshow;
		coupleshow.m_nRoleID           = pCouple->GetRoleID();
		coupleshow.m_strRoleName       = pCouple->GetRoleName();
		coupleshow.m_bIsHost           = false;
		coupleshow.m_eRoleType         = 0;
		coupleshow.m_eRoleSex          = pCouple->GetSex();
		coupleshow.m_nRoleSkin         = pCouple->GetColor();
		coupleshow.m_nIsVip            = pCouple->IsVIP();
		coupleshow.m_nVipLv            = (unsigned short)pCouple->GetVIPLevel();
		coupleshow.m_pRoleItem         = pCouple->GetRoleItem();
		coupleshow.m_pRoleGene         = pCouple->GetRoleGene();
		pCouple->GetDanceGroupInfo( pCouple->GetRoleID(), coupleshow.m_strDanceGroupName, coupleshow.m_nGroupTitle, coupleshow.m_nDanceGroupBadge, coupleshow.m_nDanceGroupEffect );

		GameMsg_S2C_MeetMatchSucceed s2cSelfMsg;
		s2cSelfMsg.m_MatchPlayers.push_back( selfshow );
		s2cSelfMsg.m_MatchPlayers.push_back( coupleshow );
		pSelf->SendPlayerMsg( &s2cSelfMsg );

		GameMsg_S2C_MeetMatchSucceed s2cCoupleMsg;
		s2cCoupleMsg.m_MatchPlayers.push_back( coupleshow );
		s2cCoupleMsg.m_MatchPlayers.push_back( selfshow );
		pCouple->SendPlayerMsg( &s2cCoupleMsg );
	}
}
