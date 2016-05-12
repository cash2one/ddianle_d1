#ifndef	__GAMEMSG_S2C_UPDATE_GIFT_INFO_H__
#define	__GAMEMSG_S2C_UPDATE_GIFT_INFO_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include <list>

class CParamPool;

class GameMsg_S2C_UpdateGiftInfo : public GameMsg_Base
{
public:
	GameMsg_S2C_UpdateGiftInfo(int nIndexID, int nGiftID, unsigned int nExpiredTime, std::list< CItem > & rRewardList, int nRewardMoney);
	virtual ~GameMsg_S2C_UpdateGiftInfo();
public:
	bool doEncode(CParamPool &IOBuff);
private:
	int m_nIndexID;						//���
	int m_nGiftID;						//��Ʒ���
	unsigned int m_nExpiredTime;		//����ʱ��
	int m_nRewardMoney;					//������Ǯ
	std::list< CItem > m_RewardList;	//������Ʒ
};

#endif
// end of file


