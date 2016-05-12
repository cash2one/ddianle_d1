#ifndef __AMUSEACTION_CREATETIMEEGG__H__
#define __AMUSEACTION_CREATETIMEEGG__H__


#include "AmuseActionBase.h"
#include <string>
#include "../../datastructure/DataStruct_Base.h"
#include "../mall/MallDefine.h"

class CEntityMallComponent;
struct CreateTimeEggEntry;

class CAmuseAction_CreateTimeEgg : public CAmuseAction_Base
{
public:
	CAmuseAction_CreateTimeEgg(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID);
	virtual ~CAmuseAction_CreateTimeEgg();

	virtual bool FormParam(CAmuseActionParam_Base* pActionParam);

	virtual bool GoProcess(std::list<CAmuseAction_Base*>& rNewAction);

	void OnCreateTimeEggReturn(int nError, const string& strError);
private:
	void DoCreateTimeEgg(CreateTimeEggEntry* pCreateTimeEggEntry);
private:
	std::map<CEntityMallComponent*, CurrencyMap> m_mapBills;
};

#endif
//file end

