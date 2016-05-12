#pragma once


// CExecuteDlg 对话框

class CExecuteDlg : public CDialog
{
	DECLARE_DYNAMIC(CExecuteDlg)

public:
	CExecuteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExecuteDlg();

// 对话框数据
	enum { IDD = IDD_EXECUTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sCommandLine;
protected:
	virtual void OnOK();
};
