#include "ItemMsgDefine.h"
#include "../share/ServerMsgDef.h"
#include "../../datastructure/ItemBase.h"
#include"../../datastructure/EquipItem.h"
#include "EntityItemComponent.h"
#include "../../datastructure/ItemErrorInfo.h"
#include "../mall/CurrencyType.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_RefreshItem
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RefreshItem::GameMsg_S2C_RefreshItem(void):_baseClass(MSG_S2C_RefreshItem)
{
}

GameMsg_S2C_RefreshItem::~GameMsg_S2C_RefreshItem(void)
{
}

bool GameMsg_S2C_RefreshItem::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt(m_nItemType);
	IOBuff.AddUShort(m_nColumn);
	IOBuff.AddUShort((unsigned short)m_mapItems.size());
	std::map<unsigned short,ItemBase*>::iterator it = m_mapItems.begin();
	for ( ; it != m_mapItems.end(); ++it )
	{
        IOBuff.AddUShort( it->first );
        ItemBase * pItemBase = it->second;
		IOBuff.AddUChar(pItemBase==NULL?0:1);
		if (pItemBase != NULL)
		{
			pItemBase->Encode(&IOBuff);
		}
	}

	return true;
}


GameMsg_S2C_BatchRefreshItem::GameMsg_S2C_BatchRefreshItem(void)
    :GameMsg_Base(MSG_S2C_BatchRefreshItem)
{
}

GameMsg_S2C_BatchRefreshItem::~GameMsg_S2C_BatchRefreshItem(void)
{
}

