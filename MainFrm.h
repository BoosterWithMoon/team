
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#include "ScaleWnd.h"
#include "LogoWnd.h"
#include "PropertiesWnd.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void	SetStatusText(const char* s);

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CMFCMenuBar       m_wndMenuBar;
	
	CMFCToolBar			m_StdToolBar;
	CMFCToolBar			m_DataAcqToolBar;
	CMFCToolBar			m_DataPalyToolBar;
	CMFCToolBar			m_ROIREFToolBar;
	CMFCToolBar			m_FocusToolBar;	

	CMFCToolBar*		m_pUserToolbar;

	CMFCStatusBar		m_wndStatusBar;
	CMFCToolBarImages	m_UserImages;
	//CScaleWnd			m_wndScale;
	CLogoWnd			m_wndLogo;
	
public:
	CPropertiesWnd		m_wndProperties;

	CSliderCtrl			m_DataPlaySlider;
	bool				m_bNoNotify;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LONG OnDDAQDataMsg(UINT wParam, LONG lParam);		
	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);	
	
	CPyroSoftMDoc* GetDocument();
	
	void LocateToolBar(CMainFrame *pFW, CMFCToolBar *pBar1, CMFCToolBar *pBar2);


	CPyroSoftMDoc* m_pCurrentDoc;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnClose();
};


