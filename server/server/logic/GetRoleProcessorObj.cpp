#include "GetRoleProcessorObj.h"
#include "../logic/PlayerManager.h"
#include "../../socket/Query.h"

CGetRoleProcessorObj::CGetRoleProcessorObj(void)
{
}

CGetRoleProcessorObj::~CGetRoleProcessorObj(void)
{
}
GameMsg_Processor * CGetRoleProcessorObj::ProcessorObjectGet( GameMsg_Base & rfMsg )
{
    GameMsg_Processor * pProcessor = NULL;
    IEntity * pEntity = (IEntity *)CPlayerManager::Instance().GetEntity( rfMsg.nIndex );
    if(pEntity != NULL && pEntity->IsValid() && pEntity->GetSessionID() == rfMsg.SessionID)
    {
        pProcessor = (GameMsg_Processor *)pEntity->GetComponent(m_nIndex);
    }

    return pProcessor;
}

GameMsg_Processor * CGetRoleProcessorObj::ProcessorObjectGet( QueryBase & rfQeury )
{
	GameMsg_Processor * pProcessor = NULL;
	IEntity * pEntity = (IEntity *)CPlayerManager::Instance().GetEntity( rfQeury.m_nEntityIndex );
	if(pEntity != NULL && pEntity->GetSessionID() == rfQeury.m_nSessionID && pEntity->IsValid() )
	{
		pProcessor = (GameMsg_Processor *)pEntity->GetComponent(m_nIndex);
	}
	return pProcessor;
}


//end file