bool GameMsg_S2C_BatchRefreshItem::doEncode( CParamPool& IOBuff )
{
    IOBuff.AddUShort((unsigned short)m_mapChangeItem.size());
    std::map<itemgrid_t, ItemBase*>::iterator it = m_mapChangeItem.begin();
    for (; it != m_mapChangeItem.end(); ++it)
    {
        IOBuff.AddUShort((unsigned short)it->first.first); //栏位
        IOBuff.AddUShort((unsigned short)it->first.second); //格子
        IOBuff.AddBool(it->second != NULL ? true : false);//是否有物品
        if (it->second != NULL)
        {
            IOBuff.AddUInt(it->second->GetType());
            it->second->Encode(&IOBuff); // 具体物品信息
        }
    }

    return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_RefreshItemFailed
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_UseItemFailed::GameMsg_S2C_UseItemFailed()
:_baseClass(MSG_S2C_UseItemFailed)
,m_nErrorCode(0)
{
}

GameMsg_S2C_UseItemFailed::~GameMsg_S2C_UseItemFailed(void)
{
}

bool GameMsg_S2C_UseItemFailed::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUChar(m_nErrorCode);
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_UseItem
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_UseItem::GameMsg_C2S_UseItem(void):_baseClass(MSG_C2S_UseItem)
{
	nColumn = 0;
	nIndex = 0;
	nDestRoleId = 0;
}

GameMsg_C2S_UseItem::~GameMsg_C2S_UseItem(void)
{
}

bool GameMsg_C2S_UseItem::doDecode( CParamPool& IOBuff )
{
	nColumn = IOBuff.GetUShort();
	nIndex = IOBuff.GetUShort();
	nDestRoleId = IOBuff.GetUInt();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_BatchUseItem::GameMsg_C2S_BatchUseItem(void)
	:GameMsg_Base(MSG_C2S_BatchUseItem)
	,m_nColumn(0)
	,m_nIndex(0)
	,m_nItemID(0)
	,m_nCount(0)
{
}

GameMsg_C2S_BatchUseItem::~GameMsg_C2S_BatchUseItem(void)
{
}

bool GameMsg_C2S_BatchUseItem::doDecode( CParamPool& IOBuff )
{
	m_nColumn = IOBuff.GetUShort();
	m_nIndex = IOBuff.GetUShort();
	m_nItemID = IOBuff.GetUInt();
	m_nCount = IOBuff.GetInt();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_PatchUseItemEffect::GameMsg_S2C_PatchUseItemEffect(void)
	:GameMsg_Base(MSG_S2C_PatchUseItemEffect)
	,m_nItemID(0)
	,m_nCount(0)
	,m_strDesc("")
{
}

GameMsg_S2C_PatchUseItemEffect::~GameMsg_S2C_PatchUseItemEffect(void)
{
}

bool GameMsg_S2C_PatchUseItemEffect::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nItemID );
	IOBuff.AddInt( m_nCount );
	IOBuff.AddStringW( m_strDesc );
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_ChangeEquipItem
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_ChangeEquipItem::GameMsg_C2S_ChangeEquipItem(void):_baseClass(MSG_C2S_ChangeEquipItem)
{
	int i = 0 ;
	for( i = 0 ;i <eItemClothType_Max ; i++)
	{
		nIndexs[i][0] = -1;
		nIndexs[i][1] = -1;
	}
}

GameMsg_C2S_ChangeEquipItem::~GameMsg_C2S_ChangeEquipItem(void)
{
}

bool GameMsg_C2S_ChangeEquipItem::doDecode( CParamPool& IOBuff )
{
	short nPart;
	short nConum;
	short nIndex;
	while( IOBuff.HasRemaining() )
	{
		if( IOBuff.Remaining() )
		{
			nPart = IOBuff.GetShort();
			nConum = IOBuff.GetShort();
			nIndex = IOBuff.GetShort();
			if( nConum == EItemColumn_ClothBag || nConum == EItemColumn_ClothOnPlayer )
			{
				if( nPart >= 0 && nPart< eItemClothType_Max )
				{
					nIndexs[ nPart ][0] = nConum;
					nIndexs[ nPart ][1] = nIndex;
				}
			}
/*
			else if( nConum = ItemBagColumn_Badge )
			{
				if( nPart >= 0 && nPart< BADGE_BAG_MAXNUM )
				{
					nIndexsBadge[ nPart ][0] = nConum;
					nIndexsBadge[ nPart ][1] = nIndex;
				}
			}
*/
		}
		else
		{
			break;
		}
	}
	return true;

}

GameMsg_S2C_ChangeEquipItem::GameMsg_S2C_ChangeEquipItem()
    :_baseClass(MSG_S2C_ChangeEquipItem)
    ,m_nRoleID(0)
    ,m_pItemComponent(NULL)
{

}

GameMsg_S2C_ChangeEquipItem::~GameMsg_S2C_ChangeEquipItem(void)
{
}

bool GameMsg_S2C_ChangeEquipItem::doEncode( CParamPool& IOBuff )
{
    if (m_pItemComponent == NULL)
    {
        return true;
    }

    IOBuff.AddUInt(m_nRoleID);
    for(int i = 0 ;i< eItemClothType_Max; i++)
    {
        ItemEquip * pOldCloth = NULL;
        ItemEquip * pNewCloth = NULL;
        std::map<int,int>::iterator it = m_mapOldEquipSlot.find(i);
        if (it != m_mapOldEquipSlot.end())
        {
           // pOldCloth = (ItemEquip*)m_pItemComponent->GetItem(EItemColumn_ClothBag, it->second);
        }

        std::map<int,int>::iterator iter = m_mapNewEquipSlot.find(i);
        if (iter != m_mapNewEquipSlot.end())
        {
            //pNewCloth = (ItemEquip*)m_pItemComponent->GetItem(EItemColumn_ClothBag, iter->second);
        }
        
        if( pOldCloth != pNewCloth )
        {
            if( pNewCloth == NULL )
            {
                IOBuff.AddUChar( 1 );
            }
            else
            {
                IOBuff.AddUChar( 2 );
                IOBuff.AddUInt(pNewCloth->GetType());
                pNewCloth->Encode(&IOBuff);
            }
        }
        else
        {
            IOBuff.AddUChar( 0 );
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_ChangeEquipItemFail
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_ChangeEquipItemFail::GameMsg_S2C_ChangeEquipItemFail()
:GameMsg_Base(MSG_S2C_ChangeEquipItemFail)
,m_nColumn(0)
,m_nIndex(0)
,m_nError(0)
{

}

GameMsg_S2C_ChangeEquipItemFail::~GameMsg_S2C_ChangeEquipItemFail()
{

}

bool GameMsg_S2C_ChangeEquipItemFail::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUChar(m_nError);
	IOBuff.AddUShort(m_nColumn);
	IOBuff.AddUShort(m_nIndex);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_RemoveItem
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_RemoveItem::GameMsg_C2S_RemoveItem(void):_baseClass(MSG_C2S_RemoveItem)
{
}

GameMsg_C2S_RemoveItem::~GameMsg_C2S_RemoveItem(void)
{
}

bool GameMsg_C2S_RemoveItem::doDecode( CParamPool& IOBuff )
{
	nColumn = IOBuff.GetUShort();
	nIndex = IOBuff.GetUShort();

	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_C2S_ItemColumn
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_C2S_ItemColumn::GameMsg_C2S_ItemColumn(void):_baseClass(MSG_C2S_ItemColumn)
{
	nColumn = 0;
	nAddIndex = 0;
}

GameMsg_C2S_ItemColumn::~GameMsg_C2S_ItemColumn(void)
{
}

bool GameMsg_C2S_ItemColumn::doDecode( CParamPool& IOBuff )
{
	nColumn = IOBuff.GetUShort();
	nAddIndex = IOBuff.GetUShort();

	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_ItemColumn
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_ItemColumn::GameMsg_S2C_ItemColumn(void):_baseClass(MSG_S2C_ItemColumn)
{
	nResult = 0;
	nColumn = 0;
	nTotalGridNum = 0;
	strError = "";
}

GameMsg_S2C_ItemColumn::~GameMsg_S2C_ItemColumn(void)
{
}

bool GameMsg_S2C_ItemColumn::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUChar(nResult);
	IOBuff.AddUShort(nColumn);
	IOBuff.AddUShort(nTotalGridNum);
	IOBuff.AddStringW(strError.c_str());

	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_RefreshMoney
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RefreshMoney::GameMsg_S2C_RefreshMoney(void):_baseClass(MSG_S2C_RefreshMoney)
{
	nMoney = 0;
}

GameMsg_S2C_RefreshMoney::~GameMsg_S2C_RefreshMoney(void)
{
}

bool GameMsg_S2C_RefreshMoney::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddInt(nMoney);
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_RefreshPt
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RefreshPt::GameMsg_S2C_RefreshPt(void)
    :_baseClass(MSG_S2C_RefreshPt)
    ,m_nPoint(0)
    ,m_nTotalPoint(0)
{
}

GameMsg_S2C_RefreshPt::~GameMsg_S2C_RefreshPt(void)
{
}

bool GameMsg_S2C_RefreshPt::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt(m_nPoint);
    IOBuff.AddUInt(m_nTotalPoint);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_RefreshBindPt
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RefreshBindPt::GameMsg_S2C_RefreshBindPt(void):_baseClass(MSG_S2C_RefreshBindPt)
{
	nBindPoint = 0;
}

GameMsg_S2C_RefreshBindPt::~GameMsg_S2C_RefreshBindPt(void)
{
}

bool GameMsg_S2C_RefreshBindPt::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt(nBindPoint);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_RefreshExp
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RefreshExp::GameMsg_S2C_RefreshExp(void):_baseClass(MSG_S2C_RefreshExp)
{
	nExp = 0;
}

GameMsg_S2C_RefreshExp::~GameMsg_S2C_RefreshExp(void)
{
}

bool GameMsg_S2C_RefreshExp::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt(nExp);
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_RefreshLevel
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RefreshLevel::GameMsg_S2C_RefreshLevel(void)
    : _baseClass(MSG_S2C_RefreshLevel)
{
    m_nLevel = 0;
    m_nExp = 0;
    m_nFunctionID = 0;
}

GameMsg_S2C_RefreshLevel::~GameMsg_S2C_RefreshLevel(void)
{
}

bool GameMsg_S2C_RefreshLevel::doEncode( CParamPool& IOBuff )
{
    IOBuff.AddUShort(m_nLevel);
    IOBuff.AddUInt(m_nExp);
    IOBuff.AddInt(m_nFunctionID);
    return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_RefreshChange
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RefreshChange::GameMsg_S2C_RefreshChange(void):_baseClass(MSG_S2C_RefreshChange)
{
	nChange = 0;
}

GameMsg_S2C_RefreshChange::~GameMsg_S2C_RefreshChange(void)
{
}

bool GameMsg_S2C_RefreshChange::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt(nChange);
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_RefreshHot
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_RefreshHot::GameMsg_S2C_RefreshHot(void):GameMsg_Base(MSG_S2C_RefreshHot)
{
	nHot = 0;
}

GameMsg_S2C_RefreshHot::~GameMsg_S2C_RefreshHot(void)
{
}

bool GameMsg_S2C_RefreshHot::doEncode(CParamPool& IOBuff )
{
	IOBuff.AddUInt(nHot);
	return true;
}

GameMsg_S2C_RefreshTransform::GameMsg_S2C_RefreshTransform( void ):GameMsg_Base(MSG_S2C_RefreshTransform)
{
	nTransformId = 0;
}

GameMsg_S2C_RefreshTransform::~GameMsg_S2C_RefreshTransform( void )
{

}

bool GameMsg_S2C_RefreshTransform::doEncode( CParamPool& IOBuff )
{
	IOBuff << nTransformId;
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//GameMsg_S2C_SimulateRandomItem
//////////////////////////////////////////////////////////////////////////////////////////////////////
GameMsg_S2C_SimulateRandomItem::GameMsg_S2C_SimulateRandomItem(void):GameMsg_Base(MSG_S2C_SimulateRandomItem)
{
}

GameMsg_S2C_SimulateRandomItem::~GameMsg_S2C_SimulateRandomItem(void)
{

}

bool GameMsg_S2C_SimulateRandomItem::doEncode( CParamPool& IOBuff )
{
	unsigned short nCount = (unsigned short)allItem.size();
	IOBuff.AddUShort( nCount );

	for ( std::map<CItem, unsigned int>::iterator it = allItem.begin(); it != allItem.end(); ++it )
	{
		it->first.doEncode( IOBuff );
		IOBuff.AddUInt( it->second );
	}

	return true;
}

/////////////////////////////////////////////////
GameMsg_S2C_OpenBoxResult::GameMsg_S2C_OpenBoxResult()
:GameMsg_Base(MSG_S2C_OpenBoxResult)
,m_nCount(0)
,m_nItemType(0)
{

}

GameMsg_S2C_OpenBoxResult::~GameMsg_S2C_OpenBoxResult()
{

}

bool GameMsg_S2C_OpenBoxResult::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt( m_nCount );
	IOBuff.AddUInt( m_nItemType );
	IOBuff.AddUShort((unsigned short)m_listItems.size());
	for (std::list<CItem>::iterator itr = m_listItems.begin(); itr != m_listItems.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	return true;
}

/////////////////////////////////////////////////
GameMsg_S2C_OpenPackageResult::GameMsg_S2C_OpenPackageResult()
:GameMsg_Base(MSG_S2C_OpenPackageResult)
,m_nCount(0)
{
}

GameMsg_S2C_OpenPackageResult::~GameMsg_S2C_OpenPackageResult()
{

}

bool GameMsg_S2C_OpenPackageResult::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUInt(m_nCount);
	IOBuff.AddUInt(m_nItemTypeID);

    std::map<int, int>::iterator it = m_mapCurrency.find((int)EMallCurrencyType_Money);
    if (it != m_mapCurrency.end())
    {
        IOBuff.AddInt(it->second);
    }
    else
    {
        IOBuff.AddInt(0);
    }

    std::map<int, int>::iterator itr = m_mapCurrency.find((int)EMallCurrencyType_BindMCoin);
    if (itr != m_mapCurrency.end())
    {
        IOBuff.AddInt(itr->second);
    }
    else
    {
        IOBuff.AddInt(0);
    }

	IOBuff.AddUShort((unsigned short)m_listItems.size());
	for (std::list<CItem>::iterator itr = m_listItems.begin(); itr != m_listItems.end(); ++itr) {
		itr->doEncode(IOBuff);
	}

	return true;
}

/////////////////////////////////////////////////
GameMsg_S2C_SynExtendItemBinData::GameMsg_S2C_SynExtendItemBinData()
:GameMsg_Base(MSG_S2C_SynExtendItemBinData)
,m_nBinDataSize(0)
{
	memset(m_szExtendItemBinData, 0, C_EXTEND_ITEM_DATA_SIZE);
}

GameMsg_S2C_SynExtendItemBinData::~GameMsg_S2C_SynExtendItemBinData()
{

}

bool GameMsg_S2C_SynExtendItemBinData::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUShort(m_nBinDataSize);
	IOBuff.AddMemory(m_szExtendItemBinData, m_nBinDataSize);

	return true;
}

GameMsg_S2C_GetClothGroups::GameMsg_S2C_GetClothGroups():GameMsg_Base(MSG_S2C_GetClothGroups)
{
	m_nCurGroupID = 0;
}

GameMsg_S2C_GetClothGroups::~GameMsg_S2C_GetClothGroups()
{

}

bool GameMsg_S2C_GetClothGroups::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar(m_nCurGroupID);
	IOBuff.AddUShort((unsigned short)m_mapClothGroup.size());
	for (std::map<unsigned char, RoleClothGroup>::iterator it = m_mapClothGroup.begin();
		it != m_mapClothGroup.end(); ++it)
	{
		it->second.doEncode(IOBuff);
	}
	return true;
}

bool ClothGroupItem::operator==(const ClothGroupItem & item) 
{
	return m_nType == item.m_nType;
}

bool ClothGroupItem::doEncode( CParamPool &IOBuff ) const
{
	IOBuff << m_nType << m_nItemType;
	return true;
}

bool ClothGroupItem::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_nType >> m_nItemType;
	return true;
}

GameMsg_C2S_SelectClothGroup::GameMsg_C2S_SelectClothGroup():GameMsg_Base(MSG_C2S_SelectClothGroup)
{
	m_nGroupID = 0;
}

GameMsg_C2S_SelectClothGroup::~GameMsg_C2S_SelectClothGroup()
{

}

bool GameMsg_C2S_SelectClothGroup::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_nGroupID;
	return true;
}

GameMsg_S2C_SelectClothGroup::GameMsg_S2C_SelectClothGroup():GameMsg_Base(MSG_S2C_SelectClothGroup)
{
	m_nGroupID = 0;
}

GameMsg_S2C_SelectClothGroup::~GameMsg_S2C_SelectClothGroup()
{

}

bool GameMsg_S2C_SelectClothGroup::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_nGroupID << m_strError;
	IOBuff << (unsigned short)m_mapItemError.size();
	for (std::map<itemtype_t, stringw>::iterator it = m_mapItemError.begin();
		it != m_mapItemError.end(); ++it)
	{
		IOBuff << it->first << it->second;
	}
	return true;
}

GameMsg_C2S_UpdateClothGroup::GameMsg_C2S_UpdateClothGroup():GameMsg_Base(MSG_C2S_UpdateClothGroup)
{
	m_nGroupID = 0;
}

GameMsg_C2S_UpdateClothGroup::~GameMsg_C2S_UpdateClothGroup()
{

}

bool GameMsg_C2S_UpdateClothGroup::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_nGroupID >> m_listItems;
	return true;
}

GameMsg_S2C_UpdateClothGroup::GameMsg_S2C_UpdateClothGroup():GameMsg_Base(MSG_S2C_UpdateClothGroup)
{
	m_nGroupID = 0;
}

GameMsg_S2C_UpdateClothGroup::~GameMsg_S2C_UpdateClothGroup()
{

}

bool GameMsg_S2C_UpdateClothGroup::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_nGroupID << m_strError;
	IOBuff << (unsigned short)m_mapItemError.size();
	for (std::map<itemtype_t, stringw>::iterator it = m_mapItemError.begin();
		it != m_mapItemError.end(); ++it)
	{
		IOBuff << it->first << it->second;
	}
	IOBuff << m_listItems;
	return true;
}

GameMsg_C2S_UpdateClothGroupName::GameMsg_C2S_UpdateClothGroupName():GameMsg_Base(MSG_C2S_UpdateClothGroupName)
{
	m_nGroupID = 0;
}

GameMsg_C2S_UpdateClothGroupName::~GameMsg_C2S_UpdateClothGroupName()
{

}

bool GameMsg_C2S_UpdateClothGroupName::doDecode( CParamPool &IOBuff )
{
	IOBuff >> m_nGroupID >> m_strGroupName;
	return true;
}

GameMsg_S2C_UpdateClothGroupName::GameMsg_S2C_UpdateClothGroupName():GameMsg_Base(MSG_S2C_UpdateClothGroupName)
{
	m_nGroupID = 0;
}

GameMsg_S2C_UpdateClothGroupName::~GameMsg_S2C_UpdateClothGroupName()
{

}

bool GameMsg_S2C_UpdateClothGroupName::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_nGroupID << m_strError << m_strGroupName;
	return true;
}

