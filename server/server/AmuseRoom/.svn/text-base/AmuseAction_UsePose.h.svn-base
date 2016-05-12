#ifndef __AMUSEACTION_USEPOSE__H__
#define __AMUSEACTION_USEPOSE__H__


#include "AmuseActionBase.h"


class CAmuseAction_UsePose : public CAmuseAction_Base
{
public:
	CAmuseAction_UsePose(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID);
	virtual ~CAmuseAction_UsePose();

	virtual bool FormParam(CAmuseActionParam_Base* pActionParam);
	virtual CAmuseActionParam_Base* ToParam(int nNowTime);

	virtual bool GoProcess(std::list<CAmuseAction_Base*>& rNewAction);

public:
	int m_nPoseID;
};

#endif
//file end

