// MonitorDoc.cpp :  CMonitorDoc 类的实现
//

#include "stdafx.h"
#include "Monitor.h"

#include "MonitorDoc.h"
#include "SetAgentDlg.h"
#include "ExecuteDlg.h"
#include "AnnounceDlg.h"
#include ".\monitordoc.h"
#include "MonitorMainFrm.h"

#include "../server/share/ServerMsgDef.h"

#include "../server/AgentMsg_Close.h"
#include "../server/AgentMsg_Announce.h"
#include "../server/AgentMsg_Execute.h"
#include "../server/AgentMsg_Exit.h"
#include "../server/AgentMsg_Kill.h"
#include "../server/AgentMsg_SendGift_Control.h"
#include "../server/AgentMsg_Setting.h"
#include "../server/AgentMsg_ShutdownInTime.h"
#include "../server/AgentMsg_Start.h"
#include "../server/AgentMsg_Stat.h"
#include "../server/AgentMsg_SysInfo.h"


#include "../socket/GameMsg_System_Connect.h"
#include "../socket/GameMsg_System_Disconnect.h"
#include "../socket/GameMsg_Map.h"

#pragma warning(disable:4312)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMonitorDoc

IMPLEMENT_DYNCREATE(CMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CMonitorDoc, CDocument)
	ON_COMMAND(ID_SERVERSTATEUPDATE_PASSIVE, OnServerStateUpdateSpeedPassive)
	ON_UPDATE_COMMAND_UI(ID_SERVERSTATEUPDATE_PASSIVE, OnUpdateServerStateUpdateSpeedPassive)
	ON_COMMAND(ID_SERVERSTATEUPDATE_NORMAL, OnServerStateUpdateSpeedNormal)
	ON_UPDATE_COMMAND_UI(ID_SERVERSTATEUPDATE_NORMAL, OnUpdateServerStateUpdateSpeedNormal)
	ON_COMMAND(ID_SERVERSTATEUPDATE_HIGH, OnServerStateUpdateSpeedHigh)
	ON_UPDATE_COMMAND_UI(ID_SERVERSTATEUPDATE_HIGH, OnUpdateServerStateUpdateSpeedHigh)
	ON_COMMAND(ID_SYSTEMSTATEUPDATESPEED_PASSIVE, OnSystemStateUpdateSpeedPassive)
	ON_UPDATE_COMMAND_UI(ID_SYSTEMSTATEUPDATESPEED_PASSIVE, OnUpdateSystemStateUpdateSpeedPassive)
	ON_COMMAND(ID_SYSTEMSTATEUPDATESPEED_NORMAL, OnSystemStateUpdateSpeedNormal)
	ON_UPDATE_COMMAND_UI(ID_SYSTEMSTATEUPDATESPEED_NORMAL, OnUpdateSystemStateUpdateSpeedNormal)
	ON_COMMAND(ID_SYSTEMSTATEUPDATESPEED_HIGH, OnSystemStateUpdateSpeedHigh)
	ON_UPDATE_COMMAND_UI(ID_SYSTEMSTATEUPDATESPEED_HIGH, OnUpdateSystemStateUpdateSpeedHigh)
	ON_COMMAND(ID_OPTION_BEEPONERROR, OnOptionBeeponerror)
	ON_UPDATE_COMMAND_UI(ID_OPTION_BEEPONERROR, OnUpdateOptionBeeponerror)
	ON_COMMAND(IDM_VIEW_ONLINECOUNT, OnViewOnlinecount)
	ON_COMMAND(IDM_SHUTDOWN10, OnShutdown10)
	ON_UPDATE_COMMAND_UI(IDM_SHUTDOWN10, OnUpdateShutdown10)
	ON_COMMAND(IDM_SHUTDOWN15, OnShutdown15)
	ON_UPDATE_COMMAND_UI(IDM_SHUTDOWN15, OnUpdateShutdown15)
	ON_COMMAND(IDM_SHUTDOWN30, OnShutdown30)
	ON_UPDATE_COMMAND_UI(IDM_SHUTDOWN30, OnUpdateShutdown30)
	ON_COMMAND(IDM_SHUTDOWN60, OnShutdown60)
	ON_UPDATE_COMMAND_UI(IDM_SHUTDOWN60, OnUpdateShutdown60)
	ON_COMMAND(IDM_COMFIRM, OnComfirm)
	ON_UPDATE_COMMAND_UI(IDM_COMFIRM, OnUpdateComfirm)
	ON_COMMAND(IDM_SHUTDOWNALL, OnShutdownAll)
	ON_COMMAND(IDM_RESTOREALL, OnRestoreAll)
