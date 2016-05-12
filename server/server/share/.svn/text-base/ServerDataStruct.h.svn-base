#ifndef __SERVER_COMMON_DATASTRUCTURE_H__
#define __SERVER_COMMON_DATASTRUCTURE_H__

#include "../../socket/MsgBaseStruct.h"
#include "ServerMsgDef.h"
#include <memory.h>

//各Server识别角色的标记，这个标记包含该客户在LinkSvr，LogicSvr中的各种索引
typedef struct PLAYERTAG
{
	unsigned int   m_nAccount;		//账号ID,数据库中存储的主键 ，唯一， m_mapSlot主键
	int            m_nVersionType;	//版本类型
	__int64        m_nSessionID;	//SessionID
	unsigned int   m_nRoleID;		//角色id
	unsigned short m_nSlot;		    //GateServer连接GameServer的slot
	unsigned short m_nIndexLogic;   //在LogicServer的playermanager数组中的Index
	
	//unsigned long m_tickLogin;
	PLAYERTAG()
	{
		Clear();
	}
	void Clear()
	{
		memset(this, 0, sizeof(PLAYERTAG));
	}

}*LPPLAYERTAG;



class CSession
{
//public:
//	CSession();
//	~CSession();
//private:
//	char m_szSession[MAX_SESSION_LEN];
//	char m_szSessionChar[MAX_SESSION_STR_LEN];	
//	__int64 m_nSesssionID;
//public:
//	char *GetSession();
//	char *GetSessionChar();
//	__int64 GetSessionNum();
};

// 货币类型枚举
enum ECurrencyType
{
    ECurrencyType_Unknown = 0,      // 未知货币类型

    ECurrencyType_USD = 1,          // 美元
    ECurrencyType_CAD = 2,          // 加拿大元
    ECurrencyType_MXN = 3,
    ECurrencyType_AUD = 4,
    ECurrencyType_NZD = 5,
    ECurrencyType_JPY = 6,          // 日元
    ECurrencyType_CNY = 7,          // 人民币
    ECurrencyType_SGD = 8,
    ECurrencyType_HKD = 9,          // 港币
    ECurrencyType_TWD = 10,         // 台币
    ECurrencyType_IDR = 11,
    ECurrencyType_INR = 12,
    ECurrencyType_RUB = 13,
    ECurrencyType_TRY = 14,
    ECurrencyType_ILS = 15,
    ECurrencyType_ZAR = 16,
    ECurrencyType_SAR = 17,
    ECurrencyType_AED = 18,
    ECurrencyType_GBP = 19,
    ECurrencyType_DKK = 20,
    ECurrencyType_SEK = 21,
    ECurrencyType_CHF = 22,
    ECurrencyType_NOK = 23,
    ECurrencyType_Euro = 24,
    ECurrencyType_EUR = 25,

    ECurrencyType_Max = 32,
};


#endif//__SERVER_COMMON_DATASTRUCTURE_H__



//file end

