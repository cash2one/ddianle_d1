/*
*2011-10-22
*by wangshijun
*网络消息创建与处理框架。
*/
#ifndef __I_QUERYPROCESSOR_OBJGET_H__
#define __I_QUERYPROCESSOR_OBJGET_H__

class QueryBase;
class GameMsg_Processor;
class IQueryProcessorObjGet
{
public:
	IQueryProcessorObjGet(void)
	{

	}
	virtual ~IQueryProcessorObjGet(void)
	{

	}

	virtual GameMsg_Processor * ProcessorObjectGet( QueryBase & rfMsg ) = 0;

};

#endif

//file end

