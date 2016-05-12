#include "StdAfx.h"
#include "LogicInitData.h"
#include "../socket/Ini.h"
#include "../socket/Log.h"
#include "../socket/Formatter.h"
#include "../socket/Windefine.h"
#include "datainfor/ItemType.h"
#include "datainfor/ConfigManager.h"

#include <limits.h>
#include <stdlib.h>

#ifdef WIN32
#pragma warning(disable:4061)
#endif

extern string g_sPath;

#define BASE_ITEM_CAPACITY		120	//物品栏的容量
#define MAX_EXPAND_NUM		14	//物品栏的容量

#define MAX_TASK_COUNT		16	//角色可同时接受的最大任务数

#define MAX_BUF	256 // to read ini

CLogicInitData::CLogicInitData()
{
	//[级别]
	m_nMaxLevel = 0;
	m_anExpNeed = NULL;

	m_anItemMaxNumber[EItemColumn_ClothBag]    = BASE_ITEM_CAPACITY;
	m_anItemMaxNumber[EItemColumn_ExpandableBag]      = BASE_ITEM_CAPACITY;
	m_anItemMaxNumber[EItemColumn_BadgeBag]           = BASE_ITEM_CAPACITY;
	m_anItemMaxNumber[EItemColumn_WishList]          = BASE_ITEM_CAPACITY;
	
	m_anItemMaxNumber[EItemColumn_BadgeOnPlayer]        = 5;
	m_anItemMaxNumber[EItemColumn_ClothOnPlayer] = eItemClothType_Max;
	

	m_anItemExpandNumber[EItemColumn_ClothBag]    = 0;
	m_anItemExpandNumber[EItemColumn_ExpandableBag]      = 0;
	m_anItemExpandNumber[EItemColumn_BadgeBag]           = 0;
    m_anItemExpandNumber[EItemColumn_WishList]          = 0;

	m_anItemExpandNumber[EItemColumn_BadgeOnPlayer]        = 0;
	m_anItemExpandNumber[EItemColumn_ClothOnPlayer] = 0;
	m_anItemExpandNumber[EItemColumn_WeddingRingOnPlayer]        = 0;

	m_nAutoTimeMin = 300*1000;
    m_nAutoTimeMax = 500 * 1000;

	m_nExpGain = 100;
	m_nMoneyGain = 100;
	m_nItemDropGain = 100;
}


CLogicInitData::~CLogicInitData()
{
	if (m_anExpNeed != NULL)
	{
		delete []m_anExpNeed;
		m_anExpNeed = NULL;
	}
}

CLogicInitData& CLogicInitData::Instance()
{
	static CLogicInitData stcLogicData;
	return stcLogicData;
}

