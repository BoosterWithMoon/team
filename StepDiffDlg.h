#pragma once
#include "afxwin.h"


// CStepDiffDlg ��ȭ �����Դϴ�.

class CStepDiffDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStepDiffDlg)

public:
	CPyroSoftMDoc* pDoc;

	CEdit m_upStep_diff;
	CEdit m_middleStep_diff;
	CEdit m_downStep_diff;

	CStepDiffDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CStepDiffDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STEP_DIFF };
#endif

protected:
	void PrintStepDiff();
	void CheckEmptyValue(CString str, int* checkCount);
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
