#ifndef __GAMEMSG_ACCOUNT_CREATEACCOUNT_H__
#define __GAMEMSG_ACCOUNT_CREATEACCOUNT_H__

#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Base.h"
#include <string>

using namespace std;

class CParamPool;

class GameMsg_Account_CreateAccount :	// 该消息在gate server被截取，添加gatesession，打包顺序被修改。
	public GameMsg_Base
{
public:
	unsigned char nVersionType;
	__int64 nGateSessionID;
	unsigned char nSex;//性别保留
	char szName[MAX_NAME_LEN];		//MAX_NAME_LEN
	char szPass[MAX_MD5_LEN] ;
	char szIDCard[MAX_IDCARD_LEN];	//最长身份证长度为18+1个字节		MAX_IDCARD_LEN
	char szLoginIp[MAX_IP_LEN];
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Account_CreateAccount);
	GameMsg_Account_CreateAccount();
	~GameMsg_Account_CreateAccount();

public:
	virtual bool doDecode(CParamPool &IOBuff);
};

#endif


//end file


