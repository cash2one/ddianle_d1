#ifndef	__IQUESTMAN_H__
#define __IQUESTMAN_H__
#include "../server/quest/IQuest.h"

class CQuestDoc;

class IQuestMan
{
	public:
	IQuestMan()
	{
	};
	virtual ~IQuestMan()
	{
	}
public:
	virtual void Register(unsigned int nRoleID, IQuest *pQuestComponet) = 0;
	virtual void Remove(unsigned int nRoleID) = 0;
	virtual void Notify(CQuestDoc * pQuestDoc) = 0;

	virtual void PushPlayerQuest(IQuest *pQuestComponet) = 0;
};


#endif

//end file


