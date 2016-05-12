#ifndef	__GAMEMSG_S2C_UPDATE_EXTRA_EXP_ACTIVITY_STATE_H__
#define	__GAMEMSG_S2C_UPDATE_EXTRA_EXP_ACTIVITY_STATE_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include <list>

class CParamPool;

class GameMsg_S2C_UpdateExtraExpActivityState : public GameMsg_Base
{
public:
	GameMsg_S2C_UpdateExtraExpActivityState(int nActivityID, bool bOpening, unsigned char nStartHour, unsigned char nStartMinute,
		unsigned char nStopHour, unsigned char nStopMinute, unsigned short nRewardExpRate);
	~GameMsg_S2C_UpdateExtraExpActivityState();
public:
	bool doEncode(CParamPool &IOBuff);
private:
	int					m_nActivityID;		//����
	bool				m_bOpening;			//�Ƿ���
	unsigned char		m_nStartHour;		//���Ӫ��ʼʱ�䣭Сʱ
	unsigned char		m_nStartMinute;		//���Ӫ��ʼʱ�䣭����
	unsigned char		m_nStopHour;		//���Ӫ����ʱ�䣭Сʱ
	unsigned char		m_nStopMinute;		//���Ӫ����ʱ�䣭����
	unsigned short		m_nRewardExpRate;	//���齱���ٷֱ�
};

#endif
// end of file


