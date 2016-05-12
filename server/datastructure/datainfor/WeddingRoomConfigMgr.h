/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\WeddingRoomConfigMgr
    author:     

    purpose:    对创建婚房配置数据进行管理
*********************************************************************/

#ifndef __WEDDINGROOMCONFIGMGR_H_
#define __WEDDINGROOMCONFIGMGR_H_

#include <vector>
#include <map>
#include "./WeddingRoomConfig.h"

//////////////////////////////////////////////////////////////////////////
///婚房配置数据管理器
//////////////////////////////////////////////////////////////////////////
class WeddingRoomConfigMgr
{
public:
	WeddingRoomConfigMgr() ;
	virtual ~WeddingRoomConfigMgr() ;

public:
	//释放所有对象
	virtual void Destroy();

    // 加载婚房配置
	bool LoadWeddingRoomConfig(const char* pszFileName);

    // 获取创建婚房消耗物品
	void GetWeddingCostItemInfo(unsigned short nTypeID, itemtype_t & nItemID, unsigned short & nCount);

private:
	std::map<unsigned short,WeddingRoomConfig> m_WeddingRoomConfig ;
};

#endif