END_MESSAGE_MAP()


// CMonitorDoc 构造/析构

CMonitorDoc::CMonitorDoc():GameMsg_Processor(true,true)
{
	// TODO: 在此添加一次性构造代码
	m_aSock = NULL;
	m_aSlotPeer = NULL;
	m_nServerStateUpdateSpeed = SPEED_Passive;
	m_nSystemStateUpdateSpeed = SPEED_Passive;
	m_nShutdownInSeconds = MIN10;
	m_bBeepOnError = true;
	m_bConfirmOperate = true;
	m_pDlg = NULL;
}

CMonitorDoc::~CMonitorDoc()
{
	if(m_aSock != NULL)
	{
		delete [] m_aSock;
	}
	if(m_aSlotPeer != NULL)
	{
		delete [] m_aSlotPeer;
	}

	delete m_pDlg;
}

BOOL CMonitorDoc::Initialize(LPCTSTR strFile)
{
	m_nAgentPort = 	GetPrivateProfileInt("Server", "Port", 0, strFile);
	if(m_nAgentPort == 0)
	{
		AfxMessageBox("Invalid configuration!\n", MB_OK);
		return FALSE;
	}

	char szFileList[MAX_PATH];//,sTemp[MAX_PATH];
	GetPrivateProfileString("OnlineCount", "FileName", "OnlineCount.txt",szFileList, MAX_PATH, strFile);
	m_strOnlineCountFile = szFileList;
	m_nRecordInterval = GetPrivateProfileInt("OnlineCount", "RecordInterval", 1, strFile);
	GetPrivateProfileString("Server", "IPList", "iplist.txt", szFileList, MAX_PATH, strFile);

	int nSecCnt = GetPrivateProfileInt("Section", "Count", 0, szFileList);
	char szSection[MAX_PATH];
	char szKey[MAX_PATH];
	char szValue[MAX_PATH];
	unsigned short nPort = 0;
	for(int i = 0; i < nSecCnt; i++)
	{
		sprintf(szSection, "Section%02d",i);
		strcpy(szKey, "Name");
		GetPrivateProfileString(szSection, szKey, "", szValue, MAX_PATH, szFileList);
		CString strName = szValue;
		//vector< CString > aAgent;
		vector< ItemAddrAndIp > aAgent;
		
		int nSvrCnt = GetPrivateProfileInt(szSection, "SvrCnt", 0, szFileList);
		for(int k = 0; k < nSvrCnt; k++)
		{
			nPort = 0;
			sprintf(szKey, "Svr%02d", k);
			GetPrivateProfileString(szSection, szKey, "", szValue, MAX_PATH, szFileList);

			sprintf(szKey, "Port%02d", k);
			nPort = GetPrivateProfileInt(szSection, szKey, 0, szFileList);

			in_addr addr;
			if( (addr.s_addr = inet_addr(szValue)) != 0)
			{
				ItemAddrAndIp aTemp;
				strcpy(aTemp.szAddr,inet_ntoa(addr));
				aTemp.nPort = nPort;
				aAgent.push_back(aTemp);
				STAT_MON stat;
				stat.Clear();
				strcpy(stat.szAddr, inet_ntoa(addr));
				stat.nPort = nPort;
				m_aAgent.push_back(stat);
				m_aItem.push_back(NULL);
			}
		}
		g_map.push_back( pair< CString, vector< ItemAddrAndIp > > (strName, aAgent) );
	}

	//FILE* pFile = fopen(szFileList, "r");
	//if (pFile == NULL) 
	//{
	//	AfxMessageBox("Can not open control command file for read!\n", MB_OK);
	//	return false;
	//}
	//in_addr addr;
	//while( fgets(sTemp, MAX_PATH, pFile))
	//{
	//	if( (addr.s_addr = inet_addr(sTemp)) != 0)
	//	{
	//		STAT_MON stat;
	//		strcpy(stat.szAddr, inet_ntoa(addr));
	//		m_aAgent.push_back(stat);
	//	}
	//}
	//fclose(pFile);

	if( g_map.size() == 0)
	{
		AfxMessageBox("Invalid IP list!\n", MB_OK);
		return FALSE;
	}

	m_aSock = new CClientSocketMgr[m_aAgent.size()];
	m_aSlotPeer = new CSlotPeer[m_aAgent.size()];
	HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();
	for(int i = 0; i < m_aAgent.size(); i++)
	{
		m_aSock[i].Init(hWnd);
		if (m_aAgent[i].nPort > 0)
		{
			m_aSock[i].Connect(NULL, m_aAgent[i].szAddr, m_aAgent[i].nPort);
		}
		else
			m_aSock[i].Connect(NULL, m_aAgent[i].szAddr, m_nAgentPort);

		m_aSlotPeer[i].SetSocketMgr(&m_aSock[i]);
	}

	m_pDlg = new CDlgOnlineCount;
	m_pDlg->Create(IDD_ONLINECOUNT);

	SetTitle(strFile);

	RegisterMessage();
	RegisterMessageProcessor();
	return TRUE;
}

