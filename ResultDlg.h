#pragma once

#include "Resource.h"
#include "ZoneInfDlg.h"

#include "afxwin.h"
#include "afxcmn.h"
#include "PyroSoftMDoc.h"

// Tab Dialogs
#include "MaxTabDlg.h"
#include "MinTapDlg.h"
#include "AvgTabDlg.h"

#include <afxmt.h>

// CResultDlg ��ȭ �����Դϴ�.

class CPyroSoftMDoc;

class CResultDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CResultDlg)

public:
	CResultDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CResultDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_RESULT };
	
	CWnd*			m_pWndShow;

	CMaxTabDlg		m_MaxTabDlg;
	CMinTapDlg		m_MinTabDlg;
	CAvgTabDlg		m_AvgTabDlg;

protected:
	template <typename T>
	T CResultDlg::ModifiedVal(T object, T a, T b);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	void ChangeProperties(CPyroSoftMDoc* pDoc);
	COLORREF ColorRef(int series_idx);

	unsigned short sizeX, sizeY;

	DECLARE_MESSAGE_MAP()
public:
	CPyroSoftMDoc* pDoc;	



	// font ����
	bool	m_font_mdf;

	// �ּ� �µ� ��
	CEdit m_MinTemp;
	// �ִ� �µ� ��
	CEdit m_MaxTemp;
	// ��� �µ� ��
	CEdit m_AvgTemp;

	unsigned short		m_bmp_isize_x;	// internal size
	unsigned short		m_bmp_isize_y;

	void CheckROICount(int ROI_count, int cROI_count);
	
	CStatic m_Static_RDRef;
	CStatic m_Static_RNRate;
	CStatic m_Static_RDLA;
	CStatic m_Static_RDLNRate;

	CEdit m_ROI_Min;
	CEdit m_ROI_Max;
	CEdit m_ROI_Avg;

	CTabCtrl m_Result_Tap;

	CButton m_ZoneInf_btn;
	CButton m_Coefficient_btn;

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedZoneBtn();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCoefficientBtn();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_StepDiff_btn;
	afx_msg void OnBnClickedStepdifferenceBtn();
};