GameMsg_C2S_GetCoolDown::GameMsg_C2S_GetCoolDown():GameMsg_Base(MSG_C2S_GetCoolDown)
{

}

GameMsg_C2S_GetCoolDown::~GameMsg_C2S_GetCoolDown()
{

}

bool GameMsg_C2S_GetCoolDown::doDecode( CParamPool &IOBuff )
{
	return true;
}

GameMsg_S2C_GetCoolDown::GameMsg_S2C_GetCoolDown():GameMsg_Base(MSG_S2C_GetCoolDown)
{

}

GameMsg_S2C_GetCoolDown::~GameMsg_S2C_GetCoolDown()
{

}

bool GameMsg_S2C_GetCoolDown::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_listItemCoolDown;
	return true;
}

GameMsg_S2C_UpdateCoolDown::GameMsg_S2C_UpdateCoolDown():GameMsg_Base(MSG_S2C_UpdateCoolDown)
{

}

GameMsg_S2C_UpdateCoolDown::~GameMsg_S2C_UpdateCoolDown()
{

}

bool GameMsg_S2C_UpdateCoolDown::doEncode( CParamPool &IOBuff )
{
	IOBuff << m_ItemCoolDown;
	return true;
}

GameMsg_C2S_ReplaceClothEffect::GameMsg_C2S_ReplaceClothEffect()
    :GameMsg_Base(MSG_C2S_ReplaceClothEffect)
    ,m_nClothColumn(0)
    ,m_nClothSlot(0)
    ,m_nEffectID(0)
{

}

