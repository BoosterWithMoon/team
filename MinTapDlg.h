#pragma once
#include "tchart1.h"


// MinTapDlg ��ȭ �����Դϴ�.

class CMinTapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMinTapDlg)

public:
	CMinTapDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.

	CPyroSoftMDoc* pDoc;
	virtual ~CMinTapDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MIN_TAB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTChart m_Min_Chart;
	DECLARE_EVENTSINK_MAP()
	void OnDblClickTchart1();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
