#pragma once

#include "Resource.h"

#include "PyroSoftMDoc.h"
#include "afxwin.h"

// CScaleDlg ��ȭ �����Դϴ�.

class CScaleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScaleDlg)

public: 
	CPyroSoftMDoc* pDoc;
	CPyroSoftMDoc* GetDocument();

public:
	CScaleDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CScaleDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SCALE };

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		
};
