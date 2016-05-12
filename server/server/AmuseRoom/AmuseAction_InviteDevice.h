#ifndef __AMUSEACTION_INVITEDEVICE__H__
#define __AMUSEACTION_INVITEDEVICE__H__


#include "AmuseActionBase.h"


class CAmuseAction_InviteDevice : public CAmuseAction_Base
{
public:
	CAmuseAction_InviteDevice(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID);
	virtual ~CAmuseAction_InviteDevice();

	virtual bool FormParam(CAmuseActionParam_Base* pActionParam);
	virtual bool GoProcess(std::list<CAmuseAction_Base*>& rNewAction);

public:
	unsigned int m_nInviteeID;
};

#endif
//file end

