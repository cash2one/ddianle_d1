#ifndef __ID_CREATER_H__
#define __ID_CREATER_H__

#include "../../socket/Windefine.h"
#include "../../socket/GameMsg_Processor.h"

class CIDCreater : public GameMsg_Processor
{
public:
    CIDCreater();
    virtual ~CIDCreater() {}

    unsigned int GetID32();
    __int64 GetID64();

protected:
    void RegComponentNetMsgMap();
    void OnQueryIDFinish(QueryBase & pQuery);
    void GetBatchID64();

protected:
    unsigned int    m_nID32ProcessInternal;
    __int64         m_nID64ProcessInternal;
    __int64         m_nID64;
    __int64         m_nID64Seg;
};

#endif//__ID_CREATER_H__

