/*
*2011-10-22
*by wangshijun
*网络消息创建与处理框架。
*/
#ifndef __DEFAUTPROCESSOR_OBJGET_H__
#define __DEFAUTPROCESSOR_OBJGET_H__
#include "IProcessorObjGet.h"
#ifdef WIN32
#pragma warning(disable:4625 4626 4512)
#endif

class GameMsg_Processor;
class CDefaultProcessorObjGet :
	public IProcessorObjGet
{
public:
	CDefaultProcessorObjGet(GameMsg_Processor & refProcessor);
	~CDefaultProcessorObjGet(void);

	GameMsg_Processor * ProcessorObjectGet( GameMsg_Base & rfMsg );

private:
	GameMsg_Processor & m_refProcessor;
};

#endif


//file end

