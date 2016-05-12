#include "StdAfx.h"		//因为需要加在client项目中,故需要
#include "DataStruct_Game.h"
#include "EquipItem.h"
#include "RoleEntity.h"
#include "datainfor/ItemConfig.h"
#include "../server/libServerFrame/QueryManager.h"
#include "datainfor/ConfigManager.h"
#include "../server/lua/InitLua.h"
#include "../server/util/CommonFunc.h"

extern CQueryManager* g_pQueryMgr;


EItemColumn ItemType2Column(ItemConfig* pItemInfo)
{
	EItemColumn nColumn = EItemColumn_Invalid;
	if(pItemInfo != NULL)
	{
		switch(pItemInfo->m_anType[0])
		{
		case eItemClassType_Equip:
			{	
				if (pItemInfo->IsCloth())
				{
					nColumn = EItemColumn_ClothBag;
				}
				else if (pItemInfo->IsBadge())
				{
					nColumn = EItemColumn_BadgeBag;
				}
                else if (pItemInfo->IsHorse() )
                {
                    nColumn = EItemColumn_ClothBag;
                }
			}
			break;
		case eItemClassType_Expendable:
			nColumn = EItemColumn_ExpandableBag;
			break;
		default:
			ASSERT(0);
			break;
		}
	}
	return nColumn;
}

EItemColumn ItemType2Column(ItemBase* pItem)
{
    ItemConfig * pItemConfig = ConfigManager::Instance().GetItemConfigManager().GetByID(pItem->GetType());
    if (pItemConfig != NULL)
    {
        return pItemConfig->GetItemColumnByType();
    }

    return EItemColumn_Invalid;
}

EItemColumn ItemType2Column(itemtype_t nItemType)
{
	ItemConfig* pItemInfo = ConfigManager::Instance().GetItemConfigManager().GetByID(nItemType);
	return ItemType2Column(pItemInfo);
}

