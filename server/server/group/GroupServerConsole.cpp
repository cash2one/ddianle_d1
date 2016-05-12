#include "../share/StdAfx.h"
#include "GroupServerConsole.h"

int CGroupServerConsole::ProcessCommand(int nParam /* = 0 */)
{
    CConsole::ProcessCommand(nParam);
    switch (m_nCommand) 
    {
    case 1:
    default:
        m_strExport.assign("Unknown command\n");
        break;
    }

    return m_nCommand;
}

