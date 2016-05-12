#include "EntityMonthCardComponent.h"
#include "../share/ServerMsgDef.h"
#include "GameMsg_C2S_MonthCard.h"
#include "GameMsg_S2C_MonthCard.h"
#include "MonthCardDataMgr.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../../socket/GameMsg_Map.h"
#include "../item/ItemProcess.h"


CEntityMonthCardComponent::CEntityMonthCardComponent()
:CEntityComponent(true,true)
,m_pRoleAttr(NULL)
,m_pRoleNet(NULL)
{
}

CEntityMonthCardComponent::~CEntityMonthCardComponent()
{
}

void CEntityMonthCardComponent::Start()
{
	m_pRoleAttr = GetComponent<CRoleAttribute>();
	m_pRoleNet	= GetComponent<CRoleNet>();
	m_pRoleItem = GetComponent<CRoleItem>();
}

void CEntityMonthCardComponent::RegComponentNetMsgMap()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetMonthCardInfo);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_OpenMonthCard);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RenewMonthCard);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetMonthCardEveryDayReward);

	RegMsgProcessor(MSG_C2S_GetMonthCardInfo, &CEntityMonthCardComponent::OnGetMonthCardInfo);
	RegMsgProcessor(MSG_C2S_OpenMonthCard, &CEntityMonthCardComponent::OnOpenMonthCard);
	RegMsgProcessor(MSG_C2S_RenewMonthCard, &CEntityMonthCardComponent::OnRenewMonthCard);
	RegMsgProcessor(MSG_C2S_GetMonthCardReward, &CEntityMonthCardComponent::OnGetEveryDayReward);	
}

bool CEntityMonthCardComponent::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
	m_MonthCard = pRoleInforDB->m_mapMonthCard;
	return true;
}

bool CEntityMonthCardComponent::PacketToDB(ROLEINFO_DB*pRoleInforDB) const
{
	return true;
}

bool CEntityMonthCardComponent::PacketToCache( ROLEINFO_DB* pRoleInforDB ) const
{
    if ( pRoleInforDB == NULL )
        return false;

    pRoleInforDB->m_mapMonthCard = m_MonthCard;

    return true;
}

void CEntityMonthCardComponent::SerializeComponent(CParamPool &IOBuff)
{
}

void CEntityMonthCardComponent::OnLogin()	
{
}

void CEntityMonthCardComponent::OnLogout()
{
}

void CEntityMonthCardComponent::OnUpdate(const unsigned long & nTimeElapsed)
{
	
}

void CEntityMonthCardComponent::OnEvent(CComponentEvent& refEvent )
{
	                                                 
}

void CEntityMonthCardComponent::OnGetMonthCardInfo(GameMsg_Base &rMsg, CSlotPeer & slotPeer)
{
	GameMsg_S2C_GetMonthCardInfoResult resultMsg;

	const MonthCardInfo * pMonthCardConfig = CMonthCardDataMgr::Instance().GetMonthCardInfo(EMonthCardType_Month);
	if ( pMonthCardConfig != NULL)
	{
		resultMsg.m_nOpenDays = pMonthCardConfig->nOpenDays;
		resultMsg.m_nRenewDays = pMonthCardConfig->nRenewDays;
		resultMsg.m_listItem = m_pRoleAttr->GetSex() == ESexType_Male ? pMonthCardConfig->listEveryDayMaleItemReward : pMonthCardConfig->listEveryDayFemaleItemReward ;	
		resultMsg.m_nMoney = pMonthCardConfig->nEveryDayMoneyReward;
		resultMsg.m_nBindMBill = pMonthCardConfig->nEveryDayMBillReward;
		resultMsg.m_strDesc = pMonthCardConfig->strDesc;
		resultMsg.m_nPayBill = pMonthCardConfig->nPrice;
		resultMsg.m_nRewardBindBill = pMonthCardConfig->nOpenMBillReward;

		MonthCardDBTable::iterator it = m_MonthCard.find( EMonthCardType_Month );
		if ( it != m_MonthCard.end() )
		{
			resultMsg.m_nOpenTime = it->second.nOpenTime;
			resultMsg.m_bIsReward = !isCanReward( it->second.nOpenTime, it->second.nRewardTime );
		}
	}

	m_pRoleNet->SendPlayerMsg( &resultMsg );
}

