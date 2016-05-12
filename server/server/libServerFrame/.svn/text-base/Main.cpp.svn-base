#include "Main.h"
#include "../../socket/Utility.h"
#include "../../socket/Windefine.h"
#include "../../socket/Log.h"
#include "../../socket/Formatter.h"
#include "../util/CommonFunc.h"
#include <time.h>
#include <stdlib.h>
#include <string>
using namespace std;

#include "../../datastructure/DataStruct_Agent.h"

string g_sPath;
CIni g_Config;
CIni g_GameConfig;

int g_nSection = 0;
int g_nGroup = -1;
int g_nLine = -1;
int g_nPort = -1;
const char *g_sSection = s_szSvrType[SVRTYPE_UNKNOWN];


bool InitConf(int argc, char* argv[],bool &bSuccessInitConf)
{
    //initialze global path
    g_sPath = GetExecPath(argv);

#ifndef WIN32
    string::size_type pos = g_sPath.find_last_of('/', g_sPath.size() -2);
    if( pos != string::npos)
        g_sPath = g_sPath.substr(0, pos+1);
#endif

    bool bRes = false;

    //Load configuration
    string confPath = g_sPath + "etc/";

    //从服务器地图中读取自己所在的区组线
    CIni &ini = g_Config;
    ini.Load((confPath + "servermap.conf").c_str());
    string strHost = s_Host.GetHostAddr();

    for(int i = 1; i < argc-1; i++)
    {
        if(strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "/p") == 0)
        {
            g_nPort = atoi(argv[i+1]);
            break;
        }
    }

#define MAX_ENV_STRING	80

    if( g_nPort >= 0 )
    {
        std::string strPorts;
        Formatter::Format(strPorts, ".%d", g_nPort);
        strHost.append(strPorts);
    }

    g_nSection = ini.GetInt(strHost.c_str(), "section", -1);
    g_nGroup = ini.GetInt(strHost.c_str(), "group", -1);
    g_nLine = ini.GetInt(strHost.c_str(), "line", -1);

#ifdef WIN32
#ifdef _DEBUG
    if ( g_nSection == -1 )
    {
        g_nSection = 0;
    }

    if ( g_nGroup == -1 )
    {
        g_nGroup = 0;
    }

    if ( g_nLine == -1 )
    {
        g_nLine = 0;
    }
#endif
#endif

    if(g_nSection == -1)
    {
        bSuccessInitConf = false;
    }

    ini.Clear();

    //载入全局的配置文件
    string strConf = confPath + "global.conf";
    bRes = ini.Load(strConf.c_str());
    printf("Load configuration global.conf...[%s]\n", bRes ? "OK" : "Failed");

    //从全局配置中获得自己所在的区的配置文件，载入之
    char szSection[MAX_ENV_STRING];
    Formatter::Format(szSection, sizeof(szSection), "section%d", g_nSection);
    ini.GetString(szSection, "conf",strConf, "" );
    if(strConf.size() > 0)
    {
        strConf = confPath + strConf;
        bRes = ini.Load(strConf.c_str());
        printf("Load configuration (%s)... [%s]\n", strConf.c_str(), bRes ? "OK" : "Failed");
    }

    if( g_nGroup >= 0)
    {
        //如果有组，则载入所在组的配置文件
        Formatter::Format(szSection, sizeof(szSection), "group%d", g_nGroup);
        ini.GetString(szSection, "conf", strConf, "");
        if(strConf.size() > 0)
        {
            strConf = confPath + strConf;
            bRes = ini.Load(strConf.c_str());
            printf("Load configuration (%s)... [%s]\n", strConf.c_str(), bRes ? "OK" : "Failed");
        }

        if(g_nLine >= 0)
        {
            //如果有线，则载入所在线的配置文件
            Formatter::Format(szSection, sizeof(szSection), "line%d", g_nLine);
            ini.GetString(szSection, "conf", strConf, "");
            if(strConf.size() > 0)
            {
                strConf = confPath + strConf;
                bRes = ini.Load(strConf.c_str());
                printf("Load configuration (%s)... [%s]\n", strConf.c_str(), bRes ? "OK" : "Failed");
            }
        }
    }

    string sFile;
    for(int i = 1; i < argc-1; i++)
    {
        if(strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "/f") == 0)
        {
            sFile = argv[i+1];
            g_Config.Load(sFile.c_str());
            break;
        }
    }

    //载入game.conf配置文件
    char szFileName[MAX_PATH];
    g_Config.GetString("GameServer", "GameConfFile",szFileName, MAX_PATH, "etc/game.conf");

    strConf = g_sPath + szFileName;
    bRes = g_GameConfig.Load(strConf.c_str());
    printf("Load configuration (%s)... [%s]\n", szFileName, bRes ? "OK" : "Failed");

    std::string strProcessTime;
    ini.GetString("Log", "DbWarningProcessTime", strProcessTime, "-1");
    g_DbWarningProcessTime = atoi(strProcessTime.c_str());

    std::string strWaitTime;
    ini.GetString("Log", "DbWarningWaitTime", strWaitTime, "-1");
    g_DbWarningWaitTime = atoi(strWaitTime.c_str());

    std::string strQueueSize;
    ini.GetString("Log", "DbWarningQueueSize", strQueueSize, "-1");
    g_DbWarningQueueSize = atoi(strQueueSize.c_str());

    bool bDaemon = !g_Config.GetInt(g_sSection, "runmode", 1);
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "-d") == 0)
        {
            bDaemon = true;
            break;
        }
    }

