
// ChildFrm.h : CChildFrame Ŭ������ �������̽�
//
#pragma once


class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Ư���Դϴ�.
public:
	CSplitterWnd		m_wndSplitter;
	CSplitterWnd		m_GFVSplitter;
	CSplitterWnd		m_ScaleSplitter;

// �۾��Դϴ�.
public:

// �������Դϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
	
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	bool bSplitterCreaged;
	afx_msg void OnClose();
};