void CEntityMonthCardComponent::OnOpenMonthCard(GameMsg_Base &rMsg, CSlotPeer & slotPeer)
{
	int nError = OpenMonthCard();
 	if ( nError != EMonthCardMsgError_Success )
	{
		GameMsg_S2C_OpenMonthCardFail failmsg;
		failmsg.m_nError = nError;
		m_pRoleNet->SendPlayerMsg( &failmsg );
	}
	else
	{
		GameMsg_S2C_OpenMonthCardSuc sucMsg;
		const MonthCardInfo * pMonthCardConfig = CMonthCardDataMgr::Instance().GetMonthCardInfo(EMonthCardType_Month);
		if ( pMonthCardConfig != NULL)
		{
			sucMsg.m_nOpenDays = pMonthCardConfig->nOpenDays;
			sucMsg.m_nRenewDays = pMonthCardConfig->nRenewDays;
			sucMsg.m_listItem = m_pRoleAttr->GetSex() == ESexType_Male ? pMonthCardConfig->listOpenMaleItemReward : pMonthCardConfig->listOpenFemaleItemReward ;
			sucMsg.m_nMoney = pMonthCardConfig->nOpenMoneyReward;
			sucMsg.m_nBindMBill = pMonthCardConfig->nOpenMBillReward;

			MonthCardDBTable::iterator it = m_MonthCard.find(EMonthCardType_Month );
			if ( it != m_MonthCard.end() )
			{
				sucMsg.m_nOpenTime = it->second.nOpenTime;
				sucMsg.m_bIsReward = !isCanReward( it->second.nOpenTime, it->second.nRewardTime );
			}
		}

		m_pRoleNet->SendPlayerMsg( &sucMsg );
	}

}

int CEntityMonthCardComponent::OpenMonthCard()
{
	const MonthCardInfo * pMonthCardConfig = CMonthCardDataMgr::Instance().GetMonthCardInfo(EMonthCardType_Month);
	if ( pMonthCardConfig == NULL)
	{
		return EMonthCardMsgError_ConfigNot;
	}

 	unsigned int nCurrentTime = (unsigned int)time(NULL);
	MonthCardDBTable::iterator it = m_MonthCard.find(EMonthCardType_Month );
	if ( it != m_MonthCard.end() )
	{
		if( it->second.nOpenTime + pMonthCardConfig->nOpenDays >= nCurrentTime )
		{
			return EMonthCardMsgError_HaveOpen;
		}
	}

	if ( m_pRoleAttr->GetPoint() < (unsigned int)pMonthCardConfig->nPrice )
	{
		return EMonthCardMsgError_MCoinNotEnough;
	}

	// ¿Û³ýM±Ò
	m_pRoleAttr->ChangeBill( (-1)*pMonthCardConfig->nPrice, EChangeBillCause_Del_MonthCard );

	// ½±Àø
	m_pRoleAttr->ChangeBindBill( pMonthCardConfig->nOpenMBillReward, EChangeBindBillCause_Add_OpenMonthCard );
	
	if ( m_pRoleAttr->GetSex() == ESexType_Male )
	{
		CItemProcess::AddItems( *(CRoleEntity *)Entity(), pMonthCardConfig->listOpenMaleItemReward, EItemAction_Add_OpenMonthCard, 0, true );
	}
	else
	{
		CItemProcess::AddItems( *(CRoleEntity *)Entity(), pMonthCardConfig->listOpenFemaleItemReward, EItemAction_Add_OpenMonthCard, 0, true );
	}
	
	m_pRoleAttr->ChangeMoney( pMonthCardConfig->nOpenMoneyReward, EChangeMoneyCause_Add_OpenMonthCard, 0 );

	// ¿ªÍ¨
	time_t now = nCurrentTime;
	tm * pTemp                  = localtime( &now );
	tm tmNow(*pTemp);
	unsigned int nOpenSec = nCurrentTime  - tmNow.tm_hour*3600 - tmNow.tm_min*60 - tmNow.tm_sec;
	if ( it != m_MonthCard.end() )
	{
		it->second.nOpenTime = nOpenSec;
		MonthCardDB * pSaveMonthCard = new MonthCardDB( it->second );
		AddQuery(Query_Update_MonthCardInfo, m_pRoleAttr->GetRoleID(), pSaveMonthCard );
	}
	else
	{
		MonthCardDB cardinfo;
		cardinfo.nCardType = (int)EMonthCardType_Month;
		cardinfo.nOpenTime = nOpenSec;
		m_MonthCard.insert( std::make_pair( cardinfo.nCardType, cardinfo ));

		MonthCardDB * pSaveMonthCard = new MonthCardDB( cardinfo );
		AddQuery(Query_Insert_MonthCardInfo, m_pRoleAttr->GetRoleID(), pSaveMonthCard );
	}

	return EMonthCardMsgError_Success;
};

