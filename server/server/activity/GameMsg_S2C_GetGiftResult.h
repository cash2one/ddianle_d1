#ifndef	__GAMEMSG_S2C_GET_GIFT_RESULT_H__
#define	__GAMEMSG_S2C_GET_GIFT_RESULT_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include <list>

class CParamPool;

enum EGetGiftResult
{
	EGetGiftResult_Success,			//�ɹ�
	EGetGiftResult_Error,			//����
	EGetGiftResult_DoNotHave,		//û�и���Ʒ
	EGetGiftResult_Expired,			//��Ʒ����
	EGetGiftResult_GiftEmpty,		//��ƷΪ��
};
class GameMsg_S2C_GetGiftResult : public GameMsg_Base
{
public:
	GameMsg_S2C_GetGiftResult(int nIndexID, int nResult);
	virtual ~GameMsg_S2C_GetGiftResult();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	int					m_nIndexID;		//���
	int					m_nResult;		//���
};

#endif
// end of file


