#include "GameMsg_Player_CreateRole.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"
#include "../../datastructure/datainfor/ItemType.h"
#include "../../socket/Utility.h"
#include "../../datastructure/datainfor/ServerConfig.h"

GameMsg_Player_CreateRole::GameMsg_Player_CreateRole():GameMsg_Base(MSG_C2S_CreateRole)
{
	nInstallerId = 0;
	m_nVersionType = 0;
	memset(m_szRoleName, 0, MAX_NAME_LEN);
	m_nSex = ESexType_Male;
	memset(m_szSignature, 0 , MAX_SIGNATURE_LEN);
	memset(m_szYear, 0, MAX_YEAR_LEN);	
	memset(m_szMonth, 0, MAX_MONTH_LEN); 	
	memset(m_szDay, 0, MAX_DAY_LEN); 		
	m_nColor = BODY_COLOR_YELLOW;
}

GameMsg_Player_CreateRole::~GameMsg_Player_CreateRole()
{
}

bool GameMsg_Player_CreateRole::doDecode(CParamPool &IOBuff)
{	
	nInstallerId = IOBuff.GetUChar();
	m_nVersionType = IOBuff.GetUChar();

	string strTemp;

	IOBuff.GetStringW(strTemp);
	CopyString2Buffer(strTemp,m_szRoleName,g_ServerConfig.m_MaxNameLimit-1);

	m_nSex = IOBuff.GetUChar();

	IOBuff.GetStringW(strTemp);
	CopyString2Buffer(strTemp,m_szSignature,g_ServerConfig.m_SignatureLength-1);

	IOBuff.GetStringW(strTemp);
	CopyString2Buffer(strTemp,m_szYear,MAX_YEAR_LEN-1);

	IOBuff.GetStringW(strTemp);
	CopyString2Buffer(strTemp,m_szMonth,MAX_MONTH_LEN-1);

	IOBuff.GetStringW(strTemp);
	CopyString2Buffer(strTemp,m_szDay,MAX_DAY_LEN-1);
	
	m_nColor = IOBuff.GetUChar();

	return true;
}



//file end

