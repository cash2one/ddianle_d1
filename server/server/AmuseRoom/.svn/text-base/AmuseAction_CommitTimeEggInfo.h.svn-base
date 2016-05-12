#ifndef __AMUSEACTION_COMMITTIMEEGGINFO__H__
#define __AMUSEACTION_COMMITTIMEEGGINFO__H__


#include "AmuseActionBase.h"
#include <string>
#include "../../datastructure/DataStruct_Base.h"
#include "../mall/MallDefine.h"

class CAmuseAction_CommitTimeEggInfo : public CAmuseAction_Base
{
public:
    CAmuseAction_CommitTimeEggInfo(CAmuseRoomComponent* pAmusePlayer, unsigned int nActionID);
    virtual ~CAmuseAction_CommitTimeEggInfo();

    virtual bool FormParam(CAmuseActionParam_Base* pActionParam);

    virtual bool GoProcess(std::list<CAmuseAction_Base*>& rNewAction);

    const std::string& GetWords() const { return m_strWords; }

public:
    VecPeddleGoods  m_listGift;     // ºÕƒÓ∆∑

protected:
    std::string     m_strWords;     // ¡Ù—‘
};

#endif
//file end

