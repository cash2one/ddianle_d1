// ExecuteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Monitor.h"
#include "ExecuteDlg.h"
#include ".\executedlg.h"


// CExecuteDlg 对话框
static CString s_strLastCmd;

IMPLEMENT_DYNAMIC(CExecuteDlg, CDialog)
CExecuteDlg::CExecuteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExecuteDlg::IDD, pParent)
	, m_sCommandLine(_T(""))
{
	m_sCommandLine = s_strLastCmd;
}

CExecuteDlg::~CExecuteDlg()
{
}

void CExecuteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sCommandLine);
	DDV_MaxChars(pDX, m_sCommandLine, 250);
}


BEGIN_MESSAGE_MAP(CExecuteDlg, CDialog)
END_MESSAGE_MAP()


// CExecuteDlg 消息处理程序

void CExecuteDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData();
	s_strLastCmd = m_sCommandLine;

	CDialog::OnOK();
}
