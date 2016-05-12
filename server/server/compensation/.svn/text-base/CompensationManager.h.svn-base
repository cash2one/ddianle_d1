#ifndef __COMPENSATION_MGR_H__
#define __COMPENSATION_MGR_H__

#include "../../datastructure/DataStruct_DB.h"
#include <map>


#define INVALID_COMPESSATION_ID	-1

class CompensationMgr
{
public:
	CompensationMgr();
	~CompensationMgr();

	static CompensationMgr& Instance();

	bool InsertCompensation(CompensationInfo& cInfo);
	void ClearCompensation();

public:
	map<int, CompensationInfo> m_CompensationMap;
};

#endif
