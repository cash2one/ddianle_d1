/*
*2011-10-22
*by wangshijun
*������Ϣ�����봦���ܡ�
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

