// MonitorView.cpp : CMonitorView 类的实现
//

#include "stdafx.h"
#include "Monitor.h"

#include "MonitorDoc.h"
#include "MonitorView.h"
#include ".\monitorview.h"
#include "ExecuteDlg.h"
#include "AnnounceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMonitorView

IMPLEMENT_DYNCREATE(CMonitorView, CTreeListView)

BEGIN_MESSAGE_MAP(CMonitorView, CTreeListView)
	ON_COMMAND(ID_CLOSESEVER_ALL, OnCloseserverAll)
	ON_COMMAND(ID_CLOSESEVER_LOGICSERVER, OnCloseserverLogicserver)
	ON_COMMAND(ID_CLOSESEVER_LOGICSERVER2, OnCloseserverLogicserver2)
	ON_COMMAND(ID_CLOSESEVER_LOGICSERVER3, OnCloseserverLogicserver3)
	ON_COMMAND(ID_CLOSESEVER_LOGICSERVER4, OnCloseserverLogicserver4)
	ON_COMMAND(ID_CLOSESEVER_LOGINSERVER, OnCloseserverLoginserver)
	ON_COMMAND(ID_CLOSESERVER_WORLDHOST, OnCloseserverWorldHost)
	ON_COMMAND(ID_STARTSERVER_ALL, OnStartserverAll)
	ON_COMMAND(ID_STARTSERVER_LOGICSERVER, OnStartserverLogicserver)
	ON_COMMAND(ID_STARTSERVER_LOGICSERVER2, OnStartserverLogicserver2)
	ON_COMMAND(ID_STARTSERVER_LOGICSERVER3, OnStartserverLogicserver3)
	ON_COMMAND(ID_STARTSERVER_LOGICSERVER4, OnStartserverLogicserver4)
	ON_COMMAND(ID_STARTSERVER_LOGINSERVER, OnStartserverLoginserver)
	ON_COMMAND(ID_STARTSERVER_WORLDHOST, OnStartserverWorldHost)
	ON_COMMAND(IDM_AGENT_CONNECT, OnControlConnect)
	ON_COMMAND(IDM_AGENT_DISCONNECT, OnControlDisconnect)
	ON_COMMAND(IDM_AGENT_SETTING, OnControlSetting)
	ON_COMMAND(IDM_AGENT_RESTART, OnControlRestart)
	ON_COMMAND(ID_CONTROL_EXECUTE, OnControlExecute)
	ON_COMMAND(ID_CONTROL_ANNOUNCE, OnControlAnnounce)
	ON_COMMAND(IDM_KILL_ACCOUNTSERVER, OnKillAccountserver)
	ON_COMMAND(IDM_KILL_ALL, OnKillAll)
	ON_COMMAND(IDM_KILL_GAMESERVER, OnKillGameserver)
	ON_COMMAND(IDM_KILL_GAMESERVER2, OnKillGameserver2)
	ON_COMMAND(IDM_KILL_GAMESERVER3, OnKillGameserver3)
	ON_COMMAND(IDM_KILL_GAMESERVER4, OnKillGameserver4)
	ON_COMMAND(IDM_KILL_GROUPSERVER, OnKillGroupserver)
	ON_COMMAND(IDM_SHUTDOWNINTIME, OnShutdownintime)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_AGENT_RestoreKeep, OnAgentRestorekeep)
	ON_COMMAND(ID_AGENT_StopKeep, OnAgentStopkeep)
	ON_COMMAND(ID_SENDGIFT_CLOSE, &CMonitorView::OnSendgiftClose)
	ON_COMMAND(ID_SENDGIFT_OPEN, &CMonitorView::OnSendgiftOpen)
END_MESSAGE_MAP()

// CMonitorView 构造/析构

CMonitorView::CMonitorView()
{
	// TODO: 在此处添加构造代码

}

CMonitorView::~CMonitorView()
{
}

//BOOL CMonitorView::PreCreateWindow(CREATESTRUCT& cs)
//{
//	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
//	// 样式
//	m_dwDefaultStyle |=  LVS_REPORT | LVS_SHOWSELALWAYS ;//| LVS_SINGLESEL ;
//
//	return CListView::PreCreateWindow(cs);
//}

