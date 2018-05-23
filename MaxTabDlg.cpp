// MaxTabDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PyroSoftM.h"
#include "MaxTabDlg.h"
#include "afxdialogex.h"
#include "teeeditor.h"


// MaxTabDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMaxTabDlg, CDialogEx)

CMaxTabDlg::CMaxTabDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAX_TAB_DIALOG, pParent)
{
}

CMaxTabDlg::~CMaxTabDlg()
{
}

void CMaxTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAX_TCHART, m_Max_Chart);
}


BEGIN_MESSAGE_MAP(CMaxTabDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// MaxTabDlg �޽��� ó�����Դϴ�.


BOOL CMaxTabDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
BEGIN_EVENTSINK_MAP(CMaxTabDlg, CDialogEx)
	ON_EVENT(CMaxTabDlg, IDC_MAX_TCHART, 8, CMaxTabDlg::OnDblClickMaxTchart, VTS_NONE)
END_EVENTSINK_MAP()


void CMaxTabDlg::OnDblClickMaxTchart()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if(m_Max_Chart.GetSeriesCount() > 0)
		m_Max_Chart.ShowEditor(0);
}

void CMaxTabDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	if (m_Max_Chart.m_hWnd == NULL) return;

	CRect rect;
	GetClientRect(&rect);

	m_Max_Chart.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER);
}