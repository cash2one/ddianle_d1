#include <stdio.h>
#include "GMCirculator.h"
#include "../libServerFrame/Main.h"

extern string g_sPath;
extern CIni g_Config;
extern CIni g_GameConfig;

extern int g_nPort;
extern const char *g_sSection;


bool InitGMConf(int argc, char* argv[], bool &bSuccessInitConf);

int main(int argc, char* argv[])
{
    g_sSection = s_szSvrType[SVRTYPE_GM];

    bool bSuccessInitConf = true;
    bool bDaemon = InitGMConf(argc, argv, bSuccessInitConf);

    if (!bSuccessInitConf)
        return 0;

    // start server
    CGMCirculator circulator;
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


bool InitGMConf(int argc, char* argv[], bool &bSuccessInitConf)
{
    //initialze global path
    g_sPath = GetExecPath(argv);

#ifndef WIN32
    string::size_type pos = g_sPath.find_last_of('/', g_sPath.size() - 2);
    if (pos != string::npos)
        g_sPath = g_sPath.substr(0, pos + 1);
#endif

    //Load configuration
    string confPath = g_sPath + "GM/etc/";

    CIni &ini = g_Config;
    string strHost = s_Host.GetHostAddr();

    ini.Clear();

    //载入全局的配置文件
    string strConf = confPath + "gm.conf";
    printf("Load configuration (%s)... ", strConf.c_str());
    printf("%s\n", ini.Load(strConf.c_str()) ? "OK" : "Failed");

    bool bDaemon = !g_Config.GetInt(g_sSection, "runmode", 1);
#ifdef WIN32
#else
    if (bDaemon)
    {
        printf("Enter daemon mode.\n");
        init_daemon();
    }

    signal(SIGPIPE, SIG_IGN);
#endif
    string sLogPath;
    string sLogFileName;
    g_Config.GetString(g_sSection, "logpath", sLogPath, g_sPath.c_str());
    g_Config.GetString(g_sSection, "logfile", sLogFileName, "logs/defaultlog");
    int nLogMode = LOGMODE_FILE | LOGMODE_CR | LOGMODE_TRUNCATE;
    if (!bDaemon) nLogMode |= LOGMODE_STDOUT;
#ifdef WIN32
    nLogMode |= LOGMODE_STDOUT;
#endif

    if (sLogFileName[0] != '/')
        sLogFileName.assign(sLogPath + sLogFileName);

    g_logDefault.SetLogMode(nLogMode, sLogFileName.c_str());

    sLogFileName.append(".core");
    g_logCore.SetLogMode(nLogMode, sLogFileName.c_str());

    return bDaemon;
}

