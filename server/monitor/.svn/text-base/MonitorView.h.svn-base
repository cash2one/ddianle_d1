// MonitorView.h : CMonitorView 类的接口
//
#include "treelistview.h"

#pragma once


class CMonitorView : public CTreeListView
{
protected: // 仅从序列化创建
	CMonitorView();
	DECLARE_DYNCREATE(CMonitorView)

// 属性
public:
	CMonitorDoc* GetDocument() const;
	CImageList m_ImageList;
	CImageList m_ImageListState;

// 操作
public:
	void UpdateList(int nIndex);

// 重写
	public:
//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	afx_msg void OnCloseserverAll();
	afx_msg void OnCloseserverLogicserver();
	afx_msg void OnCloseserverLogicserver2();
	afx_msg void OnCloseserverLogicserver3();
	afx_msg void OnCloseserverLogicserver4();
	afx_msg void OnCloseserverLoginserver();
	afx_msg void OnCloseserverWorldHost();
	afx_msg void OnStartserverAll();
	afx_msg void OnStartserverLogicserver();
	afx_msg void OnStartserverLogicserver2();
	afx_msg void OnStartserverLogicserver3();
	afx_msg void OnStartserverLogicserver4();
	afx_msg void OnStartserverLoginserver();
	afx_msg void OnStartserverWorldHost();
public:
	afx_msg void OnControlConnect();
	afx_msg void OnControlDisconnect();
	afx_msg void OnControlSetting();
	afx_msg void OnControlRestart();
	afx_msg void OnControlExecute();
	afx_msg void OnControlAnnounce();
	afx_msg void OnKillAccountserver();
	afx_msg void OnKillAll();
	afx_msg void OnKillGameserver();
	afx_msg void OnKillGameserver2();
	afx_msg void OnKillGameserver3();
	afx_msg void OnKillGameserver4();
	afx_msg void OnKillGroupserver();
	afx_msg void OnShutdownintime();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnAgentRestorekeep();
	afx_msg void OnAgentStopkeep();
public:
	afx_msg void OnSendgiftClose();
public:
	afx_msg void OnSendgiftOpen();
};

#ifndef _DEBUG  // MonitorView.cpp 的调试版本
inline CMonitorDoc* CMonitorView::GetDocument() const
   { return reinterpret_cast<CMonitorDoc*>(m_pDocument); }
#endif

