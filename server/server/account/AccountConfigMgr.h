/*
 * AccountConfigMgr.h
 * Description: accountServer使用的配置信息
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: Liguanghao
 * Create time: 2015-07-20 17:14:50
 */

#ifndef __ACCOUNTCONFIGMGR_H__
#define __ACCOUNTCONFIGMGR_H__

#include <list>
#include <string>
#include "../../datastructure/DataStruct_Base.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/GameMsg_Processor.h"

class CRoleEntity;

class AccountConfigMgr
{
private:
	AccountConfigMgr();
	~AccountConfigMgr();
public:
	static AccountConfigMgr & Instance();

	void InitForbiddenRegPlatform(std::string strForbidden); // p1|p2|p3
	bool IsForbiddenRegPlatform(unsigned short platformId); 

public:

	std::vector<unsigned short> m_vecForbiddenRegPlatform; // 禁止注册的平台

};

#endif

