#ifndef __AMUSEACTION_LEAVEDEVICE__H__
#define __AMUSEACTION_LEAVEDEVICE__H__


#include "AmuseActionBase.h"


class CAmuseAction_LeaveDevice : public CAmuseAction_Base
{
public:
	CAmuseAction_LeaveDevice(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID);
	virtual ~CAmuseAction_LeaveDevice();

	virtual bool FormParam(CAmuseActionParam_Base* pActionParam);
	virtual bool GoProcess(std::list<CAmuseAction_Base*>& rNewAction);
};

#endif
//file end

