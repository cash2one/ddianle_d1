#ifndef __AMUSEACTION_REQUESTDEVICE__H__
#define __AMUSEACTION_REQUESTDEVICE__H__


#include "AmuseActionBase.h"


class CAmuseAction_RequestDevice : public CAmuseAction_Base
{
public:
	CAmuseAction_RequestDevice(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID);
	virtual ~CAmuseAction_RequestDevice();

	virtual bool FormParam(CAmuseActionParam_Base* pActionParam);
	virtual bool GoProcess(std::list<CAmuseAction_Base*>& rNewAction);

public:
	unsigned int m_nInviterID;
	int m_nDeviceID;
};

#endif
//file end

