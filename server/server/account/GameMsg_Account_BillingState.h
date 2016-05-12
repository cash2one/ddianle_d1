#ifndef __GAMEMSG_ACCOUNT_BILLINGSTATE_H__
#define __GAMEMSG_ACCOUNT_BILLINGSTATE_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

typedef class GameMsg_Account_BillingState :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GameMsg_Account_BillingState(void);
	~GameMsg_Account_BillingState(void);
	int nPt;// £”‡µ„ ˝
	int nChange;//µ÷”√æÌ
public :
	virtual bool doEncode( CParamPool& IoBuff );
}* LPGameMsg_Account_BillingState;
#endif

//end file


