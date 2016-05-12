#include "ItemProcess.h"
#include "../../datastructure/RoleEntity.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../../datastructure/ItemErrorInfo.h"
#include "../../socket/Utility.h"
#include "EntityItemComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../mail/MailMgr.h"
#include "../../socket/Log.h"
#include "../../datastructure/datainfor/ConfigManager.h"

CItemProcess::CItemProcess()
{
}
CItemProcess::~CItemProcess()
{
}

//给玩家添加物品
bool CItemProcess::AddItems(CRoleEntity & rRoleEntity, const std::list< CItem > & rItemList, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient)
{
	CEntityItemComponent * pItemCom = rRoleEntity.GetComponent< CRoleItem >();
	if (pItemCom != NULL)
	{
		if (pItemCom->CanAddAllItem(rItemList) == EItemError_Success)
		{
			std::list< CItem >::const_iterator it = rItemList.begin();
			for (; it != rItemList.end(); ++it)
			{
				const CItem & rItem = *it;
				if (EItemError_Success != pItemCom->AddItem(rItem.m_nItemType, rItem.m_nItemCount, rItem.m_nValidTime, eItemAction, nSrcData, bNoticeClient, false))
				{
					WriteLog(LOGLEVEL_ERROR, "Error: CItemProcess::AddItems.");
				}
			}

			return true;
		}
	}

	return false;
}

//先给玩家发送物品，如果失败，则发邮件
void CItemProcess::AddOrMailItems(CRoleEntity &rRoleEntity, const std::list<CItem> &rItemList, EItemAction eItemAction, __int64 nSrcData, 
    bool bNoticeClient, EMailType nMailType, const std::string &rMailTitle, const std::string &rMailContent)
{
    AddOrMailItems(rRoleEntity, rItemList, eItemAction, nSrcData, bNoticeClient, nMailType, "", rMailTitle, rMailContent);
}

void CItemProcess::AddOrMailItems(CRoleEntity &rRoleEntity, const std::list<CItem> &rItemList, EItemAction eItemAction, __int64 nSrcData, 
    bool bNoticeClient, EMailType nMailType, const std::string &rMailSender, const std::string &rMailTitle, const std::string &rMailContent)
{
    CEntityItemComponent *pItemCom = rRoleEntity.GetComponent<CRoleItem>();
    CEntityAttributeComponent *pAttrCom = rRoleEntity.GetComponent<CRoleAttribute>();

    if (pItemCom != NULL && pAttrCom != NULL)
    {
        for (std::list<CItem>::const_iterator itr = rItemList.begin(); 
             itr != rItemList.end(); ++itr)
        {
            if (EItemUse_ColumnFull == pItemCom->AddItem(itr->m_nItemType, itr->m_nItemCount, itr->m_nValidTime, eItemAction, 
                nSrcData, bNoticeClient, false) )
            {
                CMailMgr::Instance().SendMailToPlayer( pAttrCom->GetRoleID(), nMailType, rMailSender, rMailTitle,
                    rMailContent, *itr );
            }
        }
    }
}

bool CheckItemVec(const std::vector<CItem>& itemVec)
{
	bool bIsExistItem = true;
	for (std::vector<CItem>::const_iterator it = itemVec.begin(); it != itemVec.end(); ++it)
	{
		ItemConfig* pItem = ConfigManager::Instance().GetItemConfigManager().GetByID( it->m_nItemType );
		if ( pItem == NULL )
		{
			bIsExistItem = false;
			WriteLog(LOGLEVEL_ERROR, "Error: CheckItemVec item is not exist. item=%d", (int)it->m_nItemType );
		}
	}
	return bIsExistItem;
}

CItemRandCollect::CItemRandCollect()
{

}
CItemRandCollect::~CItemRandCollect()
{

}

void CItemRandCollect::AddRandItem(CItem& rRandItem, int nRate)
{
	if ( nRate < 0 )
	{
		nRate = 0;
	}

	m_RandItemVec.push_back( rRandItem );
	m_RandRateVec.push_back( nRate );
}

bool CItemRandCollect::RandSingleItem(CItem& rItem) const
{
	if ( !m_RandItemVec.empty() )
	{
		int nIndex = RateRandom( m_RandRateVec );

		if ( nIndex >= 0 )
		{
			rItem = m_RandItemVec[nIndex];
			return true;
		}
	}

	return false;
}

void CItemRandCollect::RandMultiItem(std::list<CItem>& rItemList, int nCount) const
{
	if ( !m_RandItemVec.empty() && nCount > 0 )
	{
		std::vector<int> curRateVec = m_RandRateVec;
		for ( int i = 0; i < nCount; ++i )
		{
			int nIndex = RateRandom( curRateVec );

			if ( nIndex >= 0 )
			{
				rItemList.push_back( m_RandItemVec[nIndex] );
				curRateVec[nIndex] = 0;
			}
		}
	}
}

const std::vector<CItem>& CItemRandCollect::RandItemCollect() const
{
	return m_RandItemVec;
}

bool CItemRandCollect::Check() const
{
	return CheckItemVec(m_RandItemVec);
}
void CItemRandCollect::GetItem( std::list<CItem> & itemlist ) const
{
	std::vector<CItem>::const_iterator it = m_RandItemVec.begin();
	for( ; it != m_RandItemVec.end(); ++it )
	{
		itemlist.push_back( *it );
	}
}

CItemBetCollect::CItemBetCollect()
{
}

CItemBetCollect::~CItemBetCollect()
{
}

void CItemBetCollect::AddBetItem(CItem & rBetItem, bool bCanFree, int nFreeRate, int nChargeRate)
{
	if ( !bCanFree || nFreeRate < 0 )
	{
		nFreeRate = 0;
	}

	if ( nChargeRate < 0 )
	{
		nChargeRate = 0;
	}

	char chBetMode = ( bCanFree ? 0 : 1 );
	m_BetItemVec.push_back( rBetItem );
	m_BetModeVec.push_back( chBetMode );
	m_FreeRateVec.push_back( nFreeRate );
	m_ChargeRateVec.push_back( nChargeRate );
}

int CItemBetCollect::BetSingleItem(CItem& rItem, bool bIsFree) const
{
	int nIndex = -1;

	if ( !m_BetItemVec.empty() )
	{
		nIndex = ( bIsFree ? RateRandom( m_FreeRateVec ) : RateRandom( m_ChargeRateVec ) );

		if ( nIndex >= 0 )
		{
			rItem = m_BetItemVec[nIndex];
		}
	}

	return nIndex;
}

void CItemBetCollect::BetMultiItem(std::list<CItem>& rItemList, int nCount, bool bIsFree) const
{
	if ( !m_BetItemVec.empty() && nCount > 0 )
	{
		std::vector<int> curRateVec = ( bIsFree ? m_FreeRateVec : m_ChargeRateVec );
		for ( int i = 0; i < nCount; ++i )
		{
			int nIndex = RateRandom( curRateVec );

			if ( nIndex >= 0 )
			{
				rItemList.push_back( m_BetItemVec[nIndex] );
				curRateVec[nIndex] = 0;
			}
		}
	}
}

const std::vector<CItem>& CItemBetCollect::BetItemCollect() const
{
	return m_BetItemVec;
}

const std::vector<char>& CItemBetCollect::BetModeCollect() const
{
	return m_BetModeVec;
}

bool CItemBetCollect::Check() const
{
	return CheckItemVec(m_BetItemVec);
}

