// Monitor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Monitor.h"
#include "MonitorMainFrm.h"
#include "splashdlg.h"

#include "MonitorDoc.h"
#include "MonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSplashDlg * g_pSplashDlg = NULL;
DWORD WINAPI SplashProc(LPVOID pParam)
{
	//CnsytApp* pApp = (CnsytApp*)pParam;
	if(g_pSplashDlg)g_pSplashDlg->ShowWindow(SW_SHOW);
	while(g_pSplashDlg)
	{
		g_pSplashDlg->UpdateWindow();
		Sleep(100);
	}
	return 0;
}


// CMonitorApp

BEGIN_MESSAGE_MAP(CMonitorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 基于文件的标准文档命令
	//ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CMonitorApp 构造

CMonitorApp::CMonitorApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMonitorApp 对象

CMonitorApp theApp;

CString g_sPath;

//vector< pair< CString, vector< CString > > > g_map;

vector< pair< CString, vector< ItemAddrAndIp > > > g_map;

vector< STAT_MON > m_aAgent;
vector< HTREEITEM >m_aItem;
int g_nTotalPlayer = 0;

Chart g_chart;

// CMonitorApp 初始化

BOOL CMonitorApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	g_sPath = theApp.m_pszHelpFilePath;
	//g_sPath.MakeLower();
	int n = g_sPath.ReverseFind('\\');
	g_sPath = g_sPath.Left(n+1);
	SetCurrentDirectory(g_sPath);

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	//SetRegistryKey(_T("the9.com"));
	LoadStdProfileSettings(0);  // 加载标准 INI 文件选项(包括 MRU)

	free((void*)theApp.m_pszProfileName);
	//Change the name of the .INI file.
	//The CWinApp destructor will free the memory.
	theApp.m_pszProfileName=_tcsdup( g_sPath + _T("monitor.ini"));

	BOOL bShowSplash = GetProfileInt("Splash", "Disabled", 0) == 0;
	if(bShowSplash)
	{
		g_pSplashDlg = new CSplashDlg;
		g_pSplashDlg->Create(IDD_SPLASH);
		g_pSplashDlg->ShowWindow(SW_SHOW);
		g_pSplashDlg->UpdateWindow();
		DWORD dwThreadID;
		HANDLE hThread = ::CreateThread(NULL, 0,SplashProc, this, 0, &dwThreadID);
	}


	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMonitorDoc),
		RUNTIME_CLASS(CMonitorMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CMonitorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// 仅当存在后缀时才调用 DragAcceptFiles，
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生


	if(g_pSplashDlg != NULL)
	{
		g_pSplashDlg->ShowWindow(SW_HIDE);
		delete g_pSplashDlg;
		g_pSplashDlg = NULL;
	}

	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CMonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CMonitorApp 消息处理程序