void CMonitorView::OnInitialUpdate()
{
	CTreeListView::OnInitialUpdate();

	int nCol = 0;
	int nWidth = 90;
	int nWidth2 = 70;
	CNewTreeListCtrl& refTree = m_wndTreeList.m_tree;
	refTree.InsertColumn(nCol++, "IP", LVCFMT_LEFT, 160);
	refTree.InsertColumn(nCol++, "Agent", LVCFMT_LEFT, 50);
	refTree.InsertColumn(nCol++, "GameServer", LVCFMT_LEFT, nWidth);
	refTree.InsertColumn(nCol++, "GameServer2", LVCFMT_LEFT, nWidth);
	refTree.InsertColumn(nCol++, "GameServer3", LVCFMT_LEFT, nWidth);
	refTree.InsertColumn(nCol++, "GameServer4", LVCFMT_LEFT, nWidth);
	refTree.InsertColumn(nCol++, "GroupServer", LVCFMT_LEFT, nWidth);
	refTree.InsertColumn(nCol++, "AccountServer", LVCFMT_LEFT, nWidth);
	refTree.InsertColumn(nCol++, "%CPU(user)", LVCFMT_RIGHT, nWidth2);
	refTree.InsertColumn(nCol++, "%CPU(system)", LVCFMT_RIGHT, nWidth2);
	refTree.InsertColumn(nCol++, "%CPU(idle)", LVCFMT_RIGHT, nWidth2);
	refTree.InsertColumn(nCol++, "MemUsed/MemLeft", LVCFMT_RIGHT, 130);

	int nItem = 0;
	for(vector< pair< CString, vector< ItemAddrAndIp > > >::iterator itSec = g_map.begin(); itSec != g_map.end(); itSec ++)
	{
		HTREEITEM hTmp = refTree.InsertItem(itSec->first,0,0);
		refTree.SetItemData(hTmp, nItem++);
		for( vector< ItemAddrAndIp >::iterator itSvr = itSec->second.begin(); itSvr != itSec->second.end(); itSvr ++)
		{
			HTREEITEM hItem = NULL;
			//HTREEITEM hItem = refTree.InsertItem(*itSvr, 0, 0, hTmp);
			if ((*itSvr).nPort >0)
			{
				char strTemp[32];
				memset(strTemp,0,32);
				sprintf(strTemp,"%s:%d",(*itSvr).szAddr,(*itSvr).nPort);
				hItem = refTree.InsertItem(strTemp, 0, 0, hTmp);
			}
			else
				hItem = refTree.InsertItem((*itSvr).szAddr, 0, 0, hTmp);

			for(int i = 0; i < m_aAgent.size(); i++)
			{
				if( strcmp(m_aAgent[i].szAddr, (*itSvr).szAddr) == 0 && m_aAgent[i].nPort == (*itSvr).nPort)
				{
					m_aItem[i] = hItem;
					refTree.SetItemData(hItem, i);
					break;
				}
			}
		}
	}

//
//	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
//	// 从而可以用项填充 ListView。
//
//	CListCtrl &refList = GetListCtrl();
//	ListView_SetExtendedListViewStyle(refList.GetSafeHwnd(), LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
//
//	int nCol = 0;
//	int nWidth = 90;
//	refList.InsertColumn(nCol++, "IP", LVCFMT_LEFT, 100);
//	refList.InsertColumn(nCol++, "Agent", LVCFMT_LEFT, nWidth);
//	refList.InsertColumn(nCol++, "GameServer", LVCFMT_LEFT, nWidth);
//	refList.InsertColumn(nCol++, "GroupServer", LVCFMT_LEFT, nWidth);
//	refList.InsertColumn(nCol++, "AccountServer", LVCFMT_LEFT, nWidth);
//	refList.InsertColumn(nCol++, "GlobalServer", LVCFMT_LEFT, nWidth);
//	refList.InsertColumn(nCol++, "%CPU(user)", LVCFMT_RIGHT, nWidth);
//	refList.InsertColumn(nCol++, "%CPU(system)", LVCFMT_RIGHT, nWidth);
//	refList.InsertColumn(nCol++, "%CPU(idle)", LVCFMT_RIGHT, nWidth);
//	refList.InsertColumn(nCol++, "%Mem", LVCFMT_RIGHT, nWidth);
//
//	m_ImageList.Create(IDB_LIST_IMAGE, 9, 1, RGB(255,255,255));
//	refList.GetHeaderCtrl()->SetImageList(&m_ImageList);
//
//	m_ImageListState.Create(IDB_BITMAP2, 12, 1, RGB(255,255,255));
//	refList.SetImageList(&m_ImageListState, LVSIL_SMALL);
//
//	for(int i = 0; i < m_aAgent.size(); i++)
//	{
//		refList.InsertItem(i, m_aAgent[i].szAddr,1);
//	}
}


