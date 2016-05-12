#ifndef	__REQUIRMENTHANDLERMANAGER_H__
#define __REQUIRMENTHANDLERMANAGER_H__

#include "IRequirementHandler.h"
#include "../../datastructure/Macro_Define.h"


class CRequirementHandlerManager
{
protected:
	CRequirementHandlerManager();
	~CRequirementHandlerManager();

public:
	static CRequirementHandlerManager& Instance();

	void Init();
	void Add(IRequirementHandler *pRequirementHandler);
	void Remove(unsigned int nRequirementHanderID);

	const IRequirementHandler * GetHandler(unsigned short nHandlerID);
public:
	IRequirementHandler		**m_ppRequirementHandler;
};


#endif


//end file


