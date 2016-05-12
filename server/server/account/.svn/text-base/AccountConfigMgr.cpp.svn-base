#include <algorithm>
#include "AccountConfigMgr.h"
// #include "../ItemProcess.h"
// #include "../../datastructure/RoleEntity.h"
#include "../../socket/Log.h"
// #include "../EntityAttributeComponent.h"

#include "../util/CommonFunc.h"
#include "../SystemSetting/SystemSettingMgr.h"



AccountConfigMgr::AccountConfigMgr()
{
}

AccountConfigMgr::~AccountConfigMgr()
{
}

AccountConfigMgr & AccountConfigMgr::Instance()
{
	static AccountConfigMgr s_accountConfigMgr;

	return s_accountConfigMgr;
}

void AccountConfigMgr::InitForbiddenRegPlatform(std::string strForbidden)
{
	// ½âÎö
	std::vector<int> vecIntTemp;
	SplitString(strForbidden, "|", vecIntTemp);

	m_vecForbiddenRegPlatform.clear(); 
	for (unsigned i = 0; i < vecIntTemp.size(); ++i)
	{
		m_vecForbiddenRegPlatform.push_back((unsigned short)vecIntTemp[i]);
	}

	return ;
}

bool AccountConfigMgr::IsForbiddenRegPlatform(unsigned short platformId)
{
	return std::find(m_vecForbiddenRegPlatform.begin(), m_vecForbiddenRegPlatform.end(), platformId) 
		!= m_vecForbiddenRegPlatform.end();
}



// end.