GameMsg_C2S_ReplaceClothEffect::~GameMsg_C2S_ReplaceClothEffect()
{

}

bool GameMsg_C2S_ReplaceClothEffect::doDecode( CParamPool &IOBuff )
{
    m_nClothColumn = IOBuff.GetUShort();
    m_nClothSlot = IOBuff.GetUShort();
    m_nEffectID = IOBuff.GetUInt();
    return true;
}

GameMsg_S2C_ReplaceClothEffectFail::GameMsg_S2C_ReplaceClothEffectFail()
    :GameMsg_Base(MSG_S2C_ReplaceClothEffectFail)
{

}

GameMsg_S2C_ReplaceClothEffectFail::~GameMsg_S2C_ReplaceClothEffectFail()
{

}

bool GameMsg_S2C_ReplaceClothEffectFail::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddInt(m_nError);
    return true;
}

GameMsg_S2C_ReplaceClothEffectResult::GameMsg_S2C_ReplaceClothEffectResult()
    :GameMsg_Base(MSG_S2C_ReplaceClothEffectResult)
{

}

GameMsg_S2C_ReplaceClothEffectResult::~GameMsg_S2C_ReplaceClothEffectResult()
{

}

bool GameMsg_S2C_ReplaceClothEffectResult::doEncode( CParamPool &IOBuff )
{
    return true;
}

