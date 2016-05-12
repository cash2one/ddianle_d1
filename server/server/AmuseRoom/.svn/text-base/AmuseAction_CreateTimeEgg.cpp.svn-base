#include "AmuseAction_CreateTimeEgg.h"
#include "AmuseAction_UseDevice.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "GameMsg_TimeEggMsg.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "AmuseRoomDataManager.h"
#include "AmuseRoomDevice.h"
#include "../mall/EntityMallComponent.h"
#include "AmuseAction_CommitTimeEggInfo.h"
#include "../attribute/EntityAttributeComponent.h"

CAmuseAction_CreateTimeEgg::CAmuseAction_CreateTimeEgg(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID)
:CAmuseAction_Base(pAmusePlayer, nActionID, eAmuseAction_CreateTimeEgg, true, true)
{
}

CAmuseAction_CreateTimeEgg::~CAmuseAction_CreateTimeEgg(void)
{
}

bool CAmuseAction_CreateTimeEgg::FormParam(CAmuseActionParam_Base* pActionParam)
{
	if ( pActionParam != NULL && pActionParam->m_nActionType == eAmuseAction_CreateTimeEgg )
	{
		return true;
	}

	return false;
}

bool CAmuseAction_CreateTimeEgg::GoProcess(std::list<CAmuseAction_Base*>& rNewAction)
{
	CAmuseRoom *pRoom = m_pActPlayer->AmuseRoom();
	if (pRoom == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Amuse room is null");
		return false;
	}

	CAmuseRoomDevice* pAmuseDevice = NULL;
	CAmuseRoomComponent* pPartner = NULL;
	CAmuseAction_UseDevice* pUseDeviceAction = (CAmuseAction_UseDevice*)(m_pActPlayer->UniqueAmuseAction( eAmuseAction_UseDevice ));
	if (pUseDeviceAction)
	{
		pAmuseDevice = m_pActPlayer->AmuseRoom()->GetRoomDevice( pUseDeviceAction->m_nDeviceID );
		if ( pAmuseDevice != NULL )
		{
			pPartner = pAmuseDevice->GetOtherPlayer(m_pActPlayer->AttrCOM()->GetRoleID());
		}
	}
	CAmuseAction_CreateTimeEgg* pCreateAction = (CAmuseAction_CreateTimeEgg*)(m_pActPlayer->UniqueAmuseAction( eAmuseAction_CreateTimeEgg ));
	if ( pCreateAction )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_AlreadyCreateEgg";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
		return false;
	}
	else if ( pUseDeviceAction == NULL || pAmuseDevice == NULL ||
		pAmuseDevice->GetDeviceType() != AmuseRoomDeviceType_TimeEgg )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_NotInRightDevice";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
		return false;
	}
	else if ( pUseDeviceAction->m_bIsInvited )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_NotInviter";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
		return false;
	}
	else if (m_pActPlayer->GetTimeEggInCurScene())
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_AlreadyExistEgg";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
		return false;
	}
	else if ( pPartner == NULL )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_NoPartner";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
		return false;
	}
	list<CItem>& vecCostItem = CAmuseRoomDataManager::Instance().GetTimeEggSetting().m_listCostItem;
	if ( !vecCostItem.empty() )
	{
		CEntityItemComponent* pItemCOM = m_pActPlayer->ItemCOM();
		if ( pItemCOM )
		{
			for (list<CItem>::iterator it = vecCostItem.begin();
				it != vecCostItem.end(); ++it)
			{
				if ( it->m_nItemCount > pItemCOM->GetItemCount(it->m_nItemType, false, false) )
				{
					GameMsg_S2C_DoAmuseActionFail msgResponse;
					msgResponse.m_strError = "DoAmuseAction_NoEnoughItem";
					m_pActPlayer->SendPlayerMsg( &msgResponse );
					return false;
				}
			}
		}
	}

	TimeEggSetting& setting = CAmuseRoomDataManager::Instance().GetTimeEggSetting();
	CreateTimeEggEntry* pCreateTimeEggEntry = new CreateTimeEggEntry;
	
	m_mapBills.clear();
	bool bAllCommit = true;
	std::vector<CAmuseRoomComponent*> players;
	pAmuseDevice->GetPlayers(players);
	pCreateTimeEggEntry->m_roleinfo.resize(players.size());
	pCreateTimeEggEntry->m_egg.m_nSceneID = m_pActPlayer->AmuseRoom()->RoomScene();
	pCreateTimeEggEntry->m_egg.m_nRelation = pUseDeviceAction->m_nPartnerType;
	pCreateTimeEggEntry->m_egg.m_nEndTime = setting.m_nTimeEggOpenDuration*60;
	pCreateTimeEggEntry->m_egg.m_nDestroyTime = setting.m_nTimeEggDestroyDuration*60;
	size_t i = 0;
	for (std::vector<CAmuseRoomComponent*>::iterator it = players.begin(); 
		it != players.end(); ++it)
	{
		CAmuseRoomComponent* pAmuseCOM = *it;
		if (pAmuseCOM)
		{
			CAmuseAction_CommitTimeEggInfo* pCommitAction = (CAmuseAction_CommitTimeEggInfo*)pAmuseCOM->UniqueAmuseAction(eAmuseAction_CommitTimeEggInfo);
			if ( pCommitAction == NULL )
			{
				GameMsg_S2C_DoAmuseActionFail msgResponse;
				msgResponse.m_strError = "DoAmuseAction_NotAllCommitEggInfo";
				m_pActPlayer->SendPlayerMsg( &msgResponse );
				bAllCommit = false;
				break;
			}
			pPartner = pAmuseDevice->GetOtherPlayer(pAmuseCOM->AttrCOM()->GetRoleID());
			if ( pPartner == NULL )
			{
				GameMsg_S2C_DoAmuseActionFail msgResponse;
				msgResponse.m_strError = "DoAmuseAction_NoPartner";
				m_pActPlayer->SendPlayerMsg( &msgResponse );
				bAllCommit = false;
				break;
			}

            // wedding ring not in egg
            VecPeddleGoods::iterator itr = pCommitAction->m_listGift.begin();
            for (; itr != pCommitAction->m_listGift.end(); ++itr)
            {
                ItemConfig * pGiftItem = ConfigManager::Instance().GetItemConfigManager().GetByID(itr->m_nType);
                if (pGiftItem != NULL && pGiftItem->IsBadge_Type_WeddingRing())
                {
                    GameMsg_S2C_DoAmuseActionFail msgResponse;
                    msgResponse.m_strError = "DoAmuseAction_WeddingRingNotInEgg";
                    m_pActPlayer->SendPlayerMsg( &msgResponse );

                    bAllCommit = false;
                    break;
                }
            }

            if (!bAllCommit)
            {
                break;
            }

			CEntityMallComponent* pMallCOM = (CEntityMallComponent*)pAmuseCOM->GetComponent<CRoleMall>();
			if ( pMallCOM )
			{
				//MapPeddleBills mapBills;
                VecPeddleGoods vecGoods = pCommitAction->m_listGift;
                MapPeddleGoods mapGoods;
				unsigned char nDestPlayerSexType = pPartner->AttrCOM()->GetSex();
				if (!vecGoods.empty() && pMallCOM->CanBuy(vecGoods, mapGoods, m_mapBills[pMallCOM], false, true, setting.m_nGiftDiscount, nDestPlayerSexType)!=MALL_OK)
				{
					GameMsg_S2C_DoAmuseActionFail msgResponse;
					msgResponse.m_strError = "DoAmuseAction_CannotBuy";
					m_pActPlayer->SendPlayerMsg( &msgResponse );
					bAllCommit = false;
					break;
				}
			}

			pCreateTimeEggEntry->m_roleinfo[i].m_nSceneID = pCreateTimeEggEntry->m_egg.m_nSceneID;
			pCreateTimeEggEntry->m_roleinfo[i].m_nRoleID = pAmuseCOM->RoleID();
			if ( !pCommitAction->m_listGift.empty() )
			{
				CEntityMallComponent::GetCItemByPeddleGood(pCommitAction->m_listGift[0], pCreateTimeEggEntry->m_roleinfo[i].m_item);
			}
			pCreateTimeEggEntry->m_roleinfo[i].m_words = pCommitAction->GetWords();
			pCreateTimeEggEntry->m_roleinfo[i].m_nSex = pAmuseCOM->AttrCOM()->GetSex();
			++i;
		}
	}
	if (pCreateTimeEggEntry != NULL)
	{
		LOG_DEBUG << "Try to create time egg: " << std::endl;

		for (vector<RoleTimeEggEntry>::iterator it = pCreateTimeEggEntry->m_roleinfo.begin(); it != pCreateTimeEggEntry->m_roleinfo.end(); ++it)
		{
			LOG_DEBUG << "role id(" << it->m_nRoleID << ") " << std::endl;
		}
	}
	if (i == players.size() && bAllCommit)
	{
		DoCreateTimeEgg(pCreateTimeEggEntry);

		m_bSuccess = true;
		return true;
	}
	delete pCreateTimeEggEntry;
	pCreateTimeEggEntry = NULL;
	return false;
}

