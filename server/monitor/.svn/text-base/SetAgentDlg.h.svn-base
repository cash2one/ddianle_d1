#pragma once

#include "../datastructure/datastruct_agent.h"
#include "afxwin.h"
// CSetAgentDlg 对话框

class CSetAgentDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetAgentDlg)

public:
	CSetAgentDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetAgentDlg();

// 对话框数据
	enum { IDD = IDD_SETAGENT };

	AGENT_SETTING setting;
	BOOL bRestore;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nCheckInterval;
	int m_nSysInfoInterval;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedButton1();
	BOOL m_bKeepGameServer;
	BOOL m_bKeepGroupServer;
	BOOL m_bKeepAccountServer;

	BOOL m_bKeepGameServer2;
	BOOL m_bKeepGameServer3;
	BOOL m_bKeepGameServer4;
};