#ifdef WIN32
#else
    if(bDaemon)
    {
        printf("Enter daemon mode\n");
        init_daemon();
    }

    signal(SIGPIPE, SIG_IGN);
#endif

    //	if(bRes)
    {
        string sLogPath;
        string strLogFileName;
        string strSlowQueryLogFileName;
        g_Config.GetString(g_sSection, "logpath", sLogPath, g_sPath.c_str());
        g_Config.GetString(g_sSection, "logfile", strLogFileName, "logs/defaultlog");
        g_Config.GetString(g_sSection, "dbquerylogfile", strSlowQueryLogFileName, "logs/defaultlog");
        int nLogMode = LOGMODE_FILE|LOGMODE_CR|LOGMODE_TRUNCATE;
        if(!bDaemon) nLogMode |= LOGMODE_STDOUT;
#ifdef WIN32
        nLogMode |= LOGMODE_STDOUT;
#endif
        //目录不存在则自动创建
        std::string::size_type nPos = strLogFileName.find('/');
        if ( nPos != std::string::npos )
        {
            std::vector< std::string > rStrVec;
            SplitString(strLogFileName, "/", rStrVec);
            if ( rStrVec.size() == 2 )
            {
                std::string strPath = sLogPath + rStrVec[0];
                if ( !CheckDirExist(strPath.c_str()) )
                {
                    CreateDir(strPath.c_str());
                }
            }
        }

        nPos = strSlowQueryLogFileName.find('/');
        if (nPos != std::string::npos)
        {
            std::vector< std::string > rStrVec;
            SplitString(strSlowQueryLogFileName, "/", rStrVec);
            if (rStrVec.size() == 2)
            {
                std::string strPath = sLogPath + rStrVec[0];
                if (!CheckDirExist(strPath.c_str()))
                {
                    CreateDir(strPath.c_str());
                }
            }
        }

        if(strLogFileName[0] != '/')
            strLogFileName.insert(0, sLogPath);

        if (strSlowQueryLogFileName[0] != '/')
        {
            strSlowQueryLogFileName.insert(0, sLogPath);
        }

        if(g_nGroup >= 0 && g_nLine >= 0)
            Formatter::Format(strLogFileName, "%s%02d-%02d", strLogFileName.c_str(), g_nGroup, g_nLine);

        if (g_nGroup >= 0 && g_nLine >= 0)
            Formatter::Format(strSlowQueryLogFileName, "%s%02d-%02d", strSlowQueryLogFileName.c_str(), g_nGroup, g_nLine);

        g_logDefault.SetLogMode(nLogMode, strLogFileName.c_str());
        g_DBQueryExceptionLog.SetLogMode(nLogMode, strSlowQueryLogFileName.c_str());

        strLogFileName.append(".core");
        g_logCore.SetLogMode(nLogMode, strLogFileName.c_str());
    }

    WriteLog( LOGLEVEL_SYSTEM, "Load configuration completed.\n" );

    return bDaemon;
}

void sig_pipe(int nSignal)
{
    //Handle signal SIGPIPE
}

