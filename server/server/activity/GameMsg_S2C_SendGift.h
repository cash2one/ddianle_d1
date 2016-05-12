#ifndef	__GAMEMSG_S2C_SEND_GIFT_H__
#define	__GAMEMSG_S2C_SEND_GIFT_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include <list>

class CParamPool;

class GameMsg_S2C_SendGift: public GameMsg_Base
{
public:
	GameMsg_S2C_SendGift(int nGiftID, std::list< CItem > & rRewardList, int nRewardMoney, std::string & rstrMessage);
	virtual ~GameMsg_S2C_SendGift();
public:
	bool doEncode(CParamPool &IOBuff);
private:
	int						m_nGiftID;		//��Ʒ���
	int						m_nRewardMoney;	//������Ǯ
	std::string				m_strMessage;	//��Ϣ
	std::list< CItem >		m_RewardList;	//������Ʒ
};

#endif
// end of file


