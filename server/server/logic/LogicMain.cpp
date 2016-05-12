#include "../share/StdAfx.h"
#include "../libServerFrame/Main.h"
#include "LogicCirculator.h"


int main(int argc, char* argv[])
{
    g_sSection = s_szSvrType[SVRTYPE_GAME];

    bool bSuccessInitConf = true;
    bool bDaemon = InitConf(argc, argv, bSuccessInitConf);

    if (!bSuccessInitConf)
        return 0;

    // start server
    CLogicCirculator circulator;
    if (!circulator.Start(!bDaemon))
    {
        WriteLog(LOGLEVEL_ERROR, "circulator.Start() return false!\n");
#ifdef WIN32
        (void)getchar();
#endif
        return 0;
    }
    else
    {
        WriteLog(LOGLEVEL_DEBUG, "circulator.Start() return true!\n");
    }

    // stop server
    circulator.Stop();

    return 0;
}

