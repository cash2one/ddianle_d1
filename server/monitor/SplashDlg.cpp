// SplashDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Monitor.h"
#include "SplashDlg.h"
#include ".\splashdlg.h"


// CSplashDlg �Ի���

IMPLEMENT_DYNAMIC(CSplashDlg, CDialog)
CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashDlg::IDD, pParent)
{
}

CSplashDlg::~CSplashDlg()
{
}

void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSplashDlg, CDialog)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CSplashDlg ��Ϣ�������

LRESULT CSplashDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return HTCAPTION;

	//return CDialog::OnNcHitTest(point);
}

BOOL CSplashDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
