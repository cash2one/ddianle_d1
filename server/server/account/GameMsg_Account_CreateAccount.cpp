#include "GameMsg_Account_CreateAccount.h"
#include "../../socket/ParamPool.h"
#include "../../socket/Utility.h"
#include "AccountMsgDef.h"
#include "../../datastructure/datainfor/ItemType.h"

GameMsg_Account_CreateAccount::GameMsg_Account_CreateAccount():GameMsg_Base(MSG_ACCOUNT_CreateAccount)
{
	nVersionType = 0;
	nGateSessionID = 0;
	nSex = ESexType_Male;
	memset(szName , 0 , MAX_NAME_LEN) ;
	memset(szPass , 0 , sizeof(szPass)) ;
	memset(szIDCard , 0 , MAX_IDCARD_LEN) ;
	memset(szLoginIp, 0, sizeof(szLoginIp));
}

GameMsg_Account_CreateAccount::~GameMsg_Account_CreateAccount()
{
}

bool GameMsg_Account_CreateAccount::doDecode(CParamPool &IOBuff)
{
	nVersionType = IOBuff.GetUChar();
	nGateSessionID = IOBuff.GetInt64();

	string strTemp;
	IOBuff.GetStringW(strTemp);
	safememcpy(szName, MAX_NAME_LEN, strTemp.c_str(), (unsigned int)strTemp.size());

	IOBuff.GetMemory(szPass , MAX_MD5_LEN - 1) ;

	IOBuff.GetStringW(strTemp);
	safememcpy(szIDCard, MAX_IDCARD_LEN, strTemp.c_str(), (unsigned int)strTemp.size());
	nSex = IOBuff.GetUChar();

	IOBuff.GetStringW(strTemp);
	safememcpy(szLoginIp, MAX_IP_LEN-1, strTemp.c_str(), (unsigned int)strTemp.length());

	return true;
}

//end file





