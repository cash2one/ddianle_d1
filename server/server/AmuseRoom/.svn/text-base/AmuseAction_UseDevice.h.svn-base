#ifndef __AMUSEACTION_USEDEVICE__H__
#define __AMUSEACTION_USEDEVICE__H__


#include "AmuseActionBase.h"

class CAmuseRoomDevice;


class CAmuseAction_UseDevice : public CAmuseAction_Base
{
public:
	CAmuseAction_UseDevice(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID);
	virtual ~CAmuseAction_UseDevice();

	virtual bool FormParam(CAmuseActionParam_Base* pActionParam);
	virtual CAmuseActionParam_Base* ToParam(int nNowTime);

	virtual bool GoProcess(std::list<CAmuseAction_Base*>& rNewAction);
	virtual void GoTerminate();

public:
	void SetAsInvited();

protected:
	int _GetUseDeviceType(CAmuseRoomDevice& rAmuseDevice);

	bool _ProcessUseDevice(CAmuseRoomDevice& rAmuseDevice, std::list<CAmuseAction_Base*>& rNewAction);
	bool _ProcessUseNormal(CAmuseRoomDevice& rAmuseDevice, std::list<CAmuseAction_Base*>& rNewAction);
	bool _ProcessUseTimeEgg(CAmuseRoomDevice& rAmuseDevice, std::list<CAmuseAction_Base*>& rNewAction);
	bool _ProcessUseSwing(CAmuseRoomDevice& rAmuseDevice, std::list<CAmuseAction_Base*>& rNewAction);

	void _TerminateUseDevice(CAmuseRoomDevice& rAmuseDevice);
	void _TerminateUseSwing(CAmuseRoomDevice& rAmuseDevice);
	void _TerminateUseTimeEgg(CAmuseRoomDevice& rAmuseDevice);

public:
	int m_nDeviceID;
	int m_nUseType;
	int m_nPartnerType;

	int m_nStartTime;
	bool m_bIsInvited;
};

#endif
//file end