// CMonitorView 诊断

#ifdef _DEBUG
void CMonitorView::AssertValid() const
{
	CTreeListView::AssertValid();
}

void CMonitorView::Dump(CDumpContext& dc) const
{
	CTreeListView::Dump(dc);
}

CMonitorDoc* CMonitorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMonitorDoc)));
	return (CMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMonitorView 消息处理程序

void CMonitorView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* pHint)
{
	switch(lHint) 
	{
	case HINT_UPDATE_ALL:
		{
			for(int i = 0; i < m_aAgent.size(); i++)
			{
				UpdateList(i);
			}
		}
		break;
	case HINT_UPDATE_ITEM:
		{
			UpdateList((int)pHint);
		}
		break;
	default:
		break;
	}
}

void CMonitorView::UpdateList(int nIndex)
{
	CNewTreeListCtrl &refTree = m_wndTreeList.m_tree;
	int nCol = 1;
	int nSvrType = SVRTYPE_ACCOUNT;
	CString strItem;
	refTree.SetItemText(m_aItem[nIndex], nCol++, s_szStat[m_aAgent[nIndex].stat[SVRTYPE_AGENT].nStat]);

	strItem.Format("%s[%d]",s_szStat[m_aAgent[nIndex].stat[SVRTYPE_GAME].nStat], m_aAgent[nIndex].stat[SVRTYPE_GAME].nCltCnt);
	refTree.SetItemText(m_aItem[nIndex], nCol++, strItem);

	strItem.Format("%s[%d]",s_szStat[m_aAgent[nIndex].stat[SVRTYPE_GAME2].nStat], m_aAgent[nIndex].stat[SVRTYPE_GAME2].nCltCnt);
	refTree.SetItemText(m_aItem[nIndex], nCol++, strItem);

	strItem.Format("%s[%d]",s_szStat[m_aAgent[nIndex].stat[SVRTYPE_GAME3].nStat], m_aAgent[nIndex].stat[SVRTYPE_GAME3].nCltCnt);
	refTree.SetItemText(m_aItem[nIndex], nCol++, strItem);

	strItem.Format("%s[%d]",s_szStat[m_aAgent[nIndex].stat[SVRTYPE_GAME4].nStat], m_aAgent[nIndex].stat[SVRTYPE_GAME4].nCltCnt);
	refTree.SetItemText(m_aItem[nIndex], nCol++, strItem);

	strItem.Format("%s[%d]",s_szStat[m_aAgent[nIndex].stat[SVRTYPE_GROUP].nStat], m_aAgent[nIndex].stat[SVRTYPE_GROUP].nCltCnt);
	refTree.SetItemText(m_aItem[nIndex], nCol++, strItem);

	strItem.Format("%s[%d]",s_szStat[m_aAgent[nIndex].stat[SVRTYPE_ACCOUNT].nStat], m_aAgent[nIndex].stat[SVRTYPE_ACCOUNT].nCltCnt);
	refTree.SetItemText(m_aItem[nIndex], nCol++, strItem);


	strItem.Format("%3.1f", m_aAgent[nIndex].sysInfo.nCPU[0]/10.0f);
	refTree.SetItemText(m_aItem[nIndex], nCol++, strItem);
	strItem.Format("%3.1f", m_aAgent[nIndex].sysInfo.nCPU[1]/10.0f);
	refTree.SetItemText(m_aItem[nIndex], nCol++, strItem);
	strItem.Format("%3.1f", m_aAgent[nIndex].sysInfo.nCPU[2]/10.0f);
	refTree.SetItemText(m_aItem[nIndex], nCol++, strItem);
	strItem.Format("%dM/%dM", m_aAgent[nIndex].sysInfo.nMemUsed, m_aAgent[nIndex].sysInfo.nMemLeft);
	refTree.SetItemText(m_aItem[nIndex], nCol++, strItem);

	bool bError = false;
	for(int i = 0; i < SVRTYPE_COUNT; i++)
	{
		if(m_aAgent[nIndex].stat[i].nStat == 0)
		{
			bError = true;
			break;
		}
	}

#define RED		0
#define GREEN	1
#define YELLOW	2
	int nImage = RED;
	if(!bError)
	{
		if(m_aAgent[nIndex].sysInfo.nCPU[0]/10.0f > 40 && m_aAgent[nIndex].sysInfo.nCPU[2]/10.0f < 50)
			nImage = YELLOW;
		else if (m_aAgent[nIndex].stat[SVRTYPE_ACCOUNT].nCltCnt > 5000)
			nImage = YELLOW;
		else
			nImage = GREEN;
	}

	refTree.SetItemImage(m_aItem[nIndex], nImage, nImage);
	
	HTREEITEM hParent = refTree.GetParentItem(m_aItem[nIndex]);

	HTREEITEM hNextItem;
	HTREEITEM hChildItem = refTree.GetChildItem(hParent);
	int nImageParent = GREEN;
	int nImageSel;
	int nTotalCount = 0;
	while (hChildItem != NULL)
	{
		refTree.GetItemImage(hChildItem, nImage, nImageSel);
		if(nImageParent != RED && (nImage == YELLOW || nImage == RED) )
			nImageParent = nImage;
		DWORD nIndex = refTree.GetItemData(hChildItem);
        nTotalCount += m_aAgent[nIndex].stat[SVRTYPE_GAME].nCltCnt;
		nTotalCount += m_aAgent[nIndex].stat[SVRTYPE_GAME2].nCltCnt;
		nTotalCount += m_aAgent[nIndex].stat[SVRTYPE_GAME3].nCltCnt;
		nTotalCount += m_aAgent[nIndex].stat[SVRTYPE_GAME4].nCltCnt;

		hNextItem = refTree.GetNextItem(hChildItem, TVGN_NEXT);
		hChildItem = hNextItem;
	}
	refTree.SetItemImage(hParent, nImageParent, nImageParent);
	strItem.Format("%d", nTotalCount);
	refTree.SetItemText(hParent, 2, strItem);
}