void CEntityMonthCardComponent::OnRenewMonthCard(GameMsg_Base &rMsg, CSlotPeer & slotPeer)
{
	int nError = RenewMonthCard();
	if ( nError != EMonthCardMsgError_Success )
	{
		GameMsg_S2C_RenewMonthCardFail failmsg;
		failmsg.m_nError = nError;
		m_pRoleNet->SendPlayerMsg( &failmsg );
	}
	else
	{
		GameMsg_S2C_RenewMonthCardSuc sucMsg;
		const MonthCardInfo * pMonthCardConfig = CMonthCardDataMgr::Instance().GetMonthCardInfo(EMonthCardType_Month);
		if ( pMonthCardConfig != NULL)
		{
			sucMsg.m_nOpenDays = pMonthCardConfig->nOpenDays;
			sucMsg.m_nRenewDays = pMonthCardConfig->nRenewDays;
			sucMsg.m_listItem = m_pRoleAttr->GetSex() == ESexType_Male ? pMonthCardConfig->listOpenMaleItemReward : pMonthCardConfig->listOpenFemaleItemReward ;
			sucMsg.m_nMoney = pMonthCardConfig->nOpenMoneyReward;
			sucMsg.m_nBindMBill = pMonthCardConfig->nOpenMBillReward;

			MonthCardDBTable::iterator it = m_MonthCard.find(EMonthCardType_Month );
			if ( it != m_MonthCard.end() )
			{
				sucMsg.m_nOpenTime = it->second.nOpenTime;
				sucMsg.m_bIsReward = !isCanReward( it->second.nOpenTime, it->second.nRewardTime );
			}
		}

		m_pRoleNet->SendPlayerMsg( &sucMsg );
	}
}

int CEntityMonthCardComponent::RenewMonthCard()
{
	const MonthCardInfo * pMonthCardConfig = CMonthCardDataMgr::Instance().GetMonthCardInfo(EMonthCardType_Month);
	if ( pMonthCardConfig == NULL)
	{
		return EMonthCardMsgError_ConfigNot;
	}

	unsigned int nCurrentTime = (unsigned int)time(NULL);
	MonthCardDBTable::iterator it = m_MonthCard.find( EMonthCardType_Month );
	if ( it != m_MonthCard.end() )
	{
		if( it->second.nOpenTime + pMonthCardConfig->nOpenDays < nCurrentTime )
		{
			return EMonthCardMsgError_NotOpen;
		}
	}
	else
	{
		return EMonthCardMsgError_NotOpen;
	}

	if ( m_pRoleAttr->GetPoint() < (unsigned int)pMonthCardConfig->nPrice )
	{
		return EMonthCardMsgError_MCoinNotEnough;
	}

	// ¿Û³ýM±Ò
	m_pRoleAttr->ChangeBill( (-1)*pMonthCardConfig->nPrice, EChangeBillCause_Del_MonthCard );

	// ½±Àø
	m_pRoleAttr->ChangeBindBill( pMonthCardConfig->nOpenMBillReward, EChangeBindBillCause_Add_OpenMonthCard );

	if ( m_pRoleAttr->GetSex() == ESexType_Male )
	{
		CItemProcess::AddItems( *(CRoleEntity *)Entity(), pMonthCardConfig->listOpenMaleItemReward, EItemAction_Add_OpenMonthCard, 0, true );
	}
	else
	{
		CItemProcess::AddItems( *(CRoleEntity *)Entity(), pMonthCardConfig->listOpenFemaleItemReward, EItemAction_Add_OpenMonthCard, 0, true );
	}

	m_pRoleAttr->ChangeMoney( pMonthCardConfig->nOpenMoneyReward, EChangeMoneyCause_Add_OpenMonthCard, 0 );

	// ¿ªÍ¨
	if ( it != m_MonthCard.end() )
	{
		it->second.nOpenTime += pMonthCardConfig->nOpenDays;
		MonthCardDB * pSaveMonthCard = new MonthCardDB( it->second );
		AddQuery(Query_Update_MonthCardInfo, m_pRoleAttr->GetRoleID(), pSaveMonthCard );
	}

	return EMonthCardMsgError_Success;
}

