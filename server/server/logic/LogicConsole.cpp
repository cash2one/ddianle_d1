#include "../share/stdafx.h"
#include "LogicConsole.h"
#include "LogicCirculator.h"
#include "../logic/PlayerManager.h"
#include "../../socket/Formatter.h"

extern CLogicCirculator* g_pCirculator;

int CLogicConsole::ProcessCommand(int nParam /* = 0 */)
{
    CConsole::ProcessCommand(nParam);
    switch (m_nCommand) 
    {
    case 1: //count
        Formatter(m_strExport) << "Current player count = " << g_pCirculator->GetCurLoginPlayerSize() << "\n";
        break;
    default:
        m_strExport.assign("Unknown command\n");
        break;
    }

    return m_nCommand;
}

