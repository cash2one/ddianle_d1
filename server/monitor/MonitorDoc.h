// MonitorDoc.h :  CMonitorDoc 类的接口
//

#pragma once

#include "../socket/ClientSocketMgr.h"
#include "../socket/SlotPeer.h"
#include "../socket/GameMsg_Processor.h"
#pragma comment(lib, "socket.lib")
#include "dlgonlinecount.h"

#define HINT_UPDATE_ALL			1
#define HINT_UPDATE_ITEM		2

using namespace ZeroSocket;

class CMonitorDoc : public CDocument ,public GameMsg_Processor
{
protected: // 仅从序列化创建
	CMonitorDoc();
	DECLARE_DYNCREATE(CMonitorDoc)

// 属性
public:
	CClientSocketMgr *m_aSock;
	CSlotPeer * m_aSlotPeer;
	unsigned short m_nAgentPort;

#define SPEED_High		2
#define SPEED_Normal	1
#define SPEED_Passive	0
	int m_nServerStateUpdateSpeed;
	int m_nSystemStateUpdateSpeed;
#define MIN60			3600
#define MIN30			1800
#define MIN15			900
#define MIN10			600
	int m_nShutdownInSeconds;
	bool m_bBeepOnError;
	bool m_bConfirmOperate;
	CString m_strOnlineCountFile;//记录在线玩家数量的文件
	int m_nRecordInterval;//记录在线数量的时间间隔，单位　分

	CDlgOnlineCount *m_pDlg;
// 操作
public:
	void OnNetworkMsg();
	void ProcessNetMessage(GameMsg_Base * pMsg,int nSlot);
	void CloseServer(int nItem, int nSvrType);
	void StartServer(int nItem, int nSvrType);
	void KillServer(int nItem, int nSvrType);
	void AskServerState();
	void AskSystemState();
	void ConnectAgent(int nItem);
	void DisconnectAgent(int nItem);
	void SetAgent(int nItem);
	void RestartAgent(int nItem);
	void Execute(int nItem, LPCTSTR lpszCmd);
	void Announce(int nItem, LPCTSTR lpszAnnounce);
	void ShutdownInTime(int nItem);
	BOOL Initialize(LPCTSTR lpszIniFile);
	void BeepOnError();
	void RecordOnlineCount();
	void ClacTotalPlayer();
	void RestoreKeep(int nItem);
	void StopKeep(int nItem);

	void ControlSendGift(int nItem ,int nSvrType,bool bOpen);

private:
	void RegisterMessage();
	void RegisterMessageProcessor();

	void OnAgentStat(GameMsg_Base & msg ,CSlotPeer & slotPeer);
	void OnAgentSysInfo(GameMsg_Base & msg ,CSlotPeer & slotPeer);
	void OnAgentSetting(GameMsg_Base & msg ,CSlotPeer & slotPeer);

// 重写
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	void OnTimer(UINT nIDEvent);
	virtual ~CMonitorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnServerStateUpdateSpeedPassive();
	afx_msg void OnUpdateServerStateUpdateSpeedPassive(CCmdUI *pCmdUI);
	afx_msg void OnServerStateUpdateSpeedNormal();
	afx_msg void OnUpdateServerStateUpdateSpeedNormal(CCmdUI *pCmdUI);
	afx_msg void OnServerStateUpdateSpeedHigh();
	afx_msg void OnUpdateServerStateUpdateSpeedHigh(CCmdUI *pCmdUI);
	afx_msg void OnSystemStateUpdateSpeedPassive();
	afx_msg void OnUpdateSystemStateUpdateSpeedPassive(CCmdUI *pCmdUI);
	afx_msg void OnSystemStateUpdateSpeedNormal();
	afx_msg void OnUpdateSystemStateUpdateSpeedNormal(CCmdUI *pCmdUI);
	afx_msg void OnSystemStateUpdateSpeedHigh();
	afx_msg void OnUpdateSystemStateUpdateSpeedHigh(CCmdUI *pCmdUI);
	afx_msg void OnOptionBeeponerror();
	afx_msg void OnUpdateOptionBeeponerror(CCmdUI *pCmdUI);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnViewOnlinecount();
	afx_msg void OnAgentConnect();
	afx_msg void OnAgentDisconnect();
	afx_msg void OnShutdown10();
	afx_msg void OnUpdateShutdown10(CCmdUI *pCmdUI);
	afx_msg void OnShutdown15();
	afx_msg void OnUpdateShutdown15(CCmdUI *pCmdUI);
	afx_msg void OnShutdown30();
	afx_msg void OnUpdateShutdown30(CCmdUI *pCmdUI);
	afx_msg void OnShutdown60();
	afx_msg void OnUpdateShutdown60(CCmdUI *pCmdUI);
	afx_msg void OnComfirm();
	afx_msg void OnUpdateComfirm(CCmdUI *pCmdUI);
	afx_msg void OnShutdownAll();
	afx_msg void OnRestoreAll();
};