void CMonitorDoc::RegisterMessage()
{
	GAMEMSG_REGISTERCREATOR(AgentMsg_Stat);
	GAMEMSG_REGISTERCREATOR(AgentMsg_SysInfo);
	GAMEMSG_REGISTERCREATOR(AgentMsg_Setting);
}

void CMonitorDoc::RegisterMessageProcessor()
{
	RegMsgProcessor(MSG_AGENT_Stat,&CMonitorDoc::OnAgentStat);//Agent
	RegMsgProcessor(MSG_AGENT_SysInfo,&CMonitorDoc::OnAgentSysInfo);
	RegMsgProcessor(MSG_AGENT_Setting,&CMonitorDoc::OnAgentSetting);
	
	//RegMsgProcessor(MSG_SYSTEM_Connect,&CMonitorDoc::OnMonitorConnect);//Agent
	//RegMsgProcessor(MSG_SYSTEM_Disconnect,&CMonitorDoc::OnMonitorDisConnect);//Agent
}

BOOL CMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	CString strFile = theApp.m_pszProfileName;

	return Initialize(strFile);

	return TRUE;
}




// CMonitorDoc 序列化

void CMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CMonitorDoc 诊断

#ifdef _DEBUG
void CMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMonitorDoc 命令
void CMonitorDoc::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case IDEvent1://间隔1000ms
		{
			OnNetworkMsg();
			if(m_nServerStateUpdateSpeed == SPEED_High)
				AskServerState();
			if(m_nSystemStateUpdateSpeed == SPEED_High)
				AskSystemState();
		}
		break;
	case IDEvent2://间隔2000ms
		{
			if(m_nServerStateUpdateSpeed == SPEED_Normal)
				AskServerState();
			if(m_nSystemStateUpdateSpeed == SPEED_Normal)
				AskSystemState();

			UpdateAllViews(NULL, HINT_UPDATE_ALL);

			ClacTotalPlayer();
		}
		break;
	case IDEvent60://间隔60秒
		{//记录在线数
			static int s_nTime = 0;
			s_nTime++;
			if(s_nTime >= m_nRecordInterval)
			{
				s_nTime = 0;
				RecordOnlineCount();
			}
		}
		break;
	case IDEventBeep://报警
		BeepOnError();
		break;
	default:
		break;
	}
}

