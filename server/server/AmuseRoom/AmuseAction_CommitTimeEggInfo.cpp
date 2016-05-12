#include "AmuseAction_CommitTimeEggInfo.h"
#include "AmuseAction_UseDevice.h"
#include "AmuseAction_GetTimeEggGiftList.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "GameMsg_TimeEggMsg.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "AmuseRoomDataManager.h"
#include "../mall/EntityMallComponent.h"
#include "AmuseRoomDevice.h"
#include "../attribute/EntityAttributeComponent.h"

CAmuseAction_CommitTimeEggInfo::CAmuseAction_CommitTimeEggInfo(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID)
:CAmuseAction_Base(pAmusePlayer, nActionID, eAmuseAction_CommitTimeEggInfo, true, true)
{
}

CAmuseAction_CommitTimeEggInfo::~CAmuseAction_CommitTimeEggInfo(void)
{
}

bool CAmuseAction_CommitTimeEggInfo::FormParam(CAmuseActionParam_Base* pActionParam)
{
	if ( pActionParam != NULL && pActionParam->m_nActionType == eAmuseAction_CommitTimeEggInfo )
	{
		CAmuseActionParam_CommitTimeEggInfo* pUsePoseParam = (CAmuseActionParam_CommitTimeEggInfo*)pActionParam;
		m_strWords = pUsePoseParam->m_strWords;
		m_listGift = pUsePoseParam->m_listGift;

		if (m_strWords.size() > (size_t)g_ServerConfig.m_nMaxEggWordsLength)
			m_strWords.resize(g_ServerConfig.m_nMaxEggWordsLength);

		return true;
	}

	return false;
}

bool CAmuseAction_CommitTimeEggInfo::GoProcess(std::list<CAmuseAction_Base*>& rNewAction)
{
	CAmuseRoom *pRoom = m_pActPlayer->AmuseRoom();
	if (pRoom == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "Amuse room is null");
		return false;
	}

	bool bRes = true;
	CAmuseRoomDevice* pAmuseDevice = NULL;
	CAmuseAction_UseDevice* pUseDeviceAction = (CAmuseAction_UseDevice*)(m_pActPlayer->UniqueAmuseAction( eAmuseAction_UseDevice ));
	if (pUseDeviceAction)
	{
		pAmuseDevice = m_pActPlayer->AmuseRoom()->GetRoomDevice( pUseDeviceAction->m_nDeviceID );
	}
	CAmuseAction_GetTimeEggGiftList* pGetGiftAction = (CAmuseAction_GetTimeEggGiftList*)(m_pActPlayer->UniqueAmuseAction( eAmuseAction_GetTimeEggGiftList ));
	CAmuseAction_CommitTimeEggInfo* pCommitAction = (CAmuseAction_CommitTimeEggInfo*)(m_pActPlayer->UniqueAmuseAction( eAmuseAction_CommitTimeEggInfo ));
	if ( pUseDeviceAction == NULL || pAmuseDevice == NULL ||
		pAmuseDevice->GetDeviceType() != AmuseRoomDeviceType_TimeEgg )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_NotInRightDevice";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
		bRes = false;
	}
	if ( bRes && !m_listGift.empty() && pGetGiftAction == NULL )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_GiftNotInGiftList";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
		bRes = false;
	}
	if (bRes && m_pActPlayer->GetTimeEggInCurScene())
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_AlreadyExistEgg";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
		bRes = false;
	}
	if (bRes && pCommitAction)
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_AlreadyCommitEggInfo";
		m_pActPlayer->SendPlayerMsg( &msgResponse );
		bRes = false;
	}
	CAmuseRoomComponent* pPartner = NULL;
	if ( bRes )
	{
		pPartner = pAmuseDevice->GetOtherPlayer(m_pActPlayer->AttrCOM()->GetRoleID());
		if ( pPartner == NULL )
		{
			GameMsg_S2C_DoAmuseActionFail msgResponse;
			msgResponse.m_strError = "DoAmuseAction_NoPartner";
			m_pActPlayer->SendPlayerMsg( &msgResponse );
			bRes = false;
		}
	}
	TimeEggSetting& setting = CAmuseRoomDataManager::Instance().GetTimeEggSetting();
	if ( bRes )
	{
		for (VecPeddleGoods::iterator it = m_listGift.begin(); it != m_listGift.end(); ++it)
		{
			if ( pGetGiftAction->m_setGiftAndDesireItemId.find(it->m_nType) == 
				pGetGiftAction->m_setGiftAndDesireItemId.end() )
			{
				GameMsg_S2C_DoAmuseActionFail msgResponse;
				msgResponse.m_strError = "DoAmuseAction_GiftNotInGiftList";
				m_pActPlayer->SendPlayerMsg( &msgResponse );
				bRes = false;
			}
		}
	}

	if (bRes)
	{
        VecPeddleGoods::iterator itr = m_listGift.begin();
        for (; itr != m_listGift.end(); ++itr)
        {
            ItemConfig * pGiftItem = ConfigManager::Instance().GetItemConfigManager().GetByID(itr->m_nType);
            if (pGiftItem != NULL && pGiftItem->IsBadge_Type_WeddingRing())
            {
                GameMsg_S2C_DoAmuseActionFail msgResponse;
                msgResponse.m_strError = "DoAmuseAction_WeddingRingNotInEgg";
                m_pActPlayer->SendPlayerMsg( &msgResponse );

                bRes = false;
                break;
            }
        }
    }

    if (bRes)
    {
		CEntityMallComponent* pMall = (CEntityMallComponent*)m_pActPlayer->GetComponent<CRoleMall>();
		if (pMall == NULL)
		{
			GameMsg_S2C_DoAmuseActionFail msgResponse;
			msgResponse.m_strError = "DoAmuseAction_Unknown";
			m_pActPlayer->SendPlayerMsg( &msgResponse );
			bRes = false;
		}
		else
		{
			VecPeddleGoods vecGoods = m_listGift;
			MapPeddleGoods mapGoods;
			CurrencyMap mapBills;
			int nBuyError = MALL_OK;
			unsigned char nDestPlayerSexType = pPartner->AttrCOM()->GetSex();
			if (!vecGoods.empty() && (nBuyError = pMall->CanBuy(vecGoods, mapGoods, mapBills, false, true, setting.m_nGiftDiscount, nDestPlayerSexType))!=MALL_OK)
			{
				GameMsg_S2C_DoAmuseActionFail msgResponse;
				CEntityMallComponent::GetMallBuyErrorString(nBuyError, msgResponse.m_strError);
				m_pActPlayer->SendPlayerMsg( &msgResponse );
				bRes = false;
			}
		}
	}
	if (bRes)
	{
		CAmuseActionParam_CommitTimeEggInfo* pParam = new CAmuseActionParam_CommitTimeEggInfo();
		pParam->m_nPlayerID = m_pActPlayer->AttrCOM()->GetRoleID();

		GameMsg_S2C_DoAmuseActionSuc msgResponse;
		msgResponse.m_pActionParam = pParam;
		pAmuseDevice->SendMsgToAll( &msgResponse );

		m_bSuccess = true;
		return true;
	}
	else
	{
		m_pActPlayer->AmuseActionOver(this);
	}
	return false;
}

//file end

