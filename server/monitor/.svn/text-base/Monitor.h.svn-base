// Monitor.h : Monitor 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h” 
#endif

#include "resource.h"       // 主符号
#include "../datastructure/DataStruct_Agent.h"

// CMonitorApp:
// 有关此类的实现，请参阅 Monitor.cpp
//
#include <map>
#include <vector>
#include <list>
using namespace std;

#define IDEvent1	1
#define IDEvent2	2
#define IDEvent60	3
#define IDEventBeep 4

typedef struct ItemAddrAndIp
{
	char szAddr[16];//ip地址
	unsigned short nPort;

	ItemAddrAndIp()
	{
		memset(szAddr,0,16);
		nPort = 0;
	}

}*LP_ItemAddrAndIp;

class CMonitorApp : public CWinApp
{
public:
	CMonitorApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMonitorApp theApp;
extern CString g_sPath;
extern vector< pair< CString, vector< ItemAddrAndIp > > > g_map;
extern vector< STAT_MON > m_aAgent;
extern vector< HTREEITEM >m_aItem;
extern int g_nTotalPlayer;

class ChartPoint
{
public:
	string strTime;
	vector< int > aCount;
};

class Chart
{
public:
	vector< string > aName;
	list < ChartPoint > listPoint;
public:
#define TIME_LEN	11
#define COUNT_LEN	4
	void LoadFromFile(LPCTSTR lpszFile)
	{
		FILE *fp = fopen(lpszFile, "r");

		if(fp != NULL)
		{
			char szBuff[2048];
			char *p;
			memset(szBuff, 0, 2048);
			while (fgets(szBuff, 2048, fp) != NULL )
			{
				if(strlen(szBuff) > TIME_LEN + COUNT_LEN)
				{
					p = szBuff;
					ChartPoint pt;
					*(p+TIME_LEN) = 0;
					pt.strTime = p;
					p += TIME_LEN+1;

					while(*p != 0)
					{
						*(p+COUNT_LEN) = 0;
						pt.aCount.push_back( atoi(p) );
						p += COUNT_LEN + 1;
					}
					listPoint.push_back(pt);
					memset(szBuff, 0, 2048);
				}
			}
			fclose(fp);
		}
	}

};

extern Chart g_chart;