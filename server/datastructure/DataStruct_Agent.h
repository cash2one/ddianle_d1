#ifndef __DATASTRUCT_AGENT_H__
#define __DATASTRUCT_AGENT_H__

#include <string.h>

#define SVRTYPE_COUNT		7

#define SVRTYPE_UNKNOWN		0
#define SVRTYPE_AGENT		0
#define SVRTYPE_ACCOUNT		1
#define SVRTYPE_GROUP		2
#define SVRTYPE_GAME		3

//先支持每台服务器同时开四条线吧。。。。。
#define SVRTYPE_GAME2		4
#define SVRTYPE_GAME3		5
#define SVRTYPE_GAME4		6

#define SVRTYPE_GM			7


static const char s_szSvrType[][20] = 
{
	"agent",
	"AccountServer",
	"GroupServer",
	"GameServer",
	"GameServer2",
	"GameServer3",
	"GameServer4",
	"GMServer"
};

//static char s_szStat[][3] = {"X","R","-" };

typedef struct STAT_BASE
{
public:
	STAT_BASE(){Clear();};
	void Clear()
	{
		nType = 0;
		nStat = 0;
		nCltCnt = 0;
		nVersion = 0;
		nPort = 0;
	};
	const STAT_BASE& operator = (const STAT_BASE& st)
	{
		memcpy(this, &st, sizeof(STAT_BASE));
		return *this;
	}
	int nType;//Server类型
	int nStat;//Server状态
	int nCltCnt;//客户数量
	int nVersion;//版本
	unsigned short nPort;
protected:
private:
}*LPSTAT_BASE;


typedef struct STAT_SYS
{
	unsigned short nCPU[3];//cpu占用率（百分率的10倍），user,system,idle

	unsigned short nMemUsed;
	unsigned short nMemLeft;
	STAT_SYS(){Clear();	};
	void Clear(){memset(this, 0, sizeof(STAT_SYS));};
}*LPSTAT_SYS;


typedef struct AGENT_SETTING
{
	int nSysInfoInterval;//向Monitor报告系统信息的时间间隔
	int nCheckInterval;//检查服务是否开启的时间间隔
	bool abKeepServer[SVRTYPE_COUNT];//是否保持各种服务器运行
}*LPAGENT_SETTING;


#ifdef WIN32
typedef struct STAT_MON
{
public:
	char szAddr[16];//ip地址
	STAT_BASE stat[SVRTYPE_COUNT];//各服务器状态
	STAT_SYS sysInfo;//系统信息
	AGENT_SETTING setting;//设置信息
	unsigned short nPort;
	void Clear()
	{
		for(int i = 0; i < SVRTYPE_COUNT; i ++)
		{
			stat[i].Clear();
		}
		sysInfo.Clear();
		nPort = 0;
	}
protected:
private:
}*LPSTAT_MON;
#endif

#endif //__DATASTRUCT_AGENT_H__


//file end