void CMonitorDoc::OnNetworkMsg()
{
	for(int i = 0; i < m_aAgent.size(); i++)
	{
		GameMsg_Base* pMsg = NULL;
		while ( (pMsg = m_aSock[i].GetGameMsg()) != NULL)
		{
			ProcessNetMessage(pMsg,i);

			delete pMsg;
		}
	}
}

void CMonitorDoc::ProcessNetMessage(GameMsg_Base * pMsg,int nSlot)
{
	switch(pMsg->nMsg) 
	{
	case MSG_SYSTEM_Connect:
		{
			m_aAgent[nSlot].stat[0].nStat = ((GameMsg_System_Connect *)pMsg)->nConnCode == 0;
			if(m_aAgent[nSlot].stat[0].nStat == 0)
			{
				m_aAgent[nSlot].Clear();
			}
		}
		break;
	case MSG_SYSTEM_Disconnect:
		{
			m_aAgent[nSlot].Clear();
		}
		break;
	default:
		m_aSlotPeer[nSlot].SetSlot(nSlot);
		GameMsg_Map::Instantce().ProcessMsg(*pMsg,m_aSlotPeer[nSlot]);
		break;
	}
	UpdateAllViews(NULL, HINT_UPDATE_ITEM, (CObject*)nSlot);
}

void CMonitorDoc::OnAgentStat(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	AgentMsg_Stat * pMsgStat = (AgentMsg_Stat *)&msg;
	m_aAgent[nSlot].stat[pMsgStat->stat.nType] = pMsgStat->stat;
}

void CMonitorDoc::OnAgentSysInfo(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	AgentMsg_SysInfo * pMsgSysInfo = (AgentMsg_SysInfo *) &msg;
	m_aAgent[nSlot].sysInfo = pMsgSysInfo->stat;
}

void CMonitorDoc::OnAgentSetting(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	AgentMsg_Setting * pMsgSetting = (AgentMsg_Setting *) &msg;
	m_aAgent[nSlot].setting = pMsgSetting->setting;
}


#define SureUnderControl()	\
	if(m_aAgent[nItem].stat[0].nStat == 0)\
	{\
		CString strPrompt;\
		strPrompt.Format(IDS_AGENT_NOTUNDERCONTROL, m_aAgent[nItem].szAddr);\
		AfxMessageBox( strPrompt, MB_OK|MB_ICONINFORMATION);\
		return ;\
	}

void CMonitorDoc::CloseServer(int nItem, int nSvrType)
{
	SureUnderControl();

	CString strPrompt;
	strPrompt.Format(IDS_AGENT_SURECLOSE, m_aAgent[nItem].szAddr );
	if( !m_bConfirmOperate || IDYES == AfxMessageBox(strPrompt, MB_YESNO|MB_ICONQUESTION) )
	{
		AgentMsg_Close msgClose;
		memset(msgClose.bCloseSvr, 0, sizeof(msgClose.bCloseSvr));
		msgClose.bCloseSvr[nSvrType] = TRUE;

#ifdef _ADMINISTATOR
		m_aSock[nItem].SendGameMsg(&msgClose);
#endif
	}
}

void CMonitorDoc::StartServer(int nItem, int nSvrType)
{
	SureUnderControl();

	CString strPrompt;
	strPrompt.Format(IDS_AGENT_SURESTART, m_aAgent[nItem].szAddr );
	if( !m_bConfirmOperate || IDYES == AfxMessageBox(strPrompt, MB_YESNO|MB_ICONQUESTION) )
	{
		AgentMsg_Start msgStart;
		memset(msgStart.bStartSvr, 0, sizeof(msgStart.bStartSvr));
		msgStart.bStartSvr[nSvrType] = TRUE;

#ifdef _ADMINISTATOR
		m_aSock[nItem].SendGameMsg(&msgStart);
#endif	
	}
}

