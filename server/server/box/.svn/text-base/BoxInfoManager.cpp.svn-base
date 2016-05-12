#include "BoxInfoManager.h"
#include "../../socket/Windefine.h"
#include "../../socket/Utility.h"
#include "../../socket/Log.h"
#include "../util/CommonFunc.h"


CBoxInfo::CBoxInfo(itemtype_t nBoxID)
{
	m_nBoxID = nBoxID;
}

CBoxInfo::~CBoxInfo(void)
{
}


void CBoxInfo::AddTheGroup(int nGroupID, int nGroupRate, bool bMale)
{
	std::map<int, int>& boxGroup = ( bMale ? m_MaleBoxGroup : m_FemaleBoxGroup );
	if ( boxGroup.find( nGroupID ) == boxGroup.end() )
	{
		boxGroup.insert( std::make_pair( nGroupID, nGroupRate ) );
	}
	else
	{
		WriteLog(LOGLEVEL_WARNING, "Error: CBoxInfoManager::AddBoxGroupInfo. Same group(%d) for same sex type, ignore", nGroupID );
	}
}

void CBoxInfo::AddTheItem(int nGroupID, const char* pItemInfo, bool bAnnounce, int nItemRate, bool bMale)
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
			WriteLog(LOGLEVEL_ERROR, "Error: CBoxInfoManager::AddBoxItemInfo. Check item list size error. group=%d iteminfo=%s", nGroupID, pItemInfo );
		}
	}
	else
	{
		WriteLog(LOGLEVEL_ERROR, "Error: CBoxInfoManager::AddBoxItemInfo. Transfer item list error. group=%d iteminfo=%s", nGroupID, pItemInfo );
	}

}

void CBoxInfo::GetRandomItem(bool bIsMale, std::list<CItem>& rItemList) const
{
	if ( bIsMale )
	{
		_RandomItem( rItemList, m_MaleBoxGroup, m_MaleBoxItem );
	}
	else
	{
		_RandomItem( rItemList, m_FemaleBoxGroup, m_FemaleBoxItem );
	}
}

void CBoxInfo::_RandomItem(std::list<CItem>& rItemList, const std::map<int, int>& rBoxGroup, const std::map<int, CItemRandCollect>& rBoxItem) const
{
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

bool CBoxInfo::CheckBoxInfo() const
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
				WriteLog(LOGLEVEL_ERROR, "Error: CBoxInfoManager::CheckBoxData. Empty rand list for male group(%d), box(%d)", it_MaleGroup->first, m_nBoxID );
			}
		}
	}

	for ( std::map<int, CItemRandCollect>::const_iterator it_MaleItem = m_MaleBoxItem.begin(); it_MaleItem != m_MaleBoxItem.end(); ++it_MaleItem )
	{
		if ( !it_MaleItem->second.Check() )
		{
			WriteLog(LOGLEVEL_ERROR, "Error: CBoxInfoManager::CheckBoxData. group(%d) male item not exist", it_MaleItem->first);
			return false;
		}
	}

	for ( std::map<int, int>::const_iterator it_FemaleGroup = m_FemaleBoxGroup.begin(); it_FemaleGroup != m_FemaleBoxGroup.end(); ++it_FemaleGroup )
	{
		if ( !bFemaleGroupOK && it_FemaleGroup->second == 10000 )
		{
			bFemaleGroupOK = true;

			if ( m_FemaleBoxItem.find( it_FemaleGroup->first ) == m_FemaleBoxItem.end() )
			{
				WriteLog(LOGLEVEL_ERROR, "Error: CBoxInfoManager::CheckBoxData. Empty rand list for female group(%d), box(%d)", it_FemaleGroup->first, m_nBoxID );
			}
		}
	}


	for ( std::map<int, CItemRandCollect>::const_iterator it_FemaleItem = m_FemaleBoxItem.begin(); it_FemaleItem != m_FemaleBoxItem.end(); ++it_FemaleItem )
	{
		if ( !it_FemaleItem->second.Check() )
		{
			WriteLog(LOGLEVEL_ERROR, "Error: CBoxInfoManager::CheckBoxData. group(%d) female item not exist", it_FemaleItem->first);
			return false;
		}
	}

	bool bValid = true;

	if ( !bMaleGroupOK )
	{
		bValid = false;
		WriteLog(LOGLEVEL_ERROR, "Error: CBoxInfoManager::CheckBoxData. No 100 rand group for male, box(%d)", m_nBoxID );
	}

	if ( !bFemaleGroupOK )
	{
		bValid = false;
		WriteLog(LOGLEVEL_ERROR, "Error: CBoxInfoManager::CheckBoxData. No 100 rand group for female, box(%d)", m_nBoxID );
	}

	return bValid;
}


