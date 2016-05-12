#include "DynamciBoxInfoManager.h"
#include "../../socket/Windefine.h"
#include "../../socket/Utility.h"
#include "../../socket/Log.h"
#include "../util/CommonFunc.h"
#include "BoxInfoManager.h"
#include <algorithm>


CDynamicBoxInfo::CDynamicBoxInfo(itemtype_t nBoxID)
	:m_nRelatedItemID(0)
	,m_nRelatedRate(0)
	,m_nItemRate(0)
{
	m_nBoxID = nBoxID;
}

CDynamicBoxInfo::~CDynamicBoxInfo()
{
}


void CDynamicBoxInfo::AddTheGroup(int nGroupID, int nGroupRate, bool bMale)
{
	std::map<int, int>& boxGroup = ( bMale ? m_MaleBoxGroup : m_FemaleBoxGroup );
	if ( boxGroup.find( nGroupID ) == boxGroup.end() )
	{
		boxGroup.insert( std::make_pair( nGroupID, nGroupRate ) );
	}
	else
	{
		WriteLog( "Error: CDynamicBoxInfoManager::AddBoxGroupInfo. Same group(%d) for same sex type, ignore", nGroupID );
	}
}

void CDynamicBoxInfo::AddRelatedGroup( unsigned int nItemID, unsigned int nRelatedRate, unsigned int nItemRated )
{
	m_nRelatedItemID = nItemID;
	m_nRelatedRate = nRelatedRate;
	m_nItemRate	= nItemRated;
}

void CDynamicBoxInfo::AddTheItem(int nGroupID, const char* pItemInfo, bool bAnnounce, int nItemRate, bool bMale)
{
	std::list<CItem> itemList;
	if ( TransferItemList( pItemInfo, itemList ) )
	{
		if ( itemList.size() == 1 )
		{
			std::list<CItem>::iterator it_Item = itemList.begin();
			CItem& randomItem = *it_Item;
			randomItem.m_bAnnounce = bAnnounce;

			std::map<int, CItemRandCollect>& boxItem = ( bMale ? m_MaleBoxItem : m_FemaleBoxItem );
			std::map<int, CItemRandCollect>::iterator it_IRC = boxItem.find( nGroupID );
			if ( it_IRC == boxItem.end() )
			{
				CItemRandCollect itemRC;
				itemRC.AddRandItem( randomItem, nItemRate );

				boxItem.insert( std::make_pair( nGroupID, itemRC ) );
			}
			else
			{
				CItemRandCollect& itemRC = it_IRC->second;
				itemRC.AddRandItem( randomItem, nItemRate );
			}
		}
		else
		{
			WriteLog( "Error: CDynamicBoxInfo::AddBoxItemInfo. Check item list size error. group=%d iteminfo=%s", nGroupID, pItemInfo );
		}
	}
	else
	{
		WriteLog( "Error: CDynamicBoxInfo::AddBoxItemInfo. Transfer item list error. group=%d iteminfo=%s", nGroupID, pItemInfo );
	}

}

void CDynamicBoxInfo::GetRandomItem(int nRelatedValue, bool bIsMale, std::list<CItem>& rItemList) const
{
	if ( bIsMale )
	{
		_RandomItem( nRelatedValue, rItemList, m_MaleBoxGroup, m_MaleBoxItem );
	}
	else
	{
		_RandomItem( nRelatedValue, rItemList, m_FemaleBoxGroup, m_FemaleBoxItem );
	}
}

void CDynamicBoxInfo::_RandomItem(int nRelatedValue, std::list<CItem>& rItemList, const std::map<int, int>& rBoxGroup, const std::map<int, CItemRandCollect>& rBoxItem) const
{
	// 关联物品
	if ( m_nRelatedItemID > 0 && m_nItemRate > 0 )
	{
		CItem item;
		item.m_nItemType = (itemtype_t)m_nRelatedItemID;
		item.m_nItemCount = (itemcount_t)(nRelatedValue*m_nRelatedRate/m_nItemRate);

		if ( item.m_nItemCount != 0)
		{
			rItemList.push_back( item );
		}
	}

	// 随机物品
	int nRandom = Random( 1, 10000 );
	for ( std::map<int, int>::const_iterator it_Group = rBoxGroup.begin(); it_Group != rBoxGroup.end(); ++it_Group )
	{
		if ( nRandom <= it_Group->second )
		{
			std::map<int, CItemRandCollect>::const_iterator it_Item = rBoxItem.find( it_Group->first );
			if ( it_Item != rBoxItem.end() )
			{
				CItem randomItem;
				it_Item->second.RandSingleItem( randomItem );

				rItemList.push_back( randomItem );
			}
		}
	}
}

bool CDynamicBoxInfo::CheckBoxInfo() const
{
	bool bMaleGroupOK = false;
	bool bFemaleGroupOK = false;

	for ( std::map<int, int>::const_iterator it_MaleGroup = m_MaleBoxGroup.begin(); it_MaleGroup != m_MaleBoxGroup.end(); ++it_MaleGroup )
	{
		if ( !bMaleGroupOK && it_MaleGroup->second == 10000 )
		{
			bMaleGroupOK = true;

			if ( m_MaleBoxItem.find( it_MaleGroup->first ) == m_MaleBoxItem.end() )
			{
				WriteLog( "Error: CBoxInfoManager::CheckBoxData. Empty rand list for male group(%d), box(%d)", it_MaleGroup->first, m_nBoxID );
			}
		}
	}

	for ( std::map<int, int>::const_iterator it_FemaleGroup = m_FemaleBoxGroup.begin(); it_FemaleGroup != m_FemaleBoxGroup.end(); ++it_FemaleGroup )
	{
		if ( !bFemaleGroupOK && it_FemaleGroup->second == 10000 )
		{
			bFemaleGroupOK = true;

			if ( m_FemaleBoxItem.find( it_FemaleGroup->first ) == m_FemaleBoxItem.end() )
			{
				WriteLog( "Error: CBoxInfoManager::CheckBoxData. Empty rand list for female group(%d), box(%d)", it_FemaleGroup->first, m_nBoxID );
			}
		}
	}

	bool bValid = true;

	if ( !bMaleGroupOK )
	{
		bValid = false;
		WriteLog( "Error: CBoxInfoManager::CheckBoxData. No 100 rand group for male, box(%d)", m_nBoxID );
	}

	if ( !bFemaleGroupOK )
	{
		bValid = false;
		WriteLog( "Error: CBoxInfoManager::CheckBoxData. No 100 rand group for female, box(%d)", m_nBoxID );
	}

	return bValid;
}


