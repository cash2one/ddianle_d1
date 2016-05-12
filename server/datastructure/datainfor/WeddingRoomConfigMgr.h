/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\WeddingRoomConfigMgr
    author:     

    purpose:    �Դ����鷿�������ݽ��й���
*********************************************************************/

#ifndef __WEDDINGROOMCONFIGMGR_H_
#define __WEDDINGROOMCONFIGMGR_H_

#include <vector>
#include <map>
#include "./WeddingRoomConfig.h"

//////////////////////////////////////////////////////////////////////////
///�鷿�������ݹ�����
//////////////////////////////////////////////////////////////////////////
class WeddingRoomConfigMgr
{
public:
	WeddingRoomConfigMgr() ;
	virtual ~WeddingRoomConfigMgr() ;

public:
	//�ͷ����ж���
	virtual void Destroy();

    // ���ػ鷿����
	bool LoadWeddingRoomConfig(const char* pszFileName);

    // ��ȡ�����鷿������Ʒ
	void GetWeddingCostItemInfo(unsigned short nTypeID, itemtype_t & nItemID, unsigned short & nCount);

private:
	std::map<unsigned short,WeddingRoomConfig> m_WeddingRoomConfig ;
};

#endif
