// LogoWnd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"

#include "LogoWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "PyroSoftM.h"

// CLogoWnd

IMPLEMENT_DYNAMIC(CLogoWnd, CDockablePane)

CLogoWnd::CLogoWnd()
{

}

CLogoWnd::~CLogoWnd()
{
}


BEGIN_MESSAGE_MAP(CLogoWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CLogoWnd �޽��� ó�����Դϴ�.




int CLogoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (m_LogoDlg.GetSafeHwnd() == 0)
	{
		if (!m_LogoDlg.Create(IDD_LOGO, this))
			return FALSE;

		m_LogoDlg.ModifyStyle(0, WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0);

		theApp.m_pLogoDlg = &m_LogoDlg;

		m_LogoDlg.InvalidateRect(NULL, TRUE);

		AdjustLayout();
	}

	return 0;
}


void CLogoWnd::OnSize(UINT nType, int cx, int cy)
{
	if (GetSafeHwnd() == NULL) return;

	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout();
}

void CLogoWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_LogoDlg.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}
