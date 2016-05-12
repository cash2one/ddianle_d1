/*
*2011-10-22
*by wangshijun
*网络消息创建与处理框架。
*/
#ifndef __DEFAUTQUERYPROCESSOR_OBJGET_H__
#define __DEFAUTQUERYPROCESSOR_OBJGET_H__
#include "IQueryProcessorObjGet.h"

#ifdef WIN32
#pragma warning(disable:4625 4626 4512)
#endif

class CDefaultQueryProcessorObjGet
	:public IQueryProcessorObjGet
{
public:
	CDefaultQueryProcessorObjGet(GameMsg_Processor & refProcessor);
	~CDefaultQueryProcessorObjGet(void);

	GameMsg_Processor * ProcessorObjectGet( QueryBase & rfMsg );
private:
	GameMsg_Processor & m_refProcessor;
};
#endif


//file end

