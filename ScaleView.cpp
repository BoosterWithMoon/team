// ScaleView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ScaleView.h"
#include "PyroSoftM.h"


// CScaleView

IMPLEMENT_DYNCREATE(CScaleView, CView)

CScaleView::CScaleView()
{
	m_scaleSizeFlag = false;
	m_scaleSizeCount = 0;
}

CScaleView::~CScaleView()
{
}

BEGIN_MESSAGE_MAP(CScaleView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CScaleView �׸����Դϴ�.

void CScaleView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

}

// CScaleView �����Դϴ�.
#ifdef _DEBUG
void CScaleView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CScaleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CScaleView �޽��� ó�����Դϴ�.


int CScaleView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	if (m_ScaleDlg.GetSafeHwnd() == 0)
	{
		if (!m_ScaleDlg.Create(IDD_SCALE, this))
			return FALSE;

		m_ScaleDlg.ModifyStyle(0, WS_VISIBLE, 0);

		theApp.m_pScaleDlg = &m_ScaleDlg;

		AdjustLayout();
	}

	return 0;
}

void CScaleView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int dlgH = 0;

	//if (m_scaleSizeFlag && !theApp.m_resultDlgMove)
	//{
	//	theApp.nScaleDlgWidth = rectClient.Width() + theApp.nResultDlgWidth;
	//	theApp.m_resultDlgMove = true;
	//}

	//if (theApp.adjust_count == 5)
	//{
	//	m_scaleSizeCount++;
	//	if (m_scaleSizeCount == 2)
	//		m_scaleSizeFlag = true;
	//}

	int k = rectClient.left;

	m_ScaleDlg.SetWindowPos(NULL, rectClient.left,
		rectClient.top + dlgH,
		rectClient.Width(),
		rectClient.Height() - dlgH,
		SWP_NOACTIVATE | SWP_NOZORDER);
}


void CScaleView::OnSize(UINT nType, int cx, int cy)
{
	if (GetSafeHwnd() == NULL) return;

	CView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	AdjustLayout();
}


void CScaleView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}
