#include "../../socket/Utility.h"
#include "GameMsg_Account_CheckActivate.h"
#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Base.h"
#include "AccountMsgDef.h"


GameMsg_Account_CheckActivate::GameMsg_Account_CheckActivate():
GameMsg_Base(MSG_ACCOUNT_CheckActivate)
{
	memset(m_szActivateCode, 0, MAX_ACTIVATE_CODE);
	m_nLine = 0;
	m_strPUID = "";
}

GameMsg_Account_CheckActivate::~GameMsg_Account_CheckActivate()
{
}

bool GameMsg_Account_CheckActivate::doDecode(CParamPool &IOBuff)
{
	string strTemp;
	IOBuff.GetStringW(strTemp);
	safememcpy(m_szActivateCode, MAX_ACTIVATE_CODE, strTemp.c_str(), (unsigned int)strTemp.size());
	m_nLine = IOBuff.GetInt();
	IOBuff.GetStringW(m_strPUID);
	if (m_strPUID.size() > MAX_PUID_LEN)
	{
		m_strPUID.resize(MAX_PUID_LEN);
	}

	return true;
}




//end file


