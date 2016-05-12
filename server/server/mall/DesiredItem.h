#ifndef __WISHITEM_H__
#define __DESIREDITEM_H__

#include <map>
#include "../../datastructure/DataStruct_Base.h"

class DesiredItem
{
public:
    DesiredItem();
    virtual ~DesiredItem();

public:
    // 初始化
    void Initialize(const std::list<itemtype_t> & listDesire);

    // 是否满了
    bool IsFull();

    // 是否存在
    bool IsExist(itemtype_t nItemType);

    // 添加
    int AddDesiredItem(itemtype_t nItemType);

    // 删除
    bool RemoveDesiredItem(itemtype_t nItemType);

	// 获取列表
	void GetListOfDesiredItem(std::list<itemtype_t> & listItemType) const;

public:
    unsigned int                   m_nMaxWishItemCount;
    std::list<itemtype_t>          m_listDesiredItem;

    
};

#endif