void CMonitorDoc::KillServer(int nItem, int nSvrType)
{
	SureUnderControl();

	CString strPrompt;
	strPrompt.Format(IDS_AGENT_SUREKILL, m_aAgent[nItem].szAddr );
	if( !m_bConfirmOperate || IDYES == AfxMessageBox(strPrompt, MB_YESNO|MB_ICONQUESTION) )
	{
		AgentMsg_Kill msgKill;
		memset(msgKill.bSvr, 0, sizeof(msgKill.bSvr));
		msgKill.bSvr[nSvrType] = TRUE;
#ifdef _ADMINISTATOR
		m_aSock[nItem].SendGameMsg(&msgKill);
#endif
	}
}

void CMonitorDoc::ControlSendGift(int nItem , int nSvrType,bool bOpen)
{
	SureUnderControl();

	if (nSvrType == SVRTYPE_ACCOUNT )
	{
		CString strPrompt;
		if (bOpen == false)
		{
			strPrompt.Format("确定要关闭充值送礼吗" );
			if(IDYES == AfxMessageBox(strPrompt, MB_YESNO|MB_ICONQUESTION) )
			{
				AgentMsg_SendGift_Control msg;
				msg.nOpen = 0;
#ifdef _ADMINISTATOR
				m_aSock[nItem].SendGameMsg(&msg);
#endif
			}
		}
		else
		{
			strPrompt.Format("确定要开启充值送礼吗" );
			if(IDYES == AfxMessageBox(strPrompt, MB_YESNO|MB_ICONQUESTION) )
			{
				AgentMsg_SendGift_Control msg;
				msg.nOpen = 1;
#ifdef _ADMINISTATOR
				m_aSock[nItem].SendGameMsg(&msg);
#endif
			}
		}
	}
}

void CMonitorDoc::ShutdownInTime(int nItem)
{
	SureUnderControl();
	CString strPrompt;
	strPrompt.Format(IDS_AGENT_SURESHUTDOWN, m_aAgent[nItem].szAddr );
	if(!m_bConfirmOperate || IDYES == AfxMessageBox(strPrompt, MB_YESNO|MB_ICONQUESTION) )
	{
		//执行倒计时停机
		AgentMsg_ShutdownInTime msgShutdown;
		msgShutdown.nSeconds = m_nShutdownInSeconds;

#ifdef _ADMINISTATOR
		m_aSock[nItem].SendGameMsg(&msgShutdown);
#endif
		
		//关闭自动重启功能
		AgentMsg_Setting msgSetting;
		msgSetting.bRestore = FALSE;
		msgSetting.setting.abKeepServer[SVRTYPE_ACCOUNT] = false;	
		msgSetting.setting.abKeepServer[SVRTYPE_GROUP] = false;
		msgSetting.setting.abKeepServer[SVRTYPE_GAME] = false;
		msgSetting.setting.abKeepServer[SVRTYPE_GAME2] = false;
		msgSetting.setting.abKeepServer[SVRTYPE_GAME3] = false;
		msgSetting.setting.abKeepServer[SVRTYPE_GAME4] = false;
		msgSetting.setting.nCheckInterval = m_aAgent[nItem].setting.nCheckInterval;
		msgSetting.setting.nSysInfoInterval = m_aAgent[nItem].setting.nSysInfoInterval;

#ifdef _ADMINISTATOR
		m_aSock[nItem].SendGameMsg(&msgSetting);
#endif
	}
}


void CMonitorDoc::AskServerState()
{
	AgentMsg_Stat msgAsk;
	for(int i = 0; i < m_aAgent.size(); i++)
	{
		m_aSock[i].SendGameMsg(&msgAsk);
	}
}
void CMonitorDoc::AskSystemState()
{
	AgentMsg_SysInfo msgAsk;
	for(int i = 0; i < m_aAgent.size(); i++)
	{
		m_aSock[i].SendGameMsg(&msgAsk);
	}
}

void CMonitorDoc::ConnectAgent(int nItem)
{
	m_aSock[nItem].Reconnect();
}

