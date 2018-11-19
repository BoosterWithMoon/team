// StatusDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PyroSoftM.h"
#include "ROIGridDlg.h"
#include "ZoneInfDlg.h"
#include "afxdialogex.h"


// CROIGridDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CROIGridDlg, CDialogEx)

CROIGridDlg::CROIGridDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STATUS_INF, pParent)
{

}

CROIGridDlg::~CROIGridDlg()
{
}

void CROIGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// ��ó�� ���� ���� ��

	DDX_Control(pDX, IDC_MSFLEXGRID1, m_ROI_GridView);
}


BEGIN_MESSAGE_MAP(CROIGridDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_DRAWITEM()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CROIGridDlg �޽��� ó�����Դϴ�.
HBRUSH CROIGridDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CROIGridDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_MSFLEXGRID1);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	controlWidth = rect.Width();

	return TRUE;
}


int CROIGridDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}

BOOL CROIGridDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CROIGridDlg::SetThrechold()
{

}

template <typename T>
T CROIGridDlg::ModifiedVal(T object, T a, T b)
{
	T result;

	if (object < a)
	{
		result = a;
	}
	else if (object > b)
	{
		result = b;
	}
	return result;
}

void CROIGridDlg::OnBnClickedRefInf()
{

}


void CROIGridDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{

}


void CROIGridDlg::OnBnClickedNmsTrace()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CROIGridDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	CRect r;
	//GetWindowRect(r);
	GetClientRect(r);

	if (GetDlgItem(IDC_MSFLEXGRID1)->GetSafeHwnd())
	{
		GetDlgItem(IDC_MSFLEXGRID1)->SetWindowPos(NULL, 0, 0, r.Width(), r.Height(), SWP_NOZORDER);
	}
	if (r.Width() < 590)
	{

	}
}

BOOL CROIGridDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint point) {
	if (zDelta <= 0) {
		DWORD dw = SB_LINERIGHT;
		m_ROI_GridView.SendMessage(WM_HSCROLL, dw, 0);
	}
	else {
		DWORD dw = SB_LINELEFT;
		m_ROI_GridView.SendMessage(WM_HSCROLL, dw, 0);
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, point);
}