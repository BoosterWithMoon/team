// MinTapDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PyroSoftM.h"
#include "MinTapDlg.h"
#include "afxdialogex.h"
#include "teeeditor.h"


// MinTapDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMinTapDlg, CDialogEx)

CMinTapDlg::CMinTapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MIN_TAB_DIALOG, pParent)
{

}

CMinTapDlg::~CMinTapDlg()
{
}

void CMinTapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART1, m_Spread_Chart);
	DDX_Control(pDX, IDC_SPREAD_STATIC, spread_static);
	DDX_Control(pDX, IDC_SPREAD_EDIT, spread_edit);
}


BEGIN_MESSAGE_MAP(CMinTapDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// MinTapDlg �޽��� ó�����Դϴ�.
BEGIN_EVENTSINK_MAP(CMinTapDlg, CDialogEx)
	ON_EVENT(CMinTapDlg, IDC_TCHART1, 8, CMinTapDlg::OnDblClickTchart1, VTS_NONE)
END_EVENTSINK_MAP()


void CMinTapDlg::OnDblClickTchart1()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (m_Spread_Chart.GetSeriesCount() > 0)
		m_Spread_Chart.ShowEditor(0); 
}

void CMinTapDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	if (m_Spread_Chart.m_hWnd == NULL) return;

	CRect rect;
	GetClientRect(&rect);

	m_Spread_Chart.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER);
}

HBRUSH CMinTapDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
	{
		if ( pWnd->GetDlgCtrlID() == IDC_SPREAD_EDIT && pDoc->m_bSpreadCondition)
			pDC->SetBkColor(RGB(0, 255, 0));
		else
			pDC->SetBkColor(RGB(255, 0, 0));
		break;
	}
	default:
		break;
	}

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


BOOL CMinTapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	spread_static_font.CreateFontA(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "����");
	spread_static.SetFont(&spread_static_font);

	spread_edit_font.CreateFontA(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "����");
	spread_edit.SetFont(&spread_edit_font);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
