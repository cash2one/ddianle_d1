// DlgOnlineCount.cpp : 实现文件
//

#include "stdafx.h"
#include "Monitor.h"
#include "DlgOnlineCount.h"
#include ".\dlgonlinecount.h"


// CDlgOnlineCount 对话框

IMPLEMENT_DYNAMIC(CDlgOnlineCount, CDialog)
CDlgOnlineCount::CDlgOnlineCount(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOnlineCount::IDD, pParent)
{
}

CDlgOnlineCount::~CDlgOnlineCount()
{
}

void CDlgOnlineCount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_bar);
}


BEGIN_MESSAGE_MAP(CDlgOnlineCount, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

COLORREF aColor[] = 
{
	RGB(255,0,0),
	RGB(0,255,0),
	RGB(0,0,255),
	RGB(255,255,0),
	RGB(255,0,255),
	RGB(0,255,255),
};
void Draw(CDC *pDC, CRect &rc, int xStep, int yStep, string strBeginTime)
{
	int x0 = rc.left + 20;
	int y0 = rc.bottom - 20;

	//画坐标
	pDC->MoveTo(x0,y0);
	pDC->LineTo(x0,0);
	pDC->MoveTo(x0,y0);
	pDC->LineTo(rc.right, y0);

	for(unsigned int i = 0; i < m_aAgent.size(); i++)
	{
		CPen pen(PS_SOLID, 1, aColor[i]);
		CPen* pOldPen = pDC->SelectObject(&pen);

		int x = 0, y = 0;
		bool bDraw = false;
		list< ChartPoint >::iterator it;
		
		for(it = g_chart.listPoint.begin(); it != g_chart.listPoint.end(); ++it)
		{
			if(it->strTime.compare(strBeginTime) == 0)
			{
				bDraw = true;
				pDC->MoveTo(x0, y0 - it->aCount[i] / yStep);
			}
			if(bDraw)
			{
				pDC->LineTo( x0 + x * xStep, y0 - it->aCount[i] / yStep );
				x++;
			}
			if(x0 + x * xStep > rc.right )
				break;
		}
		pDC->SelectObject(pOldPen);
	}
}

// CDlgOnlineCount 消息处理程序

void CDlgOnlineCount::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	CRect rc;
	GetClientRect(rc);
	Draw(&dc, rc, 2, 2, "02-24 19:01");
}

void CDlgOnlineCount::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_bar.GetSafeHwnd() != NULL)
	{
		m_bar.SetWindowPos(NULL, 0, 0, cx, 30, SWP_NOZORDER);
		InvalidateRect(NULL);
	}
	// TODO: 在此处添加消息处理程序代码
}

void CDlgOnlineCount::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	g_chart.LoadFromFile("onlinecount.txt");
}

BOOL CDlgOnlineCount::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rc;
	GetClientRect(rc);
	m_bar.SetWindowPos(NULL, 0, 0, rc.Width(), 30, SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
