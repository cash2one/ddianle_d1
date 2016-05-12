#include "GameMsg_Processor.h"
#include "GameMsg_Map.h"
#include "DBQuery_Map.h"
#include "DefaultProcessorObjGet.h"
#include "DefaultQueryProcessorObjGet.h"
#include "Windefine.h"


GameMsg_Processor::GameMsg_Processor(bool bProNetMsg, bool bProQueryMsg)
{
    m_IProcessorObjGet = NULL;
    m_IQueryProcessorObjGet = NULL;
    if (bProNetMsg)
    {
        m_pMsgList = new RegisterMsgList;
    }
    else
    {
        m_pMsgList = NULL;
    }

    if (bProQueryMsg)
    {
        m_pdbMsgList = new RegisterMsgList;
    }
    else
    {
        m_pdbMsgList = NULL;
    }
}

GameMsg_Processor::~GameMsg_Processor(void)
{
    DetachAllMsgProcssor();
    DetachAllQueryProcssor();
    if (m_IProcessorObjGet != NULL)
    {
        delete m_IProcessorObjGet;
    }
    if (m_IQueryProcessorObjGet != NULL)
    {
        delete m_IQueryProcessorObjGet;
    }
    if (m_pMsgList != NULL)
    {
        delete m_pMsgList;
    }
    if (m_pdbMsgList != NULL)
    {
        delete m_pdbMsgList;
    }
}

void GameMsg_Processor::RegMsgProcessorImpl(unsigned short nMsg, Processor_MSG fProcessor)
{
    if (m_IProcessorObjGet == NULL)
    {
        m_IProcessorObjGet = CreateIProcessorObjGet();
    }
    if (m_IProcessorObjGet != NULL && m_pMsgList != NULL)
    {
        if (GameMsg_Map::Instantce().AddMsgProcessor(nMsg, m_IProcessorObjGet, fProcessor))
        {
            m_pMsgList->push_back(nMsg);
        }
    }
    else
    {
        ASSERT(0);
    }
}

void GameMsg_Processor::RegQueryProcessorImpl(unsigned short nMsg, Processor_DBQuery fProcessor)
{
    if (m_IQueryProcessorObjGet == NULL)
    {
        m_IQueryProcessorObjGet = CreateIQueryProcessorObjGet();
    }
    if (m_IQueryProcessorObjGet != NULL)
    {
        if (CDBQuery_Map::Instantce().AddMsgProcessor(nMsg, m_IQueryProcessorObjGet, fProcessor))
        {
            m_pdbMsgList->push_back(nMsg);
        }
    }
    else
    {
        ASSERT(0);
    }
}

void GameMsg_Processor::DetachAllMsgProcssor()
{
    if (m_IProcessorObjGet != NULL)
    {
        RegisterMsgList::iterator it = m_pMsgList->begin();
        RegisterMsgList::iterator endit = m_pMsgList->end();
        for (; it != endit; it++)
        {
            GameMsg_Map::Instantce().RemoveMsgProcessor(*it, m_IProcessorObjGet);
        }
    }
}

void GameMsg_Processor::DetachAllQueryProcssor()
{
    if (m_IProcessorObjGet != NULL)
    {
        RegisterMsgList::iterator it = m_pMsgList->begin();
        RegisterMsgList::iterator endit = m_pMsgList->end();
        for (; it != endit; it++)
        {
            GameMsg_Map::Instantce().RemoveMsgProcessor(*it, m_IProcessorObjGet);
        }
    }
}

IProcessorObjGet * GameMsg_Processor::CreateIProcessorObjGet()
{
    return new CDefaultProcessorObjGet(*this);
}

IQueryProcessorObjGet * GameMsg_Processor::CreateIQueryProcessorObjGet()
{
    return new CDefaultQueryProcessorObjGet(*this);
}

