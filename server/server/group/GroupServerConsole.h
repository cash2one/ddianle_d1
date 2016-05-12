#ifndef __GROUPSERVER_CONSOLE_H__
#define __GROUPSERVER_CONSOLE_H__

#include "../libServerFrame/Console.h"

class CGroupServerConsole:public CConsole
{
public:
    virtual int ProcessCommand(int nParam = 0);
};

#endif //__GROUPSERVER_CONSOLE_H__

