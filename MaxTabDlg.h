#pragma once

// Chart header
#include "tchart1.h"	/// ��Ʈ ����ž�
#include "tchart.h"
#include "fastlineseries.h"
#include "histogramfunction.h"
#include "histogramseries.h"
#include "horizhistogramseries.h"
#include "TeeChartDefines.h"
#include "series.h"
#include "axis.h"
#include "valuelist.h"
#include "valuelists.h"


// MaxTabDlg ��ȭ �����Դϴ�.

class CMaxTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMaxTabDlg)

public:
	CMaxTabDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.

	CPyroSoftMDoc* pDoc;
	virtual ~CMaxTabDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAX_TAB_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTChart m_Max_Chart;

	DECLARE_EVENTSINK_MAP()
	void OnDblClickMaxTchart();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
