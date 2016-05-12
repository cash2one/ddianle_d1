#ifndef __DATASTRUCT_AGENT_H__
#define __DATASTRUCT_AGENT_H__

#include <string.h>

#define SVRTYPE_COUNT		7

#define SVRTYPE_UNKNOWN		0
#define SVRTYPE_AGENT		0
#define SVRTYPE_ACCOUNT		1
#define SVRTYPE_GROUP		2
#define SVRTYPE_GAME		3

//��֧��ÿ̨������ͬʱ�������߰ɡ���������
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
	int nType;//Server����
	int nStat;//Server״̬
	int nCltCnt;//�ͻ�����
	int nVersion;//�汾
	unsigned short nPort;
protected:
private:
}*LPSTAT_BASE;


typedef struct STAT_SYS
{
	unsigned short nCPU[3];//cpuռ���ʣ��ٷ��ʵ�10������user,system,idle

	unsigned short nMemUsed;
	unsigned short nMemLeft;
	STAT_SYS(){Clear();	};
	void Clear(){memset(this, 0, sizeof(STAT_SYS));};
}*LPSTAT_SYS;


typedef struct AGENT_SETTING
{
	int nSysInfoInterval;//��Monitor����ϵͳ��Ϣ��ʱ����
	int nCheckInterval;//�������Ƿ�����ʱ����
	bool abKeepServer[SVRTYPE_COUNT];//�Ƿ񱣳ָ��ַ���������
}*LPAGENT_SETTING;


#ifdef WIN32
typedef struct STAT_MON
{
public:
	char szAddr[16];//ip��ַ
	STAT_BASE stat[SVRTYPE_COUNT];//��������״̬
	STAT_SYS sysInfo;//ϵͳ��Ϣ
	AGENT_SETTING setting;//������Ϣ
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

