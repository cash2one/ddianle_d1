// MonitorMainFrm.cpp : CMonitorMainFrame 类的实现
//

#include "stdafx.h"
#include "Monitor.h"

#include "MonitorMainFrm.h"
#include "MonitorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMonitorMainFrame

IMPLEMENT_DYNCREATE(CMonitorMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMonitorMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_MESSAGE(UM_GAMEMSG, OnNetworkMsg)
	ON_UPDATE_COMMAND_UI ( ID_INDICATOR_TOTAL, OnUpdatePane )
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_TOTAL,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMonitorMainFrame 构造/析构

CMonitorMainFrame::CMonitorMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMonitorMainFrame::~CMonitorMainFrame()
{
}


int CMonitorMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	UINT nID, nStyle; int cxWidth;
	m_wndStatusBar.GetPaneInfo(1, nID, nStyle, cxWidth);
	m_wndStatusBar.SetPaneInfo(1, nID, nStyle, 120);

	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	SetTimer(IDEvent1, 1000, NULL);
	SetTimer(IDEvent2, 2000, NULL);
	SetTimer(IDEvent60, 60000, NULL);
	SetTimer(IDEventBeep, 5000, NULL);

	return 0;
}

BOOL CMonitorMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	return m_wndSplitter.Create(this,
		2, 2,               // TODO: 调整行号和列号
		CSize(10, 10),      // TODO: 调整最小窗格大小
		pContext);
}

BOOL CMonitorMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}


// CMonitorMainFrame 诊断

#ifdef _DEBUG
void CMonitorMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMonitorMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMonitorMainFrame 消息处理程序


void CMonitorMainFrame::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMonitorDoc* pDoc = (CMonitorDoc*)GetActiveDocument();
	if(pDoc)pDoc->OnTimer(nIDEvent);

	CFrameWnd::OnTimer(nIDEvent);
}

LRESULT CMonitorMainFrame::OnNetworkMsg(WPARAM wParam, LPARAM lParam)
{
	CMonitorDoc* pDoc = (CMonitorDoc*)GetActiveDocument();
	if(pDoc)pDoc->OnNetworkMsg();
	return 0;
}