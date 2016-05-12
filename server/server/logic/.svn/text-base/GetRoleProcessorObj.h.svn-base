/*
*2011-10-22
*by wangshijun
*网络消息创建与处理框架。
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