void CAmuseAction_CreateTimeEgg::DoCreateTimeEgg(CreateTimeEggEntry* pCreateTimeEggEntry)
{
	//数据库请求
	CDelHelper delHelper(pCreateTimeEggEntry);
	m_pActPlayer->AddQuery(Query_CreateTimeEgg, 0, pCreateTimeEggEntry, 0, NULL, &delHelper);
}

void CAmuseAction_CreateTimeEgg::OnCreateTimeEggReturn(int nError, const string& strError)
{
	if ( m_pActPlayer->AmuseRoom() )
	{
		CAmuseRoomDevice* pAmuseDevice = NULL;
		CAmuseAction_UseDevice* pUseDeviceAction = (CAmuseAction_UseDevice*)(m_pActPlayer->UniqueAmuseAction( eAmuseAction_UseDevice ));
		if (pUseDeviceAction)
		{
			pAmuseDevice = m_pActPlayer->AmuseRoom()->GetRoomDevice( pUseDeviceAction->m_nDeviceID );
		}
		if ( pAmuseDevice )
		{
			if ( nError == ECreateTimeEggError_OK )
			{
				//扣除消耗品
				CEntityItemComponent* pItemCOM = m_pActPlayer->ItemCOM();
				if ( pItemCOM )
				{
					list<CItem>& vecCostItem = CAmuseRoomDataManager::Instance().GetTimeEggSetting().m_listCostItem;
					for (list<CItem>::iterator it = vecCostItem.begin();
						it != vecCostItem.end(); ++it)
					{
						pItemCOM->RemoveItemByType( it->m_nItemType, it->m_nItemCount, EItemAction_Del_TimeEgg);
					}
				}
				//扣除各个玩家纪念品的钱
				for (std::map<CEntityMallComponent*, CurrencyMap>::iterator it = m_mapBills.begin();
					it != m_mapBills.end(); ++it)
				{
					it->first->FinishBill(it->second, EFinishBillFrom_Egg);
				}
				CAmuseActionParam_CreateTimeEgg* pParam = new CAmuseActionParam_CreateTimeEgg();

				GameMsg_S2C_DoAmuseActionSuc msgResponse;
				msgResponse.m_pActionParam = pParam;
				pAmuseDevice->SendMsgToAll( &msgResponse );
			}
			else
			{
				GameMsg_S2C_DoAmuseActionFail msgResponse;
				msgResponse.m_strError = strError;
				m_pActPlayer->SendPlayerMsg( &msgResponse );
			}
		}
	}
}

//file end