bool CLogicInitData::InitAllData()
{
	CIni fileReadIni;

	extern CIni g_Config;
	char szFileName[MAX_PATH];
	g_Config.GetString("GameServer", "LogicParamFile",szFileName, MAX_PATH, "data/logicparam.ini");
	if(fileReadIni.Load((g_sPath + szFileName).c_str()) == false)
	{
		return false;
	};

	//[升级]
	m_nMaxLevel		= (unsigned short)fileReadIni.GetInt("Upgrade","The_Maximum_Level",100);
	
	//人物经验读取静态数据表
	m_anExpNeed = new int[m_nMaxLevel];
	memset(m_anExpNeed, 0 , sizeof(int)*m_nMaxLevel);
	for (int i = 0 ; i < m_nMaxLevel ; i++)
	{
		int nExp = ConfigManager::Instance().GetExpMgr().GetByLev((unsigned short)(i+1)) ;
		if(nExp >= 0)
		{
			m_anExpNeed[i] = nExp;
		}
		else
		{
			m_anExpNeed[i] = 0;
			WriteLog(LOGLEVEL_WARNING, "WARNING: Level[%d] Exp[%d] < 0", i+1, nExp);
		}
	}
	
	m_nAutoTimeMin			= fileReadIni.GetInt("Common_Parameters","Auto_Save_Time_Min",600);	//自动保存时间
	if(m_nAutoTimeMin<60)m_nAutoTimeMin=60;
	m_nAutoTimeMin *= 1000;

    m_nAutoTimeMax = fileReadIni.GetInt("Common_Parameters", "Auto_Save_Time_Max", 1800);	//自动保存时间
    if (m_nAutoTimeMax < 100)m_nAutoTimeMax = 100;
    m_nAutoTimeMax *= 1000;


	m_nExpGain	= (unsigned short)fileReadIni.GetInt("Common_Parameters","Experience_Multiple",100);	//经验倍数
	m_nMoneyGain	= (unsigned short)fileReadIni.GetInt("Common_Parameters","Money_Multiplier",100);
	m_nItemDropGain	= (unsigned short)fileReadIni.GetInt("Common_Parameters","Loot_Ratio",100);

	std::string strBuff;

	int nItemSendNum = fileReadIni.GetInt("Male_Initial_Item","Item_Num",6);
	unsigned int nItemType = 0; 
	unsigned int nItemNum = 0;
	for(int i=1;i<=nItemSendNum;i++)
	{
		Formatter(strBuff) << "Item" << i;
		nItemType = (unsigned int)fileReadIni.GetInt("Male_Initial_Item", strBuff.c_str(), 0);
		Formatter(strBuff) << "Item" << i << "Count";
		nItemNum = (unsigned int)fileReadIni.GetInt("Male_Initial_Item", strBuff.c_str(), 0);
		if( nItemType>0 && nItemNum>0)
			m_aInitItem.push_back(make_pair(nItemType, nItemNum));
	}

	int nItemSendNum2 = fileReadIni.GetInt("Female_Initial_Item","Item_Num",6);
	unsigned int nItemType2 = 0;
	unsigned int nItemNum2= 0;
	for(int i=1;i<=nItemSendNum2;i++)
	{
		Formatter(strBuff) << "Item" << i;
		nItemType2 = (unsigned int)fileReadIni.GetInt("Female_Initial_Item", strBuff.c_str(), 0);
		Formatter(strBuff) << "Item" << i << "Count";
		nItemNum2 = (unsigned int)fileReadIni.GetInt("Female_Initial_Item", strBuff.c_str(), 0);
		if( nItemType2>0 && nItemNum2>0)
			m_aInitItem2.push_back(make_pair(nItemType2, nItemNum2));
	}

	strBuff = "Clothing_Column_Limit";
	m_anItemMaxNumber[EItemColumn_ClothBag] = (unsigned short)fileReadIni.GetInt("Items_Column", strBuff.c_str(), BASE_ITEM_CAPACITY);
	strBuff = "Consumables_Column_Limit";
	m_anItemMaxNumber[EItemColumn_ExpandableBag] = (unsigned short)fileReadIni.GetInt("Items_Column", strBuff.c_str(), BASE_ITEM_CAPACITY);
	strBuff = "Cap_Badge_Bar_Limit";
	m_anItemMaxNumber[EItemColumn_BadgeBag] = (unsigned short)fileReadIni.GetInt("Items_Column", strBuff.c_str(), BASE_ITEM_CAPACITY);
	strBuff = "Desire_To_Limit";
	m_anItemMaxNumber[EItemColumn_WishList] = (unsigned short)fileReadIni.GetInt("Items_Column", strBuff.c_str(), BASE_ITEM_CAPACITY);

	strBuff = "Extended_Ceiling_Bar_Limit";
	unsigned short nMaxNumber = (unsigned short)fileReadIni.GetInt("Items_Column", strBuff.c_str(), MAX_EXPAND_NUM);
	m_anItemExpandNumber[EItemColumn_ClothBag] = nMaxNumber;
	m_anItemExpandNumber[EItemColumn_ExpandableBag] = nMaxNumber;
	m_anItemExpandNumber[EItemColumn_BadgeBag] = nMaxNumber;
	m_anItemExpandNumber[EItemColumn_WishList] = nMaxNumber;

	//[聊天]
	m_nDefaultColor[0] = (unsigned short)fileReadIni.GetInt("Chat","Nonroom_Chat",0xffff);
	m_nDefaultColor[1] = (unsigned short)fileReadIni.GetInt("Chat","Room_Chat",0);

	m_DefaultColth[0][0] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Boy_hair",0);
	m_DefaultColth[0][1] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Boy_face",0);
	m_DefaultColth[0][2] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Boy_clothes",0);
	m_DefaultColth[0][3] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Boy_leg",0);
	m_DefaultColth[0][4] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Boy_shoes",0);
	m_DefaultColth[0][5] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Boy_gloves",0);
	m_DefaultColth[0][6] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Boy_color_1",0);
	m_DefaultColth[0][7] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Boy_color_2",0);
	m_DefaultColth[0][8] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Boy_color_3",0);

	m_DefaultColth[1][0] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Girl_hair",0);
	m_DefaultColth[1][1] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Girl_face",0);
	m_DefaultColth[1][2] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Girl_clothes",0);
	m_DefaultColth[1][3] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Girl_leg",0);
	m_DefaultColth[1][4] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Girl_shoes",0);
	m_DefaultColth[1][5] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Girl_gloves",0);
	m_DefaultColth[1][6] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Girl_color_1",0);
	m_DefaultColth[1][7] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Girl_color_2",0);
	m_DefaultColth[1][8] =  (unsigned int)fileReadIni.GetInt("Default_Clothing","Girl_color_3",0);

	return true;
};

