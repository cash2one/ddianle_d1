// SetAgentDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Monitor.h"
#include "SetAgentDlg.h"
#include ".\setagentdlg.h"


// CSetAgentDlg 对话框

IMPLEMENT_DYNAMIC(CSetAgentDlg, CDialog)
CSetAgentDlg::CSetAgentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetAgentDlg::IDD, pParent)
	, m_nCheckInterval(0)
	, m_nSysInfoInterval(0)
	, m_bKeepGameServer(FALSE)
	, m_bKeepGroupServer(FALSE)
	, m_bKeepAccountServer(FALSE)
	, m_bKeepGameServer2(FALSE)
	, m_bKeepGameServer3(FALSE)
	, m_bKeepGameServer4(FALSE)
{
}

CSetAgentDlg::~CSetAgentDlg()
{
}

void CSetAgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nCheckInterval);
	DDV_MinMaxInt(pDX, m_nCheckInterval, 0, 2000000000);
	DDX_Text(pDX, IDC_EDIT2, m_nSysInfoInterval);
	DDV_MinMaxInt(pDX, m_nSysInfoInterval, 0, 10000);
	DDX_Check(pDX, IDC_CHECK1, m_bKeepGameServer);
	DDX_Check(pDX, IDC_CHECK7, m_bKeepGameServer2);
	DDX_Check(pDX, IDC_CHECK8, m_bKeepGameServer3);
	DDX_Check(pDX, IDC_CHECK9, m_bKeepGameServer4);
	DDX_Check(pDX, IDC_CHECK2, m_bKeepGroupServer);
	DDX_Check(pDX, IDC_CHECK3, m_bKeepAccountServer);
}


BEGIN_MESSAGE_MAP(CSetAgentDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CSetAgentDlg 消息处理程序

BOOL CSetAgentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	bRestore = FALSE;
	m_nCheckInterval = setting.nCheckInterval;
	m_nSysInfoInterval = setting.nSysInfoInterval;
	m_bKeepGameServer		= (setting.abKeepServer[SVRTYPE_GAME])?1:0;
	m_bKeepGroupServer		= (setting.abKeepServer[SVRTYPE_GROUP])?1:0;
	m_bKeepAccountServer	= (setting.abKeepServer[SVRTYPE_ACCOUNT])?1:0;
	m_bKeepGameServer2		= (setting.abKeepServer[SVRTYPE_GAME2])?1:0;
	m_bKeepGameServer3		= (setting.abKeepServer[SVRTYPE_GAME3])?1:0;
	m_bKeepGameServer4		= (setting.abKeepServer[SVRTYPE_GAME4])?1:0;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSetAgentDlg::OnOK()
{
	UpdateData();
	setting.nCheckInterval = m_nCheckInterval;
	setting.nSysInfoInterval = m_nSysInfoInterval;
	setting.abKeepServer[SVRTYPE_GAME] = (m_bKeepGameServer)?true:false;
	setting.abKeepServer[SVRTYPE_GROUP] = (m_bKeepGroupServer)?true:false;
	setting.abKeepServer[SVRTYPE_ACCOUNT] = (m_bKeepAccountServer)?true:false;

	setting.abKeepServer[SVRTYPE_GAME2] = (m_bKeepGameServer2)?true:false;
	setting.abKeepServer[SVRTYPE_GAME3] = (m_bKeepGameServer3)?true:false;
	setting.abKeepServer[SVRTYPE_GAME4] = (m_bKeepGameServer4)?true:false;

	CDialog::OnOK();
}

void CSetAgentDlg::OnBnClickedButton1()
{
	bRestore = TRUE;
	CDialog::OnOK();
}
