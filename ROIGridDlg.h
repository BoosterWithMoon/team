#pragma once
#include "afxwin.h"
#include "GridCtrl.h"
#include "msflexgrid1.h"

// CROIGridDlg ��ȭ �����Դϴ�.

class CROIGridDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CROIGridDlg)

public:
	CPyroSoftMDoc* pDoc;

	CGridCtrl m_Grid;


	CROIGridDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.

	virtual ~CROIGridDlg();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATUS_INF };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	void SetThrechold();

	template <typename T>
	T ModifiedVal(T object, T a, T b);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedRefInf();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBnClickedNmsTrace();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	CMsflexgrid1 m_ROI_GridView;
	int controlWidth;

	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
};
