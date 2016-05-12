#ifndef __AMUSEACTION_GETTIMEEGGGIFTLIST__H__
#define __AMUSEACTION_GETTIMEEGGGIFTLIST__H__


#include "AmuseActionBase.h"
#include <string>
#include "../../datastructure/DataStruct_Base.h"

class CAmuseAction_GetTimeEggGiftList : public CAmuseAction_Base
{
public:
	CAmuseAction_GetTimeEggGiftList(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID);
	virtual ~CAmuseAction_GetTimeEggGiftList();

	virtual bool FormParam(CAmuseActionParam_Base* pActionParam);

	virtual bool GoProcess(std::list<CAmuseAction_Base*>& rNewAction);
public:
	set<itemtype_t> m_setGiftAndDesireItemId;
};

#endif
//file end

