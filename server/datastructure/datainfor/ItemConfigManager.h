/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\ItemConfigManager.h
    author:     

    purpose:    对物品配置数据进行管理
*********************************************************************/

#ifndef __ITEMCONFIGMANAGER_H_
#define __ITEMCONFIGMANAGER_H_

#include <vector>
#include <map>
#include "./ItemConfig.h"
#include "../DataStruct_Base.h"


const int C_EXTEND_ITEM_TYPEID_BEGIN = 60000;
const int C_EXTEND_ITEM_TYPEID_END = 64999;
const int C_EXTEND_ITEM_DATA_SIZE = 64000;	//只发一条消息，确保该数值不会大于最大消息包的大小(该值不包含其他数据)

class ItemConfigManager
{
public:
	ItemConfigManager(void);
	virtual ~ItemConfigManager(void);

	//从文件里读取所有unit的信息
	bool LoadItemInfo(const char* pszFileName);
	//把所有基因数据存到一个文件
	bool SaveItemInfo(const char* pszFileName);

	bool LoadExtendItemBinData(const char* pszFileName);
	bool LoadExtendItemInfo(const char* pszFileName);
	bool SaveExtendItemInfo(const char* pszFileName);

	// Add iteminfo;
	bool Add(ItemConfig* pItemInfo);
	bool AddExtendItem(ItemConfig* pItemInfo);

	unsigned short GetNumber() const
	{
		return (unsigned short )m_ItemMap.size();
	}

	//根据ID得到对应的CUintInfor
	ItemConfig * GetByID(itemtype_t nType)const;

    // 根据ID得到所属栏位
    EItemColumn GetColumnByItemType(itemtype_t nType);

	// 加入肤色数据 
	bool AddSkinInfo(CSkinInfo* pInfo);

	// 根据物品ID得到肤色 
	CSkinInfo* GetSkinInfoByID(itemtype_t nType) const;

	//额外的对外接口
	void GetItemByClass(vector<ItemConfig*>& apItemInfo,unsigned char nClassType1,unsigned char nClassType2,unsigned char nClassType3)const;
	bool IsEquip(itemtype_t nItemType)const;
	bool IsExpandable(itemtype_t nItemType)const;

	//服饰动画
	CItemAniInfo* GetItemAniByID(unsigned short nType) const;
	bool AddItemAniInfo(CItemAniInfo* pInfo);

	int ExtendItemBinDataSize();
	const char * ExtendItemBinData();

	//变身
	CItemTransformInfo* GetTransformByID(unsigned int nId) const;
	bool AddItemTransformInfo(CItemTransformInfo* pInfo);
	bool LoadTransformInfo(const char* pszFileName);
	bool SaveTransformInfo(const char* pszFileName);

	EItemClassType GetGeneItemClass(unsigned short nGeneID);

	//释放所有对象
	virtual void Destroy();

private:
	//物品数据存放
	std::map<itemtype_t, ItemConfig*> m_ItemMap;
	int m_nExtendItemDataSize;
	char m_szExtendItemBinData[C_EXTEND_ITEM_DATA_SIZE];
	std::map<itemtype_t, ItemConfig*> m_ExtendItemMap;

	// 肤色数据存放
	std::map<itemtype_t, CSkinInfo* > m_mapSkinInfo;
	//服饰动画存放
	std::map<unsigned short, CItemAniInfo*> m_mapItemAniMap;
	//变身表
	std::map<unsigned int, CItemTransformInfo*> m_mapTransformInfo;
    //基因id对应的道具大类
    std::map<int, EItemClassType> m_mapGene2ItemClass;
};


#endif//__iteminfoMANAGER_H_



//end file


