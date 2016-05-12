#include "../share/StdAfx.h"
#include "AgentConsole.h"
#include "AgentCirculator.h"
#include "../../socket/Utility.h"

extern CAgentCirculator* g_pCirculator;

int CAgentConsole::ProcessCommand(int nParam /* = 0 */)
{
	CConsole::ProcessCommand(nParam);
	switch(m_nCommand) 
	{
	case 1://closeservers
		{
			unsigned char bCloseServer[SVRTYPE_COUNT];
			for(int i = 0; i < SVRTYPE_COUNT; i++)
			{
				bCloseServer[i] = 1;
			}
			g_pCirculator->CloseServer(bCloseServer);
		}
		break;
	default:
		break;
	}

	return m_nCommand;
}


//end file


