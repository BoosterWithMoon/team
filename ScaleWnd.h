
#pragma once

#include "ScaleDlg.h"

/////////////////////////////////////////////////////////////////////////////
// COutputList â

class CScaleWnd : public CDockablePane
{
// �����Դϴ�.
public:
	CScaleWnd();


// Ư���Դϴ�.
protected:



// �����Դϴ�.
public:
	virtual ~CScaleWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	CScaleDlg m_ScaleDlg;

	void AdjustLayout();


};

