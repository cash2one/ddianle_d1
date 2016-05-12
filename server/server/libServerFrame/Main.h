#ifndef __MAIN_H__
#define __MAIN_H__

#include "../../socket/Ini.h"
#include "../../socket/Log.h"
extern string g_sPath;
extern CIni g_Config;
extern const char* g_sSection;
//初始化配置信息
bool InitConf(int argc, char* argv[],bool &bSuccessInitConf);

#ifndef WIN32
#include <signal.h>
void sig_pipe(int nSignal);
#endif
#endif//__MAIN_H__


//file end

