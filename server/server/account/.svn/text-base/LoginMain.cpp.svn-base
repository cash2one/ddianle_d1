#include "../../socket/inc/SocketLib.h"
#include "../libServerFrame/Main.h"
#include "LoginCirculator.h"


int main(int argc, char* argv[])
{
    g_sSection = s_szSvrType[SVRTYPE_ACCOUNT];

    bool bSuccessInitConf = true;
    bool bDaemon = InitConf(argc, argv, bSuccessInitConf);

    if (!bSuccessInitConf)
        return 0;

    // start server
    CLoginCirculator circulator;
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