void CEntityMonthCardComponent::OnGetEveryDayReward(GameMsg_Base &rMsg, CSlotPeer & slotPeer)
{
	int nError = GetEveryDayReward();
	if ( nError != EMonthCardMsgError_Success )
	{
		GameMsg_S2C_GetMonthCardEveryDayRewardFail failmsg;
		failmsg.m_nError = nError;
		m_pRoleNet->SendPlayerMsg( &failmsg );
	}
	else
	{
		GameMsg_S2C_GetMonthCardEveryDayRewardSuc sucmsg;
		sucmsg.m_bIsReward = true;
		m_pRoleNet->SendPlayerMsg( &sucmsg );
	}
}

bool CEntityMonthCardComponent::isCanReward(unsigned int nOpenTime, unsigned int nRewardTime)
{
	const MonthCardInfo * pMonthCardConfig = CMonthCardDataMgr::Instance().GetMonthCardInfo(EMonthCardType_Month);
	if ( pMonthCardConfig != NULL)
	{
		unsigned int nCurrentTime = (unsigned int)time(NULL);
		if( nOpenTime + pMonthCardConfig->nOpenDays >= nCurrentTime )
		{
			time_t now = nCurrentTime;
			tm * pTemp                  = localtime( &now );
			tm tmNow(*pTemp);

			time_t rewardtime_t	= nRewardTime;
			pTemp                       = localtime( &rewardtime_t);
			tm tmReward(*pTemp);

			if (  tmReward.tm_year != tmNow.tm_year || tmReward.tm_mon != tmNow.tm_mon || tmReward.tm_mday != tmNow.tm_mday )
			{
				return true;
			}
		}
	}
	
	return false;
}

int CEntityMonthCardComponent::GetEveryDayReward()
{
	const MonthCardInfo * pMonthCardConfig = CMonthCardDataMgr::Instance().GetMonthCardInfo(EMonthCardType_Month);
	if ( pMonthCardConfig == NULL)
	{
		return EMonthCardMsgError_ConfigNot;
	}

	unsigned int nCurrentTime = (unsigned int)time(NULL);
	MonthCardDBTable::iterator it = m_MonthCard.find(EMonthCardType_Month );
	if ( it != m_MonthCard.end() )
	{
		if( it->second.nOpenTime + pMonthCardConfig->nOpenDays < nCurrentTime )
		{
			return EMonthCardMsgError_NotOpen;
		}
	}
	else
	{
		return EMonthCardMsgError_NotOpen;
	}

	if ( !isCanReward( it->second.nOpenTime, it->second.nRewardTime ))
	{
		return EMonthCardMsgError_HaveGetReward;
	}
	
	it->second.nRewardTime = nCurrentTime;
	MonthCardDB * pSaveMonthCard = new MonthCardDB( it->second );
	AddQuery(Query_Update_MonthCardInfo, m_pRoleAttr->GetRoleID(), pSaveMonthCard );

	// ½±Àø
	m_pRoleAttr->ChangeBindBill( pMonthCardConfig->nEveryDayMBillReward, EChangeBindBillCause_Add_MonthCardEveryDayReward );

	if ( m_pRoleAttr->GetSex() == ESexType_Male )
	{
		CItemProcess::AddItems( *(CRoleEntity *)Entity(), pMonthCardConfig->listEveryDayMaleItemReward, EItemAction_Add_MonthCardEveryDayReward, 0, true );
	}
	else
	{
		CItemProcess::AddItems( *(CRoleEntity *)Entity(), pMonthCardConfig->listEveryDayFemaleItemReward, EItemAction_Add_MonthCardEveryDayReward, 0, true );
	}

	m_pRoleAttr->ChangeMoney( pMonthCardConfig->nEveryDayMoneyReward, EChangeMoneyCause_Add_MonthCardEveryDayReward, 0 ); 

	return EMonthCardMsgError_Success;
}








 // end of file

