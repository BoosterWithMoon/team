#pragma once

//#include "PyroSoftMDoc.h"

#include "ResultDlg.h"

class CPyroSoftMDoc;

// CGlassDetectView ���Դϴ�.

class CGlassDetectView : public CView
{
	DECLARE_DYNCREATE(CGlassDetectView)

protected:
	CGlassDetectView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CGlassDetectView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	
	

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CResultDlg m_ResultDlg;

	void AdjustLayout();
	void UpdateResult();	
	
public:
	int m_Dlg_H;	

	CPyroSoftMDoc* pDoc;	
	
	// For Image 
	unsigned short	m_data_X, m_data_Y;
	float			m_zoomRate;	
		
	int		m_bmp_size_x;	// drawing size
	int		m_bmp_size_y;
	int		m_bmp_ofs_x;	// drawing ofs
	int		m_bmp_ofs_y;
	
	int		m_Margin;

	CDC		memDC;
	CBitmap bmpBuffer;
	
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

	void UpateMemDC();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	void InitROIData();
};


