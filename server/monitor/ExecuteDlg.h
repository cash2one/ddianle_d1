#pragma once


// CExecuteDlg �Ի���

class CExecuteDlg : public CDialog
{
	DECLARE_DYNAMIC(CExecuteDlg)

public:
	CExecuteDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExecuteDlg();

// �Ի�������
	enum { IDD = IDD_EXECUTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_sCommandLine;
protected:
	virtual void OnOK();
};
