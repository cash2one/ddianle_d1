#pragma once
#include "afxwin.h"


// CAnnounceDlg �Ի���

class CAnnounceDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnnounceDlg)

public:
	CAnnounceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAnnounceDlg();

// �Ի�������
	enum { IDD = IDD_ANNOUNCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_sAnnounce;
	CComboBox m_cmb;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
