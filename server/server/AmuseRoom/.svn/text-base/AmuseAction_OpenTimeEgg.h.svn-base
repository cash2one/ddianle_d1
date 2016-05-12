#ifndef __AMUSEACTION_OPENTIMEEGG__H__
#define __AMUSEACTION_OPENTIMEEGG__H__


#include "AmuseActionBase.h"
#include <string>
#include "../../datastructure/DataStruct_Base.h"

struct OpenTimeEggEntry;

class CAmuseAction_OpenTimeEgg : public CAmuseAction_Base
{
public:
	CAmuseAction_OpenTimeEgg(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID);
	virtual ~CAmuseAction_OpenTimeEgg();

	virtual bool FormParam(CAmuseActionParam_Base* pActionParam);

	virtual bool GoProcess(std::list<CAmuseAction_Base*>& rNewAction);

private:
	void DoOpenTimeEgg(OpenTimeEggEntry* pOpenTimeEggEntry);
public:
	unsigned char m_chOpenType;
};

#endif
//file end

