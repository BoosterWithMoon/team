#pragma once
#include "tchart1.h"


// AvgTabDlg ��ȭ �����Դϴ�.

class CAvgTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAvgTabDlg)

public:
	CAvgTabDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CPyroSoftMDoc* pDoc;

	virtual ~CAvgTabDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AVG_TAB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTChart m_Avg_Chart;
	DECLARE_EVENTSINK_MAP()
	void OnDblClickTchart1();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