GameMsg_C2S_LevelUpEffect::GameMsg_C2S_LevelUpEffect()
    :GameMsg_Base(MSG_C2S_LevelUpEffect)
    ,m_nItemColumn(0)
    ,m_nItemSlot(0)
{

}

GameMsg_C2S_LevelUpEffect::~GameMsg_C2S_LevelUpEffect()
{

}

bool GameMsg_C2S_LevelUpEffect::doDecode( CParamPool &IOBuff )
{
    m_nItemColumn = IOBuff.GetUShort();
    m_nItemSlot   = IOBuff.GetUShort();
    return true;
}

GameMsg_S2C_LevelUpEffectFail::GameMsg_S2C_LevelUpEffectFail()
    :GameMsg_Base(MSG_S2C_LevelUpEffectFail)
{

}

GameMsg_S2C_LevelUpEffectFail::~GameMsg_S2C_LevelUpEffectFail()
{

}

bool GameMsg_S2C_LevelUpEffectFail::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddInt(m_nError);
    return true;
}

GameMsg_S2C_LevelUpEffectResult::GameMsg_S2C_LevelUpEffectResult()
    :GameMsg_Base(MSG_S2C_LevelUpEffectResult)
    ,m_nLevelUpEffectID(0)
    ,m_nItemtype(0)
{

}

