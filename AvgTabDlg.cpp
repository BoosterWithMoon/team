// AvgTabDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PyroSoftM.h"
#include "AvgTabDlg.h"
#include "afxdialogex.h"
#include "teeeditor.h"


// AvgTabDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAvgTabDlg, CDialogEx)

CAvgTabDlg::CAvgTabDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_AVG_TAB_DIALOG, pParent)
{

}

CAvgTabDlg::~CAvgTabDlg()
{
}

void CAvgTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART1, m_Avg_Chart);
}


BEGIN_MESSAGE_MAP(CAvgTabDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// AvgTabDlg �޽��� ó�����Դϴ�.
BEGIN_EVENTSINK_MAP(CAvgTabDlg, CDialogEx)
	ON_EVENT(CAvgTabDlg, IDC_TCHART1, 8, CAvgTabDlg::OnDblClickTchart1, VTS_NONE)
END_EVENTSINK_MAP()


void CAvgTabDlg::OnDblClickTchart1()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (m_Avg_Chart.GetSeriesCount() > 0)
		m_Avg_Chart.ShowEditor(0);
}

void CAvgTabDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	if (m_Avg_Chart.m_hWnd == NULL) return;

	CRect rect;
	GetClientRect(&rect);

	m_Avg_Chart.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER);
}