CBoxInfoManager::CBoxInfoManager(void)
{	
}

CBoxInfoManager::~CBoxInfoManager(void)
{
	for ( std::map<itemtype_t, CBoxInfo*>::iterator it = m_BoxMap.begin(); it != m_BoxMap.end(); ++it )
	{
		delete it->second;
		it->second = NULL;
	}

	m_BoxMap.clear();
}


CBoxInfoManager& CBoxInfoManager::Instance()
{
	static CBoxInfoManager s_Instance;
	return s_Instance;
}

void CBoxInfoManager::AddBoxGroup(itemtype_t nBoxID, int nGroupID, int nGroupRate, bool bMale)
{
	if ( nBoxID > 0 && nGroupID > 0 && nGroupRate > 0 )
	{
		std::map<itemtype_t, CBoxInfo*>::iterator it = m_BoxMap.find( nBoxID );
		if ( it == m_BoxMap.end() )
		{
			CBoxInfo* pBoxInfo = new CBoxInfo( nBoxID );
			pBoxInfo->AddTheGroup( nGroupID, nGroupRate, bMale );

			m_BoxMap.insert( std::make_pair( nBoxID, pBoxInfo ) );
		}
		else
		{
			CBoxInfo* pBoxInfo = it->second;
			if ( pBoxInfo != NULL )
			{
				pBoxInfo->AddTheGroup( nGroupID, nGroupRate, bMale );
			}
		}
	}
}

void CBoxInfoManager::AddBoxItem(itemtype_t nBoxID, int nGroupID, const char* pItemInfo, bool bAnnounce, int nItemRate, bool bMale)
{
	if ( nBoxID > 0 && nGroupID > 0 && nItemRate > 0 )
	{
		std::map<itemtype_t, CBoxInfo*>::iterator it_BoxInfo = m_BoxMap.find( nBoxID );
		if ( it_BoxInfo == m_BoxMap.end() )
		{
			CBoxInfo* pBoxInfo = new CBoxInfo( nBoxID );
			pBoxInfo->AddTheItem( nGroupID, pItemInfo, bAnnounce, nItemRate, bMale );

			m_BoxMap.insert( std::make_pair( nBoxID, pBoxInfo ) );
		}
		else
		{
			CBoxInfo* pBoxInfo = it_BoxInfo->second;
			if ( pBoxInfo != NULL )
			{
				pBoxInfo->AddTheItem( nGroupID, pItemInfo, bAnnounce, nItemRate, bMale );
			}
		}
	}
}

CBoxInfo* CBoxInfoManager::GetBoxInfoByID(itemtype_t nBoxID) const
{
	CBoxInfo* pBoxInfo = NULL;

	if ( nBoxID > 0 )
	{
		std::map<itemtype_t, CBoxInfo*>::const_iterator it= m_BoxMap.find( nBoxID );
		if( it != m_BoxMap.end() )
		{
			pBoxInfo = it->second;
		}
	}
	
	return pBoxInfo;
}

bool CBoxInfoManager::CheckAllBox() const
{
	bool bValid = true;

	for ( std::map<itemtype_t, CBoxInfo*>::const_iterator it = m_BoxMap.begin(); it != m_BoxMap.end(); ++it )
	{
		CBoxInfo* pBoxInfo = it->second;

		if ( pBoxInfo != NULL && !pBoxInfo->CheckBoxInfo() )
		{
			bValid = false;
			break;
		}
	}

	return bValid;
}