GameMsg_S2C_LevelUpEffectResult::~GameMsg_S2C_LevelUpEffectResult()
{

}

bool GameMsg_S2C_LevelUpEffectResult::doEncode( CParamPool &IOBuff )
{
    IOBuff.AddUInt(m_nItemtype);
    IOBuff.AddUInt(m_nLevelUpEffectID);
    return true;
}

GameMsg_S2C_RefreshHandbookProgress::GameMsg_S2C_RefreshHandbookProgress()
    :GameMsg_Base(MSG_S2C_RefreshHandbookProgress)
    ,m_nSuitID(0)
    ,m_pHandbookProgress(NULL)
{

}

GameMsg_S2C_RefreshHandbookProgress::~GameMsg_S2C_RefreshHandbookProgress()
{

}

bool GameMsg_S2C_RefreshHandbookProgress::doEncode( CParamPool &IOBuff )
{
    if (m_pHandbookProgress != NULL)
    {
        IOBuff.AddUShort(m_nSuitID);
        m_pHandbookProgress->doEncode(IOBuff);
    }

    return true;
}

GameMsg_S2C_RefreshColorProgress::GameMsg_S2C_RefreshColorProgress()
    :GameMsg_Base(MSG_S2C_RefreshColorProgress)
    ,m_nSuitID(0)
    ,m_pSevenColorProgress(NULL)
{

}

