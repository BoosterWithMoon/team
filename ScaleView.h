#pragma once

#include "ScaleDlg.h"


// CScaleView ���Դϴ�.

class CScaleView : public CView
{
	DECLARE_DYNCREATE(CScaleView)

protected:
	CScaleView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.

	bool		m_scaleSizeFlag;
	int			m_scaleSizeCount;

	void			AdjustLayout();
	virtual ~CScaleView();

public:
	CPyroSoftMDoc*	pDoc;
	CScaleDlg		m_ScaleDlg;




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
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};


