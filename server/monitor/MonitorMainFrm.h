// MonitorMainFrm.h : CMonitorMainFrame ��Ľӿ�
//


#pragma once
class CMonitorMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMonitorMainFrame();
	DECLARE_DYNCREATE(CMonitorMainFrame)

// ����
protected:
	CSplitterWnd m_wndSplitter;
public:

// ����
public:
	void UpdateStatusPaneTotal(LPCTSTR lpszText)
	{
		m_wndStatusBar.SetPaneText(1, lpszText);
	}
	void OnUpdatePane(CCmdUI *pCmdUI){pCmdUI->Enable();};

// ��д
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMonitorMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	LRESULT OnNetworkMsg(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT nIDEvent);
};