int CLogicInitData::GetExpNeed(unsigned short nCurLevel)const
{
	if(nCurLevel<m_nMaxLevel)
	{
		int nTempLevel = nCurLevel;
		if (nCurLevel > 0)
			nTempLevel = nCurLevel - 1;
		return m_anExpNeed[nTempLevel];
	}
	else
		return INT_MAX;
};

unsigned int CLogicInitData::GetColumnMaxSlotNum(EBagColumn eColumn)
{
	switch (eColumn)
	{
	case EBagColumn_ClothBag:return m_anItemMaxNumber[EItemColumn_ClothBag];
	case EBagColumn_ExpandableBag:return (unsigned int)m_anItemMaxNumber[EItemColumn_ExpandableBag];
	case EBagColumn_BadgeBag:return (unsigned int)m_anItemMaxNumber[EItemColumn_BadgeBag];
	default:
		break;
	}

	return 0;
}

unsigned int CLogicInitData::GetColumnMaxSlotNum(EEquipColumn eColumn)
{
	switch (eColumn)
	{
	case EEquipColumn_BadgeOnPlayer:return m_anItemMaxNumber[EItemColumn_BadgeBag];
	case EEquipColumn_ClothOnPlayer:return (unsigned int)eItemClothType_Max;
	case EEquipColumn_TransformClothOnPlayer:return (unsigned int)eItemClothType_Max;
	case EEquipColumn_WeddingRingOnPlayer:return WeddingRing_BAG_MAXNUM;
	default:
		break;
	}

	return 0;
}

unsigned int CLogicInitData::GetDesireMaxSize()
{
    return (unsigned int)m_anItemMaxNumber[EItemColumn_WishList];
}

unsigned int CLogicInitData::GetColumnMaxSlotNum(EItemColumn eColumn)
{
    switch (eColumn)
    {
    case EItemColumn_ClothBag:
    case EItemColumn_ExpandableBag:
    case EItemColumn_BadgeBag:
    case EItemColumn_WishList:
    case EItemColumn_BadgeOnPlayer:
        {
            return (unsigned int)m_anItemMaxNumber[eColumn];
        }
    case EItemColumn_WeddingRingOnPlayer:
        {
            return WeddingRing_BAG_MAXNUM;
        }
    case  EItemColumn_TransformClothOnPlayer:
    case  EItemColumn_ClothOnPlayer:
    case  EItemColumn_DefaultClothOnPlayer:
        {
            return eItemClothType_Max;
        }
    default:
        break;
    }

    return 0;
}

void CLogicInitData::SetGainPer(unsigned short nGain,char nType)
{
	if (nGain%100 != 0)
	{
		return;
	}

	switch(nType)
	{
	case SET_EXP_GAIN:
		m_nExpGain = nGain;
		break;
	case SET_MONEY_GAIN:
		m_nMoneyGain = nGain;
		break;
	case SET_ITEMDROP_GAIN:
		m_nItemDropGain = nGain;
		break;
	case SET_ALL_GAIN:
		{
			m_nExpGain = nGain;
			m_nMoneyGain = nGain;
			m_nItemDropGain = nGain;
		}
		break;
	default:
		break;
	}
};


void CLogicInitData::AddGainPer(short nGain,char nType)
{
	if (nGain%100 != 0)
		return;

	switch(nType)
	{
	case SET_EXP_GAIN:
		_SetParam(m_nExpGain,nGain);
		break;
	case SET_MONEY_GAIN:
		_SetParam(m_nMoneyGain,nGain);
		break;
	case SET_ITEMDROP_GAIN:
		_SetParam(m_nItemDropGain,nGain);
		break;
	case SET_ALL_GAIN:
		{
			_SetParam(m_nExpGain,nGain);
			_SetParam(m_nMoneyGain,nGain);
			_SetParam(m_nItemDropGain,nGain);
		}
		break;
	default:
		break;
	}
};

void CLogicInitData::_SetParam(unsigned short & nOriGain,short nGain)
{
	if (nGain < 0)
	{
		if (nOriGain > abs(nGain))
		{
			nOriGain = nOriGain - (unsigned short)abs(nGain);
			if (nOriGain < 100)
				nOriGain = 100;
		}
		else
			nOriGain = 100;
	}
	else
		nOriGain = nOriGain + nGain;
};
unsigned int CLogicInitData::GetDefaultColth( bool bMan ,unsigned int nPart )
{
	if(  nPart < DEFUALT_COLTH_NUM )
	{
		if(bMan)
		{
			return m_DefaultColth[0][nPart];
		}
		else
		{
			return m_DefaultColth[1][nPart];

		}
	}
	return 0;
}

bool CLogicInitData::IsDefaultColth(unsigned int nTypeID) const
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < DEFUALT_COLTH_NUM; ++j)
        {
            if (m_DefaultColth[i][j] == nTypeID)
                return true;
        }
    }
    return false;
}

//end