//#define DoSomethingOnSelectedMachine(func) \
//	CListCtrl & refList = GetListCtrl();\
//	UINT uSelectedCount = refList.GetSelectedCount();\
//	UINT i;\
//	int  nItem = -1;\
//	if (uSelectedCount > 0)\
//	{\
//		for (i=0;i < uSelectedCount;i++)\
//		{\
//			nItem = refList.GetNextItem(nItem, LVNI_SELECTED);\
//			ASSERT(nItem != -1);\
//			GetDocument()->func;\
//		}\
//	}\
//	else\
//	{\
//		AfxMessageBox("No machine selected!");\
//	}

#define DoSomethingOnSelectedMachine(func) \
	CNewTreeListCtrl &refTree = m_wndTreeList.m_tree;\
	HTREEITEM hItem = refTree.GetSelectedItem();\
	if(hItem != NULL)\
	{\
		if(refTree.ItemHasChildren(hItem))\
		{\
			HTREEITEM hNextItem;\
			HTREEITEM hChildItem = refTree.GetChildItem(hItem);\
			while (hChildItem != NULL)\
			{\
				DWORD nItem = refTree.GetItemData(hChildItem);\
				GetDocument()->func;\
				hNextItem = refTree.GetNextItem(hChildItem, TVGN_NEXT);\
				hChildItem = hNextItem;\
			}\
		}\
		else\
		{\
			DWORD nItem = refTree.GetItemData(hItem);\
			GetDocument()->func;\
		}\
	}\
	else\
	{\
		AfxMessageBox("No machine selected!");\
	}

void CMonitorView::OnCloseserverAll()
{
	DoSomethingOnSelectedMachine(CloseServer(nItem, SVRTYPE_UNKNOWN));
}


void CMonitorView::OnCloseserverLogicserver()
{
	DoSomethingOnSelectedMachine(CloseServer(nItem, SVRTYPE_GAME));
}

void CMonitorView::OnCloseserverLogicserver2()
{
	DoSomethingOnSelectedMachine(CloseServer(nItem, SVRTYPE_GAME2));
}

void CMonitorView::OnCloseserverLogicserver3()
{
	DoSomethingOnSelectedMachine(CloseServer(nItem, SVRTYPE_GAME3));
}

void CMonitorView::OnCloseserverLogicserver4()
{
	DoSomethingOnSelectedMachine(CloseServer(nItem, SVRTYPE_GAME4));
}

void CMonitorView::OnCloseserverLoginserver()
{
	DoSomethingOnSelectedMachine(CloseServer(nItem, SVRTYPE_ACCOUNT));
}
void CMonitorView::OnCloseserverWorldHost()
{
	DoSomethingOnSelectedMachine(CloseServer(nItem, SVRTYPE_GROUP));
}

