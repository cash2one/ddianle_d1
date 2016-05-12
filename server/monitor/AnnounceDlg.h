#pragma once
#include "afxwin.h"


// CAnnounceDlg 对话框

class CAnnounceDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnnounceDlg)

public:
	CAnnounceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnnounceDlg();

// 对话框数据
	enum { IDD = IDD_ANNOUNCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sAnnounce;
	CComboBox m_cmb;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