void CMonitorDoc::DisconnectAgent(int nItem)
{
	m_aSock[nItem].Disconnect();
	m_aAgent[nItem].Clear();
	UpdateAllViews(NULL, HINT_UPDATE_ALL);
}

void CMonitorDoc::SetAgent(int nItem)
{
	SureUnderControl();

	CSetAgentDlg dlg;
	dlg.setting = m_aAgent[nItem].setting;
	if( IDOK == dlg.DoModal())
	{
		if(!dlg.bRestore)m_aAgent[nItem].setting = dlg.setting;

		AgentMsg_Setting msgSetting;
		msgSetting.bRestore = dlg.bRestore;
		msgSetting.setting = dlg.setting;

#ifdef _ADMINISTATOR
		m_aSock[nItem].SendGameMsg(&msgSetting);
#endif		
	}
}

void CMonitorDoc::RestartAgent(int nItem)
{
	SureUnderControl();

	CString strPrompt;
	strPrompt.Format(IDS_AGENT_SURERESTART, m_aAgent[nItem].szAddr );
	if( !m_bConfirmOperate || IDYES == AfxMessageBox(strPrompt, MB_YESNO|MB_ICONQUESTION) )
	{
		AgentMsg_Execute msgCmd;
		msgCmd.strCmdLine = "#RestartAgent";
#ifdef _ADMINISTATOR
		m_aSock[nItem].SendGameMsg(&msgCmd);
#endif	
	}	
}

void CMonitorDoc::Execute(int nItem, LPCTSTR lpszCmd)
{
	SureUnderControl();

	AgentMsg_Execute msgCmd;
	msgCmd.strCmdLine = lpszCmd;
#ifdef _ADMINISTATOR
	m_aSock[nItem].SendGameMsg(&msgCmd);
#endif	
}

void CMonitorDoc::Announce(int nItem, LPCTSTR lpszAnnounce)
{
	SureUnderControl();

	{
		AgentMsg_Announce msgAnnounce;
		msgAnnounce.szContent = lpszAnnounce;
		//strcpy( msgAnnounce.szContent, lpszAnnounce);
#ifdef _ADMINISTATOR
		m_aSock[nItem].SendGameMsg(&msgAnnounce);
#endif
	}
}

void CMonitorDoc::OnServerStateUpdateSpeedHigh()
{
	m_nServerStateUpdateSpeed = SPEED_High;
}
void CMonitorDoc::OnServerStateUpdateSpeedNormal()
{
	m_nServerStateUpdateSpeed = SPEED_Normal;
}
void CMonitorDoc::OnServerStateUpdateSpeedPassive()
{
	m_nServerStateUpdateSpeed = SPEED_Passive;
}
void CMonitorDoc::OnSystemStateUpdateSpeedHigh()
{
	m_nSystemStateUpdateSpeed = SPEED_High;
}
void CMonitorDoc::OnSystemStateUpdateSpeedNormal()
{
	m_nSystemStateUpdateSpeed = SPEED_Normal;
}
void CMonitorDoc::OnSystemStateUpdateSpeedPassive()
{
	m_nSystemStateUpdateSpeed = SPEED_Passive;
}

void CMonitorDoc::OnUpdateServerStateUpdateSpeedHigh(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio( m_nServerStateUpdateSpeed == SPEED_High);
}
void CMonitorDoc::OnUpdateServerStateUpdateSpeedNormal(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio( m_nServerStateUpdateSpeed == SPEED_Normal);
}
void CMonitorDoc::OnUpdateServerStateUpdateSpeedPassive(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio( m_nServerStateUpdateSpeed == SPEED_Passive);
}
void CMonitorDoc::OnUpdateSystemStateUpdateSpeedHigh(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio( m_nSystemStateUpdateSpeed == SPEED_High);
}
void CMonitorDoc::OnUpdateSystemStateUpdateSpeedNormal(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio( m_nSystemStateUpdateSpeed == SPEED_Normal);
}
void CMonitorDoc::OnUpdateSystemStateUpdateSpeedPassive(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio( m_nSystemStateUpdateSpeed == SPEED_Passive);
}
void CMonitorDoc::OnOptionBeeponerror()
{
	m_bBeepOnError = !m_bBeepOnError;
}

