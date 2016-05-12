#ifndef	__GAMEMSG_S2C_GET_GIFT_RESULT_H__
#define	__GAMEMSG_S2C_GET_GIFT_RESULT_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include <list>

class CParamPool;

enum EGetGiftResult
{
	EGetGiftResult_Success,			//成功
	EGetGiftResult_Error,			//错误
	EGetGiftResult_DoNotHave,		//没有该礼品
	EGetGiftResult_Expired,			//礼品过期
	EGetGiftResult_GiftEmpty,		//礼品为空
};
class GameMsg_S2C_GetGiftResult : public GameMsg_Base
{
public:
	GameMsg_S2C_GetGiftResult(int nIndexID, int nResult);
	virtual ~GameMsg_S2C_GetGiftResult();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	int					m_nIndexID;		//编号
	int					m_nResult;		//结果
};

#endif
// end of file


