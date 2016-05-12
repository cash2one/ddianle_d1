#include "AmuseAction_GetTimeEggGiftList.h"
#include "AmuseAction_UseDevice.h"
#include "AmuseRoomComponent.h"
#include "AmuseRoom.h"
#include "AmuseEnum.h"
#include "AmuseParam.h"
#include "GameMsg_S2C_AmuseMsg.h"
#include "GameMsg_TimeEggMsg.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "AmuseRoomDataManager.h"
#include "../mall/Mall.h"
#include "AmuseRoomDevice.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../mall/EntityMallComponent.h"

CAmuseAction_GetTimeEggGiftList::CAmuseAction_GetTimeEggGiftList(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID)
:CAmuseAction_Base(pAmusePlayer, nActionID, eAmuseAction_GetTimeEggGiftList, true, true)
{
}

CAmuseAction_GetTimeEggGiftList::~CAmuseAction_GetTimeEggGiftList(void)
{
}

bool CAmuseAction_GetTimeEggGiftList::FormParam(CAmuseActionParam_Base* pActionParam)
{
	if ( pActionParam != NULL && pActionParam->m_nActionType == eAmuseAction_GetTimeEggGiftList )
	{
		return true;
	}

	return false;
}

bool CAmuseAction_GetTimeEggGiftList::GoProcess(std::list<CAmuseAction_Base*>& rNewAction)
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
	if ( pUseDeviceAction == NULL || pAmuseDevice == NULL ||
		pAmuseDevice->GetDeviceType() != AmuseRoomDeviceType_TimeEgg )
	{
		GameMsg_S2C_DoAmuseActionFail msgResponse;
		msgResponse.m_strError = "DoAmuseAction_NotInRightDevice";
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
	if (bRes)
	{
		m_setGiftAndDesireItemId.clear();
		CAmuseActionParam_GetTimeEggGiftList* pParam = new CAmuseActionParam_GetTimeEggGiftList();
		//添加礼物道具
		TimeEggSetting& setting = CAmuseRoomDataManager::Instance().GetTimeEggSetting();
		pParam->m_nGiftDiscount = setting.m_nGiftDiscount;

		if (CSystemSettingMgr::Instance().IsOpenSendGift() )
		{
			for (std::list<CItem>::iterator it = setting.m_listGiftItem.begin();
				it != setting.m_listGiftItem.end(); ++it)
			{
				pParam->m_listGift.push_back(it->m_nItemType);
				m_setGiftAndDesireItemId.insert(it->m_nItemType);
			}

			//添加对方许愿道具
			CAmuseRoomComponent* pPartner = pAmuseDevice->GetOtherPlayer(m_pActPlayer->AttrCOM()->GetRoleID());
			if (pPartner)
			{
                CEntityMallComponent * pMallComponent = pPartner->RoleMall();
				if (pMallComponent)
				{
                    std::list<itemtype_t> listItemType;
                    pMallComponent->GetDesireList(listItemType);

                    std::list<itemtype_t>::iterator it = listItemType.begin();
                    for ( ; it != listItemType.end(); ++it)
                    {
                        if (CMall::Instance().CanBuyGoodByID(*it))
                        {
                            pParam->m_listDesire.push_back(*it);
                            m_setGiftAndDesireItemId.insert(*it);
                        }
                    }
				}
			}
		}

		GameMsg_S2C_DoAmuseActionSuc msgResponse;
		msgResponse.m_pActionParam = pParam;
		m_pActPlayer->SendPlayerMsg( &msgResponse );

		m_bSuccess = true;
		return true;
	}
	return false;
}

//file end

