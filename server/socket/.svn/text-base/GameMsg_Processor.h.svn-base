/*
*2011-10-22
*by wangshijun
*网络消息创建与处理，数据库处理框架通用类。
*/
#ifndef __GAMEMSG_PROCESSOR_H__
#define __GAMEMSG_PROCESSOR_H__


#include <stdlib.h>
#include<list>
#include "SlotPeer.h"


class GameMsg_Processor;
class GameMsg_Base;
class CDefaultProcessorObjGet;
class QueryBase;
class IProcessorObjGet;
class IQueryProcessorObjGet;

using namespace ZeroSocket;

typedef void (GameMsg_Processor::*Processor_MSG)(GameMsg_Base & pMsg, CSlotPeer & SlotPeer);
typedef void (GameMsg_Processor::*Processor_DBQuery)(QueryBase & refMsg);
typedef std::list<unsigned short > RegisterMsgList;

#define RegMsgProcessor(nMsg,func) RegMsgProcessorImpl(nMsg,static_cast<Processor_MSG>(func));
#define RegQueryProcessor(nMsg,func) RegQueryProcessorImpl(nMsg,static_cast<Processor_DBQuery>(func));


class GameMsg_Processor
{
public:
    GameMsg_Processor(bool bProNetMsg, bool bProQueryMsg);
    virtual ~GameMsg_Processor(void);
    virtual IProcessorObjGet * CreateIProcessorObjGet();
    virtual IQueryProcessorObjGet * CreateIQueryProcessorObjGet();

protected:
    void RegMsgProcessorImpl(unsigned short nMsg, Processor_MSG fProcessor);
    void RegQueryProcessorImpl(unsigned short nMsg, Processor_DBQuery fProcessor);
    void DetachAllQueryProcssor();
    void DetachAllMsgProcssor();
    void ClearProcessorObjGet() {
        m_IProcessorObjGet = NULL;
    }
    void ClearQueryProcessorObjGet() {
        m_IQueryProcessorObjGet = NULL;
    }

private:
    RegisterMsgList *m_pMsgList;
    RegisterMsgList *m_pdbMsgList;

    IProcessorObjGet *m_IProcessorObjGet;
    IQueryProcessorObjGet *m_IQueryProcessorObjGet;
};

#endif

