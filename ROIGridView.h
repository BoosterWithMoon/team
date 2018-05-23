#pragma once

#include "ROIGridDlg.h"
#include "ZoneInfDlg.h"

// CROIGridView ���Դϴ�.

class CROIGridView : public CView
{
	DECLARE_DYNCREATE(CROIGridView)

protected:
	CROIGridView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CROIGridView();

	void UpdateStatusData();


public:
	CPyroSoftMDoc* pDoc;
	CROIGridDlg	m_ROIGridDlg;
	CRect m_rectCurHist;


	void AdjustLayout();
	void InitStatusData();
	void SetFont();

	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	

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
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void UpdateGridControlSet();
};