GameMsg_S2C_RefreshColorProgress::~GameMsg_S2C_RefreshColorProgress()
{

}

bool GameMsg_S2C_RefreshColorProgress::doEncode( CParamPool &IOBuff )
{
    if (m_pSevenColorProgress != NULL)
    {
        IOBuff.AddUShort(m_nSuitID);
        m_pSevenColorProgress->doEncode(IOBuff);
    }

    return true;
}

GameMsg_C2S_MoveItem::GameMsg_C2S_MoveItem()
:GameMsg_Base(MSG_C2S_MoveItem)
{

}

GameMsg_C2S_MoveItem::~GameMsg_C2S_MoveItem()
{

}

bool GameMsg_C2S_MoveItem::doDecode(CParamPool &IOBuff)
{
    unsigned short nCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nCount; ++i)
    {
        itemgrid_t grid;
        grid.first = (int)IOBuff.GetUShort();
        grid.second = (int)IOBuff.GetUShort();
        m_listFromGrid.push_back(grid);
    }
    return true;
}

GameMsg_S2C_MoveItemFail::GameMsg_S2C_MoveItemFail()
:GameMsg_Base(MSG_S2C_MoveItemFail)
, m_nErrorFlag(0)
, m_nLeftCapacity(0)
{

}

GameMsg_S2C_MoveItemFail::~GameMsg_S2C_MoveItemFail()
{

}

bool GameMsg_S2C_MoveItemFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nErrorFlag);
    IOBuff.AddInt(m_nLeftCapacity);
    return true;
}

GameMsg_S2C_MoveItemScuess::GameMsg_S2C_MoveItemScuess()
:GameMsg_Base(MSG_S2C_MoveItemScuess)
{

}

GameMsg_S2C_MoveItemScuess::~GameMsg_S2C_MoveItemScuess()
{

}

bool GameMsg_S2C_MoveItemScuess::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listGrid.size());
    std::list<itemgrid_t>::iterator it = m_listGrid.begin();
    for (; it != m_listGrid.end(); ++it)
    {
        IOBuff.AddUShort((unsigned short)it->first);
        IOBuff.AddUShort((unsigned short)it->second);
    }
    return true;
}


GameMsg_C2S_AddStorageCapacity::GameMsg_C2S_AddStorageCapacity()
:GameMsg_Base(MSG_C2S_AddStorageCapacity)
{

}

GameMsg_C2S_AddStorageCapacity::~GameMsg_C2S_AddStorageCapacity()
{

}

bool GameMsg_C2S_AddStorageCapacity::doDecode(CParamPool &IOBuff)
{
    return true;
}

GameMsg_S2C_AddStorageCapacityFail::GameMsg_S2C_AddStorageCapacityFail()
:GameMsg_Base(MSG_S2C_AddStorageCapacityFail)
, m_nErrorFlag(0)
{

}

GameMsg_S2C_AddStorageCapacityFail::~GameMsg_S2C_AddStorageCapacityFail()
{

}

bool GameMsg_S2C_AddStorageCapacityFail::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nErrorFlag);
    return true;
}

GameMsg_S2C_AddStorageCapacityScuess::GameMsg_S2C_AddStorageCapacityScuess()
:GameMsg_Base(MSG_S2C_AddStorageCapacityScuess)
, m_nStorageCapacity(0)
{

}

GameMsg_S2C_AddStorageCapacityScuess::~GameMsg_S2C_AddStorageCapacityScuess()
{

}

bool GameMsg_S2C_AddStorageCapacityScuess::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nStorageCapacity);
    return true;
}