void CMonitorDoc::OnUpdateOptionBeeponerror(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bBeepOnError);
}

BOOL CMonitorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码


	return Initialize(lpszPathName);

	return TRUE;
}

void CMonitorDoc::BeepOnError()
{
	BOOL bError = FALSE;
	for(int i = 0; i < m_aAgent.size(); i++)
	{
		for(int k = 0; k < SVRTYPE_COUNT; k++)
		if( m_aAgent[i].stat[k].nStat == 0 )
		{
			bError = TRUE;
		}
	}

	if(bError)
	{
		if(m_bBeepOnError)
		{
			Beep(4000, 300);
			Beep(5000, 300);

			MessageBeep(MB_ICONEXCLAMATION);//MB_ICONASTERISK);
		}

		CWnd* pWnd = AfxGetMainWnd();
		//if( pWnd->GetSafeHwnd() != GetForegroundWindow())
		{
			pWnd->FlashWindow(TRUE);
		}
	}
}

void CMonitorDoc::RecordOnlineCount()
{
	HANDLE hFile = CreateFile(m_strOnlineCountFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(hFile, 0, NULL, FILE_END);
		SYSTEMTIME now;
		GetLocalTime(&now);
		char szBuff[2048];
		char *p = szBuff;
		sprintf(p, "%02d-%02d %02d:%02d", now.wMonth, now.wDay, now.wHour, now.wMinute);
		p += strlen(p);
		for(int i = 0; i < m_aAgent.size(); i++)
		{
			sprintf(p, " %4d",m_aAgent[i].stat[SVRTYPE_GAME].nCltCnt);
			p += strlen(p);

			sprintf(p, " %4d",m_aAgent[i].stat[SVRTYPE_GAME2].nCltCnt);
			p += strlen(p);

			sprintf(p, " %4d",m_aAgent[i].stat[SVRTYPE_GAME3].nCltCnt);
			p += strlen(p);

			sprintf(p, " %4d",m_aAgent[i].stat[SVRTYPE_GAME4].nCltCnt);
			p += strlen(p);
		}
		strcat(p,"\r\n");
		DWORD dwBytes;
		WriteFile( hFile, szBuff, strlen(szBuff), &dwBytes, NULL);
		CloseHandle(hFile);
	}
}


void CMonitorDoc::OnViewOnlinecount()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(SW_SHOW);
	}
}


void CMonitorDoc::OnShutdown10()
{
	m_nShutdownInSeconds = MIN10;
}

void CMonitorDoc::OnUpdateShutdown10(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio( m_nShutdownInSeconds == MIN10);
}

void CMonitorDoc::OnShutdown15()
{
	m_nShutdownInSeconds = MIN15;
}

void CMonitorDoc::OnUpdateShutdown15(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio( m_nShutdownInSeconds == MIN15);
}

void CMonitorDoc::OnShutdown30()
{
	m_nShutdownInSeconds = MIN30;
}

void CMonitorDoc::OnUpdateShutdown30(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio( m_nShutdownInSeconds == MIN30);
}

void CMonitorDoc::OnShutdown60()
{
	m_nShutdownInSeconds = MIN60;
}

void CMonitorDoc::OnUpdateShutdown60(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio( m_nShutdownInSeconds == MIN60);
}

void CMonitorDoc::OnComfirm()
{
	m_bConfirmOperate = !m_bConfirmOperate;
}

void CMonitorDoc::OnUpdateComfirm(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bConfirmOperate);
}

