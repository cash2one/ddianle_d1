#ifndef __AMUSEACTION_RESPONSEDEVICE__H__
#define __AMUSEACTION_RESPONSEDEVICE__H__


#include "AmuseActionBase.h"


class CAmuseAction_ResponseDevice : public CAmuseAction_Base
{
public:
	CAmuseAction_ResponseDevice(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID);
	virtual ~CAmuseAction_ResponseDevice();

	virtual bool FormParam(CAmuseActionParam_Base* pActionParam);
	virtual bool GoProcess(std::list<CAmuseAction_Base*>& rNewAction);

public:
	unsigned int m_nInviterID;
	int m_nDeviceID;
	bool m_bAccept;
};

#endif
//file end

