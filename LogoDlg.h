#pragma once

#include "resource.h"
#include "PyroSoftMDoc.h"
#include "afxwin.h"

// CLogoDlg ��ȭ �����Դϴ�.

class CLogoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLogoDlg)

public:
	CLogoDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLogoDlg();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGO };
#endif

protected:
	BOOL			MyLoadFromResource(CImage& img, LPCTSTR pName, LPCTSTR pType, HMODULE hInst);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int               dlgWidth;
	int               xPosForStatic;
	int               heightForStatic;

	CImage         samsungLogo;
	CImage         gtcLogo;
	CImage         programName;

	CString         ssLogoPath;
	CString         gtcLogoPath;
	CString         programNamePath;

	HRESULT      samsungHandle;
	HRESULT      gtcHandle;
	HRESULT      pnHandle;

	/*bool            isFontCreated;

	LOGFONT      logFont;*/

public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual BOOL OnInitDialog();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};