void CMonitorView::OnStartserverAll()
{
	DoSomethingOnSelectedMachine(StartServer(nItem, SVRTYPE_UNKNOWN));
}
void CMonitorView::OnStartserverLogicserver()
{
	DoSomethingOnSelectedMachine(StartServer(nItem, SVRTYPE_GAME));
}

void CMonitorView::OnStartserverLogicserver2()
{
	DoSomethingOnSelectedMachine(StartServer(nItem, SVRTYPE_GAME2));
}

void CMonitorView::OnStartserverLogicserver3()
{
	DoSomethingOnSelectedMachine(StartServer(nItem, SVRTYPE_GAME3));
}

void CMonitorView::OnStartserverLogicserver4()
{
	DoSomethingOnSelectedMachine(StartServer(nItem, SVRTYPE_GAME4));
}

void CMonitorView::OnStartserverLoginserver()
{
	DoSomethingOnSelectedMachine(StartServer(nItem, SVRTYPE_ACCOUNT));
}
void CMonitorView::OnStartserverWorldHost()
{
	DoSomethingOnSelectedMachine(StartServer(nItem, SVRTYPE_GROUP));
}

void CMonitorView::OnControlConnect()
{
	DoSomethingOnSelectedMachine(ConnectAgent(nItem));
}
void CMonitorView::OnControlDisconnect()
{
	DoSomethingOnSelectedMachine(DisconnectAgent(nItem));
}
void CMonitorView::OnControlSetting()
{
	DoSomethingOnSelectedMachine(SetAgent(nItem));
}
void CMonitorView::OnControlRestart()
{
	DoSomethingOnSelectedMachine(RestartAgent(nItem));
}
void CMonitorView::OnControlExecute()
{
	CExecuteDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		DoSomethingOnSelectedMachine(Execute(nItem, dlg.m_sCommandLine));
	}

}
void CMonitorView::OnControlAnnounce()
{
	CAnnounceDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		DoSomethingOnSelectedMachine(Announce(nItem, dlg.m_sAnnounce));
	}
}
void CMonitorView::OnKillAccountserver()
{
	DoSomethingOnSelectedMachine(KillServer(nItem, SVRTYPE_ACCOUNT));
}

void CMonitorView::OnKillAll()
{
	DoSomethingOnSelectedMachine(KillServer(nItem, SVRTYPE_UNKNOWN));
}

void CMonitorView::OnKillGameserver()
{
	DoSomethingOnSelectedMachine(KillServer(nItem, SVRTYPE_GAME));
}

void CMonitorView::OnKillGameserver2()
{
	DoSomethingOnSelectedMachine(KillServer(nItem, SVRTYPE_GAME2));
}

void CMonitorView::OnKillGameserver3()
{
	DoSomethingOnSelectedMachine(KillServer(nItem, SVRTYPE_GAME3));
}

void CMonitorView::OnKillGameserver4()
{
	DoSomethingOnSelectedMachine(KillServer(nItem, SVRTYPE_GAME4));
}

void CMonitorView::OnKillGroupserver()
{
	DoSomethingOnSelectedMachine(KillServer(nItem, SVRTYPE_GROUP));
}

void CMonitorView::OnShutdownintime()
{
	DoSomethingOnSelectedMachine(ShutdownInTime(nItem));
}

void CMonitorView::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
	POINT pt;
	GetCursorPos(&pt);
	HWND hwnd = this->m_hWnd;
	HMENU hmenu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MAINFRAME));
	HMENU hmnuPopup = GetSubMenu(hmenu, 3);
	SetMenuDefaultItem(hmnuPopup, IDOK, FALSE);
	::SetForegroundWindow(hwnd);
	TrackPopupMenu(hmnuPopup, TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
	::SetForegroundWindow(hwnd);
	DestroyMenu(hmnuPopup);
	DestroyMenu(hmenu);
}

void CMonitorView::OnAgentRestorekeep()
{
	DoSomethingOnSelectedMachine(RestoreKeep(nItem));
}

void CMonitorView::OnAgentStopkeep()
{
	DoSomethingOnSelectedMachine(StopKeep(nItem));
}

void CMonitorView::OnSendgiftClose()
{
	// TODO: Add your command handler code here

	DoSomethingOnSelectedMachine(ControlSendGift(nItem, SVRTYPE_ACCOUNT,false));
}

void CMonitorView::OnSendgiftOpen()
{
	// TODO: Add your command handler code here
	DoSomethingOnSelectedMachine(ControlSendGift(nItem, SVRTYPE_ACCOUNT,true));
}