CDynamicBoxInfoManager::CDynamicBoxInfoManager()
{	
}

CDynamicBoxInfoManager::~CDynamicBoxInfoManager()
{
	for ( std::map<itemtype_t, CDynamicBoxInfo*>::iterator it = m_BoxMap.begin(); it != m_BoxMap.end(); ++it )
	{
		delete it->second;
		it->second = NULL;
	}

	m_BoxMap.clear();
}


CDynamicBoxInfoManager& CDynamicBoxInfoManager::Instance()
{
	static CDynamicBoxInfoManager s_Instance;
	return s_Instance;
}

void CDynamicBoxInfoManager::AddDynamicBoxRelatedGroup(itemtype_t nBoxID, unsigned int nItemID, unsigned int nRelatedRate, unsigned int nItemRated )
{
	if ( nBoxID > 0 && nItemID > 0 )
	{
		std::map<itemtype_t, CDynamicBoxInfo*>::iterator it = m_BoxMap.find( nBoxID );
		if ( it == m_BoxMap.end() )
		{
			CDynamicBoxInfo* pBoxInfo = new CDynamicBoxInfo( nBoxID );
			pBoxInfo->AddRelatedGroup( nItemID, nRelatedRate, nItemRated );

			m_BoxMap.insert( std::make_pair( nBoxID, pBoxInfo ) );
		}
		else
		{
			CDynamicBoxInfo* pBoxInfo = it->second;
			if ( pBoxInfo != NULL )
			{
				pBoxInfo->AddRelatedGroup( nItemID, nRelatedRate, nItemRated );
			}
		}
	}
}

void CDynamicBoxInfoManager::AddDynamicBoxGroup(itemtype_t nBoxID, int nGroupID, int nGroupRate, bool bMale)
{
	if ( nBoxID > 0 && nGroupID > 0 && nGroupRate > 0 )
	{
		std::map<itemtype_t, CDynamicBoxInfo*>::iterator it = m_BoxMap.find( nBoxID );
		if ( it == m_BoxMap.end() )
		{
			CDynamicBoxInfo* pBoxInfo = new CDynamicBoxInfo( nBoxID );
			pBoxInfo->AddTheGroup( nGroupID, nGroupRate, bMale );

			m_BoxMap.insert( std::make_pair( nBoxID, pBoxInfo ) );
		}
		else
		{
			CDynamicBoxInfo* pBoxInfo = it->second;
			if ( pBoxInfo != NULL )
			{
				pBoxInfo->AddTheGroup( nGroupID, nGroupRate, bMale );
			}
		}
	}
}

void CDynamicBoxInfoManager::AddDynamicBoxItem(itemtype_t nBoxID, int nGroupID, const char* pItemInfo, bool bAnnounce, int nItemRate, bool bMale)
{
	if ( nBoxID > 0 && nGroupID > 0 && nItemRate > 0 )
	{
		std::map<itemtype_t, CDynamicBoxInfo*>::iterator it_BoxInfo = m_BoxMap.find( nBoxID );
		if ( it_BoxInfo == m_BoxMap.end() )
		{
			CDynamicBoxInfo* pBoxInfo = new CDynamicBoxInfo( nBoxID );
			pBoxInfo->AddTheItem( nGroupID, pItemInfo, bAnnounce, nItemRate, bMale );

			m_BoxMap.insert( std::make_pair( nBoxID, pBoxInfo ) );
		}
		else
		{
			CDynamicBoxInfo* pBoxInfo = it_BoxInfo->second;
			if ( pBoxInfo != NULL )
			{
				pBoxInfo->AddTheItem( nGroupID, pItemInfo, bAnnounce, nItemRate, bMale );
			}
		}
	}
}

CDynamicBoxInfo* CDynamicBoxInfoManager::GetDynamicBoxInfoByID(itemtype_t nBoxID) const
{
	CDynamicBoxInfo* pBoxInfo = NULL;

	if ( nBoxID > 0 )
	{
		std::map<itemtype_t, CDynamicBoxInfo*>::const_iterator it= m_BoxMap.find( nBoxID );
		if( it != m_BoxMap.end() )
		{
			pBoxInfo = it->second;
		}
	}

	return pBoxInfo;
}

bool CDynamicBoxInfoManager::CheckAllBox() const
{
	bool bValid = true;

	for ( std::map<itemtype_t, CDynamicBoxInfo*>::const_iterator it = m_BoxMap.begin(); it != m_BoxMap.end(); ++it )
	{
		CDynamicBoxInfo* pBoxInfo = it->second;

		if ( pBoxInfo != NULL && !pBoxInfo->CheckBoxInfo() )
		{
			bValid = false;
			break;
		}
	}

	return bValid;
}

