#ifndef	__GMQUERY_H__
#define	__GMQUERY_H__

#include "../../socket/Query.h"
#include "../../socket/QueryManagerBase.h"
#include "GMDBInterface.h"
//#include "GMSlaveDBInterface.h"
#include "GMQueryType.h"

class CGMQueryMgr : public CQueryManagerBase
{
public:
	CGMQueryMgr();
	virtual ~CGMQueryMgr();
public:
	bool OpenDatabase();
	int ProcessQuery(QueryBase* &pQuery);
	int GetQueueSize();
	bool GetThreadState();
	bool RecreateThread();
protected:
	GMDBInterface m_dbGM;
    GMDBInterface m_dbSlaveGM;
    GMDBInterface m_dbSubLog;

    unsigned int m_nMinRoleRange;
    unsigned int m_nMaxRoleRange;

};

#endif

