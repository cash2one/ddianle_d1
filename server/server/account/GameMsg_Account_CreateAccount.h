#ifndef __GAMEMSG_ACCOUNT_CREATEACCOUNT_H__
#define __GAMEMSG_ACCOUNT_CREATEACCOUNT_H__

#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Base.h"
#include <string>

using namespace std;

class CParamPool;

class GameMsg_Account_CreateAccount :	// ����Ϣ��gate server����ȡ�����gatesession�����˳���޸ġ�
	public GameMsg_Base
{
public:
	unsigned char nVersionType;
	__int64 nGateSessionID;
	unsigned char nSex;//�Ա���
	char szName[MAX_NAME_LEN];		//MAX_NAME_LEN
	char szPass[MAX_MD5_LEN] ;
	char szIDCard[MAX_IDCARD_LEN];	//����֤����Ϊ18+1���ֽ�		MAX_IDCARD_LEN
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


