
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "PyroSoftM.h"

#include "MainFrm.h"
#include "GlassFlowView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_WM_SETTINGCHANGE()
	ON_MESSAGE(DDAQ_WM_DATAMSG, OnDDAQDataMsg)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	m_pCurrentDoc = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

//	SetMenu(NULL);

	BOOL bNameValid;

	// ��� ����� �������̽� ��Ҹ� �׸��� �� ����ϴ� ���־� �����ڸ� �����մϴ�.
	//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
	
	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // ����� �� �ִ� �ٸ� ��Ÿ��...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // FALSE�� �����Ͽ� �� ���� �����ʿ� �ݱ� ���߸� ��ġ�մϴ�.
	mdiTabParams.m_bTabIcons = FALSE;    // TRUE�� �����Ͽ� MDI ���� ���� �������� Ȱ��ȭ�մϴ�.
	mdiTabParams.m_bAutoColor = TRUE;    // FALSE�� �����Ͽ� MDI ���� �ڵ� �� ������ ��Ȱ��ȭ�մϴ�.
	mdiTabParams.m_bDocumentMenu = TRUE; // �� ������ ������ �����ڸ��� ���� �޴��� Ȱ��ȭ�մϴ�.
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	
	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("�޴� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// �޴� ������ Ȱ��ȭ�ص� ��Ŀ���� �̵����� �ʰ� �մϴ�.
	CMFCPopupMenu::SetForceMenuFocus(FALSE);
	
	//**********************************************************************************************************************
	CString strToolBarName;

	if (!m_StdToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1, 1, 1, 1), IDR_MAINFRAME_256) ||
		!m_StdToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_StdToolBar.SetWindowText(strToolBarName);	
	
	///
	if (!m_DataAcqToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1, 1, 1, 1), IDR_DATAACQ_256) ||
		!m_DataAcqToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_DATAACQ_256 : IDR_DATAACQ_256))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_DATAACQ);
	ASSERT(bNameValid);
	m_DataAcqToolBar.SetWindowText(strToolBarName);

	///
	if (!m_DataPalyToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_DATAPLAYER_256) ||	
		!m_DataPalyToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_DATAPLAYER_256 : IDR_DATAPLAYER_256))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_DATAPLAY);
	ASSERT(bNameValid);
	m_DataPalyToolBar.SetWindowText(strToolBarName);
		
	m_DataPalyToolBar.SetButtonInfo(1, ID_DP_SLIDER, TBBS_SEPARATOR, 172);
	CRect itemRect;
	m_DataPalyToolBar.GetItemRect(1,&itemRect);

	itemRect.left+=4;
	itemRect.top+=2;
	itemRect.bottom-=4;

    if(!m_DataPlaySlider.Create(WS_CHILD | WS_VISIBLE | SBS_HORZ,itemRect,&m_DataPalyToolBar,ID_DP_SLIDER)) {
		TRACE0("�����̴��� ������ ���߽��ϴ�.\n");
		return -1;      
	}	
	
	m_DataPlaySlider.EnableWindow(0);	

	/// Focus *******************************************************************
	if (!m_FocusToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_TOOLBAR_FOCUS) ||
		!m_FocusToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_TOOLBAR_FOCUS : IDR_TOOLBAR_FOCUS))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_FOCUS);
	ASSERT(bNameValid);
	m_FocusToolBar.SetWindowText(strToolBarName);	


	/// ROI & REF *******************************************************************
	if (!m_ROIREFToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_ROI_REF) ||
		!m_ROIREFToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_ROI_REF : IDR_ROI_REF))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_ROIREF);
	ASSERT(bNameValid);
	m_ROIREFToolBar.SetWindowText(strToolBarName);
		
	//**********************************************************************************************************************

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	
	//m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// ����� ���� ���� ���� �۾��� ����մϴ�.
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ���� ���� �� �޴� ������ ��ŷ�� �� ���� �Ϸ��� �� �ټ� ���� �����Ͻʽÿ�.
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);	
	m_StdToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_DataAcqToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_DataPalyToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_FocusToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_ROIREFToolBar.EnableDocking(CBRS_ALIGN_ANY);
	
	EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&m_wndMenuBar);

	DockPane(&m_StdToolBar);		
	DockPane(&m_DataAcqToolBar);
	DockPane(&m_DataPalyToolBar);
	DockPane(&m_FocusToolBar);
	DockPane(&m_ROIREFToolBar);

	///m_wndMenuBar.ShowWindow(0);
	
	// Visual Studio 2005 ��Ÿ�� ��ŷ â ������ Ȱ��ȭ�մϴ�.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 ��Ÿ�� ��ŷ â �ڵ� ���� ������ Ȱ��ȭ�մϴ�.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ��ŷ â�� ����ϴ�.
	if (!CreateDockingWindows())
	{
		TRACE0("��ŷ â�� ������ ���߽��ϴ�.\n");
		return -1;
	}
		
	//m_wndScale.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_LEFT);
	m_wndLogo.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_TOP);
	m_wndProperties.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
	//m_wndScale.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);

	DockPane(&m_wndProperties);
	//DockPane(&m_wndScale);	
	DockPane(&m_wndLogo);
	
	// ���� â ���� ��ȭ ���ڸ� Ȱ��ȭ�մϴ�.
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ���� ���� �� ��ŷ â �޴� �ٲٱ⸦ Ȱ��ȭ�մϴ�.
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ����(<Alt> Ű�� ���� ä ����) ���� ���� ����� ������ Ȱ��ȭ�մϴ�.
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// ����� ���� ���� ���� �̹����� �ε��մϴ�.
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}
	
	// �޴� ���� ������ Ȱ��ȭ�մϴ�(���� �ֱٿ� ����� ���).
	// TODO: ������� �⺻ ����� �����Ͽ� �� Ǯ�ٿ� �޴��� �ϳ� �̻��� �⺻ ����� ���Խ�ŵ�ϴ�.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	//lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);	
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	//CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// â ���� ǥ���ٿ��� ���� �̸� �� ���� ���α׷� �̸��� ������ ��ȯ�մϴ�.
	// ���� �̸��� ����� �׸��� �Բ� ǥ�õǹǷ� �۾� ǥ������ ��ɼ��� �����˴ϴ�.
	//ModifyStyle(0, FWS_PREFIXTITLE);
	this->SetWindowText("RHK Thermal Imaging System");

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style &= FWS_ADDTOTITLE;
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;
	
	// �Ӽ� â�� ����ϴ�.
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 250, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("�Ӽ� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
	}

	// ������ â�� ����ϴ�.
	//CString strOutputWnd;
	//bNameValid = strOutputWnd.LoadString(IDS_SCALE_WND);
	//ASSERT(bNameValid);
	//if (!m_wndScale.Create(strOutputWnd, this, CRect(0, 0, 50, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("��� â�� ������ ���߽��ϴ�.\n");
	//	return FALSE; // ������ ���߽��ϴ�.
	//}
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_LOGO_WND);
	ASSERT(bNameValid);
	if (!m_wndLogo.Create(strOutputWnd, this, CRect(0, 0, 50, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("��� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
	}
	
	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndScale.SetIcon(hOutputBarIcon, FALSE);
	m_wndLogo.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CMDIFrameWndEx::OnShowWindow(bShow, nStatus);

	m_pUserToolbar->ResetAll();

	//m_wndScale.DockToFrameWindow(CBRS_ALIGN_LEFT);
	m_wndLogo.DockToFrameWindow(CBRS_ALIGN_TOP);
	m_wndProperties.DockToFrameWindow(CBRS_ALIGN_LEFT);
}


LONG CMainFrame::OnDDAQDataMsg(UINT wParam, LONG )
{
	CPyroSoftMDoc*	pDoc = theApp.GetDocumentFromDevice(wParam);
	if (pDoc)
		pDoc->OnNewDataReady();

	return 0;
}


void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* �޴��� �˻��մϴ�. */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	m_pUserToolbar = pUserToolbar;

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		
	return lres;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// �⺻ Ŭ������ ���� �۾��� �����մϴ�.

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// ��� ����� ���� ������ ����� ���� ���߸� Ȱ��ȭ�մϴ�.
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	//
}

/////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetStatusText(const char* s)
{
	m_wndStatusBar.SetPaneText(0, s);
}
/////////////////////////////////////////////////////////////////////////////

BOOL CMainFrame::DestroyWindow()
{
	theApp.EndDataMsgThread();

	return CMDIFrameWndEx::DestroyWindow();
}

CPyroSoftMDoc* CMainFrame::GetDocument()
{ 
	//*
	CPyroSoftMDoc *pDoc = NULL;

	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

	if(pFrame->GetSafeHwnd() == NULL) {	
		return (CPyroSoftMDoc*) pDoc;
	}

	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
	if(pChild->GetSafeHwnd() == NULL) {	
		return (CPyroSoftMDoc*) pDoc;
	}

	pDoc = (CPyroSoftMDoc *)pChild->GetActiveDocument();

	
	return (CPyroSoftMDoc*) pDoc;	
	//*/ 
}

BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{	
	bool ret;

	if (wParam==ID_DP_SLIDER && !m_bNoNotify)
	{ 
		NMHDR* pnm = (NMHDR*)lParam;		
		
		if(pnm->code==NM_CUSTOMDRAW) 
		{
			CPyroSoftMDoc *pDoc = m_pCurrentDoc;
			

			if(pDoc==NULL) 
				return CMDIFrameWndEx::OnNotify(wParam, lParam, pResult);
						

			if (pDoc->m_hIRDX_Doc == INVALID_HANDLE_VALUE)
				return CMDIFrameWndEx::OnNotify(wParam, lParam, pResult);
			
			unsigned long pos = m_DataPlaySlider.GetPos();

			if(pos!=pDoc->m_IdxDS && pDoc->m_NoDS>1 && pos < pDoc->m_NoDS) 
			{
				ret = theApp.DDAQ_IRDX_FILE_SetCurDataSet(pDoc->m_hIRDX_Doc, pos);

				if(ret) {
					pDoc->UpdateAllViews(NULL, 1);
					theApp.m_pPropertyWnd->UpdateDataSet();		
				}
				else {
					int aaaa = 100;
				}
			}
		}
	}
	
	return CMDIFrameWndEx::OnNotify(wParam, lParam, pResult);
}

LRESULT CMainFrame::OnPropertyChanged(WPARAM wparam, LPARAM lparam)
{
	::PostMessage(m_wndProperties.m_hWnd, AFX_WM_PROPERTY_CHANGED, (WPARAM) wparam, (LPARAM) lparam);

	return 0;
}

void CMainFrame::LocateToolBar(CMainFrame *pFW, CMFCToolBar *pBar1, CMFCToolBar *pBar2)
{ 
	int n=0;
	CRect rect;

	pFW->RecalcLayout();
	pBar1->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	
	/*
	dw=pBar1->GetBarStyle();

	n = (dw & CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw & CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw & CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw & CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	*/

	//pFW->DockControlBar(pBar2, n, &rect);
	pFW->DockPane(pBar2, CBRS_ALIGN_LEFT, &rect);
}




void CMainFrame::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if (IDNO == AfxMessageBox("���α׷��� ���� �Ͻðڽ��ϱ�?", MB_YESNO))
		return;
	else
	{
		if (theApp.m_bLoggingRunning)
		{
			CPyroSoftMDoc *pDoc = m_pCurrentDoc;

			pDoc->OnDeviceLoggingStop();

			Sleep(200);
		}
	}


	CMDIFrameWndEx::OnClose();
}
