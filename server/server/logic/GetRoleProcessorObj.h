/*
*2011-10-22
*by wangshijun
*������Ϣ�����봦���ܡ�
*/
#ifndef __ROLEPROCESSOR_OBJGET_H__
#define __ROLEPROCESSOR_OBJGET_H__
#include "../../socket/IProcessorObjGet.h"
#include "../../socket/IQueryProcessorObjGet.h"
class CGetRoleProcessorObj :
	public IProcessorObjGet,
	public IQueryProcessorObjGet
{
public:
	CGetRoleProcessorObj(void);
	~CGetRoleProcessorObj(void);

	virtual GameMsg_Processor * ProcessorObjectGet( GameMsg_Base & rfMsg );
	virtual GameMsg_Processor * ProcessorObjectGet( QueryBase & rfQeury ) ;
public:
	int m_nIndex;
};

#endif



//end file