GameMsg_S2C_SyncBagConfig::GameMsg_S2C_SyncBagConfig()
:GameMsg_Base(MSG_S2C_SyncBagConfig)
, m_nBadgeCapacity(0)
, m_nStorageCapacity(0)
, m_strBagDescribe("")
, m_nAddStorageCapacityCurrencyType(0)
, m_nAddStorageCapacityCurrencyValue(0)
{
}

GameMsg_S2C_SyncBagConfig::~GameMsg_S2C_SyncBagConfig()
{

}

bool GameMsg_S2C_SyncBagConfig::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nBadgeCapacity);
    IOBuff.AddInt(m_nStorageCapacity);
    IOBuff.AddStringW(m_strBagDescribe);
    IOBuff.AddInt(m_nAddStorageCapacityCurrencyType);
    IOBuff.AddInt(m_nAddStorageCapacityCurrencyValue);
    return true;
}

GameMsg_S2C_NoticeStorageItemUsed::GameMsg_S2C_NoticeStorageItemUsed()
:GameMsg_Base(MSG_S2C_NoticeStorageItemUsed)
{
}

GameMsg_S2C_NoticeStorageItemUsed::~GameMsg_S2C_NoticeStorageItemUsed()
{

}

bool GameMsg_S2C_NoticeStorageItemUsed::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort((unsigned short)m_listUsedItem.size());
    std::list<CItem>::iterator it = m_listUsedItem.begin();
    for (; it != m_listUsedItem.end(); ++it)
    {
        it->doEncode(IOBuff);
    }
    return true;
}

GameMsg_S2C_NoticeItemInStorage::GameMsg_S2C_NoticeItemInStorage()
:GameMsg_Base(MSG_S2C_NoticeItemInStorage)
, m_nItemType(0)
{
}

GameMsg_S2C_NoticeItemInStorage::~GameMsg_S2C_NoticeItemInStorage()
{

}

bool GameMsg_S2C_NoticeItemInStorage::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nItemType);
    return true;
}


GameMsg_C2S_RequestGuideFinish::GameMsg_C2S_RequestGuideFinish()
:GameMsg_Base(MSG_C2S_RequestGuideFinish)
, m_nGuideEnum(0)
{
}

GameMsg_C2S_RequestGuideFinish::~GameMsg_C2S_RequestGuideFinish()
{

}

bool GameMsg_C2S_RequestGuideFinish::doDecode(CParamPool &IOBuff)
{
    m_nGuideEnum = IOBuff.GetUInt();
    return true;
}

GameMsg_S2C_RequestGuideFinishResult::GameMsg_S2C_RequestGuideFinishResult()
:GameMsg_Base(MSG_S2C_RequestGuideFinishResult)
, m_nGuideData(0)
{
}

GameMsg_S2C_RequestGuideFinishResult::~GameMsg_S2C_RequestGuideFinishResult()
{

}

bool GameMsg_S2C_RequestGuideFinishResult::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nGuideData);
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_RefreshPerpetualEquipCount::GameMsg_S2C_RefreshPerpetualEquipCount()
    : GameMsg_Base( MSG_S2C_RefreshPerpetualEquipCount )
    , m_nCount( 0 )
{
}

GameMsg_S2C_RefreshPerpetualEquipCount::~GameMsg_S2C_RefreshPerpetualEquipCount()
{
}

bool GameMsg_S2C_RefreshPerpetualEquipCount::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_nCount;
    return true;
}

GameMsg_S2C_RefreshPerpetualEffectScore::GameMsg_S2C_RefreshPerpetualEffectScore()
    : GameMsg_Base( MSG_S2C_RefreshPerpetualEffectScore )
    , m_nScore( 0 )
{
}

GameMsg_S2C_RefreshPerpetualEffectScore::~GameMsg_S2C_RefreshPerpetualEffectScore()
{
}

bool GameMsg_S2C_RefreshPerpetualEffectScore::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_nScore;
    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_SerializeItemInfo::GameMsg_S2C_SerializeItemInfo()
    : GameMsg_Base( MSG_S2C_SerializeItemInfo )
    , m_bEnd( false )
    , m_nBuffSize( 0 )
{
}

GameMsg_S2C_SerializeItemInfo::~GameMsg_S2C_SerializeItemInfo()
{
}

bool GameMsg_S2C_SerializeItemInfo::doEncode( CParamPool &IOBuff )
{
    IOBuff << m_bEnd << m_nBuffSize;
    IOBuff.AddMemory( m_pBuff, m_nBuffSize );

    return true;
}

