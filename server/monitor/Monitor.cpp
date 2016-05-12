// Monitor.cpp : ����Ӧ�ó��������Ϊ��
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
	// �����ļ��ı�׼�ĵ�����
	//ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CMonitorApp ����

CMonitorApp::CMonitorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMonitorApp ����

CMonitorApp theApp;

CString g_sPath;

//vector< pair< CString, vector< CString > > > g_map;

vector< pair< CString, vector< ItemAddrAndIp > > > g_map;

vector< STAT_MON > m_aAgent;
vector< HTREEITEM >m_aItem;
int g_nTotalPlayer = 0;

Chart g_chart;

// CMonitorApp ��ʼ��

BOOL CMonitorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	g_sPath = theApp.m_pszHelpFilePath;
	//g_sPath.MakeLower();
	int n = g_sPath.ReverseFind('\\');
	g_sPath = g_sPath.Left(n+1);
	SetCurrentDirectory(g_sPath);

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("the9.com"));
	LoadStdProfileSettings(0);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

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


	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMonitorDoc),
		RUNTIME_CLASS(CMonitorMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CMonitorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	// �������ں�׺ʱ�ŵ��� DragAcceptFiles��
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����


	if(g_pSplashDlg != NULL)
	{
		g_pSplashDlg->ShowWindow(SW_HIDE);
		delete g_pSplashDlg;
		g_pSplashDlg = NULL;
	}

	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CMonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CMonitorApp ��Ϣ�������

