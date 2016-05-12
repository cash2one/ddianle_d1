#include "../share/StdAfx.h"
#include "../libServerFrame/Main.h"
#include "GroupServerCirculator.h"


int main(int argc, char* argv[])
{
    g_sSection = s_szSvrType[SVRTYPE_GROUP];

    bool bSuccessInitConf = true;
    bool bDaemon = InitConf(argc, argv, bSuccessInitConf);

    if (!bSuccessInitConf)
        return 0;

    // start server
    CGroupServerCirculator circulator;
    if (!circulator.Start(!bDaemon))
    {
#ifdef WIN32
        (void)getchar();
#endif
        return 0;
    }

    // stop server
    circulator.Stop();

    return 0;
}

