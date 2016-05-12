// MonitorMainFrm.h : CMonitorMainFrame 类的接口
//


#pragma once
class CMonitorMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMonitorMainFrame();
	DECLARE_DYNCREATE(CMonitorMainFrame)

// 属性
protected:
	CSplitterWnd m_wndSplitter;
public:

// 操作
public:
	void UpdateStatusPaneTotal(LPCTSTR lpszText)
	{
		m_wndStatusBar.SetPaneText(1, lpszText);
	}
	void OnUpdatePane(CCmdUI *pCmdUI){pCmdUI->Enable();};

// 重写
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMonitorMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	LRESULT OnNetworkMsg(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT nIDEvent);
};


