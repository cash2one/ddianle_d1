// AnnounceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Monitor.h"
#include "AnnounceDlg.h"
#include ".\announcedlg.h"

static CString s_strLast;

// CAnnounceDlg 对话框

IMPLEMENT_DYNAMIC(CAnnounceDlg, CDialog)
CAnnounceDlg::CAnnounceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnnounceDlg::IDD, pParent)
	, m_sAnnounce(_T(""))
{
}

CAnnounceDlg::~CAnnounceDlg()
{
}

void CAnnounceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cmb);
	DDX_CBString(pDX, IDC_COMBO1, m_sAnnounce);
	DDV_MaxChars(pDX, m_sAnnounce, 120);
}


BEGIN_MESSAGE_MAP(CAnnounceDlg, CDialog)
END_MESSAGE_MAP()


// CAnnounceDlg 消息处理程序

BOOL CAnnounceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cmb.Clear();
	m_cmb.AddString("服务器将要重启，请立即退出！");
	m_cmb.AddString("服务器将要重启，请立即退出，以免造成不必要的回档！");
	m_cmb.AddString("服务器将要停机维护，请立即退出！");
	m_cmb.AddString("服务器将要停机维护，请立即退出，以免造成不必要的回档！");
	m_cmb.AddString("服务器将在1分钟内重新启动，请立即退出！");

	m_sAnnounce = s_strLast;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAnnounceDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData();
	s_strLast = m_sAnnounce;

	CDialog::OnOK();
}