void CMonitorDoc::OnShutdownAll()
{
	CString strPrompt = "确定要对全部服务器执行倒计时停机吗？";
	if( IDYES == AfxMessageBox(strPrompt, MB_YESNO|MB_ICONQUESTION))
	{
		//执行倒计时停机
		AgentMsg_ShutdownInTime msgShutdown;
		msgShutdown.nSeconds = m_nShutdownInSeconds;

		AgentMsg_Setting msgSetting;
		msgSetting.bRestore = FALSE;
		msgSetting.setting.abKeepServer[SVRTYPE_ACCOUNT] = false;	
		msgSetting.setting.abKeepServer[SVRTYPE_GROUP] = false;
		msgSetting.setting.abKeepServer[SVRTYPE_GAME] = false;
		msgSetting.setting.abKeepServer[SVRTYPE_GAME2] = false;
		msgSetting.setting.abKeepServer[SVRTYPE_GAME3] = false;
		msgSetting.setting.abKeepServer[SVRTYPE_GAME4] = false;
		for(int i = 0; i < m_aAgent.size(); i++)
		{
			msgSetting.setting.nCheckInterval = m_aAgent[i].setting.nCheckInterval;
			msgSetting.setting.nSysInfoInterval = m_aAgent[i].setting.nSysInfoInterval;
#ifdef _ADMINISTATOR
			m_aSock[i].SendGameMsg(&msgSetting);
			m_aSock[i].SendGameMsg(&msgShutdown);
#endif	
		}
	}
}

void CMonitorDoc::OnRestoreAll()
{
	CString strPrompt = "确定要恢复全部服务器吗？";
	if( IDYES == AfxMessageBox(strPrompt, MB_YESNO|MB_ICONQUESTION))
	{
		AgentMsg_Setting msgSetting;
		msgSetting.bRestore = TRUE;
#ifdef _ADMINISTATOR
		for(int i = 0; i < m_aAgent.size(); i++)
		{
			m_aSock[i].SendGameMsg(&msgSetting);
		}
#endif
	}
}

void CMonitorDoc::ClacTotalPlayer()
{
	g_nTotalPlayer = 0;
	for(int i = 0; i < m_aAgent.size(); i++)
	{
		g_nTotalPlayer += m_aAgent[i].stat[SVRTYPE_GAME].nCltCnt;
		g_nTotalPlayer += m_aAgent[i].stat[SVRTYPE_GAME2].nCltCnt;
		g_nTotalPlayer += m_aAgent[i].stat[SVRTYPE_GAME3].nCltCnt;
		g_nTotalPlayer += m_aAgent[i].stat[SVRTYPE_GAME4].nCltCnt;
	}
	CString strTotal;
	strTotal.Format("Total: %d", g_nTotalPlayer);
	CMonitorMainFrame* pFrame = (CMonitorMainFrame*)AfxGetMainWnd();
	if(pFrame)
		pFrame->UpdateStatusPaneTotal(strTotal);
}

void CMonitorDoc::StopKeep(int nItem)
{
	AgentMsg_Setting msgSetting;
	msgSetting.bRestore = FALSE;
	msgSetting.setting.abKeepServer[SVRTYPE_ACCOUNT] = false;	
	msgSetting.setting.abKeepServer[SVRTYPE_GROUP] = false;
	msgSetting.setting.abKeepServer[SVRTYPE_GAME] = false;
	msgSetting.setting.abKeepServer[SVRTYPE_GAME2] = false;
	msgSetting.setting.abKeepServer[SVRTYPE_GAME3] = false;
	msgSetting.setting.abKeepServer[SVRTYPE_GAME4] = false;
	msgSetting.setting.nCheckInterval = m_aAgent[nItem].setting.nCheckInterval;
	msgSetting.setting.nSysInfoInterval = m_aAgent[nItem].setting.nSysInfoInterval;
#ifdef _ADMINISTATOR
	m_aSock[nItem].SendGameMsg(&msgSetting);
#endif
}

void CMonitorDoc::RestoreKeep(int nItem)
{
	AgentMsg_Setting msgSetting;
	msgSetting.bRestore = TRUE;
#ifdef _ADMINISTATOR
	m_aSock[nItem].SendGameMsg(&msgSetting);
#endif
}