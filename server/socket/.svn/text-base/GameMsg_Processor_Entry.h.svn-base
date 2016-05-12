/*
*2011-10-22
*by wangshijun
*网络消息创建与处理框架。
*/
#ifndef __GAMEMSG_PROCESSOR_ENTRY_H__
#define __GAMEMSG_PROCESSOR_ENTRY_H__

#include "GameMsg_Processor.h"
#include "IProcessorObjGet.h"
#include "GameMsg_Base.h"
class GameMsg_Base;
class GameMsg_Processor_Entry
{
public:
	GameMsg_Processor_Entry( IProcessorObjGet * pIObjGet ,  Processor_MSG fProcessor );
	~GameMsg_Processor_Entry(void);

	void OnProcess( GameMsg_Base & rfMsg,CSlotPeer & SlotPeer )
	{
		if( m_pObjGet) 
		{
			GameMsg_Processor *pObj = m_pObjGet->ProcessorObjectGet( rfMsg );
			if( pObj && m_fProcessor )
			{
				( pObj->*m_fProcessor )( rfMsg,SlotPeer );
			}
		}

	}
private:
	IProcessorObjGet * m_pObjGet;
	Processor_MSG m_fProcessor;
};
#endif


//file end

